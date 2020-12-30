/*!
  \file vulkan.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_HPP
#define ZIVC_VULKAN_HPP

#if defined(ZIVC_DYNAMIC_VULKAN_LOADING)
  #define VK_NO_PROTOTYPES 1
#endif // ZIVC_DYNAMIC_VULKAN_LOADING

#include <vulkan/vulkan.h>

#endif // ZIVC_VULKAN_HPP
