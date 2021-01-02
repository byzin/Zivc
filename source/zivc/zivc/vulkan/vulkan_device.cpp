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
#include <numeric>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/thread/bitset.hpp"
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
  const VkCommandBuffer& cmd = isDebugMode() ? command_buffer : VK_NULL_HANDLE;
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
    const VkCommandBufferUsageFlags flags) const noexcept
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
  const VkQueue& que = isDebugMode() ? q : VK_NULL_HANDLE;
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
  auto& fence_list = *fence_list_;
  for (std::size_t i = 0; i < fence_list.size(); ++i) {
    if (zisc::cast<zivcvk::Fence>(fence_list[i]) == *dest) {
      fence_manager_->testAndSet(i, true);
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

  \param [in] vk_object_type No description.
  \param [in] vk_handle No description.
  \param [in] object_name No description.
  \param [in] zivc_object No description.
  */
void VulkanDevice::setDebugInfo(const VkObjectType vk_object_type,
                                const void* vk_handle,
                                const std::string_view object_name,
                                const ZivcObject* zivc_object) noexcept
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
    auto result = d.setDebugUtilsObjectNameEXT(std::addressof(name_info), *loader);
  }
  // Tag
  if (zivc_object != nullptr) {
    const auto tag_name = zisc::cast<uint64b>(zivc_object->id().id());
    const zivcvk::DebugUtilsObjectTagInfoEXT tag_info{object_type,
                                                      handle,
                                                      tag_name,
                                                      sizeof(zivc_object),
                                                      std::addressof(zivc_object)};
    auto result = d.setDebugUtilsObjectTagEXT(std::addressof(tag_info), *loader);
  }
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void VulkanDevice::setFenceSize(const std::size_t s)
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
    fence_manager_->testAndSet(fence_list.size(), true);
    fence_list.emplace_back(zisc::cast<VkFence>(fence));
    updateFenceDebugInfo(i);
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
    if (fence_manager_->testAndSet(i, false)) {
      zivcvk::Fence f = zisc::cast<zivcvk::Fence>(fence_list[i]);
      auto result = d.resetFences(1, std::addressof(f), *loader);
      *dest = f;
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
  constexpr uint64b timeout = (std::numeric_limits<uint64b>::max)();
  const auto result = d.waitForFences(1, f, VK_TRUE, timeout, *loader);
  if (result != zivcvk::Result::eSuccess) {
    //! \todo Raise an error
    printf("[Warning] Waiting for a fence failed.\n");
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
    vm_allocator_ = VK_NULL_HANDLE;
  }

  zivcvk::Device d{device()};
  if (d) {
    auto& sub_platform = parentImpl();
    zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
    const auto loader = dispatcher().loaderImpl();

    setFenceSize(0); // Destroy all fences

    // Shader modules
    for (auto& module : *shader_module_list_) {
      zivcvk::ShaderModule m{module.second.module_};
      if (m)
        d.destroyShaderModule(m, alloc, *loader);
    }

    // Command pool
    zivcvk::CommandPool command_pool{command_pool_};
    if (command_pool) {
      //! \todo Fix me. AMD gpu won't work with custom allocator
      d.destroyCommandPool(command_pool, nullptr /* alloc */, *loader);
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
  setFenceSize(1);
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
  \param [in,out] user_data No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryFreeing(
    [[maybe_unused]] VmaAllocator vm_allocator,
    uint32b memory_type,
    [[maybe_unused]] VkDeviceMemory memory,
    VkDeviceSize size,
    void* user_data)
{
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

  extensions = {//VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME,
#if defined(Z_MAC)
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                "VK_KHR_portability_subset",
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

  \param [in] index No description.
  */
void VulkanDevice::updateFenceDebugInfo(const std::size_t index) noexcept
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

  \param [in] id No description.
  */
void VulkanDevice::updateShaderModuleDebugInfo(const uint32b id) noexcept
{
  const zivcvk::Device d{device()};
  if (isDebugMode() && d) {
    const IdData& id_data = ZivcObject::id();
    const ModuleData& module_data = getShaderModule(id);
    const VkShaderModule handle = module_data.module_;
    const zivcvk::ShaderModule m{handle};
    if (m) {
      IdData::NameType module_name{""};
      std::strcat(module_name.data(), id_data.name().data());
      std::strcat(module_name.data(), "_");
      std::strcat(module_name.data(), module_data.name_.data());
      const std::string_view name = module_name.data();
      setDebugInfo(zisc::cast<VkObjectType>(m.objectType), handle, name, this);
    }
  }
}

} // namespace zivc
