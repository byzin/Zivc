/*!
  \file vulkan_backend.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BACKEND_HPP
#define ZIVC_VULKAN_BACKEND_HPP

// Standard C++ library
#include <cstddef>
#include <map>
#include <memory>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/structure/map.hpp"
#include "zisc/structure/mutex_bst.hpp"
// Zivc
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan.hpp"
#include "zivc/backend.hpp"
#include "zivc/device.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/zivc_object.hpp"

namespace zivc {

// Forward declaration
class Context;
class ContextOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanBackend : public Backend
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


  //! Create an empty vulkan backend
  VulkanBackend(Context* context) noexcept;

  //! Finalize the backend
  ~VulkanBackend() noexcept override;


  //! Create a unique device
  [[nodiscard]]
  SharedDevice createDevice(const DeviceInfo& device_info) override;

  //! Return the underlyiing device info
  const DeviceInfo& deviceInfo(const std::size_t index) const noexcept override;

  //! Return the list of underlyiing device info
  std::span<const VulkanDeviceInfo> deviceInfoList() const noexcept;

  //! Return the device list
  std::span<const VkPhysicalDevice> deviceList() const noexcept;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return the engine name
  std::string_view engineName() const noexcept;

  //! Return the list of extension properties supported in the instance
  std::span<const VkExtensionProperties> extensionPropertiesList() const noexcept;

  //! Check if the backend has own VkInstance
  bool hasOwnInstance() const noexcept;

  //! Return the underlying Vulkan instance
  VkInstance& instance() noexcept;

  //! Return the underlying Vulkan instance
  const VkInstance& instance() const noexcept;

  //! Check if the backend is available
  bool isAvailable() const noexcept override;

  //! Return the list of layer properties supported in the instance
  std::span<const VkLayerProperties> layerPropertiesList() const noexcept;

  //! Create a host memory allocator for Vulkan object
  VkAllocationCallbacks createAllocator() noexcept;

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

  //! Return the backend type
  BackendType type() const noexcept override;

  //! Update the underlying device info
  void updateDeviceInfo() override;

  //! Return the window surface type activated
  WindowSurfaceType windowSurfaceType() const noexcept;

 protected:
  //! Destroy the backend
  void destroyData() noexcept override;

  //! Initialize the backend
  void initData(ContextOptions& options) override;

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
    using MemoryMapImpl = zisc::MutexBst<double>;
    using MemoryMap = zisc::Map<MemoryMapImpl, MemoryMapImpl::KeyT>;
    static_assert(MemoryMap::isConcurrent(), "The map must be concurrent.");


    //! Initialize the allocator data
    AllocatorData(zisc::pmr::memory_resource* mem_resource) noexcept;

    //! Finalize the allocator data
    ~AllocatorData() noexcept;


    //! Return the underlying memory list
    std::span<MemoryData> memoryList() noexcept;

    //! Return the underlying memory list
    std::span<const MemoryData> memoryList() const noexcept;

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

  //! Create an application info
  VkApplicationInfo createApplicationInfo(
      const std::string_view app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) const noexcept;

  //! Initialize an allocator which used in the backend
  void initAllocator() noexcept;

  //! Make a vulkan instance
  void initInstance(ContextOptions& options);

  //! Initialize the physical device list
  void initDeviceList();

  //! Initialize the device info list
  void initDeviceInfoList() noexcept;

  //! Initialize the vulkan dispatch loader
  void initDispatcher(ContextOptions& options);

  //! Initialize the backend properties
  void initProperties();

  //! Initialize window surface
  void initWindowSurface(const ContextOptions& options,
                         zisc::pmr::vector<const char*>* extension_list);

  //! Check if the given extension is supported in the instance
  bool isExtensionSupported(const std::string_view name) const noexcept;

  //! Check if the given layer is supported in the instance
  bool isLayerSupported(const std::string_view name) const noexcept;


  VkInstance instance_ = ZIVC_VK_NULL_HANDLE;
  std::add_pointer_t<VkInstance> instance_ref_ = nullptr;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  zisc::pmr::unique_ptr<AllocatorData> allocator_data_;
  ZivcObject::UniqueVectorT<VkExtensionProperties> extension_properties_list_;
  ZivcObject::UniqueVectorT<VkLayerProperties> layer_properties_list_;
  ZivcObject::UniqueVectorT<VkPhysicalDevice> device_list_;
  ZivcObject::UniqueVectorT<VulkanDeviceInfo> device_info_list_;
  WindowSurfaceType window_surface_type_ = WindowSurfaceType::kNone;
  [[maybe_unused]] Padding<4> pad_;
  char engine_name_[32] = "Zivc";
};

} // namespace zivc

#include "vulkan_backend-inl.hpp"

#endif // ZIVC_VULKAN_BACKEND_HPP
