# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.22)


cmake_path(SET __zivc_dir__ "${CMAKE_SOURCE_DIR}/source/zivc")


function(getExampleWarningFlags example_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(ZIVC_SUPPRESS_EXCESSIVE_WARNING)
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
  endif()

  set(${example_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getExampleWarningFlags)


function(addGlfw binary_dir)
  #
  include("${__zivc_dir__}/cmake/general.cmake")
  Zivc_setInternalValue(GLFW_BUILD_EXAMPLES OFF)
  Zivc_setInternalValue(GLFW_BUILD_TESTS OFF)
  Zivc_setInternalValue(GLFW_BUILD_DOCS OFF)
  Zivc_setInternalValue(GLFW_INSTALL OFF)
  Zivc_setInternalValue(GLFW_VULKAN_STATIC OFF)
  Zivc_setInternalValue(GLFW_USE_OSMESA OFF)
  Zivc_setInternalValue(GLFW_USE_HYBRID_HPG OFF)
  Zivc_setInternalValue(GLFW_USE_WAYLAND OFF)
  Zivc_setInternalValue(USE_MSVC_RUNTIME_LIBRARY_DLL ON)
  cmake_path(SET glfw_path "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/dependencies/glfw")
  Zivc_checkSubmodule("${glfw_path}")
  add_subdirectory("${glfw_path}" "${binary_dir}" EXCLUDE_FROM_ALL)
  Zivc_checkTarget(glfw)
  add_library(Glfw::glfw ALIAS glfw)
  # Check target properties
  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_populateTargetOptions(Zisc::Zisc glfw)
  target_include_directories(glfw SYSTEM INTERFACE "${glfw_path}/include")
  # Suppress warnings
  set(glfw_warning_flags "")
  if(Z_CLANG)
    list(APPEND glfw_warning_flags -Wno-unused-command-line-argument
                                   )
  endif()
  target_compile_options(glfw PRIVATE ${glfw_warning_flags})
endfunction(addGlfw)


function(addImgui)
  include("${__zivc_dir__}/cmake/general.cmake")
  cmake_path(SET imgui_path "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/dependencies/imgui")
  Zivc_checkSubmodule("${imgui_path}")
  #
  file(GLOB imgui_source_files ${imgui_path}/*[.h|.cpp])
  cmake_path(SET backend_dir "${imgui_path}/backends")
  list(APPEND imgui_source_files ${backend_dir}/imgui_impl_glfw.cpp
                                 ${backend_dir}/imgui_impl_glfw.h
                                 ${backend_dir}/imgui_impl_vulkan.cpp
                                 ${backend_dir}/imgui_impl_vulkan.h)
  source_group(${PROJECT_NAME}/imgui FILES ${imgui_source_files})

  #
  add_library(imgui STATIC ${imgui_source_files})
  add_library(Imgui::imgui ALIAS imgui)

  # Compiler flags
  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_populateTargetOptions(Zisc::Zisc imgui)
  Zivc_checkTarget(Glfw::glfw)
  Zivc_checkTarget(Zivc::Vulkan)
  target_link_libraries(imgui PRIVATE Zivc::Vulkan PUBLIC Glfw::glfw)
  target_compile_definitions(imgui PUBLIC IMGUI_IMPL_VULKAN_NO_PROTOTYPES)
  target_include_directories(imgui SYSTEM PUBLIC "${imgui_path}")
  # Suppress warnings
  set(imgui_warning_flags "")
  if(Z_GCC)
    list(APPEND imgui_warning_flags
                                   )
  endif()
  target_compile_options(imgui PRIVATE ${imgui_warning_flags})
endfunction(addImgui)


function(addGui)
  cmake_path(SET gui_path "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/gui")
  #
  file(GLOB_RECURSE gui_source_files ${gui_path}/*[.hpp|.cpp])
  source_group(${PROJECT_NAME}/gui FILES ${gui_source_files})

  #
  add_library(gui STATIC ${gui_source_files})
  add_library(Gui::gui ALIAS gui)

  # Compiler flags
  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_populateTargetOptions(Zisc::Zisc gui)
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getSanitizerFlags(compile_sanitizer_flags linker_sanitizer_flags)
  target_compile_options(gui PRIVATE ${cxx_compile_warning_flags}
                                     ${compile_sanitizer_flags})
  target_link_libraries(gui PRIVATE Zivc::Zivc PUBLIC Glfw::glfw Imgui::imgui)
  target_link_options(gui PRIVATE ${linker_sanitizer_flags})
  target_include_directories(gui INTERFACE ${gui_path})
  Zivc_enableIpo(gui)
  set(clang_tidy_exclusion_checks bugprone-branch-clone
                                  bugprone-exception-escape
                                  bugprone-reserved-identifier
                                  misc-no-recursion
                                  readability-function-cognitive-complexity
                                  readability-identifier-length
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
  source_group(${PROJECT_NAME}/${target} FILES ${example_source_files})

  #
  add_executable(${target} ${example_source_files})

  # Set properties
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20
                                             CXX_STANDARD_REQUIRED ON)
  include("${__zivc_dir__}/cmake/compiler.cmake")
  Zivc_getCxxWarningFlags(cxx_compile_warning_flags)
  Zivc_getSanitizerFlags(compile_sanitizer_flags linker_sanitizer_flags)
  getExampleWarningFlags(example_warning_flags)
  target_compile_options(${target} PRIVATE ${cxx_compile_warning_flags}
                                           ${compile_sanitizer_flags}
                                           ${example_warning_flags})
  target_link_libraries(${target} PRIVATE Zivc::Zivc ${ZIVC_DEPENDS})
  target_link_options(${target} PRIVATE ${linker_sanitizer_flags})
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
                                  readability-identifier-length
                                  readability-isolate-declaration
                                  readability-magic-numbers
                                  readability-named-parameter
                                  readability-static-accessed-through-instance
                                  readability-uppercase-literal-suffix)
  Zivc_setStaticAnalyzer(${target}
                         CLANG_TIDY_EXCLUSION_CHECKS ${clang_tidy_exclusion_checks})
  Zivc_createLinkToTarget(${target} ${PROJECT_BINARY_DIR})

  # Installation settings
  include("${__zivc_dir__}/cmake/packaging.cmake")
  # Save the dependency list
  if(save_dependency_list)
    Zivc_getSaveDependencyCode(${target} "${PROJECT_BINARY_DIR}" "" "" "" dependency_code)
    install(CODE ${dependency_code}
            COMPONENT example)
  endif()
  # Install actual binary
  install(TARGETS ${target}
          DESTINATION "${CMAKE_INSTALL_BINDIR}"
          COMPONENT example)
  Zivc_getDeployingNonSystemDependencyCode(${target} "${CMAKE_INSTALL_LIBDIR}" "" "" "" deploy_code)
  install(CODE ${deploy_code}
          COMPONENT example)
endfunction(addExample)


#
macro(setExampleProject)
  set(project_description "Examples of Zivc")
  project(Examples VERSION ${ZivcProject_VERSION} DESCRIPTION "${project_description}" LANGUAGES  CXX)

  # Initialize platform info
  include("${__zivc_dir__}/cmake/general.cmake")
  include("${__zivc_dir__}/cmake/platform.cmake")
  Zivc_getPlatformFlags(platform_definitions)
  Zivc_setVariablesOnCMake(${platform_definitions})

  # Check dependencies
  Zivc_checkTarget(Zivc::Zivc)

  include("${CMAKE_SOURCE_DIR}/source/zivc/cmake/kernel.cmake")
  # Build examples
  ## 1. context example
  addExample(ContextExample ON
             SOURCE_FILES "${PROJECT_SOURCE_DIR}/context_example.cpp")
  ## 2. buffer example
  addExample(BufferExample OFF
             SOURCE_FILES "${PROJECT_SOURCE_DIR}/buffer_example.cpp")
  ## 3. kernel example
  addExample(KernelExample OFF
             SOURCE_FILES "${PROJECT_SOURCE_DIR}/kernel_example.cpp")
  Zivc_addKernelSet(example ${PROJECT_VERSION} kernel_example_kernel
      SOURCE_FILES "kernels/example.cl")
  target_link_libraries(KernelExample PRIVATE ${kernel_example_kernel})

  # Demos using vulkan
  if(ZIVC_ENABLE_VULKAN_BACKEND)
    # Build dependencies
    addGlfw("${PROJECT_BINARY_DIR}/GLFW")
    addImgui()
    addGui()
    target_link_libraries(gui PRIVATE ${kernel_example_kernel})
    ## 4. vulkan backend example
    addExample(VulkanBackendExample OFF
               SOURCE_FILES "${PROJECT_SOURCE_DIR}/vulkan_backend_example.cpp")
    ## 5. GUI example
    addExample(GuiExample ON
               SOURCE_FILES "${PROJECT_SOURCE_DIR}/gui_example.cpp"
                            "${PROJECT_SOURCE_DIR}/cli.cpp"
               DEPENDS CLI11::CLI11 Gui::gui)
  endif()

  # Kernel set
endmacro(setExampleProject)


##
setExampleProject()
