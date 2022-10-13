/*!
  \file kernel_launch_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP
#define ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP

#include "kernel_launch_options.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <span>
#include <string_view>
#include <type_traits>
// Zivc
#include "id_data.hpp"
#include "launch_options.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions(const std::span<const uint32b, kDim>& work_size) noexcept :
    work_size_{work_size}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] queue_index No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions(const std::span<const uint32b, kDim>& work_size,
                    const uint32b queue_index) noexcept :
    LaunchOptions(queue_index),
    work_size_{work_size}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t
KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
globalIdOffset() const noexcept -> std::span<const uint32b, kDim>
{
  return global_id_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfArgs() noexcept
{
  const std::size_t s = sizeof...(Args);
  return s;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] dim No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setGlobalIdOffset(const uint32b offset, const std::size_t dim) noexcept
{
  global_id_offset_[dim] = offset;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setGlobalIdOffset(const std::span<const uint32b, kDim>& offset) noexcept
{
  std::copy_n(offset.begin(), offset.size(), global_id_offset_.begin());
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] dim No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setWorkSize(const uint32b work_size, const std::size_t dim) noexcept
{
  work_size_[dim] = work_size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setWorkSize(const std::span<const uint32b, kDim>& work_size) noexcept
{
  std::copy_n(work_size.begin(), work_size.size(), work_size_.begin());
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
workSize() const noexcept -> std::span<const uint32b, kDim>
{
  return work_size_;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initialize() noexcept
{
  work_size_.fill(1);
  global_id_offset_.fill(0);
  setLabel(KSet::name());
}

} // namespace zivc

#endif // ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP
