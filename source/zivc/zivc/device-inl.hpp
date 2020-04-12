/*!
  \file device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INL_HPP
#define ZIVC_DEVICE_INL_HPP

#include "device.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "buffer.hpp"
#include "zivc_config.hpp"
#include "utility/kernel_arg_parser.hpp"

namespace zivc {

// Forward declaration
template <typename T>
SharedBuffer<T> makeBuffer(Device* device, const BufferUsage flag);
template <std::size_t kDimension, typename SetType, typename ...ArgTypes>
SharedKernel<kDimension, SetType, ArgTypes...> makeKernel(
    Device* device,
    const KernelParameters<SetType, ArgTypes...>& parameters);

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> Device::makeBuffer(const BufferUsage flag)
{
  auto buffer = zivc::makeBuffer<T>(this, flag);
  return buffer;
}

/*!
  \details No detailed description

  \param [in] parameters No description.
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...ArgTypes> inline
auto Device::makeKernel(const KernelParameters<SetType, ArgTypes...>& parameters)
    -> SharedKernel<kDimension, SetType, ArgTypes...>
{
  auto kernel = zivc::makeKernel<kDimension, SetType, ArgTypes...>(this,
                                                                     parameters);
  return kernel;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const DeviceInfo& Device::deviceInfo() const noexcept
{
  return *device_info_;
}

} // namespace zivc

#endif // ZIVC_DEVICE_INL_HPP
