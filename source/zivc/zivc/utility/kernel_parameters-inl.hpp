/*!
  \file kernel_parameters_inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_PARAMETERS_INL_HPP
#define ZIVC_KERNEL_PARAMETERS_INL_HPP

#include "kernel_parameters.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>
// Zisc
#include "id_data.hpp"
#include "zisc/error.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <typename SetType, typename ...ArgTypes> inline
KernelParameters<SetType, ArgTypes...>::KernelParameters(
    Function ptr,
    std::string_view kernel_name) noexcept :
        function_{ptr}
{
  initialize(kernel_name);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType, typename ...ArgTypes> inline
auto KernelParameters<SetType, ArgTypes...>::func() const noexcept -> Function
{
  return function_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType, typename ...ArgTypes> inline
std::string_view KernelParameters<SetType, ArgTypes...>::kernelName() const noexcept
{
  std::string_view name{kernel_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType, typename ...ArgTypes> inline
constexpr std::size_t KernelParameters<SetType, ArgTypes...>::maxKernelNameLength() noexcept
{
  return IdData::maxNameLength();
}

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <typename SetType, typename ...ArgTypes> inline
void KernelParameters<SetType, ArgTypes...>::setFunc(Function ptr) noexcept
{
  function_ = ptr;
}

/*!
  \details No detailed description

  \param [in] kernel_name No description.
  */
template <typename SetType, typename ...ArgTypes> inline
void KernelParameters<SetType, ArgTypes...>::setKernelName(
    std::string_view kernel_name) noexcept
{
  const std::size_t s = kernel_name.size();
  ZISC_ASSERT(s < maxKernelNameLength(),
              "The kernel '", kernel_name, "' exceed the limit of name length. ",
              maxKernelNameLength(), " < n=", s, ".");
  std::strncpy(kernel_name_.data(), kernel_name.data(), s);
  kernel_name_[s] = '\0';
}

/*!
  \details No detailed description
  */
template <typename SetType, typename ...ArgTypes> inline
void KernelParameters<SetType, ArgTypes...>::initialize(
    std::string_view kernel_name) noexcept
{
  kernel_name_.fill('\0');
  setKernelName(kernel_name);
}

} // namespace zivc

#endif // ZIVC_KERNEL_PARAMETERS_INL_HPP
