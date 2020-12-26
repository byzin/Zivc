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
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

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
  using Reference = typename Buffer<T>::Reference;
  using ConstReference = typename Buffer<T>::ConstReference;
  using Pointer = typename Buffer<T>::Pointer;
  using ConstPointer = typename Buffer<T>::ConstPointer;


  /*!
    \brief No brief description

    No detailed description.
    */
  enum class DescriptorType : uint32b
  {
    kUniform,
    kStorage
  };


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

  //! Return the command buffer reference
  VkCommandBuffer& commandBuffer() noexcept;

  //! Return the command buffer reference
  const VkCommandBuffer& commandBuffer() const noexcept;

  //! Return the underlying descriptor type
  DescriptorType descriptorType() const noexcept;

  //! Return the underlying descriptor type
  VkBufferUsageFlagBits descriptorTypeVk() const noexcept;

  //! Check if the buffer is the most efficient for the device access
  bool isDeviceLocal() const noexcept override;

  //! Check if the buffer is cached on the host
  bool isHostCached() const noexcept override;

  //! Check if the buffer doesn't need to be unmapped
  bool isHostCoherent() const noexcept override;

  //! Check if the buffer can be mapped for the host access
  bool isHostVisible() const noexcept override;

  //! Return the vulkan copy region
  static VkBufferCopy2KHR makeCopyRegion(const VkDeviceSize source_offset,
                                         const VkDeviceSize dest_offset,
                                         const VkDeviceSize size) noexcept;

  //! Set the descriptor type
  void setDescriptorType(const DescriptorType type) noexcept;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the number of elements
  std::size_t size() const noexcept override;

 protected:
  //! Copy from the given buffer
  LaunchResult copyFromImpl(const Buffer<T>& source,
                            const BufferLaunchOptions<T>& launch_options) override;

  //! Clear the contents of the buffer
  void destroyData() noexcept override;

  //! Fill the buffer with specified value
  LaunchResult fillImpl(ConstReference value,
                        const BufferLaunchOptions<T>& launch_options) override;

  //! Initialize the buffer
  void initData() override;

  //! Map a buffer memory to a host
  Pointer mappedMemory() const override;

  //! Unmap a buffer memory
  void unmapMemory() const noexcept override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  //! Copy on the device
  LaunchResult copyOnDevice(const Buffer<T>& source,
                            const BufferLaunchOptions<T>& launch_options);

  //! Copy on the host
  LaunchResult copyOnHost(const Buffer<T>& source,
                          const BufferLaunchOptions<T>& launch_options) noexcept;

  //! Fill the buffer on device with specified value
  LaunchResult fillFastOnDevice(ConstReference value,
                                const BufferLaunchOptions<T>& launch_options);

  //! Fill the buffer on device with specified value
  LaunchResult fillOnDevice(ConstReference value,
                            const BufferLaunchOptions<T>& launch_options);

  //! Fill the buffer on host with specified value
  LaunchResult fillOnHost(ConstReference value,
                          const BufferLaunchOptions<T>& launch_options) noexcept;

  //! Check if the buffer has the given memory property flag
  bool hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept;

  //! Initialize the command buffer
  void initCommandBuffer();

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;


  VkBuffer buffer_ = VK_NULL_HANDLE;
  VmaAllocation vm_allocation_ = VK_NULL_HANDLE;
  VmaAllocationInfo vm_alloc_info_;
  DescriptorType desc_type_ = DescriptorType::kStorage;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
};

} // namespace zivc

#include "vulkan_buffer-inl.hpp"

#endif // ZIVC_VULKAN_BUFFER_HPP
