set(sycl_compile_list_single "${sycl_compile_list}")
list(APPEND sycl_compile_list_single SINGLE_PRECISION)

set(alpaka_compile_list_single "${alpaka_compile_list}")
list(APPEND alpaka_compile_list_single SINGLE_PRECISION)

set(cuda_compile_list_single "${cuda_compile_list}")
list(APPEND cuda_compile_list_single SINGLE_PRECISION)

set(compile_list_single "${compile_list}")
list(APPEND compile_list_single SINGLE_PRECISION)

if(alpaka)

  if(alpakacpu)
    add_executable(BoostAlpakaCpu BoostAlpaka.cxx)
    target_link_libraries(BoostAlpakaCpu PUBLIC alpaka::alpaka)
    target_compile_options(
      BoostAlpakaCpu PRIVATE -DALPAKA_HOST_ONLY
                             -DALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
    target_include_directories(
      BoostAlpakaCpu PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      BoostAlpakaCpu PRIVATE ${alpaka_compile_list} ALPAKA_HOST_ONLY
                             ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)

    add_executable(InvariantMassesAlpakaCpu InvariantMassesAlpaka.cxx)
    target_link_libraries(InvariantMassesAlpakaCpu PUBLIC alpaka::alpaka)
    target_include_directories(
      InvariantMassesAlpakaCpu PRIVATE ${Alpaka_SOURCE_DIR}/include
                                       ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      InvariantMassesAlpakaCpu PRIVATE ${alpaka_compile_list} ALPAKA_HOST_ONLY
                                       ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
    if(single_precision)
      add_executable(SBoostAlpakaCpu BoostAlpaka.cxx)
      target_link_libraries(SBoostAlpakaCpu PUBLIC alpaka::alpaka)
      target_compile_options(
        SBoostAlpakaCpu PRIVATE -DALPAKA_HOST_ONLY
                              -DALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
      target_include_directories(
        SBoostAlpakaCpu PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SBoostAlpakaCpu PRIVATE ${alpaka_compile_list_single} ALPAKA_HOST_ONLY
                              ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)

      add_executable(SInvariantMassesAlpakaCpu InvariantMassesAlpaka.cxx)
      target_link_libraries(SInvariantMassesAlpakaCpu PUBLIC alpaka::alpaka)
      target_include_directories(
        SInvariantMassesAlpakaCpu PRIVATE ${Alpaka_SOURCE_DIR}/include
                                        ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SInvariantMassesAlpakaCpu PRIVATE ${alpaka_compile_list_single} ALPAKA_HOST_ONLY
                                        ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED)
    endif()
  endif()

  if(alpakacuda)
	  set_source_files_properties(BoostAlpaka.cxx PROPERTIES LANGUAGE CUDA)
    add_executable(BoostAlpakaCuda BoostAlpaka.cxx)
    target_link_libraries(BoostAlpakaCuda PUBLIC alpaka::alpaka)
    target_compile_options(BoostAlpakaCuda
                           PRIVATE -DALPAKA_ACC_GPU_CUDA_ENABLED)
    target_include_directories(
      BoostAlpakaCuda PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      BoostAlpakaCuda PRIVATE ${alpaka_compile_list}
                              ALPAKA_ACC_GPU_CUDA_ENABLED)

	  set_source_files_properties(InvariantMassesAlpaka.cxx PROPERTIES LANGUAGE CUDA)
    add_executable(InvariantMassesAlpakaCuda InvariantMassesAlpaka.cxx)
    target_link_libraries(InvariantMassesAlpakaCuda PUBLIC alpaka::alpaka)
    target_include_directories(
      InvariantMassesAlpakaCuda PRIVATE ${Alpaka_SOURCE_DIR}/include
                                        ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      InvariantMassesAlpakaCuda PRIVATE ${alpaka_compile_list}
                                        ALPAKA_ACC_GPU_CUDA_ENABLED)
    if(single_precision)
      set_source_files_properties(SBoostAlpaka.cxx PROPERTIES LANGUAGE CUDA)
      add_executable(SBoostAlpakaCuda BoostAlpaka.cxx)
      target_link_libraries(SBoostAlpakaCuda PUBLIC alpaka::alpaka)
      target_compile_options(SBoostAlpakaCuda
                            PRIVATE -DALPAKA_ACC_GPU_CUDA_ENABLED)
      target_include_directories(
        SBoostAlpakaCuda PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SBoostAlpakaCuda PRIVATE ${alpaka_compile_list_single}
                                ALPAKA_ACC_GPU_CUDA_ENABLED)

      set_source_files_properties(SInvariantMassesAlpaka.cxx PROPERTIES LANGUAGE CUDA)
      add_executable(SInvariantMassesAlpakaCuda InvariantMassesAlpaka.cxx)
      target_link_libraries(SInvariantMassesAlpakaCuda PUBLIC alpaka::alpaka)
      target_include_directories(
        SInvariantMassesAlpakaCuda PRIVATE ${Alpaka_SOURCE_DIR}/include
                                          ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SInvariantMassesAlpakaCuda PRIVATE ${alpaka_compile_list_single}
                                          ALPAKA_ACC_GPU_CUDA_ENABLED)
    endif()
  endif()

  if(alpakahip)
	  set_source_files_properties(BoostAlpaka.cxx PROPERTIES LANGUAGE HIP)
    add_executable(BoostAlpakaHip BoostAlpaka.cxx)
    target_link_libraries(BoostAlpakaHip PUBLIC alpaka::alpaka)
    target_include_directories(
      BoostAlpakaHip PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      BoostAlpakaHip PRIVATE ${alpaka_compile_list} ALPAKA_ACC_GPU_HIP_ENABLED)

	  set_source_files_properties(InvariantMassesAlpaka.cxx PROPERTIES LANGUAGE HIP)
    add_executable(InvariantMassesAlpakaHip InvariantMassesAlpaka.cxx)
    target_link_libraries(InvariantMassesAlpakaHip PUBLIC alpaka::alpaka)
    target_include_directories(
      InvariantMassesAlpakaHip PRIVATE ${Alpaka_SOURCE_DIR}/include
                                       ${Boost_INCLUDE_DIR})
    target_compile_definitions(
      InvariantMassesAlpakaHip PRIVATE ${alpaka_compile_list}
                                       ALPAKA_ACC_GPU_HIP_ENABLED)
    if(single_precision)
      set_source_files_properties(SBoostAlpaka.cxx PROPERTIES LANGUAGE HIP)
      add_executable(SBoostAlpakaHip BoostAlpaka.cxx)
      target_link_libraries(SBoostAlpakaHip PUBLIC alpaka::alpaka)
      target_include_directories(
        SBoostAlpakaHip PRIVATE ${Alpaka_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SBoostAlpakaHip PRIVATE ${alpaka_compile_list_single} ALPAKA_ACC_GPU_HIP_ENABLED)

      set_source_files_properties(SInvariantMassesAlpaka.cxx PROPERTIES LANGUAGE HIP)
      add_executable(SInvariantMassesAlpakaHip InvariantMassesAlpaka.cxx)
      target_link_libraries(SInvariantMassesAlpakaHip PUBLIC alpaka::alpaka)
      target_include_directories(
        SInvariantMassesAlpakaHip PRIVATE ${Alpaka_SOURCE_DIR}/include
                                        ${Boost_INCLUDE_DIR})
      target_compile_definitions(
        SInvariantMassesAlpakaHip PRIVATE ${alpaka_compile_list_single}
                                        ALPAKA_ACC_GPU_HIP_ENABLED)
    endif()
  endif()

endif()

if(oneapi)
  add_executable(InvariantMassesSYCL InvariantMassesSYCL.cxx)
  target_link_options(InvariantMassesSYCL PUBLIC -E)
  add_sycl_to_root_target_exe(
    TARGET
    InvariantMassesSYCL
    SOURCES
    InvariantMassesSYCL.cxx
    DEPENDENCIES
    GenVectorX
    SYCLMath
    VecOps
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/inc
    COMPILE_DEFINITIONS
    ${sycl_compile_list})

  add_executable(BoostSYCL BoostSYCL.cxx)
  target_link_options(BoostSYCL PUBLIC -E)
  add_sycl_to_root_target_exe(
    TARGET
    BoostSYCL
    SOURCES
    BoostSYCL.cxx
    DEPENDENCIES
    GenVectorX
    SYCLMath
    VecOps
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/inc
    COMPILE_DEFINITIONS
    ${sycl_compile_list})

  if(single_precision)
    add_executable(SInvariantMassesSYCL InvariantMassesSYCL.cxx)
    target_link_options(SInvariantMassesSYCL PUBLIC -E)
    add_sycl_to_root_target_exe(
      TARGET
      SInvariantMassesSYCL
      SOURCES
      InvariantMassesSYCL.cxx
      DEPENDENCIES
      GenVectorX
      SYCLMath
      VecOps
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc
      COMPILE_DEFINITIONS
      ${sycl_compile_list_single})

    add_executable(SBoostSYCL BoostSYCL.cxx)
    target_link_options(SBoostSYCL PUBLIC -E)
    add_sycl_to_root_target_exe(
      TARGET
      SBoostSYCL
      SOURCES
      BoostSYCL.cxx
      DEPENDENCIES
      GenVectorX
      SYCLMath
      VecOps
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc
      COMPILE_DEFINITIONS
      ${sycl_compile_list_single})

  endif()

endif()

if(adaptivecpp)
  add_executable(InvariantMassesSYCL InvariantMassesSYCL.cxx)
  add_sycl_to_root_target(
    TARGET
    InvariantMassesSYCL
    SOURCES
    InvariantMassesSYCL.cxx
    DEPENDENCIES
    GenVectorX
    SYCLMath
    VecOps
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/inc
    COMPILE_DEFINITIONS
    ${sycl_compile_list})

  add_executable(BoostSYCL BoostSYCL.cxx)
  add_sycl_to_root_target(
    TARGET
    BoostSYCL
    SOURCES
    BoostSYCL.cxx
    DEPENDENCIES
    GenVectorX
    SYCLMath
    VecOps
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/inc
    COMPILE_DEFINITIONS
    ${sycl_compile_list})

  if(single_precision)
    add_executable(SInvariantMassesSYCL InvariantMassesSYCL.cxx)
    add_sycl_to_root_target(
      TARGET
      SInvariantMassesSYCL
      SOURCES
      InvariantMassesSYCL.cxx
      DEPENDENCIES
      GenVectorX
      SYCLMath
      VecOps
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc
      COMPILE_DEFINITIONS
      ${sycl_compile_list_single})

    add_executable(SBoostSYCL BoostSYCL.cxx)
    add_sycl_to_root_target(
      TARGET
      SBoostSYCL
      SOURCES
      BoostSYCL.cxx
      DEPENDENCIES
      GenVectorX
      SYCLMath
      VecOps
      INCLUDE_DIRECTORIES
      ${CMAKE_SOURCE_DIR}/inc
      COMPILE_DEFINITIONS
      ${sycl_compile_list_single})

  endif()

endif()

if(cuda)
  add_executable(InvariantMassesCUDA InvariantMassesCUDA.cu)
  set_target_properties(InvariantMassesCUDA PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(InvariantMassesCUDA PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(InvariantMassesCUDA PRIVATE -rdc=true -lineinfo
                                                     --expt-relaxed-constexpr)
  set_target_properties(InvariantMassesCUDA
                        PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(InvariantMassesCUDA PUBLIC ${cuda_compile_list})

  add_executable(InvariantMassesCUDAStreamed InvariantMassesCUDAStreamed.cu)
  set_target_properties(InvariantMassesCUDAStreamed PROPERTIES LINKER_LANGUAGE
                                                               CUDA)
  target_link_libraries(InvariantMassesCUDAStreamed PUBLIC GenVectorX SYCLMath
                                                           VecOps)
  target_compile_options(InvariantMassesCUDAStreamed
                         PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(InvariantMassesCUDAStreamed
                        PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(InvariantMassesCUDAStreamed
                             PUBLIC ${cuda_compile_list})

  add_executable(BoostCUDA BoostCUDA.cu)
  set_target_properties(BoostCUDA PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(BoostCUDA PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(BoostCUDA PRIVATE -rdc=true -lineinfo
                                           --expt-relaxed-constexpr)
  set_target_properties(BoostCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(BoostCUDA PUBLIC ${cuda_compile_list})

  add_executable(BoostCUDAStreamed BoostCUDAStreamed.cu)
  set_target_properties(BoostCUDAStreamed PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(BoostCUDAStreamed PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(BoostCUDAStreamed PRIVATE -rdc=true -lineinfo
                                                   --expt-relaxed-constexpr)
  set_target_properties(BoostCUDAStreamed PROPERTIES CUDA_SEPARABLE_COMPILATION
                                                     ON)
  target_compile_definitions(BoostCUDAStreamed PUBLIC ${cuda_compile_list})

  if(single_precision)
    add_executable(SInvariantMassesCUDA InvariantMassesCUDA.cu)
    set_target_properties(SInvariantMassesCUDA PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(InvariantMassesCUDA PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SInvariantMassesCUDA
                           PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
    set_target_properties(SInvariantMassesCUDA
                          PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SInvariantMassesCUDA
                               PUBLIC ${cuda_compile_list_single})

    add_executable(SInvariantMassesCUDAStreamed InvariantMassesCUDAStreamed.cu)
    set_target_properties(SInvariantMassesCUDAStreamed
                          PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(InvariantMassesCUDAStreamed PUBLIC GenVectorX
                                                             SYCLMath VecOps)
    target_compile_options(SInvariantMassesCUDAStreamed
                           PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
    set_target_properties(SInvariantMassesCUDAStreamed
                          PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SInvariantMassesCUDAStreamed
                               PUBLIC ${cuda_compile_list_single})

    add_executable(SBoostCUDA BoostCUDA.cu)
    set_target_properties(SBoostCUDA PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(SBoostCUDA PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SBoostCUDA PRIVATE -rdc=true -lineinfo
                                              --expt-relaxed-constexpr)
    set_target_properties(SBoostCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SBoostCUDA PUBLIC ${cuda_compile_list_single})

    add_executable(SBoostCUDAStreamed BoostCUDAStreamed.cu)
    set_target_properties(SBoostCUDAStreamed PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(SBoostCUDAStreamed PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SBoostCUDAStreamed PRIVATE -rdc=true -lineinfo
                                                      --expt-relaxed-constexpr)
    set_target_properties(SBoostCUDAStreamed
                          PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SBoostCUDAStreamed
                               PUBLIC ${cuda_compile_list_single})
  endif()

endif()

add_executable(InvariantMasses InvariantMasses.cxx)
target_compile_definitions(InvariantMasses PUBLIC ${compile_list})

add_executable(Boost Boost.cxx)
target_compile_definitions(Boost PUBLIC ${compile_list})

if(single_precision)
  add_executable(SInvariantMasses InvariantMasses.cxx)
  target_compile_definitions(SInvariantMasses PRIVATE ${compile_list_single})

  add_executable(SBoost Boost.cxx)
  target_compile_definitions(SBoost PRIVATE ${compile_list_single})
endif()
