/*!
  \file cpu_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_sub_platform.hpp"
// Standard C++ library
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_device.hpp"
#include "cpu_device_info.hpp"
#include "zivc/device.hpp"
#include "zivc/platform.hpp"
#include "zivc/platform_options.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/error.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
CpuSubPlatform::CpuSubPlatform(Platform* platform) noexcept :
    SubPlatform(platform)
{
}

/*!
  \details No detailed description
  */
CpuSubPlatform::~CpuSubPlatform() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in,out] device_info_list No description.
  */
void CpuSubPlatform::getDeviceInfoList(
    zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept
{
  device_info_list.emplace_back(device_info_.get());
}

/*!
  \details No detailed description

  \param [in] device_info No description.
  \return No description
  */
SharedDevice CpuSubPlatform::makeDevice(const DeviceInfo& device_info)
{
  auto info = zisc::cast<const CpuDeviceInfo*>(std::addressof(device_info));
  if (device_info_.get() != info) {
    const char* message = "Invalid cpu device info is passed.";
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }

  zisc::pmr::polymorphic_allocator<CpuDevice> alloc{memoryResource()};
  SharedDevice device = std::allocate_shared<CpuDevice>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwnPtr()};
  WeakDevice own{device};
  device->initialize(std::move(parent), std::move(own), device_info);

  return device;
}

/*!
  \details No detailed description

  \return No description
  */
bool CpuSubPlatform::isAvailable() const noexcept
{
  const auto& own = getOwnPtr();
  const bool result = !own.expired();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuSubPlatform::numOfDevices() const noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType CpuSubPlatform::type() const noexcept
{
  return SubPlatformType::kCpu;
}

/*!
  \details No detailed description
  */
void CpuSubPlatform::updateDeviceInfoList()
{
  device_info_->fetch();
}

/*!
  \details No detailed description
  */
void CpuSubPlatform::destroyData() noexcept
{
  num_of_threads_ = 0;
  task_batch_size_ = 0;
  device_info_.reset();
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void CpuSubPlatform::initData(PlatformOptions& options)
{
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<CpuDeviceInfo> alloc{mem_resource};
  device_info_ = zisc::pmr::allocateUnique<CpuDeviceInfo>(alloc, mem_resource);
  num_of_threads_ = options.cpuNumOfThreads();
  constexpr uint32b max_batch_size = maxTaskBatchSize();
  task_batch_size_ = options.cpuTaskBatchSize();
  task_batch_size_ = zisc::clamp(task_batch_size_, 1u, max_batch_size);
}

/*!
  \details No detailed description
  */
void CpuSubPlatform::updateDebugInfoImpl()
{
}

} // namespace zivc
