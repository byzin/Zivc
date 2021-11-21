/*!
  \file vulkan_dispatch_loader-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DISPATCH_LOADER_INL_HPP
#define ZIVC_VULKAN_DISPATCH_LOADER_INL_HPP

#include "vulkan_dispatch_loader.hpp"
// Standard C++ library
#include <memory>

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isAvailable() const noexcept
{
  const bool result = loaderImpl() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isDispatchableForDevice() const noexcept
{
  const bool result = deviceProcAddr() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isDispatchableForInstance() const noexcept
{
  const bool result = instanceProcAddr() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loaderImpl() noexcept -> LoaderImplPtr
{
  return loader_impl_.get();
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loaderImpl() const noexcept -> ConstLoaderImplPtr
{
  return loader_impl_.get();
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
  const bool result = lhs.loaderImpl() == rhs.loaderImpl();
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
