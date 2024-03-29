# file: general.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


set(__zisc_general_cmake_path
    ${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/general.cmake)


macro(Zivc_setBooleanOption variable value doc_string)
  include(${__zisc_general_cmake_path})
  Zisc_setBooleanOption(${variable} ${value} "${doc_string}")
endmacro(Zivc_setBooleanOption)


macro(Zivc_setStringOption variable value doc_string)
  include(${__zisc_general_cmake_path})
  Zisc_setStringOption(${variable} ${value} "${doc_string}")
endmacro(Zivc_setStringOption)


function(Zivc_restrictBuildDirectory build_dir)
  include(${__zisc_general_cmake_path})
  Zisc_restrictBUildDirectory(${build_dir})
endfunction(Zivc_restrictBuildDirectory)


function(Zivc_checkSubmodule submodule_path)
  include(${__zisc_general_cmake_path})
  Zisc_checkSubmodule("${submodule_path}")
endfunction(Zivc_checkSubmodule)


function(Zivc_checkTarget target)
  include(${__zisc_general_cmake_path})
  Zisc_checkTarget(${target})
endfunction(Zivc_checkTarget)


function(Zivc_printSystemInfo message_prefix)
  include(${__zisc_general_cmake_path})
  Zisc_printSystemInfo("${message_prefix}")
endfunction(Zivc_printSystemInfo)


function(Zivc_setVariablesOnCMake)
  foreach(value IN LISTS ARGV)
    set(${value} ON PARENT_SCOPE)
  endforeach(value)
endfunction(Zivc_setVariablesOnCMake)


function(Zivc_getSaveDependencyCode target output_dir exe_path dirs rpaths code)
  include(${__zisc_general_cmake_path})
  Zisc_getSaveDependencyCode(${target} "${output_dir}" "${exe_path}" "${dirs}" "${rpaths}" code_out)

  # Output
  set(${code} ${code_out} PARENT_SCOPE)
endfunction(Zivc_getSaveDependencyCode)
