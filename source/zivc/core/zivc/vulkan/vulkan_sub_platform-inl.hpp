/*!
  \file vulkan_sub_platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_SUB_PLATFORM_INL_HPP
#define ZIVC_VULKAN_SUB_PLATFORM_INL_HPP

#include "vulkan_sub_platform.hpp"
// Standard C++ library
#include <cstddef>
#include <limits>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/data_structure/bounded_bst.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/memory_heap_info.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<VkPhysicalDevice>& VulkanSubPlatform::deviceList() const noexcept
{
  return *device_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<VulkanDeviceInfo>& VulkanSubPlatform::deviceInfoList() const noexcept
{
  return *device_info_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDispatchLoader& VulkanSubPlatform::dispatcher() const noexcept
{
  return *dispatcher_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view VulkanSubPlatform::engineName() const noexcept
{
  std::string_view engine_name{engine_name_};
  return engine_name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::extensionPropertiesList() const noexcept
    -> const zisc::pmr::vector<VkExtensionProperties>&
{
  return *extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanSubPlatform::hasOwnInstance() const noexcept
{
  const bool result = instance_ != ZIVC_VK_NULL_HANDLE;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VkInstance& VulkanSubPlatform::instance() noexcept
{
  return *instance_ref_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VkInstance& VulkanSubPlatform::instance() const noexcept
{
  return *instance_ref_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::layerPropertiesList() const noexcept
    -> const zisc::pmr::vector<VkLayerProperties>&
{
  return *layer_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::AllocatorData::memoryList() noexcept
    -> zisc::pmr::vector<MemoryData>&
{
  return mem_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::AllocatorData::memoryList() const noexcept
    -> const zisc::pmr::vector<MemoryData>&
{
  return mem_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::AllocatorData::memoryMap() noexcept -> MemoryMap&
{
  return mem_map_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* VulkanSubPlatform::AllocatorData::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t VulkanSubPlatform::AllocatorData::mapCapacity() noexcept
{
  const std::size_t c = (std::numeric_limits<uint16b>::max)(); //!< Is it enough?
  return c;
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \param [in] heap_index No description.
  \param [in] size No description.
  */
inline
void VulkanSubPlatform::notifyOfDeviceMemoryAllocation(
    const std::size_t device_index,
    const std::size_t heap_index,
    const std::size_t size) noexcept
{
  VulkanDeviceInfo& device_info = (*device_info_list_)[device_index];
  MemoryHeapInfo& heap_info = device_info.heapInfo(heap_index);
  zisc::Memory::Usage& usage = heap_info.usedSizeForBuffer();
  usage.add(size);
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \param [in] heap_index No description.
  \param [in] size No description.
  */
inline
void VulkanSubPlatform::notifyOfDeviceMemoryDeallocation(
    const std::size_t device_index,
    const std::size_t heap_index,
    const std::size_t size) noexcept
{
  VulkanDeviceInfo& device_info = (*device_info_list_)[device_index];
  MemoryHeapInfo& heap_info = device_info.heapInfo(heap_index);
  zisc::Memory::Usage& usage = heap_info.usedSizeForBuffer();
  usage.release(size);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanSubPlatform::windowSurfaceType() const noexcept -> WindowSurfaceType
{
  return window_surface_type_;
}

} // namespace zivc

#endif // ZIVC_VULKAN_SUB_PLATFORM_INL_HPP
