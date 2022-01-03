/*!
  \file kernel_init_params_inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_INIT_PARAMS_INL_HPP
#define ZIVC_KERNEL_INIT_PARAMS_INL_HPP

#include "kernel_init_params.hpp"
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
KernelInitParams<kDim, KSet, Args...>::KernelInitParams(Function ptr,
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
constexpr std::size_t KernelInitParams<kDim, KSet, Args...>::dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto KernelInitParams<kDim, KSet, Args...>::func() const noexcept -> Function
{
  return function_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
std::string_view KernelInitParams<kDim, KSet, Args...>::kernelName() const noexcept
{
  std::string_view name{kernel_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
constexpr std::size_t KernelInitParams<kDim, KSet, Args...>::maxKernelNameLength() noexcept
{
  return IdData::maxNameLength();
}

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelInitParams<kDim, KSet, Args...>::setFunc(Function ptr) noexcept
{
  function_ = ptr;
}

/*!
  \details No detailed description

  \param [in] kernel_name No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelInitParams<kDim, KSet, Args...>::setKernelName(
    std::string_view kernel_name) noexcept
{
  const std::size_t s = kernel_name.size();
  ZISC_ASSERT(kernel_name.size() < maxKernelNameLength(),
              "The kernel '", kernel_name, "' exceed the limit of name length. ",
              maxKernelNameLength(), " < n=", kernel_name.size(), ".");
  copyStr(kernel_name.data(), kernel_name_.data());
  kernel_name_[s] = '\0';
}

/*!
  \details No detailed description

  \param [in] command_buffer_ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelInitParams<kDim, KSet, Args...>::setVulkanCommandBufferPtr(
    const void* command_buffer_ptr) noexcept
{
  command_buffer_ptr_ = command_buffer_ptr;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
const void* KernelInitParams<kDim, KSet, Args...>::vulkanCommandBufferPtr()
    const noexcept
{
  return command_buffer_ptr_;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void KernelInitParams<kDim, KSet, Args...>::initialize(
    std::string_view kernel_name) noexcept
{
  kernel_name_.fill('\0');
  setKernelName(kernel_name);
}

} // namespace zivc

#endif // ZIVC_KERNEL_INIT_PARAMS_INL_HPP
