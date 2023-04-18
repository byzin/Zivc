# file: compiler.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


cmake_path(SET __zisc_compiler_cmake_path NORMALIZE
  "${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/compiler.cmake")


function(Zivc_getCxxCompilerFlags architecture cxx_compiler_flags cxx_linker_flags cxx_definitions)
  include("${__zisc_compiler_cmake_path}")
  Zisc_getCxxCompilerFlags(${architecture} cxx_com_flags cxx_lin_flags cxx_defs)

  # Output
  set(${cxx_compiler_flags} ${cxx_com_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${cxx_lin_flags} PARENT_SCOPE)
  set(${cxx_definitions} ${cxx_defs} PARENT_SCOPE)
endfunction(Zivc_getCxxCompilerFlags)


function(Zivc_getCxxWarningFlags compile_warning_flags)
  include("${__zisc_compiler_cmake_path}")
  Zisc_getCxxWarningFlags(compile_war_flags)

  # Output
  set(${compile_warning_flags} ${compile_war_flags} PARENT_SCOPE)
endfunction(Zivc_getCxxWarningFlags)


function(Zivc_getArchitectureName only_representative arch_name_list)
  include("${__zisc_compiler_cmake_path}")
  Zisc_getArchitectureName(${only_representative} name_list)

  # Output
  set(${arch_name_list} ${name_list} PARENT_SCOPE)
endfunction(Zivc_getArchitectureName)


function(Zivc_getArchitectureTargetName project_name arch_name target_name)
  include("${__zisc_compiler_cmake_path}")
  Zisc_getArchitectureTargetName(${project_name} ${arch_name} name)

  # Output
  set(${target_name} ${name} PARENT_SCOPE)
endfunction(Zivc_getArchitectureTargetName)


function(Zivc_setStaticAnalyzer target)
  include("${__zisc_compiler_cmake_path}")
  Zisc_setStaticAnalyzer(${target} ${ARGN})
endfunction(Zivc_setStaticAnalyzer)


function(Zivc_getSanitizerFlags compile_sanitizer_flags linker_sanitizer_flags)
  include("${__zisc_compiler_cmake_path}")
  Zisc_getSanitizerFlags(compile_flags linker_flags)

  # Output
  set(${compile_sanitizer_flags} ${compile_flags} PARENT_SCOPE)
  set(${linker_sanitizer_flags} ${linker_flags} PARENT_SCOPE)
endfunction(Zivc_getSanitizerFlags)


function(Zivc_createSanitizerIgnoreList source_files output_dir list_name cxx_compile_flags)
  include("${__zisc_compiler_cmake_path}")
  Zisc_createSanitizerIgnoreList(${source_files} ${output_dir} ${list_name} compile_flags)

  # Output
  set(${cxx_compile_flags} ${compile_flags} PARENT_SCOPE)
endfunction(Zivc_createSanitizerIgnoreList)


function(Zivc_createLinkToTarget target output_dir)
  include("${__zisc_compiler_cmake_path}")
  Zisc_createLinkToTarget(${target} "${output_dir}")
endfunction(Zivc_createLinkToTarget)


function(Zivc_populateTargetOptions source_target dest_target)
  include("${__zisc_compiler_cmake_path}")
  Zisc_populateTargetOptions("${source_target}" "${dest_target}")
endfunction(Zivc_populateTargetOptions)


function(Zivc_enableIpo target)
  include("${__zisc_compiler_cmake_path}")
  Zisc_enableIpo(${target})
endfunction(Zivc_enableIpo)


function(Zivc_setRequiredCxxFlagsToTarget target)
  include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake")
  Zivc_populateTargetOptions(Zisc ${target})
endfunction(Zivc_setRequiredCxxFlagsToTarget)
