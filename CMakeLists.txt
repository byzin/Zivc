# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.17)


project(ZivcProject VERSION 0.0.1 LANGUAGES CXX)

## Cmake configurations
set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(InstallRequiredSystemLibraries)

set(zisc_path ${PROJECT_SOURCE_DIR}/source/dependencies/Zisc/source/zisc)
include(${zisc_path}/cmake/general.cmake)
message(STATUS "System information:")
Zisc_printSystemInfo("  + ")
## Project options
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initProjectOptions()
Zisc_restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)

# Build Zivc
set(zivc_path ${PROJECT_SOURCE_DIR}/source/zivc)
add_subdirectory(${zivc_path} ${PROJECT_BINARY_DIR}/Zivc)

# Build examples
add_subdirectory(${PROJECT_SOURCE_DIR}/example ${PROJECT_BINARY_DIR}/Examples)

# Build unit test
if(ZIVC_BUILD_TESTS)
  addZiscGoogleTest()
  add_subdirectory(${PROJECT_SOURCE_DIR}/test ${PROJECT_BINARY_DIR}/UnitTest)
endif()

# Build documents
add_subdirectory(${PROJECT_SOURCE_DIR}/document ${PROJECT_BINARY_DIR}/Document)
