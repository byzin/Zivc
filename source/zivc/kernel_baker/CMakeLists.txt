# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.21)


#
macro(Zivc_setKernelBakerProject)
  set(project_description "Kernel baker.")
  project(KernelBaker VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
  message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

  set(dependencies_dir ${PROJECT_SOURCE_DIR}/../../dependencies)

  include(${PROJECT_SOURCE_DIR}/../cmake/general.cmake)
  include(${PROJECT_SOURCE_DIR}/../cmake/platform.cmake)
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  set(source_files ${PROJECT_SOURCE_DIR}/kernel_baker.cpp)
  add_executable(${PROJECT_NAME} ${source_files})
  source_group(${PROJECT_NAME} FILES ${source_files})

  # Compiler flags
  include(${PROJECT_SOURCE_DIR}/../cmake/compiler.cmake)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                        CXX_STANDARD_REQUIRED ON)
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getZivcFlags(zivc_compile_flags zivc_linker_flags zivc_definitions)
  Zivc_getZivcWarningFlags(zivc_warning_flags)
  target_compile_options(${PROJECT_NAME} PRIVATE ${cxx_compile_warning_flags}
                                                 ${zivc_compile_flags}
                                                 ${zivc_warning_flags})
  target_include_directories(${PROJECT_NAME} PRIVATE ${zivc_core_dir})
  target_link_libraries(${PROJECT_NAME} PRIVATE Zisc libzstd_static)
  target_link_options(${PROJECT_NAME} PRIVATE ${zivc_linker_flags})
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${zivc_definitions})
  Zivc_enableIpo(${PROJECT_NAME})

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  )
  Zivc_setStaticAnalyzer(${PROJECT_NAME}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
endmacro(Zivc_setKernelBakerProject)


##
Zivc_setKernelBakerProject()
