set(sycl_compile_list_single "${sycl_compile_list}")
list(APPEND sycl_compile_list_single SINGLE_PRECISION)

set(cuda_compile_list_single "${cuda_compile_list}")
list(APPEND cuda_compile_list_single SINGLE_PRECISION)

set(hip_compile_list_single "${hip_compile_list}")
list(APPEND hip_compile_list_single SINGLE_PRECISION)

if (oneapi)
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

if (hip)
  set_source_files_properties(InvariantMassesHIP.cxx PROPERTIES LANGUAGE HIP)
  add_executable(InvariantMassesHIP InvariantMassesHIP.cxx)
  target_link_libraries(InvariantMassesHIP PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_definitions(InvariantMassesHIP PUBLIC ${hip_compile_list})

  set_source_files_properties(InvariantMassesHIPStreamed.cxx PROPERTIES LANGUAGE HIP)
  add_executable(InvariantMassesHIPStreamed InvariantMassesHIPStreamed.cxx)
  target_link_libraries(InvariantMassesHIPStreamed PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_definitions(InvariantMassesHIPStreamed PUBLIC ${hip_compile_list})

  set_source_files_properties(BoostHIP.cxx PROPERTIES LANGUAGE HIP)
  add_executable(BoostHIP BoostHIP.cxx)
  target_link_libraries(BoostHIP PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_definitions(BoostHIP PUBLIC ${hip_compile_list})

  set_source_files_properties(BoostHIPStreamed.cxx PROPERTIES LANGUAGE HIP)
  add_executable(BoostHIPStreamed BoostHIPStreamed.cxx)
  target_link_libraries(BoostHIPStreamed PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_definitions(BoostHIPStreamed PUBLIC ${hip_compile_list})

  if (single_precision)
	add_executable(SInvariantMassesHIP InvariantMassesCUDA.cxx)
	target_link_libraries(InvariantMassesHIP PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_definitions(SInvariantMassesHIP PUBLIC ${hip_compile_list_single})

	add_executable(SInvariantMassesHIPStreamed InvariantMassesCUDAStreamed.cxx)
	target_link_libraries(InvariantMassesHIPStreamed PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_definitions(SInvariantMassesHIPStreamed PUBLIC ${hip_compile_list_single})

	add_executable(SBoostHIP BoostHIP.cxx)
	target_link_libraries(SBoostHIP PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_definitions(SBoostHIP PUBLIC ${hip_compile_list_single})

	add_executable(SBoostHIPStreamed BoostHIPStreamed.cxx)
	target_link_libraries(SBoostHIPStreamed PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_definitions(SBoostHIPStreamed PUBLIC ${hip_compile_list_single})

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
