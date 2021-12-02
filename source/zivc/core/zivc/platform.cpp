/*!
  \file platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "platform.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "device.hpp"
#include "platform_options.hpp"
#include "sub_platform.hpp"
#include "zivc_config.hpp"
#include "cpu/cpu_sub_platform.hpp"
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_sub_platform.hpp"
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
#include "utility/error.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
Platform::Platform() noexcept
{
  setDebugMode(false);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Platform::Platform(Platform&& other) noexcept :
    default_mem_resource_{std::move(other.default_mem_resource_)},
    custom_mem_resource_{other.custom_mem_resource_},
    device_list_{std::move(other.device_list_)},
    device_info_list_{std::move(other.device_info_list_)},
    id_count_{other.id_count_.load(std::memory_order::acquire)},
    is_debug_mode_{other.is_debug_mode_}
{
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
}

/*!
  \details No detailed description
  */
Platform::~Platform() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
Platform& Platform::operator=(Platform&& other) noexcept
{
  default_mem_resource_ = std::move(other.default_mem_resource_);
  custom_mem_resource_ = other.custom_mem_resource_;
  device_list_ = std::move(other.device_list_);
  device_info_list_ = std::move(other.device_info_list_);
  id_count_ = other.id_count_.load(std::memory_order::acquire);
  is_debug_mode_ = other.is_debug_mode_;
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
  return *this;
}

/*!
  \details No detailed description
  */
void Platform::destroy() noexcept
{
  device_info_list_.reset();
  device_list_.reset();
  for (auto& sub_platform : sub_platform_list_)
    sub_platform.reset();
  default_mem_resource_.reset();
  custom_mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void Platform::initialize(PlatformOptions& options)
{
  // Clear the previous platform data first 
  destroy();

  setMemoryResource(options.memoryResource());
  setDebugMode(options.debugModeEnabled());
  id_count_.store(0, std::memory_order::release);

  // Initialize sub-platforms
  initSubPlatform<CpuSubPlatform>(options);
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
  if (options.vulkanSubPlatformEnabled())
    initSubPlatform<VulkanSubPlatform>(options);
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM

  // Device list
  using DeviceList = decltype(device_list_)::element_type;
  DeviceList new_device_list{DeviceList::allocator_type{memoryResource()}};
  device_list_ = zisc::pmr::allocateUnique<DeviceList>(
    memoryResource(),
    std::move(new_device_list));

  // Get device info list
  using DeviceInfoList = decltype(device_info_list_)::element_type;
  DeviceInfoList new_info_list{DeviceInfoList::allocator_type{memoryResource()}};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      memoryResource(),
      std::move(new_info_list));
  updateDeviceInfoList();
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Platform::queryDevice(const std::size_t device_index)
{
  if (device_list_->size() <= device_index) {
    const char* message = "The device index is out of range.";
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }
  SharedDevice device;
  // Check if the device is already created
  if (WeakDevice d = (*device_list_)[device_index]; !d.expired()) {
    device = d.lock();
  }
  // If not, create a new device
  if (!device) {
    device = makeDevice(device_index);
    (*device_list_)[device_index] = device;
  }
  return device;
}

/*!
  \details No detailed description

  \tparam SubPlatformType No description.
  \param [in,out] options No description.
  */
template <typename SubPlatformType>
void Platform::initSubPlatform(PlatformOptions& options)
{
  zisc::pmr::polymorphic_allocator<SubPlatformType> alloc{memoryResource()};
  SharedSubPlatform sub_platform = std::allocate_shared<SubPlatformType>(alloc, this);
  WeakSubPlatform own{sub_platform};
  sub_platform->initialize(std::move(own), options);
  setSubPlatform(std::move(sub_platform));
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Platform::makeDevice(const std::size_t device_index)
{
  const auto& info_list = deviceInfoList();
  if (info_list.size() <= device_index) {
    const char* message = "The device index is out of range.";
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }
  const DeviceInfo* info = info_list[device_index];
  SubPlatform* sub_platform = subPlatform(info->type());
  ZISC_ASSERT(sub_platform->isAvailable(), "The platform isn't available.");
  auto device = sub_platform->makeDevice(*info);
  return device;
}

/*!
  \details No detailed description

  \param [in] is_debug_mode No description.
  */
void Platform::setDebugMode(const bool is_debug_mode) noexcept
{
  is_debug_mode_ = is_debug_mode ? Config::scalarResultTrue()
                                 : Config::scalarResultFalse();
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
void Platform::setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept
{
  custom_mem_resource_ = mem_resource;
  if (mem_resource != nullptr)
    default_mem_resource_.reset();
  else
    default_mem_resource_ = std::make_unique<zisc::SimpleMemoryResource>();
}

/*!
  \details No detailed description
  */
void Platform::updateDeviceInfoList()
{
  device_info_list_->clear();
  std::size_t num_of_devices = 0;
  // 
  for (auto& sub_platform : sub_platform_list_) {
    if (sub_platform && sub_platform->isAvailable()) {
      sub_platform->updateDeviceInfoList();
      num_of_devices += sub_platform->numOfDevices();
    }
  }
  //
  device_info_list_->reserve(num_of_devices);
  for (const auto& sub_platform : sub_platform_list_) {
    if (sub_platform && sub_platform->isAvailable())
      sub_platform->getDeviceInfoList(*device_info_list_);
  }
  device_list_->resize(num_of_devices);
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  \return No description
  */
SharedPlatform makePlatform(PlatformOptions& options)
{
  SharedPlatform platform;

  // Create a platform
  auto* mem_resource = options.memoryResource();
  if (mem_resource != nullptr) {
    zisc::pmr::polymorphic_allocator<Platform> alloc{mem_resource};
    platform = std::allocate_shared<Platform>(alloc);
  }
  else {
    platform = std::make_shared<Platform>();
  }

  // Initialize the platform
  try {
    platform->initialize(options);
  }
  catch (const std::runtime_error& error) {
    platform->destroy();
    platform.reset();
    throw error;
  }

  return platform;
}

} // namespace zivc
