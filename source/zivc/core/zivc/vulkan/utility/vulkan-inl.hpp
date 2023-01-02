/*!
  \file vulkan-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_INL_HPP
#define ZIVC_VULKAN_INL_HPP

#include "vulkan.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

static_assert(VK_NULL_HANDLE == nullptr, "'ZIVC_VK_NULL_HANDLE' isn't null.");

namespace zivc {

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \param [in] version No description.
  \return No description
  */
inline
constexpr uint32b vkGetVersionMajor(const uint32b version) noexcept
{
  return VK_VERSION_MAJOR(version);
}

/*!
  \details No detailed description

  \param [in] version No description.
  \return No description
  */
inline
constexpr uint32b vkGetVersionMinor(const uint32b version) noexcept
{
  return VK_VERSION_MINOR(version);
}

/*!
  \details No detailed description

  \param [in] version No description.
  \return No description
  */
inline
constexpr uint32b vkGetVersionPatch(const uint32b version) noexcept
{
  return VK_VERSION_PATCH(version);
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b vkGetVulkanApiVersion() noexcept
{
  return VK_API_VERSION_1_3;
}

/*!
  \details No detailed description

  \param [in] major No description.
  \param [in] minor No description.
  \param [in] patch No description.
  \return No description
  */
inline
constexpr uint32b vkMakeVersion(const uint32b major,
                                const uint32b minor,
                                const uint32b patch) noexcept
{
  return VK_MAKE_VERSION(major, minor, patch);
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

} // namespace zivc

#endif // ZIVC_VULKAN_INL_HPP
