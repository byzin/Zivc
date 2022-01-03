# file: kernel.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


function(Zivc_initZivcKernelOptions)
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake)

  set(option_description "Use built-in math funcs instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN OFF ${option_description})

  set(option_description "Use built-in 'frexp' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_FREXP OFF ${option_description})

  set(option_description "Use built-in 'ldexp' instead of the Zivc funcs.")
  if(Z_MAC)
    Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_LDEXP OFF ${option_description})
  else()
    Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_LDEXP ON ${option_description})
  endif()

  set(option_description "Use built-in 'ilogb' instead of the Zivc func.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_ILOGB OFF ${option_description})

  set(option_description "Use built-in 'modf' instead of the Zivc func.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_MODF OFF ${option_description})

  set(option_description "Use built-in 'round' and 'rint' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_ROUND OFF ${option_description})

  set(option_description "Use built-in 'fmod' instead of the Zivc func.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_FMOD ON ${option_description})

  set(option_description "Use built-in 'exp' and 'exp2' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_EXP ON ${option_description})

  set(option_description "Use built-in 'log' and 'log2' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_LOG ON ${option_description})

  set(option_description "Use built-in 'pow' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_POW ON ${option_description})

  set(option_description "Use built-in 'sqrt' and 'rsqrt' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_SQRT ON ${option_description})

  set(option_description "Use built-in 'cbrt' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_CBRT OFF ${option_description})

  set(option_description "Use built-in 'sin', 'cos' and 'tan' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_TRIGONOMETRIC ON ${option_description})

  set(option_description "Use built-in 'asin', 'acos' and 'atan' instead of the Zivc funcs.")
  Zivc_setBooleanOption(ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC ON ${option_description})
endfunction(Zivc_initZivcKernelOptions)


function(Zivc_getZivcKernelFlags zivc_compile_flags zivc_definitions)
  set(compile_flags "")
  set(definitions "")

  # Math
  if(ZIVC_MATH_BUILTIN)
    list(APPEND definitions ZIVC_MATH_BUILTIN=1)
  endif()
  if(ZIVC_MATH_BUILTIN_FREXP)
    list(APPEND definitions ZIVC_MATH_BUILTIN_FREXP=1)
  endif()
  if(ZIVC_MATH_BUILTIN_LDEXP)
    list(APPEND definitions ZIVC_MATH_BUILTIN_LDEXP=1)
  endif()
  if(ZIVC_MATH_BUILTIN_ILOGB)
    list(APPEND definitions ZIVC_MATH_BUILTIN_ILOGB=1)
  endif()
  if(ZIVC_MATH_BUILTIN_MODF)
    list(APPEND definitions ZIVC_MATH_BUILTIN_MODF=1)
  endif()
  if(ZIVC_MATH_BUILTIN_ROUND)
    list(APPEND definitions ZIVC_MATH_BUILTIN_ROUND=1)
  endif()
  if(ZIVC_MATH_BUILTIN_FMOD)
    list(APPEND definitions ZIVC_MATH_BUILTIN_FMOD=1)
  endif()
  if(ZIVC_MATH_BUILTIN_EXP)
    list(APPEND definitions ZIVC_MATH_BUILTIN_EXP=1)
  endif()
  if(ZIVC_MATH_BUILTIN_LOG)
    list(APPEND definitions ZIVC_MATH_BUILTIN_LOG=1)
  endif()
  if(ZIVC_MATH_BUILTIN_POW)
    list(APPEND definitions ZIVC_MATH_BUILTIN_POW=1)
  endif()
  if(ZIVC_MATH_BUILTIN_SQRT)
    list(APPEND definitions ZIVC_MATH_BUILTIN_SQRT=1)
  endif()
  if(ZIVC_MATH_BUILTIN_CBRT)
    list(APPEND definitions ZIVC_MATH_BUILTIN_CBRT=1)
  endif()
  if(ZIVC_MATH_BUILTIN_TRIGONOMETRIC)
    list(APPEND definitions ZIVC_MATH_BUILTIN_TRIGONOMETRIC=1)
  endif()
  if(ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC)
    list(APPEND definitions ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC=1)
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


function(Zivc_addKernelSet kernel_set_name kernel_set_version)
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
  set(zivc_path ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..)
  set(zivc_core_dir ${zivc_path}/core)
  get_filename_component(zivc_path "${zivc_path}" REALPATH)
  set(kernel_set_template_dir ${zivc_path}/template)
  set(kernel_set_base_dir ${CMAKE_CURRENT_SOURCE_DIR})
  set(kernel_set_source_files ${ZIVC_SOURCE_FILES})
  set(kernel_set_include_dirs ${zivc_core_dir} ${ZIVC_INCLUDE_DIRS})
  set(kernel_set_definitions ${ZIVC_DEFINITIONS})
  set(kernel_set_depends ${ZIVC_DEPENDS})
  # SPIR-V version
  if(ZIVC_SPIRV_VERSION)
    set(kernel_set_spirv_version ${ZIVC_SPIRV_VERSION})
  else()
    set(kernel_set_spirv_version ${ZIVC_DEFAULT_SPIRV_VERSION})
  endif()
  set(supported_spirv_versions 1.0 1.3 1.4 1.5)
  if(NOT (kernel_set_spirv_version IN_LIST supported_spirv_versions))
    message(WARNING "The specified SPIR-V version '${kernel_set_spirv_version}' isn't supported. Use '1.3' instead.")
    set(kernel_set_spirv_version 1.3)
  endif()

  # Make a CMakeLists.txt of the given kernel set
  set(kernel_set_dir ${PROJECT_BINARY_DIR}/KernelSet/${kernel_set_name})
  file(MAKE_DIRECTORY ${kernel_set_dir})
  configure_file(${kernel_set_template_dir}/kernel_set.cmake.in
                 ${kernel_set_dir}/CMakeLists.txt
                 @ONLY)
  add_subdirectory(${kernel_set_dir} ${kernel_set_dir}/build)
endfunction(Zivc_addKernelSet)
