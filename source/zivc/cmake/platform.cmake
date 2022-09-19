# file: platform.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


function(Zivc_getPlatformFlags platform_definitions)
  cmake_path(SET zisc_dir NORMALIZE
      "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../dependencies/Zisc/source/zisc")
  cmake_path(SET zisc_platform_path NORMALIZE "${zisc_dir}/cmake/platform.cmake")
  include("${zisc_platform_path}")
  Zisc_getPlatformFlags(platform_defs)

  # Output
  set(${platform_definitions} ${platform_defs} PARENT_SCOPE)
endfunction(Zivc_getPlatformFlags)


# Build clspv
function(Zivc_addClspv binary_dir)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET clspv_source_path NORMALIZE "${zivc_path}/../dependencies/clspv")
  #
  if(NOT TARGET Python3::Interpreter)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
  endif()
  message(STATUS "Fetching clspv dependencies ... ")
  execute_process(
    COMMAND ${Python3_EXECUTABLE} "${clspv_source_path}/utils/fetch_sources.py"
    RESULT_VARIABLE fetch_result
    OUTPUT_VARIABLE fetch_output
    ERROR_VARIABLE fetch_error)
  if(${fetch_result} EQUAL 0)
    message("        completed.")
  else()
    message(FATAL_ERROR "    failed.\n${fetch_error}")
  endif()

  # Add clspv
  unset(CMAKE_CONFIGURATION_TYPES)
  set(SKIP_CLSPV_TOOLS_INSTALL ON)
  set(SKIP_SPIRV_TOOLS_INSTALL ON)
  Zivc_checkSubmodule("${clspv_source_path}")
  add_subdirectory("${clspv_source_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(clspv)
endfunction(Zivc_addClspv)
