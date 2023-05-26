/*!
  \file vulkan_kernel_impl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_IMPL_HPP
#define ZIVC_VULKAN_KERNEL_IMPL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <span>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "../zivc_config.hpp"
#include "../internal/shader_desc_map.hpp"
#include "zivc/vulkan/vulkan.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanKernelImpl : private zisc::NonCopyable<VulkanKernelImpl>
{
  using BufferMapT = internal::ShaderDescMap::KernelDescMap::BufferMap;

 public:
  //! Initialize the vulkan kernel impl
  VulkanKernelImpl(VulkanDevice* device) noexcept;

  //! Finalize the vulkan kernel impl
  ~VulkanKernelImpl() noexcept;


  //! Add a memory barrier for the given POD buffer
  void addPodBarrierCmd(const VkCommandBuffer& command_buffer,
                        const VkBuffer& pod_buffer);

  //! Destroy a descriptor set
  void destroyDescriptorSet(VkDescriptorPool* descriptor_pool) noexcept;

  //! Record dispatching the given kernel to the vulkan device
  void dispatchCmd(const VkCommandBuffer& command_buffer,
                   const void* kernel_data,
                   const VkDescriptorSet& descriptor_set,
                   const std::span<const uint32b, 3>& dispatch_size);

  //! Initialize a descriptor set of the kernel
  void initDescriptorSet(const std::size_t num_of_storage_buffers,
                         const std::size_t num_of_uniform_buffers,
                         const void* kernel_data,
                         VkDescriptorPool* descriptor_pool,
                         VkDescriptorSet* descriptor_set);

  //! Record push constant command
  template <typename Type>
  void pushConstantCmd(const VkCommandBuffer& command_buffer,
                       const void* kernel_data,
                       const std::size_t offset,
                       const Type& data);

  //! Record push constant command
  void pushConstantCmd(const VkCommandBuffer& command_buffer,
                       const void* kernel_data,
                       const std::size_t offset,
                       const std::size_t size,
                       const void* data);

  //! Update the given descriptor set with the given buffers
  template <std::size_t kN>
  void updateDescriptorSet(const VkDescriptorSet& descriptor_set,
                           const std::array<VkBuffer, kN>& buffer_list,
                           const std::array<VkDescriptorType, kN>& desc_type_list,
                           const std::span<const BufferMapT> buffer_map_list);

 private:
  //! Return the underlying device object
  VulkanDevice& device() noexcept;

  //! Return the underlying device object
  const VulkanDevice& device() const noexcept;

  //! Update the given descriptor set with the given buffers
  void updateDescriptorSet(const VkDescriptorSet& descriptor_set,
                           const std::span<const VkBuffer> buffer_list,
                           const std::span<const VkDescriptorType> desc_type_list,
                           const std::span<const BufferMapT> buffer_map_list,
                           std::span<VkDescriptorBufferInfo> desc_info_list,
                           std::span<VkWriteDescriptorSet> write_desc_list);


  VulkanDevice* device_ = nullptr;
};

} // namespace zivc

#include "vulkan_kernel_impl-inl.hpp"

#endif // ZIVC_VULKAN_KERNEL_IMPL_HPP
