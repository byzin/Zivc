/*!
  \file vulkan.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_HPP
#define ZIVC_VULKAN_HPP

#if defined(ZIVC_DYNAMIC_VULKAN_LOADING)
  #define VK_NO_PROTOTYPES 1
#endif // ZIVC_DYNAMIC_VULKAN_LOADING

#define VK_ENABLE_BETA_EXTENSIONS

// Vulkan
#include <vulkan/vulkan.h>
// Zivc
#include "zivc/zivc_config.hpp"

//! Represent a non-valid object handle
#define ZIVC_VK_NULL_HANDLE nullptr

namespace zivc {

//! Return the major version from the given version number
constexpr uint32b vkGetVersionMajor(const uint32b version) noexcept;

//! Return the minor version from the given version number
constexpr uint32b vkGetVersionMinor(const uint32b version) noexcept;

//! Return the patch version from the given version number
constexpr uint32b vkGetVersionPatch(const uint32b version) noexcept;

//! Return the Vulkan API version number which used in Zivc
constexpr uint32b vkGetVulkanApiVersion() noexcept;

//! Construct an API version number
constexpr uint32b vkMakeVersion(const uint32b major,
                                const uint32b minor,
                                const uint32b patch) noexcept;

} // namespace zivc

#include "vulkan-inl.hpp"

#endif // ZIVC_VULKAN_HPP
