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
                                      VmaAllocationInfo* alloc_info)
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

  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = 0;
  switch (buffer_usage) {
   case BufferUsage::kDeviceOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    break;
   }
   case BufferUsage::kHostOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    break;
   }
   case BufferUsage::kHostToDevice: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    break;
   }
   case BufferUsage::kDeviceToHost: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
    break;
   }
  }
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = VK_NULL_HANDLE;
  alloc_create_info.pUserData = user_data;

  // 
  const auto binfo = zisc::cast<VkBufferCreateInfo>(buffer_create_info);
  const auto result = vmaCreateBuffer(device().memoryAllocator(),
                                      std::addressof(binfo),
                                      std::addressof(alloc_create_info),
                                      buffer,
                                      vm_allocation,
                                      alloc_info);
  if (result != VK_SUCCESS) {
    //! \todo Handle exception
    printf("[Warning]: Device memory allocation failed.\n");
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
                               const VkBufferCopy& region)
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
    [[maybe_unused]]VmaAllocationInfo* alloc_info) noexcept
{
  if (zivcvk::Buffer{*buffer}) {
    vmaDestroyBuffer(device().memoryAllocator(), *buffer, *vm_allocation);
  }
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
                                   const uint32b data) noexcept
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

  \return No description
  */
inline
VulkanDevice& VulkanBufferImpl::device() noexcept
{
  return *device_;
}

} // namespace zivc
