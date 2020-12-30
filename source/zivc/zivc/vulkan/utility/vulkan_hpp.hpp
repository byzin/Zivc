/*!
  \file vulkan_hpp.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_HPP_HPP
#define ZIVC_VULKAN_HPP_HPP

// Zivc
#include "vulkan.hpp"
#include "zivc/zivc_config.hpp"

// Namespace
#define VULKAN_HPP_NAMESPACE zivcvk
// Disable the smart handler of the Vulkan-HPP
#define VULKAN_HPP_NO_SMART_HANDLE 1
#define VULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST 1
// Dynamic loading
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#if defined(ZIVC_DYNAMIC_VULKAN_LOADING)
  #define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#else // ZIVC_DYNAMIC_VULKAN_LOADING
  #define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#endif // ZIVC_DYNAMIC_VULKAN_LOADING
// Prohibit default dispatcher
#define VULKAN_HPP_DEFAULT_DISPATCHER int{0}
#define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE int

// Vulkan
#include <vulkan/vulkan.hpp>

#endif // ZIVC_VULKAN_HPP_HPP
