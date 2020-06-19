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
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/memory.hpp"
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/buffer.hpp"
#include "zivc/device.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class VulkanDeviceInfo;
class VulkanSubPlatform;
template <typename SetType, typename ...ArgTypes> class KernelParameters;
template <typename SetType> class KernelSet;

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

//  //! Allocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void allocate(const std::size_t size,
//                VulkanBuffer<kDescriptor, Type>* buffer) noexcept;

//  //! Return the workgroup size for the work dimension
//  template <std::size_t kDimension>
//  std::array<uint32b, 3> calcWorkGroupSize(
//      const std::array<uint32b, kDimension>& works) const noexcept;

  //! Create a Vulkan kernel
//  void createKernel(const std::size_t num_of_buffers,
//                    VkDescriptorSetLayout descriptor_set_layout,
//                    VkDescriptorPool* descriptor_pool,
//                    VkDescriptorSet* descriptor_set,
//                    VkPipelineLayout* pipeline_layout,
//                    VkPipeline* compute_pipeline,
//                    VkCommandBuffer* command_buffer);

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

  //! Return the shader module by the the given kernel set ID
  const ModuleData& getShaderModule(const uint32b id) const noexcept;

  //! Check if the device has the shader module of the given kernel set ID
  bool hasShaderModule(const uint32b id) const noexcept;

  //! Return the invalid queue index in queue families
  static constexpr uint32b invalidQueueIndex() noexcept;

//  //! Return the local-work size for the work dimension
//  template <std::size_t kDimension>
//  const std::array<uint32b, 3>& localWorkSize() const noexcept;

  //! Make a buffer
  template <typename Type>
  SharedBuffer<Type> makeBuffer(const BufferUsage flag);

  //! Make a kernel
  template <std::size_t kDimension, typename SetType, typename ...ArgTypes>
  SharedKernel<kDimension, SetType, ArgTypes...> makeKernel(
      const KernelParameters<SetType, ArgTypes...>& parameters);

  //! Return the memory allocator of the device
  VmaAllocator& memoryAllocator() noexcept;

  //! Return the memory allocator of the device
  const VmaAllocator& memoryAllocator() const noexcept;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the peak memory usage of the heap of the given number
  std::size_t peakMemoryUsage(const std::size_t number) const noexcept override;

  //! Set the debug info
  void setDebugInfo(const VkObjectType object_type,
                    const uint64b object_handle,
                    const std::string_view object_name,
                    const ZivcObject* zivc_object) noexcept;

  //! Return the current memory usage of the heap of the given number
  std::size_t totalMemoryUsage(const std::size_t number) const noexcept override;

  //! Submit a command
//  void submit(const QueueType queue_type,
//              const uint32b queue_index,
//              const vk::CommandBuffer& command) const noexcept;

//  //! Wait this thread until all commands in the device are completed
//  void waitForCompletion() const noexcept override;
//
//  //! Wait this thread until all commands in the queues are completed
//  void waitForCompletion(const QueueType queue_type) const noexcept override;
//
//  //! Wait this thread until all commands in the queue are completed
//  void waitForCompletion(const QueueType queue_type,
//                         const uint32b queue_index) const noexcept override;

  // For buffer

  //! Allocate a device memory
  void allocateMemory(const std::size_t size,
                      const BufferUsage buffer_usage,
                      void* user_data,
                      VkBuffer* buffer,
                      VmaAllocation* vm_allocation,
                      VmaAllocationInfo* alloc_info);

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) noexcept;

  // For kernel

  //! Destroy descriptor set
  void destroyKernelDescriptorSet(VkDescriptorSetLayout* descriptor_set_layout,
                                  VkDescriptorPool* descriptor_pool) noexcept;

  //! Destroy pipeline
  void destroyKernelPipeline(VkPipelineLayout* pipeline_layout,
                             VkPipeline* compute_pipeline) noexcept;

  //! Dispatch the given kernel to the vulkan device
  void dispatchCmd(const VkCommandBuffer& command_buffer,
                   const VkDescriptorSet& descriptor_set,
                   const VkPipelineLayout& pipeline_layout,
                   const VkPipeline& pipeline,
                   const uint32b queue_index,
                   const std::array<uint32b, 3>& num_of_work_groups);

  //! Initialize a kernel command buffer
  void initKernelCommandBuffer(VkCommandBuffer* command_buffer);

  //! Initialize a descriptor set of a kernel
  void initKernelDescriptorSet(const std::size_t num_of_buffers,
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

  //! Update a buffer's contents from host memory
  template <typename Type>
  void updateBufferCmd(const VkCommandBuffer& command_buffer,
                       const Type& source,
                       const VkBuffer& dest);

  //! Update a buffer's contents from host memory
  void updateBufferCmd(const VkCommandBuffer& command_buffer,
                       const VkDeviceSize size,
                       const void* source,
                       const VkDeviceSize offset,
                       const VkBuffer& dest);

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

  //! Find the index of the optimal queue familty
  uint32b findQueueFamily() const noexcept;

  //! Get Vulkan function pointers used in VMA
  VmaVulkanFunctions getVmaVulkanFunctions() noexcept;

//  //! Return a queue
//  vk::Queue getQueue(const QueueType queue_type,
//                     const uint32b queue_index) const noexcept;

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

  //! Update the debug info of the shader module of the given ID
  void updateShaderModuleDebugInfo(const uint32b id) noexcept;


  VkDevice device_ = VK_NULL_HANDLE;
  VmaAllocator vm_allocator_ = VK_NULL_HANDLE;
  VkCommandPool command_pool_ = VK_NULL_HANDLE;
  zisc::pmr::unique_ptr<zisc::pmr::vector<zisc::Memory::Usage>> heap_usage_list_;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  zisc::pmr::unique_ptr<zisc::pmr::map<uint32b, ModuleData>> shader_module_list_;
  uint32b queue_family_index_ = invalidQueueIndex();
  std::array<std::array<uint32b, 3>, 3> work_group_size_list_;
};

} // namespace zivc

#include "vulkan_device-inl.hpp"

#endif // ZIVC_VULKAN_DEVICE_HPP
