# file: platform.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


cmake_path(SET __zisc_platform_cmake_path NORMALIZE
  "${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/platform.cmake")


function(Zivc_getPlatformFlags platform_definitions)
  include("${__zisc_platform_cmake_path}")
  Zisc_getPlatformFlags(platform_defs)

  # Output
  set(${platform_definitions} ${platform_defs} PARENT_SCOPE)
endfunction(Zivc_getPlatformFlags)


function(Zivc_findVulkan vulkan_target)
  find_package(Vulkan 1.2.198.1 REQUIRED)
  if(ZIVC_DYNAMIC_VULKAN_LOADING)
    set(target Vulkan::Headers)
  else()
    set(target Vulkan::Vulkan)
  endif()

  # Output
  set(${vulkan_target} ${target} PARENT_SCOPE)
endfunction(Zivc_findVulkan)
