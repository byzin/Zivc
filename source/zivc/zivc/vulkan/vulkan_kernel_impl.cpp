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
  const auto loader = zdevice.dispatcher().loaderImpl();
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

  \param [out] descriptor_set_layout No description.
  \param [out] descriptor_pool No description.
  */
void VulkanKernelImpl::destroyDescriptorSet(
    VkDescriptorSetLayout* descriptor_set_layout,
    VkDescriptorPool* descriptor_pool) noexcept
{
  auto& zdevice = device();
  zivcvk::Device d{zdevice.device()};
  if (d) {
    const auto loader = zdevice.dispatcher().loaderImpl();
    zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};
    {
      zivcvk::DescriptorPool desc_pool{*descriptor_pool};
      if (desc_pool)
        d.destroyDescriptorPool(desc_pool, alloc, *loader);
      *descriptor_pool = ZIVC_VK_NULL_HANDLE;
    }
    {
      zivcvk::DescriptorSetLayout desc_set_layout{*descriptor_set_layout};
      if (desc_set_layout)
        d.destroyDescriptorSetLayout(desc_set_layout, alloc, *loader);
      *descriptor_set_layout = ZIVC_VK_NULL_HANDLE;
    }
  }
}

/*!
  \details No detailed description

  \param [out] pipeline_layout No description.
  \param [out] compute_pipeline No description.
  */
void VulkanKernelImpl::destroyPipeline(
    VkPipelineLayout* pipeline_layout,
    VkPipeline* compute_pipeline) noexcept
{
  auto& zdevice = device();
  zivcvk::Device d{zdevice.device()};
  if (d) {
    const auto loader = zdevice.dispatcher().loaderImpl();
    zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};
    {
      zivcvk::Pipeline compute_pline{*compute_pipeline};
      if (compute_pline)
        d.destroyPipeline(compute_pline, alloc, *loader);
      *compute_pipeline = ZIVC_VK_NULL_HANDLE;
    }
    {
      zivcvk::PipelineLayout pline_layout{*pipeline_layout};
      if (pline_layout)
        d.destroyPipelineLayout(pline_layout, alloc, *loader);
      *pipeline_layout = ZIVC_VK_NULL_HANDLE;
    }
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] descriptor_set No description.
  \param [in] pipeline_layout No description.
  \param [in] pipeline No description.
  \param [in] work_dimension No description.
  \param [in] work_group_size No description.
  */
void VulkanKernelImpl::dispatchCmd(const VkCommandBuffer& command_buffer,
                                   const VkDescriptorSet& descriptor_set,
                                   const VkPipelineLayout& pipeline_layout,
                                   const VkPipeline& pipeline,
                                   const std::size_t work_dimension,
                                   const std::array<uint32b, 3>& work_group_size)
{
  const auto loader = device().dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");

  constexpr auto bind_point = zivcvk::PipelineBindPoint::eCompute;
  const zivcvk::PipelineLayout pline_layout{pipeline_layout};
  const zivcvk::DescriptorSet desc_set{descriptor_set};
  command.bindDescriptorSets(bind_point, pline_layout, 0, desc_set, nullptr, *loader);

  const zivcvk::Pipeline pline{pipeline};
  command.bindPipeline(bind_point, pline, *loader);

  const auto disp_size = calcDispatchSize(work_dimension, work_group_size);
  command.dispatch(disp_size[0], disp_size[1], disp_size[2], *loader);
}

/*!
  \details No detailed description

  \param [in] num_of_storage_buffers No description.
  \param [in] num_of_uniform_buffers No description.
  \param [out] descriptor_set_layout No description.
  \param [out] descriptor_pool No description.
  \param [out] descriptor_set No description.
  */
void VulkanKernelImpl::initDescriptorSet(
    const std::size_t num_of_storage_buffers,
    const std::size_t num_of_uniform_buffers,
    VkDescriptorSetLayout* descriptor_set_layout,
    VkDescriptorPool* descriptor_pool,
    VkDescriptorSet* descriptor_set)
{
  auto& zdevice = device();

  zivcvk::Device d{zdevice.device()};
  const auto loader = zdevice.dispatcher().loaderImpl();
  auto mem_resource = zdevice.memoryResource();
  zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};

  // Initialize descriptor set layout
  zivcvk::DescriptorSetLayout desc_set_layout;
  {
    using BindingList = zisc::pmr::vector<zivcvk::DescriptorSetLayoutBinding>;
    BindingList::allocator_type bindings_alloc{mem_resource};
    BindingList layout_bindings{bindings_alloc};
    layout_bindings.resize(num_of_storage_buffers + num_of_uniform_buffers);
    // Storage buffers
    for (std::size_t index = 0; index < num_of_storage_buffers; ++index) {
      layout_bindings[index] = zivcvk::DescriptorSetLayoutBinding{
          zisc::cast<uint32b>(index),
          zivcvk::DescriptorType::eStorageBuffer,
          1,
          zivcvk::ShaderStageFlagBits::eCompute};
    }
    // Uniform buffer
    for (std::size_t i = 0; i < num_of_uniform_buffers; ++i) {
      const std::size_t index = num_of_storage_buffers + i;
      layout_bindings[index] = zivcvk::DescriptorSetLayoutBinding{
          zisc::cast<uint32b>(index),
          zivcvk::DescriptorType::eUniformBuffer,
          1,
          zivcvk::ShaderStageFlagBits::eCompute};
    }
    const zivcvk::DescriptorSetLayoutCreateInfo create_info{
        zivcvk::DescriptorSetLayoutCreateFlags{},
        zisc::cast<uint32b>(layout_bindings.size()),
        layout_bindings.data()};
    desc_set_layout = d.createDescriptorSetLayout(create_info, alloc, *loader);
  }

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
    const zivcvk::DescriptorSetAllocateInfo alloc_info{desc_pool,
                                                       1,
                                                       &desc_set_layout};
    zisc::pmr::polymorphic_allocator<zivcvk::DescriptorSet> set_alloc{mem_resource};
    auto desc_set = d.allocateDescriptorSets(alloc_info, set_alloc, *loader);
    ZISC_ASSERT(desc_set.size() == 1, "Multiple descriptor sets were created.");

    // Output
    *descriptor_set_layout = zisc::cast<VkDescriptorSetLayout>(desc_set_layout);
    *descriptor_pool = zisc::cast<VkDescriptorPool>(desc_pool);
    *descriptor_set = zisc::cast<VkDescriptorSet>(desc_set[0]);
  }
}

/*!
  \details No detailed description

  \param [out] pipeline_layout No description.
  \param [out] compute_pipeline No description.
  */
void VulkanKernelImpl::initPipeline(const std::size_t work_dimension,
                                    const std::size_t num_of_local_args,
                                    const VkDescriptorSetLayout& set_layout,
                                    const VkShaderModule& module,
                                    const std::string_view kernel_name,
                                    VkPipelineLayout* pipeline_layout,
                                    VkPipeline* compute_pipeline)
{
  auto& zdevice = device();

  zivcvk::Device d{zdevice.device()};
  const auto loader = zdevice.dispatcher().loaderImpl();
  auto mem_resource = zdevice.memoryResource();
  zivcvk::AllocationCallbacks alloc{zdevice.makeAllocator()};

  // Pipeline
  zivcvk::PipelineLayout pline_layout;
  {
    // For clspv global and region offsets
    const zivcvk::PushConstantRange push_constant_range{
        zivcvk::ShaderStageFlagBits::eCompute, 0, 28};

    zivcvk::DescriptorSetLayout desc_set_layout{set_layout};
    const zivcvk::PipelineLayoutCreateInfo create_info{
        zivcvk::PipelineLayoutCreateFlags{},
        1,
        std::addressof(desc_set_layout),
        1,
        std::addressof(push_constant_range)};
    pline_layout = d.createPipelineLayout(create_info, alloc, *loader);
  }
  // Specialization constants
  const auto& work_group_size = zdevice.workGroupSizeDim(work_dimension);
  zisc::pmr::vector<uint32b>::allocator_type spec_alloc{mem_resource};
  zisc::pmr::vector<uint32b> spec_constants{spec_alloc};
  {
    spec_constants.reserve(work_group_size.size() + 1);
    // For clspv work group size
    for (const uint32b s : work_group_size)
      spec_constants.emplace_back(s);
    // For clspv local element size
    const auto& info = zdevice.deviceInfoData();
    spec_constants.emplace_back(info.workGroupSize());
  }
  using MapEntryList = zisc::pmr::vector<zivcvk::SpecializationMapEntry>;
  MapEntryList::allocator_type entry_alloc{mem_resource};
  MapEntryList entries{entry_alloc};
  {
    entries.reserve(work_group_size.size() + num_of_local_args);
    // For clspv work group size
    for (std::size_t i = 0; i < work_group_size.size(); ++i) {
      zivcvk::SpecializationMapEntry entry{
          zisc::cast<uint32b>(i),
          zisc::cast<uint32b>(i * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // For clspv local element size
    const uint32b local_size_index = zisc::cast<uint32b>(entries.size());
    for (std::size_t i = 0; i < num_of_local_args; ++i) {
      zivcvk::SpecializationMapEntry entry{
          local_size_index + zisc::cast<uint32b>(i),
          zisc::cast<uint32b>(local_size_index * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
  }
  const zivcvk::SpecializationInfo spec_info{zisc::cast<uint32b>(entries.size()),
                                             entries.data(),
                                             spec_constants.size() * sizeof(uint32b),
                                             spec_constants.data()};
  // Compute pipeline
  zivcvk::Pipeline pline;
  {
    const zivcvk::PipelineShaderStageCreateInfo stage_info{
        zivcvk::PipelineShaderStageCreateFlags{},
        zivcvk::ShaderStageFlagBits::eCompute,
        zivcvk::ShaderModule{module},
        kernel_name.data(),
        std::addressof(spec_info)};
    zivcvk::PipelineCreateFlags pipeline_flags;
    if (zdevice.isDebugMode()) {
      pipeline_flags |= zivcvk::PipelineCreateFlagBits::eCaptureStatisticsKHR;
    }
    const zivcvk::ComputePipelineCreateInfo pipeline_info{pipeline_flags,
                                                          stage_info,
                                                          pline_layout};
    auto result = d.createComputePipeline(zivcvk::PipelineCache{},
                                          pipeline_info,
                                          alloc,
                                          *loader);
    if (result.result != zivcvk::Result::eSuccess) {
      //! \todo Error handling
    }
    pline = zisc::cast<zivcvk::Pipeline>(result.value);
  }

  *pipeline_layout = zisc::cast<VkPipelineLayout>(pline_layout);
  *compute_pipeline = zisc::cast<VkPipeline>(pline);
}

/*!
  \details No detailed description

  \param [in] work_dimension No description.
  \param [in] work_size No description.
  \return No description
  */
std::array<uint32b, 3> VulkanKernelImpl::calcDispatchSize(
    const std::size_t work_dimension,
    const std::array<uint32b, 3>& work_size) const noexcept
{
  std::array<uint32b, 3> dispatch_size{{1, 1, 1}};
  const auto& group_size = device().workGroupSizeDim(work_dimension);
  for (std::size_t i = 0; i < work_dimension; ++i) {
    const uint32b s = (work_size[i] + group_size[i] - 1) / group_size[i];
    dispatch_size[i] = s;
  }
  return dispatch_size;
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
  \param [in] pipeline_layout No description.
  \param [in] offset No description.
  \param [in] size No description.
  \param [in] data No description.
  */
void VulkanKernelImpl::pushConstantCmd(const VkCommandBuffer& command_buffer,
                                       const VkPipelineLayout& pipeline_layout,
                                       const std::size_t offset,
                                       const std::size_t size,
                                       const void* data)
{
  const auto loader = device().dispatcher().loaderImpl();
  const zivcvk::CommandBuffer command{command_buffer};
  const zivcvk::PipelineLayout pline_l{pipeline_layout};
  constexpr auto stage_flag = zivcvk::ShaderStageFlagBits::eCompute;
  const uint32b o = zisc::cast<uint32b>(offset);
  const uint32b s = zisc::cast<uint32b>(size);
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
    auto desc_info = ::new (desc_info_list + i) zivcvk::DescriptorBufferInfo{};
    desc_info->buffer= zisc::cast<const zivcvk::Buffer>(buffer_list[i]);
    desc_info->offset = 0;
    desc_info->range = VK_WHOLE_SIZE;

    auto write_desc = ::new (write_desc_list + i) zivcvk::WriteDescriptorSet{};
    write_desc->dstSet = zisc::cast<const zivcvk::DescriptorSet>(descriptor_set);
    write_desc->dstBinding = zisc::cast<uint32b>(i);
    write_desc->dstArrayElement = 0;
    write_desc->descriptorCount = 1;
    const auto desc_type = zisc::cast<zivcvk::DescriptorType>(desc_type_list[i]);
    write_desc->descriptorType = desc_type;
    write_desc->pBufferInfo = desc_info;
  }

  zivcvk::Device d{device().device()};
  const auto loader = device().dispatcher().loaderImpl();
  auto descs = zisc::reinterp<const zivcvk::WriteDescriptorSet*>(write_desc_list);
  d.updateDescriptorSets(zisc::cast<uint32b>(n), descs, 0, nullptr, *loader);
}

} // namespace zivc
