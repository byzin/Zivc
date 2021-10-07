# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.17)


set(__zisc_dir__ ${CMAKE_SOURCE_DIR}/source/dependencies/Zisc/source/zisc)


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
      list(APPEND warning_flags -Wno-covered-switch-default
                                -Wno-global-constructors
                                -Wno-sign-conversion
                                -Wno-suggest-override
                                -Wno-used-but-marked-unused
                                )
      if(Z_VISUAL_STUDIO)
        list(APPEND warning_flags -Wno-implicit-float-conversion
                                  -Wno-unused-result
                                  )
      endif()
    elseif(Z_GCC)
      list(APPEND warning_flags -Wno-attributes
                                -Wno-noexcept
                                -Wno-sign-conversion
                                -Wno-strict-overflow
                                -Wno-unused-but-set-variable
                                )
    endif()
  endif()

  # Output variable
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getTestWarningFlags)


#
set(project_description "Zivc unit test.")
project(UnitTest VERSION 0.0.1 DESCRIPTION ${project_description} LANGUAGES CXX)


# Initialize platform info
include(${__zisc_dir__}/cmake/general.cmake)
include(${__zisc_dir__}/cmake/platform.cmake)
Zisc_getPlatformFlags(platform_definitions)
Zisc_setVariablesOnCMake(${platform_definitions})

# Check dependencies
Zisc_checkTarget(Zivc)
Zisc_checkTarget(gtest_main)

# Create a unit test
file(GLOB unittest_source_files ${PROJECT_SOURCE_DIR}/unittest/*.cpp
                                ${PROJECT_SOURCE_DIR}/unittest/*.hpp)
add_executable(${PROJECT_NAME} ${unittest_source_files})
source_group(${PROJECT_NAME} FILES ${unittest_source_files})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

include(${__zisc_dir__}/cmake/compiler.cmake)
Zisc_getCxxWarningFlags(cxx_compile_warning_flags)
getTestCompileFlags(test_compile_flags test_linker_flags test_definitions)
getTestWarningFlags(test_warning_flags)
target_compile_options(${PROJECT_NAME} PRIVATE ${test_compile_flags}
                                               ${cxx_compile_warning_flags}
                                               ${test_warning_flags})
target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11
                                              gtest_main
                                              Zivc
                                              ${test_linker_flags})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${test_definitions})
Zisc_enableIpo(${PROJECT_NAME})

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
                                readability-implicit-bool-conversion
                                readability-isolate-declaration
                                readability-magic-numbers
                                readability-named-parameter
                                readability-static-accessed-through-instance
                                readability-uppercase-literal-suffix)
Zisc_setStaticAnalyzer(${PROJECT_NAME}
                       CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
Zisc_createLinkToTarget(${PROJECT_NAME} ${PROJECT_BINARY_DIR})

# Save the dependency list
Zisc_getSaveDependencyCode(${PROJECT_NAME} ${PROJECT_BINARY_DIR} "" "" "" dependency_code)
install(CODE ${dependency_code} COMPONENT test)
# Install actual binary
install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT test)

# Add kernel sets
include(${CMAKE_SOURCE_DIR}/source/zivc/cmake/kernel.cmake)
## KernelTest
set(kernel_set_kernel_test_dir ${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test)
file(GLOB_RECURSE kernel_test_sources ${kernel_set_kernel_test_dir}/*.cl)
Zivc_addKernelSet(kernel_test ${PROJECT_VERSION}
    SOURCE_FILES ${kernel_test_sources}
    INCLUDE_DIRS ${kernel_set_kernel_test_dir})
target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test)
## kernelTest2
set(kernel_set_kernel_test2_dir ${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test2)
file(GLOB_RECURSE kernel_test2_sources ${kernel_set_kernel_test2_dir}/*.cl)
# TODO remove the mac issue definition
if(Z_MAC)
  set(kernel_test2_mac_definitions ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
endif()
Zivc_addKernelSet(kernel_test2 ${PROJECT_VERSION}
    SOURCE_FILES ${kernel_test2_sources}
    INCLUDE_DIRS ${kernel_set_kernel_test2_dir}
    DEFINITIONS ${kernel_test2_mac_definitions})
target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test2)
## kernelTestPod
set(kernel_set_kernel_test_pod_dir ${PROJECT_SOURCE_DIR}/unittest/kernels/kernel_test_pod)
file(GLOB_RECURSE kernel_test_pod_sources ${kernel_set_kernel_test_pod_dir}/*.cl)
Zivc_addKernelSet(kernel_test_pod ${PROJECT_VERSION}
    SOURCE_FILES ${kernel_test_pod_sources}
    INCLUDE_DIRS ${kernel_set_kernel_test_pod_dir})
target_link_libraries(${PROJECT_NAME} PRIVATE KernelSet_kernel_test_pod)

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
