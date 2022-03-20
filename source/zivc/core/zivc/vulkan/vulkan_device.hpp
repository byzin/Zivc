/*!
  \file vulkan_device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_HPP
#define ZIVC_VULKAN_DEVICE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <map>
#include <memory>
#include <span>
#include <shared_mutex>
#include <string_view>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/data_structure/query_value.hpp"
#include "zisc/data_structure/queue.hpp"
#include "zisc/data_structure/scalable_circular_queue.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/device.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class Fence;
class VulkanBackend;
class VulkanDeviceInfo;

template <typename Type>
concept LabelOptions = requires (const Type& o) {
  {o.label()} -> zisc::ConvertibleTo<std::string_view>;
  {o.labelColor()} -> zisc::ConvertibleTo<const std::template span<const float, 4>>;
};

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanDevice : public Device
{
 public:
  /*!
    \brief No brief description

    No detailed description.
    */
  struct ModuleData
  {
    std::string_view name_;
    VkShaderModule module_ = ZIVC_VK_NULL_HANDLE;
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  struct KernelData
  {
    const ModuleData* module_ = nullptr;
    IdData::NameType kernel_name_;
    VkDescriptorSetLayout desc_set_layout_ = ZIVC_VK_NULL_HANDLE;
    VkPipelineLayout pipeline_layout_ = ZIVC_VK_NULL_HANDLE;
    VkPipeline pipeline_ = ZIVC_VK_NULL_HANDLE;
  };

  // Type aliases
  using Capability = VulkanDeviceCapability;
  static constexpr std::size_t kNumOfCapabilities = 2;


  //! Initialize the vulkan device
  VulkanDevice(IdData&& id);

  //! Finalize the vulkan instance
  ~VulkanDevice() noexcept override;


  //! Add a kernel of the give kernel name
  const KernelData& addShaderKernel(const ModuleData& module,
                                    const std::string_view kernel_name,
                                    const std::size_t work_dimension,
                                    const std::size_t num_of_storage_buffers,
                                    const std::size_t num_of_uniform_buffers,
                                    const std::size_t num_of_local_args);

  //! Add a shader module of the given kernel set
  template <typename SetType>
  const ModuleData& addShaderModule(const KernelSet<SetType>& kernel_set);

  //! Return the command pool for compute
  VkCommandPool& commandPool() noexcept;

  //! Return the command pool for compute
  const VkCommandPool& commandPool() const noexcept;

  //! Create a device memory allocation notifier
  VmaDeviceMemoryCallbacks createAllocationNotifier() noexcept;

  //! Create a command buffer
  [[nodiscard]]
  VkCommandBuffer createCommandBuffer();

  //! Create a host memory allocator for Vulkan object
  VkAllocationCallbacks createAllocator() noexcept;

  //! Return the underlying vulkan device
  VkDevice& device() noexcept;

  //! Return the underlying vulkan device
  const VkDevice& device() const noexcept;

  //! Return the underlying device info
  const VulkanDeviceInfo& deviceInfoImpl() const noexcept;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return the queue by the index
  VkQueue& getQueue(const Capability cap, const std::size_t index) noexcept;

  //! Return the queue by the index
  const VkQueue& getQueue(const Capability cap, const std::size_t index) const noexcept;

  //! Return the queue family index list
  std::array<uint32b, kNumOfCapabilities> getQueueFamilyIndexList(uint32b* size) const noexcept;

  //! Return the kernel id of the given kernel name
  static uint64b getKernelId(const std::string_view module_name,
                             const std::string_view kernel_name) noexcept;

  //! Return the shader module by the the given kernel set ID
  const KernelData& getShaderKernel(const uint64b id) const noexcept;

  //! Return the shader module by the the given kernel set ID
  const ModuleData& getShaderModule(const uint64b id) const noexcept;

  //! Check if the device can be used for the given capability
  bool hasCapability(const Capability cap) const noexcept;

  //! Check if the device has the kernel of the give kernel id
  bool hasShaderKernel(const uint64b id) const noexcept;

  //! Check if the device has the shader module of the given kernel set ID
  bool hasShaderModule(const uint64b id) const noexcept;

  //! Return the invalid queue index in queue families
  static constexpr uint32b invalidQueueIndex() noexcept;

  //! Make a debug label for a commandbuffer
  template <LabelOptions Options>
  [[nodiscard]]
  CmdDebugLabelRegion makeCmdDebugLabel(const VkCommandBuffer& command_buffer,
                                        const Options& options) const noexcept;

  //! Make a debug label for a commandbuffer
  [[nodiscard]]
  CmdDebugLabelRegion makeCmdDebugLabel(const VkCommandBuffer& command_buffer,
                                        const std::string_view label_name,
                                        const std::span<const float, 4> color) const noexcept;

  //! Make a record region for a commandbuffer
  [[nodiscard]]
  CmdRecordRegion makeCmdRecord(const VkCommandBuffer& command_buffer,
                                const VkCommandBufferUsageFlags flags) const;

  //! Make a debug label for a queue
  template <LabelOptions Options>
  [[nodiscard]]
  QueueDebugLabelRegion makeQueueDebugLabel(const VkQueue& q,
                                            const Options& options) const noexcept;

  //! Make a debug label for a queue
  [[nodiscard]]
  QueueDebugLabelRegion makeQueueDebugLabel(const VkQueue& q,
                                            const std::string_view label_name,
                                            const std::span<const float, 4> color) const noexcept;

  //! Return the memory allocator of the device
  VmaAllocator& memoryAllocator() noexcept;

  //! Return the memory allocator of the device
  const VmaAllocator& memoryAllocator() const noexcept;

  //! Return the memory usage by the given heap index
  zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) noexcept override;

  //! Return the memory usage by the given heap index
  const zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) const noexcept override;

  //! Return the number of available fences
  std::size_t numOfFences() const noexcept override;

  //! Return the number of underlying command queues for compute
  std::size_t numOfQueues() const noexcept override;

  //! Return the number of underlying command queues for compute
  std::size_t numOfQueues(const Capability cap) const noexcept;

  //! Return an index of a queue family
  uint32b queueFamilyIndex(const Capability cap) const noexcept;

  //! Return the use of the given fence to the device
  void returnFence(Fence* fence) noexcept override;

  //! Set debug info of the given object
  void setDebugInfo(const VkObjectType vk_object_type,
                    const void* vk_handle,
                    const std::string_view object_name,
                    const ZivcObject* zivc_object);

  //! Set the number of faces
  void setFenceSize(const std::size_t s) override;

  //! Submit the given command
  void submit(const VkCommandBuffer& command_buffer,
              const VkQueue& q,
              const Fence& fence) const;

  //! Take a use of a fence from the device
  void takeFence(Fence* fence) override;

  //! Wait for a device to be idle
  void waitForCompletion() const override;

  //! Wait for a queue to be idle
  void waitForCompletion(const uint32b queue_index) const override;

  //! Wait for a queue for compute to be idle
  void waitForCompletion(const Capability cap,const uint32b queue_index) const;

  //! Wait for a fence to be signaled
  void waitForCompletion(const Fence& fence) const override;

  //! Return the work group size of the given dimension
  std::span<const uint32b, 3> workGroupSizeDim(const std::size_t dim) const noexcept;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

  //! Initialize the vulkan device
  void initData() override;

  //! Update the debug info
  void updateDebugInfoImpl() override;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  class Callbacks
  {
   public:
    //! Return the index of memory heap
    static std::size_t getHeapIndex(const VulkanDevice& device,
                                    const uint32b memory_type) noexcept;

    //! Notify of a memory allocation in VMA
    static void notifyOfDeviceMemoryAllocation(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size,
        void* user_data);

    //! Notify of a memory deallocation in VMA
    static void notifyOfDeviceMemoryDeallocation(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size,
        void* user_data);
  };

  using IndexQueueImpl = zisc::ScalableCircularQueue<zisc::QueryValueU64>;
  using IndexQueue = zisc::Queue<IndexQueueImpl, zisc::QueryValueU64>;
  static_assert(IndexQueue::isConcurrent(), "The queue must be concurrent.");
  using UniqueModuleData = zisc::pmr::unique_ptr<ModuleData>;
  using UniqueKernelData = zisc::pmr::unique_ptr<KernelData>;


  //! Add a shader module of the given kernel set
  const ModuleData& addShaderModule(const uint64b id,
                                    const std::span<const uint32b> spirv_code,
                                    const std::string_view module_name);

  //! Return the capability of the index
  static constexpr Capability getCapability(const std::size_t index) noexcept;

  //! Return the index of the give capability
  static constexpr std::size_t getCapabilityIndex(const Capability cap) noexcept;

  //! Check if the give queue family property has required flags
  static bool checkQueueFamilyFlags(const VkQueueFamilyProperties& props,
                                    const bool graphics_required,
                                    const bool compute_required,
                                    const bool graphics_excluded,
                                    const bool compute_excluded) noexcept;

  void destroyShaderKernel(KernelData* kernel) noexcept;

  //! Destroy shader module data
  void destroyShaderModule(ModuleData* module) noexcept;

  //! Return the underlying fence index queue
  IndexQueue& fenceIndexQueue() noexcept;

  //! Return the underlying fence index queue
  const IndexQueue& fenceIndexQueue() const noexcept;

  //! Find the index of the optimal queue familty
  uint32b findQueueFamily(const Capability cap, uint32b* queue_count) const noexcept;

  //! Get Vulkan function pointers used in VMA
  VmaVulkanFunctions getVmaVulkanFunctions() const noexcept;

  //! Initialize capabilities
  void initCapability() noexcept;

  //! Initialize a command pool
  void initCommandPool();

  //! Initialize a device
  void initDevice();

  //! Initialize the vulkan dispatch loader
  void initDispatcher();

  //! Initialize device properties
  void initProperties(zisc::pmr::vector<const char*>* extensions,
                      zisc::pmr::vector<const char*>* layers);

  //! Initialize work group size of dimensions
  void initWorkGroupSizeDim() noexcept;

  //! Initialize queue create info list
  void initQueueCreateInfoList(
      zisc::pmr::vector<VkDeviceQueueCreateInfo>* create_info_list,
      zisc::pmr::vector<float>* priority_list) noexcept;

  //! Initialize a queue family index list
  void initQueueFamilyIndexList();

  //! Initialize a queue list
  void initQueueList();

  //! Initialize a vulkan memory allocator
  void initMemoryAllocator();

  //! Return the number of supported capabilities
  static constexpr std::size_t numOfCapabilities() noexcept;

  //! Return the backend
  VulkanBackend& parentImpl() noexcept;

  //! Return the backend
  const VulkanBackend& parentImpl() const noexcept;

  //! Return the offset of queue list for the given capability
  std::size_t queueOffset(const Capability cap) const noexcept;

  //! Update the debug info of the fence by the given index
  void updateFenceDebugInfo(const std::size_t index);

  //! Update the debug info of the kernel data of the given ID
  void updateKernelDataDebugInfo(const KernelData& kernel);

  //! Update the debug info of the shader module of the given ID
  void updateShaderModuleDebugInfo(const ModuleData& module);


  mutable std::shared_mutex shader_mutex_;
  VkDevice device_ = ZIVC_VK_NULL_HANDLE;
  VmaAllocator vm_allocator_ = ZIVC_VK_NULL_HANDLE;
  VkCommandPool command_pool_ = ZIVC_VK_NULL_HANDLE;
  zisc::pmr::unique_ptr<IndexQueueImpl> fence_index_queue_;
  ZivcObject::UniqueVector<VkFence> fence_list_;
  ZivcObject::UniqueVector<VkQueue> queue_list_;
  ZivcObject::UniqueVector<zisc::Memory::Usage> heap_usage_list_;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  ZivcObject::UniqueMap<uint64b, UniqueModuleData> module_data_list_;
  ZivcObject::UniqueMap<uint64b, UniqueKernelData> kernel_data_list_;
  std::array<uint32b, kNumOfCapabilities> queue_family_index_list_;
  std::array<uint32b, kNumOfCapabilities> queue_count_list_;
  std::array<uint32b, kNumOfCapabilities> queue_offset_list_;
  uint32b capabilities_;
  std::array<std::array<uint32b, 3>, 3> work_group_size_list_;
};

} // namespace zivc

#include "vulkan_device-inl.hpp"

#endif // ZIVC_VULKAN_DEVICE_HPP
