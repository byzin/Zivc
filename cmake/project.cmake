# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


function(populateTargetOptions source_target dest_target)
  include(${zisc_path}/cmake/general.cmake)
  Zisc_checkTarget(${source_target})
  Zisc_checkTarget(${dest_target})
  get_target_property(zivc_compile_flags ${source_target} INTERFACE_COMPILE_OPTIONS)
  if(zivc_compile_flags)
    target_compile_options(${dest_target} PRIVATE ${zivc_compile_flags})
  endif()
  get_target_property(zivc_libraries ${source_target} INTERFACE_LINK_LIBRARIES)
  if(zivc_libraries)
    target_link_libraries(${dest_target} PRIVATE ${zivc_libraries})
  endif()
  get_target_property(zivc_linker_flags ${source_target} INTERFACE_LINK_OPTIONS)
  if(zivc_linker_flags)
    target_link_options(${dest_target} PRIVATE ${zivc_linker_flags})
  endif()
endfunction(populateTargetOptions)


function(addZiscGoogleTest)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  Zisc_addGoogleTest(${PROJECT_SOURCE_DIR}/test/dependencies/googletest
                     ${PROJECT_BINARY_DIR}/GoogleTest)
  populateTargetOptions(Zisc gtest)
  populateTargetOptions(Zisc gtest_main)
endfunction(addZiscGoogleTest)
