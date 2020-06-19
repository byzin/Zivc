/*!
  \file vulkan_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_HPP
#define ZIVC_VULKAN_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;

template <typename T>
class VulkanBuffer : public Buffer<T>
{
 public:
  // Type aliases
  using Type = typename Buffer<T>::Type;
  using ConstType = typename Buffer<T>::ConstType;
  using Pointer = typename Buffer<T>::Pointer;
  using ConstPointer = typename Buffer<T>::ConstPointer;


  //! Initialize the buffer
  VulkanBuffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~VulkanBuffer() noexcept override;


  //! Return the allocation data
  VmaAllocation& allocation() noexcept;

  //! Return the allocation data
  const VmaAllocation& allocation() const noexcept;

  //! Return the allocation ifno
  const VmaAllocationInfo& allocationInfo() const noexcept;

  //! Return the buffer data
  VkBuffer& buffer() noexcept;

  //! Return the buffer data
  const VkBuffer& buffer() const noexcept;

  //! Check if the buffer is the most efficient for the device access
  bool isDeviceLocal() const noexcept override;

  //! Check if the buffer is cached on the host
  bool isHostCached() const noexcept override;

  //! Check if the buffer doesn't need to be unmapped
  bool isHostCoherent() const noexcept override;

  //! Check if the buffer can be mapped for the host access
  bool isHostVisible() const noexcept override;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the number of elements
  std::size_t size() const noexcept override;

 protected:
  //! Clear the contents of the buffer
  void destroyData() noexcept override;

  //! Initialize the buffer
  void initData() override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  //! Check if the buffer has the given memory property flag
  bool hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept;

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;


  VkBuffer buffer_ = VK_NULL_HANDLE;
  VmaAllocation vm_allocation_ = VK_NULL_HANDLE;
  VmaAllocationInfo vm_alloc_info_;
};

} // namespace zivc

#include "vulkan_buffer-inl.hpp"

#endif // ZIVC_VULKAN_BUFFER_HPP

///*!
//  \file vulkan_buffer.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZIVC_VULKAN_BUFFER_HPP
//#define ZIVC_VULKAN_BUFFER_HPP
//
//// Standard C++ library
//#include <cstddef>
//// Vulkan
//#include <vulkan/vulkan.hpp>
//#include "vk_mem_alloc.h"
//// Zivc
//#include "zivc/buffer.hpp"
//#include "zivc/zivc_config.hpp"
//
//namespace zivc {
//
//// Forward declaration
//class VulkanDevice;
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T>
//class VulkanBuffer : public Buffer<kDescriptor, T>
//{
// public:
//  //! The type of the buffer. "const", "volatile" and "reference" are removed
//  using Type = typename Buffer<kDescriptor, T>::Type;
//  using Pointer = typename Buffer<kDescriptor, T>::Pointer;
//  using ConstPointer = typename Buffer<kDescriptor, T>::ConstPointer;
//
//
//  //! Create an empty buffer
//  VulkanBuffer(const VulkanDevice* device,
//               const BufferUsage usage_flag) noexcept;
//
//  //! Create a buffer
//  VulkanBuffer(const VulkanDevice* device,
//               const BufferUsage usage_flag,
//               const std::size_t size) noexcept;
//
//  //! Destroy a buffer
//  ~VulkanBuffer() noexcept override;
//
//
//  //! Return the allocation info
//  VmaAllocationInfo& allocationInfo() noexcept;
//
//  //! Return the allocation info
//  const VmaAllocationInfo& allocationInfo() const noexcept;
//
//  //! Return the buffer body
//  vk::Buffer& buffer() noexcept;
//
//  //! Return the buffer body
//  const vk::Buffer& buffer() const noexcept;
//
//  //! Copy this buffer to a dst buffer
//  template <DescriptorType kDstDescriptor>
//  void copyTo(VulkanBuffer<kDstDescriptor, T>* dst,
//              const std::size_t count,
//              const std::size_t src_offset,
//              const std::size_t dst_offset,
//              const uint32b queue_index) const noexcept;
//
//  //! Destroy a buffer
//  void destroy() noexcept;
//
//  //! Return the device type
//  SubPlatformType SubPlatformType() const noexcept override;
//
//  //! Check if a buffer memory is on device
//  bool isDeviceMemory() const noexcept override;
//
//  //! Check if a buffer memory is on host
//  bool isHostMemory() const noexcept override;
//
//  //! Check if a buffer memory is host visible
//  bool isHostVisible() const noexcept override;
//
//  //! Return the memory allocation
//  VmaAllocation& memory() noexcept;
//
//  //! Return the memory allocation
//  const VmaAllocation& memory() const noexcept;
//
//  //! Return the memory usage
//  std::size_t memoryUsage() const noexcept override;
//
//  //! Read a data from a buffer
//  void read(Pointer data,
//            const std::size_t count,
//            const std::size_t offset,
//            const uint32b queue_index) const noexcept override;
//
//  //! Set a size of a buffer
//  void setSize(const std::size_t size) noexcept override;
//
//  //! Return a size of a buffer
//  std::size_t size() const noexcept override;
//
//  //! Write a data to a buffer
//  void write(ConstPointer data,
//             const std::size_t count,
//             const std::size_t offset,
//             const uint32b queue_index) noexcept override;
//
// private:
//  //! Initialize a buffer
//  void initialize() noexcept;
//
//  //! Map a buffer memory to a host
//  Pointer mappedMemory() const noexcept override;
//
//  //! Unmap a buffer memory
//  void unmapMemory() const noexcept override;
//
//
//  const VulkanDevice* device_;
//  vk::Buffer buffer_;
//  vk::CommandBuffer copy_command_;
//  VmaAllocation memory_ = VK_NULL_HANDLE;
//  VmaAllocationInfo alloc_info_;
//  std::size_t size_ = 0;
//};
//
//// Type aliases
//template <typename Type>
//using VulkanUniformBuffer = VulkanBuffer<DescriptorType::kUniform, Type>;
//template <typename Type>
//using VulkanStorageBuffer = VulkanBuffer<DescriptorType::kStorage, Type>;
//
//} // namespace zivc
//
//#include "vulkan_buffer-inl.hpp"
//
//#endif // ZIVC_VULKAN_BUFFER_HPP
