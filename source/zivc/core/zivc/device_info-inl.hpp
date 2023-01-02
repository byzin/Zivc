/*!
  \file device_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INFO_INL_HPP
#define ZIVC_DEVICE_INFO_INL_HPP

#include "device_info.hpp"
// Standard C++ library
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "auxiliary/memory_heap_info.hpp"

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
std::span<MemoryHeapInfo> DeviceInfo::heapInfoList() noexcept
{
  return heap_info_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::span<const MemoryHeapInfo> DeviceInfo::heapInfoList() const noexcept
{
  return heap_info_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t DeviceInfo::maxNumOfFunctionParameters() noexcept
{
  return 256;
}

} // namespace zivc

#endif // ZIVC_DEVICE_INFO_INL_HPP
