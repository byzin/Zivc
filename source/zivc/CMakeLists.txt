# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.21)


function(Zivc_initZivcOptions)
  set(option_description "Enable Vulkan sub-platform.")
  Zivc_setBooleanOption(ZIVC_ENABLE_VULKAN_SUB_PLATFORM ON ${option_description})

  set(option_description "Load Vulkan library dynamically at runtime.")
  Zivc_setBooleanOption(ZIVC_DYNAMIC_VULKAN_LOADING ON ${option_description})

  set(option_description "Enable Zivc to build VMA implementation.")
  Zivc_setBooleanOption(ZIVC_ENABLE_VMA_IMPLEMENTATION ON ${option_description})

  set(option_description "Zivc uses own 'clspv' instead of existing 'clspv'.")
  Zivc_setBooleanOption(ZIVC_USES_OWN_CLSPV OFF ${option_description})

  set(option_description "Specify the default SPIR-V version used in Zivc.")
  Zivc_setStringOption(ZIVC_DEFAULT_SPIRV_VERSION "1.3" ${option_description})

  set(option_description "Bake Vulkan SPIR-V kernels into the library.")
  Zivc_setBooleanOption(ZIVC_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable SPIR-V analysis.")
  Zivc_setBooleanOption(ZIVC_ENABLE_SPIRV_ANALYSIS ON ${option_description})

  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/kernel.cmake)
  Zivc_initZivcKernelOptions()
endfunction(Zivc_initZivcOptions)


function(Zivc_getZivcFlags zivc_compile_flags zivc_linker_flags zivc_definitions)
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/kernel.cmake)
  Zivc_getZivcKernelFlags(compile_flags definitions)

  if(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    list(APPEND definitions ZIVC_ENABLE_VULKAN_SUB_PLATFORM=1)
    # Dynamic vulkan loading
    if(ZIVC_DYNAMIC_VULKAN_LOADING)
      list(APPEND definitions ZIVC_DYNAMIC_VULKAN_LOADING=1)
    endif()
    # Vulkan Memory Allocator
    if(ZIVC_ENABLE_VMA_IMPLEMENTATION)
      list(APPEND definitions ZIVC_ENABLE_VMA_IMPLEMENTATION=1)
    endif()
    if(ZIVC_BAKE_KERNELS)
      list(APPEND definitions ZIVC_BAKE_KERNELS=1)
    endif()
  endif()

  # C++
  list(APPEND definitions ZIVC_CPU=1)
  # Suppress warnings
  if(Z_VISUAL_STUDIO)
    list(APPEND definitions _CRT_SECURE_NO_WARNINGS)
  endif()

  # Output variables
  set(${zivc_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zivc_definitions} ${definitions} PARENT_SCOPE)
endfunction(Zivc_getZivcFlags)


function(Zivc_getZivcWarningFlags zivc_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(Z_CLANG)
    list(APPEND warning_flags -Wno-covered-switch-default
                              # -Wno-padded
                              -Wno-sign-conversion
                              )
    if(Z_VISUAL_STUDIO)
      list(APPEND warning_flags -Wno-nonportable-system-include-path)
    endif()
  elseif(Z_GCC)
    list(APPEND warning_flags -Wno-attributes
                              -Wno-sign-conversion
                              )
  elseif(Z_VISUAL_STUDIO)
    list(APPEND warning_flags /wd4244 # conversion from '' to 'unsigned short', possible loss of data
                              /wd4267 # conversion from 'size_t' to '', possible loss of data
                              /wd4389 # '==': signed/unsigned mismatch
                              )
  endif()

  set(${zivc_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(Zivc_getZivcWarningFlags)


function(Zivc_addClspv clspv_path)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  message(STATUS "Fetching clspv dependencies ... ")
  execute_process(
    COMMAND ${Python3_EXECUTABLE} ${clspv_path}/utils/fetch_sources.py
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
  add_subdirectory(${clspv_path} ${PROJECT_BINARY_DIR}/Clspv EXCLUDE_FROM_ALL)
  Zivc_checkTarget(clspv)
endfunction(Zivc_addClspv)


function(Zivc_addZstd zstd_path)
  set(ZSTD_BUILD_SHARED OFF)
  set(ZSTD_BUILD_PROGRAMS OFF)
  set(ZSTD_MULTITHREAD_SUPPORT OFF)
  set(zstd_dest_path ${PROJECT_BINARY_DIR}/Zstd)
  add_subdirectory(${zstd_path}/build/cmake ${zstd_dest_path} EXCLUDE_FROM_ALL)
  Zivc_checkTarget(libzstd_static)
  Zivc_setRequiredCxxFlagsToTarget(libzstd_static)
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
  file(MAKE_DIRECTORY ${zstd_dest_path}/include)
  file(COPY ${zstd_path}/lib/zstd.h ${zstd_path}/lib/zstd_errors.h
       DESTINATION ${zstd_dest_path}/include)
  target_include_directories(libzstd_static SYSTEM INTERFACE $<BUILD_INTERFACE:${zstd_dest_path}/include>)
endfunction(Zivc_addZstd)


function(Zivc_addZivcKernels target version)
  set(kernel_project_dir ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
  include(${kernel_project_dir}/cmake/kernel.cmake)

  set(kernel_set_dir ${kernel_project_dir}/internal_kernel/zivc)
  file(GLOB_RECURSE kernel_sources ${kernel_set_dir}/kernel/*.cl)
  set(kernel_set_name "zivc_internal_kernel")
  Zivc_addKernelSet(${kernel_set_name} ${version}
      SOURCE_FILES ${kernel_sources}
      INCLUDE_DIRS ${kernel_set_dir})
  target_link_libraries(${target} PRIVATE KernelSet_${kernel_set_name})
endfunction(Zivc_addZivcKernels)


#
macro(Zivc_setZivcProject)
  set(project_description "Zivc.")
  project(Zivc VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
  message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

  set(dependencies_dir ${PROJECT_SOURCE_DIR}/../dependencies)

  include(${PROJECT_SOURCE_DIR}/cmake/general.cmake)
  include(${PROJECT_SOURCE_DIR}/cmake/platform.cmake)
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  Zivc_initZivcOptions()

  # Check dependencies
  ## Zisc
  set(zisc_path ${dependencies_dir}/Zisc/source/zisc)
  add_subdirectory(${zisc_path} ${PROJECT_BINARY_DIR}/Zisc)
  ## cpu_features
  set(cpu_features_path ${dependencies_dir}/cpu_features)
  add_subdirectory(${cpu_features_path} ${PROJECT_BINARY_DIR}/CpuFeatures EXCLUDE_FROM_ALL)
  Zivc_checkTarget(cpu_features)

  set(zivc_core_dir ${PROJECT_SOURCE_DIR}/core)

  # Make zivc configuration header file
  set(zivc_config_dir ${PROJECT_BINARY_DIR}/include/zivc)
  file(MAKE_DIRECTORY ${zivc_config_dir})
  configure_file(${zivc_core_dir}/zivc/zivc_config-inl.hpp.in
                 ${zivc_config_dir}/zivc_config-inl.hpp
                 @ONLY)

  # Create Zivc library
  file(GLOB_RECURSE zivc_source_files ${zivc_core_dir}/zivc/*[.hpp|.cpp])
  if(NOT ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    foreach(source_file IN LISTS zivc_source_files)
      if(source_file MATCHES "${zivc_core_dir}/zivc/vulkan/.*")
        list(REMOVE_ITEM zivc_source_files ${source_file})
      endif()
    endforeach(source_file)
  endif()
  list(APPEND zivc_source_files ${zivc_config_dir}/zivc_config-inl.hpp)
  add_library(${PROJECT_NAME} STATIC ${zivc_source_files})
  source_group(${PROJECT_NAME} FILES ${zivc_source_files})

  # Compiler flags
  include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                   CXX_STANDARD_REQUIRED ON)

  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getZivcFlags(zivc_compile_flags zivc_linker_flags zivc_definitions)
  Zivc_getZivcWarningFlags(zivc_warning_flags)
  target_compile_options(${PROJECT_NAME} PUBLIC ${zivc_compile_flags}
                                         PRIVATE ${cxx_compile_warning_flags}
                                                 ${zivc_warning_flags})
  target_link_libraries(${PROJECT_NAME} PUBLIC Zisc cpu_features)
  target_link_options(${PROJECT_NAME} PUBLIC ${zivc_linker_flags})
  target_compile_definitions(${PROJECT_NAME} PUBLIC ${zivc_definitions})
  target_include_directories(${PROJECT_NAME} PUBLIC ${zivc_core_dir}
                                                    ${PROJECT_BINARY_DIR}/include)

  if(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    # Find dependencies
    if(ZIVC_USES_OWN_CLSPV)
      set(clspv_path ${dependencies_dir}/clspv)
      Zivc_checkSubmodule(${clspv_path})
      Zivc_addClspv(${clspv_path})
    endif()
    if(ZIVC_BAKE_KERNELS)
      set(zstd_path ${dependencies_dir}/zstd)
      Zivc_checkSubmodule(${zstd_path})
      Zivc_addZstd(${zstd_path})
      add_subdirectory(${PROJECT_SOURCE_DIR}/kernel_baker ${PROJECT_BINARY_DIR}/KernelBaker)
    endif()
    # Vulkan
    Zivc_findVulkan(vulkan_target)
    Zivc_checkTarget(${vulkan_target})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${vulkan_target})
    # Vulkan Memory Allocator
    set(vma_path ${dependencies_dir}/VulkanMemoryAllocator)
    Zivc_checkSubmodule(${vma_path})
    set(vma_include_dir ${vma_path}/include)
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${vma_include_dir})
    #
    Zivc_addZivcKernels(${PROJECT_NAME} ${PROJECT_VERSION})
  endif()

  Zivc_enableIpo(${PROJECT_NAME})

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  misc-misplaced-const
                                  misc-no-recursion
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zivc_setStaticAnalyzer(${PROJECT_NAME}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
endmacro(Zivc_setZivcProject)


##
Zivc_setZivcProject()
