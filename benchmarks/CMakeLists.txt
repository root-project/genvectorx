
add_executable(Boost_bench Boost.cpp)
target_link_libraries(Boost_bench PRIVATE benchmark::benchmark)
target_compile_definitions(Boost_bench PRIVATE ${compile_list})

add_executable(InvariantMasses_bench InvariantMasses.cpp)
target_link_libraries(InvariantMasses_bench PRIVATE benchmark::benchmark)
target_compile_definitions(InvariantMasses_bench PRIVATE ${compile_list})

if (single_precision)
	add_executable(SBoost_bench Boost.cpp)
	target_link_libraries(SBoost_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SBoost_bench PRIVATE ${compile_list_single})

	add_executable(SInvariantMasses_bench InvariantMasses.cpp)
	target_link_libraries(SInvariantMasses_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SInvariantMasses_bench PRIVATE ${compile_list_single})
endif()

if (oneapi)

  add_executable(InvariantMassesSYCL_bench InvariantMassesSYCL.cpp)
  target_link_options(InvariantMassesSYCL_bench PUBLIC -E)
  add_sycl_to_root_target_exe(TARGET InvariantMassesSYCL_bench SOURCES InvariantMassesSYCL.cpp 
                              DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                              INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                              COMPILE_DEFINITIONS ${sycl_compile_list})

  add_executable(BoostSYCL_bench BoostSYCL.cpp)
  target_link_options(BoostSYCL_bench PUBLIC -E)
  add_sycl_to_root_target_exe(TARGET BoostSYCL_bench SOURCES BoostSYCL.cpp 
                              DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                              INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                              COMPILE_DEFINITIONS ${sycl_compile_list})

  if (single_precision)
    add_executable(SInvariantMassesSYCL_bench InvariantMassesSYCL.cpp)
    target_link_options(SInvariantMassesSYCL_bench PUBLIC -E)
    add_sycl_to_root_target_exe(TARGET SInvariantMassesSYCL_bench SOURCES InvariantMassesSYCL.cpp 
                                DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                                INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                                COMPILE_DEFINITIONS ${sycl_compile_list_single})

  
    add_executable(SBoostSYCL_bench BoostSYCL.cpp)
    target_link_options(SBoostSYCL_bench PUBLIC -E)
    add_sycl_to_root_target_exe(TARGET SBoostSYCL_bench SOURCES BoostSYCL.cpp 
                                DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                                INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                                COMPILE_DEFINITIONS ${sycl_compile_list_single})
  
    endif()

endif()

if (adaptivecpp)
  add_executable(InvariantMassesSYCL_bench InvariantMassesSYCL.cpp)
  add_sycl_to_root_target(TARGET InvariantMassesSYCL_bench SOURCES InvariantMassesSYCL.cpp 
                          DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                          INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                          COMPILE_DEFINITIONS ${sycl_compile_list})
 

  add_executable(BoostSYCL_bench BoostSYCL.cpp)
  add_sycl_to_root_target(TARGET BoostSYCL_bench SOURCES BoostSYCL.cpp 
                          DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                          INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                          COMPILE_DEFINITIONS ${sycl_compile_list})
  

  if (single_precision)
    add_executable(SInvariantMassesSYCL_bench InvariantMassesSYCL.cpp)
    add_sycl_to_root_target(TARGET SInvariantMassesSYCL_bench SOURCES InvariantMassesSYCL.cpp 
                            DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                            INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                            COMPILE_DEFINITIONS ${sycl_compile_list_single}) 


    add_executable(SBoostSYCL_bench BoostSYCL.cpp)
    add_sycl_to_root_target(TARGET SBoostSYCL_bench SOURCES BoostSYCL.cpp 
                            DEPENDENCIES GenVectorX SYCLMath VecOps benchmark
                            INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc 
                            COMPILE_DEFINITIONS ${sycl_compile_list_single})

  endif()

endif()


if (cuda)
  add_executable(BoostCUDA_bench BoostCUDA.cu)
  set_target_properties(BoostCUDA_bench PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(BoostCUDA_bench PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(BoostCUDA_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(BoostCUDA_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_link_libraries(BoostCUDA_bench PRIVATE benchmark::benchmark)
  target_compile_definitions(BoostCUDA_bench PRIVATE ${cuda_compile_list})

  add_executable(BoostCUDAStreamed_bench BoostCUDAStreamed.cu)
  set_target_properties(BoostCUDAStreamed_bench PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(BoostCUDAStreamed_bench PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(BoostCUDAStreamed_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(BoostCUDAStreamed_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_link_libraries(BoostCUDAStreamed_bench PRIVATE benchmark::benchmark)
  target_compile_definitions(BoostCUDAStreamed_bench PRIVATE ${cuda_compile_list})

  add_executable(InvariantMassesCUDA_bench InvariantMassesCUDA.cu)
  set_target_properties(InvariantMassesCUDA_bench PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(InvariantMassesCUDA_bench PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(InvariantMassesCUDA_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(InvariantMassesCUDA_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_link_libraries(InvariantMassesCUDA_bench PRIVATE benchmark::benchmark)
  target_compile_definitions(InvariantMassesCUDA_bench PRIVATE ${cuda_compile_list})

  add_executable(InvariantMassesCUDAStreamed_bench InvariantMassesCUDAStreamed.cu)
  set_target_properties(InvariantMassesCUDAStreamed_bench PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(InvariantMassesCUDAStreamed_bench PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(InvariantMassesCUDAStreamed_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(InvariantMassesCUDAStreamed_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_link_libraries(InvariantMassesCUDAStreamed_bench PRIVATE benchmark::benchmark)
  target_compile_definitions(InvariantMassesCUDAStreamed_bench PRIVATE ${cuda_compile_list})

  if (single_precision)
	add_executable(SBoostCUDA_bench BoostCUDA.cu)
	set_target_properties(SBoostCUDA_bench PROPERTIES LINKER_LANGUAGE CUDA)
	target_link_libraries(SBoostCUDA_bench PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_options(SBoostCUDA_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
	set_target_properties(SBoostCUDA_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
	target_link_libraries(SBoostCUDA_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SBoostCUDA_bench PRIVATE ${cuda_compile_list_single})

	add_executable(SBoostCUDAStreamed_bench BoostCUDAStreamed.cu)
	set_target_properties(SBoostCUDAStreamed_bench PROPERTIES LINKER_LANGUAGE CUDA)
	target_link_libraries(SBoostCUDAStreamed_bench PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_options(SBoostCUDAStreamed_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
	set_target_properties(SBoostCUDAStreamed_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
	target_link_libraries(SBoostCUDAStreamed_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SBoostCUDAStreamed_bench PRIVATE ${cuda_compile_list_single})

	add_executable(SInvariantMassesCUDA_bench InvariantMassesCUDA.cu)
	set_target_properties(SInvariantMassesCUDA_bench PROPERTIES LINKER_LANGUAGE CUDA)
	target_link_libraries(SInvariantMassesCUDA_bench PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_options(SInvariantMassesCUDA_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
	set_target_properties(SInvariantMassesCUDA_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
	target_link_libraries(SInvariantMassesCUDA_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SInvariantMassesCUDA_bench PRIVATE ${cuda_compile_list_single})

	add_executable(SInvariantMassesCUDAStreamed_bench InvariantMassesCUDAStreamed.cu)
	set_target_properties(SInvariantMassesCUDAStreamed_bench PROPERTIES LINKER_LANGUAGE CUDA)
	target_link_libraries(SInvariantMassesCUDAStreamed_bench PUBLIC GenVectorX SYCLMath VecOps)
	target_compile_options(SInvariantMassesCUDAStreamed_bench PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
	set_target_properties(SInvariantMassesCUDAStreamed_bench PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
	target_link_libraries(SInvariantMassesCUDAStreamed_bench PRIVATE benchmark::benchmark)
	target_compile_definitions(SInvariantMassesCUDAStreamed_bench PRIVATE ${cuda_compile_list_single})
  endif()

endif()

