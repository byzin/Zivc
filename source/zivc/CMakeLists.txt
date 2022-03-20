# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.22)


function(Zivc_initZivcOptions)
  set(option_description "Enable Vulkan backend.")
  Zivc_setBooleanOption(ZIVC_ENABLE_VULKAN_BACKEND ON ${option_description})

  set(option_description "Load Vulkan library dynamically at runtime.")
  Zivc_setBooleanOption(ZIVC_DYNAMIC_VULKAN_LOADING ON ${option_description})

  set(option_description "Enable Zivc to build VMA implementation.")
  Zivc_setBooleanOption(ZIVC_ENABLE_VMA_IMPLEMENTATION ON ${option_description})

  set(option_description "Zivc uses own 'clspv' instead of existing 'clspv'.")
  Zivc_setBooleanOption(ZIVC_USES_OWN_CLSPV OFF ${option_description})

  set(option_description "Specify the default SPIR-V version used in Zivc.")
  Zivc_setStringOption(ZIVC_DEFAULT_SPIRV_VERSION "1.3" ${option_description})

  set(option_description "Bake Vulkan SPIR-V kernels into it's library.")
  Zivc_setBooleanOption(ZIVC_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable SPIR-V analysis.")
  Zivc_setBooleanOption(ZIVC_ENABLE_SPIRV_ANALYSIS ON ${option_description})

  include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/kernel.cmake")
  Zivc_initZivcKernelOptions()
endfunction(Zivc_initZivcOptions)


function(Zivc_getZivcFlags zivc_compile_flags zivc_linker_flags zivc_definitions)
  include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/kernel.cmake")
  Zivc_getZivcKernelFlags(compile_flags definitions)

  if(ZIVC_ENABLE_VULKAN_BACKEND)
    list(APPEND definitions ZIVC_ENABLE_VULKAN_BACKEND=1)
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
  list(APPEND definitions ZIVC_CL_CPU=1)
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
    list(APPEND warning_flags
                             )

    if(Z_VISUAL_STUDIO)
      list(APPEND warning_flags
                               )
    endif()
  elseif(Z_GCC)
    list(APPEND warning_flags
                             )
  elseif(Z_VISUAL_STUDIO)
    list(APPEND warning_flags 
                             )
  endif()

  set(${zivc_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(Zivc_getZivcWarningFlags)


function(Zivc_addVulkanMemoryAllocator vma_target)
  # Define target
  add_library(VulkanMemoryAllocator INTERFACE)
  #
  cmake_path(SET vma_source_path "${dependencies_dir}/VulkanMemoryAllocator")
  Zivc_checkSubmodule(${vma_source_path})
  cmake_path(SET vma_include_dir "${vma_source_path}/include")
  target_include_directories(VulkanMemoryAllocator SYSTEM INTERFACE "${vma_include_dir}")
  # Define alias
  add_library(Vma::VulkanMemoryAllocator ALIAS VulkanMemoryAllocator)
  # Output variable
  set(${vma_target} Vma::VulkanMemoryAllocator PARENT_SCOPE)
endfunction(Zivc_addVulkanMemoryAllocator)


function(Zivc_addZivcKernels target version)
  cmake_path(SET zivc_path ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
  include("${zivc_path}/cmake/kernel.cmake")

  cmake_path(SET kernel_set_dir "${zivc_path}/internal_kernel/zivc")
  file(GLOB_RECURSE kernel_sources ${kernel_set_dir}/kernel/*.cl)
  set(kernel_set_name "zivc_internal_kernel")
  Zivc_addKernelSet(${kernel_set_name} ${version}
      SOURCE_FILES ${kernel_sources}
      INCLUDE_DIRS "${kernel_set_dir}")
  target_link_libraries(${target} PRIVATE KernelSet_${kernel_set_name})
endfunction(Zivc_addZivcKernels)


#
macro(Zivc_setZivcProject)
  set(project_description "Zivc.")
  project(Zivc VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
  message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

  cmake_path(SET dependencies_dir NORMALIZE "${PROJECT_SOURCE_DIR}/../dependencies")

  include("${PROJECT_SOURCE_DIR}/cmake/general.cmake")
  include("${PROJECT_SOURCE_DIR}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})
  Zivc_initZivcOptions()

  # Check dependencies
  include("${PROJECT_SOURCE_DIR}/cmake/compiler.cmake")
  ## Zisc
  cmake_path(SET zisc_path "${dependencies_dir}/Zisc")
  Zivc_checkSubmodule("${zisc_path}")
  add_subdirectory(${zisc_path} "${PROJECT_BINARY_DIR}/Zisc")
  Zivc_checkTarget(Zisc)
  ## cpu_features
  Zivc_checkTarget(CpuFeature::cpu_features)
  Zivc_setRequiredCxxFlagsToTarget(cpu_features)

  cmake_path(SET zivc_core_dir "${PROJECT_SOURCE_DIR}/core")
  # Make zivc configuration header file
  cmake_path(SET zivc_config_dir "${PROJECT_BINARY_DIR}/include/zivc")
  file(MAKE_DIRECTORY "${zivc_config_dir}")
  configure_file("${zivc_core_dir}/zivc/zivc_config-inl.hpp.in"
                 "${zivc_config_dir}/zivc_config-inl.hpp"
                 @ONLY)

  # Create Zivc library
  file(GLOB_RECURSE zivc_source_files ${zivc_core_dir}/zivc/*[.hpp|.cpp])
  if(NOT ZIVC_ENABLE_VULKAN_BACKEND)
    foreach(source_file IN LISTS zivc_source_files)
      if(source_file MATCHES "${zivc_core_dir}/zivc/vulkan/.*")
        list(REMOVE_ITEM zivc_source_files "${source_file}")
      endif()
    endforeach(source_file)
  endif()
  list(APPEND zivc_source_files "${zivc_config_dir}/zivc_config-inl.hpp")
  add_library(${PROJECT_NAME} STATIC ${zivc_source_files})
  source_group(${PROJECT_NAME} FILES ${zivc_source_files})

  # Compiler flags
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                   CXX_STANDARD_REQUIRED ON)

  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getZivcFlags(zivc_compile_flags zivc_linker_flags zivc_definitions)
  Zivc_getZivcWarningFlags(zivc_warning_flags)
  target_compile_options(${PROJECT_NAME} PUBLIC ${zivc_compile_flags}
                                         PRIVATE ${cxx_compile_warning_flags}
                                                 ${zivc_warning_flags})
  target_link_libraries(${PROJECT_NAME} PUBLIC Zisc
                                        PRIVATE CpuFeature::cpu_features)
  target_link_options(${PROJECT_NAME} PUBLIC ${zivc_linker_flags})
  target_compile_definitions(${PROJECT_NAME} PUBLIC ${zivc_definitions})
  target_include_directories(${PROJECT_NAME} PUBLIC "${zivc_core_dir}"
                                                    "${PROJECT_BINARY_DIR}/include")
  Zivc_enableIpo(${PROJECT_NAME})

  # Vulkan backend setup
  if(ZIVC_ENABLE_VULKAN_BACKEND)
    # Find dependencies
    if(ZIVC_USES_OWN_CLSPV)
      Zivc_addClspv("${PROJECT_BINARY_DIR}/Clspv")
    endif()
    if(ZIVC_BAKE_KERNELS)
      add_subdirectory("${PROJECT_SOURCE_DIR}/kernel_baker"
                       "${PROJECT_BINARY_DIR}/KernelBaker")
    endif()
    # Vulkan
    Zivc_findVulkan(vulkan_target)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${vulkan_target})
    # Vulkan Memory Allocator
    Zivc_addVulkanMemoryAllocator(vma_target)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${vma_target})
    # Internal kernels
    Zivc_addZivcKernels(${PROJECT_NAME} ${PROJECT_VERSION})
  endif()

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  misc-misplaced-const
                                  misc-no-recursion
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zivc_setStaticAnalyzer(${PROJECT_NAME}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})

  #
  add_library(Zivc::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endmacro(Zivc_setZivcProject)


##
Zivc_setZivcProject()
