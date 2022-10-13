/*!
  \file vulkan_backend-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BACKEND_INL_HPP
#define ZIVC_VULKAN_BACKEND_INL_HPP

#include "vulkan_backend.hpp"
// Standard C++ library
#include <cstddef>
#include <limits>
#include <span>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/structure/map.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/memory_heap_info.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
std::span<const VulkanDeviceInfo> VulkanBackend::deviceInfoList() const noexcept
{
  return *device_info_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::span<const VkPhysicalDevice> VulkanBackend::deviceList() const noexcept
{
  return *device_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDispatchLoader& VulkanBackend::dispatcher() const noexcept
{
  return *dispatcher_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view VulkanBackend::engineName() const noexcept
{
  std::string_view engine_name{engine_name_};
  return engine_name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanBackend::extensionPropertiesList() const noexcept
    -> std::span<const VkExtensionProperties>
{
  return *extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanBackend::hasOwnInstance() const noexcept
{
  const bool result = instance_ != ZIVC_VK_NULL_HANDLE;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VkInstance& VulkanBackend::instance() noexcept
{
  return *instance_ref_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VkInstance& VulkanBackend::instance() const noexcept
{
  return *instance_ref_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanBackend::layerPropertiesList() const noexcept
    -> std::span<const VkLayerProperties>
{
  return *layer_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanBackend::AllocatorData::memoryList() noexcept
    -> std::span<MemoryData>
{
  return mem_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanBackend::AllocatorData::memoryList() const noexcept
    -> std::span<const MemoryData>
{
  return mem_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanBackend::AllocatorData::memoryMap() noexcept -> MemoryMap&
{
  return mem_map_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* VulkanBackend::AllocatorData::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t VulkanBackend::AllocatorData::mapCapacity() noexcept
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
void VulkanBackend::notifyOfDeviceMemoryAllocation(
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
void VulkanBackend::notifyOfDeviceMemoryDeallocation(
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
auto VulkanBackend::windowSurfaceType() const noexcept -> WindowSurfaceType
{
  return window_surface_type_;
}

} // namespace zivc

#endif // ZIVC_VULKAN_BACKEND_INL_HPP
