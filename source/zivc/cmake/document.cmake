# file: document.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


set(__zisc_document_cmake_path
    ${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/document.cmake)


function(Zivc_addDoxygenDoc target destination)
  include(${__zisc_document_cmake_path})
  Zisc_addDoxygenDoc(${target} "${destination}" ${ARGN})
endfunction(Zivc_addDoxygenDoc)
