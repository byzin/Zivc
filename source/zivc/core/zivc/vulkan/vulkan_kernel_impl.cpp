/*!
  \file vulkan_kermel_impl.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_kernel_impl.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <span>
#include <string_view>
#include <utility>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "../zivc_config.hpp"
#include "../auxiliary/error.hpp"
#include "../internal/shader_desc_map.hpp"
#include "zivc/vulkan/vulkan_dispatch_loader.hpp"
#include "zivc/vulkan/vulkan_hpp.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] device No description.
  */
VulkanKernelImpl::VulkanKernelImpl(VulkanDevice* device) noexcept : device_{device}
{
}

/*!
  \details No detailed description
  */
VulkanKernelImpl::~VulkanKernelImpl() noexcept
{
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] buffer No description.
  */
void VulkanKernelImpl::addPodBarrierCmd(const VkCommandBuffer& command_buffer,
                                        const VkBuffer& buffer)
{
  const VulkanDevice& zdevice = device();
  const vk::BufferMemoryBarrier buff_barrier{
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eUniformRead,
      zdevice.queueFamilyIndex(VulkanDeviceCapability::kCompute),
      zdevice.queueFamilyIndex(VulkanDeviceCapability::kCompute),
      buffer,
      0,
      VK_WHOLE_SIZE};
  const vk::CommandBuffer command{command_buffer};
  command.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                          vk::PipelineStageFlagBits::eComputeShader,
                          vk::DependencyFlags{},
                          nullptr,
                          buff_barrier,
                          nullptr,
                          zdevice.dispatcher().loader());
}

/*!
  \details No detailed description

  \param [out] descriptor_pool No description.
  */
void VulkanKernelImpl::destroyDescriptorSet(VkDescriptorPool* descriptor_pool) noexcept
{
  VulkanDevice& zdevice = device();
  const vk::Device d{zdevice.device()};
  if (d) {
    const vk::AllocationCallbacks alloc{zdevice.createAllocator()};
    {
      const vk::DescriptorPool desc_pool{*descriptor_pool};
      if (desc_pool)
        d.destroyDescriptorPool(desc_pool, alloc, zdevice.dispatcher().loader());
      *descriptor_pool = ZIVC_VK_NULL_HANDLE;
    }
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] descriptor_set No description.
  \param [in] kernel_data No description.
  \param [in] dispatch_size No description.
  */
void VulkanKernelImpl::dispatchCmd(const VkCommandBuffer& command_buffer,
                                   const void* kernel_data,
                                   const VkDescriptorSet& descriptor_set,
                                   const std::span<const uint32b, 3>& dispatch_size)
{
  const auto* kdata = static_cast<const VulkanDevice::KernelData*>(kernel_data);
  VulkanDispatchLoader::ConstLoaderReference loader = device().dispatcher().loader();

  const vk::CommandBuffer command{command_buffer};
  ZIVC_ASSERT(command, "The given command buffer is null.");

  constexpr vk::PipelineBindPoint bind_point = vk::PipelineBindPoint::eCompute;
  const vk::PipelineLayout pline_layout{kdata->pipeline_layout_};
  const vk::DescriptorSet desc_set{descriptor_set};
  command.bindDescriptorSets(bind_point, pline_layout, 0, desc_set, nullptr, loader);

  const vk::Pipeline pline{kdata->pipeline_};
  command.bindPipeline(bind_point, pline, loader);

  command.dispatch(dispatch_size[0], dispatch_size[1], dispatch_size[2], loader);
}

/*!
  \details No detailed description

  \param [in] num_of_storage_buffers No description.
  \param [in] num_of_uniform_buffers No description.
  \param [in] kernel_data No description.
  \param [out] descriptor_pool No description.
  \param [out] descriptor_set No description.
  */
void VulkanKernelImpl::initDescriptorSet(
    const std::size_t num_of_storage_buffers,
    const std::size_t num_of_uniform_buffers,
    const void* kernel_data,
    VkDescriptorPool* descriptor_pool,
    VkDescriptorSet* descriptor_set)
{
  VulkanDevice& zdevice = device();

  const vk::Device d{zdevice.device()};
  VulkanDispatchLoader::ConstLoaderReference loader = zdevice.dispatcher().loader();
  zisc::pmr::memory_resource* mem_resource = zdevice.memoryResource();
  const vk::AllocationCallbacks alloc{zdevice.createAllocator()};

  // Initialize descriptor pool
  vk::DescriptorPool desc_pool;
  {
    using PoolSizeListT = zisc::pmr::vector<vk::DescriptorPoolSize>;
    const PoolSizeListT::allocator_type pool_alloc{mem_resource};
    PoolSizeListT pool_size_list{pool_alloc};
    pool_size_list.reserve(2);
    // Storage buffers
    if (0 < num_of_storage_buffers) {
      pool_size_list.emplace_back(vk::DescriptorType::eStorageBuffer,
                                  zisc::cast<uint32b>(num_of_storage_buffers));
    }
    // Uniform buffer
    if (0 < num_of_uniform_buffers) {
      pool_size_list.emplace_back(vk::DescriptorType::eUniformBuffer,
                                  zisc::cast<uint32b>(num_of_uniform_buffers));
    }
    const vk::DescriptorPoolCreateInfo create_info{
        vk::DescriptorPoolCreateFlags{},
        1,
        zisc::cast<uint32b>(pool_size_list.size()),
        pool_size_list.data()};

    desc_pool = d.createDescriptorPool(create_info, alloc, loader);
  }

  // Initialize descriptor set
  {
    const auto* kdata = static_cast<const VulkanDevice::KernelData*>(kernel_data);
    const vk::DescriptorSetLayout desc_set_layout{kdata->desc_set_layout_};
    const vk::DescriptorSetAllocateInfo alloc_info{desc_pool,
                                                       1,
                                                       &desc_set_layout};
    //! \todo Resolve the error
    // The allocate function causes undefined symbol error with custom allocator
    //const zisc::pmr::polymorphic_allocator<vk::DescriptorSet> alloc{mem_resource};
    //auto desc_set = d.allocateDescriptorSets(alloc_info, alloc, loader);
    std::vector desc_set = d.allocateDescriptorSets(alloc_info, loader);
    ZIVC_ASSERT(desc_set.size() == 1, "Multiple descriptor sets were created.");

    // Output
    *descriptor_pool = zisc::cast<VkDescriptorPool>(desc_pool);
    *descriptor_set = zisc::cast<VkDescriptorSet>(desc_set[0]);
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] kernel_data No description.
  \param [in] offset No description.
  \param [in] size No description.
  \param [in] data No description.
  */
void VulkanKernelImpl::pushConstantCmd(const VkCommandBuffer& command_buffer,
                                       const void* kernel_data,
                                       const std::size_t offset,
                                       const std::size_t size,
                                       const void* data)
{
  const auto* kdata = static_cast<const VulkanDevice::KernelData*>(kernel_data);
  VulkanDispatchLoader::ConstLoaderReference loader = device().dispatcher().loader();
  const vk::CommandBuffer command{command_buffer};
  const vk::PipelineLayout pline_l{kdata->pipeline_layout_};
  constexpr vk::ShaderStageFlags stage_flag = vk::ShaderStageFlagBits::eCompute;
  const auto o = zisc::cast<uint32b>(offset);
  const auto s = zisc::cast<uint32b>(size);
  command.pushConstants(pline_l, stage_flag, o, s, data, loader);
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanDevice& VulkanKernelImpl::device() noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDevice& VulkanKernelImpl::device() const noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \param [in] descriptor_set No description.
  \param [in] n No description.
  \param [in] buffer_list No description.
  \param [in] desc_type_list No description.
  \param [in] buffer_map_list No description.
  \param [in] desc_info_list No description.
  \param [in] write_desc_list No description.
  */
void VulkanKernelImpl::updateDescriptorSet(
    const VkDescriptorSet& descriptor_set,
    const std::span<const VkBuffer> buffer_list,
    const std::span<const VkDescriptorType> desc_type_list,
    const std::span<const BufferMapT> buffer_map_list,
    std::span<VkDescriptorBufferInfo> desc_info_list,
    std::span<VkWriteDescriptorSet> write_desc_list)
{
  const std::size_t n = buffer_list.size();
  for (std::size_t i = 0; i < n; ++i) {
    auto* desc_info = ::new (&desc_info_list[i]) vk::DescriptorBufferInfo{};
    desc_info->buffer= zisc::cast<const vk::Buffer>(buffer_list[i]);
    desc_info->offset = 0;
    desc_info->range = VK_WHOLE_SIZE;

    auto* write_desc = ::new (&write_desc_list[i]) vk::WriteDescriptorSet{};
    write_desc->dstSet = zisc::cast<const vk::DescriptorSet>(descriptor_set);
    write_desc->dstBinding = buffer_map_list[i].binding_;
    write_desc->dstArrayElement = 0;
    write_desc->descriptorCount = 1;
    const auto desc_type = static_cast<vk::DescriptorType>(desc_type_list[i]);
    write_desc->descriptorType = desc_type;
    write_desc->pBufferInfo = desc_info;
  }

  const vk::Device d{device().device()};
  VulkanDispatchLoader::ConstLoaderReference loader = device().dispatcher().loader();
  const auto* descs = zisc::reinterp<const vk::WriteDescriptorSet*>(write_desc_list.data());
  d.updateDescriptorSets(zisc::cast<uint32b>(n), descs, 0, nullptr, loader);
}

} // namespace zivc
