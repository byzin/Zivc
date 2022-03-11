# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 



# Build CLI11
function(addCli11 binary_dir)
  cmake_path(SET zivc_path "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  # Add CLI11
  Zivc_setInternalValue(CLI11_WARNINGS_AS_ERRORS OFF)
  Zivc_setInternalValue(CLI11_SINGLE_FILE OFF)
  Zivc_setInternalValue(CLI11_SANITIZERS OFF)
  Zivc_setInternalValue(CLI11_BUILD_DOCS OFF)
  Zivc_setInternalValue(CLI11_BUILD_TESTS OFF)
  Zivc_setInternalValue(CLI11_BUILD_EXAMPLES OFF)
  Zivc_setInternalValue(CLI11_BUILD_EXAMPLES_JSON OFF)
  Zivc_setInternalValue(CLI11_SINGLE_FILE_TESTS OFF)
  Zivc_setInternalValue(CLI11_INSTALL OFF)
  Zivc_setInternalValue(CLI11_FORCE_LIBCXX OFF)
  Zivc_setInternalValue(CLI11_CUDA_TESTS OFF)
  Zivc_setInternalValue(CLI11_CLANG_TIDY_OPTIONS "")
  cmake_path(SET cli11_source_path NORMALIZE
      "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../example/dependencies/CLI11")
  Zivc_checkSubmodule("${cli11_source_path}")
  add_subdirectory("${cli11_source_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  # Check the target properties
  Zivc_checkTarget(CLI11::CLI11)
endfunction(addCli11)
