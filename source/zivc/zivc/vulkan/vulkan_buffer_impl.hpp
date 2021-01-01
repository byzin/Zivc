/*!
  \file vulkan_buffer_impl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_IMPL_HPP
#define ZIVC_VULKAN_BUFFER_IMPL_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanBufferImpl : private zisc::NonCopyable<VulkanBufferImpl>
{
 public:
  //! Initialize the vulkan buffer impl
  VulkanBufferImpl(VulkanDevice* device) noexcept;

  //! Finalize the vulkan buffer impl
  ~VulkanBufferImpl() noexcept;


  //! Allocate a memory for the device
  void allocateMemory(const std::size_t size,
                      const BufferUsage buffer_usage,
                      const VkBufferUsageFlagBits desc_type,
                      void* user_data,
                      VkBuffer* buffer,
                      VmaAllocation* vm_allocation,
                      VmaAllocationInfo* alloc_info);

  //! Perform copy operation of buffer memory
  void copyCmd(const VkCommandBuffer& command_buffer,
               const VkBuffer& source_buffer,
               const VkBuffer& dest_buffer,
               const VkBufferCopy& region);

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) noexcept;

  //! Fill the given buffer with the specified value
  void fillFastCmd(const VkCommandBuffer& command_buffer,
                   const VkBuffer& buffer,
                   const std::size_t dest_offset,
                   const std::size_t size,
                   const uint32b data) noexcept;

 private:
  //! Return the underlying device object
  VulkanDevice& device() noexcept;


  VulkanDevice* device_ = nullptr;
};

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_IMPL_HPP
