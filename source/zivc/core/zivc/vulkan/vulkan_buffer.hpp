/*!
  \file vulkan_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_HPP
#define ZIVC_VULKAN_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/concepts.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/buffer.hpp"
#include "zivc/kernel_common.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/buffer_init_params.hpp"
#include "zivc/utility/buffer_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;

template <KernelArg T>
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
  using LaunchOptions = typename Buffer<T>::LaunchOptions;
  using DescriptorType = BufferInitParams::DescriptorType;


  /*!
    \brief No brief description

    No detailed description.
    */
  struct BufferData
  {
    VkBuffer buffer_ = ZIVC_VK_NULL_HANDLE;
    VmaAllocation vm_allocation_ = ZIVC_VK_NULL_HANDLE;
    VmaAllocationInfo vm_alloc_info_;
    VkCommandBuffer command_buffer_ = ZIVC_VK_NULL_HANDLE;
    SharedKernelCommon fill_kernel_;
    SharedBuffer<uint8b> fill_data_;
    DescriptorType desc_type_ = DescriptorType::kStorage;
    [[maybe_unused]] Padding<4> pad_;
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

  //! Return the capacity of the buffer in bytes
  std::size_t capacityInBytes() const noexcept override;

  //! Return the command buffer reference
  VkCommandBuffer& commandBuffer() noexcept;

  //! Return the command buffer reference
  const VkCommandBuffer& commandBuffer() const noexcept;

  //! Return the underlying descriptor type
  DescriptorType descriptorType() const noexcept;

  //! Return the underlying descriptor type
  VkBufferUsageFlagBits descriptorTypeVk() const noexcept;

  //! Return the index of used heap
  std::size_t heapIndex() const noexcept override;

  //! Check if the buffer is the most efficient for the device access
  bool isDeviceLocal() const noexcept override;

  //! Check if the buffer is cached on the host
  bool isHostCached() const noexcept override;

  //! Check if the buffer doesn't need to be unmapped
  bool isHostCoherent() const noexcept override;

  //! Check if the buffer can be mapped for the host access
  bool isHostVisible() const noexcept override;

  //! Map a buffer memory to a host
  [[nodiscard]]
  void* mapMemoryData() const override;

  //! Return the underlying buffer data
  BufferData& rawBuffer() noexcept;

  //! Return the underlying buffer data
  const BufferData& rawBuffer() const noexcept;

  //! Return the underlying buffer data
  void* rawBufferData() noexcept override;

  //! Return the underlying buffer data
  const void* rawBufferData() const noexcept override;

  //! Set the descriptor type
  void setDescriptorType(const DescriptorType type) noexcept;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the size of the buffer in bytes
  std::size_t sizeInBytes() const noexcept override;

  //! Unmap a buffer memory
  void unmapMemoryData() const noexcept override;

 protected:
  //! Clear the contents of the buffer
  void destroyData() noexcept override;

  //! Initialize the buffer
  void initData(const BufferInitParams& params) override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  friend Buffer<T>;


  //! Copy from the given buffer
  template <KernelArg D>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  static LaunchResult copyFromImpl(const BufferCommon& source,
                                   BufferCommon* dest,
                                   const BufferLaunchOptions<D>& launch_options);

  //! Copy on the device
  template <KernelArg D>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  static LaunchResult copyOnDevice(const BufferCommon& source,
                                   BufferCommon* dest,
                                   const BufferLaunchOptions<D>& launch_options);

  //! Copy on the host
  template <KernelArg D>
  static LaunchResult copyOnHost(const BufferCommon& source,
                                 BufferCommon* dest,
                                 const BufferLaunchOptions<D>& launch_options) noexcept;

  //! Fill the buffer on device with specified value
  template <KernelArg D>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  static LaunchResult fillFastOnDevice(typename Buffer<D>::ConstReference value,
                                       BufferCommon* dest,
                                       const BufferLaunchOptions<D>& launch_options);

  //! Fill the buffer with specified value
  template <KernelArg D>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  static LaunchResult fillImpl(typename Buffer<D>::ConstReference value,
                               BufferCommon* dest,
                               const BufferLaunchOptions<D>& launch_options);

  //! Fill the buffer on device with specified value
  template <KernelArg D>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  static LaunchResult fillOnDevice(typename Buffer<D>::ConstReference value,
                                   BufferCommon* dest,
                                   const BufferLaunchOptions<D>& launch_options);

  //! Fill the buffer on host with specified value
  template <KernelArg D>
  static LaunchResult fillOnHost(typename Buffer<D>::ConstReference value,
                                 BufferCommon* dest,
                                 const BufferLaunchOptions<D>& launch_options) noexcept;

  //! Check if the buffer has the given memory property flag
  bool hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept;

  //! Initialize the command buffer
  void initCommandBuffer();

  //! Initialize the fill kernel
  void initFillKernel();

  //! Check if the buffer is internal
  bool isInternal() const noexcept;

  //! Make a data for fast fill on device
  static uint32b makeDataForFillFast(ConstReference value) noexcept;

  //! Return the underlying memory type
  const VkMemoryType& memoryType() const noexcept;

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;


  BufferData buffer_data_;
  std::size_t size_ = 0;
  zisc::Boolean is_internal_;
  [[maybe_unused]] Padding<7> pad_;
};

} // namespace zivc

#include "vulkan_buffer-inl.hpp"

#endif // ZIVC_VULKAN_BUFFER_HPP
