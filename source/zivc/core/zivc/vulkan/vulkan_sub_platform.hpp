/*!
  \file vulkan_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_SUB_PLATFORM_HPP
#define ZIVC_VULKAN_SUB_PLATFORM_HPP

// Standard C++ library
#include <cstddef>
#include <map>
#include <memory>
#include <mutex>
#include <string_view>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/data_structure/bounded_bst.hpp"
#include "zisc/data_structure/mutex_bst.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/device.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class Platform;
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanSubPlatform : public SubPlatform
{
 public:
  /*!
    \brief No brief description

    No detailed description.
    */
  enum class WindowSurfaceType : uint32b
  {
    kNone = 0,
    kWin32,
    kMacOS,
    kMetal,
    kXlib,
    kXcb,
    kWayland
  };


  //! Create an empty vulkan sub-platform
  VulkanSubPlatform(Platform* platform) noexcept;

  //! Finalize the sub-platform
  ~VulkanSubPlatform() noexcept override;


  //! Return the device list
  const zisc::pmr::vector<VkPhysicalDevice>& deviceList() const noexcept;

  //! Return the device info list
  const zisc::pmr::vector<VulkanDeviceInfo>& deviceInfoList() const noexcept;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return the engine name
  std::string_view engineName() const noexcept;

  //! Return the list of extension properties supported in the instance
  const zisc::pmr::vector<VkExtensionProperties>& extensionPropertiesList() const noexcept;

  //! Add the underlying device info into the given list
  void getDeviceInfoList(zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept override;

  //! Check if the sub-platform has own VkInstance
  bool hasOwnInstance() const noexcept;

  //! Return the underlying Vulkan instance
  VkInstance& instance() noexcept;

  //! Return the underlying Vulkan instance
  const VkInstance& instance() const noexcept;

  //! Check if the sub-platform is available
  bool isAvailable() const noexcept override;

  //! Return the list of layer properties supported in the instance
  const zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() const noexcept;

  //! Make a host memory allocator for Vulkan object
  VkAllocationCallbacks makeAllocator() noexcept;

  //! Make a unique device
  [[nodiscard]]
  SharedDevice makeDevice(const DeviceInfo& device_info) override;

  //! Make a unique device
  [[nodiscard]]
  SharedDevice makeDevice(const DeviceInfo& device_info,
                          const VulkanDeviceCapability capability);

  //! Notify of device memory allocation
  void notifyOfDeviceMemoryAllocation(const std::size_t device_index,
                                      const std::size_t heap_index,
                                      const std::size_t size) noexcept;

  //! Notify of device memory deallocation
  void notifyOfDeviceMemoryDeallocation(const std::size_t device_index,
                                        const std::size_t heap_index,
                                        const std::size_t size) noexcept;

  //! Return the number of available devices
  std::size_t numOfDevices() const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Update the device info list
  void updateDeviceInfoList() override;

  //! Return the window surface type activated
  WindowSurfaceType windowSurfaceType() const noexcept;

 protected:
  //! Destroy the sub-platform
  void destroyData() noexcept override;

  //! Initialize the sub-platform
  void initData(PlatformOptions& options) override;

  //! Update the debug info
  void updateDebugInfoImpl() override;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  struct MemoryData
  {
    size_t size_ = 0;
    size_t alignment_ = 0;
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  class AllocatorData : private zisc::NonCopyable<AllocatorData>
  {
   public:
    using MemoryMapImpl = zisc::MutexBst;
    using MemoryMap = zisc::BoundedBst<MemoryMapImpl>;


    //! Initialize the allocator data
    AllocatorData(zisc::pmr::memory_resource* mem_resource) noexcept;

    //! Finalize the allocator data
    ~AllocatorData() noexcept;


    //! Return the underlying memory list
    zisc::pmr::vector<MemoryData>& memoryList() noexcept;

    //! Return the underlying memory list
    const zisc::pmr::vector<MemoryData>& memoryList() const noexcept;

    //! Return the underlying memory map
    MemoryMap& memoryMap() noexcept;

    //! Return the underlying memory resource
    zisc::pmr::memory_resource* memoryResource() noexcept;

   private:
    //! Initialize the data
    void initialize();

    //! Return the maximum possible capacity of the memory map
    static constexpr std::size_t mapCapacity() noexcept;


    zisc::pmr::memory_resource* mem_resource_ = nullptr;
    MemoryMapImpl mem_map_;
    zisc::pmr::vector<MemoryData> mem_list_;
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  class Callbacks
  {
   public:
    // Type aliases
    using DebugMessengerReturnType = std::invoke_result_t<
        PFN_vkDebugUtilsMessengerCallbackEXT,
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        const VkDebugUtilsMessengerCallbackDataEXT*,
        void*>;
    using AllocationReturnType = std::invoke_result_t<
        PFN_vkAllocationFunction,
        void*,
        size_t,
        size_t,
        VkSystemAllocationScope>;
    using ReallocationReturnType = std::invoke_result_t<
        PFN_vkReallocationFunction,
        void*,
        void*,
        size_t,
        size_t,
        VkSystemAllocationScope>;


    //! Allocate a memory block
    static AllocationReturnType allocateMemory(
        void* user_data,
        size_t size,
        size_t alignment,
        VkSystemAllocationScope scope);

    //! Deallocate a memory block
    static void deallocateMemory(
        void* user_data,
        void* memory);

    //! Notify of a memory allocation
    static void notifyOfMemoryAllocation(
        void* user_data,
        size_t size,
        VkInternalAllocationType type,
        VkSystemAllocationScope scope);

    //! Notify of a memory deallocation
    static void notifyOfMemoryDeallocation(
        void* user_data,
        size_t size,
        VkInternalAllocationType type,
        VkSystemAllocationScope scope);

    //! Print a debug message
    static DebugMessengerReturnType printDebugMessage(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity_flags,
        VkDebugUtilsMessageTypeFlagsEXT message_types,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data);

    //! Reallocate a memory block
    static ReallocationReturnType reallocateMemory(
        void* user_data,
        void* original_memory,
        size_t,
        size_t,
        VkSystemAllocationScope);
  };

  //! Compare two properties by it's name
  static bool compareProperties(const std::string_view lhs,
                                const std::string_view rhs) noexcept;

  //! Make an application info
  VkApplicationInfo makeApplicationInfo(
      const std::string_view app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) const noexcept;

  //! Initialize an allocator which used in the sub-platform
  void initAllocator() noexcept;

  //! Make a vulkan instance
  void initInstance(PlatformOptions& options);

  //! Initialize the physical device list
  void initDeviceList();

  //! Initialize the device info list
  void initDeviceInfoList() noexcept;

  //! Initialize the vulkan dispatch loader
  void initDispatcher(PlatformOptions& options);

  //! Initialize the sub-platform properties
  void initProperties();

  //! Initialize window surface
  void initWindowSurface(const PlatformOptions& options,
                         zisc::pmr::vector<const char*>* extension_list);

  //! Check if the given extension is supported in the instance
  bool isExtensionSupported(const std::string_view name) const noexcept;

  //! Check if the given layer is supported in the instance
  bool isLayerSupported(const std::string_view name) const noexcept;


  VkInstance instance_ = ZIVC_VK_NULL_HANDLE;
  std::add_pointer_t<VkInstance> instance_ref_ = nullptr;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  zisc::pmr::unique_ptr<AllocatorData> allocator_data_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkExtensionProperties>> extension_properties_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkLayerProperties>> layer_properties_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkPhysicalDevice>> device_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VulkanDeviceInfo>> device_info_list_;
  WindowSurfaceType window_surface_type_ = WindowSurfaceType::kNone;
  [[maybe_unused]] Padding<4> pad_;
  char engine_name_[32] = "Zivc";
};

} // namespace zivc

#include "vulkan_sub_platform-inl.hpp"

#endif // ZIVC_VULKAN_SUB_PLATFORM_HPP
