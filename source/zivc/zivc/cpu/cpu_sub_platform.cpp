/*!
  \file cpu_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "cpu_device.hpp"
#include "cpu_device_info.hpp"
#include "zivc/device.hpp"
#include "zivc/platform.hpp"
#include "zivc/platform_options.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"

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
SharedDevice CpuSubPlatform::makeDevice(const DeviceInfo& device_info) noexcept
{
  auto info = zisc::cast<const CpuDeviceInfo*>(std::addressof(device_info));
  if (device_info_.get() != info) {
    //! \todo Throw an exception
    std::cerr << "[Error] Invalid device info is passed." << std::endl;
  }
  zisc::pmr::polymorphic_allocator<CpuDevice> alloc{memoryResource()};
  SharedDevice device = std::allocate_shared<CpuDevice>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
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
  return true;
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
void CpuSubPlatform::updateDeviceInfoList() noexcept
{
  device_info_->fetch();
}

/*!
  \details No detailed description
  */
void CpuSubPlatform::destroyData() noexcept
{
  num_of_threads_ = 0;
  task_batch_size_ = 32;
  device_info_.reset();
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void CpuSubPlatform::initData(PlatformOptions& platform_options)
{
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<CpuDeviceInfo> alloc{mem_resource};
  device_info_ = zisc::pmr::allocateUnique<CpuDeviceInfo>(alloc, mem_resource);
  num_of_threads_ = platform_options.cpuNumOfThreads();
  const uint32b max_batch_size = maxTaskBatchSize();
  task_batch_size_ = platform_options.cpuTaskBatchSize();
  task_batch_size_ = zisc::clamp(task_batch_size_, 1, max_batch_size);
}

} // namespace zivc
