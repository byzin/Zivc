# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.22)


project(ZivcProject VERSION 0.0.1 LANGUAGES CXX)

cmake_path(SET zivc_path "${PROJECT_SOURCE_DIR}/source/zivc")
if(PROJECT_IS_TOP_LEVEL)
  ## Cmake configurations
  set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  include(InstallRequiredSystemLibraries)
  # Print system environment information
  include("${zivc_path}/cmake/general.cmake")
  message(STATUS "System information:")
  Zivc_printSystemInfo("  + ")
  ## Project options
  include("${PROJECT_SOURCE_DIR}/cmake/option.cmake")
  initProjectOptions()
  Zivc_restrictBuildDirectory("${PROJECT_SOURCE_DIR}/build")

  # Add dependencies
  include("${zivc_path}/cmake/platform.cmake")
  include("${PROJECT_SOURCE_DIR}/cmake/project.cmake")
  Zivc_addCpuFeatures("${PROJECT_BINARY_DIR}/CpuFeatures")
  Zivc_addZstd("${PROJECT_BINARY_DIR}/Zstd")
  addCli11("${PROJECT_BINARY_DIR}/CLI11")
endif()

# Build Zivc
add_subdirectory("${zivc_path}" "${PROJECT_BINARY_DIR}/Zivc")

# Build examples
if(PROJECT_IS_TOP_LEVEL AND ZIVC_BUILD_EXAMPLES)
  add_subdirectory("${PROJECT_SOURCE_DIR}/example" "${PROJECT_BINARY_DIR}/Examples")
endif()

# Build unit test
if(PROJECT_IS_TOP_LEVEL AND ZIVC_BUILD_TESTS)
  enable_testing()
  add_subdirectory("${PROJECT_SOURCE_DIR}/test" "${PROJECT_BINARY_DIR}/UnitTest")
endif()

if(PROJECT_IS_TOP_LEVEL)
  # Build documents
  add_subdirectory("${PROJECT_SOURCE_DIR}/document" "${PROJECT_BINARY_DIR}/Document")

  # Packaging
  include("${PROJECT_SOURCE_DIR}/cmake/packaging.cmake")
  packageProject()
endif()
