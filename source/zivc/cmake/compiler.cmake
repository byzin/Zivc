# file: compiler.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


set(__zisc_compiler_cmake_path
    ${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/compiler.cmake)


function(Zivc_getCxxCompilerFlags cxx_compiler_flags cxx_linker_flags cxx_definitions)
  include(${__zisc_compiler_cmake_path})
  Zisc_getCxxCompilerFlags(cxx_com_flags cxx_lin_flags cxx_defs)

  # Output
  set(${cxx_compiler_flags} ${cxx_com_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${cxx_lin_flags} PARENT_SCOPE)
  set(${cxx_definitions} ${cxx_defs} PARENT_SCOPE)
endfunction(Zivc_getCxxCompilerFlags)


function(Zivc_getCxxWarningFlags compile_warning_flags)
  include(${__zisc_compiler_cmake_path})
  Zisc_getCxxWarningFlags(compile_war_flags)

  # Output
  set(${compile_warning_flags} ${compile_war_flags} PARENT_SCOPE)
endfunction(Zivc_getCxxWarningFlags)


function(Zivc_setStaticAnalyzer target)
  include(${__zisc_compiler_cmake_path})
  Zisc_setStaticAnalyzer(${target} ${ARGN})
endfunction(Zivc_setStaticAnalyzer)


function(Zivc_createLinkToFiles output_dir)
  include(${__zisc_compiler_cmake_path})
  Zisc_createLinkToFiles("${output_dir}" ${ARGN})
endfunction(Zivc_createLinkToFiles)


function(Zivc_createLinkToTarget target output_dir)
  include(${__zisc_compiler_cmake_path})
  Zisc_createLinkToTarget(${target} "${output_dir}")
endfunction(Zivc_createLinkToTarget)


function(Zivc_enableIpo target)
  include(${__zisc_compiler_cmake_path})
  Zisc_enableIpo(${target})
endfunction(Zivc_enableIpo)


function(Zivc_populateTargetOptions source_target dest_target)
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake)

  Zivc_checkTarget(${source_target})
  Zivc_checkTarget(${dest_target})

  # TODO. Why it's needed?
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)

  get_target_property(zivc_compile_flags ${source_target} INTERFACE_COMPILE_OPTIONS)
  if(zivc_compile_flags)
    target_compile_options(${dest_target} PRIVATE ${zivc_compile_flags})
  endif()
  get_target_property(zivc_libraries ${source_target} INTERFACE_LINK_LIBRARIES)
  if(zivc_libraries)
    target_link_libraries(${dest_target} PRIVATE ${zivc_libraries})
  endif()
  get_target_property(zivc_linker_flags ${source_target} INTERFACE_LINK_OPTIONS)
  if(zivc_linker_flags)
    target_link_options(${dest_target} PRIVATE ${zivc_linker_flags})
  endif()
  get_target_property(zivc_definitions ${source_target} INTERFACE_COMPILE_DEFINITIONS)
  if(zivc_definitions)
    target_compile_definitions(${dest_target} PRIVATE ${zivc_definitions})
  endif()
  #  get_target_property(zivc_include_dirs ${source_target} INTERFACE_INCLUDE_DIRECTORIES)
  #  if(zivc_include_dirs)
  #    target_include_directories(${dest_target} PRIVATE ${zivc_include_dirs})
  #  endif()
  #  get_target_property(zivc_include_dirs ${source_target} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
  #  if(zivc_include_dirs)
  #    target_include_directories(${dest_target} SYSTEM PRIVATE ${zivc_include_dirs})
  #  endif()
endfunction(Zivc_populateTargetOptions)


function(Zivc_setRequiredCxxFlagsToTarget target)
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/general.cmake)
  Zivc_populateTargetOptions(Zisc ${target})
endfunction(Zivc_setRequiredCxxFlagsToTarget)
