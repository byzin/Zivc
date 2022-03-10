# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.22)


cmake_path(SET __zisc_dir__ "${CMAKE_SOURCE_DIR}/source/dependencies/Zisc")
cmake_path(SET __zivc_dir__ "${CMAKE_SOURCE_DIR}/source/zivc")


macro(setDocumentProject)
  set(project_description "Zivc documents")
  project(Document VERSION 0.0.1)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # Check dependencies
    include("${__zivc_dir__}/cmake/general.cmake")
    Zivc_checkTarget(Zivc)

    include("${__zivc_dir__}/cmake/document.cmake")

    # Doxygen custom properties
    # Project
    set(DOXYGEN_PROJECT_NAME Zivc)
    set(DOXYGEN_PROJECT_NUMBER 0.0.1)
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
    get_target_property(zivc_definitions Zivc COMPILE_DEFINITIONS)
    if(zivc_definitions)
      set(DOXYGEN_PREDEFINED ${zivc_definitions})
    endif()

    get_target_property(zivc_source_files Zivc SOURCES)
    Zivc_addDoxygenDoc(document "${PROJECT_BINARY_DIR}"
      SOURCE_FILES_OR_DIRS "${CMAKE_SOURCE_DIR}/README.md"
                           "${CMAKE_SOURCE_DIR}/LICENSE.md"
                           "${__zivc_dir__}/document.dox"
                           ${zivc_source_files})
  endif()
endmacro(setDocumentProject)


##
setDocumentProject()
