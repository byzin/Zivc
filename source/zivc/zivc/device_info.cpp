/*!
  \file device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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

  \param [in] other No description.
  */
DeviceInfo::DeviceInfo(DeviceInfo&& other) noexcept
{
  static_cast<void>(other);
}

/*!
  \details No detailed description
  */
DeviceInfo::~DeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
DeviceInfo& DeviceInfo::operator=(DeviceInfo&& other) noexcept
{
  static_cast<void>(other);
  return *this;
}

} // namespace zivc
