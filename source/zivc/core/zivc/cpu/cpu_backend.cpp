/*!
  \file cpu_backend.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_backend.hpp"
// Standard C++ library
#include <cstddef>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/concurrency/thread_manager.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_device.hpp"
#include "cpu_device_info.hpp"
#include "zivc/backend.hpp"
#include "zivc/context.hpp"
#include "zivc/context_options.hpp"
#include "zivc/device.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/error.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
CpuBackend::CpuBackend(Context* context) noexcept :
    Backend(context)
{
}

/*!
  \details No detailed description
  */
CpuBackend::~CpuBackend() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] device_info No description.
  \return No description
  */
SharedDevice CpuBackend::createDevice(const DeviceInfo& device_info)
{
  const auto* info = static_cast<const CpuDeviceInfo*>(std::addressof(device_info));
  if (device_info_.get() != info) {
    const std::string message = createErrorMessage(
        *this,
        "Invalid cpu device info is passed.");
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }

  zisc::pmr::polymorphic_allocator<CpuDevice> alloc{memoryResource()};
  SharedDevice device = std::allocate_shared<CpuDevice>(alloc, issueId());

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

  \param [in] index No description.
  \return No description
  */
const DeviceInfo& CpuBackend::deviceInfo([[maybe_unused]] const std::size_t index) const noexcept
{
  return *device_info_;
}

/*!
  \details No detailed description

  \return No description
  */
bool CpuBackend::isAvailable() const noexcept
{
  const auto& own = getOwnPtr();
  const bool result = !own.expired();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuBackend::numOfDevices() const noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \return No description
  */
BackendType CpuBackend::type() const noexcept
{
  return BackendType::kCpu;
}

/*!
  \details No detailed description
  */
void CpuBackend::updateDeviceInfo()
{
  device_info_->fetch();
}

/*!
  \details No detailed description
  */
void CpuBackend::destroyData() noexcept
{
  task_batch_size_ = 0;
  thread_manager_.reset();
  device_info_.reset();
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void CpuBackend::initData(ContextOptions& options)
{
  auto* mem_resource = memoryResource();
  // Init device info
  {
    zisc::pmr::polymorphic_allocator<CpuDeviceInfo> alloc{mem_resource};
    device_info_ = zisc::pmr::allocateUnique<CpuDeviceInfo>(alloc, mem_resource);
  }
  // Init the thread manager
  {
    zisc::pmr::polymorphic_allocator<zisc::ThreadManager> alloc{mem_resource};
    thread_manager_ = zisc::pmr::allocateUnique(alloc,
                                                options.cpuNumOfThreads(),
                                                mem_resource);
  }
  // Init batch size
  {
    constexpr uint32b max_batch_size = maxTaskBatchSize();
    task_batch_size_ = options.cpuTaskBatchSize();
    task_batch_size_ = zisc::clamp(task_batch_size_, 1U, max_batch_size);
  }
}

/*!
  \details No detailed description
  */
void CpuBackend::updateDebugInfoImpl()
{
}

} // namespace zivc
