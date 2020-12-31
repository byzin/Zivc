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
#include <array>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam SetType No description.
  */
template <typename SetType> inline
void VulkanDevice::addShaderModule(const KernelSet<SetType>& kernel_set)
{
  const uint32b id = kernel_set.id();
  if (!hasShaderModule(id)) {
    zisc::pmr::vector<uint32b>::allocator_type alloc{memoryResource()};
    zisc::pmr::vector<uint32b> spirv_code{alloc};
    kernel_set.loadSpirVCode(std::addressof(spirv_code));
    const std::string_view module_name = kernel_set.name();
    addShaderModule(id, spirv_code, module_name);
  }
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

  \param [in] index No description.
  \return No description
  */
inline
VkQueue& VulkanDevice::getQueue(const std::size_t index) noexcept
{
  const std::size_t qindex = index % numOfQueues();
  VkQueue& q = (*queue_list_)[qindex];
  return q;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
const VkQueue& VulkanDevice::getQueue(const std::size_t index) const noexcept
{
  const std::size_t qindex = index % numOfQueues();
  const VkQueue& q = (*queue_list_)[qindex];
  return q;
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
auto VulkanDevice::getShaderModule(const uint32b id) const noexcept -> const ModuleData&
{
  ZISC_ASSERT(hasShaderModule(id), "Shader module not found. id = ", id);
  auto module = shader_module_list_->find(id);
  return module->second;
}

/*!
  \details No detailed description

  \param [in] id No description.
  \return No description
  */
inline
bool VulkanDevice::hasShaderModule(const uint32b id) const noexcept
{
  auto module = shader_module_list_->find(id);
  const bool result = module != shader_module_list_->end();
  return result;
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
CmdDebugLabelRegion VulkanDevice::makeCmdDebugLabel(
    const VkCommandBuffer& command_buffer,
    const Options& options) const noexcept
{
  auto region = makeCmdDebugLabel(command_buffer,
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
QueueDebugLabelRegion VulkanDevice::makeQueueDebugLabel(
    const VkQueue& q,
    const Options& options) const noexcept
{
  auto region = makeQueueDebugLabel(q,
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
uint32b VulkanDevice::queueFamilyIndex() const noexcept
{
  return queue_family_index_;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
const std::array<uint32b, 3>& VulkanDevice::workGroupSizeDim(const std::size_t dimension) const noexcept
{
  return work_group_size_list_[dimension - 1];
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanSubPlatform& VulkanDevice::parentImpl() noexcept
{
  auto p = getParent();
  return *zisc::reinterp<VulkanSubPlatform*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanSubPlatform& VulkanDevice::parentImpl() const noexcept
{
  const auto p = getParent();
  return *zisc::reinterp<const VulkanSubPlatform*>(p);
}

} // namespace zivc

#endif // ZIVC_VULKAN_DEVICE_INL_HPP
