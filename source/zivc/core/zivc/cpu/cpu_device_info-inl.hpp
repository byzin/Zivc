/*!
  \file cpu_device_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_DEVICE_INOF_INL_HPP
#define ZIVC_CPU_DEVICE_INOF_INL_HPP

#include "cpu_device_info.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b CpuDeviceInfo::maxWorkGroupSize() noexcept
{
  //! \todo Resolve dynamic storage allocation for local address variables in kernels
  //! \todo Investigate C++20 coroutine for work group impelemntation
  //return 4;
  return 1;
}

} /* namespace zivc */

#endif /* ZIVC_CPU_DEVICE_INOF_INL_HPP */
