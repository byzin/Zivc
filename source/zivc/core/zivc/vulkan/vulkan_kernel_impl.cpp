/*!
  \file vulkan_kermel_impl.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_kernel_impl.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "zivc/zivc_config.hpp"

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
  auto& zdevice = device();
  const auto* loader = zdevice.dispatcher().loaderImpl();
  const zivcvk::BufferMemoryBarrier buff_barrier{
      zivcvk::AccessFlagBits::eTransferWrite,
      zivcvk::AccessFlagBits::eUniformRead,
      zdevice.queueFamilyIndex(),
      zdevice.queueFamilyIndex(),
      buffer,
      0,
      VK_WHOLE_SIZE};
  zivcvk::CommandBuffer command{command_buffer};
  command.pipelineBarrier(zivcvk::PipelineStageFlagBits::eTransfer,
                          zivcvk::PipelineStageFlagBits::eComputeShader,
                          zivcvk::DependencyFlags{},
                          nullptr,
                          buff_barrier,
                          nullptr,
                          *loader);
}

/*!
  \details No detailed description

  \param [out] descriptor_pool No description.
  */
void VulkanKernelImpl::destroyDescriptorSet(VkDescriptorPool* descriptor_pool) noexcept
{
  auto& zdevice = device();
  zivcvk::Device d{zdevice.device()};
  if (d) {
    const auto* loader = zdevice.dispatcher().loaderImpl();
    zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};
    {
      zivcvk::DescriptorPool desc_pool{*descriptor_pool};
      if (desc_pool)
        d.destroyDescriptorPool(desc_pool, alloc, *loader);
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
                                   const std::array<uint32b, 3>& dispatch_size)
{
  const auto* kdata = zisc::cast<const VulkanDevice::KernelData*>(kernel_data);
  const auto* loader = device().dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");

  constexpr auto bind_point = zivcvk::PipelineBindPoint::eCompute;
  const zivcvk::PipelineLayout pline_layout{kdata->pipeline_layout_};
  const zivcvk::DescriptorSet desc_set{descriptor_set};
  command.bindDescriptorSets(bind_point, pline_layout, 0, desc_set, nullptr, *loader);

  const zivcvk::Pipeline pline{kdata->pipeline_};
  command.bindPipeline(bind_point, pline, *loader);

  command.dispatch(dispatch_size[0], dispatch_size[1], dispatch_size[2], *loader);
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
  auto& zdevice = device();

  zivcvk::Device d{zdevice.device()};
  const auto* loader = zdevice.dispatcher().loaderImpl();
  auto* mem_resource = zdevice.memoryResource();
  zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};

  // Initialize descriptor pool
  zivcvk::DescriptorPool desc_pool;
  {
    using PoolSizeList = zisc::pmr::vector<zivcvk::DescriptorPoolSize>;
    PoolSizeList::allocator_type pool_alloc{mem_resource};
    PoolSizeList pool_size_list{pool_alloc};
    pool_size_list.reserve(2);
    // Storage buffers
    if (0 < num_of_storage_buffers) {
      pool_size_list.emplace_back(zivcvk::DescriptorType::eStorageBuffer,
                                  zisc::cast<uint32b>(num_of_storage_buffers));
    }
    // Uniform buffer
    if (0 < num_of_uniform_buffers) {
      pool_size_list.emplace_back(zivcvk::DescriptorType::eUniformBuffer,
                                  zisc::cast<uint32b>(num_of_uniform_buffers));
    }
    const zivcvk::DescriptorPoolCreateInfo create_info{
        zivcvk::DescriptorPoolCreateFlags{},
        1,
        zisc::cast<uint32b>(pool_size_list.size()),
        pool_size_list.data()};

    desc_pool = d.createDescriptorPool(create_info, alloc, *loader);
  }

  // Initialize descriptor set
  {
    const auto* kdata = zisc::cast<const VulkanDevice::KernelData*>(kernel_data);
    const zivcvk::DescriptorSetLayout desc_set_layout{kdata->desc_set_layout_};
    const zivcvk::DescriptorSetAllocateInfo alloc_info{desc_pool,
                                                       1,
                                                       &desc_set_layout};
    zisc::pmr::polymorphic_allocator<zivcvk::DescriptorSet> set_alloc{mem_resource};
    auto desc_set = d.allocateDescriptorSets(alloc_info, set_alloc, *loader);
    ZISC_ASSERT(desc_set.size() == 1, "Multiple descriptor sets were created.");

    // Output
    *descriptor_pool = zisc::cast<VkDescriptorPool>(desc_pool);
    *descriptor_set = zisc::cast<VkDescriptorSet>(desc_set[0]);
  }
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
  const auto* kdata = zisc::cast<const VulkanDevice::KernelData*>(kernel_data);
  const auto* loader = device().dispatcher().loaderImpl();
  const zivcvk::CommandBuffer command{command_buffer};
  const zivcvk::PipelineLayout pline_l{kdata->pipeline_layout_};
  constexpr auto stage_flag = zivcvk::ShaderStageFlagBits::eCompute;
  const auto o = zisc::cast<uint32b>(offset);
  const auto s = zisc::cast<uint32b>(size);
  command.pushConstants(pline_l, stage_flag, o, s, data, *loader);
}

/*!
  \details No detailed description

  \param [in] descriptor_set No description.
  \param [in] n No description.
  \param [in] buffer_list No description.
  \param [in] desc_type_list No description.
  \param [in] desc_info_list No description.
  \param [in] write_desc_list No description.
  */
void VulkanKernelImpl::updateDescriptorSet(const VkDescriptorSet& descriptor_set,
                                           const std::size_t n,
                                           const VkBuffer* buffer_list,
                                           const VkDescriptorType* desc_type_list,
                                           VkDescriptorBufferInfo* desc_info_list,
                                           VkWriteDescriptorSet* write_desc_list)
{
  for (std::size_t i = 0; i < n; ++i) {
    auto* desc_info = ::new (desc_info_list + i) zivcvk::DescriptorBufferInfo{};
    desc_info->buffer= zisc::cast<const zivcvk::Buffer>(buffer_list[i]);
    desc_info->offset = 0;
    desc_info->range = VK_WHOLE_SIZE;

    auto* write_desc = ::new (write_desc_list + i) zivcvk::WriteDescriptorSet{};
    write_desc->dstSet = zisc::cast<const zivcvk::DescriptorSet>(descriptor_set);
    write_desc->dstBinding = zisc::cast<uint32b>(i);
    write_desc->dstArrayElement = 0;
    write_desc->descriptorCount = 1;
    const auto desc_type = zisc::cast<zivcvk::DescriptorType>(desc_type_list[i]);
    write_desc->descriptorType = desc_type;
    write_desc->pBufferInfo = desc_info;
  }

  zivcvk::Device d{device().device()};
  const auto* loader = device().dispatcher().loaderImpl();
  const auto* descs = zisc::reinterp<const zivcvk::WriteDescriptorSet*>(write_desc_list);
  d.updateDescriptorSets(zisc::cast<uint32b>(n), descs, 0, nullptr, *loader);
}

} // namespace zivc
