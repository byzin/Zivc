/*!
  \file kernel_params_inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_PARAMS_INL_HPP
#define ZIVC_KERNEL_PARAMS_INL_HPP

#include "kernel_params.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>
// Zisc
#include "zisc/error.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/kernel_set.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
KernelParams<kDim, KSet, Args...>::KernelParams(Function ptr,
                                                std::string_view kernel_name) noexcept
    : function_{ptr}
{
  initialize(kernel_name);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
constexpr std::size_t KernelParams<kDim, KSet, Args...>::dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto KernelParams<kDim, KSet, Args...>::func() const noexcept -> Function
{
  return function_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
std::string_view KernelParams<kDim, KSet, Args...>::kernelName() const noexcept
{
  std::string_view name{kernel_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
constexpr std::size_t KernelParams<kDim, KSet, Args...>::maxKernelNameLength() noexcept
{
  return IdData::maxNameLength();
}

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelParams<kDim, KSet, Args...>::setFunc(Function ptr) noexcept
{
  function_ = ptr;
}

/*!
  \details No detailed description

  \param [in] kernel_name No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelParams<kDim, KSet, Args...>::setKernelName(
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
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelParams<kDim, KSet, Args...>::initialize(
    std::string_view kernel_name) noexcept
{
  kernel_name_.fill('\0');
  setKernelName(kernel_name);
}

} // namespace zivc

#endif // ZIVC_KERNEL_PARAMS_INL_HPP
