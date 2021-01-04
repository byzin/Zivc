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

} // namespace zivc

#endif // ZIVC_DEVICE_INFO_INL_HPP
