/*!
  \file device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "device_info.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
DeviceInfo::DeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
DeviceInfo::DeviceInfo([[maybe_unused]] DeviceInfo&& other) noexcept
{
}

/*!
  \details No detailed description
  */
DeviceInfo::~DeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
DeviceInfo& DeviceInfo::operator=([[maybe_unused]] DeviceInfo&& other) noexcept
{
  return *this;
}

} // namespace zivc
