# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
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
      list(APPEND warning_flags -Wno-float-equal
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
function(addExample target save_prerequisites)
  set(example_source_files ${ARGN})
  add_executable(${target} ${example_source_files})
  source_group(${PROJECT_NAME} FILES ${example_source_files})

  #
  set_target_properties(${target} PROPERTIES CXX_STANDARD 17
                                             CXX_STANDARD_REQUIRED ON)

  include(${__zisc_dir__}/cmake/compiler.cmake)
  Zisc_getCxxWarningFlags(cxx_compile_warning_flags)
  getExampleWarningFlags(example_warning_flags)
  target_compile_options(${target} PRIVATE ${cxx_compile_warning_flags}
                                           ${example_warning_flags})
  target_link_libraries(${target} PRIVATE Zivc)
  Zisc_enableIpo(${target})

  #
  Zisc_setStaticAnalyzer(${target})
  Zisc_createLinkToTarget(${target} ${PROJECT_BINARY_DIR})

  # Save the dependency list
  if(save_prerequisites)
    Zisc_savePrerequisites(${target} ${PROJECT_BINARY_DIR} "" "" "")
  endif()
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
    SOURCE_FILES ${PROJECT_SOURCE_DIR}/kernels/example.cl)
target_link_libraries(KernelExample PRIVATE KernelSet_example)
