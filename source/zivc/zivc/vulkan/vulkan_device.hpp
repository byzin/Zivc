/*!
  \file vulkan_device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
#include <string_view>
#include <tuple>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/thread/bitset.hpp"
// Zivc
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
class VulkanDeviceInfo;
class VulkanSubPlatform;

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
    VkShaderModule module_ = VK_NULL_HANDLE;
    std::string_view name_;
  };


  //! Initialize the vulkan device
  VulkanDevice(IdData&& id);

  //! Finalize the vulkan instance
  ~VulkanDevice() noexcept override;


  //! Add a shader module of the given kernel set
  template <typename SetType>
  void addShaderModule(const KernelSet<SetType>& kernel_set);

  //! Return the command pool
  VkCommandPool& commandPool() noexcept;

  //! Return the command pool
  const VkCommandPool& commandPool() const noexcept;

  //! Return the underlying vulkan device
  VkDevice& device() noexcept;

  //! Return the underlying vulkan device
  const VkDevice& device() const noexcept;

  //! Return the underlying device info
  const VulkanDeviceInfo& deviceInfoData() const noexcept;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return the queue by the index
  VkQueue& getQueue(const uint32b index) noexcept;

  //! Return the queue by the index
  const VkQueue& getQueue(const uint32b index) const noexcept;

  //! Return the shader module by the the given kernel set ID
  const ModuleData& getShaderModule(const uint32b id) const noexcept;

  //! Check if the device has the shader module of the given kernel set ID
  bool hasShaderModule(const uint32b id) const noexcept;

  //! Return the invalid queue index in queue families
  static constexpr uint32b invalidQueueIndex() noexcept;

  //! Make a command buffer
  VkCommandBuffer makeCommandBuffer();

  //! Return the memory allocator of the device
  VmaAllocator& memoryAllocator() noexcept;

  //! Return the memory allocator of the device
  const VmaAllocator& memoryAllocator() const noexcept;

  //! Return the number of available fences
  std::size_t numOfFences() const noexcept override;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the peak memory usage of the heap of the given number
  std::size_t peakMemoryUsage(const std::size_t number) const noexcept override;

  //! Return the ownership of the fence to the device
  void returnFence(Fence* fence) noexcept override;

  //! Set the debug info
  void setDebugInfo(const VkObjectType object_type,
                    const uint64b object_handle,
                    const std::string_view object_name,
                    const ZivcObject* zivc_object) noexcept;

  //! Set the number of faces
  void setNumOfFences(const std::size_t s) override;

  //! Submit the given command
  void submit(const VkCommandBuffer& command_buffer,
              const VkQueue& q,
              const Fence& fence);

  //! Take a fence data from the device
  void takeFence(Fence* fence) override;

  //! Return the current memory usage of the heap of the given number
  std::size_t totalMemoryUsage(const std::size_t number) const noexcept override;

  //! Wait for a device to be idle
  void waitForCompletion() const noexcept override;

  //! Wait for a queue to be idle
  void waitForCompletion(const uint32b queue_index) const noexcept override;

  //! Wait for a fence to be signaled
  void waitForCompletion(const Fence& fence) const noexcept override;

  // For buffer

  //! Allocate a device memory
  void allocateMemory(const std::size_t size,
                      const BufferUsage buffer_usage,
                      const VkBufferUsageFlagBits desc_type,
                      void* user_data,
                      VkBuffer* buffer,
                      VmaAllocation* vm_allocation,
                      VmaAllocationInfo* alloc_info);

  //! Copy
  void copyBufferCmd(const VkCommandBuffer& command_buffer,
                     const VkBuffer& source_buffer,
                     const VkBuffer& dest_buffer,
                     const VkBufferCopy& region);

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) noexcept;

  //! Fill the given buffer with specified value
  void fillBufferFastCmd(const VkCommandBuffer& command_buffer,
                         const VkBuffer& buffer,
                         const std::size_t dest_offset,
                         const std::size_t size,
                         const uint32b data) noexcept;

  // For kernel

  //! Destroy descriptor set
  void destroyKernelDescriptorSet(VkDescriptorSetLayout* descriptor_set_layout,
                                  VkDescriptorPool* descriptor_pool) noexcept;

  //! Destroy pipeline
  void destroyKernelPipeline(VkPipelineLayout* pipeline_layout,
                             VkPipeline* compute_pipeline) noexcept;

  //! Record dispatching the given kernel to the vulkan device
  void dispatchKernelCmd(const VkCommandBuffer& command_buffer,
                         const VkDescriptorSet& descriptor_set,
                         const VkPipelineLayout& pipeline_layout,
                         const VkPipeline& pipeline,
                         const std::size_t work_dimension,
                         const std::array<uint32b, 3>& work_group_size);

  //! Initialize a descriptor set of a kernel
  void initKernelDescriptorSet(const std::size_t num_of_storage_buffers,
                               const std::size_t num_of_uniform_buffers,
                               VkDescriptorSetLayout* descriptor_set_layout,
                               VkDescriptorPool* descriptor_pool,
                               VkDescriptorSet* descriptor_set);

  //! Initialize a pipeline of a kernel
  void initKernelPipeline(const std::size_t work_dimension,
                          const std::size_t num_of_local_args,
                          const VkDescriptorSetLayout& set_layout,
                          const VkShaderModule& module,
                          const std::string_view kernel_name,
                          VkPipelineLayout* pipeline_layout,
                          VkPipeline* compute_pipeline);

  //! Record push constant command
  template <typename Type>
  void pushConstantCmd(const VkCommandBuffer& command_buffer,
                       const VkPipelineLayout& pline_layout,
                       const std::size_t offset,
                       const Type& data);

  //! Record push constant command
  void pushConstantCmd(const VkCommandBuffer& command_buffer,
                       const VkPipelineLayout& pline_layout,
                       const std::size_t offset,
                       const std::size_t size,
                       const void* data);

  //! Update the given descriptor set with the given buffers
  template <std::size_t kN>
  void updateDescriptorSet(const VkDescriptorSet& descriptor_set,
                           const std::array<VkBuffer, kN>& buffer_list,
                           const std::array<VkDescriptorType, kN>& desc_type_list);

  //! Return the work group size of the given dimension
  const std::array<uint32b, 3>& workGroupSizeDim(const std::size_t dimension) const noexcept;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

  //! Initialize the vulkan device
  void initData() override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  class Callbacks
  {
   public:
    //! Return the index of memory heap
    static bool getHeapNumber(const VulkanDevice& device,
                              const uint32b memory_type,
                              std::size_t* number) noexcept;

    //! Notify of a memory allocation in VMA
    static void notifyOfDeviceMemoryAllocation(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size,
        void* user_data);

    //! Notify of a memory freeing in VMA
    static void notifyOfDeviceMemoryFreeing(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size,
        void* user_data);
  };

  //! Add a shader module of the given kernel set
  void addShaderModule(const uint32b id,
                       const zisc::pmr::vector<uint32b>& spirv_code,
                       const std::string_view module_name);

  //! Calculate the dispatch size
  std::array<uint32b, 3> calcDispatchSize(
      const std::size_t work_dimension,
      const std::array<uint32b, 3>& work_size) const noexcept;

  //! Find the index of the optimal queue familty
  uint32b findQueueFamily() const noexcept;

  //! Get Vulkan function pointers used in VMA
  VmaVulkanFunctions getVmaVulkanFunctions() noexcept;

  //! Initialize a command pool
  void initCommandPool();

  //! Initialize a device
  void initDevice();

  //! Initialize the vulkan dispatch loader
  void initDispatcher();

  //! Initialize work group size of dimensions
  void initWorkGroupSizeDim() noexcept;

  //! Initialize a queue family index list
  void initQueueFamilyIndexList() noexcept;

  //! Initialize a vulkan memory allocator
  void initMemoryAllocator();

  //! Make a device memory allocation notifier
  VmaDeviceMemoryCallbacks makeAllocationNotifier() noexcept;

  //! Return the sub-platform
  VulkanSubPlatform& parentImpl() noexcept;

  //! Return the sub-platform
  const VulkanSubPlatform& parentImpl() const noexcept;

  //! Return an index of a queue family
  uint32b queueFamilyIndex() const noexcept;

  //! Update the given descriptor set with the given buffers
  void updateDescriptorSet(const VkDescriptorSet& descriptor_set,
                           const std::size_t n,
                           const VkBuffer* buffer_list,
                           const VkDescriptorType* desc_type_list,
                           VkDescriptorBufferInfo* desc_info_list,
                           VkWriteDescriptorSet* write_desc_list);

  //! Update the debug info of the shader module of the given ID
  void updateShaderModuleDebugInfo(const uint32b id) noexcept;


  VkDevice device_ = VK_NULL_HANDLE;
  VmaAllocator vm_allocator_ = VK_NULL_HANDLE;
  VkCommandPool command_pool_ = VK_NULL_HANDLE;
  zisc::pmr::unique_ptr<zisc::Bitset> fence_manager_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkFence>> fence_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkQueue>> queue_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<zisc::Memory::Usage>> heap_usage_list_;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  zisc::pmr::unique_ptr<zisc::pmr::map<uint32b, ModuleData>> shader_module_list_;
  uint32b queue_family_index_ = invalidQueueIndex();
  std::array<std::array<uint32b, 3>, 3> work_group_size_list_;
};

} // namespace zivc

#include "vulkan_device-inl.hpp"

#endif // ZIVC_VULKAN_DEVICE_HPP
