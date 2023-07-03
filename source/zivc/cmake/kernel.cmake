# file: kernel.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


function(Zivc_initZivcKernelOptions)
  include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake")

  set(option_description "Use precision cl math implementation instead of the built-in cl math.")
  Zivc_setBooleanOption(ZIVC_MATH_PRECISION OFF ${option_description})
endfunction(Zivc_initZivcKernelOptions)


function(Zivc_getZivcKernelFlags zivc_compile_flags zivc_definitions)
  set(compile_flags "")
  set(definitions "")

  # Math
  if(ZIVC_MATH_PRECISION)
    list(APPEND definitions ZIVC_MATH_PRECISION=1)
  endif()

  # Output variables
  set(${zivc_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zivc_definitions} ${definitions} PARENT_SCOPE)
endfunction(Zivc_getZivcKernelFlags)


function(Zivc_issueKernelSetNumber kernel_set_name number)
  string(SHA3_256 kernel_set_number "${kernel_set_name}")
  string(SUBSTRING "${kernel_set_number}" 0 16 kernel_set_number)

  # Output
  set(${number} ${kernel_set_number} PARENT_SCOPE)
endfunction(Zivc_issueKernelSetNumber)


function(Zivc_addKernelSet kernel_set_name kernel_set_version target)
  set(target_name KernelSet_${kernel_set_name})
  if(TARGET ${target_name})
    return()
  else()
    message(STATUS "Add a kernel set '${kernel_set_name}'.")
  endif()

  # Parse arguments
  set(options "")
  set(one_value_args SPIRV_VERSION)
  set(multi_value_args SOURCE_FILES INCLUDE_DIRS DEFINITIONS DEPENDS)
  cmake_parse_arguments(PARSE_ARGV 2 ZIVC "${options}" "${one_value_args}" "${multi_value_args}")

  # Check source files
  if(NOT ZIVC_SOURCE_FILES)
    message(FATAL_ERROR "The kernel set '${kernel_set_name}' has no source.")
  endif()

  # Set kernel properties
  Zivc_issueKernelSetNumber("${kernel_set_name}" kernel_set_number)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_include_dir "${zivc_path}/core/include")
  file(REAL_PATH "${zivc_path}" zivc_path)
  cmake_path(SET kernel_set_template_dir "${zivc_path}/template")
  cmake_path(SET kernel_set_base_dir "${CMAKE_CURRENT_SOURCE_DIR}")
  set(kernel_set_source_files ${ZIVC_SOURCE_FILES})
  set(kernel_set_include_dirs "${zivc_include_dir}" "${ZIVC_INCLUDE_DIRS}")
  set(kernel_set_definitions ${ZIVC_DEFINITIONS})
  set(kernel_set_depends ${ZIVC_DEPENDS})
  # SPIR-V version
  if(ZIVC_SPIRV_VERSION)
    set(kernel_set_spirv_version ${ZIVC_SPIRV_VERSION})
  else()
    set(kernel_set_spirv_version ${ZIVC_DEFAULT_SPIRV_VERSION})
  endif()
  set(supported_spirv_versions 1.0 1.3 1.5)
  if(NOT (kernel_set_spirv_version IN_LIST supported_spirv_versions))
    message(WARNING "The specified SPIR-V version '${kernel_set_spirv_version}' isn't supported. Use '1.5' instead.")
    set(kernel_set_spirv_version 1.5)
  endif()

  # Make a CMakeLists.txt of the given kernel set
  cmake_path(SET kernel_set_dir "${PROJECT_BINARY_DIR}/KernelSet/${kernel_set_name}")
  file(MAKE_DIRECTORY "${kernel_set_dir}")
  configure_file("${kernel_set_template_dir}/kernel_set.cmake.in"
                 "${kernel_set_dir}/CMakeLists.txt"
                 @ONLY)
  add_subdirectory("${kernel_set_dir}" "${kernel_set_dir}/build")

  # Check target
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake)
  Zivc_checkTarget(${target_name})

  # Output
  set(${target} ${target_name} PARENT_SCOPE)
endfunction(Zivc_addKernelSet)
