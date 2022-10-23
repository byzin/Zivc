/*!
  \file vulkan_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device.hpp"
// Standard C++ library
#include <array>
#include <algorithm>
#include <charconv>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
#include "zisc/hash/fnv_1a_hash_engine.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_backend.hpp"
#include "vulkan_device_info.hpp"
#include "internal/cmd_debug_label_region.hpp"
#include "internal/cmd_record_region.hpp"
#include "internal/queue_debug_label_region.hpp"
#include "internal/vulkan_memory_allocator.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/error.hpp"
#include "zivc/auxiliary/fence.hpp"
#include "zivc/auxiliary/id_data.hpp"

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
    zivc::vk::PhysicalDevice16BitStorageFeatures b16bit_storage_;
    zivc::vk::PhysicalDevice8BitStorageFeatures b8bit_storage_;
    zivc::vk::PhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_;
    zivc::vk::PhysicalDeviceMaintenance4FeaturesKHR maintenance4_;
    zivc::vk::PhysicalDeviceRayQueryFeaturesKHR ray_query_;
    zivc::vk::PhysicalDeviceRayTracingMaintenance1FeaturesKHR ray_tracing_maintenance1_;
    zivc::vk::PhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_;
    zivc::vk::PhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    zivc::vk::PhysicalDeviceShaderAtomicFloat2FeaturesEXT shader_atomic_float2_;
    zivc::vk::PhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    zivc::vk::PhysicalDeviceShaderClockFeaturesKHR shader_clock_;
    zivc::vk::PhysicalDeviceShaderFloat16Int8Features shader_float16_int8_;
    zivc::vk::PhysicalDeviceShaderIntegerDotProductFeatures shader_integer_dot_product_;
    zivc::vk::PhysicalDeviceVariablePointersFeatures variable_pointers_;
    zivc::vk::PhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_model_;
  };

  const zivc::VulkanDeviceInfo::Features& inputs = info.features();

  zivc::vk::PhysicalDeviceFeatures2 features;
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
  f->maintenance4_ = inputs.maintenance4_;
  f->ray_query_ = inputs.ray_query_;
  f->ray_tracing_maintenance1_ = inputs.ray_tracing_maintenance1_;
  f->ray_tracing_pipeline_ = inputs.ray_tracing_pipeline_features_;
  f->shader_atomic_float_ = inputs.shader_atomic_float_;
  f->shader_atomic_float2_ = inputs.shader_atomic_float2_;
  f->shader_atomic_int64_ = inputs.shader_atomic_int64_;
  f->shader_clock_ = inputs.shader_clock_;
  f->shader_float16_int8_ = inputs.shader_float16_int8_;
  f->shader_integer_dot_product_ = inputs.shader_integer_dot_product_;
  f->variable_pointers_ = inputs.variable_pointers_;
  f->vulkan_memory_model_ = inputs.vulkan_memory_model_;

  zivc::VulkanDeviceInfo::link(features,
                               f->b16bit_storage_,
                               f->b8bit_storage_,
//                               f->acceleration_structure_,
//                               f->maintenance4_,
//                               f->ray_query_,
//                               f->ray_tracing_maintenance1_,
//                               f->ray_tracing_pipeline_,
                               f->shader_atomic_float_,
                               f->shader_atomic_float2_,
                               f->shader_atomic_int64_,
                               f->shader_clock_,
                               f->shader_float16_int8_,
                               f->shader_integer_dot_product_,
                               f->variable_pointers_,
                               f->vulkan_memory_model_);

  return std::make_tuple(features, std::move(f));
}

} // namespace

namespace zivc {

static_assert(std::alignment_of_v<Fence::DataT> % std::alignment_of_v<vk::Fence> == 0U);
static_assert(sizeof(vk::Fence) <= sizeof(Fence::DataT));

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
  const uint64b id = getKernelId(module.name_, kernel_name);
  if (hasShaderKernel(id))
    return getShaderKernel(id);

  const vk::Device d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  zisc::pmr::memory_resource* mem_resource = memoryResource();
  const vk::AllocationCallbacks alloc{createAllocator()};

  // Initialize descriptor set layout
  vk::DescriptorSetLayout desc_set_layout;
  {
    using BindingList = zisc::pmr::vector<vk::DescriptorSetLayoutBinding>;
    const BindingList::allocator_type bindings_alloc{mem_resource};
    BindingList layout_bindings{bindings_alloc};
    layout_bindings.resize(num_of_storage_buffers + num_of_uniform_buffers);
    // Storage buffers
    for (std::size_t index = 0; index < num_of_storage_buffers; ++index) {
      layout_bindings[index] = vk::DescriptorSetLayoutBinding{
          zisc::cast<uint32b>(index),
          vk::DescriptorType::eStorageBuffer,
          1,
          vk::ShaderStageFlagBits::eCompute};
    }
    // Uniform buffer
    for (std::size_t i = 0; i < num_of_uniform_buffers; ++i) {
      const std::size_t index = num_of_storage_buffers + i;
      layout_bindings[index] = vk::DescriptorSetLayoutBinding{
          zisc::cast<uint32b>(index),
          vk::DescriptorType::eUniformBuffer,
          1,
          vk::ShaderStageFlagBits::eCompute};
    }
    const vk::DescriptorSetLayoutCreateInfo create_info{
        vk::DescriptorSetLayoutCreateFlags{},
        zisc::cast<uint32b>(layout_bindings.size()),
        layout_bindings.data()};
    desc_set_layout = d.createDescriptorSetLayout(create_info, alloc, loader);
  }

  // Pipeline
  vk::PipelineLayout pline_layout;
  {
    // For clspv module scope push constants
    const vk::PushConstantRange push_constant_range{
        vk::ShaderStageFlagBits::eCompute, 0, 92};

    const vk::PipelineLayoutCreateInfo create_info{
        vk::PipelineLayoutCreateFlags{},
        1,
        std::addressof(desc_set_layout),
        1,
        std::addressof(push_constant_range)};
    pline_layout = d.createPipelineLayout(create_info, alloc, loader);
  }
  // Specialization constants
  const std::span work_group_size = workGroupSizeDim(work_dimension);
  const zisc::pmr::vector<uint32b>::allocator_type spec_alloc{mem_resource};
  zisc::pmr::vector<uint32b> spec_constants{spec_alloc};
  {
    spec_constants.reserve(work_group_size.size() + 2);
    // For clspv work group size
    for (const uint32b s : work_group_size)
      spec_constants.emplace_back(s);
    // Work dimension
    spec_constants.emplace_back(zisc::cast<uint32b>(work_dimension));
    // For clspv local element size
    const VulkanDeviceInfo& info = deviceInfoImpl();
    spec_constants.emplace_back(info.workGroupSize());
  }
  using MapEntryList = zisc::pmr::vector<vk::SpecializationMapEntry>;
  const MapEntryList::allocator_type entry_alloc{mem_resource};
  MapEntryList entries{entry_alloc};
  {
    entries.reserve(work_group_size.size() + 1 + num_of_local_args);
    // For clspv work group size
    for (std::size_t i = 0; i < work_group_size.size(); ++i) {
      const vk::SpecializationMapEntry entry{
          zisc::cast<uint32b>(i),
          zisc::cast<uint32b>(i * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // Work dimension
    {
      const auto work_dim_index = zisc::cast<uint32b>(entries.size());
      const vk::SpecializationMapEntry entry{
          work_dim_index,
          zisc::cast<uint32b>(work_dim_index * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
    // For clspv local element size
    for (std::size_t i = 0; i < num_of_local_args; ++i) {
      const auto local_size_index = zisc::cast<uint32b>(entries.size());
      const vk::SpecializationMapEntry entry{
          local_size_index,
          zisc::cast<uint32b>(local_size_index * sizeof(uint32b)),
          sizeof(uint32b)};
      entries.emplace_back(entry);
    }
  }
  const vk::SpecializationInfo spec_info{zisc::cast<uint32b>(entries.size()),
                                             entries.data(),
                                             spec_constants.size() * sizeof(uint32b),
                                             spec_constants.data()};
  // Compute pipeline
  vk::Pipeline pline;
  {
    const vk::PipelineShaderStageCreateInfo stage_info{
        vk::PipelineShaderStageCreateFlags{},
        vk::ShaderStageFlagBits::eCompute,
        vk::ShaderModule{module.module_},
        kernel_name.data(),
        std::addressof(spec_info)};
    vk::PipelineCreateFlags pipeline_flags;
    if (isDebugMode()) {
      pipeline_flags |= vk::PipelineCreateFlagBits::eCaptureStatisticsKHR;
    }
    const vk::ComputePipelineCreateInfo pipeline_info{pipeline_flags,
                                                          stage_info,
                                                          pline_layout};
    const vk::ResultValue result = d.createComputePipeline(vk::PipelineCache{},
                                                           pipeline_info,
                                                           alloc,
                                                           loader);
    if (result.result != vk::Result::eSuccess) {
      const std::string message = createErrorMessage(
          *this,
          "Compute pipeline creation failed.");
      vk::throwResultException(result.result, message.data());
    }
    pline = zisc::cast<vk::Pipeline>(result.value);
  }

  const KernelData* data = nullptr;
  {
    auto kernel_data = zisc::pmr::allocateUnique<KernelData>(mem_resource);
    data = kernel_data.get();
    kernel_data->module_ = std::addressof(module);
    copyStr(kernel_name, kernel_data->kernel_name_.data());
    kernel_data->desc_set_layout_ =
        zisc::cast<VkDescriptorSetLayout>(desc_set_layout);
    kernel_data->pipeline_layout_ = zisc::cast<VkPipelineLayout>(pline_layout);
    kernel_data->pipeline_ = zisc::cast<VkPipeline>(pline);

    [[maybe_unused]] const std::optional<std::size_t> result = kernelDataList().add(id, std::move(kernel_data));
    //! \todo Raise an exception
    ZIVC_ASSERT(result.has_value(), "The kernel list overflowed.");
  }
  updateKernelDataDebugInfo(*data);
  return *data;
}

/*!
  \details No detailed description

  \return No description
  */
VmaDeviceMemoryCallbacks VulkanDevice::createAllocationNotifier() noexcept
{
  VmaDeviceMemoryCallbacks notifier;
  notifier.pfnAllocate = Callbacks::notifyOfDeviceMemoryAllocation;
  notifier.pfnFree = Callbacks::notifyOfDeviceMemoryDeallocation;
  notifier.pUserData = this;
  return notifier;
}

/*!
  \details No detailed description
  */
VkCommandBuffer VulkanDevice::createCommandBuffer()
{
  const vk::Device d{device()};
  [[maybe_unused]] zisc::pmr::memory_resource* mem_resource = memoryResource();

  const vk::CommandBufferAllocateInfo alloc_info{
      commandPool(),
      vk::CommandBufferLevel::ePrimary,
      1};
  //! \todo Resolve the error
  // The create function causes undefined symbol error with custom allocator
  //const zisc::pmr::vector<vk::CommandBuffer>::allocator_type alloc{mem_resource};
  //auto commands = d.allocateCommandBuffers(alloc_info, alloc, dispatcher().loader());
  std::vector commands = d.allocateCommandBuffers(alloc_info, dispatcher().loader());
  ZIVC_ASSERT(commands.size() == 1, "The size of command buffers isn't 1.");
  return zisc::cast<VkCommandBuffer>(commands[0]);
}

/*!
  \details No detailed description

  \return No description
  */
VkAllocationCallbacks VulkanDevice::createAllocator() noexcept
{
  VulkanBackend& backend_p = parentImpl();
  return backend_p.createAllocator();
}

/*!
  \details No detailed description

  \param [out] size No description.
  \return No description
  */
auto VulkanDevice::getQueueFamilyIndexList(uint32b* size) const noexcept
    -> std::array<uint32b, kNumOfCapabilities>
{
  std::array<uint32b, kNumOfCapabilities> index_list{};
  index_list.fill(invalidQueueIndex());
  uint32b s = 0;
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    if (!hasCapability(cap))
      continue;
    const uint32b family_index = queueFamilyIndex(cap);
    const auto ite = std::find(index_list.begin(), index_list.end(), family_index);
    if (ite == index_list.end())
      index_list[s++] = family_index;
  }
  if (size != nullptr)
    *size = s;
  return index_list;
}

/*!
  \details No detailed description

  \param [in] module_name No description.
  \param [in] kernel_name No description.
  \return No description
  */
uint64b VulkanDevice::getKernelId(const std::string_view module_name,
                                  const std::string_view kernel_name) noexcept
{
  IdData::NameT kernel_id_name{""};
  copyStr(module_name, kernel_id_name.data());
  concatStr(kernel_name, kernel_id_name.data());
  const uint64b kernel_id = zisc::Fnv1aHash64::hash(kernel_id_name.data());
  return kernel_id;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] label_name No description.
  \param [in] color No description.
  \return No description
  */
internal::CmdDebugLabelRegion VulkanDevice::makeCmdDebugLabel(
    const VkCommandBuffer& command_buffer,
    const std::string_view label_name,
    const std::span<const float, 4> color) const noexcept
{
  const VkCommandBuffer& cmd = isDebugMode() ? command_buffer : ZIVC_VK_NULL_HANDLE;
  return internal::CmdDebugLabelRegion{cmd, dispatcher(), label_name, color};
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] flags No description.
  \return No description
  */
internal::CmdRecordRegion VulkanDevice::makeCmdRecord(
    const VkCommandBuffer& command_buffer,
    const VkCommandBufferUsageFlags flags) const
{
  return internal::CmdRecordRegion{command_buffer, dispatcher(), flags};
}

/*!
  \details No detailed description

  \param [in] q No description.
  \param [in] label_name No description.
  \param [in] color No description.
  \return No description
  */
internal::QueueDebugLabelRegion VulkanDevice::makeQueueDebugLabel(
    const VkQueue& q,
    const std::string_view label_name,
    const std::span<const float, 4> color) const noexcept
{
  const VkQueue& que = isDebugMode() ? q : ZIVC_VK_NULL_HANDLE;
  return internal::QueueDebugLabelRegion{que, dispatcher(), label_name, color};
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
  const zisc::Memory::Usage& usage = (*heap_usage_list_)[heap_index];
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
  const std::size_t n = numOfQueues(CapabilityT::kCompute);
  return n;
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void VulkanDevice::returnFence(Fence* fence) noexcept
{
  auto* dest = zisc::reinterp<vk::Fence*>(std::addressof(fence->data()));

  constexpr std::size_t invalid = (std::numeric_limits<std::size_t>::max)();
  std::size_t fence_index = invalid;
  {
    const zisc::pmr::vector<VkFence>& fence_list = *fence_list_;
    const auto* const value = zisc::cast<VkFence>(*dest);
    const auto ite = std::lower_bound(fence_list.begin(), fence_list.end(), value);
    if ((ite != fence_list.end()) && (*ite == value))
      fence_index = zisc::cast<std::size_t>(std::distance(fence_list.begin(), ite));
  }

  if (fence_index != invalid) {
    IndexQueueT& index_queue = fenceIndexQueue();
    [[maybe_unused]] const std::optional result = index_queue.enqueue(fence_index);
    ZIVC_ASSERT(result.has_value(), "Queueing fence index failed.");
    *dest = vk::Fence{};
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
                          const Fence& fence) const
{
  const vk::CommandBuffer command{command_buffer};
  const vk::Queue que{q};
  const vk::Fence fen = fence.isActive()
      ? *zisc::reinterp<const vk::Fence*>(std::addressof(fence.data()))
      : vk::Fence{};
  vk::SubmitInfo info{};
  info.setCommandBufferCount(1);
  info.setPCommandBuffers(std::addressof(command));
  que.submit(info, fen, dispatcher().loader());
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
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  const vk::Device d{device()};
  const auto object_type = static_cast<vk::ObjectType>(vk_object_type);
  const auto handle = zisc::cast<uint64b>(zisc::reinterp<std::size_t>(vk_handle));
  // Name
  {
    const vk::DebugUtilsObjectNameInfoEXT name_info{object_type,
                                                        handle,
                                                        object_name.data()};
    d.setDebugUtilsObjectNameEXT(name_info, loader);
  }
  // Tag
  if (zivc_object != nullptr) {
    const auto tag_name = zisc::cast<uint64b>(zivc_object->id().id());
    const vk::DebugUtilsObjectTagInfoEXT tag_info{object_type,
                                                      handle,
                                                      tag_name,
                                                      sizeof(void*),
                                                      std::addressof(zivc_object)};
    d.setDebugUtilsObjectTagEXT(tag_info, loader);
  }
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void VulkanDevice::setFenceSize(const std::size_t s)
{
  const vk::Device d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  const vk::AllocationCallbacks alloc{createAllocator()};

  IndexQueueT& index_queue = fenceIndexQueue();
  index_queue.setCapacity(s);
  index_queue.clear();
  const std::size_t fence_size = (0 < s) ? index_queue.capacity() : 0;
  for (std::size_t index = 0; index < fence_size; ++index) {
    [[maybe_unused]] const std::optional result = index_queue.enqueue(index);
    ZIVC_ASSERT(result.has_value(), "Enqueing fence index failed: ", index);
  }

  zisc::pmr::vector<VkFence>& fence_list = *fence_list_;
  // Add new fences
  for (std::size_t index = fence_list.size(); index < fence_size; ++index) {
    const vk::FenceCreateInfo info{};
    const vk::Fence fence = d.createFence(info, alloc, loader);
    fence_list.emplace_back(zisc::cast<VkFence>(fence));
    updateFenceDebugInfo(index);
  }
  // Remove fences
  for (std::size_t i = fence_list.size(); fence_size < i; --i) {
    auto fence = zisc::cast<vk::Fence>(fence_list.back());
    d.destroyFence(fence, alloc, loader);
    fence_list.pop_back();
  }
  // Reset fences
  for (VkFence& fence : fence_list) {
    auto f = zisc::cast<vk::Fence>(fence);
    d.resetFences(f, loader);
  }
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void VulkanDevice::takeFence(Fence* fence)
{
  IndexQueueT& index_queue = fenceIndexQueue();
  const std::optional index_result = index_queue.dequeue();
  if (not index_result.has_value()) [[unlikely]] {
    //! \todo Available fence isn't found. Raise an exception?
    const std::string message = createErrorMessage(
        *this,
        "Available fence not found.");
    throw SystemError{ErrorCode::kAvailableFenceNotFound, message};
  }

  void* memory = std::addressof(fence->data());
  auto* dest = ::new (memory) vk::Fence{};
  {
    const vk::Device d{device()};
    const std::size_t fence_index = *index_result;
    auto f = zisc::cast<vk::Fence>((*fence_list_)[fence_index]);
    d.resetFences(f, dispatcher().loader());
    *dest = f;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::waitForCompletion() const
{
  const vk::Device d{device()};
  d.waitIdle(dispatcher().loader());
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
void VulkanDevice::waitForCompletion(const uint32b queue_index) const
{
  waitForCompletion(CapabilityT::kCompute, queue_index);
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \param [in] queue_index No description.
  */
void VulkanDevice::waitForCompletion(const CapabilityT cap, 
                                     const uint32b queue_index) const
{
  ZIVC_ASSERT(hasCapability(cap), "Unsupported capability is specified in wait.");
  const auto q = zisc::cast<vk::Queue>(getQueue(cap, queue_index));
  q.waitIdle(dispatcher().loader());
}

/*!
  \details No detailed description

  \param [in] fence No description.
  */
void VulkanDevice::waitForCompletion(const Fence& fence) const
{
  if (fence) {
    const vk::Device d{device()};
    const auto* f = zisc::reinterp<const vk::Fence*>(&fence.data());
    constexpr uint64b timeout = (std::numeric_limits<uint64b>::max)();
    [[maybe_unused]] const vk::Result result = d.waitForFences(*f,
                                                               VK_TRUE,
                                                               timeout,
                                                               dispatcher().loader());
    ZIVC_ASSERT(result == vk::Result::eSuccess, "Waiting for a fence failed.");
    const_cast<Fence*>(std::addressof(fence))->clear();
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::destroyData() noexcept
{
  const vk::Device d{device()};
  if (d)
    waitForCompletion();

  // Queue info
  queue_family_index_list_.fill(invalidQueueIndex());
  queue_count_list_.fill(0);
  queue_offset_list_.fill((std::numeric_limits<uint32b>::max)());

  if (vm_allocator_ != nullptr) {
    vmaDestroyAllocator(vm_allocator_);
    vm_allocator_ = ZIVC_VK_NULL_HANDLE;
  }

  if (d) {
    const vk::AllocationCallbacks alloc{createAllocator()};
    VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();

    setFenceSize(0); // Destroy all fences

    // Kernel data
    for (auto& kernel : kernelDataList())
      destroyShaderKernel(kernel.second.get());

    // Shader modules
    for (auto& module : moduleDataList())
      destroyShaderModule(module.second.get());

    // Command pool
    vk::CommandPool command_pool{command_pool_};
    if (command_pool) {
      //! \todo Fix me. AMD gpu won't work with custom allocator
      d.destroyCommandPool(command_pool, /* alloc */ nullptr, loader);
      command_pool = nullptr;
    }

    d.destroy(alloc, loader);
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
  zisc::pmr::memory_resource* mem_resource = memoryResource();
  {
    const zisc::pmr::polymorphic_allocator<IndexQueueImplT> alloc{mem_resource};
    fence_index_queue_ = zisc::pmr::allocateUnique(alloc, mem_resource);
  }
  {
    using FenceListT = decltype(fence_list_)::element_type;
    const FenceListT::allocator_type alloc{mem_resource};
    fence_list_ = zisc::pmr::allocateUnique<FenceListT>(mem_resource,
                                                        FenceListT{mem_resource});
  }
  {
    using QueueListT = decltype(queue_list_)::element_type;
    const QueueListT::allocator_type alloc{mem_resource};
    queue_list_ = zisc::pmr::allocateUnique<QueueListT>(mem_resource,
                                                        QueueListT{alloc});
  }
  {
    using UsageListT = decltype(heap_usage_list_)::element_type;
    const UsageListT::allocator_type alloc{mem_resource};
    heap_usage_list_ = zisc::pmr::allocateUnique<UsageListT>(mem_resource,
                                                             UsageListT{alloc});
    const VulkanDeviceInfo& info = deviceInfoImpl();
    heap_usage_list_->resize(info.heapInfoList().size());
  }
  {
    using ShaderModuleListT = decltype(module_data_list_)::element_type;
    const std::size_t cap = parentImpl().capacityForModulesPerDevice();
    module_data_list_ = zisc::pmr::allocateUnique<ShaderModuleListT>(mem_resource,
                                                                     cap,
                                                                     mem_resource);
  }
  {
    using KernelDataListT = decltype(kernel_data_list_)::element_type;
    const std::size_t cap = parentImpl().capacityForKernelsPerDevice();
    kernel_data_list_ = zisc::pmr::allocateUnique<KernelDataListT>(mem_resource,
                                                                   cap,
                                                                   mem_resource);
  }

  initCapability();
  initDispatcher();
  initWorkGroupSizeDim();
  initQueueFamilyIndexList();
  initDevice();
  initQueueList();
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
    const vk::Device d{handle};
    const std::string_view name = id_data.name();
    if (d) {
      setDebugInfo(static_cast<VkObjectType>(d.objectType), handle, name, this);
    }
  }
  // Command pool
  {
    const VkCommandPool handle = commandPool();
    const vk::CommandPool p{handle};
    if (p) {
      IdData::NameT obj_name{""};
      copyStr(id_data.name(), obj_name.data());
      concatStr("_pool", obj_name.data());
      const std::string_view name = obj_name.data();
      setDebugInfo(static_cast<VkObjectType>(p.objectType), handle, name, this);
    }
  }
  // Queue
  const std::array<std::string_view, numOfCapabilities()> cap_name_list{{"_compute",
                                                                         "_gui"}};
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    if (!hasCapability(cap))
      continue;
    for (std::size_t q_index = 0; q_index < numOfQueues(cap); ++q_index) {
      const VkQueue handle = getQueue(cap, q_index);
      const vk::Queue q{handle};
      if (q) {
        IdData::NameT obj_name{""};
        copyStr(id_data.name(), obj_name.data());
        concatStr(cap_name_list[i], obj_name.data());
        concatStr("_queue", obj_name.data());
        IdData::NameT idx{""};
        auto [end, result] = std::to_chars(idx.data(),
                                           idx.data() + idx.size(),
                                           q_index);
        *end = '\0';
        concatStr(idx.data(), obj_name.data());
        const std::string_view name = obj_name.data();
        setDebugInfo(static_cast<VkObjectType>(q.objectType), handle, name, this);
      }
    }
  }
  // Fence
  for (std::size_t i = 0; i < fence_list_->size(); ++i)
    updateFenceDebugInfo(i);
  // Shader modules
  for (const auto& module : moduleDataList())
    updateShaderModuleDebugInfo(*module.second);
  // Kernel data
  for (const auto& kernel : kernelDataList())
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
  const VulkanDeviceInfo& info = device.deviceInfoImpl();
  const VkPhysicalDeviceMemoryProperties& mem_props = info.memoryProperties().properties1_;
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
  auto* device = static_cast<VulkanDevice*>(user_data);
  const std::size_t heap_index = getHeapIndex(*device, memory_type);
  (*device->heap_usage_list_)[heap_index].add(size);

  VulkanBackend& backend_p = device->parentImpl();
  const VulkanDeviceInfo& info = device->deviceInfoImpl();
  const std::size_t device_index = info.deviceIndex();
  backend_p.notifyOfDeviceMemoryAllocation(device_index, heap_index, size);
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
  auto* device = static_cast<VulkanDevice*>(user_data);
  const std::size_t heap_index = getHeapIndex(*device, memory_type);
  (*device->heap_usage_list_)[heap_index].release(size);

  VulkanBackend& backend_p = device->parentImpl();
  const VulkanDeviceInfo& info = device->deviceInfoImpl();
  const std::size_t device_index = info.deviceIndex();
  backend_p.notifyOfDeviceMemoryDeallocation(device_index, heap_index, size);
}

/*!
  \details No detailed description

  \param [in] id No description.
  \param [in] spirv_code No description.
  */
auto VulkanDevice::addShaderModule(const uint64b id,
                                   const std::span<const uint32b> spirv_code,
                                   const std::string_view module_name)
    -> const ModuleData&
{
  const vk::Device d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  zisc::pmr::memory_resource* mem_resource = memoryResource();
  const vk::AllocationCallbacks alloc{createAllocator()};

  using SpirVCodeT = decltype(spirv_code);
  const std::size_t code_size = spirv_code.size() * sizeof(SpirVCodeT::value_type);
  const vk::ShaderModuleCreateInfo create_info{vk::ShaderModuleCreateFlags{},
                                                   code_size,
                                                   spirv_code.data()};
  const vk::ShaderModule module = d.createShaderModule(create_info, alloc, loader);

  const ModuleData* data = nullptr;
  {
    auto module_data = zisc::pmr::allocateUnique<ModuleData>(mem_resource);
    data = module_data.get();
    module_data->name_ = module_name;
    module_data->module_ = zisc::cast<VkShaderModule>(module);

    [[maybe_unused]] const std::optional<std::size_t> result = moduleDataList().add(id, std::move(module_data));
    //! \todo Raise an exception
    ZIVC_ASSERT(result.has_value(), "The module list overflowed.");
  }
  updateShaderModuleDebugInfo(*data);
  return *data;
}

/*!
  \details No detailed description

  \return No description
  */
bool VulkanDevice::checkQueueFamilyFlags(const VkQueueFamilyProperties& props,
                                         const bool graphics_required,
                                         const bool compute_required,
                                         const bool graphics_excluded,
                                         const bool compute_excluded) noexcept
{
  const auto* p = zisc::reinterp<const vk::QueueFamilyProperties*>(&props);
  const auto has_flag = [p](const vk::QueueFlagBits flag) noexcept
  {
    const bool result = (p->queueFlags & flag) == flag;
    return result;
  };

  using QFlagBits = vk::QueueFlagBits;
  const bool result = has_flag(QFlagBits::eTransfer) &&
                      (!graphics_required || has_flag(QFlagBits::eGraphics)) &&
                      (!compute_required || has_flag(QFlagBits::eCompute)) &&
                      (!graphics_excluded || !has_flag(QFlagBits::eGraphics)) &&
                      (!compute_excluded || !has_flag(QFlagBits::eCompute));
  return result;
}

/*!
  \details No detailed description

  \param [in,out] kernel No description.
  */
void VulkanDevice::destroyShaderKernel(KernelData* kernel) noexcept
{
  const vk::Device d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  const vk::AllocationCallbacks alloc{createAllocator()};

  {
    const vk::Pipeline pline{kernel->pipeline_};
    if (pline)
      d.destroyPipeline(pline, alloc, loader);
    kernel->pipeline_ = ZIVC_VK_NULL_HANDLE;
  }
  {
    const vk::PipelineLayout pline_layout{kernel->pipeline_layout_};
    if (pline_layout)
      d.destroyPipelineLayout(pline_layout, alloc, loader);
    kernel->pipeline_layout_ = ZIVC_VK_NULL_HANDLE;
  }
  {
    const vk::DescriptorSetLayout desc_set_layout{kernel->desc_set_layout_};
    if (desc_set_layout)
      d.destroyDescriptorSetLayout(desc_set_layout, alloc, loader);
    kernel->desc_set_layout_ = ZIVC_VK_NULL_HANDLE;
  }
}

/*!
  \details No detailed description

  \param [in,out] module No description.
  */
void VulkanDevice::destroyShaderModule(ModuleData* module) noexcept
{
  const vk::Device d{device()};
  const vk::AllocationCallbacks alloc{createAllocator()};
  {
    const vk::ShaderModule m{module->module_};
    if (m)
      d.destroyShaderModule(m, alloc, dispatcher().loader());
    module->module_ = ZIVC_VK_NULL_HANDLE;
  }
}

/*!
  \details No detailed description

  \param [in] cap No description.
  \param [out] queue_count No description.
  \return No description
  */
uint32b VulkanDevice::findQueueFamily(const CapabilityT cap,
                                      uint32b* queue_count) const noexcept
{
  const auto find_queue_family = [this](const VulkanDeviceInfo& info,
                                        const bool graphics_required,
                                        const bool compute_required,
                                        const bool graphics_excluded,
                                        const bool compute_excluded,
                                        const bool allow_overlap,
                                        uint32b* index,
                                        uint32b* q_count) noexcept
  {
    const std::span queue_family_list = info.queueFamilyPropertiesList();
    for (uint32b i = 0; i < queue_family_list.size(); ++i) {
      if (!allow_overlap) {
        const std::array index_list = queue_family_index_list_;
        const auto ite = std::find(index_list.begin(), index_list.end(), i);
        if (ite != index_list.end())
          continue;
      }

      const VulkanDeviceInfo::QueueFamilyProperties& qp = queue_family_list[i];
      if (qp.properties1_.queueCount < *q_count)
        continue;

      const auto* p = zisc::cast<const VkQueueFamilyProperties*>(&qp.properties1_);
      if (checkQueueFamilyFlags(*p, graphics_required,
                                    compute_required,
                                    graphics_excluded,
                                    compute_excluded)) {
        *index = i;
        *q_count = qp.properties1_.queueCount;
      }
    }
  };

  const VulkanDeviceInfo& info = deviceInfoImpl();
  uint32b index = invalidQueueIndex();
  uint32b q_count = 0;

  switch (cap) {
   case CapabilityT::kCompute: {
    // It tries to find a queue family that has compute but doesn't have graphics
    // to avoid conflict with graphics use first
    find_queue_family(info, false, true, true, false, false, &index, &q_count);
    if (index == invalidQueueIndex()) {
      // Find a queue family that has compute.
      // It doesn't matter if the queue has graphics
      find_queue_family(info, false, true, false, false, false, &index, &q_count);
    }
    if (index == invalidQueueIndex()) {
      find_queue_family(info, false, true, false, false, true, &index, &q_count);
    }
    break;
   }
   case CapabilityT::kGui: {
    // It tries to find a queue family that has graphics but doesn't have compute
    // to avoid conflict with compute use first
    find_queue_family(info, true, false, false, true, false, &index, &q_count);
    if (index == invalidQueueIndex()) {
      // Find a queue family that has graphics.
      // It doesn't matter if the queue has compute 
      find_queue_family(info, true, false, false, false, false, &index, &q_count);
    }
    if (index == invalidQueueIndex()) {
      find_queue_family(info, true, false, false, false, true, &index, &q_count);
    }
    break;
   }
   default:
    break;
  }

  if (queue_count != nullptr)
    *queue_count = q_count;
  return index;
}

/*!
  \details No detailed description

  \return No description
  */
VmaVulkanFunctions VulkanDevice::getVmaVulkanFunctions() const noexcept
{
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  VmaVulkanFunctions functions;
  functions.vkGetInstanceProcAddr = nullptr;
  functions.vkGetDeviceProcAddr = nullptr;
  functions.vkGetPhysicalDeviceProperties = loader.vkGetPhysicalDeviceProperties;
  functions.vkGetPhysicalDeviceMemoryProperties = loader.vkGetPhysicalDeviceMemoryProperties;
  functions.vkAllocateMemory = loader.vkAllocateMemory;
  functions.vkFreeMemory = loader.vkFreeMemory;
  functions.vkMapMemory = loader.vkMapMemory;
  functions.vkUnmapMemory = loader.vkUnmapMemory;
  functions.vkFlushMappedMemoryRanges = loader.vkFlushMappedMemoryRanges;
  functions.vkInvalidateMappedMemoryRanges = loader.vkInvalidateMappedMemoryRanges;
  functions.vkBindBufferMemory = loader.vkBindBufferMemory;
  functions.vkBindImageMemory = loader.vkBindImageMemory;
  functions.vkGetBufferMemoryRequirements = loader.vkGetBufferMemoryRequirements;
  functions.vkGetImageMemoryRequirements = loader.vkGetImageMemoryRequirements;
  functions.vkCreateBuffer = loader.vkCreateBuffer;
  functions.vkDestroyBuffer = loader.vkDestroyBuffer;
  functions.vkCreateImage = loader.vkCreateImage;
  functions.vkDestroyImage = loader.vkDestroyImage;
  functions.vkCmdCopyBuffer = loader.vkCmdCopyBuffer;

  const auto select_func = [](auto func0, auto func1) noexcept
  {
    return (func0 != nullptr) ? func0 : func1;
  };

  functions.vkGetBufferMemoryRequirements2KHR = select_func(
      loader.vkGetBufferMemoryRequirements2,
      loader.vkGetBufferMemoryRequirements2KHR);
  functions.vkGetImageMemoryRequirements2KHR = select_func(
      loader.vkGetImageMemoryRequirements2,
      loader.vkGetImageMemoryRequirements2KHR);
  functions.vkBindBufferMemory2KHR = select_func(
      loader.vkBindBufferMemory2,
      loader.vkBindBufferMemory2KHR);
  functions.vkBindImageMemory2KHR = select_func(
      loader.vkBindImageMemory2,
      loader.vkBindImageMemory2KHR);
  functions.vkGetPhysicalDeviceMemoryProperties2KHR = select_func(
      loader.vkGetPhysicalDeviceMemoryProperties2,
      loader.vkGetPhysicalDeviceMemoryProperties2KHR);
  functions.vkGetDeviceBufferMemoryRequirements = select_func(
      loader.vkGetDeviceBufferMemoryRequirements,
      loader.vkGetDeviceBufferMemoryRequirementsKHR);
  functions.vkGetDeviceImageMemoryRequirements = select_func(
      loader.vkGetDeviceImageMemoryRequirements,
      loader.vkGetDeviceImageMemoryRequirementsKHR);
  return functions;
}

/*!
  \details No detailed description
  */
void VulkanDevice::initCapability() noexcept
{
  constexpr uint32b compute_mask = 0b1u << static_cast<uint32b>(CapabilityT::kCompute);
  capabilities_ = compute_mask;

  const VulkanBackend& backend_p = parentImpl();
  using SurfaceType = VulkanBackend::WindowSurfaceType;
  if (backend_p.windowSurfaceType() != SurfaceType::kNone) {
    constexpr uint32b gui_mask = 0b1u << static_cast<uint32b>(CapabilityT::kGui);
    capabilities_ = capabilities_ | gui_mask;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initCommandPool()
{
  const vk::Device d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher().loader();
  const vk::AllocationCallbacks alloc{createAllocator()};

  const vk::CommandPoolCreateInfo create_info{
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      queueFamilyIndex(CapabilityT::kCompute)};

  //! \todo Fix me. AMD gpu won't work with custom allocator
  const vk::CommandPool command_pool = d.createCommandPool(create_info, /* alloc */ nullptr, loader);
  command_pool_ = zisc::cast<VkCommandPool>(command_pool);
}

/*!
  \details No detailed description
  */
void VulkanDevice::initDevice()
{
  VulkanBackend& backend_p = parentImpl();
  const VulkanDeviceInfo& info = deviceInfoImpl();

  const zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};
  initProperties(&extensions, &layers);

  // Queue create info
  using PriorityList = zisc::pmr::vector<float>;
  const PriorityList::allocator_type priority_alloc{memoryResource()};
  PriorityList priority_list{priority_alloc};
  using QueueCreateInfoList = zisc::pmr::vector<vk::DeviceQueueCreateInfo>;
  const QueueCreateInfoList::allocator_type q_info_alloc{memoryResource()};
  QueueCreateInfoList q_create_info_list{q_info_alloc};
  initQueueCreateInfoList(
    zisc::reinterp<zisc::pmr::vector<VkDeviceQueueCreateInfo>*>(&q_create_info_list),
    &priority_list);

  // Device features
  auto [device_features, f] = ::getDefaultFeatures(info, memoryResource());

  vk::DeviceCreateInfo device_create_info{
      vk::DeviceCreateFlags{},
      zisc::cast<uint32b>(q_create_info_list.size()),
      q_create_info_list.data(),
      zisc::cast<uint32b>(layers.size()),
      layers.data(),
      zisc::cast<uint32b>(extensions.size()),
      extensions.data(),
      nullptr};
  device_create_info.setPNext(std::addressof(device_features));

  const vk::AllocationCallbacks alloc{backend_p.createAllocator()};
  const auto pdevice = zisc::cast<vk::PhysicalDevice>(info.device());
  const vk::Device d = pdevice.createDevice(device_create_info, alloc, dispatcher().loader());
  device_ = zisc::cast<VkDevice>(d);
  dispatcher_->set(device());
}

/*!
  \details No detailed description
  */
void VulkanDevice::initDispatcher()
{
  const VulkanBackend& backend_p = parentImpl();
  dispatcher_ = zisc::pmr::allocateUnique<VulkanDispatchLoader>(memoryResource(),
                                                                backend_p.dispatcher());
  ZIVC_ASSERT(dispatcher().isDispatchableForInstance(), "Unexpected init.");
  ZIVC_ASSERT(dispatcher().isDispatchableForDevice(), "Unexpected init.");
}

/*!
  \details No detailed description

  \param [out] extensions No description.
  \param [out] layers No description.
  */
void VulkanDevice::initProperties(zisc::pmr::vector<const char*>* extensions,
                                  zisc::pmr::vector<const char*>* layers)
{
  const VulkanBackend& backend_p = parentImpl();

#if defined(Z_MAC)
  *extensions = {VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                 VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                 VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME,
                 VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME};
#endif // Z_MAC

  if (hasCapability(CapabilityT::kCompute)) {
    extensions->emplace_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
                            //VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME,
    // extensions->emplace_back(VK_KHR_MAINTENANCE_4_EXTENSION_NAME);
  }
  if (hasCapability(CapabilityT::kGui)) {
    extensions->emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  }

  if (backend_p.isDebugMode()) {
    layers->emplace_back("VK_LAYER_KHRONOS_validation");
  }

  // Check if properties are supported
  const auto throw_exception = [this](const char* type, const char* name)
  {
    constexpr std::size_t max_message_size = 256;
    std::array<char, max_message_size> m;
    std::sprintf(m.data(),
                 "%s property '%s' isn't supported in the device.",
                 type,
                 name);
    const std::string message = createErrorMessage(*this, m.data());
    throw SystemError{ErrorCode::kVulkanInitializationFailed, message};
  };
  const VulkanDeviceInfo& info = deviceInfoImpl();
  for (const char* prop : *extensions) {
    if (!info.isExtensionSupported(prop))
      throw_exception("Extension", prop);
  }
  for (const char* prop : *layers) {
    if (!info.isLayerSupported(prop))
      throw_exception("Layer", prop);
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initWorkGroupSizeDim() noexcept
{
  const VulkanDeviceInfo& info = deviceInfoImpl();
  const uint32b group_size = info.workGroupSize();

  for (uint32b dim = 1; dim <= work_group_size_list_.size(); ++dim) {
    std::array<uint32b, 3> work_group_size{{1, 1, 1}};
    const auto product = [](const std::array<uint32b, 3>& s) noexcept
    {
      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<>());
    };
    for (uint32b i = 0; product(work_group_size) < group_size; i = (i + 1) % dim)
      work_group_size[i] *= 2;
    [[maybe_unused]] const uint32b s = product(work_group_size);
    ZIVC_ASSERT(s == group_size,
                "The work-group size should be power of 2: group size = ", s);
    work_group_size_list_[dim - 1] = work_group_size;
  }
}

/*!
  \details No detailed description

  \param [out] create_info_list No description.
  \param [out] priority_list No description.
  */
void VulkanDevice::initQueueCreateInfoList(
    zisc::pmr::vector<VkDeviceQueueCreateInfo>* create_info_list,
    zisc::pmr::vector<float>* priority_list) noexcept
{
  const VulkanDeviceInfo& info = deviceInfoImpl();
  const std::span family_info_list = info.queueFamilyPropertiesList();

  using CreateInfoList = zisc::pmr::vector<vk::DeviceQueueCreateInfo>;
  auto* info_list = zisc::reinterp<CreateInfoList*>(create_info_list);

  const auto find_create_info = [info_list](const uint32b family_index) noexcept
  {
    vk::DeviceQueueCreateInfo* create_info = nullptr;
    for (vk::DeviceQueueCreateInfo& c_info : *info_list) {
      if (c_info.queueFamilyIndex == family_index) {
        create_info = &c_info;
        break;
      }
    }
    return create_info;
  };

  // Create info
  std::size_t total_queues = 0;
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    if (!hasCapability(cap))
      continue;
    const uint32b family_index = queueFamilyIndex(cap);
    if (find_create_info(family_index) == nullptr) {
      info_list->emplace_back();
      vk::DeviceQueueCreateInfo& create_info = info_list->back();
      create_info.setQueueFamilyIndex(family_index);
      const uint32b count = family_info_list[family_index].properties1_.queueCount;
      create_info.setQueueCount(count);
      total_queues += count;
    }
  }

  // Set priorities
  priority_list->resize(total_queues, 0.0f);
  std::size_t offset = 0;
  for (vk::DeviceQueueCreateInfo& create_info : *info_list) {
    create_info.setPQueuePriorities(priority_list->data() + offset);
    offset += create_info.queueCount;
  }

  // Set the priority for GUI to high
  if (hasCapability(CapabilityT::kGui)) {
    const uint32b family_index = queueFamilyIndex(CapabilityT::kGui);
    vk::DeviceQueueCreateInfo* create_info = find_create_info(family_index);
    ZIVC_ASSERT(create_info != nullptr, "Setting priority for GUI failed.");
    const float* p = nullptr;
    std::size_t n = 0;
    if (queueFamilyIndex(CapabilityT::kCompute) == queueFamilyIndex(CapabilityT::kGui)) {
      p = &create_info->pQueuePriorities[create_info->queueCount - 1];
      n = 1;
    }
    else {
      p = &create_info->pQueuePriorities[0];
      n = create_info->queueCount;
    }
    for (std::size_t i = 0; i < n; ++i)
      *const_cast<float*>(p + i) = 1.0f;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initQueueFamilyIndexList()
{
  //! \todo Support queue family option

  queue_family_index_list_.fill(invalidQueueIndex());
  queue_count_list_.fill(0);
  queue_offset_list_.fill((std::numeric_limits<uint32b>::max)());

  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    if (!hasCapability(cap))
      continue;
    uint32b& queue_family_index = queue_family_index_list_[i];
    uint32b& queue_count = queue_count_list_[i];
    queue_family_index = findQueueFamily(cap, &queue_count);
    if (queue_family_index == invalidQueueIndex()) {
      const std::string message = createErrorMessage(
          *this,
          "Appropriate device queue not found.");
      throw SystemError{ErrorCode::kVulkanInitializationFailed, message};
    }
  }

  // Check overlap of queue family index
  if (queueFamilyIndex(CapabilityT::kCompute) == queueFamilyIndex(CapabilityT::kGui)) {
    if (numOfQueues(CapabilityT::kCompute) <= 1) {
      const std::string message = createErrorMessage(
          *this,
          "The device doesn't have enough queue.");
      throw SystemError{ErrorCode::kVulkanInitializationFailed, message};
    }
    const std::size_t compute_index = getCapabilityIndex(CapabilityT::kCompute);
    const std::size_t gui_index = getCapabilityIndex(CapabilityT::kGui);
    queue_count_list_[compute_index] -= 1;
    queue_count_list_[gui_index] = 1;
  }

  // Compute offsets
  uint32b offset = 0;
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    queue_offset_list_[i] = offset;
    offset += queue_count_list_[i];
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initQueueList()
{
  // Calculate toal queues
  uint32b total_queues = 0;
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    total_queues += numOfQueues(cap);
  }

  // Queue counter
  const zisc::pmr::map<uint32b, uint32b>::allocator_type alloc{memoryResource()};
  zisc::pmr::map<uint32b, uint32b> q_counter{alloc};
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    const uint32b family_index = queueFamilyIndex(cap);
    q_counter[family_index] = 0;
  }

  // Initialize queue list
  queue_list_->reserve(total_queues);
  const vk::Device d{device()};
  for (std::size_t i = 0; i < numOfCapabilities(); ++i) {
    const CapabilityT cap = getCapability(i);
    if (!hasCapability(cap))
      continue;
    const uint32b family_index = queueFamilyIndex(cap);
    for (std::size_t j = 0; j < numOfQueues(cap); ++j) {
      const uint32b index = q_counter[family_index]++;
      const vk::Queue q = d.getQueue(family_index, index, dispatcher().loader());
      queue_list_->emplace_back(q);
    }
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initMemoryAllocator()
{
  VulkanBackend& backend_p = parentImpl();
  const VulkanDeviceInfo& info = deviceInfoImpl();

  VkAllocationCallbacks alloc = backend_p.createAllocator();
  VmaDeviceMemoryCallbacks notifier = createAllocationNotifier();
  VmaVulkanFunctions functions = getVmaVulkanFunctions();

  VmaAllocatorCreateInfo create_info;
  create_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
  create_info.physicalDevice = info.device();
  create_info.device = device();
  create_info.preferredLargeHeapBlockSize = 0;
  create_info.pAllocationCallbacks = std::addressof(alloc);
  create_info.pDeviceMemoryCallbacks = std::addressof(notifier);
  create_info.pHeapSizeLimit = nullptr;
  create_info.pVulkanFunctions = std::addressof(functions);
  create_info.instance = backend_p.instance();
  create_info.vulkanApiVersion = vkGetVulkanApiVersion();
  create_info.pTypeExternalMemoryHandleTypes = nullptr;
  const VkResult result = vmaCreateAllocator(std::addressof(create_info),
                                             std::addressof(vm_allocator_));
  if (result != VK_SUCCESS) {
    const std::string message = createErrorMessage(
        *this,
        "VM allocator creation failed.");
    vk::throwResultException(static_cast<vk::Result>(result), message.data());
  }
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
void VulkanDevice::updateFenceDebugInfo(const std::size_t index)
{
  const vk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const VkFence handle = (*fence_list_)[index];
    const vk::Fence f{handle};
    if (f) {
      IdData::NameT obj_name{""};
      concatStr(id_data.name(), obj_name.data());
      concatStr("_fence", obj_name.data());
      IdData::NameT idx{""};
      auto [end, result] = std::to_chars(idx.data(),
                                         idx.data() + idx.size(),
                                         index);
      *end = '\0';
      concatStr(idx.data(), obj_name.data());
      const std::string_view name = obj_name.data();
      setDebugInfo(static_cast<VkObjectType>(f.objectType), handle, name, this);
    }
  }
}

/*!
  \details No detailed description

  \param [in,out] kernel No description.
  */
void VulkanDevice::updateKernelDataDebugInfo(const KernelData& kernel)
{
  const vk::Device d{device()};
  if (isDebugMode() && d) {
    auto set_debug_info = [this, &kernel](const auto& handle,
                                          const auto& obj,
                                          const std::string_view suffix)
    {
      const ModuleData& module = *kernel.module_;
      if (obj) {
        IdData::NameT kernel_name{""};
        concatStr(module.name_, kernel_name.data());
        concatStr("_", kernel_name.data());
        concatStr(kernel.kernel_name_.data(), kernel_name.data());
        concatStr(suffix.data(), kernel_name.data());
        const std::string_view name = kernel_name.data();
        setDebugInfo(static_cast<VkObjectType>(obj.objectType), handle, name, this);
      }
    };
    {
      VkDescriptorSetLayout const handle = kernel.desc_set_layout_;
      const vk::DescriptorSetLayout l{handle};
      set_debug_info(handle, l, "_descsetlayout");
    }
    {
      VkPipelineLayout const handle = kernel.pipeline_layout_;
      const vk::PipelineLayout p{handle};
      set_debug_info(handle, p, "_pipelinelayout");
    }
    {
      VkPipeline const handle = kernel.pipeline_;
      const vk::Pipeline p{handle};
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
  const vk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const VkShaderModule handle = module.module_;
    const vk::ShaderModule m{handle};
    if (m) {
      IdData::NameT module_name{""};
      concatStr(id_data.name(), module_name.data());
      concatStr("_", module_name.data());
      concatStr(module.name_.data(), module_name.data());
      const std::string_view name = module_name.data();
      setDebugInfo(static_cast<VkObjectType>(m.objectType), handle, name, this);
    }
  }
}

} // namespace zivc
