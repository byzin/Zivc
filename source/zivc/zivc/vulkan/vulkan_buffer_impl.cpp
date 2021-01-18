/*!
  \file vulkan_buffer_impl.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_buffer_impl.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/error.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] device No description.
  */
VulkanBufferImpl::VulkanBufferImpl(VulkanDevice* device) noexcept : device_{device}
{
}

/*!
  \details No detailed description
  */
VulkanBufferImpl::~VulkanBufferImpl() noexcept
{
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] buffer_usage No description.
  \param [in] desc_type No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::allocateMemory(const std::size_t size,
                                      const BufferUsage buffer_usage,
                                      const VkBufferUsageFlagBits desc_type,
                                      void* user_data,
                                      VkBuffer* buffer,
                                      VmaAllocation* vm_allocation,
                                      VmaAllocationInfo* alloc_info) const
{
  const auto binfo = makeBufferCreateInfo(size, desc_type);
  const auto alloc_create_info = makeAllocCreateInfo(buffer_usage, user_data);
  const auto result = vmaCreateBuffer(device().memoryAllocator(),
                                      std::addressof(binfo),
                                      std::addressof(alloc_create_info),
                                      buffer,
                                      vm_allocation,
                                      alloc_info);
  if (result != VK_SUCCESS) {
    const char* message = "Device memory allocation failed.";
    throwResultException(result, message);
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] source_buffer No description.
  \param [in] dest_buffer No description.
  \param [in] region No description.
  */
void VulkanBufferImpl::copyCmd(const VkCommandBuffer& command_buffer,
                               const VkBuffer& source_buffer,
                               const VkBuffer& dest_buffer,
                               const VkBufferCopy& region) const
{
  const auto loader = device().dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer source{source_buffer};
  ZISC_ASSERT(source, "The given source buffer is null.");
  const zivcvk::Buffer dest{dest_buffer};
  ZISC_ASSERT(dest, "The given dest buffer is null.");
  const zivcvk::BufferCopy copy_region{region};
  command.copyBuffer(source, dest, copy_region, *loader);
}

/*!
  \details No detailed description

  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [in,out] alloc_info No description.
  */
void VulkanBufferImpl::deallocateMemory(
    VkBuffer* buffer,
    VmaAllocation* vm_allocation,
    VmaAllocationInfo* alloc_info) const noexcept
{
  if (zivcvk::Buffer{*buffer}) {
    vmaDestroyBuffer(device().memoryAllocator(), *buffer, *vm_allocation);
  }
  alloc_info->deviceMemory = ZIVC_VK_NULL_HANDLE;
  alloc_info->offset = 0;
  alloc_info->size = 0;
  alloc_info->pMappedData = nullptr;
  alloc_info->pUserData = nullptr;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] buffer No description.
  \param [in] dest_offset No description.
  \param [in] size No description.
  \param [in] data No description.
  */
void VulkanBufferImpl::fillFastCmd(const VkCommandBuffer& command_buffer,
                                   const VkBuffer& buffer,
                                   const std::size_t dest_offset,
                                   const std::size_t size,
                                   const uint32b data) const noexcept
{
  const auto loader = device().dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer buf{buffer};
  ZISC_ASSERT(buf, "The given buffer is null.");
  command.fillBuffer(buf, dest_offset, size, data, *loader);
}

/*!
  \details No detailed description

  \param [in] buffer_usage No description.
  \param [in] desc_type No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::initAllocationInfo(const BufferUsage buffer_usage,
                                          const VkBufferUsageFlagBits desc_type,
                                          VmaAllocationInfo* alloc_info) const
{
  // Clear 
  {
    VkBuffer buffer = ZIVC_VK_NULL_HANDLE;
    deallocateMemory(std::addressof(buffer), nullptr, alloc_info);
  }

  const auto binfo = makeBufferCreateInfo(1, desc_type);
  const auto alloc_create_info = makeAllocCreateInfo(buffer_usage, nullptr);
  const auto result = vmaFindMemoryTypeIndexForBufferInfo(
      device().memoryAllocator(),
      std::addressof(binfo),
      std::addressof(alloc_create_info),
      std::addressof(alloc_info->memoryType));
  if (result != VK_SUCCESS) {
    const char* message = "Device memory allocation failed.";
    throwResultException(result, message);
  }
}

/*!
  \details No detailed description

  \param [in] result No description.
  \param [in] message No description.
  */
void VulkanBufferImpl::throwResultException(const VkResult result, const char* message)
{
  const auto r = zisc::cast<zivcvk::Result>(result);
  zivcvk::throwResultException(r, message);
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanDevice& VulkanBufferImpl::device() noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDevice& VulkanBufferImpl::device() const noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \return No description

  */
VmaAllocationCreateInfo VulkanBufferImpl::makeAllocCreateInfo(
    const BufferUsage buffer_usage,
    void* user_data) const noexcept
{
  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = 0;
  alloc_create_info.usage = toVmaUsage(buffer_usage);
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = ZIVC_VK_NULL_HANDLE;
  alloc_create_info.pUserData = user_data;

  return alloc_create_info;
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] desc_type No description.
  \return No description
  */
VkBufferCreateInfo VulkanBufferImpl::makeBufferCreateInfo(
    const std::size_t size,
    const VkBufferUsageFlagBits desc_type) const noexcept
{
  // Buffer create info
  zivcvk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  const auto descriptor_type = zisc::cast<zivcvk::BufferUsageFlagBits>(desc_type);
  buffer_create_info.usage = zivcvk::BufferUsageFlagBits::eTransferSrc |
                             zivcvk::BufferUsageFlagBits::eTransferDst |
                             descriptor_type;
  buffer_create_info.sharingMode = zivcvk::SharingMode::eExclusive;
  const uint32b queue_family_index = device().queueFamilyIndex();
  buffer_create_info.queueFamilyIndexCount = 1;
  buffer_create_info.pQueueFamilyIndices = std::addressof(queue_family_index);

  return zisc::cast<VkBufferCreateInfo>(buffer_create_info);
}

/*!
  \details No detailed description

  \param [in] usage No description.
  \return No description
  */
inline
constexpr VmaMemoryUsage VulkanBufferImpl::toVmaUsage(const BufferUsage usage) noexcept
{
  VmaMemoryUsage mem_usage = VMA_MEMORY_USAGE_UNKNOWN;
  switch (usage) {
   case BufferUsage::kDeviceOnly: {
    mem_usage = VMA_MEMORY_USAGE_GPU_ONLY;
    break;
   }
   case BufferUsage::kHostOnly: {
    mem_usage = VMA_MEMORY_USAGE_CPU_ONLY;
    break;
   }
   case BufferUsage::kHostToDevice: {
    mem_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    break;
   }
   case BufferUsage::kDeviceToHost: {
    mem_usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
    break;
   }
  }
  return mem_usage;
}

} // namespace zivc
