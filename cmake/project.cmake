# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


# Build Zisc
function(addZisc binary_dir)
  if(TARGET Zisc::Zisc)
    return()
  else()
    message(STATUS "Add Zisc subdirectory.")
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET dependencies_dir NORMALIZE "${project_dir}/source/dependencies")
  cmake_path(SET zisc_path "${dependencies_dir}/Zisc")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  # Add Zisc
  include("${zivc_path}/cmake/general.cmake")
  Zivc_checkSubmodule(${zisc_path})
  add_subdirectory(${zisc_path} ${binary_dir})
endfunction(addZisc)


#
function(addCpuFeatures binary_dir)
  if(TARGET CpuFeatures::cpu_features)
    return()
  else()
    message(STATUS "Add CpuFeatures subdirectory.")
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  include("${zivc_path}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  Zivc_setInternalValue(BUILD_EXECUTABLE OFF)
  Zivc_setInternalValue(ENABLE_INSTALL OFF)
  # Add CpuFeatures
  cmake_path(SET dependencies_dir "${project_dir}/source/dependencies")
  cmake_path(SET cpu_features_path NORMALIZE "${dependencies_dir}/cpu_features")
  Zivc_checkSubmodule("${cpu_features_path}")
  add_subdirectory("${cpu_features_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(CpuFeatures::cpu_features)
  # Supress warnings
  set(cpu_warning_flags "")
  if(Z_VISUAL_STUDIO)
    list(APPEND cpu_warning_flags
                                  )
  elseif(Z_CLANG AND NOT Z_APPLE_CLANG)
    list(APPEND cpu_warning_flags -Wno-unused-command-line-argument
                                  -Wno-unused-but-set-variable
                                  )
  endif()
  target_compile_options(cpu_features PRIVATE ${cpu_warning_flags})
endfunction(addCpuFeatures)


# Build Zstd
function(addZstd binary_dir)
  if(TARGET Zstd::Zstd)
    return()
  else()
    message(STATUS "Add Zstd subdirectory.")
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
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
  add_library(Zstd::Zstd ALIAS libzstd_static)
  Zivc_checkTarget(Zstd::Zstd)
  # Properties
  set_target_properties(libzstd_static PROPERTIES CXX_STANDARD 20
                                                  CXX_STANDARD_REQUIRED ON)
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
endfunction(addZstd)


# Build CLI11
function(addCli11 binary_dir)
  if(TARGET CLI11::CLI11)
    return()
  else()
    message(STATUS "Add CLI11 subdirectory.")
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  # Add CLI11
  Zivc_setInternalValue(CLI11_WARNINGS_AS_ERRORS OFF)
  Zivc_setInternalValue(CLI11_SINGLE_FILE OFF)
  Zivc_setInternalValue(CLI11_SANITIZERS OFF)
  Zivc_setInternalValue(CLI11_BUILD_DOCS OFF)
  Zivc_setInternalValue(CLI11_BUILD_TESTS OFF)
  Zivc_setInternalValue(CLI11_BUILD_EXAMPLES OFF)
  Zivc_setInternalValue(CLI11_BUILD_EXAMPLES_JSON OFF)
  Zivc_setInternalValue(CLI11_PRECOMPILED OFF)
  Zivc_setInternalValue(CLI11_SINGLE_FILE_TESTS OFF)
  Zivc_setInternalValue(CLI11_INSTALL OFF)
  Zivc_setInternalValue(CLI11_FORCE_LIBCXX OFF)
  Zivc_setInternalValue(CLI11_CUDA_TESTS OFF)
  Zivc_setInternalValue(CLI11_CLANG_TIDY_OPTIONS "")
  cmake_path(SET cli11_source_path "${project_dir}/example/dependencies/CLI11")
  Zivc_checkSubmodule("${cli11_source_path}")
  add_subdirectory("${cli11_source_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(CLI11::CLI11)
endfunction(addCli11)


function(findVulkan)
  Zivc_setInternalValue(ZIVC_ENABLE_VULKAN_BACKEND ${Z_ENABLE_VULKAN_BACKEND})
  if(NOT Z_ENABLE_VULKAN_BACKEND)
    return()
  endif()

  if(TARGET Zivc::Vulkan)
    return()
  else()
    message(STATUS "Find Vulkan library.")
  endif()

  # Load vulkan module
  find_package(Vulkan 1.3.261 REQUIRED)

  # Set vulkan target
  if(ZIVC_DYNAMIC_VULKAN_LOADING)
    add_library(Zivc::Vulkan ALIAS Vulkan::Headers)
  else()
    add_library(Zivc::Vulkan ALIAS Vulkan::Vulkan)
  endif()
endfunction(findVulkan)


# Zisc
function(setZiscAlias)
  if(TARGET Zisc::Zisc)
    return()
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  include("${zivc_path}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  include("${zivc_path}/cmake/compiler.cmake")
  Zivc_getArchitectureName(TRUE architecture)
  Zivc_getArchitectureTargetName("Zisc" ${architecture} zisc_name)
  Zivc_checkTarget(${zisc_name})
  add_library(Zisc::Zisc ALIAS ${zisc_name})
endfunction(setZiscAlias)


# Zisc
function(setZivcAlias)
  if(TARGET Zivc::Zivc)
    return()
  endif()

  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  include("${zivc_path}/cmake/general.cmake")
  include("${zivc_path}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  include("${zivc_path}/cmake/compiler.cmake")
  Zivc_getArchitectureName(TRUE architecture)
  Zivc_getArchitectureTargetName("Zivc" ${architecture} zisc_name)
  Zivc_checkTarget(${zisc_name})
  add_library(Zivc::Zivc ALIAS ${zisc_name})
endfunction(setZivcAlias)


# GoogleTest
function(addGoogleTest binary_dir)
  if(TARGET GTest::gtest)
    return()
  else()
    message(STATUS "Add GoogleTest subdirectory.")
  endif()


  # Add googletest
  cmake_path(SET project_dir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..")
  cmake_path(SET zivc_path "${project_dir}/source/zivc")
  cmake_path(SET dependencies_dir "${project_dir}/test/dependencies")
  cmake_path(SET googletest_path "${dependencies_dir}/googletest")
  include("${zivc_path}/cmake/general.cmake")
  Zivc_checkSubmodule(${googletest_path})
  Zivc_addGoogleTest(${googletest_path} ${binary_dir})
  Zivc_checkTarget(gtest)
  #
  include("${zivc_path}/cmake/compiler.cmake")
  Zivc_populateTargetOptions(Zisc::Zisc gtest)
  Zivc_checkTarget(GTest::gtest)
endfunction(addGoogleTest)


# 
function(addSanitizerFlags)
  #
  include("${zivc_path}/cmake/general.cmake")
  include("${zivc_path}/cmake/platform.cmake")
  include("${zivc_path}/cmake/compiler.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  #
  if(NOT Z_WINDOWS)
    return()
  endif()
  #
  Zivc_getSanitizerFlags(sa_compile_flags sa_linker_flags sa_definitions)
  foreach(target IN LISTS ARGN)
    Zivc_checkTarget(${target})
    target_compile_options(${target} PRIVATE ${sa_compile_flags})
    target_link_options(${target} PRIVATE ${sa_linker_flags})
    target_compile_definitions(${target} PRIVATE ${sa_definitions})
  endforeach(target)
endfunction(addSanitizerFlags)
