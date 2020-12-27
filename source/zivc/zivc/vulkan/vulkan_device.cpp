/*!
  \file vulkan_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device.hpp"
// Standard C++ library
#include <array>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/thread/bitset.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "vulkan_sub_platform.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/id_data.hpp"

namespace {

/*!
  \brief Enable device features used in zivc
  \details No detailed description

  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
auto getDefaultFeatures(const zivc::VulkanDeviceInfo& info,
                        zisc::pmr::memory_resource* mem_resource) noexcept
{
  struct Features
  {
    zivcvk::PhysicalDevice16BitStorageFeatures b16bit_storage_;
    zivcvk::PhysicalDevice8BitStorageFeatures b8bit_storage_;
    zivcvk::PhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_;
    zivcvk::PhysicalDeviceRayQueryFeaturesKHR ray_query_;
    zivcvk::PhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_;
    zivcvk::PhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    zivcvk::PhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    zivcvk::PhysicalDeviceShaderFloat16Int8Features shader_float16_int8_;
    zivcvk::PhysicalDeviceVariablePointersFeatures variable_pointers_;
    zivcvk::PhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_model_;
  };

  const auto& inputs = info.features();

  zivcvk::PhysicalDeviceFeatures2 features;
  features.features.shaderUniformBufferArrayDynamicIndexing =
      inputs.features1_.shaderUniformBufferArrayDynamicIndexing;
  features.features.shaderStorageBufferArrayDynamicIndexing =
      inputs.features1_.shaderStorageBufferArrayDynamicIndexing;
  features.features.shaderFloat64 = inputs.features1_.shaderFloat64;
  features.features.shaderInt64 = inputs.features1_.shaderInt64;
  features.features.shaderInt16 = inputs.features1_.shaderInt16;

//      device_features.features.vertexPipelineStoresAndAtomics =
//          features.features1_.vertexPipelineStoresAndAtomics;
//      device_features.features.fragmentStoresAndAtomics =
//          features.features1_.fragmentStoresAndAtomics;

  auto f = zisc::pmr::allocateUnique<Features>(mem_resource);
  f->b16bit_storage_ = inputs.b16bit_storage_;
  f->b8bit_storage_ = inputs.b8bit_storage_;
  f->acceleration_structure_ = inputs.acceleration_structure_;
  f->ray_query_ = inputs.ray_query_;
  f->ray_tracing_pipeline_ = inputs.ray_tracing_pipeline_features_;
  f->shader_atomic_float_ = inputs.shader_atomic_float_;
  f->shader_atomic_int64_ = inputs.shader_atomic_int64_;
  f->shader_float16_int8_ = inputs.shader_float16_int8_;
  f->variable_pointers_ = inputs.variable_pointers_;
  f->vulkan_memory_model_ = inputs.vulkan_memory_model_;

  zivc::VulkanDeviceInfo::link(features,
                               f->b16bit_storage_,
                               f->b8bit_storage_,
                               f->acceleration_structure_,
                               f->ray_query_,
                               f->ray_tracing_pipeline_,
                               f->shader_atomic_float_,
                               f->shader_atomic_int64_,
                               f->shader_float16_int8_,
                               f->variable_pointers_,
                               f->vulkan_memory_model_);

  return std::make_tuple(features, std::move(f));
}

} // namespace

namespace zivc {

static_assert(!(std::alignment_of_v<Fence::Data> % std::alignment_of_v<zivcvk::Fence>));
static_assert(sizeof(zivcvk::Fence) <= sizeof(Fence::Data));

/*!
  \details No detailed description

  \param [in] id No description.
  */
VulkanDevice::VulkanDevice(IdData&& id) : Device(std::move(id))
{
}

/*!
  \details No detailed description
  */
VulkanDevice::~VulkanDevice() noexcept
{
  destroy();
}

/*!
  \details No detailed description
  */
VkCommandBuffer VulkanDevice::makeCommandBuffer()
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();

  const zivcvk::CommandBufferAllocateInfo alloc_info{
      commandPool(),
      zivcvk::CommandBufferLevel::ePrimary,
      1};
  zisc::pmr::vector<zivcvk::CommandBuffer>::allocator_type alloc{mem_resource};
  auto commands = d.allocateCommandBuffers(alloc_info, alloc, *loader);
  ZISC_ASSERT(commands.size() == 1, "The size of command buffers isn't 1.");
  return zisc::cast<VkCommandBuffer>(commands[0]);
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDevice::numOfFences() const noexcept
{
  const std::size_t s = fence_list_->size();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDevice::numOfQueues() const noexcept
{
  const auto& info = deviceInfoData();
  const uint32b index = queueFamilyIndex();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  const std::size_t n = queue_family_list[index].queueCount;
  return n;
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t VulkanDevice::peakMemoryUsage(const std::size_t number) const noexcept
{
  std::size_t s = 0;
  if (heap_usage_list_) {
    const auto& usage = (*heap_usage_list_)[number];
    s = usage.peak();
  }
  return s;
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void VulkanDevice::returnFence(Fence* fence) noexcept
{
  auto dest = zisc::reinterp<zivcvk::Fence*>(std::addressof(fence->data()));
  *dest = zivcvk::Fence{};

  auto& fence_list = *fence_list_;
  for (std::size_t i = 0; i < fence_list.size(); ++i) {
    if (zisc::cast<zivcvk::Fence>(fence_list[i]) == *dest) {
      fence_manager_->set(i, true);
      *dest = zivcvk::Fence{};
      break;
    }
  }

  if (*dest) {
    printf("[Warning] Unmanaged fence\n");
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] q No description.
  \param [in] fence No description.
  */
void VulkanDevice::submit(const VkCommandBuffer& command_buffer,
                          const VkQueue& q,
                          const Fence& fence)
{
  const auto loader = dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  const zivcvk::Queue que{q};
  zivcvk::Fence fen{};
  if (fence.isActive())
    fen = *zisc::reinterp<const zivcvk::Fence*>(std::addressof(fence.data()));
  zivcvk::SubmitInfo info{};
  info.setCommandBufferCount(1);
  info.setPCommandBuffers(std::addressof(command));
  que.submit(info, fen, *loader);
}

/*!
  \details No detailed description

  \param [in] object_type No description.
  \param [in] object_handle No description.
  \param [in] object_name No description.
  \param [in] zivc_object No description.
  */
void VulkanDevice::setDebugInfo(const VkObjectType object_type,
                                const uint64b object_handle,
                                const std::string_view object_name,
                                const ZivcObject* zivc_object) noexcept
{
  const auto loader = dispatcher().loaderImpl();
  zivcvk::Device d{device()};
  // Name
  {
    const zivcvk::DebugUtilsObjectNameInfoEXT name_info{
        zisc::cast<zivcvk::ObjectType>(object_type),
        object_handle,
        object_name.data()};
    auto result = d.setDebugUtilsObjectNameEXT(std::addressof(name_info), *loader);
  }
  // Tag
  if (zivc_object != nullptr) {
    const IdData& id_data = zivc_object->id();
    const zivcvk::DebugUtilsObjectTagInfoEXT tag_info{
        zisc::cast<zivcvk::ObjectType>(object_type),
        object_handle,
        zisc::cast<uint64b>(id_data.id()),
        sizeof(zivc_object),
        zivc_object};
    auto result = d.setDebugUtilsObjectTagEXT(std::addressof(tag_info), *loader);
  }
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void VulkanDevice::setNumOfFences(const std::size_t s)
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  auto& fence_list = *fence_list_;
  fence_manager_->setSize(s);
  // Add new fences
  for (std::size_t i = fence_list.size(); i < s; ++i) {
    const zivcvk::FenceCreateInfo info{};
    zivcvk::Fence fence = d.createFence(info, alloc, *loader);
    auto result = d.resetFences(1, std::addressof(fence), *loader);
    fence_manager_->set(fence_list.size(), true);
    fence_list.emplace_back(zisc::cast<VkFence>(fence));
  }
  // Remove fences
  for (std::size_t i = fence_list.size(); s < i; --i) {
    zivcvk::Fence fence = zisc::cast<zivcvk::Fence>(fence_list.back());
    d.destroyFence(fence, alloc, *loader);
    fence_list.pop_back();
  }
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void VulkanDevice::takeFence(Fence* fence)
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();

  auto memory = std::addressof(fence->data());
  auto dest = ::new (zisc::cast<void*>(memory)) zivcvk::Fence{};

  auto& fence_list = *fence_list_;
  for (std::size_t i = 0; i < fence_list.size(); ++i) {
    if (fence_manager_->test(i)) {
      zivcvk::Fence f = zisc::cast<zivcvk::Fence>(fence_list[i]);
      auto result = d.resetFences(1, std::addressof(f), *loader);
      *dest = f;
      fence_manager_->set(i, false);
      break;
    }
  }

  if (!*dest) {
    //! \todo Available fence isn't found. Raise an exception?
    printf("[Warning] Available fence not found\n");
  }
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t VulkanDevice::totalMemoryUsage(const std::size_t number) const noexcept
{
  std::size_t s = 0;
  if (heap_usage_list_) {
    const auto& usage = (*heap_usage_list_)[number];
    s = usage.total();
  }
  return s;
}

/*!
  \details No detailed description
  */
void VulkanDevice::waitForCompletion() const noexcept
{
  const zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  d.waitIdle(*loader);
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
void VulkanDevice::waitForCompletion(const uint32b queue_index) const noexcept
{
  const auto q = zisc::cast<zivcvk::Queue>(getQueue(queue_index));
  const auto loader = dispatcher().loaderImpl();
  q.waitIdle(*loader);
}

/*!
  \details No detailed description

  \param [in] fence No description.
  */
void VulkanDevice::waitForCompletion(const Fence& fence) const noexcept
{
  const zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();

  auto f = zisc::reinterp<const zivcvk::Fence*>(std::addressof(fence.data()));
  constexpr uint64b timeout = std::numeric_limits<uint64b>::max();
  const auto result = d.waitForFences(1, f, VK_TRUE, timeout, *loader);
  if (result != zivcvk::Result::eSuccess) {
    //! \todo Raise an error
    printf("[Warning] Waiting for a fence failed.\n");
  }
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanDevice::allocateMemory(const std::size_t size,
                                  const BufferUsage buffer_usage,
                                  const VkBufferUsageFlagBits desc_type,
                                  void* user_data,
                                  VkBuffer* buffer,
                                  VmaAllocation* vm_allocation,
                                  VmaAllocationInfo* alloc_info)
{
  // Buffer create info
  zivcvk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  const auto descriptor_type = zisc::cast<zivcvk::BufferUsageFlagBits>(desc_type);
  buffer_create_info.usage = zivcvk::BufferUsageFlagBits::eTransferSrc |
                             zivcvk::BufferUsageFlagBits::eTransferDst |
                             descriptor_type;
  buffer_create_info.sharingMode = zivcvk::SharingMode::eExclusive;
  const uint32b queue_family_index = queueFamilyIndex();
  buffer_create_info.queueFamilyIndexCount = 1;
  buffer_create_info.pQueueFamilyIndices = std::addressof(queue_family_index);

  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = 0;
  switch (buffer_usage) {
   case BufferUsage::kDeviceOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    break;
   }
   case BufferUsage::kHostOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    break;
   }
   case BufferUsage::kHostToDevice: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    break;
   }
   case BufferUsage::kDeviceToHost: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
    break;
   }
  }
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = VK_NULL_HANDLE;
  alloc_create_info.pUserData = user_data;

  // 
  const auto binfo = zisc::cast<VkBufferCreateInfo>(buffer_create_info);
  const auto result = vmaCreateBuffer(memoryAllocator(),
                                      std::addressof(binfo),
                                      std::addressof(alloc_create_info),
                                      buffer,
                                      vm_allocation,
                                      alloc_info);
  if (result != VK_SUCCESS) {
    //! \todo Handle exception
    printf("[Warning]: Device memory allocation failed.\n");
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] source_buffer No description.
  \param [in] dest_buffer No description.
  \param [in] region No description.
  */
void VulkanDevice::copyBufferCmd(const VkCommandBuffer& command_buffer,
                                 const VkBuffer& source_buffer,
                                 const VkBuffer& dest_buffer,
                                 const VkBufferCopy& region)
{
  const auto loader = dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer source{source_buffer};
  ZISC_ASSERT(source, "The given source buffer is null.");
  const zivcvk::Buffer dest{dest_buffer};
  ZISC_ASSERT(dest, "The given dest buffer is null.");
  const zivcvk::BufferCopy copy_region{region};
  command.copyBuffer(source, dest, copy_region, *loader);
}

/*!
  \details No detailed description

  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [in,out] alloc_info No description.
  */
void VulkanDevice::deallocateMemory(VkBuffer* buffer,
                                    VmaAllocation* vm_allocation,
                                    VmaAllocationInfo* alloc_info) noexcept
{
  if (zivcvk::Buffer{*buffer}) {
    vmaDestroyBuffer(memoryAllocator(), *buffer, *vm_allocation);
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] buffer No description.
  \param [in] dest_offset No description.
  \param [in] size No description.
  \param [in] data No description.
  */
void VulkanDevice::fillBufferFastCmd(const VkCommandBuffer& command_buffer,
                                     const VkBuffer& buffer,
                                     const std::size_t dest_offset,
                                     const std::size_t size,
                                     const uint32b data) noexcept
{
  const auto loader = dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer buf{buffer};
  ZISC_ASSERT(buf, "The given buffer is null.");
  command.fillBuffer(buf, dest_offset, size, data, *loader);
}

/*!
  \details No detailed description

  \param [out] descriptor_set_layout No description.
  \param [out] descriptor_pool No description.
  */
void VulkanDevice::destroyKernelDescriptorSet(
    VkDescriptorSetLayout* descriptor_set_layout,
    VkDescriptorPool* descriptor_pool) noexcept
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  if (d) {
    zivcvk::DescriptorPool desc_pool{*descriptor_pool};
    if (desc_pool)
      d.destroyDescriptorPool(desc_pool, alloc, *loader);
    *descriptor_pool = VK_NULL_HANDLE;
    zivcvk::DescriptorSetLayout desc_set_layout{*descriptor_set_layout};
    if (desc_set_layout)
      d.destroyDescriptorSetLayout(desc_set_layout, alloc, *loader);
    *descriptor_set_layout = VK_NULL_HANDLE;
  }
}

/*!
  \details No detailed description

  \param [out] pipeline_layout No description.
  \param [out] compute_pipeline No description.
  */
void VulkanDevice::destroyKernelPipeline(
    VkPipelineLayout* pipeline_layout,
    VkPipeline* compute_pipeline) noexcept
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
  if (d) {
    zivcvk::Pipeline compute_pline{*compute_pipeline};
    if (compute_pline)
      d.destroyPipeline(compute_pline, alloc, *loader);
    *compute_pipeline = VK_NULL_HANDLE;
    zivcvk::PipelineLayout pline_layout{*pipeline_layout};
    if (pline_layout)
      d.destroyPipelineLayout(pline_layout, alloc, *loader);
    *pipeline_layout = VK_NULL_HANDLE;
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] descriptor_set No description.
  \param [in] pipeline_layout No description.
  \param [in] work_dimension No description.
  \param [in] work_size No description.
  */
void VulkanDevice::dispatchKernelCmd(const VkCommandBuffer& command_buffer,
                                     const VkDescriptorSet& descriptor_set,
                                     const VkPipelineLayout& pipeline_layout,
                                     const VkPipeline& pipeline,
                                     const std::size_t work_dimension,
                                     const std::array<uint32b, 3>& work_size)
{
  const auto loader = dispatcher().loaderImpl();

  const zivcvk::CommandBuffer command{command_buffer};
  ZISC_ASSERT(command, "The given command buffer is null.");

  constexpr auto bind_point = zivcvk::PipelineBindPoint::eCompute;
  const zivcvk::DescriptorSet desc_set{descriptor_set};
  const zivcvk::PipelineLayout pline_layout{pipeline_layout};
  command.bindDescriptorSets(bind_point, pline_layout, 0, desc_set, nullptr, *loader);

  const zivcvk::Pipeline pline{pipeline};
  command.bindPipeline(bind_point, pline, *loader);

  const auto disp_size = calcDispatchSize(work_dimension, work_size);
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
void VulkanDevice::initKernelDescriptorSet(
    const std::size_t num_of_storage_buffers,
    const std::size_t num_of_uniform_buffers,
    VkDescriptorSetLayout* descriptor_set_layout,
    VkDescriptorPool* descriptor_pool,
    VkDescriptorSet* descriptor_set)
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

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
                                                       std::addressof(desc_set_layout)};
    zisc::pmr::polymorphic_allocator<zivcvk::DescriptorSet> set_alloc{mem_resource};
    auto desc_set = d.allocateDescriptorSets(alloc_info, set_alloc, *loader);

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
void VulkanDevice::initKernelPipeline(const std::size_t work_dimension,
                                      const std::size_t num_of_local_args,
                                      const VkDescriptorSetLayout& set_layout,
                                      const VkShaderModule& module,
                                      const std::string_view kernel_name,
                                      VkPipelineLayout* pipeline_layout,
                                      VkPipeline* compute_pipeline)
{
  auto& sub_platform = parentImpl();
  const auto& info = deviceInfoData();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  // Pipeline
  zivcvk::PipelineLayout pline_layout;
  {
    const zivcvk::PushConstantRange push_constant_range{
        zivcvk::ShaderStageFlagBits::eCompute, 0, 28}; // global and region offsets

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
  const auto& work_group_size = workGroupSizeDim(work_dimension);
  zisc::pmr::vector<uint32b>::allocator_type spec_alloc{mem_resource};
  zisc::pmr::vector<uint32b> spec_constants{spec_alloc};
  {
    spec_constants.reserve(work_group_size.size() + 1);
    // Work group size
    for (const uint32b s : work_group_size)
      spec_constants.emplace_back(s);
    // Local element size
    spec_constants.emplace_back(info.workGroupSize());
  }
  using MapEntryArray = zisc::pmr::vector<zivcvk::SpecializationMapEntry>;
  MapEntryArray::allocator_type entry_alloc{mem_resource};
  MapEntryArray entries{entry_alloc};
  {
    entries.reserve(work_group_size.size() + num_of_local_args);
    // Work group size
    for (std::size_t i = 0; i < work_group_size.size(); ++i) {
      zivcvk::SpecializationMapEntry entry{
          zisc::cast<uint32b>(i),
          zisc::cast<uint32b>(i * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // Local element size
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
    if (isDebugMode()) {
      pipeline_flags |= zivcvk::PipelineCreateFlagBits::eCaptureStatisticsKHR;
    }
    const zivcvk::ComputePipelineCreateInfo pipeline_info{
        pipeline_flags,
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
  */
void VulkanDevice::destroyData() noexcept
{
  queue_family_index_ = invalidQueueIndex();

  if (vm_allocator_) {
    vmaDestroyAllocator(vm_allocator_);
    vm_allocator_ = VK_NULL_HANDLE;
  }

  zivcvk::Device d{device()};
  if (d) {
    auto& sub_platform = parentImpl();
    zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
    const auto loader = dispatcher().loaderImpl();

    setNumOfFences(0); // Destroy all fences

    // Shader modules
    for (auto& module : *shader_module_list_) {
      zivcvk::ShaderModule m{module.second.module_};
      if (m)
        d.destroyShaderModule(m, alloc, *loader);
    }

    // Command pool
    zivcvk::CommandPool command_pool{command_pool_};
    if (command_pool) {
      d.destroyCommandPool(command_pool, alloc, *loader);
      command_pool = nullptr;
    }

    d.destroy(alloc, *loader);
    device_ = VK_NULL_HANDLE;
  }

  shader_module_list_.reset();
  dispatcher_.reset();
  heap_usage_list_.reset();
  queue_list_.reset();
  fence_list_.reset();
  fence_manager_.reset();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initData()
{
  auto mem_resource = memoryResource();
  {
    using FenceManager = decltype(fence_manager_)::element_type;
    FenceManager manager{mem_resource};
    zisc::pmr::polymorphic_allocator<FenceManager> alloc{mem_resource};
    fence_manager_ = zisc::pmr::allocateUnique(alloc, std::move(manager));
  }
  {
    using FenceList = decltype(fence_list_)::element_type;
    FenceList::allocator_type allocs{mem_resource};
    FenceList fence_list{allocs};
    zisc::pmr::polymorphic_allocator<FenceList> alloc{mem_resource};
    fence_list_ = zisc::pmr::allocateUnique(alloc, std::move(fence_list));
  }
  {
    using QueueList = decltype(queue_list_)::element_type;
    QueueList::allocator_type allocs{mem_resource};
    QueueList queue_list{allocs};
    zisc::pmr::polymorphic_allocator<QueueList> alloc{mem_resource};
    queue_list_ = zisc::pmr::allocateUnique(alloc, std::move(queue_list));
  }
  {
    using UsageList = decltype(heap_usage_list_)::element_type;
    UsageList::allocator_type alloce{mem_resource};
    UsageList usage_list{alloce};
    zisc::pmr::polymorphic_allocator<UsageList> alloc{mem_resource};
    heap_usage_list_ = zisc::pmr::allocateUnique(alloc, std::move(usage_list));
    const auto& info = deviceInfoData();
    heap_usage_list_->resize(info.numOfHeaps());
  }
  {
    using ShaderModuleList = decltype(shader_module_list_)::element_type;
    ShaderModuleList::allocator_type allocs{mem_resource};
    ShaderModuleList module_list{allocs};
    zisc::pmr::polymorphic_allocator<ShaderModuleList> alloc{mem_resource};
    shader_module_list_ = zisc::pmr::allocateUnique(alloc, std::move(module_list));
  }

  initDispatcher();
  initWorkGroupSizeDim();
  initQueueFamilyIndexList();
  initDevice();
  initMemoryAllocator();
  initCommandPool();
  setNumOfFences(1);
}

/*!
  \details No detailed description
  */
void VulkanDevice::updateDebugInfoImpl() noexcept
{
  const IdData& id_data = id();
  // Device
  {
    const VkDevice& handle = device();
    const zivcvk::Device d{handle};
    if (d) {
      setDebugInfo(zisc::cast<VkObjectType>(d.objectType),
                   *zisc::reinterp<const uint64b*>(std::addressof(handle)),
                   id_data.name(),
                   this);
    }
  }
  // Command pool
  {
    const VkCommandPool handle = commandPool();
    const zivcvk::CommandPool p{handle};
    if (p) {
      IdData::NameType obj_name{""};
      std::strcat(obj_name.data(), id_data.name().data());
      std::strcat(obj_name.data(), "_pool");
      setDebugInfo(zisc::cast<VkObjectType>(p.objectType),
                   *zisc::reinterp<const uint64b*>(std::addressof(handle)),
                   obj_name.data(),
                   this);
    }
  }
  // Shader modules
  for (const auto& module : *shader_module_list_)
    updateShaderModuleDebugInfo(module.first);
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] memory_type No description.
  \param [out] number No description.
  \return No description
  */
bool VulkanDevice::Callbacks::getHeapNumber(const VulkanDevice& device,
                                           const uint32b memory_type,
                                           std::size_t* number) noexcept
{
  const auto& info = device.deviceInfoData();
  const auto& mem_props = info.memoryProperties().properties1_;
  const std::size_t index = mem_props.memoryTypes[memory_type].heapIndex;
  const zivcvk::MemoryHeap heap{mem_props.memoryHeaps[index]};
  bool is_device_heap = false;
  if (heap.flags & zivcvk::MemoryHeapFlagBits::eDeviceLocal) {
    *number = info.getDeviceHeapNumber(index);
    is_device_heap = true;
  }
  return is_device_heap;
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryAllocation(
    VmaAllocator vm_allocator,
    uint32b memory_type,
    VkDeviceMemory memory,
    VkDeviceSize size,
    void* user_data)
{
  static_cast<void>(vm_allocator);
  static_cast<void>(memory);
  auto device = zisc::cast<VulkanDevice*>(user_data);
  std::size_t heap_index = 0;
  const bool is_index_found = getHeapNumber(*device, memory_type, &heap_index);
  if (is_index_found)
    (*device->heap_usage_list_)[heap_index].add(size);
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryFreeing(
    VmaAllocator vm_allocator,
    uint32b memory_type,
    VkDeviceMemory memory,
    VkDeviceSize size,
    void* user_data)
{
  static_cast<void>(vm_allocator);
  static_cast<void>(memory);
  auto device = zisc::cast<VulkanDevice*>(user_data);
  std::size_t heap_index = 0;
  const bool is_index_found = getHeapNumber(*device, memory_type, &heap_index);
  if (is_index_found)
    (*device->heap_usage_list_)[heap_index].release(size);
}

/*!
  \details No detailed description

  \param [in] id No description.
  \param [in] spirv_code No description.
  */
void VulkanDevice::addShaderModule(const uint32b id,
                                   const zisc::pmr::vector<uint32b>& spirv_code,
                                   const std::string_view module_name)
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  const std::size_t code_size = spirv_code.size() * sizeof(spirv_code[0]);
  zivcvk::ShaderModuleCreateInfo create_info{zivcvk::ShaderModuleCreateFlags{},
                                             code_size,
                                             spirv_code.data()};

  auto module = d.createShaderModule(create_info, alloc, *loader);
  ModuleData module_data{zisc::cast<VkShaderModule>(module), module_name};
  shader_module_list_->emplace(id, std::move(module_data));
  updateShaderModuleDebugInfo(id);
}

/*!
  \details No detailed description

  \return No description
  */
uint32b VulkanDevice::findQueueFamily() const noexcept
{
  auto has_flags = [](const zivcvk::QueueFamilyProperties& props,
                      const bool graphics_excluded,
                      const bool sparse_excluded) noexcept
  {
    auto has_flag = [&props](const zivcvk::QueueFlagBits flag) noexcept
    {
      const bool result = (props.queueFlags & flag) == flag;
      return result;
    };
    const bool result =
        has_flag(zivcvk::QueueFlagBits::eCompute) &&
        has_flag(zivcvk::QueueFlagBits::eTransfer) &&
        (!has_flag(zivcvk::QueueFlagBits::eGraphics) || !graphics_excluded) &&
        (!has_flag(zivcvk::QueueFlagBits::eSparseBinding) || !sparse_excluded);
    return result;
  };

  auto find_queue_family = [has_flags](const auto& info,
                                       const bool graphics_excluded,
                                       const bool sparse_excluded,
                                       uint32b* index,
                                       uint32b* num_of_queues) noexcept
  {
    const auto& queue_family_list = info.queueFamilyPropertiesList();
    for (std::size_t i = 0; i < queue_family_list.size(); ++i) {
      const auto& p = queue_family_list[i];
      if (has_flags(p, graphics_excluded, sparse_excluded) &&
          (*num_of_queues <= p.queueCount)) {
        *index = zisc::cast<uint32b>(i);
        *num_of_queues = p.queueCount;
      }
    }
  };

  const auto& info = deviceInfoData();
  uint32b index = invalidQueueIndex();
  uint32b num_of_queues = 0;
  find_queue_family(info, true, false, &index, &num_of_queues);
  if (index == invalidQueueIndex())
    find_queue_family(info, false, false, &index, &num_of_queues);

  return index;
}

/*!
  \details No detailed description

  \return No description
  */
VmaVulkanFunctions VulkanDevice::getVmaVulkanFunctions() noexcept
{
  const auto loader = dispatcher().loaderImpl();
  VmaVulkanFunctions functions;
  functions.vkGetPhysicalDeviceProperties = loader->vkGetPhysicalDeviceProperties;
  functions.vkGetPhysicalDeviceMemoryProperties = loader->vkGetPhysicalDeviceMemoryProperties;
  functions.vkAllocateMemory = loader->vkAllocateMemory;
  functions.vkFreeMemory = loader->vkFreeMemory;
  functions.vkMapMemory = loader->vkMapMemory;
  functions.vkUnmapMemory = loader->vkUnmapMemory;
  functions.vkFlushMappedMemoryRanges = loader->vkFlushMappedMemoryRanges;
  functions.vkInvalidateMappedMemoryRanges = loader->vkInvalidateMappedMemoryRanges;
  functions.vkBindBufferMemory = loader->vkBindBufferMemory;
  functions.vkBindImageMemory = loader->vkBindImageMemory;
  functions.vkGetBufferMemoryRequirements = loader->vkGetBufferMemoryRequirements;
  functions.vkGetImageMemoryRequirements = loader->vkGetImageMemoryRequirements;
  functions.vkCreateBuffer = loader->vkCreateBuffer;
  functions.vkDestroyBuffer = loader->vkDestroyBuffer;
  functions.vkCreateImage = loader->vkCreateImage;
  functions.vkDestroyImage = loader->vkDestroyImage;
  functions.vkCmdCopyBuffer = loader->vkCmdCopyBuffer;

  if (loader->vkGetBufferMemoryRequirements2)
    functions.vkGetBufferMemoryRequirements2KHR = loader->vkGetBufferMemoryRequirements2;
  else
    functions.vkGetBufferMemoryRequirements2KHR = loader->vkGetBufferMemoryRequirements2KHR;
  if (loader->vkGetImageMemoryRequirements2)
    functions.vkGetImageMemoryRequirements2KHR = loader->vkGetImageMemoryRequirements2;
  else 
    functions.vkGetImageMemoryRequirements2KHR = loader->vkGetImageMemoryRequirements2KHR;
  if (loader->vkBindBufferMemory2)
    functions.vkBindBufferMemory2KHR = loader->vkBindBufferMemory2;
  else
    functions.vkBindBufferMemory2KHR = loader->vkBindBufferMemory2KHR;
  if (loader->vkBindImageMemory2)
    functions.vkBindImageMemory2KHR = loader->vkBindImageMemory2;
  else
    functions.vkBindImageMemory2KHR = loader->vkBindImageMemory2KHR;
  if (loader->vkGetPhysicalDeviceMemoryProperties2)
    functions.vkGetPhysicalDeviceMemoryProperties2KHR = loader->vkGetPhysicalDeviceMemoryProperties2;
  else
    functions.vkGetPhysicalDeviceMemoryProperties2KHR = loader->vkGetPhysicalDeviceMemoryProperties2KHR;
  return functions;
}

/*!
  \details No detailed description
  */
void VulkanDevice::initCommandPool()
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  const zivcvk::CommandPoolCreateInfo create_info{
      zivcvk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      queueFamilyIndex()};

  auto command_pool = d.createCommandPool(create_info, alloc, *loader);
  command_pool_ = zisc::cast<VkCommandPool>(command_pool);
}

/*!
  \details No detailed description
  */
void VulkanDevice::initDevice()
{
  auto& sub_platform = parentImpl();

  zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

  extensions = {//VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME,
#if defined(Z_MAC)
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
#endif // Z_MAC
                VK_EXT_MEMORY_BUDGET_EXTENSION_NAME};
  if (sub_platform.isDebugMode()) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
  }

  const auto& info = deviceInfoData();

  // Queue create info
  zisc::pmr::vector<float>::allocator_type priority_alloc{memoryResource()};
  zisc::pmr::vector<float> priority_list{priority_alloc};
  zivcvk::DeviceQueueCreateInfo queue_create_info;
  {
    // Queue family index
    const uint32b index = queueFamilyIndex();
    queue_create_info.setQueueFamilyIndex(index);
    // Queue counts
    const auto& queue_family_list = info.queueFamilyPropertiesList();
    const uint32b num_of_queues = queue_family_list[index].queueCount;
    queue_create_info.setQueueCount(num_of_queues);
    // Priorities
    priority_list.resize(num_of_queues, 1.0f);
    queue_create_info.setPQueuePriorities(priority_list.data());
  }

  // Device features
  auto [device_features, f] = ::getDefaultFeatures(info, memoryResource());

  zivcvk::DeviceCreateInfo device_create_info{
      zivcvk::DeviceCreateFlags{},
      1,
      &queue_create_info,
      zisc::cast<uint32b>(layers.size()),
      layers.data(),
      zisc::cast<uint32b>(extensions.size()),
      extensions.data(),
      nullptr};
  device_create_info.setPNext(std::addressof(device_features));

  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
  const auto loader = dispatcher().loaderImpl();
  const auto pdevice = zisc::cast<zivcvk::PhysicalDevice>(info.device());
  auto d = pdevice.createDevice(device_create_info, alloc, *loader);
  device_ = zisc::cast<VkDevice>(d);
  dispatcher_->set(device());

  // Initialize queue list
  queue_list_->resize(numOfQueues(), VK_NULL_HANDLE);
  for (std::size_t i = 0; i < queue_list_->size(); ++i) {
    auto q = d.getQueue(queueFamilyIndex(), zisc::cast<uint32b>(i), *loader);
    (*queue_list_)[i] = zisc::cast<VkQueue>(q);
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initDispatcher()
{
  const auto& sub_platform = parentImpl();
  dispatcher_ = zisc::pmr::allocateUnique<VulkanDispatchLoader>(
      memoryResource(),
      sub_platform.dispatcher());
}

/*!
  \details No detailed description
  */
void VulkanDevice::initWorkGroupSizeDim() noexcept
{
  const auto& info = deviceInfoData();
  const uint32b group_size = info.workGroupSize();

  for (uint32b dim = 1; dim <= work_group_size_list_.size(); ++dim) {
    std::array<uint32b, 3> work_group_size{{1, 1, 1}};
    const auto product = [](const std::array<uint32b, 3>& s) noexcept
    {
      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
    };
    for (uint32b i = 0; product(work_group_size) < group_size; i = (i + 1) % dim)
      work_group_size[i] *= 2;
    ZISC_ASSERT(product(work_group_size) == group_size,
                "The work-group size should be power of 2: group size = ",
                product(work_group_size));
    work_group_size_list_[dim - 1] = work_group_size;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initQueueFamilyIndexList() noexcept
{
  //! \todo Support queue family option
  queue_family_index_ = findQueueFamily();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initMemoryAllocator()
{
  auto& sub_platform = parentImpl();
  const auto& info = deviceInfoData();

  VkAllocationCallbacks alloc = sub_platform.makeAllocator();
  VmaDeviceMemoryCallbacks notifier = makeAllocationNotifier();
  VmaVulkanFunctions functions = getVmaVulkanFunctions();

  VmaAllocatorCreateInfo create_info;
  create_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
  create_info.physicalDevice = info.device();
  create_info.device = device();
  create_info.preferredLargeHeapBlockSize = 0;
  create_info.pAllocationCallbacks = std::addressof(alloc);
  create_info.pDeviceMemoryCallbacks = std::addressof(notifier);
  create_info.frameInUseCount = 0;
  create_info.pHeapSizeLimit = nullptr;
  create_info.pVulkanFunctions = std::addressof(functions);
  create_info.pRecordSettings = nullptr;
  create_info.instance = sub_platform.instance();
  create_info.vulkanApiVersion = sub_platform.apiVersion();
  auto result = vmaCreateAllocator(std::addressof(create_info),
                                   std::addressof(vm_allocator_));
  if (result != VK_SUCCESS) {
    //! \todo Handling exceptions
    printf("[Warning]: Vma creation failed.\n");
  }
}

/*!
  \details No detailed description

  \return No description
  */
VmaDeviceMemoryCallbacks VulkanDevice::makeAllocationNotifier() noexcept
{
  VmaDeviceMemoryCallbacks notifier;
  notifier.pfnAllocate = Callbacks::notifyOfDeviceMemoryAllocation;
  notifier.pfnFree = Callbacks::notifyOfDeviceMemoryFreeing;
  notifier.pUserData = this;
  return notifier;
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
void VulkanDevice::updateDescriptorSet(const VkDescriptorSet& descriptor_set,
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

  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto descs = zisc::reinterp<const zivcvk::WriteDescriptorSet*>(write_desc_list);
  d.updateDescriptorSets(zisc::cast<uint32b>(n), descs, 0, nullptr, *loader);
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
void VulkanDevice::updateShaderModuleDebugInfo(const uint32b id) noexcept
{
  const zivcvk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const ModuleData& module_data = getShaderModule(id);

    IdData::NameType module_name{""};
    std::strcat(module_name.data(), id_data.name().data());
    std::strcat(module_name.data(), "_");
    std::strcat(module_name.data(), module_data.name_.data());

    const zivcvk::ShaderModule m{module_data.module_};
    setDebugInfo(zisc::cast<VkObjectType>(m.objectType),
                 *zisc::reinterp<const uint64b*>(std::addressof(module_data.module_)),
                 module_name.data(),
                 this);
  }
}

} // namespace zivc
