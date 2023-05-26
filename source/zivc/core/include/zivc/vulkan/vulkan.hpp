/*!
  \file vulkan.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_HPP
#define ZIVC_VULKAN_HPP

#if defined(ZIVC_DYNAMIC_VULKAN_LOADING)
  #define VK_NO_PROTOTYPES 1
#endif // ZIVC_DYNAMIC_VULKAN_LOADING

// Enable vulkan platforms
#if defined(Z_WINDOWS)
  #define VK_USE_PLATFORM_WIN32_KHR 1
#elif defined(Z_MAC)
  #define VK_USE_PLATFORM_MACOS_MVK 1
  #define VK_USE_PLATFORM_METAL_EXT 1
#elif defined(Z_LINUX)
  #if __has_include(<X11/Xlib.h>)
    #define VK_USE_PLATFORM_XLIB_KHR 1
  #endif // xlib
  #if __has_include(<xcb/xcb.h>)
    #define VK_USE_PLATFORM_XCB_KHR 1
  #endif // xcb
  #if __has_include(<wayland-client.h>)
    #define VK_USE_PLATFORM_WAYLAND_KHR 1
  #endif // wayland
  #if !(defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_WAYLAND_KHR))
    #define ZIVC_VK_NO_PLATFORM 1
  #endif
#endif
#define VK_ENABLE_BETA_EXTENSIONS

// Vulkan
#include <vulkan/vulkan.h>
// For resolve build error on windows
#undef min
#undef max
// Zivc
#include "../../../zivc/zivc_config.hpp"

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

#include "../../../zivc/vulkan//utility/vulkan-inl.hpp"

#endif // ZIVC_VULKAN_HPP
