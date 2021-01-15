/*!
  \file device_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INFO_INL_HPP
#define ZIVC_DEVICE_INFO_INL_HPP

#include "device_info.hpp"
// Standard C++ library
#include <cstddef>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/memory_heap_info.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view DeviceInfo::invalidName() noexcept
{
  constexpr std::size_t n = std::size(kInvalidName);
  const std::string_view name{kInvalidName, n};
  return name;
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
inline
MemoryHeapInfo& DeviceInfo::heapInfo(const std::size_t heap_index) noexcept
{
  return heap_info_list_[heap_index];
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
inline
const MemoryHeapInfo& DeviceInfo::heapInfo(const std::size_t heap_index) const noexcept
{
  return heap_info_list_[heap_index];
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::vector<MemoryHeapInfo>& DeviceInfo::heapInfoList() noexcept
{
  return heap_info_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<MemoryHeapInfo>& DeviceInfo::heapInfoList() const noexcept
{
  return heap_info_list_;
}

} // namespace zivc

#endif // ZIVC_DEVICE_INFO_INL_HPP
