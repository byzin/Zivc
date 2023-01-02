# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.22)


cmake_path(SET __zivc_root_dir__ "${CMAKE_CURRENT_LIST_DIR}/..")
cmake_path(SET __zisc_dir__ "${__zivc_root_dir__}/source/dependencies/Zisc")
cmake_path(SET __zivc_dir__ "${__zivc_root_dir__}/source/zivc")


macro(setDocumentProject)
  set(project_description "Zivc documents")
  project(Document VERSION ${ZivcProject_VERSION})
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    include("${__zivc_dir__}/cmake/document.cmake")

    # Doxygen custom properties
    # Project
    set(DOXYGEN_PROJECT_NAME Zivc)
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF "Zivc is cross-platform heterogeneous compuling library.")

    # HTML
    cmake_path(SET html_theme_path "${__zisc_dir__}/document/doxygen_dark_theme")
    if(EXISTS ${html_theme_path})
      set(DOXYGEN_HTML_HEADER ${html_theme_path}/html_header.html)
      set(DOXYGEN_HTML_FOOTER ${html_theme_path}/html_footer.html)
      set(DOXYGEN_HTML_EXTRA_STYLESHEET ${html_theme_path}/custom_dark_theme.css)
    endif()

    # Example
    set(DOXYGEN_EXAMPLE_PATH "${CMAKE_SOURCE_DIR}/example")
    set(DOXYGEN_EXAMPLE_RECURSIVE YES)

    # Compile flags
    get_target_property(zivc_definitions Zivc::Zivc COMPILE_DEFINITIONS)
    if(zivc_definitions)
      set(DOXYGEN_PREDEFINED ${zivc_definitions})
    endif()

    get_target_property(zivc_source_files Zivc::Zivc SOURCES)
    Zivc_addDoxygenDoc(document "${PROJECT_BINARY_DIR}"
      SOURCE_FILES_OR_DIRS "${CMAKE_SOURCE_DIR}/README.md"
                           "${CMAKE_SOURCE_DIR}/LICENSE.md"
                           "${__zivc_dir__}/document.dox"
                           ${zivc_source_files})
  endif()
endmacro(setDocumentProject)


##
setDocumentProject()
