/*!
  \file vulkan_dispatch_loader-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DISPATCH_LOADER_INL_HPP
#define ZIVC_VULKAN_DISPATCH_LOADER_INL_HPP

#include "zivc/vulkan/vulkan_dispatch_loader.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/utility.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isAvailable() const noexcept
{
  const bool result = zisc::cast<bool>(loader_impl_);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isDispatchableForDevice() const noexcept
{
  const bool result = isAvailable() && (deviceProcAddr() != nullptr);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isDispatchableForInstance() const noexcept
{
  const bool result = isAvailable() && (instanceProcAddr() != nullptr);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loader() noexcept -> LoaderReference
{
  return *loader_impl_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loader() const noexcept -> ConstLoaderReference
{
  return *loader_impl_;
}

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator==(const VulkanDispatchLoader& lhs,
                const VulkanDispatchLoader& rhs) noexcept
{
  const bool result = lhs.isAvailable() &&
                      rhs.isAvailable() &&
                      (std::addressof(lhs.loader()) == std::addressof(rhs.loader()));
  return result;
}

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator!=(const VulkanDispatchLoader& lhs,
                const VulkanDispatchLoader& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

} // namespace zivc

#endif // ZIVC_VULKAN_DISPATCH_LOADER_INL_HPP
