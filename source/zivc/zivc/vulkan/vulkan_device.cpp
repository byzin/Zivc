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
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "vulkan_sub_platform.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

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

  \param [in] size No description.
  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanDevice::allocateMemory(const std::size_t size,
                                  const BufferUsage buffer_usage,
                                  void* user_data,
                                  VkBuffer* buffer,
                                  VmaAllocation* vm_allocation,
                                  VmaAllocationInfo* alloc_info)
{
  // Buffer create info
  zivcvk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  buffer_create_info.usage = zivcvk::BufferUsageFlagBits::eTransferSrc |
                             zivcvk::BufferUsageFlagBits::eTransferDst |
                             zivcvk::BufferUsageFlagBits::eStorageBuffer;
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

///*!
//  */
//template <std::size_t kDimension> inline
//std::array<uint32b, 3> VulkanDevice::calcWorkGroupSize(
//    const std::array<uint32b, kDimension>& works) const noexcept
//{
//  std::array<uint32b, 3> work_group_size{{1, 1, 1}};
//  const auto& local_work_size = localWorkSize<kDimension>();
//  for (std::size_t i = 0; i < kDimension; ++i) {
//    work_group_size[i] = ((works[i] % local_work_size[i]) == 0)
//        ? works[i] / local_work_size[i]
//        : works[i] / local_work_size[i] + 1;
//  }
//  return work_group_size;
//}
//
///*!
//  */
//inline
//auto VulkanDevice::commandPool(const QueueType queue_type) noexcept
//    -> vk::CommandPool&
//{
//  const std::size_t list_index = zisc::cast<std::size_t>(queue_type);
//  const std::size_t ref_index = queue_family_index_ref_list_[list_index];
//  return command_pool_list_[ref_index];
//}
//
///*!
//  */
//inline
//auto VulkanDevice::commandPool(const QueueType queue_type) const noexcept
//    -> const vk::CommandPool&
//{
//  const std::size_t list_index = zisc::cast<std::size_t>(queue_type);
//  const std::size_t ref_index = queue_family_index_ref_list_[list_index];
//  return command_pool_list_[ref_index];
//}

void VulkanDevice::deallocateMemory(VkBuffer* buffer,
                                    VmaAllocation* vm_allocation,
                                    VmaAllocationInfo* alloc_info) noexcept
{
  if (zivcvk::Buffer{*buffer}) {
    vmaDestroyBuffer(memoryAllocator(), *buffer, *vm_allocation);
  }
}

///*!
//  */
//template <DescriptorType kDescriptor, typename Type> inline
//void VulkanDevice::deallocate(VulkanBuffer<kDescriptor, Type>* buffer) noexcept
//{
//  ZISC_ASSERT(buffer != nullptr, "The buffer is null.");
//  auto& b = buffer->buffer();
//  auto& memory = buffer->memory();
//  auto& alloc_info = buffer->allocationInfo();
//  if (b) {
//    vmaDestroyBuffer(allocator_, *reinterpret_cast<VkBuffer*>(&b), memory);
//
//    if (buffer->isDeviceMemory()) {
//      const std::size_t memory_usage = deviceMemoryUsage() - buffer->memoryUsage();
//      setDeviceMemoryUsage(memory_usage);
//    }
//    else {
//      const std::size_t memory_usage = hostMemoryUsage() - buffer->memoryUsage();
//      setHostMemoryUsage(memory_usage);
//    }
//
//    b = nullptr;
//    memory = VK_NULL_HANDLE;
//    alloc_info.size = 0;
//  }
//}

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

  \param [out] command_buffer No description.
  */
void VulkanDevice::initKernelCommandBuffer(VkCommandBuffer* command_buffer)
{
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  auto mem_resource = memoryResource();

  const zivcvk::CommandBufferAllocateInfo alloc_info{
      commandPool(),
      zivcvk::CommandBufferLevel::ePrimary,
      1};
  zisc::pmr::vector<zivcvk::CommandBuffer>::allocator_type alloc{mem_resource};
  auto cbuffers = d.allocateCommandBuffers(alloc_info, alloc, *loader);
  ZISC_ASSERT(cbuffers.size() == 1, "The size of command buffers isn't 1.");
  *command_buffer = zisc::cast<VkCommandBuffer>(cbuffers[0]);
}

/*!
  \details No detailed description

  \param [in] num_of_buffers No description.
  \param [out] descriptor_set_layout No description.
  \param [out] descriptor_pool No description.
  \param [out] descriptor_set No description.
  */
void VulkanDevice::initKernelDescriptorSet(
    const std::size_t num_of_buffers,
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
    layout_bindings.resize(num_of_buffers);
    for (std::size_t index = 0; index < num_of_buffers; ++index) {
      layout_bindings[index] = zivcvk::DescriptorSetLayoutBinding{
          zisc::cast<uint32b>(index),
          zivcvk::DescriptorType::eStorageBuffer,
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
    zivcvk::DescriptorPoolSize pool_size{
        zivcvk::DescriptorType::eStorageBuffer,
        zisc::cast<uint32b>(num_of_buffers)};
    const zivcvk::DescriptorPoolCreateInfo create_info{
        zivcvk::DescriptorPoolCreateFlags{},
        1,
        1,
        std::addressof(pool_size)};

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
    zivcvk::DescriptorSetLayout desc_set_layout{set_layout};
    const zivcvk::PipelineLayoutCreateInfo create_info{
        zivcvk::PipelineLayoutCreateFlags{},
        1,
        std::addressof(desc_set_layout)};
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
    pline = d.createComputePipeline(zivcvk::PipelineCache{},
                                    pipeline_info,
                                    alloc,
                                    *loader);
  }

  *pipeline_layout = zisc::cast<VkPipelineLayout>(pline_layout);
  *compute_pipeline = zisc::cast<VkPipeline>(pline);
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

  const auto& p = queue_family_list[index].properties1_;
  const std::size_t n = p.queueCount;
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
void VulkanDevice::destroyData() noexcept
{
  queue_family_index_ = invalidQueueIndex();

  if (vm_allocator_) {
    vmaDestroyAllocator(vm_allocator_);
    vm_allocator_ = VK_NULL_HANDLE;
  }

//  if (device_) {
//    for (auto& module : shader_module_list_) {
//      if (module) {
//        device_.destroyShaderModule(module);
//        module = nullptr;
//      }
//    }
//    for (std::size_t i = 0; i < command_pool_list_.size(); ++i) {
//      auto command_pool = command_pool_list_[i];
//      if (command_pool) {
//        device_.destroyCommandPool(command_pool);
//        command_pool_list_[i] = nullptr;
//      }
//    }
//    device_.destroy();
//    device_ = nullptr;
//  }

  zivcvk::Device d{device()};
  if (d) {
    auto& sub_platform = parentImpl();
    zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
    const auto loader = dispatcher().loaderImpl();

    // Shader modules
    for (auto& module : *shader_module_list_) {
      zivcvk::ShaderModule m{module.second};
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
}

/*!
  \details No detailed description
  */
void VulkanDevice::initData()
{
  {
    auto mem_resource = memoryResource();
    using UsageList = decltype(heap_usage_list_)::element_type;
    zisc::pmr::polymorphic_allocator<zisc::Memory::Usage> alloce{mem_resource};
    UsageList usage_list{alloce};

    zisc::pmr::polymorphic_allocator<UsageList> alloc{mem_resource};
    heap_usage_list_ = zisc::pmr::allocateUnique(alloc, std::move(usage_list));

    const auto& info = deviceInfoData();
    heap_usage_list_->resize(info.numOfHeaps());
  }

  initDispatcher();
  initWorkGroupSizeDim();
  initQueueFamilyIndexList();
  initDevice();
  initMemoryAllocator();
  initCommandPool();
}

///*!
//  */
//inline
//const vk::ShaderModule& VulkanDevice::getShaderModule(
//    const std::size_t index) const noexcept
//{
//  ZISC_ASSERT(hasShaderModule(index), "The shader module doesn't exist.");
//  return shader_module_list_[index];
//}

///*!
//  */
//inline
//bool VulkanDevice::hasShaderModule(const std::size_t index) const noexcept
//{
//  const bool flag = (index < shader_module_list_.size()) &&
//                    shader_module_list_[index];
//  return flag;
//}

///*!
//  */
//template <std::size_t kDimension> inline
//const std::array<uint32b, 3>& VulkanDevice::localWorkSize() const noexcept
//{
//  static_assert((0 < kDimension) && (kDimension <= 3),
//                "The dimension is out of range.");
//  return local_work_size_list_[kDimension - 1];
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename Type> inline
//UniqueBuffer<kDescriptor, Type> VulkanDevice::makeBuffer(
//    const BufferUsage usage_flag) noexcept
//{
//  using UniqueVulkanBuffer =
//      zisc::UniqueMemoryPointer<VulkanBuffer<kDescriptor, Type>>;
//  auto buffer = UniqueVulkanBuffer::make(memoryResource(), this, usage_flag);
//  return std::move(buffer);
//}
//
///*!
//  */
//template <std::size_t kDimension, typename Function, typename ...ArgumentTypes>
//inline
//UniqueKernel<kDimension, ArgumentTypes...> VulkanDevice::makeKernel(
//    const uint32b module_index,
//    const std::string_view kernel_name) noexcept
//{
//  using UniqueVulkanKernel = zisc::UniqueMemoryPointer<VulkanKernel<
//      kDimension,
//      Function,
//      ArgumentTypes...>>;
//  UniqueKernel<kDimension, ArgumentTypes...> kernel =
//      UniqueVulkanKernel::make(memoryResource(), this, module_index, kernel_name);
//  return kernel;
//}
//
///*!
//  */
//inline
//VmaAllocator& VulkanDevice::memoryAllocator() noexcept
//{
//  return allocator_;
//}
//
///*!
//  */
//inline
//const VmaAllocator& VulkanDevice::memoryAllocator() const noexcept
//{
//  return allocator_;
//}

///*!
//  */
//inline
//void VulkanDevice::submit(const QueueType queue_type,
//                          const uint32b queue_index,
//                          const vk::CommandBuffer& command) const noexcept
//{
//  auto q = getQueue(queue_type, queue_index);
//  const vk::SubmitInfo info{0, nullptr, nullptr, 1, &command};
//  const auto result = q.submit(1, &info, nullptr);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Command submission failed.");
//  (void)result;
//}

///*!
//  \details No detailed description
//  */
//void VulkanDevice::waitForCompletion() const noexcept
//{
////  const auto result = device_.waitIdle();
////  ZISC_ASSERT(result == vk::Result::eSuccess, "Device waiting failed.");
////  (void)result;
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  */
//void VulkanDevice::waitForCompletion(const QueueType queue_type) const noexcept
//{
////  const uint32b family_index = queueFamilyIndex(queue_type);
////  const auto& info = physicalDeviceInfo();
////  const auto& family_info_list = info.queueFamilyPropertiesList();
////  const auto& family_info = family_info_list[family_index].properties1_;
////  for (uint32b i = 0; i < family_info.queueCount; ++i)
////    waitForCompletion(queue_type, i);
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  \param [in] queue_index No description.
//  */
//void VulkanDevice::waitForCompletion(const QueueType queue_type,
//                                     const uint32b queue_index) const noexcept
//{
////  auto q = getQueue(queue_type, queue_index);
////  const auto result = q.waitIdle();
////  ZISC_ASSERT(result == vk::Result::eSuccess, "Queue waiting failed.");
////  (void)result;
//}

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
                                   const zisc::pmr::vector<uint32b>& spirv_code)
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
  shader_module_list_->emplace(id, zisc::cast<VkShaderModule>(module));
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
    constexpr auto graphics = zivcvk::QueueFlagBits::eGraphics;
    constexpr auto compute = zivcvk::QueueFlagBits::eCompute;
    constexpr auto transfer = zivcvk::QueueFlagBits::eTransfer;
    constexpr auto sparse = zivcvk::QueueFlagBits::eSparseBinding;

    auto has_flag = [&props](const zivcvk::QueueFlagBits flag) noexcept
    {
      const bool result = (props.queueFlags & flag) == flag;
      return result;
    };

    const bool result = (!has_flag(graphics) || !graphics_excluded) &&
                        has_flag(compute) &&
                        has_flag(transfer) &&
                        (!has_flag(sparse) || !sparse_excluded);
    return result;
  };

  const auto& info = deviceInfoData();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  uint32b index = invalidQueueIndex();
  uint32b index2 = invalidQueueIndex();
  uint32b num_of_queues = 0;
  uint32b num_of_queues2 = 0;

  for (std::size_t i = 0; i < queue_family_list.size(); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (has_flags(p, true, false) && (num_of_queues <= p.queueCount)) {
      index = zisc::cast<uint32b>(i);
      num_of_queues = p.queueCount;
    }
    if (has_flags(p, false, false) && (num_of_queues2 <= p.queueCount)) {
      index2 = zisc::cast<uint32b>(i);
      num_of_queues2 = p.queueCount;
    }
  }

  index = (index != invalidQueueIndex()) ? index : index2;
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

///*!
//  */
//inline
//vk::Queue VulkanDevice::getQueue(const QueueType queue_type,
//                                 const uint32b queue_index) const noexcept
//{
//  const uint32b family_index = queueFamilyIndex(queue_type);
//  const auto& info = physicalDeviceInfo();
//  const auto& family_info_list = info.queueFamilyPropertiesList();
//  const auto& family_info = family_info_list[family_index].properties1_;
//  const uint32b index = queue_index % family_info.queueCount;
//  auto q = device_.getQueue(family_index, index);
//  return q;
//}

/*!
  \details No detailed description
  */
void VulkanDevice::initCommandPool()
{
  auto& sub_platform = parentImpl();
  zivcvk::Device d{device()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};

  const zivcvk::CommandPoolCreateInfo create_info{zivcvk::CommandPoolCreateFlags{},
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
  zisc::pmr::vector<float> priority_list{
      zisc::pmr::vector<float>::allocator_type{memoryResource()}};
  zivcvk::DeviceQueueCreateInfo queue_create_info;
  {
    // Queue family index
    const uint32b index = queueFamilyIndex();
    queue_create_info.setQueueFamilyIndex(index);
    // Queue counts
    const auto& queue_family_list = info.queueFamilyPropertiesList();
    const auto& family_info = queue_family_list[index];
    const uint32b num_of_queues = family_info.properties1_.queueCount;
    queue_create_info.setQueueCount(num_of_queues);
    // Priorities
    priority_list.resize(num_of_queues, 1.0f);
    queue_create_info.setPQueuePriorities(priority_list.data());
  }

  // Device features
  const auto& features = info.features();
  auto b16bit_storage = features.b16bit_storage_;
  auto b8bit_storage = features.b8bit_storage_;
  auto shader_atomic_int64 = features.shader_atomic_int64_;
  auto shader_float16_int8 = features.shader_float16_int8_;
  auto variable_pointers = features.variable_pointers_;
  zivcvk::PhysicalDeviceFeatures2 device_features;
  {
    device_features.features.shaderFloat64 = features.features1_.shaderFloat64;
    device_features.features.shaderInt64 = features.features1_.shaderInt64;
    device_features.features.shaderInt16 = features.features1_.shaderInt16;
    if (sub_platform.isDebugMode()) {
      device_features.features.vertexPipelineStoresAndAtomics =
          features.features1_.vertexPipelineStoresAndAtomics;
      device_features.features.fragmentStoresAndAtomics =
          features.features1_.fragmentStoresAndAtomics;
    }
    VulkanDeviceInfo::link(device_features,
                           b16bit_storage,
                           b8bit_storage,
                           shader_atomic_int64,
                           shader_float16_int8,
                           variable_pointers);
  }

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

} // namespace zivc
