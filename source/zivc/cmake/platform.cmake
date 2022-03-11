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


function(Zivc_findVulkan vulkan_target)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  include("${zivc_path}/cmake/general.cmake")
  # Set vulkan target
  if(ZIVC_DYNAMIC_VULKAN_LOADING)
    set(target Vulkan::Headers)
  else()
    set(target Vulkan::Vulkan)
  endif()
  # Load vulkan module
  if(NOT TARGET ${target})
    find_package(Vulkan 1.2.198.1 REQUIRED)
  # else()
  #   message(STATUS "Vulkan SDK version: ${Vulkan_VERSION}")
  endif()
  Zivc_checkTarget(${target})

  # Output
  set(${vulkan_target} ${target} PARENT_SCOPE)
endfunction(Zivc_findVulkan)


# Build clspv
function(Zivc_addClspv binary_dir)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET clspv_source_path NORMALIZE "${zivc_path}/../dependencies/clspv")
  #
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
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


# Build Zstd
function(Zivc_addZstd binary_dir)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  include("${zivc_path}/cmake/general.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  # Add Zstd
  Zivc_setInternalValue(ZSTD_BUILD_STATIC ON)
  Zivc_setInternalValue(ZSTD_BUILD_SHARED OFF)
  Zivc_setInternalValue(ZSTD_LEGACY_SUPPORT OFF)
  Zivc_setInternalValue(ZSTD_MULTITHREAD_SUPPORT OFF)
  Zivc_setInternalValue(ZSTD_BUILD_PROGRAMS OFF)
  Zivc_setInternalValue(ZSTD_BUILD_CONTRIB OFF)
  Zivc_setInternalValue(ZSTD_BUILD_TESTS OFF)
  Zivc_setInternalValue(ZSTD_USE_STATIC_RUNTIME OFF)
  Zivc_setInternalValue(ZSTD_PROGRAMS_LINK_SHARED OFF)
  cmake_path(SET zstd_source_path NORMALIZE "${zivc_path}/../dependencies/zstd")
  Zivc_checkSubmodule("${zstd_source_path}")
  add_subdirectory("${zstd_source_path}/build/cmake" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(libzstd_static)
  # Supress warnings
  set(zstd_warning_flags "")
  if(Z_VISUAL_STUDIO)
    list(APPEND zstd_warning_flags /w)
  elseif(Z_CLANG AND NOT Z_APPLE_CLANG)
    list(APPEND zstd_warning_flags -Wno-unused-command-line-argument
                                   -Wno-unused-but-set-variable
                                   )
  endif()
  target_compile_options(libzstd_static PRIVATE ${zstd_warning_flags})
  # Headers
  cmake_path(SET zstd_dest_inc_dir "${binary_dir}/include")
  file(MAKE_DIRECTORY "${zstd_dest_inc_dir}")
  file(COPY "${zstd_source_path}/lib/zstd.h" "${zstd_source_path}/lib/zstd_errors.h"
       DESTINATION "${zstd_dest_inc_dir}")
  target_include_directories(libzstd_static SYSTEM INTERFACE $<BUILD_INTERFACE:${zstd_dest_inc_dir}>)
endfunction(Zivc_addZstd)


# Build CpuFeatures
function(Zivc_addCpuFeatures binary_dir)
  cmake_path(SET zivc_path NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  include("${zivc_path}/cmake/general.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  # Add CpuFeatures
  set(BUILD_TESTING OFF CACHE BOOL "")
  cmake_path(SET cpu_source_path NORMALIZE "${zivc_path}/../dependencies/cpu_features")
  Zivc_checkSubmodule("${cpu_source_path}")
  add_subdirectory("${cpu_source_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(CpuFeature::cpu_features)
  # Supress warnings
  set(cpu_warning_flags "")
  if(Z_VISUAL_STUDIO)
    list(APPEND cpu_warning_flags /w)
  elseif(Z_CLANG AND NOT Z_APPLE_CLANG)
    list(APPEND cpu_warning_flags -Wno-unused-command-line-argument
                                  -Wno-unused-but-set-variable
                                   )
  endif()
  target_compile_options(cpu_features PRIVATE ${cpu_warning_flags})

endfunction(Zivc_addCpuFeatures)


