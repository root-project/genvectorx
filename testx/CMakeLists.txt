if (oneapi)
  add_executable(InvariantMassesSYCL InvariantMassesSYCL.cxx)
  target_link_options(InvariantMassesSYCL PUBLIC -E)
  add_sycl_to_root_target_exe(TARGET InvariantMassesSYCL SOURCES InvariantMassesSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)

  add_executable(ChangeCoordSYCL ChangeCoordSYCL.cxx)
  target_link_options(ChangeCoordSYCL PUBLIC -E)
  add_sycl_to_root_target_exe(TARGET ChangeCoordSYCL SOURCES ChangeCoordSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)
 

  add_executable(BoostSYCL BoostSYCL.cxx)
  target_link_options(BoostSYCL PUBLIC -E)
  add_sycl_to_root_target_exe(TARGET BoostSYCL SOURCES BoostSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)

  if (single_precision)
    add_executable(SInvariantMassesSYCL InvariantMassesSYCL.cxx)
    target_link_options(SInvariantMassesSYCL PUBLIC -E)
    add_sycl_to_root_target_exe(TARGET SInvariantMassesSYCL SOURCES InvariantMassesSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION)

    add_executable(SChangeCoordSYCL ChangeCoordSYCL.cxx)
    target_link_options(SChangeCoordSYCL PUBLIC -E)
    add_sycl_to_root_target_exe(TARGET SChangeCoordSYCL SOURCES ChangeCoordSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION)
   
  
    add_executable(SBoostSYCL BoostSYCL.cxx)
    target_link_options(SBoostSYCL PUBLIC -E)
    add_sycl_to_root_target_exe(TARGET SBoostSYCL SOURCES BoostSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION)
  
    endif()

endif()

if (adaptivecpp)
  add_executable(InvariantMassesSYCL InvariantMassesSYCL.cxx)
  add_sycl_to_root_target(TARGET InvariantMassesSYCL SOURCES InvariantMassesSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)

  add_executable(ChangeCoordSYCL ChangeCoordSYCL.cxx)
  add_sycl_to_root_target(TARGET ChangeCoordSYCL SOURCES ChangeCoordSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)
  

  add_executable(BoostSYCL BoostSYCL.cxx)
  add_sycl_to_root_target(TARGET BoostSYCL SOURCES BoostSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING)
  

  if (single_precision)
    add_executable(SInvariantMassesSYCL InvariantMassesSYCL.cxx)
    add_sycl_to_root_target(TARGET SInvariantMassesSYCL SOURCES InvariantMassesSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION) 


    add_executable(SChangeCoordSYCL ChangeCoordSYCL.cxx)
    add_sycl_to_root_target(TARGET SChangeCoordSYCL SOURCES ChangeCoordSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION)
  

    add_executable(SBoostSYCL BoostSYCL.cxx)
    add_sycl_to_root_target(TARGET SBoostSYCL SOURCES BoostSYCL.cxx DEPENDENCIES GenVectorX SYCLMath VecOps INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/inc COMPILE_DEFINITIONS ROOT_MATH_SYCL ROOT_MEAS_TIMING SINGLE_PRECISION)

  endif()

endif()


if (cuda)
  add_executable(InvariantMassesCUDA InvariantMassesCUDA.cu)
  set_target_properties(InvariantMassesCUDA PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(InvariantMassesCUDA PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(InvariantMassesCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(InvariantMassesCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(InvariantMassesCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING)

  add_executable(ChangeCoordCUDA ChangeCoordCUDA.cu)
  set_target_properties(ChangeCoordCUDA PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(ChangeCoordCUDA PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(ChangeCoordCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(ChangeCoordCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(ChangeCoordCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING)

  add_executable(BoostCUDA BoostCUDA.cu)
  set_target_properties(BoostCUDA PROPERTIES LINKER_LANGUAGE CUDA)
  target_link_libraries(BoostCUDA PUBLIC GenVectorX SYCLMath VecOps)
  target_compile_options(BoostCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
  set_target_properties(BoostCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
  target_compile_definitions(BoostCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING)

  if (single_precision)
    add_executable(SInvariantMassesCUDA InvariantMassesCUDA.cu)
    set_target_properties(SInvariantMassesCUDA PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(InvariantMassesCUDA PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SInvariantMassesCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
    set_target_properties(SInvariantMassesCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SInvariantMassesCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING SINGLE_PRECISION)


    add_executable(SChangeCoordCUDA ChangeCoordCUDA.cu)
    set_target_properties(SChangeCoordCUDA PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(SChangeCoordCUDA PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SChangeCoordCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
    set_target_properties(SChangeCoordCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SChangeCoordCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING SINGLE_PRECISION)
  
    add_executable(SBoostCUDA BoostCUDA.cu)
    set_target_properties(SBoostCUDA PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(SBoostCUDA PUBLIC GenVectorX SYCLMath VecOps)
    target_compile_options(SBoostCUDA PRIVATE -rdc=true -lineinfo --expt-relaxed-constexpr)
    set_target_properties(SBoostCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_definitions(SBoostCUDA PUBLIC ROOT_MATH_CUDA ROOT_MEAS_TIMING SINGLE_PRECISION)

  endif()

endif()

add_executable(InvariantMasses InvariantMasses.cxx)
target_compile_definitions(InvariantMasses PUBLIC ROOT_MEAS_TIMING)
#target_link_libraries(InvariantMasses PUBLIC ROOT::MathCore ROOT::ROOTVecOps)


add_executable(Boost Boost.cxx)
target_compile_definitions(Boost PUBLIC ROOT_MEAS_TIMING)
#target_link_libraries(Boost PUBLIC ROOT::MathCore ROOT::GenVector)

if (single_precision)
  add_executable(SInvariantMasses InvariantMasses.cxx)
  target_compile_definitions(SInvariantMasses PUBLIC SINGLE_PRECISION ROOT_MEAS_TIMING)

  add_executable(SBoost Boost.cxx)
  target_compile_definitions(SBoost PUBLIC SINGLE_PRECISION ROOT_MEAS_TIMING)
  #target_link_libraries(SInvariantMasses PUBLIC ROOT::MathCore ROOT::ROOTVecOps)
endif()

#add_executable(ChangeCoord ChangeCoord.cxx)
#target_link_libraries(ChangeCoord PUBLIC ROOT::MathCore ROOT::GenVector)