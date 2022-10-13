/*!
  \file vulkan_buffer_impl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_IMPL_HPP
#define ZIVC_VULKAN_BUFFER_IMPL_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "internal/vulkan_memory_allocator.hpp"
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/buffer_launch_options.hpp"
#include "zivc/auxiliary/kernel_common.hpp"
#include "zivc/auxiliary/launch_result.hpp"
#include "zivc/auxiliary/launch_options.hpp"

namespace zivc {

// Forward declaration
class BufferCommon;
class VulkanDevice;
template <typename, typename...> class Kernel;

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
                      const BufferCommon& object,
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

  //!  a fill kernel instance
  template <KernelArg Type>
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillKernel(const VkCommandBuffer& command_buffer);

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) const noexcept;

  //! Fill the given buffer with the specified value
  template <KernelArg Type>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult fill(KernelCommon* fill_kernel,
                    Buffer<uint8b>* data_buffer,
                    Buffer<Type>* buffer,
                    const BufferLaunchOptions<Type>& launch_options) const;

  //! Fill the given buffer with the specified value
  void fillFastCmd(const VkCommandBuffer& command_buffer,
                   const VkBuffer& buffer,
                   const std::size_t dest_offset,
                   const std::size_t size,
                   const uint32b data) const noexcept;

  //! Initialize allocation info
  void initAllocationInfo(const BufferCommon& object,
                          const VkBufferUsageFlagBits desc_type,
                          VmaAllocationInfo* alloc_info) const;

  //!
  [[noreturn]] static void throwResultException(const VkResult result,
                                                const char* message);

 private:
  //! Represent the unit size in fill kernel
  enum class FillUnitSize : std::size_t
  {
    k8 = sizeof(uint8b),
    k16 = sizeof(uint16b),
    k32 = sizeof(uint32b),
    k64 = sizeof(cl_uint2),
    k128 = sizeof(cl_uint4),
  };


  //! Create a allocation create info
  static VmaAllocationCreateInfo createAllocCreateInfo(
      const BufferUsage buffer_usage,
      const BufferFlag buffer_flag,
      void* user_data) noexcept;

  //! Create a buffer create info
  static VkBufferCreateInfo createBufferCreateInfo(
      const std::size_t size,
      const VkBufferUsageFlagBits desc_type) noexcept;

  //! Create a fill8 kernel instance
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillU8Kernel(const VkCommandBuffer& command_buffer);

  //! Create a fill16 kernel instance
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillU16Kernel(const VkCommandBuffer& command_buffer);

  //! Create a fill32 kernel instance
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillU32Kernel(const VkCommandBuffer& command_buffer);

  //! Create a fill64 kernel instance
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillU64Kernel(const VkCommandBuffer& command_buffer);

  //! Create a fill128 kernel instance
  [[nodiscard("The result will have a vulkan kernel.")]]
  SharedKernelCommon createFillU128Kernel(const VkCommandBuffer& command_buffer);

  //! Return the underlying device object
  VulkanDevice& device() noexcept;

  //! Return the underlying device object
  const VulkanDevice& device() const noexcept;

  //! Fill the given buffer with the specified value in u8 elements
  template <typename Type, typename KType, typename ...KTypes>
  [[nodiscard]]
  LaunchResult fillImpl(Kernel<KType, KTypes...>* kernel,
                        Buffer<Type>* data_buffer,
                        Buffer<Type>* buffer,
                        const LaunchOptions& launch_options,
                        const std::size_t offset,
                        const std::size_t size) const;

  //! Fill the given buffer with the specified value in u8 elements
  [[nodiscard]]
  LaunchResult fillU8(KernelCommon* fill_kernel,
                      Buffer<uint8b>* data_buffer,
                      Buffer<uint8b>* buffer,
                      const LaunchOptions& launch_options,
                      const std::size_t offset,
                      const std::size_t size) const;

  //! Fill the given buffer with the specified value in u16 elements
  [[nodiscard]]
  LaunchResult fillU16(KernelCommon* fill_kernel,
                       Buffer<uint16b>* data_buffer,
                       Buffer<uint16b>* buffer,
                       const LaunchOptions& launch_options,
                       const std::size_t offset,
                       const std::size_t size) const;

  //! Fill the given buffer with the specified value in u32 elements
  [[nodiscard]]
  LaunchResult fillU32(KernelCommon* fill_kernel,
                       Buffer<uint32b>* data_buffer,
                       Buffer<uint32b>* buffer,
                       const LaunchOptions& launch_options,
                       const std::size_t offset,
                       const std::size_t size) const;

  //! Fill the given buffer with the specified value in u64 elements
  [[nodiscard]]
  LaunchResult fillU64(KernelCommon* fill_kernel,
                       Buffer<cl_uint2>* data_buffer,
                       Buffer<cl_uint2>* buffer,
                       const LaunchOptions& launch_options,
                       const std::size_t offset,
                       const std::size_t size) const;

  //! Fill the given buffer with the specified value in u128 elements
  [[nodiscard]]
  LaunchResult fillU128(KernelCommon* fill_kernel,
                        Buffer<cl_uint4>* data_buffer,
                        Buffer<cl_uint4>* buffer,
                        const LaunchOptions& launch_options,
                        const std::size_t offset,
                        const std::size_t size) const;

  //! Return the unit size used in fill kernel
  static constexpr FillUnitSize getFillUnitSize(const std::size_t size) noexcept;

  //! Convert to VMA flags
  static constexpr VmaAllocationCreateFlagBits toVmaFlag(const BufferFlag flag) noexcept;

  //! Convert to VMA usage flags
  static constexpr VmaMemoryUsage toVmaUsage(const BufferUsage usage) noexcept;


  VulkanDevice* device_ = nullptr;
};

} // namespace zivc

#include "vulkan_buffer_impl-inl.hpp"

#endif // ZIVC_VULKAN_BUFFER_IMPL_HPP
