# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.17)


set(__zisc_dir__ ${CMAKE_SOURCE_DIR}/source/dependencies/Zisc/source/zisc)


function(getExampleWarningFlags example_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(ZIVC_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags -Wno-covered-switch-default
                                # -Wno-padded
                                -Wno-sign-conversion
                                )
    elseif(Z_GCC)
      list(APPEND warning_flags -Wno-sign-conversion
                                )
    endif()
  endif()

  set(${example_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getExampleWarningFlags)


# Add an example
function(addExample target save_dependency_list)
  set(example_source_files ${ARGN})
  add_executable(${target} ${example_source_files})
  source_group(${PROJECT_NAME} FILES ${example_source_files})

  #
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20
                                             CXX_STANDARD_REQUIRED ON)

  include(${__zisc_dir__}/cmake/compiler.cmake)
  Zisc_getCxxWarningFlags(cxx_compile_warning_flags)
  getExampleWarningFlags(example_warning_flags)
  target_compile_options(${target} PRIVATE ${cxx_compile_warning_flags}
                                           ${example_warning_flags})
  target_link_libraries(${target} PRIVATE Zivc)
  Zisc_enableIpo(${target})

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  bugprone-implicit-widening-of-multiplication-result
                                  bugprone-narrowing-conversions
                                  misc-non-private-member-variables-in-classes
                                  misc-redundant-expression
                                  modernize-avoid-c-arrays
                                  modernize-use-auto
                                  readability-function-cognitive-complexity
                                  readability-isolate-declaration
                                  readability-magic-numbers
                                  readability-named-parameter
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zisc_setStaticAnalyzer(${target}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
  Zisc_createLinkToTarget(${target} ${PROJECT_BINARY_DIR})

  # Installation settings
  # Save the dependency list
  if(save_dependency_list)
    Zisc_getSaveDependencyCode(${target} ${PROJECT_BINARY_DIR} "" "" "" dependency_code)
    install(CODE ${dependency_code} COMPONENT test)
  endif()
  install(TARGETS ${target} DESTINATION bin COMPONENT example)
endfunction(addExample)


#
set(project_description "Examples of Zivc")
project(Examples VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES  CXX)

# Initialize platform info
include(${__zisc_dir__}/cmake/general.cmake)
include(${__zisc_dir__}/cmake/platform.cmake)
Zisc_getPlatformFlags(platform_definitions)
Zisc_setVariablesOnCMake(${platform_definitions})

# Check dependencies
Zisc_checkTarget(Zivc)

# Build examples
addExample(PlatformExample ON
           ${PROJECT_SOURCE_DIR}/platform_example.cpp)
addExample(VulkanSubPlatformExample OFF
           ${PROJECT_SOURCE_DIR}/vulkan_sub_platform_example.cpp)
addExample(DeviceExample OFF
           ${PROJECT_SOURCE_DIR}/device_example.cpp)
addExample(KernelExample OFF
           ${PROJECT_SOURCE_DIR}/kernel_example.cpp)

# Kernel set
include(${CMAKE_SOURCE_DIR}/source/zivc/cmake/kernel.cmake)
Zivc_addKernelSet(example ${PROJECT_VERSION}
    SOURCE_FILES kernels/example.cl
    INCLUDE_DIRS kernels)
target_link_libraries(KernelExample PRIVATE KernelSet_example)
