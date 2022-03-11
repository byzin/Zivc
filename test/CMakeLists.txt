# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
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


function(addGoogleTest binary_dir)
  cmake_path(SET zivc_path "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  include("${zivc_path}/cmake/compiler.cmake")
  #
  cmake_path(SET gtest_source_path "${PROJECT_SOURCE_DIR}/dependencies/googletest")
  Zivc_addGoogleTest("${gtest_source_path}" "${binary_dir}")
  Zivc_setRequiredCxxFlagsToTarget(gtest)
  Zivc_setRequiredCxxFlagsToTarget(gtest_main)
endfunction(addGoogleTest)


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
                               )
    endif()
  endif()

  # Output variable
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getTestWarningFlags)


#
macro(setTestProject)
  set(project_description "Zivc unit test.")
  project(UnitTest VERSION 0.0.1 DESCRIPTION ${project_description} LANGUAGES CXX)


  # Initialize platform info
  include("${__zivc_dir__}/cmake/general.cmake")
  include("${__zivc_dir__}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  # Check dependencies
  Zivc_checkTarget(Zivc)
  Zivc_checkTarget(CLI11::CLI11)
  addGoogleTest("${PROJECT_BINARY_DIR}/GoogleTest")
  Zivc_checkTarget(gtest_main)

  # Create a unit test
  file(GLOB_RECURSE unittest_source_files ${PROJECT_SOURCE_DIR}/unittest/*[.hpp|.cpp])
  add_executable(${PROJECT_NAME} ${unittest_source_files})
  source_group(${PROJECT_NAME} FILES ${unittest_source_files})

  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                   CXX_STANDARD_REQUIRED ON)

  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  getTestCompileFlags(test_compile_flags test_linker_flags test_definitions)
  getTestWarningFlags(test_warning_flags)
  target_compile_options(${PROJECT_NAME} PRIVATE ${test_compile_flags}
                                                 ${cxx_compile_warning_flags}
                                                 ${test_warning_flags})
  target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11
                                                gtest_main
                                                Zivc::Zivc
                                                ${test_linker_flags})
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
  Zivc_addKernelSet(kernel_test1 ${PROJECT_VERSION}
      SOURCE_FILES ${kernel_test1_sources}
      INCLUDE_DIRS "${kernel_set_kernel_test_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test1)
  ## kernelTest2
  cmake_path(SET kernel_set_kernel_test2_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test2")
  file(GLOB_RECURSE kernel_test2_sources ${kernel_set_kernel_test2_dir}/*.cl)
  Zivc_addKernelSet(kernel_test2 ${PROJECT_VERSION}
      SOURCE_FILES ${kernel_test2_sources}
      INCLUDE_DIRS "${kernel_set_kernel_test2_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test2)
  ## kernelTestParams
  cmake_path(SET kernel_set_kernel_test_params_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_params")
  file(GLOB_RECURSE kernel_test_params_sources ${kernel_set_kernel_test_params_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_params ${PROJECT_VERSION}
      SOURCE_FILES ${kernel_test_params_sources}
      INCLUDE_DIRS "${kernel_set_kernel_test_params_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test_params)
  ## kernelTestPod
  cmake_path(SET kernel_set_kernel_test_pod_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_pod")
  file(GLOB_RECURSE kernel_test_pod_sources ${kernel_set_kernel_test_pod_dir}/*.cl)
  Zivc_addKernelSet(kernel_test_pod ${PROJECT_VERSION}
      SOURCE_FILES ${kernel_test_pod_sources}
      INCLUDE_DIRS "${kernel_set_kernel_test_pod_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test_pod)
  ## opencl C++ test1
  cmake_path(SET kernel_set_cl_cpp_test1_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test1")
  file(GLOB_RECURSE cl_cpp_test1_sources ${kernel_set_cl_cpp_test1_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test1 ${PROJECT_VERSION}
      SOURCE_FILES ${cl_cpp_test1_sources}
      INCLUDE_DIRS "${kernel_set_cl_cpp_test1_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_cl_cpp_test1)
  ## opencl C++ type test
  cmake_path(SET kernel_set_cl_cpp_test_type_dir "${PROJECT_SOURCE_DIR}/unittest/kernels/cl_cpp_test_type")
  file(GLOB_RECURSE cl_cpp_test_type_sources ${kernel_set_cl_cpp_test_type_dir}/*.cl)
  Zivc_addKernelSet(cl_cpp_test_type ${PROJECT_VERSION}
      SOURCE_FILES ${cl_cpp_test_type_sources}
      INCLUDE_DIRS "${kernel_set_cl_cpp_test_type_dir}")
  target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_cl_cpp_test_type)

  # Add tests for CMake
  add_test(NAME ${PROJECT_NAME}-cpu COMMAND ${PROJECT_NAME} --device cpu
                                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan0 COMMAND ${PROJECT_NAME} --device vulkan0
                                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan1 COMMAND ${PROJECT_NAME} --device vulkan1
                                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan2 COMMAND ${PROJECT_NAME} --device vulkan2
                                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(NAME ${PROJECT_NAME}-vulkan3 COMMAND ${PROJECT_NAME} --device vulkan3
                                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  set(test_env_list "")
  list(APPEND test_env_list GTEST_COLOR=1
                            ZIVC_SUPPRESS_TRIVIAL_WARNINGS=1)
  set_tests_properties(${PROJECT_NAME}-cpu
                       ${PROJECT_NAME}-vulkan0
                       ${PROJECT_NAME}-vulkan1
                       ${PROJECT_NAME}-vulkan2
                       ${PROJECT_NAME}-vulkan3
                       PROPERTIES ENVIRONMENT "${test_env_list}")
endmacro(setTestProject)


##
setTestProject()
