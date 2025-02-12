project(genvectorx)

cmake_minimum_required(VERSION 3.9)

find_package(ROOT COMPONENTS MathCore ROOTVecOps GenVector)
if(ROOT_FOUND)
  message(STATUS "ROOT library found")
  message(DEBUG "ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}")
  message(DEBUG "ROOT_LIBRARIES: ${ROOT_LIBRARIES}")
  message(DEBUG "ROOT_CXX_FLAGS: ${ROOT_CXX_FLAGS}")
  message(DEBUG "ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}")
else()
  message(STATUS "ROOT library not found")
endif()

include_directories(${CMAKE_SOURCE_DIR}/inc)

string(TOUPPER "${CMAKE_BUILD_TYPE}" _BUILD_TYPE_UPPER)

if(oneapi)
  set(_sycl_search_dirs ${SYCL_ROOT_DIR} /usr/lib /usr/local/lib
                        /opt/intel/oneapi/compiler/latest/linux)
  find_program(
    SYCL_COMPILER
    NAMES icpx clang++
    HINTS ${_sycl_search_dirs}
    PATH_SUFFIXES bin)
  find_path(
    SYCL_INCLUDE_DIR
    NAMES sycl/sycl.hpp
    HINTS ${_sycl_search_dirs}
    PATH_SUFFIXES include)
  find_path(
    SYCL_LIB_DIR
    NAMES libsycl.so
    HINTS ${_sycl_search_dirs}
    PATH_SUFFIXES lib)
  message(DEBUG "_sycl_search_dirs: ${_sycl_search_dirs}")
  message(DEBUG "SYCL_COMPILER: ${SYCL_COMPILER}")
  message(DEBUG "SYCL_INCLUDE_DIR: ${SYCL_INCLUDE_DIR}")
  message(DEBUG "SYCL_LIB_DIR: ${SYCL_LIB_DIR}")

  if(syclcuda)
    set(_fsycl_targets
        "-fsycl-targets=nvptx64-nvidia-cuda,spir64_x86_64,spir64 -Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=sm_${CMAKE_CUDA_ARCHITECTURES} --cuda-path=${CUDA_TOOLKIT_ROOT_DIR} "
    )
  elseif(syclamd)
    set(_fsycl_targets
        "-fsycl-targets=amdgcn-amd-amdhsa -Xsycl-target-backend=amdgcn-amd-amdhsa --offload-arch=${CMAKE_OFFLOAD_ARCHITECTURES} "
    )
  else()
    set(_fsycl_targets "-fsycl-targets=spir64_x86_64,spir64")
  endif()

  set(SYCL_COMPILER_FLAGS
      "-fPIC -fsycl -fsycl-unnamed-lambda -sycl-std=2020 ${_fsycl_targets} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${_BUILD_TYPE_UPPER}}"
  )
  message(DEBUG "SYCL compiler flags: ${SYCL_COMPILER_FLAGS}")
  separate_arguments(SYCL_COMPILER_FLAGS NATIVE_COMMAND ${SYCL_COMPILER_FLAGS})

  if(SYCL_COMPILER)
    set(sycl ON)

    function(add_sycl_to_root_target_exe)
      cmake_parse_arguments(
        ARG "" "TARGET"
        "SOURCES;HEADERS;DEPENDENCIES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS"
        ${ARGN})

      # Get include directories for the SYCL target
      set(_inc_dirs ${ARG_INCLUDE_DIRECTORIES})
      list(APPEND _inc_dirs ${SYCL_INCLUDE_DIR})
      if(_inc_dirs)
        list(REMOVE_DUPLICATES _inc_dirs)
        list(TRANSFORM _inc_dirs PREPEND -I)
      endif()

      foreach(comp_def ${ARG_COMPILE_DEFINITIONS})
        list(APPEND _COMPILE_DEFINITIONS -D${comp_def})
      endforeach()

      # Compile the sycl source files with the found sycl compiler
      foreach(src ${ARG_SOURCES})
        set(_output_path
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${ARG_TARGET}.dir/${src}${CMAKE_CXX_OUTPUT_EXTENSION}
        )
        list(APPEND _outputs ${_output_path})

        add_custom_command(
          OUTPUT ${_output_path}
          COMMAND
            ${SYCL_COMPILER} ${SYCL_COMPILER_FLAGS} -c ${_inc_dirs}
            ${_COMPILE_DEFINITIONS} -o ${_output_path}
            ${CMAKE_CURRENT_SOURCE_DIR}/${src}
          DEPENDS ${ARG_DEPENDENCIES}
          COMMENT "Building SYCL object ${_output_path}"
          MAIN_DEPENDENCY ${src})
      endforeach()

      set(SYCL_LINKER_FLAGS "-L${SYCL_LIB_DIR}  -Wl,-rpath,${SYCL_LIB_DIR}")
      foreach(lib ${ARG_DEPENDENCIES})
        list(APPEND _lib_dep_paths "$<TARGET_FILE:${lib}>")
        list(APPEND SYCL_LINKER_FLAGS ":$<TARGET_FILE_DIR:${lib}>")
      endforeach()
      string(REPLACE ";" "" SYCL_LINKER_FLAGS ${SYCL_LINKER_FLAGS})

      separate_arguments(SYCL_LINKER_FLAGS NATIVE_COMMAND ${SYCL_LINKER_FLAGS})
      set_target_properties(${ARG_TARGET} PROPERTIES LINKER_LANGUAGE CXX)

      set_property(TARGET ${ARG_TARGET} PROPERTY SOURCES ${_outputs})
      set_property(TARGET ${ARG_TARGET} PROPERTY LINK_DEPENDS
                                                 ${ARG_DEPENDENCIES})

      add_custom_command(
        TARGET ${ARG_TARGET}
        COMMAND
          ${SYCL_COMPILER} ${SYCL_COMPILER_FLAGS} ${SYCL_LINKER_FLAGS}
          ${_lib_dep_paths} ${_COMPILE_DEFINITIONS} -o ${ARG_TARGET} ${_outputs}
          DEPENDS ${ARG_DEPENDENCIES}
        COMMENT "Building SYCL executable ${ARG_TARGET}" MAIN_DEPENDENCY
                ${ARG_TARGET})

    endfunction()

    function(add_sycl_to_root_target)
      cmake_parse_arguments(
        ARG "" "TARGET"
        "SOURCES;HEADERS;DEPENDENCIES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS"
        ${ARGN})

      set(_library_name ${ARG_TARGET})

      # Get include directories for the SYCL target
      set(_inc_dirs ${ARG_INCLUDE_DIRECTORIES})
      if(_inc_dirs)
        list(REMOVE_DUPLICATES _inc_dirs)
        list(TRANSFORM _inc_dirs PREPEND -I)
      endif()
      list(APPEND _inc_dirs -I${SYCL_INCLUDE_DIR})
      list(APPEND _inc_dirs -I${SYCL_INCLUDE_DIR}/sycl)

      foreach(lib ${ARG_DEPENDENCIES})
        list(APPEND _lib_dep_paths "$<TARGET_FILE:${lib}>")
      endforeach()

      foreach(comp_def ${ARG_COMPILE_DEFINITIONS})
        list(APPEND _COMPILE_DEFINITIONS -D${comp_def})
      endforeach()
      message(DEBUG "_COMPILE_DEFINITIONS : ${_COMPILE_DEFINITIONS}")

      # Compile the sycl source files with the found sycl compiler
      foreach(src ${ARG_SOURCES})
        set(_output_path
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${ARG_TARGET}.dir/${src}${CMAKE_CXX_OUTPUT_EXTENSION}
        )
        list(APPEND _outputs ${_output_path})

        add_custom_command(
          OUTPUT ${_output_path}
          COMMAND
            ${SYCL_COMPILER} ${SYCL_COMPILER_FLAGS} -c ${_inc_dirs}
            ${_COMPILE_DEFINITIONS} -o ${_output_path}
            ${CMAKE_CURRENT_SOURCE_DIR}/${src}
          DEPENDS ${ARG_DEPENDENCIES}
          COMMENT "Building SYCL object ${_output_path}"
          MAIN_DEPENDENCY ${src})
      endforeach()

      set(SYCL_LINKER_FLAGS
          "-shared -Wl,-soname,$<TARGET_FILE:${_library_name}> -L${SYCL_LIB_DIR} ${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,${SYCL_LIB_DIR}:$<TARGET_FILE_DIR:${_library_name}>"
      )
      separate_arguments(SYCL_LINKER_FLAGS NATIVE_COMMAND ${SYCL_LINKER_FLAGS})
      message(DEBUG "SYCL linker flags: ${SYCL_LINKER_FLAGS}")
      set_property(TARGET ${ARG_TARGET} PROPERTY LINK_DEPENDS ${_outputs})
      set_property(TARGET ${ARG_TARGET} PROPERTY LINK_DEPENDS
                                                 ${ARG_DEPENDENCIES})

      add_custom_command(
        OUTPUT ${ARG_TARGET}
        COMMAND
          ${SYCL_COMPILER} ${SYCL_LINKER_FLAGS} ${_lib_dep_paths} ${_inc_dirs}
          ${_COMPILE_DEFINITIONS} -o $<TARGET_FILE:${_library_name}> ${_outputs}
        DEPENDS ${ARG_DEPENDENCIES} ${_outputs}
        COMMENT "Linking shared library ${_library_name}"
        MAIN_DEPENDENCY ${_outputs})

    endfunction()

    message(
      STATUS
        "Found Intel OneAPI SYCL: ${SYCL_INCLUDE_DIR}  (modify with: SYCL_ROOT_DIR)"
    )
    message(STATUS "Using SYCL Host Compiler: ${SYCL_COMPILER}")
  else()
    if(fail-on-missing)
      message(FATAL_ERROR "OpenAPI SYCL library not found")
    else()
      message(STATUS "OpenAPI SYCL library not found")
      set(sycl
          OFF
          CACHE BOOL "Disabled because no SYCL implementation is not found"
                FORCE)
    endif()
  endif()
endif()

if(adaptivecpp)
  if(oneapi)
    message(WARNING "Disable OneAPI to load AdaptiveCpp")
    set(sycl
        OFF
        CACHE BOOL "Disabled because AdaptiveCpp is enabled" FORCE)
  else()
    find_package(AdaptiveCpp)
    # add_definitions(-DHIPSYCL_DEBUG_LEVEL=4)
    if(AdaptiveCpp_FOUND)
      set(sycl ON)
      function(add_sycl_to_root_target)
        cmake_parse_arguments(
          ARG "" "TARGET"
          "SOURCES;HEADERS;DEPENDENCIES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS"
          ${ARGN})
        target_compile_definitions(${ARG_TARGET}
                                   PUBLIC ${ARG_COMPILE_DEFINITIONS})
        target_link_libraries(${ARG_TARGET} PUBLIC ${ARG_DEPENDENCIES})
        target_include_directories(${ARG_TARGET}
                                   INTERFACE ${AdaptiveCpp_INCLUDE_DIRS})
        target_link_libraries(${ARG_TARGET} INTERFACE AdaptiveCpp::acpp-rt)
        add_sycl_to_target(TARGET ${ARG_TARGET} SOURCES ${ARG_SOURCES})
      endfunction()
      message(STATUS "AdaptiveCpp sycl enabled")
    else()
      if(fail-on-missing)
        message(FATAL_ERROR "AdaptiveCpp library not found")
      else()
        message(STATUS "AdaptiveCpp library not found")
        set(sycl
            OFF
            CACHE BOOL "Disabled because no SYCL implementation is not found"
                  FORCE)
      endif()
    endif()
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT ACPP_DEBUG_LEVEL)
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(ACPP_DEBUG_LEVEL
        3
        CACHE
          STRING
          "Choose the debug level, options are: 0 (no debug), 1 (print errors), 2 (also print warnings), 3 (also print general information)"
          FORCE)
  else()
    set(ACPP_DEBUG_LEVEL
        2
        CACHE
          STRING
          "Choose the debug level, options are: 0 (no debug), 1 (print errors), 2 (also print warnings), 3 (also print general information)"
          FORCE)
  endif()
endif()

if((cuda) OR ((alpaka) AND (alpakacuda)))
  include(CheckLanguage)
  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
  else()
    message(FATAL_ERROR "CUDA library not found")
  endif()
endif()

if((hip) OR ((alpaka) AND (alpakahip)))
  include(CheckLanguage)
  check_language(HIP)
  if(CMAKE_HIP_COMPILER)
    enable_language(HIP)
  else()
    message(FATAL_ERROR "HIP library not found")
  endif()
endif()

if(cuda)
  add_library(SYCLMath SHARED src/MathUtil.cxx)
  set_source_files_properties(src/MathUtil.cxx PROPERTIES LANGUAGE CUDA)
  target_compile_options(SYCLMath PRIVATE -rdc=true -lineinfo
                                          --expt-relaxed-constexpr)
  set_target_properties(SYCLMath PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(SYCLMath INTERFACE ROOT_MATH_CUDA)
  list(APPEND cuda_compile_list ROOT_MATH_CUDA)
  if(timing)
    list(APPEND cuda_compile_list ROOT_MEAS_TIMING)
  endif()
elseif(alpaka)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  find_package(Boost 1.74.0 REQUIRED)
  find_package(alpaka 1.2.0 REQUIRED)

  add_library(SYCLMath SHARED src/MathUtil.cxx)
  target_include_directories(SYCLMath PRIVATE ${Alpaka_SOURCE_DIR}/include
                                              ${Boost_INCLUDE_DIR})
  target_link_libraries(SYCLMath PUBLIC alpaka::alpaka)
  if (alpakacuda)
    set_source_files_properties(src/MathUtil.cxx PROPERTIES LANGUAGE CUDA)
    target_compile_definitions(SYCLMath PRIVATE ALPAKA_ACC_GPU_CUDA_ENABLED)
  elseif (alpakahip)
    set_source_files_properties(src/MathUtil.cxx PROPERTIES LANGUAGE HIP)
    target_compile_definitions(SYCLMath PRIVATE ALPAKA_ACC_GPU_CUDA_ENABLED)
    else()
    set_source_files_properties(src/MathUtil.cxx PROPERTIES LANGUAGE CXX)
    target_compile_definitions(SYCLMath PRIVATE ALPAKA_HOST_ONLY
                          ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
  endif()
  list(APPEND alpaka_compile_list ROOT_MATH_ALPAKA)
  if(timing)
    list(APPEND alpaka_compile_list ROOT_MEAS_TIMING)
  endif()
elseif(hip)
  add_library(SYCLMath SHARED
	src/MathUtil.cxx
  )
  set_source_files_properties(src/MathUtil.cxx PROPERTIES LANGUAGE HIP)
  target_compile_definitions(SYCLMath INTERFACE ROOT_MATH_HIP)
  list(APPEND hip_compile_list ROOT_MATH_HIP)
  if (timing)
    list(APPEND hip_compile_list ROOT_MEAS_TIMING)
  endif()
else()
  add_library(SYCLMath SHARED src/MathUtil.cxx)
  set_target_properties(SYCLMath PROPERTIES LINKER_LANGUAGE CXX)
endif()

if(sycl)
  # set_target_properties(GenVector PROPERTIES LINKER_LANGUAGE CXX)
  target_compile_definitions(SYCLMath PUBLIC ROOT_MATH_SYCL)
  list(APPEND sycl_compile_list ROOT_MATH_SYCL)
  if(timing)
    list(APPEND sycl_compile_list ROOT_MEAS_TIMING)
  endif()
  if(sycl_buffer)
    list(APPEND sycl_compile_list SYCL_BUFFER)
  endif()
  message(DEBUG "sycl_buffer: ${sycl_buffer}")
  message(DEBUG "sycl_compile_list: ${sycl_compile_list}")
  add_sycl_to_root_target(
    TARGET
    SYCLMath
    SOURCES
    src/MathUtil.cxx
    HEADERS
    inc/SYCLMath/GenVector/MathUtil.h
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/inc/
    COMPILE_DEFINITIONS
    ${sycl_compile_list})
endif()

if(timing)
  list(APPEND compile_list ROOT_MEAS_TIMING)
endif()

add_library(
  GenVectorX SHARED
  src/3DConversions.cxx
  src/BitReproducible.cxx
  src/BoostZ.cxx
  src/Quaternion.cxx
  src/RotationZYX.cxx
  src/3DDistances.cxx
  src/Boost.cxx
  src/EulerAngles.cxx
  src/QuaternionXaxial.cxx
  # src/VecOps.cxx
  src/AxisAngle.cxx
  src/BoostX.cxx
  src/LorentzRotation.cxx
  src/Rotation3D.cxx
  src/VectorUtil.cxx
  src/AxisAngleXother.cxx
  src/BoostY.cxx
  src/MathUtil.cxx
  src/Rotation3DxAxial.cxx)

target_include_directories(GenVectorX INTERFACE inc)
set_target_properties(GenVectorX PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(GenVectorX PUBLIC SYCLMath)
target_compile_definitions(GenVectorX PUBLIC ${compile_list})


if((cuda)
   OR (sycl)
   OR (alpaka)
   OR (hip))
  add_library(VecOps SHARED src/VecOps.cxx)
  target_link_libraries(VecOps PUBLIC GenVectorX SYCLMath)

  if(cuda)
    set_source_files_properties(src/VecOps.cxx PROPERTIES LANGUAGE CUDA)
    target_compile_options(VecOps PRIVATE -rdc=true -lineinfo
                                          --expt-relaxed-constexpr)
    set_target_properties(VecOps PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(VecOps PUBLIC ${cuda_compile_list})
  endif()
  if (hip)
    set_source_files_properties(src/VecOps.cxx PROPERTIES LANGUAGE HIP)
    target_compile_definitions(VecOps PUBLIC ${hip_compile_list})
  endif()
  if (sycl)
     set_target_properties(VecOps PROPERTIES LINKER_LANGUAGE CXX)
  endif()
  if(alpaka)
    target_include_directories(VecOps PRIVATE ${Alpaka_SOURCE_DIR}/include
                                              ${Boost_INCLUDE_DIR})
    if(alpakacuda)
      set_source_files_properties(src/VecOps.cxx PROPERTIES LANGUAGE CUDA)
      set_target_properties(VecOps PROPERTIES LINKER_LANGUAGE CUDA)
      target_compile_definitions(VecOps PUBLIC ${alpaka_compile_list}
                                               ALPAKA_ACC_GPU_CUDA_ENABLED)
    elseif(alpakahip)
      set_source_files_properties(src/VecOps.cxx PROPERTIES LANGUAGE HIP)
      set_target_properties(VecOps PROPERTIES LINKER_LANGUAGE HIP)
      target_compile_definitions(VecOps PUBLIC ${alpaka_compile_list}
                                               ALPAKA_ACC_GPU_HIP_ENABLED)
    else()
      set_source_files_properties(src/VecOps.cxx PROPERTIES LANGUAGE CXX)
      target_compile_definitions(
        VecOps PUBLIC ${alpaka_compile_list} ALPAKA_HOST_ONLY
                      ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
    endif()
  endif()
  
  if(sycl)
    set_target_properties(VecOps PROPERTIES LINKER_LANGUAGE CXX)

    add_sycl_to_root_target(
      TARGET
      VecOps
      SOURCES
      src/VecOps.cxx
      HEADERS
      inc/SYCLMath/VecOps.h
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc/
      DEPENDENCIES
      GenVectorX
      SYCLMath
      COMPILE_DEFINITIONS
      ${sycl_compile_list})

    set_target_properties(GenVectorX PROPERTIES LINKER_LANGUAGE CXX)

    add_sycl_to_root_target(
      TARGET
      GenVectorX
      SOURCES
      src/3DConversions.cxx
      src/BitReproducible.cxx
      src/BoostZ.cxx
      src/Quaternion.cxx
      src/RotationZYX.cxx
      src/3DDistances.cxx
      src/Boost.cxx
      src/EulerAngles.cxx
      src/QuaternionXaxial.cxx
      # src/VecOps.cxx
      src/AxisAngle.cxx
      src/BoostX.cxx
      src/LorentzRotation.cxx
      src/Rotation3D.cxx
      src/VectorUtil.cxx
      src/AxisAngleXother.cxx
      src/BoostY.cxx
      src/MathUtil.cxx
      src/Rotation3DxAxial.cxx
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc/
      DEPENDENCIES
      SYCLMath
      COMPILE_DEFINITIONS
      ${sycl_compile_list})

  endif()

  install(
    TARGETS GenVectorX SYCLMath VecOps
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
else()
  install(
    TARGETS GenVectorX SYCLMath
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(testing)
  add_subdirectory(testx)
endif()
