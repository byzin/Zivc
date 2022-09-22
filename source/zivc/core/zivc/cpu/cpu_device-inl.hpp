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
#include "zisc/concurrency/thread_manager.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "cpu_backend.hpp"
#include "cpu_device_info.hpp"
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
  return *static_cast<const CpuDeviceInfo*>(std::addressof(info));
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyAllocation(const std::size_t size) noexcept
{
  heap_usage_.add(size);
  CpuBackend& backend_p = parentImpl();
  backend_p.notifyOfDeviceMemoryAllocation(size);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyDeallocation(const std::size_t size) noexcept
{
  heap_usage_.release(size);
  CpuBackend& backend_p = parentImpl();
  backend_p.notifyOfDeviceMemoryDeallocation(size);
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
  const CpuBackend& backend_p = parentImpl();
  return backend_p.taskBatchSize();
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
CpuBackend& CpuDevice::parentImpl() noexcept
{
  auto p = getParent();
  return *static_cast<CpuBackend*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const CpuBackend& CpuDevice::parentImpl() const noexcept
{
  const auto p = getParent();
  return *static_cast<const CpuBackend*>(p);
}

} // namespace zivc

#endif // ZIVC_CPU_DEVICE_INL_HPP
