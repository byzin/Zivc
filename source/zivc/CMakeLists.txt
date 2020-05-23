# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.16)


function(initZivcOptions)
  set(option_description "Enable Vulkan sub-platform.")
  setBooleanOption(ZIVC_ENABLE_VULKAN_SUB_PLATFORM ON ${option_description})

  set(option_description "Load Vulkan library dynamically at runtime.")
  setBooleanOption(ZIVC_DYNAMIC_VULKAN_LOADING OFF ${option_description})

  set(option_description "Enable Zivc to build VMA implementation.")
  setBooleanOption(ZIVC_ENABLE_VMA_IMPLEMENTATION ON ${option_description})

  set(option_description "Zivc uses own 'clspv' instead of existing 'clspv'.")
  setBooleanOption(ZIVC_USES_OWN_CLSPV OFF ${option_description})

  set(option_description "Bake Vulkan SPIR-V kernels into the library.")
  setBooleanOption(ZIVC_BAKE_KERNELS OFF ${option_description})

  set(option_description "Send in POD kernel arguments in push constant.")
  setBooleanOption(ZIVC_POD_PUSHCONSTANT OFF ${option_description})

  set(option_description "Enable SPIR-V analysis.")
  setBooleanOption(ZIVC_ENABLE_SPIRV_ANALYSIS ON ${option_description})

  ## Zivc Kernel code options

  set(option_description "Use built-in math funcs instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN OFF ${option_description})

  set(option_description "Use built-in 'frexp' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_FREXP OFF ${option_description})

  set(option_description "Use built-in 'ldexp' instead of the Zivc funcs.")
  if(Z_MAC)
    setBooleanOption(ZIVC_MATH_BUILTIN_LDEXP OFF ${option_description})
  else()
    setBooleanOption(ZIVC_MATH_BUILTIN_LDEXP ON ${option_description})
  endif()

  set(option_description "Use built-in 'ilogb' instead of the Zivc func.")
  setBooleanOption(ZIVC_MATH_BUILTIN_ILOGB OFF ${option_description})

  set(option_description "Use built-in 'modf' instead of the Zivc func.")
  setBooleanOption(ZIVC_MATH_BUILTIN_MODF OFF ${option_description})

  set(option_description "Use built-in 'round' and 'rint' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_ROUND OFF ${option_description})

  set(option_description "Use built-in 'fmod' instead of the Zivc func.")
  setBooleanOption(ZIVC_MATH_BUILTIN_FMOD ON ${option_description})

  set(option_description "Use built-in 'exp' and 'exp2' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_EXP ON ${option_description})

  set(option_description "Use built-in 'log' and 'log2' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_LOG ON ${option_description})

  set(option_description "Use built-in 'pow' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_POW ON ${option_description})

  set(option_description "Use built-in 'sqrt' and 'rsqrt' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_SQRT ON ${option_description})

  set(option_description "Use built-in 'cbrt' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_CBRT OFF ${option_description})

  set(option_description "Use built-in 'sin', 'cos' and 'tan' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_TRIGONOMETRIC ON ${option_description})

  set(option_description "Use built-in 'asin', 'acos' and 'atan' instead of the Zivc funcs.")
  setBooleanOption(ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC ON ${option_description})
endfunction(initZivcOptions)


function(getZivcFlags zivc_compile_flags zivc_linker_flags zivc_definitions)
  include(${PROJECT_SOURCE_DIR}/cmake/kernel.cmake)
  getZivcKernelFlags(compile_flags definitions)

  if(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    list(APPEND definitions ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    # Dynamic vulkan loading
    if(ZIVC_DYNAMIC_VULKAN_LOADING)
      list(APPEND definitions ZIVC_DYNAMIC_VULKAN_LOADING)
    endif()
    # Vulkan Memory Allocator
    if(ZIVC_ENABLE_VMA_IMPLEMENTATION)
      list(APPEND definitions ZIVC_ENABLE_VMA_IMPLEMENTATION)
    endif()
    if(ZIVC_BAKE_KERNELS)
      list(APPEND definitions ZIVC_BAKE_KERNELS)
    endif()
  endif()

  # C++
  list(APPEND definitions ZIVC_CPU)

  # Output variables
  set(${zivc_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zivc_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZivcFlags)


function(getZivcWarningFlags zivc_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(Z_CLANG)
    list(APPEND warning_flags -Wno-float-equal
                              -Wno-padded
                              -Wno-sign-conversion
                              )
  elseif(Z_GCC)
    list(APPEND warning_flags -Wno-sign-conversion
                              )
  endif()

  set(${zivc_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getZivcWarningFlags)


function(addClspv clspv_path)
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
  add_subdirectory(${clspv_path} ${PROJECT_BINARY_DIR}/Clspv)
  checkTarget(clspv)
endfunction(addClspv)


function(addZstd zstd_path)
  set(ZSTD_BUILD_SHARED OFF)
  set(ZSTD_BUILD_PROGRAMS OFF)
  set(ZSTD_MULTITHREAD_SUPPORT OFF)
  set(zstd_dest_path ${PROJECT_BINARY_DIR}/Zstd)
  add_subdirectory(${zstd_path}/build/cmake ${zstd_dest_path})
  checkTarget(libzstd_static)
  # Headers
  file(MAKE_DIRECTORY ${zstd_dest_path}/include)
  file(COPY ${zstd_path}/lib/zstd.h ${zstd_path}/lib/common/zstd_errors.h
       DESTINATION ${zstd_dest_path}/include)
  target_include_directories(libzstd_static SYSTEM INTERFACE $<BUILD_INTERFACE:${zstd_dest_path}/include>)
endfunction(addZstd)


#
set(project_description "Zivc.")
project(Zivc VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(dependencies_dir ${PROJECT_SOURCE_DIR}/../dependencies)
set(zisc_path ${dependencies_dir}/Zisc/source/zisc)
set(cpu_features_path ${dependencies_dir}/cpu_features)

include(${zisc_path}/cmake/general.cmake)
include(${zisc_path}/cmake/platform.cmake)
getPlatformFlags(platform_definitions)
setVariablesOnCMake(${platform_definitions})
initZivcOptions()

# Check dependencies
add_subdirectory(${zisc_path} ${PROJECT_BINARY_DIR}/Zisc)
add_subdirectory(${cpu_features_path} ${PROJECT_BINARY_DIR}/CpuFeatures)

# Make zivc configuration header file
set(zivc_config_dir ${PROJECT_BINARY_DIR}/include/zivc)
file(MAKE_DIRECTORY ${zivc_config_dir})
configure_file(${PROJECT_SOURCE_DIR}/zivc/zivc_config-inl.hpp.in
               ${zivc_config_dir}/zivc_config-inl.hpp
               @ONLY)

# Create Zivc library
file(GLOB_RECURSE zivc_source_files ${PROJECT_SOURCE_DIR}/zivc/*[.hpp|.cpp])
if(NOT ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
  foreach(source_file IN LISTS zivc_source_files)
    if(source_file MATCHES "${PROJECT_SOURCE_DIR}/zivc/vulkan/.*")
      list(REMOVE_ITEM zivc_source_files ${source_file})
    endif()
  endforeach(source_file)
endif()
list(APPEND zivc_source_files ${zivc_config_dir}/zivc_config-inl.hpp)
add_library(${PROJECT_NAME} STATIC ${zivc_source_files})
source_group(${PROJECT_NAME} FILES ${zivc_source_files})

# Compiler flags
include(${zisc_path}/cmake/compiler.cmake)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED ON)

getCxxWarningFlags(cxx_compile_warning_flags)
getZivcFlags(zivc_compile_flags zivc_linker_flags zivc_definitions)
getZivcWarningFlags(zivc_warning_flags)
target_compile_options(${PROJECT_NAME} PUBLIC ${zivc_compile_flags}
                                       PRIVATE ${cxx_compile_warning_flags}
                                               ${zivc_warning_flags})
target_link_libraries(${PROJECT_NAME} PUBLIC Zisc cpu_features)
target_link_options(${PROJECT_NAME} PUBLIC ${zivc_linker_flags})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${zivc_definitions})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}
                                                  ${PROJECT_BINARY_DIR}/include)

if(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
  # Find dependencies
  if(ZIVC_USES_OWN_CLSPV)
    set(clspv_path ${dependencies_dir}/clspv)
    checkSubmodule(${clspv_path})
    addClspv(${clspv_path})
  endif()
  if(ZIVC_BAKE_KERNELS)
    set(zstd_path ${dependencies_dir}/zstd)
    checkSubmodule(${zstd_path})
    addZstd(${zstd_path})
    add_subdirectory(${PROJECT_SOURCE_DIR}/kernel_baker ${PROJECT_BINARY_DIR}/KernelBaker)
  endif()
  # Vulkan
  find_package(Vulkan REQUIRED)
  target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
  # Vulkan Memory Allocator
  set(vma_path ${dependencies_dir}/VulkanMemoryAllocator)
  checkSubmodule(${vma_path})
  set(vma_include_dir ${PROJECT_BINARY_DIR}/VulkanMemoryAllocator)
  file(MAKE_DIRECTORY ${vma_include_dir})
  file(COPY ${vma_path}/src/vk_mem_alloc.h DESTINATION ${vma_include_dir})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${vma_include_dir})
endif()

#
setStaticAnalyzer(${PROJECT_NAME})
