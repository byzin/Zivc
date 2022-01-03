# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.21)


project(ZivcProject VERSION 0.0.1 LANGUAGES CXX)

## Cmake configurations
set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(InstallRequiredSystemLibraries)

include(${PROJECT_SOURCE_DIR}/source/zivc/cmake/general.cmake)
message(STATUS "System information:")
Zivc_printSystemInfo("  + ")
## Project options
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initProjectOptions()
Zivc_restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)

# Build Zivc
set(zivc_path ${PROJECT_SOURCE_DIR}/source/zivc)
add_subdirectory(${zivc_path} ${PROJECT_BINARY_DIR}/Zivc)

# Add dependencies used for both example and test
if(ZIVC_BUILD_EXAMPLES OR ZIVC_BUILD_TESTS)
  # CLI11
  add_subdirectory(${PROJECT_SOURCE_DIR}/example/dependencies/CLI11
                   ${PROJECT_BINARY_DIR}/CLI11
                   EXCLUDE_FROM_ALL)
endif()

# Build examples
if(ZIVC_BUILD_EXAMPLES)
  add_subdirectory(${PROJECT_SOURCE_DIR}/example ${PROJECT_BINARY_DIR}/Examples)
endif()

# Build unit test
if(ZIVC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(${PROJECT_SOURCE_DIR}/test ${PROJECT_BINARY_DIR}/UnitTest)
endif()

# Build documents
add_subdirectory(${PROJECT_SOURCE_DIR}/document ${PROJECT_BINARY_DIR}/Document)

# Packaging
include(${PROJECT_SOURCE_DIR}/cmake/packaging.cmake)
packageProject()
