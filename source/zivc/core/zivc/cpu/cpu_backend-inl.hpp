/*!
  \file cpu_backend-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_BACKEND_INL_HPP
#define ZIVC_CPU_BACKEND_INL_HPP

#include "cpu_backend.hpp"
// Standard C++ library
#include <bit>
#include <cstddef>
// Zisc
#include "zisc/concurrency/thread_manager.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "../zivc_config.hpp"
#include "../auxiliary/memory_heap_info.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b CpuBackend::maxTaskBatchSize() noexcept
{
  constexpr uint32b size = 1024;
  static_assert(std::has_single_bit(size));
  static_assert((size % CpuDeviceInfo::maxWorkGroupSize()) == 0);
  return size;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuBackend::notifyOfDeviceMemoryAllocation(const std::size_t size) noexcept
{
  MemoryHeapInfo& heap_info = device_info_->heapInfo(0);
  zisc::Memory::Usage& usage = heap_info.usedSizeForBuffer();
  usage.add(size);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuBackend::notifyOfDeviceMemoryDeallocation(const std::size_t size) noexcept
{
  MemoryHeapInfo& heap_info = device_info_->heapInfo(0);
  zisc::Memory::Usage& usage = heap_info.usedSizeForBuffer();
  usage.release(size);
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuBackend::taskBatchSize() const noexcept
{
  return static_cast<std::size_t>(task_batch_size_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::ThreadManager& CpuBackend::threadManager() noexcept
{
  return *thread_manager_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::ThreadManager& CpuBackend::threadManager() const noexcept
{
  return *thread_manager_;
}

} // namespace zivc

#endif // ZIVC_CPU_BACKEND_INL_HPP
