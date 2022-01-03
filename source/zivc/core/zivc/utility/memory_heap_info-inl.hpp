/*!
  \file memory_heap_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_MEMORY_HEAP_INFO_INL_HPP
#define ZIVC_MEMORY_HEAP_INFO_INL_HPP

#include "memory_heap_info.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
// Zisc
#include "zisc/zisc_config.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t MemoryHeapInfo::availableSize() const noexcept
{
  return available_size_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool MemoryHeapInfo::isDeviceLocal() const noexcept
{
  const bool flag = is_device_local_ != zisc::kFalse;
  return flag;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void MemoryHeapInfo::setAvailableSize(const std::size_t size) noexcept
{
  available_size_ = size;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
void MemoryHeapInfo::setDeviceLocal(const bool flag) noexcept
{
  is_device_local_ = flag ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void MemoryHeapInfo::setTotalSize(const std::size_t size) noexcept
{
  total_size_ = size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t MemoryHeapInfo::totalSize() const noexcept
{
  return total_size_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto MemoryHeapInfo::usedSizeForBuffer() noexcept -> Usage&
{
  return used_size_for_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto MemoryHeapInfo::usedSizeForBuffer() const noexcept -> const Usage&
{
  return used_size_for_buffer_;
}

} // namespace zivc

#endif // ZIVC_MEMORY_HEAP_INFO_INL_HPP
