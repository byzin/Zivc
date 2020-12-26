/*!
  \file vulkan_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_sub_platform.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "vulkan_device_info.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zivc/device.hpp"
#include "zivc/platform.hpp"
#include "zivc/platform_options.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
VulkanSubPlatform::VulkanSubPlatform(Platform* platform) noexcept :
    SubPlatform(platform)
{
}

/*!
  \details No detailed description
  */
VulkanSubPlatform::~VulkanSubPlatform() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in,out] device_info_list No description.
  */
void VulkanSubPlatform::getDeviceInfoList(
    zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept
{
  for (const auto& device_info : *device_info_list_)
    device_info_list.emplace_back(std::addressof(device_info));
}

/*!
  \details No detailed description

  \return No description
  */
bool VulkanSubPlatform::isAvailable() const noexcept
{
  const auto& own = getOwn();
  const bool result = !own.expired() && (instance_ref_ != nullptr);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
VkAllocationCallbacks VulkanSubPlatform::makeAllocator() noexcept
{
  zivcvk::AllocationCallbacks alloc{allocator_data_.get(),
                                    Callbacks::allocateMemory,
                                    Callbacks::reallocateMemory,
                                    Callbacks::freeMemory,
                                    Callbacks::notifyOfMemoryAllocation,
                                    Callbacks::notifyOfMemoryFreeing};
  return zisc::cast<VkAllocationCallbacks>(alloc);
}

/*!
  \details No detailed description

  \param [in] device_info No description.
  \return No description
  */
SharedDevice VulkanSubPlatform::makeDevice(const DeviceInfo& device_info)
{
  if (!isAvailable()) {
    //! \todo Throw an exception
    std::cerr << "[Error] Submodule isn't ready." << std::endl;
  }

  const auto& info_list = deviceInfoList();
  const auto pred = [&device_info](const DeviceInfo& info) noexcept
  {
    const bool result = std::addressof(device_info) == std::addressof(info);
    return result;
  };
  auto it = std::find_if(info_list.begin(), info_list.end(), pred);
  if (it == info_list.end()) {
    //! \todo Throw an exception
    std::cerr << "[Error] Invalid device info is passed." << std::endl;
  }
  zisc::pmr::polymorphic_allocator<VulkanDevice> alloc{memoryResource()};
  SharedDevice device = std::allocate_shared<VulkanDevice>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakDevice own{device};
  device->initialize(std::move(parent), std::move(own), device_info);

  return device;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanSubPlatform::numOfDevices() const noexcept
{
  const std::size_t n = device_list_->size();
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType VulkanSubPlatform::type() const noexcept
{
  return SubPlatformType::kVulkan;
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::updateDeviceInfoList() noexcept
{
  device_info_list_->clear();
  device_info_list_->reserve(numOfDevices());
  for (const auto& device : deviceList()) {
    VulkanDeviceInfo info{memoryResource()};
    info.fetch(device, dispatcher());
    device_info_list_->emplace_back(std::move(info));
  }
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::destroyData() noexcept
{
  device_info_list_.reset();
  device_list_.reset();

  zivcvk::Instance ins{instance_};
  if (ins) {
    zivcvk::AllocationCallbacks alloc{makeAllocator()};
    const auto loader = dispatcher().loaderImpl();
    ins.destroy(alloc, *loader);
    instance_ = VK_NULL_HANDLE;
  }

  allocator_data_.reset();
  dispatcher_.reset();
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void VulkanSubPlatform::initData(PlatformOptions& platform_options)
{
  initDispatcher(platform_options);
  if (!dispatcher().isAvailable()) {
    if (isDebugMode())
      std::cerr << "[Warning] Loading Vulkan functions failed." << std::endl;
    return;
  }
  initAllocator();
  initInstance(platform_options);
  dispatcher_->set(instance());
  initDeviceList();
  initDeviceInfoList();
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::updateDebugInfoImpl() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \param [in] mem_map No description.
  */
VulkanSubPlatform::AllocatorData::AllocatorData(
    zisc::pmr::memory_resource* mem_resource,
    MemoryMap&& mem_map) noexcept :
        mem_resource_{mem_resource},
        mem_map_{std::move(mem_map)}
{
}

/*!
  \details No detailed description
  */
VulkanSubPlatform::AllocatorData::~AllocatorData() noexcept
{
  ZISC_ASSERT(mem_map_.size() == 0, "There are memories which aren't deallocated.");
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] alignment No description.
  \param [in] scope No description.
  \return No description
  */
auto VulkanSubPlatform::Callbacks::allocateMemory(
    void* user_data,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope scope) -> AllocationReturnType
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  static_cast<void>(scope);
  auto alloc_data = zisc::cast<AllocatorData*>(user_data);
  void* memory = alloc_data->mem_resource_->allocate(size, alignment);
  //
  const std::size_t address = zisc::reinterp<std::size_t>(memory);
  const auto mem_data = std::make_pair(size, alignment);
  //! \todo Thread safe
  alloc_data->mem_map_.emplace(std::make_pair(address, mem_data));
  return memory;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in,out] memory No description.
  */
void VulkanSubPlatform::Callbacks::freeMemory(
    void* user_data,
    void* memory)
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  auto alloc_data = zisc::cast<AllocatorData*>(user_data);
  if (memory) {
    const std::size_t address = zisc::reinterp<std::size_t>(memory);
    const auto mem = alloc_data->mem_map_.find(address);
    ZISC_ASSERT(mem != alloc_data->mem_map_.end(), "The mem is null.");
    const auto& data = mem->second;
    alloc_data->mem_resource_->deallocate(memory, data.first, data.second);
  //! \todo Thread safe
    alloc_data->mem_map_.erase(mem);
  }
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] type No description.
  \param [in] scope No description.
  */
void VulkanSubPlatform::Callbacks::notifyOfMemoryAllocation(
    void* user_data,
    size_t size,
    VkInternalAllocationType type,
    VkSystemAllocationScope scope)
{
  static_cast<void>(user_data);
  static_cast<void>(size);
  static_cast<void>(type);
  static_cast<void>(scope);
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] type No description.
  \param [in] scope No description.
  */
void VulkanSubPlatform::Callbacks::notifyOfMemoryFreeing(
    void* user_data,
    size_t size,
    VkInternalAllocationType type,
    VkSystemAllocationScope scope)
{
  static_cast<void>(user_data);
  static_cast<void>(size);
  static_cast<void>(type);
  static_cast<void>(scope);
}

/*!
  \details No detailed description

  \param [in] severity_flags No description.
  \param [in] message_types No description.
  \param [in] callback_data No description.
  \param [in] user_data No description.
  \return No description
  */
auto VulkanSubPlatform::Callbacks::printDebugMessage(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity_flags,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data) -> DebugMessengerReturnType
{
  constexpr std::size_t max_message_length = 2048;
  char message[max_message_length];
  bool is_error = false;

  if (user_data) {
    char msg[max_message_length];
    const IdData* data = zisc::cast<const IdData*>(user_data);
    std::sprintf(msg, "ID[%ld] -", data->id());
    if (data->hasName()) {
      std::sprintf(msg, "%s Name '%s'", msg, data->name().data());
    }
    if (data->hasFileInfo()) {
      const std::string_view file_name = data->fileName();
      const int64b line = data->lineNumber();
      std::sprintf(msg, "%s at line %ld in '%s'", msg, line, file_name.data());
    }
    std::strcat(msg, "\n");
    std::strcat(message, msg);
  }

  const char indent[] = "          ";
  {
    char prefix[64] = "";
    if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
      std::strcpy(prefix, "VERBOSE : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      std::strcpy(prefix, "INFO    : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      std::strcpy(prefix, "WARNING : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      std::strcpy(prefix, "ERROR   : ");
      is_error = true;
    }

    if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
      std::strcat(prefix, "GENERAL");
    }
    else if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
      std::strcat(prefix, "VALIDATION");
    }
    else if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
      std::strcat(prefix, "PERF");
    }

    char msg[max_message_length];
    std::sprintf(msg, "%s - Message ID Number %d, Message ID Name %s : %s\n",
        prefix,
        callback_data->messageIdNumber,
        callback_data->pMessageIdName,
        callback_data->pMessage);
    std::strcat(message, msg);
  }

  if (0 < callback_data->queueLabelCount) {
    char msg[max_message_length];
    std::sprintf(msg, "\n%sQueue Labels - %d\n",
        indent,
        zisc::cast<int>(callback_data->queueLabelCount));
    for (std::size_t id = 0; id < callback_data->queueLabelCount; ++id) {
      const auto& label = callback_data->pQueueLabels[id];
      std::sprintf(msg, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          msg,
          indent,
          zisc::cast<int>(id),
          label.pLabelName,
          zisc::cast<double>(label.color[0]),
          zisc::cast<double>(label.color[1]),
          zisc::cast<double>(label.color[2]),
          zisc::cast<double>(label.color[3]));
    }
    std::strcat(message, msg);
  }

  if (0 < callback_data->cmdBufLabelCount) {
    char msg[max_message_length];
    std::sprintf(msg, "\n%sCommand Buffer Labels - %d\n",
        indent,
        zisc::cast<int>(callback_data->cmdBufLabelCount));
    for (std::size_t id = 0; id < callback_data->cmdBufLabelCount; ++id) {
      const auto& label = callback_data->pCmdBufLabels[id];
      std::sprintf(msg, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          msg,
          indent,
          zisc::cast<int>(id),
          label.pLabelName,
          zisc::cast<double>(label.color[0]),
          zisc::cast<double>(label.color[1]),
          zisc::cast<double>(label.color[2]),
          zisc::cast<double>(label.color[3]));
    }
    std::strcat(message, msg);
  }

  if (0 < callback_data->objectCount) {
    char msg[max_message_length];
    std::sprintf(msg, "\n%sObjects - %d\n",
        indent,
        zisc::cast<int>(callback_data->objectCount));
    for (std::size_t id = 0; id < callback_data->objectCount; ++id) {
      const auto& object = callback_data->pObjects[id];
      const auto obj_type_name = zivcvk::to_string(
          zisc::cast<zivcvk::ObjectType>(object.objectType));
      std::sprintf(msg, "%s%s  * Object[%d] - Type '%s', Value %lu, Name '%s'\n",
          msg,
          indent,
          zisc::cast<int>(id),
          obj_type_name.c_str(),
          object.objectHandle,
          object.pObjectName);
    }
    std::strcat(message, msg);
  }

  std::strcat(message, "\n");
  std::ostream* output = (is_error) ? &std::cerr : &std::cout;
  (*output) << message;

  return VK_FALSE;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in,out] original_memory No description.
  \param [in] size No description.
  \param [in] alignment No description.
  \param [in] scope No description.
  \return No description
  */
auto VulkanSubPlatform::Callbacks::reallocateMemory(
    void* user_data,
    void* original_memory,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope scope) -> ReallocationReturnType
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  // Allocate new memory block
  void* memory = nullptr;
  if (0 < size)
    memory = allocateMemory(user_data, size, alignment, scope);
  // Copy data
  if (original_memory && memory) {
    auto alloc_data = zisc::cast<AllocatorData*>(user_data);
    const std::size_t address = zisc::reinterp<std::size_t>(original_memory);
    //! \todo Thread safe
    const auto mem = alloc_data->mem_map_.find(address);
    ZISC_ASSERT(mem != alloc_data->mem_map_.end(), "The mem is null.");
    const auto& data = mem->second;
    std::memcpy(memory, original_memory, data.first);
  }
  // Deallocate the original memory
  if (original_memory)
    freeMemory(user_data, original_memory);
  return memory;
}

/*!
  \details No detailed description

  \param [in] app_name No description.
  \param [in] app_version_major No description.
  \param [in] app_version_minor No description.
  \param [in] app_version_patch No description.
  \return No description
  */
VkApplicationInfo VulkanSubPlatform::makeApplicationInfo(
    const std::string_view app_name,
    const uint32b app_version_major,
    const uint32b app_version_minor,
    const uint32b app_version_patch) const noexcept
{
  const uint32b app_version = VK_MAKE_VERSION(app_version_major,
                                              app_version_minor,
                                              app_version_patch);
  std::string_view engine_name = engineName();
  constexpr uint32b engine_version = VK_MAKE_VERSION(Config::versionMajor(),
                                                     Config::versionMinor(),
                                                     Config::versionPatch());
  constexpr uint32b api_version = apiVersion();
  const zivcvk::ApplicationInfo data{app_name.data(),
                                     app_version,
                                     engine_name.data(),
                                     engine_version,
                                     api_version};
  return zisc::cast<VkApplicationInfo>(data);
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initAllocator() noexcept
{
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<AllocatorData> alloc{mem_resource};
  allocator_data_ = zisc::pmr::allocateUnique<AllocatorData>(
      alloc,
      mem_resource,
      MemoryMap{MemoryMap::allocator_type{mem_resource}});
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void VulkanSubPlatform::initInstance(PlatformOptions& platform_options)
{
  using InstancePtr = std::add_pointer_t<VkInstance>;
  auto ptr = zisc::cast<InstancePtr>(platform_options.vulkanInstancePtr());
  if (ptr) {
    // Use the given instance instead of allocating new instance
    instance_ = VK_NULL_HANDLE;
    instance_ref_ = ptr;
    return;
  }

  zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

//  extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  if (isDebugMode()) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // Debug utils extension
  const auto severity_flags =
//      zivcvk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
      zivcvk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      zivcvk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  const auto message_type_flags =
      zivcvk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      zivcvk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      zivcvk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  PFN_vkDebugUtilsMessengerCallbackEXT callback = Callbacks::printDebugMessage;
  zivcvk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info{
      zivcvk::DebugUtilsMessengerCreateFlagsEXT{},
      severity_flags,
      message_type_flags,
      callback};

  // Validation features
  std::array<zivcvk::ValidationFeatureEnableEXT, 4> validation_features_list{{
      zivcvk::ValidationFeatureEnableEXT::eGpuAssisted,
      zivcvk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot,
      zivcvk::ValidationFeatureEnableEXT::eBestPractices,
      zivcvk::ValidationFeatureEnableEXT::eSynchronizationValidation}};
  zivcvk::ValidationFeaturesEXT validation_features{
      zisc::cast<uint32b>(validation_features_list.size()),
      validation_features_list.data(),
      0,
      nullptr};

  const zivcvk::ApplicationInfo app_info{makeApplicationInfo(
      platform_options.platformName(),
      platform_options.platformVersionMajor(),
      platform_options.platformVersionMinor(),
      platform_options.platformVersionPatch())};
  zivcvk::InstanceCreateInfo create_info{zivcvk::InstanceCreateFlags{},
                                         std::addressof(app_info),
                                         zisc::cast<uint32b>(layers.size()),
                                         layers.data(),
                                         zisc::cast<uint32b>(extensions.size()),
                                         extensions.data()};
  if (isDebugMode()) {
    create_info.setPNext(std::addressof(debug_utils_create_info));
    debug_utils_create_info.setPNext(std::addressof(validation_features));
  }

  zivcvk::AllocationCallbacks alloc{makeAllocator()};
  const auto loader = dispatcher().loaderImpl();
  zivcvk::Instance ins = zivcvk::createInstance(create_info, alloc, *loader);
  instance_ = zisc::cast<VkInstance>(ins);
  instance_ref_ = std::addressof(instance_);
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceList()
{
  const zivcvk::Instance ins{instance()};

  using DeviceList = zisc::pmr::vector<zivcvk::PhysicalDevice>;
  DeviceList::allocator_type alloc{memoryResource()};
  const auto loader = dispatcher().loaderImpl();
  auto device_list = ins.enumeratePhysicalDevices(alloc, *loader);

  using DstDeviceList = decltype(device_list_)::element_type;
  auto list_ptr = zisc::reinterp<DstDeviceList*>(std::addressof(device_list));
  device_list_ = zisc::pmr::allocateUnique<DstDeviceList>(
      memoryResource(),
      std::move(*list_ptr));
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceInfoList() noexcept
{
  auto mem_resource = memoryResource();
  using DeviceInfoList = decltype(device_info_list_)::element_type;
  DeviceInfoList info_list{DeviceInfoList::allocator_type{mem_resource}};
  zisc::pmr::polymorphic_allocator<DeviceInfoList> alloc{mem_resource};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      alloc,
      std::move(info_list));
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDispatcher(PlatformOptions& platform_options)
{
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<VulkanDispatchLoader> alloc{mem_resource};

  using FuncPtr = std::add_pointer_t<PFN_vkGetInstanceProcAddr>;
  auto ptr = zisc::cast<FuncPtr>(platform_options.vulkanGetProcAddrPtr());
  dispatcher_ = (ptr) // Use the given loader instead of allocating new one
      ? zisc::pmr::allocateUnique<VulkanDispatchLoader>(alloc, mem_resource, *ptr)
      : zisc::pmr::allocateUnique<VulkanDispatchLoader>(alloc, mem_resource);
}

} // namespace zivc
