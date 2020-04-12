/*!
  \file vulkan_device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_INL_HPP
#define ZIVC_VULKAN_DEVICE_INL_HPP

#include "vulkan_device.hpp"
// Standard C++ library
#include <limits>
#include <memory>
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
VkDevice& VulkanDevice::device() noexcept
{
  return device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VkDevice& VulkanDevice::device() const noexcept
{
  return device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDeviceInfo& VulkanDevice::deviceInfoData() const noexcept
{
  const auto& info = deviceInfo();
  return *zisc::cast<const VulkanDeviceInfo*>(std::addressof(info));
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDispatchLoader& VulkanDevice::dispatcher() const noexcept
{
  return *dispatcher_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b VulkanDevice::invalidQueueIndex() noexcept
{
  const uint32b index = std::numeric_limits<uint32b>::max();
  return index;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VmaAllocator& VulkanDevice::memoryAllocator() noexcept
{
  return vm_allocator_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VmaAllocator& VulkanDevice::memoryAllocator() const noexcept
{
  return vm_allocator_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanSubPlatform& VulkanDevice::parentImpl() noexcept
{
  auto p = getParent();
  return *zisc::treatAs<VulkanSubPlatform*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanSubPlatform& VulkanDevice::parentImpl() const noexcept
{
  const auto p = getParent();
  return *zisc::treatAs<const VulkanSubPlatform*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b VulkanDevice::queueFamilyIndex() const noexcept
{
  return queue_family_index_;
}

} // namespace zivc

#endif // ZIVC_VULKAN_DEVICE_INL_HPP
