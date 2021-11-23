/*!
  \file vulkan_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_sub_platform.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_hpp.hpp"
#include "zivc/device.hpp"
#include "zivc/platform.hpp"
#include "zivc/platform_options.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/env_variable.hpp"
#include "zivc/utility/error.hpp"

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
  const auto& own = getOwnPtr();
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
                                    Callbacks::deallocateMemory,
                                    Callbacks::notifyOfMemoryAllocation,
                                    Callbacks::notifyOfMemoryDeallocation};
  return zisc::cast<VkAllocationCallbacks>(alloc);
}

/*!
  \details No detailed description

  \param [in] device_info No description.
  \return No description
  */
SharedDevice VulkanSubPlatform::makeDevice(const DeviceInfo& device_info)
{
  return makeDevice(device_info, VulkanDeviceCapability::kCompute);
}

/*!
  \details No detailed description

  \param [in] device_info No description.
  \return No description
  */
SharedDevice VulkanSubPlatform::makeDevice(const DeviceInfo& device_info,
                                           const VulkanDeviceCapability capability)
{
  // Check if the given device info is included in the info list
  {
    const auto& info_list = deviceInfoList();
    const auto pred = [&device_info](const DeviceInfo& info) noexcept
    {
      const bool result = std::addressof(device_info) == std::addressof(info);
      return result;
    };
    auto it = std::find_if(info_list.begin(), info_list.end(), pred);
    if (it == info_list.end()) {
      const char* message = "Invalid vulkan device info is passed.";
      throw SystemError{ErrorCode::kInitializationFailed, message};
    }
  }

  zisc::pmr::polymorphic_allocator<VulkanDevice> alloc{memoryResource()};
  SharedDevice device = std::allocate_shared<VulkanDevice>(alloc,
                                                           capability,
                                                           issueId());

  ZivcObject::SharedPtr parent{getOwnPtr()};
  WeakDevice own{device};
  try {
    device->initialize(std::move(parent), std::move(own), device_info);
  }
  catch (const std::runtime_error& error) {
    device->destroy();
    device.reset();
    throw error;
  }

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
void VulkanSubPlatform::updateDeviceInfoList()
{
  device_info_list_->clear();
  device_info_list_->reserve(numOfDevices());
  const auto& device_list = deviceList();
  for (std::size_t i = 0; i < device_list.size(); ++i) {
    const VkPhysicalDevice& device = device_list[i];
    VulkanDeviceInfo info{memoryResource()};
    info.fetch(device, dispatcher());
    info.setDeviceIndex(i);
    device_info_list_->emplace_back(std::move(info));
  }
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::destroyData() noexcept
{
  window_surface_type_ = WindowSurfaceType::kNone;
  device_info_list_.reset();
  device_list_.reset();
  layer_properties_list_.reset();
  extension_properties_list_.reset();

  zivcvk::Instance ins{instance_};
  if (ins) {
    zivcvk::AllocationCallbacks alloc{makeAllocator()};
    ins.destroy(alloc, dispatcher().loader());
    instance_ = ZIVC_VK_NULL_HANDLE;
  }

  allocator_data_.reset();
  dispatcher_.reset();
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void VulkanSubPlatform::initData(PlatformOptions& options)
{
  initDispatcher(options);
  initAllocator();
  initProperties();
  initInstance(options);
  initDeviceList();
  initDeviceInfoList();
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::updateDebugInfoImpl()
{
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanSubPlatform::AllocatorData::AllocatorData(
    zisc::pmr::memory_resource* mem_resource) noexcept :
        mem_resource_{mem_resource},
        mem_map_{mem_resource},
        mem_list_{decltype(mem_list_)::allocator_type{mem_resource}}
{
  initialize();
}

/*!
  \details No detailed description
  */
VulkanSubPlatform::AllocatorData::~AllocatorData() noexcept
{
  //! \todo check memory leak
  auto& mem_map = memoryMap();
  if (0 < mem_map.size()) {
//    zisc::MebiUnit total{0};
//    for (const auto& mem_data : mem_map_)
//      total = total + zisc::ByteUnit{zisc::cast<int64b>(mem_data.second.size_)};
//    std::cerr << "[Warning] There are memories which aren't deallocated in Vulkan: "
//              << zisc::cast<double>(total.value())
//              << " MB." << std::endl;
    std::cerr << "[Warning] Memory leak found." << std::endl;
  }
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::AllocatorData::initialize()
{
  auto& mem_map = memoryMap();
  mem_map.setCapacity(mapCapacity());
  mem_map.clear();
  mem_list_.resize(mapCapacity());
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
    [[maybe_unused]] VkSystemAllocationScope scope) -> AllocationReturnType
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  auto* alloc_data = zisc::cast<AllocatorData*>(user_data);
  void* memory = alloc_data->memoryResource()->allocate(size, alignment);

  // Store the allocation info
  static_assert(sizeof(void*) == sizeof(double));
  const auto address = zisc::bit_cast<double>(memory);
  ZISC_ASSERT(std::isfinite(address), "The address float isn't finite.");
  auto& mem_map = alloc_data->memoryMap();
  {
    const auto [result, index] = mem_map.add(address);
    ZISC_ASSERT(result, "Adding the address into the map failed.");
    auto& mem_list = alloc_data->memoryList();
    const MemoryData data{size, alignment};
    mem_list[index] = data;
  }

  return memory;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in,out] memory No description.
  */
void VulkanSubPlatform::Callbacks::deallocateMemory(
    void* user_data,
    void* memory)
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  if (memory != nullptr) {
    auto* alloc_data = zisc::cast<AllocatorData*>(user_data);

    static_assert(sizeof(void*) == sizeof(double));
    const auto address = zisc::bit_cast<double>(memory);
    ZISC_ASSERT(std::isfinite(address), "The address float isn't finite.");
    auto& mem_map = alloc_data->memoryMap();
    MemoryData data;
    {
      const auto [result, index] = mem_map.contain(address);
      ZISC_ASSERT(result, "The address isn't in the map.");
      const auto& mem_list = alloc_data->memoryList();
      data = mem_list[index];
    }
    {
      const auto [result, index] = mem_map.remove(address);
      ZISC_ASSERT(result, "The address isn't in the map.");
    }
    alloc_data->memoryResource()->deallocate(memory, data.size_, data.alignment_);
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
    [[maybe_unused]] void* user_data,
    [[maybe_unused]] size_t size,
    [[maybe_unused]] VkInternalAllocationType type,
    [[maybe_unused]] VkSystemAllocationScope scope)
{
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] type No description.
  \param [in] scope No description.
  */
void VulkanSubPlatform::Callbacks::notifyOfMemoryDeallocation(
    [[maybe_unused]] void* user_data,
    [[maybe_unused]] size_t size,
    [[maybe_unused]] VkInternalAllocationType type,
    [[maybe_unused]] VkSystemAllocationScope scope)
{
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
  using zisc::cast;

  if (getEnvNumber("ZIVC_SUPPRESS_TRIVIAL_WARNINGS") != 0) {
    // VkLayer_api_dump.json invalid layer manifest file version 1.2.0. may cause errors.
    constexpr int32b invalid_layer_manifest_id = 0;
    // UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
    constexpr int32b best_practices_specialuse_id = -2111305990;
    switch (callback_data->messageIdNumber) {
     case invalid_layer_manifest_id:
     case best_practices_specialuse_id:
      return VK_FALSE;
     default:
      break;
    }
  }

  constexpr std::size_t max_message_size = 1024;
  using MessageType = std::array<char, max_message_size>;
  MessageType message{""};
  MessageType tmp{""};
  bool is_error = false;

  if (user_data != nullptr) {
    char* msg = tmp.data();
    const auto* data = cast<const IdData*>(user_data);
    std::sprintf(msg, "ID[%lld] -", cast<long long>(data->id()));
    if (data->hasName()) {
      std::sprintf(msg, "%s Name '%s'", msg, data->name().data());
    }
    if (data->hasFileInfo()) {
      const std::string_view file_name = data->fileName();
      const int64b line = data->lineNumber();
      std::sprintf(msg, "%s at line %lld in '%s'",
                   msg,
                   cast<long long>(line),
                   file_name.data());
    }
    concatStr("\n", msg);
    concatStr(msg, message.data());
  }

  constexpr std::string_view indent{"          "};
  {
    char* prefix = tmp.data();
    if ((severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) != 0) {
      copyStr("VERBOSE : ", prefix);
    }
    else if ((severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
      copyStr("INFO    : ", prefix);
    }
    else if ((severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
      copyStr("WARNING : ", prefix);
    }
    else if ((severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
      copyStr("ERROR   : ", prefix);
      is_error = true;
    }

    if ((message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) != 0) {
      concatStr("GENERAL", prefix);
    }
    else if ((message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) != 0) {
      concatStr("VALIDATION", prefix);
    }
    else if ((message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) != 0) {
      concatStr("PERF", prefix);
    }

    char* msg = tmp.data();
    std::sprintf(msg, "%s - Message ID Number %d, Message ID Name %s : %s\n",
        prefix,
        callback_data->messageIdNumber,
        callback_data->pMessageIdName,
        callback_data->pMessage);
    concatStr(msg, message.data());
  }

  if (0 < callback_data->queueLabelCount) {
    char* msg = tmp.data();
    std::sprintf(msg, "\n%sQueue Labels - %d\n",
        indent.data(),
        cast<int>(callback_data->queueLabelCount));
    for (std::size_t id = 0; id < callback_data->queueLabelCount; ++id) {
      const auto& label = callback_data->pQueueLabels[id];
      std::sprintf(msg, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          msg,
          indent.data(),
          cast<int>(id),
          label.pLabelName,
          cast<double>(label.color[0]),
          cast<double>(label.color[1]),
          cast<double>(label.color[2]),
          cast<double>(label.color[3]));
    }
    concatStr(msg, message.data());
  }

  if (0 < callback_data->cmdBufLabelCount) {
    char* msg = tmp.data();
    std::sprintf(msg, "\n%sCommand Buffer Labels - %d\n",
        indent.data(),
        zisc::cast<int>(callback_data->cmdBufLabelCount));
    for (std::size_t id = 0; id < callback_data->cmdBufLabelCount; ++id) {
      const auto& label = callback_data->pCmdBufLabels[id];
      std::sprintf(msg, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          msg,
          indent.data(),
          cast<int>(id),
          label.pLabelName,
          cast<double>(label.color[0]),
          cast<double>(label.color[1]),
          cast<double>(label.color[2]),
          cast<double>(label.color[3]));
    }
    concatStr(msg, message.data());
  }

  if (0 < callback_data->objectCount) {
    char* msg = tmp.data();
    std::sprintf(msg, "\n%sObjects - %d\n",
        indent.data(),
        cast<int>(callback_data->objectCount));
    for (std::size_t id = 0; id < callback_data->objectCount; ++id) {
      const auto& object = callback_data->pObjects[id];
      const auto obj_type_name = zivcvk::to_string(
          cast<zivcvk::ObjectType>(object.objectType));
      std::sprintf(msg, "%s%s  * Object[%d] - Type '%s', Value %llu, Name '%s'\n",
          msg,
          indent.data(),
          cast<int>(id),
          obj_type_name.c_str(),
          cast<unsigned long long>(object.objectHandle),
          object.pObjectName);
    }
    concatStr(msg, message.data());
  }

  std::ostream* output = (is_error) ? &std::cerr : &std::cout;
  (*output) << message.data() << std::endl;

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
  if ((original_memory != nullptr) && (memory != nullptr)) {
    auto* alloc_data = zisc::cast<AllocatorData*>(user_data);

    static_assert(sizeof(void*) == sizeof(double));
    const auto address = zisc::bit_cast<double>(original_memory);
    ZISC_ASSERT(std::isfinite(address), "The address float isn't finite.");
    auto& mem_map = alloc_data->memoryMap();
    MemoryData data;
    {
      const auto [result, index] = mem_map.contain(address);
      ZISC_ASSERT(result, "The address isn't in the map.");
      const auto& mem_list = alloc_data->memoryList();
      data = mem_list[index];
    }
    const std::size_t data_size = (std::min)(size, data.size_);
    std::memcpy(memory, original_memory, data_size);
  }
  // Deallocate the original memory
  if (original_memory != nullptr)
    deallocateMemory(user_data, original_memory);
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
  const uint32b app_version = vkMakeVersion(app_version_major,
                                            app_version_minor,
                                            app_version_patch);
  std::string_view engine_name = engineName();
  constexpr uint32b engine_version = vkMakeVersion(Config::versionMajor(),
                                                   Config::versionMinor(),
                                                   Config::versionPatch());
  constexpr uint32b api_version = vkGetVulkanApiVersion();
  const zivcvk::ApplicationInfo data{app_name.data(),
                                     app_version,
                                     engine_name.data(),
                                     engine_version,
                                     api_version};
  return zisc::cast<VkApplicationInfo>(data);
}

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
bool VulkanSubPlatform::compareProperties(const std::string_view lhs,
                                          const std::string_view rhs) noexcept
{
  const bool result = lhs < rhs;
  return result;
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initAllocator() noexcept
{
  auto* mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<AllocatorData> alloc{mem_resource};
  allocator_data_ = zisc::pmr::allocateUnique<AllocatorData>(alloc, mem_resource);
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void VulkanSubPlatform::initInstance(PlatformOptions& options)
{
  using InstancePtr = std::add_pointer_t<VkInstance>;
  auto* ptr = zisc::cast<InstancePtr>(options.vulkanInstancePtr());
  if (ptr != nullptr) {
    // Use the given instance instead of allocating new instance
    instance_ = ZIVC_VK_NULL_HANDLE;
    instance_ref_ = ptr;
    dispatcher_->set(instance());
    return;
  }

  zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

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
  using FeatureEnableList = zisc::pmr::vector<zivcvk::ValidationFeatureEnableEXT>;
  FeatureEnableList::allocator_type feature_alloc{memoryResource()};
  FeatureEnableList validation_features_list{feature_alloc};
  validation_features_list.reserve(4);
  validation_features_list = {
      zivcvk::ValidationFeatureEnableEXT::eBestPractices,
      zivcvk::ValidationFeatureEnableEXT::eSynchronizationValidation
  };
  //! \todo the gpu assisted flags cause shader compilation error on macOS
#if !defined(Z_MAC)
  validation_features_list.emplace_back(
      zivcvk::ValidationFeatureEnableEXT::eGpuAssisted);
  validation_features_list.emplace_back(
      zivcvk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot);
#endif // Z_MAC
  zivcvk::ValidationFeaturesEXT validation_features{
      zisc::cast<uint32b>(validation_features_list.size()),
      validation_features_list.data(),
      0,
      nullptr};

  initWindowSurface(options, &extensions);

  const zivcvk::ApplicationInfo app_info{makeApplicationInfo(
      options.platformName(),
      options.platformVersionMajor(),
      options.platformVersionMinor(),
      options.platformVersionPatch())};
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
  const auto& loader = dispatcher().loader();
  zivcvk::Instance ins = zivcvk::createInstance(create_info, alloc, loader);
  instance_ = zisc::cast<VkInstance>(ins);
  instance_ref_ = std::addressof(instance_);
  dispatcher_->set(instance());
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceList()
{
  const zivcvk::Instance ins{instance()};

  using DeviceList = zisc::pmr::vector<zivcvk::PhysicalDevice>;
  DeviceList::allocator_type alloc{memoryResource()};
  const auto& loader = dispatcher().loader();
  auto device_list = ins.enumeratePhysicalDevices(alloc, loader);

  using DstDeviceList = decltype(device_list_)::element_type;
  auto* list_ptr = zisc::reinterp<DstDeviceList*>(std::addressof(device_list));
  device_list_ = zisc::pmr::allocateUnique<DstDeviceList>(
      memoryResource(),
      std::move(*list_ptr));
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceInfoList() noexcept
{
  auto* mem_resource = memoryResource();
  using DeviceInfoList = decltype(device_info_list_)::element_type;
  DeviceInfoList info_list{DeviceInfoList::allocator_type{mem_resource}};
  zisc::pmr::polymorphic_allocator<DeviceInfoList> alloc{mem_resource};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      alloc,
      std::move(info_list));
}

/*!
  \details No detailed description

  \param [in] options No description.
  */
void VulkanSubPlatform::initDispatcher(PlatformOptions& options)
{
  auto* mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<VulkanDispatchLoader> alloc{mem_resource};

  using FuncPtr = std::add_pointer_t<PFN_vkGetInstanceProcAddr>;
  auto* ptr = zisc::cast<FuncPtr>(options.vulkanGetProcAddrPtr());
  std::string_view lib = options.vulkanLibraryName();
  dispatcher_ = zisc::pmr::allocateUnique<VulkanDispatchLoader>(alloc);
  if (ptr != nullptr)
    dispatcher_->set(mem_resource, *ptr);
  else
    dispatcher_->set(mem_resource, lib);
  ZISC_ASSERT(dispatcher().isDispatchableForInstance(), "Unexpected init.");
  ZISC_ASSERT(!dispatcher().isDispatchableForDevice(), "Unexpected init.");
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initProperties()
{
  auto* mem_resource = memoryResource();
  const auto& loader = dispatcher().loader();

  // Instance extension properties
  {
    // Create a properties list
    {
      using PropertiesList = decltype(extension_properties_list_)::element_type;
      PropertiesList prop_list{PropertiesList::allocator_type{mem_resource}};
      zisc::pmr::polymorphic_allocator<PropertiesList> alloc{mem_resource};
      extension_properties_list_ = zisc::pmr::allocateUnique<PropertiesList>(
          alloc,
          std::move(prop_list));
    }
    auto& prop_list = *extension_properties_list_;
    // Retrieve extension properties
    uint32b size = 0;
    {
      const auto result = zivcvk::enumerateInstanceExtensionProperties(nullptr,
                                                                       &size,
                                                                       nullptr,
                                                                       loader);
      if (result != zivcvk::Result::eSuccess) {
        const char* message = "Enumerating instance extension properties failed.";
        throw SystemError{ErrorCode::kInitializationFailed, message};
      }
    }
    prop_list.resize(size);
    {
      auto* props = zisc::reinterp<zivcvk::ExtensionProperties*>(prop_list.data());
      const auto result = zivcvk::enumerateInstanceExtensionProperties(nullptr,
                                                                       &size,
                                                                       props,
                                                                       loader);
      if (result != zivcvk::Result::eSuccess) {
        const char* message = "Enumerating instance extension properties failed.";
        throw SystemError{ErrorCode::kInitializationFailed, message};
      }
    }
    {
      const auto comp = [](const VkExtensionProperties& lhs,
                           const VkExtensionProperties& rhs) noexcept
      {
        return compareProperties(lhs.extensionName, rhs.extensionName);
      };
      std::sort(prop_list.begin(), prop_list.end(), comp);
    }
  }
  {
    // Create a properties list
    {
      using PropertiesList = decltype(layer_properties_list_)::element_type;
      PropertiesList prop_list{PropertiesList::allocator_type{mem_resource}};
      zisc::pmr::polymorphic_allocator<PropertiesList> alloc{mem_resource};
      layer_properties_list_ = zisc::pmr::allocateUnique<PropertiesList>(
          alloc,
          std::move(prop_list));
    }
    auto& prop_list = *layer_properties_list_;
    // Retrieve layer properties
    uint32b size = 0;
    {
      const auto result = zivcvk::enumerateInstanceLayerProperties(&size,
                                                                   nullptr,
                                                                   loader);
      if (result != zivcvk::Result::eSuccess) {
        const char* message = "Enumerating instance layer properties failed.";
        throw SystemError{ErrorCode::kInitializationFailed, message};
      }
    }
    prop_list.resize(size);
    {
      auto* props = zisc::reinterp<zivcvk::LayerProperties*>(prop_list.data());
      const auto result = zivcvk::enumerateInstanceLayerProperties(&size,
                                                                   props,
                                                                   loader);
      if (result != zivcvk::Result::eSuccess) {
        const char* message = "Enumerating instance layer properties failed.";
        throw SystemError{ErrorCode::kInitializationFailed, message};
      }
    }
    // Sort by property name
    {
      const auto comp = [](const VkLayerProperties& lhs,
                           const VkLayerProperties& rhs) noexcept
      {
        return compareProperties(lhs.layerName, rhs.layerName);
      };
      std::sort(prop_list.begin(), prop_list.end(), comp);
    }
  }
}

/*!
  \details No detailed description

  \param [in] options No description.
  \param [out] extension_list No description.
  */
void VulkanSubPlatform::initWindowSurface(
    const PlatformOptions& options,
    zisc::pmr::vector<const char*>* extension_list)
{
  window_surface_type_ = WindowSurfaceType::kNone;
  if (!options.vulkanWSIExtensionEnabled())
    return;

  const char* ws_name = nullptr;
  if (isExtensionSupported(VK_KHR_SURFACE_EXTENSION_NAME)) {
    const auto check_wstype = [this, &ws_name](const char* name,
                                               const WindowSurfaceType type) noexcept
    {
      if ((ws_name == nullptr) && isExtensionSupported(name)) {
        window_surface_type_ = type;
        ws_name = name;
      }
    };
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    check_wstype(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, WindowSurfaceType::kWin32);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_METAL_EXT)
    check_wstype(VK_EXT_METAL_SURFACE_EXTENSION_NAME, WindowSurfaceType::kMetal);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_MACOS_MVK)
    check_wstype(VK_MVK_MACOS_SURFACE_EXTENSION_NAME, WindowSurfaceType::kMacOS);
#endif // VK_USE_PLATFORM_MACOS_MVK
#if defined(VK_USE_PLATFORM_XCB_KHR)
    check_wstype(VK_KHR_XCB_SURFACE_EXTENSION_NAME, WindowSurfaceType::kXcb);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    check_wstype(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, WindowSurfaceType::kXlib);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    check_wstype(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, WindowSurfaceType::kWayland);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
  }

  if (ws_name != nullptr) {
    extension_list->emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extension_list->emplace_back(ws_name);
  }
  else {
    // We go this code path when no platform is found
    const char* message = "No platform of window surface found.";
    throw SystemError{ErrorCode::kVulkanWindowSurfaceNotFound, message};
  }
}

/*!
  \details No detailed description

  \param [in] name No description.
  \return No description
  */
bool VulkanSubPlatform::isExtensionSupported(const std::string_view name) const noexcept
{
  const auto comp = [](const VkExtensionProperties& lhs,
                       const std::string_view rhs) noexcept
  {
    return compareProperties(lhs.extensionName, rhs);
  };
  const auto& prop_list = extensionPropertiesList();
  const auto ite = std::lower_bound(prop_list.begin(), prop_list.end(), name, comp);
  const bool result = (ite != prop_list.end()) && (ite->extensionName == name);
  return result;
}

/*!
  \details No detailed description

  \param [in] name No description.
  \return No description
  */
bool VulkanSubPlatform::isLayerSupported(const std::string_view name) const noexcept
{
  const auto comp = [](const VkLayerProperties& lhs,
                       const std::string_view rhs) noexcept
  {
    return compareProperties(lhs.layerName, rhs);
  };
  const auto& prop_list = layerPropertiesList();
  const auto ite = std::lower_bound(prop_list.begin(), prop_list.end(), name, comp);
  const bool result = (ite != prop_list.end()) && (ite->layerName == name);
  return result;
}

} // namespace zivc
