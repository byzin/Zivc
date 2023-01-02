/*!
  \file vulkan_device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_INL_HPP
#define ZIVC_VULKAN_DEVICE_INL_HPP

#include "vulkan_device.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <limits>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/structure/map.hpp"
#include "zisc/structure/queue.hpp"
// Zivc
#include "vulkan_backend.hpp"
#include "vulkan_device_info.hpp"
#include "internal/cmd_debug_label_region.hpp"
#include "internal/cmd_record_region.hpp"
#include "internal/queue_debug_label_region.hpp"
#include "internal/vulkan_memory_allocator.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/error.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam SetType No description.
  */
template <typename SetType> inline
auto VulkanDevice::addShaderModule(KernelSet<SetType>& kernel_set) -> const ModuleData&
{
  const uint64b id = kernel_set.id();
  if (hasShaderModule(id))
    return getShaderModule(id);

  kernel_set.initialize();
  const std::span<const uint32b> spirv_code = kernel_set.spirVCode();
  const std::string_view module_name = kernel_set.name();
  return addShaderModule(id, spirv_code, module_name);
}

/*!
  \details No detailed description

  \return No description
  */
inline
VkCommandPool& VulkanDevice::commandPool() noexcept
{
  return command_pool_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VkCommandPool& VulkanDevice::commandPool() const noexcept
{
  return command_pool_;
}

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
const VulkanDeviceInfo& VulkanDevice::deviceInfoImpl() const noexcept
{
  const DeviceInfo& info = deviceInfo();
  return *static_cast<const VulkanDeviceInfo*>(std::addressof(info));
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

  \param [in] cap No description.
  \param [in] index No description.
  \return No description
  */
inline
VkQueue& VulkanDevice::getQueue(const CapabilityT cap,
                                const std::size_t index) noexcept
{
  ZIVC_ASSERT(hasCapability(cap), "Unsupported capability is specified in getQueue.");
  const std::size_t qindex = index % numOfQueues(cap);
  const std::size_t qoffset = queueOffset(cap);
  VkQueue& q = (*queue_list_)[qindex + qoffset];
  return q;
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \param [in] index No description.
  \return No description
  */
inline
const VkQueue& VulkanDevice::getQueue(const CapabilityT cap,
                                      const std::size_t index) const noexcept
{
  ZIVC_ASSERT(hasCapability(cap), "Unsupported capability is specified in getQueue.");
  const std::size_t qindex = index % numOfQueues(cap);
  const std::size_t qoffset = queueOffset(cap);
  const VkQueue& q = (*queue_list_)[qindex + qoffset];
  return q;
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
auto VulkanDevice::getShaderKernel(const uint64b id) const noexcept -> const KernelData&
{
  const std::optional<std::size_t> index = kernelDataList().contain(id);
  ZIVC_ASSERT(index.has_value(), "Kernel data not found. id = ", id);
  const KernelData& data = *kernelDataList().get(*index).second;
  return data;
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
auto VulkanDevice::getShaderModule(const uint64b id) const noexcept -> const ModuleData&
{
  const std::optional<std::size_t> index = moduleDataList().contain(id);
  ZIVC_ASSERT(index.has_value(), "Shader module not found. id = ", id);
  const ModuleData& data = *moduleDataList().get(*index).second;
  return data;
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \return No description
  */
inline
bool VulkanDevice::hasCapability(const CapabilityT cap) const noexcept
{
  const uint32b mask = 0b01u << static_cast<uint32b>(cap);
  const bool flag = ((capabilities_ & mask) == mask);
  return flag;
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
bool VulkanDevice::hasShaderKernel(const uint64b id) const noexcept
{
  const std::optional<std::size_t> result = kernelDataList().contain(id);
  return result.has_value();
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
bool VulkanDevice::hasShaderModule(const uint64b id) const noexcept
{
  const std::optional<std::size_t> result = moduleDataList().contain(id);
  return result.has_value();
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b VulkanDevice::invalidQueueIndex() noexcept
{
  const uint32b index = (std::numeric_limits<uint32b>::max)();
  return index;
}

/*!
  \details No detailed description

  \tparam Options No description.
  \param [in] command_buffer No description.
  \param [in] options No description.
  \return No description
  */
template <LabelOptions Options> inline
internal::CmdDebugLabelRegion VulkanDevice::makeCmdDebugLabel(
    const VkCommandBuffer& command_buffer,
    const Options& options) const noexcept
{
  internal::CmdDebugLabelRegion region = makeCmdDebugLabel(command_buffer,
                                                           options.label(),
                                                           options.labelColor());
  return region;
}

/*!
  \details No detailed description

  \tparam Options No description.
  \param [in] q No description.
  \param [in] options No description.
  \return No description
  */
template <LabelOptions Options> inline
internal::QueueDebugLabelRegion VulkanDevice::makeQueueDebugLabel(
    const VkQueue& q,
    const Options& options) const noexcept
{
  internal::QueueDebugLabelRegion region = makeQueueDebugLabel(q,
                                                               options.label(),
                                                               options.labelColor());
  return region;
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
std::size_t VulkanDevice::numOfQueues(const CapabilityT cap) const noexcept
{
  const std::size_t index = getCapabilityIndex(cap);
  const auto n = zisc::cast<std::size_t>(queue_count_list_[index]);
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b VulkanDevice::queueFamilyIndex(const CapabilityT cap) const noexcept
{
  const std::size_t index = getCapabilityIndex(cap);
  return queue_family_index_list_[index];
}

/*!
  \details No detailed description

  \param [in] dim No description.
  \return No description
  */
inline
std::span<const uint32b, 3> VulkanDevice::workGroupSizeDim(const std::size_t dim) const noexcept
{
  return work_group_size_list_[dim - 1];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
constexpr auto VulkanDevice::getCapability(const std::size_t index) noexcept
    -> CapabilityT
{
  return static_cast<CapabilityT>(index);
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \return No description
  */
inline
constexpr std::size_t VulkanDevice::getCapabilityIndex(const CapabilityT cap) noexcept
{
  return static_cast<std::size_t>(cap);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::fenceIndexQueue() noexcept -> IndexQueueT&
{
  return *fence_index_queue_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::fenceIndexQueue() const noexcept -> const IndexQueueT&
{
  return *fence_index_queue_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanBackend& VulkanDevice::parentImpl() noexcept
{
  ZivcObject* p = getParent();
  return *static_cast<VulkanBackend*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanBackend& VulkanDevice::parentImpl() const noexcept
{
  const ZivcObject* p = getParent();
  return *static_cast<const VulkanBackend*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::kernelDataList() noexcept
    -> ShaderMapT<UniqueKernelDataT>&
{
  return *kernel_data_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::kernelDataList() const noexcept
    -> const ShaderMapT<UniqueKernelDataT>&
{
  return *kernel_data_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::moduleDataList() noexcept
    -> ShaderMapT<UniqueModuleDataT>&
{
  return *module_data_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDevice::moduleDataList() const noexcept
    -> const ShaderMapT<UniqueModuleDataT>&
{
  return *module_data_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t VulkanDevice::numOfCapabilities() noexcept
{
  return kNumOfCapabilities;
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \return No description
  */
inline
std::size_t VulkanDevice::queueOffset(const CapabilityT cap) const noexcept
{
  const std::size_t index = getCapabilityIndex(cap);
  const auto offset = zisc::cast<std::size_t>(queue_offset_list_[index]);
  return offset;
}

} // namespace zivc

#endif // ZIVC_VULKAN_DEVICE_INL_HPP
