/*!
  \file vulkan_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device.hpp"
// Standard C++ library
#include <array>
#include <algorithm>
#include <charconv>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/hash/fnv_1a_hash_engine.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "vulkan_sub_platform.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/error.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/id_data.hpp"

namespace {

/*!
  \brief Enable device features used in zivc
  \details No detailed description

  \return No description
  \exception std::exception No description.
  */
auto getDefaultFeatures(const zivc::VulkanDeviceInfo& info,
                        zisc::pmr::memory_resource* mem_resource) noexcept
{
  struct Features
  {
    zivcvk::PhysicalDevice16BitStorageFeatures b16bit_storage_;
    zivcvk::PhysicalDevice8BitStorageFeatures b8bit_storage_;
    zivcvk::PhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_;
    zivcvk::PhysicalDevicePortabilitySubsetFeaturesKHR portability_subset_;
    zivcvk::PhysicalDeviceRayQueryFeaturesKHR ray_query_;
    zivcvk::PhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_;
    zivcvk::PhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    zivcvk::PhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    zivcvk::PhysicalDeviceShaderClockFeaturesKHR shader_clock_;
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

  auto f = zisc::pmr::allocateUnique<Features>(mem_resource);
  f->b16bit_storage_ = inputs.b16bit_storage_;
  f->b8bit_storage_ = inputs.b8bit_storage_;
  f->acceleration_structure_ = inputs.acceleration_structure_;
  f->portability_subset_ = inputs.portability_subset_;
  f->ray_query_ = inputs.ray_query_;
  f->ray_tracing_pipeline_ = inputs.ray_tracing_pipeline_features_;
  f->shader_atomic_float_ = inputs.shader_atomic_float_;
  f->shader_atomic_int64_ = inputs.shader_atomic_int64_;
  f->shader_clock_ = inputs.shader_clock_;
  f->shader_float16_int8_ = inputs.shader_float16_int8_;
  f->variable_pointers_ = inputs.variable_pointers_;
  f->vulkan_memory_model_ = inputs.vulkan_memory_model_;

  zivc::VulkanDeviceInfo::link(features,
                               f->b16bit_storage_,
                               f->b8bit_storage_,
                               f->acceleration_structure_,
                               f->portability_subset_,
                               f->ray_query_,
                               f->ray_tracing_pipeline_,
                               f->shader_atomic_float_,
                               f->shader_atomic_int64_,
                               f->shader_clock_,
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

  \param [in] module No description.
  \param [in] kernel_name No description.
  \param [in] work_dimension No description.
  \param [in] num_of_storage_buffers No description.
  \param [in] num_of_uniform_buffers No description.
  \param [in] num_of_local_args No description.
  \return No description
  */
auto VulkanDevice::addShaderKernel(const ModuleData& module,
                                   const std::string_view kernel_name,
                                   const std::size_t work_dimension,
                                   const std::size_t num_of_storage_buffers,
                                   const std::size_t num_of_uniform_buffers,
                                   const std::size_t num_of_local_args)
    -> const KernelData&
{
  const uint32b id = getKernelId(module.name_, kernel_name);
  if (hasShaderKernel(id))
    return getShaderKernel(id);

  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();
  zivcvk::AllocationCallbacks alloc{makeAllocator()};

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

  // Pipeline
  zivcvk::PipelineLayout pline_layout;
  {
    // For clspv module scope push constants
    const zivcvk::PushConstantRange push_constant_range{
        zivcvk::ShaderStageFlagBits::eCompute, 0, 92};

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
    spec_constants.reserve(work_group_size.size() + 2);
    // For clspv work group size
    for (const uint32b s : work_group_size)
      spec_constants.emplace_back(s);
    // Work dimension
    spec_constants.emplace_back(zisc::cast<uint32b>(work_dimension));
    // For clspv local element size
    const auto& info = deviceInfoImpl();
    spec_constants.emplace_back(info.workGroupSize());
  }
  using MapEntryList = zisc::pmr::vector<zivcvk::SpecializationMapEntry>;
  MapEntryList::allocator_type entry_alloc{mem_resource};
  MapEntryList entries{entry_alloc};
  {
    entries.reserve(work_group_size.size() + 1 + num_of_local_args);
    // For clspv work group size
    for (std::size_t i = 0; i < work_group_size.size(); ++i) {
      zivcvk::SpecializationMapEntry entry{
          zisc::cast<uint32b>(i),
          zisc::cast<uint32b>(i * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // Work dimension
    {
      const uint32b work_dim_index = zisc::cast<uint32b>(entries.size());
      zivcvk::SpecializationMapEntry entry{
          work_dim_index,
          zisc::cast<uint32b>(work_dim_index * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // For clspv local element size
    for (std::size_t i = 0; i < num_of_local_args; ++i) {
      const uint32b local_size_index = zisc::cast<uint32b>(entries.size());
      zivcvk::SpecializationMapEntry entry{
          local_size_index,
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
        zivcvk::ShaderModule{module.module_},
        kernel_name.data(),
        std::addressof(spec_info)};
    zivcvk::PipelineCreateFlags pipeline_flags;
    if (isDebugMode()) {
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
      const char* message = "Compute pipeline creation failed.";
      zivcvk::throwResultException(result.result, message);
    }
    pline = zisc::cast<zivcvk::Pipeline>(result.value);
  }

  const KernelData* data = nullptr;
  {
    zisc::pmr::polymorphic_allocator<KernelData> data_alloc{mem_resource};
    auto kernel_data = zisc::pmr::allocateUnique(data_alloc);
    data = kernel_data.get();
    kernel_data->module_ = std::addressof(module);
    std::strcpy(kernel_data->kernel_name_.data(), kernel_name.data());
    kernel_data->desc_set_layout_ =
        zisc::cast<VkDescriptorSetLayout>(desc_set_layout);
    kernel_data->pipeline_layout_ = zisc::cast<VkPipelineLayout>(pline_layout);
    kernel_data->pipeline_ = zisc::cast<VkPipeline>(pline);

    std::unique_lock<std::shared_mutex> lock{shader_mutex_};
    kernel_data_list_->emplace(id, std::move(kernel_data));
  }
  updateKernelDataDebugInfo(*data);
  return *data;
}

/*!
  \details No detailed description

  \param [in] module_name No description.
  \param [in] kernel_name No description.
  \return No description
  */
uint32b VulkanDevice::getKernelId(const std::string_view module_name,
                                  const std::string_view kernel_name) noexcept
{
  IdData::NameType kernel_id_name{""};
  std::strncpy(kernel_id_name.data(), module_name.data(), module_name.size() + 1);
  std::strncat(kernel_id_name.data(), kernel_name.data(), kernel_name.size() + 1);
  const uint32b kernel_id = zisc::Fnv1aHash32::hash(kernel_id_name.data());
  return kernel_id;
}

/*!
  \details No detailed description

  \return No description
  */
VkAllocationCallbacks VulkanDevice::makeAllocator() noexcept
{
  auto& sub_platform = parentImpl();
  return sub_platform.makeAllocator();
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] label_name No description.
  \param [in] color No description.
  \return No description
  */
CmdDebugLabelRegion VulkanDevice::makeCmdDebugLabel(
    const VkCommandBuffer& command_buffer,
    const std::string_view label_name,
    const std::array<float, 4>& color) const noexcept
{
  const VkCommandBuffer& cmd = isDebugMode() ? command_buffer : ZIVC_VK_NULL_HANDLE;
  return CmdDebugLabelRegion{cmd, dispatcher(), label_name, color};
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] flags No description.
  \return No description
  */
CmdRecordRegion VulkanDevice::makeCmdRecord(
    const VkCommandBuffer& command_buffer,
    const VkCommandBufferUsageFlags flags) const
{
  return CmdRecordRegion{command_buffer, dispatcher(), flags};
}

/*!
  \details No detailed description

  \param [in] q No description.
  \param [in] label_name No description.
  \param [in] color No description.
  \return No description
  */
QueueDebugLabelRegion VulkanDevice::makeQueueDebugLabel(
    const VkQueue& q,
    const std::string_view label_name,
    const std::array<float, 4>& color) const noexcept
{
  const VkQueue& que = isDebugMode() ? q : ZIVC_VK_NULL_HANDLE;
  return QueueDebugLabelRegion{que, dispatcher(), label_name, color};
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

  \param [in] heap_index No description.
  \return No description
  */
zisc::Memory::Usage& VulkanDevice::memoryUsage(const std::size_t heap_index) noexcept
{
  zisc::Memory::Usage& usage = (*heap_usage_list_)[heap_index];
  return usage;
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
const zisc::Memory::Usage& VulkanDevice::memoryUsage(const std::size_t heap_index) const noexcept
{
  zisc::Memory::Usage& usage = (*heap_usage_list_)[heap_index];
  return usage;
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
  const auto& info = deviceInfoImpl();
  const uint32b index = queueFamilyIndex();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  const std::size_t n = queue_family_list[index].properties1_.queueCount;
  return n;
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void VulkanDevice::returnFence(Fence* fence) noexcept
{
  auto dest = zisc::reinterp<zivcvk::Fence*>(std::addressof(fence->data()));

  constexpr std::size_t invalid = (std::numeric_limits<std::size_t>::max)();
  std::size_t fence_index = invalid;
  {
    const VkFence value = zisc::cast<VkFence>(*dest);
    const auto ite = std::lower_bound(fence_list_->begin(), fence_list_->end(), value);
    if ((ite != fence_list_->end()) && (*ite == value))
      fence_index = zisc::cast<std::size_t>(std::distance(fence_list_->begin(), ite));
  }

  if (fence_index != invalid) {
    IndexQueue& index_queue = fenceIndexQueue();
    [[maybe_unused]] const bool result = index_queue.enqueue(fence_index);
    ZISC_ASSERT(result, "Queueing fence index failed.");
    *dest = zivcvk::Fence{};
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
  zivcvk::Fence fen = fence.isActive()
      ? *zisc::reinterp<const zivcvk::Fence*>(std::addressof(fence.data()))
      : zivcvk::Fence{};
  zivcvk::SubmitInfo info{};
  info.setCommandBufferCount(1);
  info.setPCommandBuffers(std::addressof(command));
  que.submit(info, fen, *loader);
}

/*!
  \details No detailed description

  \param [in] vk_object_type No description.
  \param [in] vk_handle No description.
  \param [in] object_name No description.
  \param [in] zivc_object No description.
  */
void VulkanDevice::setDebugInfo(const VkObjectType vk_object_type,
                                const void* vk_handle,
                                const std::string_view object_name,
                                const ZivcObject* zivc_object)
{
  const auto loader = dispatcher().loaderImpl();
  zivcvk::Device d{device()};
  const auto object_type = zisc::cast<zivcvk::ObjectType>(vk_object_type);
  const auto handle = zisc::cast<uint64b>(zisc::reinterp<std::size_t>(vk_handle));
  // Name
  {
    const zivcvk::DebugUtilsObjectNameInfoEXT name_info{object_type,
                                                        handle,
                                                        object_name.data()};
    d.setDebugUtilsObjectNameEXT(name_info, *loader);
  }
  // Tag
  if (zivc_object != nullptr) {
    const auto tag_name = zisc::cast<uint64b>(zivc_object->id().id());
    const zivcvk::DebugUtilsObjectTagInfoEXT tag_info{object_type,
                                                      handle,
                                                      tag_name,
                                                      sizeof(zivc_object),
                                                      std::addressof(zivc_object)};
    d.setDebugUtilsObjectTagEXT(tag_info, *loader);
  }
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void VulkanDevice::setFenceSize(const std::size_t s)
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{makeAllocator()};

  fence_index_queue_->setCapacity(s);
  fence_index_queue_->clear();
  const std::size_t fence_size = (0 < s) ? fence_index_queue_->capacity() : 0;
  for (std::size_t index = 0; index < fence_size; ++index) {
    [[maybe_unused]] const bool result = fence_index_queue_->enqueue(index);
    ZISC_ASSERT(result, "Enqueing fence index failed: ", index);
  }

  auto& fence_list = *fence_list_;
  // Add new fences
  for (std::size_t index = fence_list.size(); index < fence_size; ++index) {
    const zivcvk::FenceCreateInfo info{};
    zivcvk::Fence fence = d.createFence(info, alloc, *loader);
    fence_list.emplace_back(zisc::cast<VkFence>(fence));
    updateFenceDebugInfo(index);
  }
  // Remove fences
  for (std::size_t i = fence_list.size(); fence_size < i; --i) {
    zivcvk::Fence fence = zisc::cast<zivcvk::Fence>(fence_list.back());
    d.destroyFence(fence, alloc, *loader);
    fence_list.pop_back();
  }
  // Reset fences
  for (VkFence& fence : fence_list) {
    zivcvk::Fence f = zisc::cast<zivcvk::Fence>(fence);
    d.resetFences(f, *loader);
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

  IndexQueue& index_queue = fenceIndexQueue();
  const auto [result, fence_index] = index_queue.dequeue();
  if (!result) {
    //! \todo Available fence isn't found. Raise an exception?
    const char* message = "Available fence not found.";
    throw SystemError{ErrorCode::kAvailableFenceNotFound, message};
  }

  auto memory = std::addressof(fence->data());
  auto dest = ::new (zisc::cast<void*>(memory)) zivcvk::Fence{};
  {
    zivcvk::Fence f = zisc::cast<zivcvk::Fence>((*fence_list_)[fence_index]);
    d.resetFences(f, *loader);
    *dest = f;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::waitForCompletion() const
{
  const zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  d.waitIdle(*loader);
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
void VulkanDevice::waitForCompletion(const uint32b queue_index) const
{
  const auto q = zisc::cast<zivcvk::Queue>(getQueue(queue_index));
  const auto loader = dispatcher().loaderImpl();
  q.waitIdle(*loader);
}

/*!
  \details No detailed description

  \param [in] fence No description.
  */
void VulkanDevice::waitForCompletion(const Fence& fence) const
{
  if (fence) {
    const zivcvk::Device d{device()};
    const auto loader = dispatcher().loaderImpl();

    auto f = zisc::reinterp<const zivcvk::Fence*>(std::addressof(fence.data()));
    constexpr uint64b timeout = (std::numeric_limits<uint64b>::max)();
    [[maybe_unused]] const auto result = d.waitForFences(*f,
                                                         VK_TRUE,
                                                         timeout,
                                                         *loader);
    ZISC_ASSERT(result == zivcvk::Result::eSuccess, "Waiting for a fence failed.");
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::destroyData() noexcept
{
  queue_family_index_ = invalidQueueIndex();

  if (vm_allocator_) {
    vmaDestroyAllocator(vm_allocator_);
    vm_allocator_ = ZIVC_VK_NULL_HANDLE;
  }

  zivcvk::Device d{device()};
  if (d) {
    zivcvk::AllocationCallbacks alloc{makeAllocator()};
    const auto loader = dispatcher().loaderImpl();

    setFenceSize(0); // Destroy all fences

    // Kernel data
    for (auto& kernel : *kernel_data_list_)
      destroyShaderKernel(kernel.second.get());

    // Shader modules
    for (auto& module : *module_data_list_)
      destroyShaderModule(module.second.get());

    // Command pool
    zivcvk::CommandPool command_pool{command_pool_};
    if (command_pool) {
      //! \todo Fix me. AMD gpu won't work with custom allocator
      d.destroyCommandPool(command_pool, nullptr /* alloc */, *loader);
      command_pool = nullptr;
    }

    d.destroy(alloc, *loader);
    device_ = ZIVC_VK_NULL_HANDLE;
  }

  kernel_data_list_.reset();
  module_data_list_.reset();
  dispatcher_.reset();
  heap_usage_list_.reset();
  queue_list_.reset();
  fence_list_.reset();
  fence_index_queue_.reset();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initData()
{
  auto mem_resource = memoryResource();
  {
    zisc::pmr::polymorphic_allocator<IndexQueueImpl> alloc{mem_resource};
    fence_index_queue_ = zisc::pmr::allocateUnique(alloc, mem_resource);
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
    const auto& info = deviceInfoImpl();
    heap_usage_list_->resize(info.heapInfoList().size());
  }
  {
    using ShaderModuleList = decltype(module_data_list_)::element_type;
    ShaderModuleList::allocator_type allocs{mem_resource};
    ShaderModuleList module_list{allocs};
    zisc::pmr::polymorphic_allocator<ShaderModuleList> alloc{mem_resource};
    module_data_list_ = zisc::pmr::allocateUnique(alloc, std::move(module_list));
  }
  {
    using KernelDataList = decltype(kernel_data_list_)::element_type;
    KernelDataList::allocator_type allocs{mem_resource};
    KernelDataList module_list{allocs};
    zisc::pmr::polymorphic_allocator<KernelDataList> alloc{mem_resource};
    kernel_data_list_ = zisc::pmr::allocateUnique(alloc, std::move(module_list));
  }

  initDispatcher();
  initWorkGroupSizeDim();
  initQueueFamilyIndexList();
  initDevice();
  initMemoryAllocator();
  initCommandPool();
  setFenceSize(1);
}

/*!
  \details No detailed description
  */
void VulkanDevice::updateDebugInfoImpl()
{
  const IdData& id_data = id();
  // Device
  {
    const VkDevice& handle = device();
    const zivcvk::Device d{handle};
    const std::string_view name = id_data.name();
    if (d) {
      setDebugInfo(zisc::cast<VkObjectType>(d.objectType), handle, name, this);
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
      const std::string_view name = obj_name.data();
      setDebugInfo(zisc::cast<VkObjectType>(p.objectType), handle, name, this);
    }
  }
  // Queue
  for (std::size_t i = 0; i < queue_list_->size(); ++i) {
    const VkQueue handle = getQueue(i);
    const zivcvk::Queue q{handle};
    if (q) {
      IdData::NameType obj_name{""};
      std::strcat(obj_name.data(), id_data.name().data());
      std::strcat(obj_name.data(), "_queue");
      IdData::NameType idx{""};
      auto [end, result] = std::to_chars(idx.data(),
                                         idx.data() + idx.size(),
                                         i);
      *end = '\0';
      std::strcat(obj_name.data(), idx.data());
      const std::string_view name = obj_name.data();
      setDebugInfo(zisc::cast<VkObjectType>(q.objectType), handle, name, this);
    }
  }
  // Fence
  for (std::size_t i = 0; i < fence_list_->size(); ++i)
    updateFenceDebugInfo(i);
  // Shader modules
  for (const auto& module : *module_data_list_)
    updateShaderModuleDebugInfo(*module.second);
  // Kernel data
  for (const auto& kernel : *kernel_data_list_)
    updateKernelDataDebugInfo(*kernel.second);
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] memory_type No description.
  \return No description
  */
std::size_t VulkanDevice::Callbacks::getHeapIndex(const VulkanDevice& device,
                                                  const uint32b memory_type) noexcept
{
  const auto& info = device.deviceInfoImpl();
  const auto& mem_props = info.memoryProperties().properties1_;
  const std::size_t index = mem_props.memoryTypes[memory_type].heapIndex;
  return index;
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  \param [in,out] user_data No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryAllocation(
    [[maybe_unused]] VmaAllocator vm_allocator,
    uint32b memory_type,
    [[maybe_unused]] VkDeviceMemory memory,
    VkDeviceSize size,
    void* user_data)
{
  auto device = zisc::cast<VulkanDevice*>(user_data);
  const std::size_t heap_index = getHeapIndex(*device, memory_type);
  (*device->heap_usage_list_)[heap_index].add(size);

  VulkanSubPlatform& sub_platform = device->parentImpl();
  const VulkanDeviceInfo& info = device->deviceInfoImpl();
  const std::size_t device_index = info.deviceIndex();
  sub_platform.notifyOfDeviceMemoryAllocation(device_index, heap_index, size);
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  \param [in,out] user_data No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryDeallocation(
    [[maybe_unused]] VmaAllocator vm_allocator,
    uint32b memory_type,
    [[maybe_unused]] VkDeviceMemory memory,
    VkDeviceSize size,
    void* user_data)
{
  auto device = zisc::cast<VulkanDevice*>(user_data);
  const std::size_t heap_index = getHeapIndex(*device, memory_type);
  (*device->heap_usage_list_)[heap_index].release(size);

  VulkanSubPlatform& sub_platform = device->parentImpl();
  const VulkanDeviceInfo& info = device->deviceInfoImpl();
  const std::size_t device_index = info.deviceIndex();
  sub_platform.notifyOfDeviceMemoryDeallocation(device_index, heap_index, size);
}

/*!
  \details No detailed description

  \param [in] id No description.
  \param [in] spirv_code No description.
  */
auto VulkanDevice::addShaderModule(const uint32b id,
                                   const zisc::pmr::vector<uint32b>& spirv_code,
                                   const std::string_view module_name)
    -> const ModuleData&
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();
  zivcvk::AllocationCallbacks alloc{makeAllocator()};

  const std::size_t code_size = spirv_code.size() * sizeof(spirv_code[0]);
  zivcvk::ShaderModuleCreateInfo create_info{zivcvk::ShaderModuleCreateFlags{},
                                             code_size,
                                             spirv_code.data()};
  auto module = d.createShaderModule(create_info, alloc, *loader);

  const ModuleData* data = nullptr;
  {
    zisc::pmr::polymorphic_allocator<ModuleData> data_alloc{mem_resource};
    auto module_data = zisc::pmr::allocateUnique(data_alloc);
    data = module_data.get();
    module_data->name_ = module_name;
    module_data->module_ = zisc::cast<VkShaderModule>(module);

    std::unique_lock<std::shared_mutex> lock{shader_mutex_};
    module_data_list_->emplace(id, std::move(module_data));
  }
  updateShaderModuleDebugInfo(*data);
  return *data;
}

/*!
  \details No detailed description

  \param [in,out] kernel No description.
  */
void VulkanDevice::destroyShaderKernel(KernelData* kernel) noexcept
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{makeAllocator()};

  {
    zivcvk::Pipeline pline{kernel->pipeline_};
    if (pline)
      d.destroyPipeline(pline, alloc, *loader);
    kernel->pipeline_ = ZIVC_VK_NULL_HANDLE;
  }
  {
    zivcvk::PipelineLayout pline_layout{kernel->pipeline_layout_};
    if (pline_layout)
      d.destroyPipelineLayout(pline_layout, alloc, *loader);
    kernel->pipeline_layout_ = ZIVC_VK_NULL_HANDLE;
  }
  {
    zivcvk::DescriptorSetLayout desc_set_layout{kernel->desc_set_layout_};
    if (desc_set_layout)
      d.destroyDescriptorSetLayout(desc_set_layout, alloc, *loader);
    kernel->desc_set_layout_ = ZIVC_VK_NULL_HANDLE;
  }
}

/*!
  \details No detailed description

  \param [in,out] module No description.
  */
void VulkanDevice::destroyShaderModule(ModuleData* module) noexcept
{
  zivcvk::Device d{device()};
  zivcvk::AllocationCallbacks alloc{makeAllocator()};
  const auto loader = dispatcher().loaderImpl();

  {
    zivcvk::ShaderModule m{module->module_};
    if (m)
      d.destroyShaderModule(m, alloc, *loader);
    module->module_ = ZIVC_VK_NULL_HANDLE;
  }
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
      if ((*num_of_queues <= p.properties1_.queueCount) &&
          has_flags(p.properties1_, graphics_excluded, sparse_excluded)) {
        *index = zisc::cast<uint32b>(i);
        *num_of_queues = p.properties1_.queueCount;
      }
    }
  };

  const auto& info = deviceInfoImpl();
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
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{makeAllocator()};

  const zivcvk::CommandPoolCreateInfo create_info{
      zivcvk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      queueFamilyIndex()};

  //! \todo Fix me. AMD gpu won't work with custom allocator
  auto command_pool = d.createCommandPool(create_info, nullptr /* alloc */, *loader);
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

  extensions = {
#if defined(Z_MAC)
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME,
                VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
#endif // Z_MAC
                VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
                //VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME,
  };
  if (sub_platform.isDebugMode()) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
  }

  const auto& info = deviceInfoImpl();

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
    const uint32b num_of_queues = queue_family_list[index].properties1_.queueCount;
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
  queue_list_->resize(numOfQueues(), ZIVC_VK_NULL_HANDLE);
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
  const auto& info = deviceInfoImpl();
  const uint32b group_size = info.workGroupSize();

  for (uint32b dim = 1; dim <= work_group_size_list_.size(); ++dim) {
    std::array<uint32b, 3> work_group_size{{1, 1, 1}};
    const auto product = [](const std::array<uint32b, 3>& s) noexcept
    {
      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
    };
    for (uint32b i = 0; product(work_group_size) < group_size; i = (i + 1) % dim)
      work_group_size[i] *= 2;
    [[maybe_unused]] const uint32b s = product(work_group_size);
    ZISC_ASSERT(s == group_size,
                "The work-group size should be power of 2: group size = ", s);
    work_group_size_list_[dim - 1] = work_group_size;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initQueueFamilyIndexList()
{
  //! \todo Support queue family option
  queue_family_index_ = findQueueFamily();
  if (queue_family_index_ == invalidQueueIndex()) {
    const char* message = "Appropriate device queue not found.";
    throw SystemError{ErrorCode::kVulkanInitializationFailed, message};
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initMemoryAllocator()
{
  auto& sub_platform = parentImpl();
  const auto& info = deviceInfoImpl();

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
  create_info.vulkanApiVersion = vkGetVulkanApiVersion();
  create_info.pTypeExternalMemoryHandleTypes = nullptr;
  VkResult result = vmaCreateAllocator(std::addressof(create_info),
                                       std::addressof(vm_allocator_));
  if (result != VK_SUCCESS) {
    const char* message = "VM allocator creation failed.";
    zivcvk::throwResultException(zisc::cast<zivcvk::Result>(result), message);
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
  notifier.pfnFree = Callbacks::notifyOfDeviceMemoryDeallocation;
  notifier.pUserData = this;
  return notifier;
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
void VulkanDevice::updateFenceDebugInfo(const std::size_t index)
{
  const zivcvk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const VkFence handle = (*fence_list_)[index];
    const zivcvk::Fence f{handle};
    if (f) {
      IdData::NameType obj_name{""};
      std::strcat(obj_name.data(), id_data.name().data());
      std::strcat(obj_name.data(), "_fence");
      IdData::NameType idx{""};
      auto [end, result] = std::to_chars(idx.data(),
                                         idx.data() + idx.size(),
                                         index);
      *end = '\0';
      std::strcat(obj_name.data(), idx.data());
      const std::string_view name = obj_name.data();
      setDebugInfo(zisc::cast<VkObjectType>(f.objectType), handle, name, this);
    }
  }
}

/*!
  \details No detailed description

  \param [in,out] kernel No description.
  */
void VulkanDevice::updateKernelDataDebugInfo(const KernelData& kernel)
{
  const zivcvk::Device d{device()};
  if (isDebugMode() && d) {
    auto set_debug_info = [this, &kernel](const auto& handle,
                                          const auto& obj,
                                          const std::string_view suffix)
    {
      const ModuleData& module = *kernel.module_;
      if (obj) {
        IdData::NameType kernel_name{""};
        std::strcat(kernel_name.data(), module.name_.data());
        std::strcat(kernel_name.data(), "_");
        std::strcat(kernel_name.data(), kernel.kernel_name_.data());
        std::strcat(kernel_name.data(), suffix.data());
        const std::string_view name = kernel_name.data();
        setDebugInfo(zisc::cast<VkObjectType>(obj.objectType), handle, name, this);
      }
    };
    {
      const auto handle = kernel.desc_set_layout_;
      const zivcvk::DescriptorSetLayout l{handle};
      set_debug_info(handle, l, "_descsetlayout");
    }
    {
      const auto handle = kernel.pipeline_layout_;
      const zivcvk::PipelineLayout p{handle};
      set_debug_info(handle, p, "_pipelinelayout");
    }
    {
      const auto handle = kernel.pipeline_;
      const zivcvk::Pipeline p{handle};
      set_debug_info(handle, p, "_pipeline");
    }
  }
}

/*!
  \details No detailed description

  \param [in,out] module No description.
  */
void VulkanDevice::updateShaderModuleDebugInfo(const ModuleData& module)
{
  const zivcvk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const VkShaderModule handle = module.module_;
    const zivcvk::ShaderModule m{handle};
    if (m) {
      IdData::NameType module_name{""};
      std::strcat(module_name.data(), id_data.name().data());
      std::strcat(module_name.data(), "_");
      std::strcat(module_name.data(), module.name_.data());
      const std::string_view name = module_name.data();
      setDebugInfo(zisc::cast<VkObjectType>(m.objectType), handle, name, this);
    }
  }
}

} // namespace zivc
