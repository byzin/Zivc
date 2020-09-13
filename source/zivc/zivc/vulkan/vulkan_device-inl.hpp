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
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
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
VkQueue& VulkanDevice::getQueue(const uint32b index) noexcept
{
  const std::size_t qindex = zisc::cast<std::size_t>(index) % numOfQueues();
  VkQueue& q = (*queue_list_)[qindex];
  return q;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
const VkQueue& VulkanDevice::getQueue(const uint32b index) const noexcept
{
  const std::size_t qindex = zisc::cast<std::size_t>(index) % numOfQueues();
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

  \tparam kN No description.
  \param [in] descriptor_set No description.
  \param [in] buffer_list No description.
  \param [in] desc_type_list No description.
  */
template <std::size_t kN> inline
void VulkanDevice::updateDescriptorSet(
    const VkDescriptorSet& descriptor_set,
    const std::array<VkBuffer, kN>& buffer_list,
    const std::array<VkDescriptorType, kN>& desc_type_list)
{
  std::array<VkDescriptorBufferInfo, kN> desc_info_list{};
  std::array<VkWriteDescriptorSet, kN> write_desc_list{};
  updateDescriptorSet(descriptor_set, kN, buffer_list.data(), desc_type_list.data(),
                      desc_info_list.data(), write_desc_list.data());
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

  \param [in] work_dimension No description.
  \param [in] work_size No description.
  \return No description
  */
inline
std::array<uint32b, 3> VulkanDevice::calcDispatchSize(
    const std::size_t work_dimension,
    const std::array<uint32b, 3>& work_size) const noexcept
{
  std::array<uint32b, 3> dispatch_size{{1, 1, 1}};
  const auto& group_size = workGroupSizeDim(work_dimension);
  for (std::size_t i = 0; i < work_dimension; ++i) {
    const uint32b s = (work_size[i] + group_size[i] - 1) / group_size[i];
    dispatch_size[i] = s;
  }
  return dispatch_size;
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
