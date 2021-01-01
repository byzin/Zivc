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
#include <utility>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
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

namespace zivc {

/*!
  \details No detailed description
  */
Platform::Platform() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Platform::Platform(Platform&& other) noexcept :
    device_info_list_{std::move(other.device_info_list_)},
    id_count_{other.id_count_.load()},
    is_debug_mode_{other.is_debug_mode_}
{
  std::swap(mem_resource_, other.mem_resource_);
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
  std::swap(mem_resource_, other.mem_resource_);
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
  device_info_list_ = std::move(other.device_info_list_);
  id_count_ = other.id_count_.load();
  is_debug_mode_ = other.is_debug_mode_;
  return *this;
}

/*!
  \details No detailed description
  */
void Platform::destroy() noexcept
{
  device_info_list_.reset();
  for (auto& sub_platform : sub_platform_list_)
    sub_platform.reset();
  mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Platform::makeDevice(const std::size_t device_index)
{
  const DeviceInfo* info = deviceInfoList()[device_index];
  SubPlatform* sub_platform = subPlatform(info->type());
  auto device = sub_platform->makeDevice(*info);
  return device;
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initialize(PlatformOptions& platform_options)
{
  // Clear the previous platform data first 
  destroy();

  mem_resource_ = platform_options.memoryResource();
  setDebugMode(platform_options.debugModeEnabled());
  id_count_.store(0);

  // Initialize sub-platforms
  initSubPlatform<CpuSubPlatform>(platform_options);
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
  if (platform_options.vulkanSubPlatformEnabled())
    initSubPlatform<VulkanSubPlatform>(platform_options);
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM

  // Get device info list
  using DeviceInfoList = zisc::pmr::vector<const DeviceInfo*>;
  DeviceInfoList new_info_list{DeviceInfoList::allocator_type{mem_resource_}};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      mem_resource_,
      std::move(new_info_list));
  updateDeviceInfoList();
}

/*!
  \details No detailed description
  */
void Platform::updateDeviceInfoList() noexcept
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
}

/*!
  \details No detailed description

  \tparam SubPlatformType No description.
  \param [in,out] platform_options No description.
  */
template <typename SubPlatformType>
void Platform::initSubPlatform(PlatformOptions& platform_options)
{
  zisc::pmr::polymorphic_allocator<SubPlatformType> alloc{memoryResource()};
  SharedSubPlatform sub_platform = std::allocate_shared<SubPlatformType>(alloc, this);
  WeakSubPlatform own{sub_platform};
  sub_platform->initialize(std::move(own), platform_options);
  setSubPlatform(std::move(sub_platform));
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
  \return No description
  */
UniquePlatform makePlatform(zisc::pmr::memory_resource* mem_resource) noexcept
{
  auto platform = zisc::pmr::allocateUnique<Platform>(mem_resource);
  return platform;
}

} // namespace zivc
