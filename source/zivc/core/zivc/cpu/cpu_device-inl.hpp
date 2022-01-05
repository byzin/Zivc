/*!
  \file cpu_device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_DEVICE_INL_HPP
#define ZIVC_CPU_DEVICE_INL_HPP

#include "cpu_device.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/thread/thread_manager.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
const CpuDeviceInfo& CpuDevice::deviceInfoImpl() const noexcept
{
  const auto& info = deviceInfo();
  return *zisc::cast<const CpuDeviceInfo*>(std::addressof(info));
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyAllocation(const std::size_t size) noexcept
{
  heap_usage_.add(size);
  CpuSubPlatform& sub_platform = parentImpl();
  sub_platform.notifyOfDeviceMemoryAllocation(size);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyDeallocation(const std::size_t size) noexcept
{
  heap_usage_.release(size);
  CpuSubPlatform& sub_platform = parentImpl();
  sub_platform.notifyOfDeviceMemoryDeallocation(size);
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuDevice::numOfThreads() const noexcept
{
  const auto& thread_manager = threadManager();
  return thread_manager.numOfThreads();
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuDevice::taskBatchSize() const noexcept
{
  const auto& platform = parentImpl();
  return platform.taskBatchSize();
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::ThreadManager& CpuDevice::threadManager() noexcept
{
  return parentImpl().threadManager();
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::ThreadManager& CpuDevice::threadManager() const noexcept
{
  return parentImpl().threadManager();
}

/*!
  \details No detailed description

  \return No description
  */
inline
CpuSubPlatform& CpuDevice::parentImpl() noexcept
{
  auto p = getParent();
  return *zisc::cast<CpuSubPlatform*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const CpuSubPlatform& CpuDevice::parentImpl() const noexcept
{
  const auto p = getParent();
  return *zisc::cast<const CpuSubPlatform*>(p);
}

} // namespace zivc

#endif // ZIVC_CPU_DEVICE_INL_HPP
