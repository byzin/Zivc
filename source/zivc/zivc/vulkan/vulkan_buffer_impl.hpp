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
                      VmaAllocationInfo* alloc_info) const;

  //! Perform copy operation of buffer memory
  void copyCmd(const VkCommandBuffer& command_buffer,
               const VkBuffer& source_buffer,
               const VkBuffer& dest_buffer,
               const VkBufferCopy& region) const;

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) const noexcept;

  //! Fill the given buffer with the specified value
  void fillFastCmd(const VkCommandBuffer& command_buffer,
                   const VkBuffer& buffer,
                   const std::size_t dest_offset,
                   const std::size_t size,
                   const uint32b data) const noexcept;

  //! Initialize allocation info
  void initAllocationInfo(const BufferUsage buffer_usage,
                          const VkBufferUsageFlagBits desc_type,
                          VmaAllocationInfo* alloc_info) const;

  //!
  [[noreturn]] static void throwResultException(const VkResult result,
                                                const char* message);

 private:
  //! Return the underlying device object
  VulkanDevice& device() noexcept;

  //! Return the underlying device object
  const VulkanDevice& device() const noexcept;

  //! Create a allocation create info
  VmaAllocationCreateInfo makeAllocCreateInfo(
      const BufferUsage buffer_usage,
      void* user_data) const noexcept;

  //! Create a buffer create info
  VkBufferCreateInfo makeBufferCreateInfo(
      const std::size_t size,
      const VkBufferUsageFlagBits desc_type) const noexcept;

  //! Convert to VMA usage flags
  static constexpr VmaMemoryUsage toVmaUsage(const BufferUsage usage) noexcept;


  VulkanDevice* device_ = nullptr;
};

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_IMPL_HPP
