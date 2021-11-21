# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.21)


set(__zivc_dir__ ${CMAKE_SOURCE_DIR}/source/zivc)


function(getExampleWarningFlags example_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(ZIVC_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags -Wno-covered-switch-default
                                # -Wno-padded
                                -Wno-sign-conversion
                                )
    elseif(Z_GCC)
      list(APPEND warning_flags -Wno-attributes
                                -Wno-sign-conversion
                                )
    endif()
  endif()

  set(${example_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getExampleWarningFlags)


function(addGlfw glfw_path)
  set(GLFW_INSTALL OFF)
  set(GLFW_BUILD_DOCS OFF)
  set(GLFW_BUILD_EXAMPLES OFF)
  set(GLFW_BUILD_TESTS OFF)
  add_subdirectory(${glfw_path} ${PROJECT_BINARY_DIR}/GLFW EXCLUDE_FROM_ALL)
  Zivc_checkTarget(glfw)
  include(${__zivc_dir__}/cmake/compiler.cmake)
  Zivc_setRequiredCxxFlagsToTarget(glfw)
  # Suppress warnings
  set(glfw_warning_flags "")
  if(Z_CLANG)
    list(APPEND glfw_warning_flags -Wno-unused-command-line-argument)
  endif()
  target_compile_options(glfw PRIVATE ${glfw_warning_flags})
  target_include_directories(glfw SYSTEM INTERFACE ${glfw_path}/include)
endfunction(addGlfw)


function(addImgui imgui_path)
  file(GLOB imgui_source_files ${imgui_path}/*[.h|.cpp])
  set(backend_dir ${imgui_path}/backends)
  list(APPEND imgui_source_files ${backend_dir}/imgui_impl_glfw.cpp
                                 ${backend_dir}/imgui_impl_glfw.h
                                 ${backend_dir}/imgui_impl_vulkan.cpp
                                 ${backend_dir}/imgui_impl_vulkan.h)
  add_library(imgui STATIC ${imgui_source_files})
  source_group(${PROJECT_NAME}/imgui FILES ${imgui_source_files})

  # Compiler flags
  include(${__zivc_dir__}/cmake/compiler.cmake)
  Zivc_checkTarget(glfw)
  Zivc_findVulkan(vulkan_target)
  Zivc_checkTarget(${vulkan_target})
  Zivc_setRequiredCxxFlagsToTarget(imgui)
  target_link_libraries(imgui PRIVATE ${vulkan_target} PUBLIC glfw)
  target_compile_definitions(imgui PUBLIC IMGUI_IMPL_VULKAN_NO_PROTOTYPES)
  target_include_directories(imgui SYSTEM PUBLIC ${imgui_path})
endfunction(addImgui)


function(addGui gui_path)
  file(GLOB_RECURSE gui_source_files ${gui_path}/*[.hpp|.cpp])
  add_library(gui STATIC ${gui_source_files})
  source_group(${PROJECT_NAME}/gui FILES ${gui_source_files})

  # Compiler flags
  include(${__zivc_dir__}/cmake/compiler.cmake)
  Zivc_setRequiredCxxFlagsToTarget(gui)
  target_link_libraries(gui PRIVATE Zivc PUBLIC glfw imgui)
  target_include_directories(gui INTERFACE ${gui_path})
  Zivc_enableIpo(gui)
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  misc-no-recursion
                                  readability-function-cognitive-complexity
                                  readability-uppercase-literal-suffix
                                  )
  Zivc_setStaticAnalyzer(gui
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
endfunction(addGui)


# Add an example
function(addExample target save_dependency_list)
  # Parse arguments
  set(options "")
  set(one_value_args "")
  set(multi_value_args SOURCE_FILES DEPENDS)
  cmake_parse_arguments(PARSE_ARGV 2 ZIVC "${options}" "${one_value_args}" "${multi_value_args}")

  # Check source files
  if(NOT ZIVC_SOURCE_FILES)
    message(FATAL_ERROR "The example '${target}' has no sources.")
  endif()

  set(example_source_files ${ZIVC_SOURCE_FILES})
  add_executable(${target} ${example_source_files})
  source_group(${PROJECT_NAME}/${target} FILES ${example_source_files})

  #
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20
                                             CXX_STANDARD_REQUIRED ON)

  include(${__zivc_dir__}/cmake/compiler.cmake)
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  getExampleWarningFlags(example_warning_flags)
  target_compile_options(${target} PRIVATE ${cxx_compile_warning_flags}
                                           ${example_warning_flags})
  target_link_libraries(${target} PRIVATE Zivc ${ZIVC_DEPENDS})
  Zivc_enableIpo(${target})

  #
  set(clang_tidy_exclusion_checks bugprone-exception-escape
                                  bugprone-implicit-widening-of-multiplication-result
                                  bugprone-narrowing-conversions
                                  misc-non-private-member-variables-in-classes
                                  misc-redundant-expression
                                  modernize-avoid-c-arrays
                                  modernize-use-auto
                                  readability-function-cognitive-complexity
                                  readability-isolate-declaration
                                  readability-magic-numbers
                                  readability-named-parameter
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zivc_setStaticAnalyzer(${target}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
  Zivc_createLinkToTarget(${target} ${PROJECT_BINARY_DIR})

  # Installation settings
  # Save the dependency list
  if(save_dependency_list)
    Zivc_getSaveDependencyCode(${target} ${PROJECT_BINARY_DIR} "" "" "" dependency_code)
    install(CODE ${dependency_code} COMPONENT test)
  endif()
  install(TARGETS ${target} DESTINATION bin COMPONENT example)
endfunction(addExample)


#
macro(setExampleProject)
  set(project_description "Examples of Zivc")
  project(Examples VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES  CXX)

  # Initialize platform info
  include(${__zivc_dir__}/cmake/general.cmake)
  include(${__zivc_dir__}/cmake/platform.cmake)
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  # Check dependencies
  Zivc_checkTarget(Zivc)

  include(${CMAKE_SOURCE_DIR}/source/zivc/cmake/kernel.cmake)
  # Build examples
  ## 1. platform example
  addExample(PlatformExample ON
             SOURCE_FILES ${PROJECT_SOURCE_DIR}/platform_example.cpp)
  ## 2. device example
  addExample(DeviceExample OFF
             SOURCE_FILES ${PROJECT_SOURCE_DIR}/device_example.cpp)
  ## 3. kernel example
  addExample(KernelExample OFF
             SOURCE_FILES ${PROJECT_SOURCE_DIR}/kernel_example.cpp)
  Zivc_addKernelSet(example ${PROJECT_VERSION}
      SOURCE_FILES kernels/example.cl
      INCLUDE_DIRS kernels)
  target_link_libraries(KernelExample PRIVATE KernelSet_example)

  # Demos using vulkan
  if(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
    # Build dependencies
    addGlfw(${PROJECT_SOURCE_DIR}/dependencies/glfw)
    addImgui(${PROJECT_SOURCE_DIR}/dependencies/imgui)
    addGui(${PROJECT_SOURCE_DIR}/gui)
    ## 4. vulkan sub-platform example
    addExample(VulkanSubPlatformExample OFF
               SOURCE_FILES ${PROJECT_SOURCE_DIR}/vulkan_sub_platform_example.cpp)
    ## 5. GUI example
    addExample(GuiExample ON
               SOURCE_FILES ${PROJECT_SOURCE_DIR}/gui_example.cpp
               DEPENDS gui)
  endif()

  # Kernel set
endmacro(setExampleProject)


##
setExampleProject()
