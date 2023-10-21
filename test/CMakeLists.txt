# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.22)


cmake_path(SET __zivc_dir__ "${CMAKE_SOURCE_DIR}/source/zivc")


function(getTestCompileFlags test_compile_flags test_linker_flags test_definitions)
  set(compile_flags "")
  set(linker_flags "")
  set(definitions "")

  # Output variable
  set(${test_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${test_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${test_definitions} ${definitions} PARENT_SCOPE)
endfunction(getTestCompileFlags)


function(getTestWarningFlags test_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(ZIVC_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags
                               )
      if(Z_VISUAL_STUDIO)
        list(APPEND warning_flags
                                 )
      endif()
    elseif(Z_GCC)
      list(APPEND warning_flags
                               )
    elseif(Z_VISUAL_STUDIO)
      list(APPEND warning_flags
                               /wd4267 # conversion from 'size_t' to 'ArgT', possible loss of data
                               /wd4456 # declaration of 'result' hides previous local declaration
                               /wd6326 # Potential comparison of a constant with another constant
                               /wd6246 # Local declaration of 'result' hides declaration of the same name in outer scope.
                               )
    endif()
  endif()

  # Output variable
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getTestWarningFlags)


#
macro(setTestProject)
  set(project_description "Zivc unit test.")
  project(UnitTest VERSION ${ZivcProject_VERSION} DESCRIPTION ${project_description} LANGUAGES CXX)


  # Initialize platform info
  include("${__zivc_dir__}/cmake/general.cmake")
  include("${__zivc_dir__}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  # Check dependencies
  Zivc_checkTarget(Zivc::Zivc)
  Zivc_checkTarget(CLI11::CLI11)
  Zivc_checkTarget(GTest::gtest)

  # Create a unit test
  file(GLOB_RECURSE unittest_source_files ${PROJECT_SOURCE_DIR}/unittest/*[.hpp|.cpp])
  source_group(${PROJECT_NAME} FILES ${unittest_source_files})

  #
  add_executable(${PROJECT_NAME} ${unittest_source_files})

  # Set properties
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                   CXX_STANDARD_REQUIRED ON)
  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getSanitizerFlags(compile_sanitizer_flags linker_sanitizer_flags)
  getTestCompileFlags(test_compile_flags test_linker_flags test_definitions)
  getTestWarningFlags(test_warning_flags)
  target_compile_options(${PROJECT_NAME} PRIVATE ${test_compile_flags}
                                                 ${cxx_compile_warning_flags}
                                                 ${compile_sanitizer_flags}
                                                 ${test_warning_flags})
  target_link_libraries(${PROJECT_NAME} PRIVATE Zivc::Zivc
                                                CLI11::CLI11
                                                GTest::gtest
                                                ${test_linker_flags})
  target_link_options(${PROJECT_NAME} PRIVATE ${linker_sanitizer_flags})
  target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/unittest")
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${test_definitions})
  Zivc_enableIpo(${PROJECT_NAME})

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  bugprone-implicit-widening-of-multiplication-result
                                  bugprone-narrowing-conversions
                                  misc-non-private-member-variables-in-classes
                                  misc-redundant-expression
                                  modernize-avoid-c-arrays
                                  modernize-loop-convert
                                  modernize-use-auto
                                  readability-function-cognitive-complexity
                                  readability-function-size
                                  readability-identifier-length
                                  readability-implicit-bool-conversion
                                  readability-isolate-declaration
                                  readability-magic-numbers
                                  readability-named-parameter
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zivc_setStaticAnalyzer(${PROJECT_NAME}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
  Zivc_createLinkToTarget(${PROJECT_NAME} ${PROJECT_BINARY_DIR})

  # Installation settings
  include(${__zivc_dir__}/cmake/packaging.cmake)
  ## Save the dependency list
  Zivc_getSaveDependencyCode(${PROJECT_NAME} "${PROJECT_BINARY_DIR}" "" "" "" dependency_code)
  install(CODE ${dependency_code}
          COMPONENT test)
  ## Install actual binary
  install(TARGETS ${PROJECT_NAME}
          DESTINATION "${CMAKE_INSTALL_BINDIR}"
          COMPONENT test)
  ## Dependencies
  Zivc_getDeployingNonSystemDependencyCode(${PROJECT_NAME} "${CMAKE_INSTALL_LIBDIR}" "" "" "" deploy_code)
  install(CODE ${deploy_code}
          COMPONENT test)
  Zivc_getDeployingVulkanDependencyCode("${CMAKE_INSTALL_LIBDIR}" share vulkan_dep_code vulkan_envs)
  install(CODE ${vulkan_dep_code}
          COMPONENT test)
  ## Set vulkan installed directory
  cmake_path(SET vulkan_sdk_dir "\\\${Z_SCRIPT_DIR}/..")
  cmake_path(NATIVE_PATH vulkan_sdk_dir vulkan_sdk_dir)
  list(INSERT vulkan_envs 0 "VULKAN_SDK=${vulkan_sdk_dir}")
  ## Runner script
  Zivc_getMakeSimpleRunScriptCode(${PROJECT_NAME} "../${CMAKE_INSTALL_BINDIR}" "../${CMAKE_INSTALL_LIBDIR}" "${vulkan_envs}" "runUnitTest" "${PROJECT_NAME}" run_script_code)
  install(CODE ${run_script_code}
          COMPONENT test)

  # Add kernel sets
  include("${__zivc_dir__}/cmake/kernel.cmake")
  ## KernelTest1
  cmake_path(SET kernel_set_kernel_test1_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test1")
  file(GLOB_RECURSE kernel_test1_sources ${kernel_set_kernel_test1_dir}/*.cl)
  Zivc_addKernelSet(kernel_test1 ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test1_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTest2
  cmake_path(SET kernel_set_kernel_test2_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test2")
  file(GLOB_RECURSE kernel_test2_sources ${kernel_set_kernel_test2_dir}/*.cl)
  Zivc_addKernelSet(kernel_test2 ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test2_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTest3
  cmake_path(SET kernel_set_kernel_test3_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test3")
  file(GLOB_RECURSE kernel_test3_sources ${kernel_set_kernel_test3_dir}/*.cl)
  Zivc_addKernelSet(kernel_test3 ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test3_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTestParams
  cmake_path(SET kernel_set_kernel_test_params_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_params")
  file(GLOB_RECURSE kernel_test_params_sources ${kernel_set_kernel_test_params_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_params ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test_params_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTestPod
  cmake_path(SET kernel_set_kernel_test_pod_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_pod")
  file(GLOB_RECURSE kernel_test_pod_sources ${kernel_set_kernel_test_pod_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_pod ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test_pod_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTestLocal
  cmake_path(SET kernel_set_kernel_test_local_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_local")
  file(GLOB_RECURSE kernel_test_local_sources ${kernel_set_kernel_test_local_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_local ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test_local_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## kernelTestLocal
  cmake_path(SET kernel_set_kernel_test_local_nv_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_local_nv")
  file(GLOB_RECURSE kernel_test_local_nv_sources ${kernel_set_kernel_test_local_nv_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_local_nv ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${kernel_test_local_nv_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ test1
  cmake_path(SET kernel_set_cl_cpp_test1_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test1")
  file(GLOB_RECURSE cl_cpp_test1_sources ${kernel_set_cl_cpp_test1_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test1 ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test1_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ type test
  cmake_path(SET kernel_set_cl_cpp_test_type_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_type")
  file(GLOB_RECURSE cl_cpp_test_type_sources ${kernel_set_cl_cpp_test_type_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_type ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_type_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ geometry
  cmake_path(SET kernel_set_cl_cpp_test_geometry_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_geometry")
  file(GLOB_RECURSE cl_cpp_test_geometry_sources ${kernel_set_cl_cpp_test_geometry_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_geometry ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_geometry_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ relation
  cmake_path(SET kernel_set_cl_cpp_test_relation_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_relation")
  file(GLOB_RECURSE cl_cpp_test_relation_sources ${kernel_set_cl_cpp_test_relation_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_relation ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_relation_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ atomic
  cmake_path(SET kernel_set_cl_cpp_test_atomic_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_atomic")
  file(GLOB_RECURSE cl_cpp_test_atomic_sources ${kernel_set_cl_cpp_test_atomic_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_atomic ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_atomic_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ atomic
  cmake_path(SET kernel_set_cl_cpp_test_atomic2_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_atomic_compare_and_exchange")
  file(GLOB_RECURSE cl_cpp_test_atomic_sources2 ${kernel_set_cl_cpp_test_atomic2_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_atomic_compare_and_exchange ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_atomic_sources2})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ math
  cmake_path(SET kernel_set_cl_cpp_test_math_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_math")
  file(GLOB_RECURSE cl_cpp_test_math_sources ${kernel_set_cl_cpp_test_math_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_math ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_math_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ math power
  cmake_path(SET kernel_set_cl_cpp_test_math_power_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_math_power")
  file(GLOB_RECURSE cl_cpp_test_math_sources ${kernel_set_cl_cpp_test_math_power_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_math_power ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_math_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ math trigonometric
  cmake_path(SET kernel_set_cl_cpp_test_math_trigonometric_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_math_trigonometric")
  file(GLOB_RECURSE cl_cpp_test_math_sources ${kernel_set_cl_cpp_test_math_trigonometric_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_math_trigonometric ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_math_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})
  ## opencl C++ math float
  cmake_path(SET kernel_set_cl_cpp_test_math_float_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_math_float")
  file(GLOB_RECURSE cl_cpp_test_math_sources ${kernel_set_cl_cpp_test_math_float_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_math_float ${PROJECT_VERSION} kernel_name
      SOURCE_FILES ${cl_cpp_test_math_sources})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${kernel_name})


  # Add tests for CMake
  add_test(NAME ${PROJECT_NAME}-cpu COMMAND ${PROJECT_NAME} --device cpu
                                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan0-debug
               COMMAND ${PROJECT_NAME} --device vulkan0
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan0-math
               COMMAND ${PROJECT_NAME} --device vulkan0
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan0-nodebug
               COMMAND ${PROJECT_NAME} --device vulkan0 --nodebug
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan0-full
               COMMAND ${PROJECT_NAME} --device vulkan0
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan1-debug
               COMMAND ${PROJECT_NAME} --device vulkan1
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan1-math
               COMMAND ${PROJECT_NAME} --device vulkan1
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan1-nodebug
               COMMAND ${PROJECT_NAME} --device vulkan1 --nodebug
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan1-full
               COMMAND ${PROJECT_NAME} --device vulkan1
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan2-debug
               COMMAND ${PROJECT_NAME} --device vulkan2
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan2-math
               COMMAND ${PROJECT_NAME} --device vulkan2
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan2-nodebug
               COMMAND ${PROJECT_NAME} --device vulkan2 --nodebug
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan2-full
               COMMAND ${PROJECT_NAME} --device vulkan2
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan3-debug
               COMMAND ${PROJECT_NAME} --device vulkan3
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan3-math
               COMMAND ${PROJECT_NAME} --device vulkan3
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan3-nodebug
               COMMAND ${PROJECT_NAME} --device vulkan3 --nodebug
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan3-full
               COMMAND ${PROJECT_NAME} --device vulkan3
               WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  set(test_env_list "")
  list(APPEND test_env_list GTEST_COLOR=1
                            ZIVC_SUPPRESS_TRIVIAL_WARNINGS=1)
  set_tests_properties(${PROJECT_NAME}-cpu
                       ${PROJECT_NAME}-vulkan0-full
                       ${PROJECT_NAME}-vulkan1-full
                       ${PROJECT_NAME}-vulkan2-full
                       ${PROJECT_NAME}-vulkan3-full
                       PROPERTIES ENVIRONMENT "${test_env_list}")
  # \todo Resolve the AtomicCompareAndExchangeTest
  set(gtest_filter "GTEST_FILTER=-*AllocationExceptionTest*:*AtomicCompareExchangeTest*")
  set(env_list ${test_env_list} ${gtest_filter})
  set_tests_properties(${PROJECT_NAME}-vulkan0-math
                       ${PROJECT_NAME}-vulkan1-math
                       ${PROJECT_NAME}-vulkan2-math
                       ${PROJECT_NAME}-vulkan3-math
                       PROPERTIES ENVIRONMENT "${env_list}")
  set(gtest_filter "${gtest_filter}:*Geometry*Cross*:*Geometry*Distance*:*MathImpl*:*Math*Pow*")
  set(env_list ${test_env_list} ${gtest_filter})
  set_tests_properties(${PROJECT_NAME}-vulkan0-debug
                       ${PROJECT_NAME}-vulkan0-nodebug
                       ${PROJECT_NAME}-vulkan1-debug
                       ${PROJECT_NAME}-vulkan1-nodebug
                       ${PROJECT_NAME}-vulkan2-debug
                       ${PROJECT_NAME}-vulkan2-nodebug
                       ${PROJECT_NAME}-vulkan3-debug
                       ${PROJECT_NAME}-vulkan3-nodebug
                       PROPERTIES ENVIRONMENT "${env_list}")

  # Copy test files
  cmake_path(SET dst_resource_dir "${PROJECT_BINARY_DIR}/resources")
  cmake_path(SET zisc_dir NORMALIZE ${__zivc_dir__}/../dependencies/Zisc)
  file(GLOB math_reference_files "${zisc_dir}/test/math_reference/reference/*.7z")
  foreach(reference_path IN LISTS math_reference_files)
    cmake_path(SET src_file_path NORMALIZE "${reference_path}")
    cmake_path(GET src_file_path STEM LAST_ONLY reference_name)
    cmake_path(SET dst_file_path "${dst_resource_dir}/${reference_name}")
    if("${src_file_path}" IS_NEWER_THAN "${dst_file_path}")
      file(ARCHIVE_EXTRACT INPUT "${src_file_path}"
                           DESTINATION "${dst_resource_dir}")
      file(TOUCH_NOCREATE "${dst_file_path}") 
    endif()
  endforeach(reference_path)
endmacro(setTestProject)


##
setTestProject()
