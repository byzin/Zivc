/*!
  \file kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_INL_HPP
#define ZIVC_KERNEL_INL_HPP

#include "kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>
// Zivc
#include "kernel_common.hpp"
#include "kernel_set.hpp"
#include "utility/kernel_launch_options.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::Kernel(IdData&& id) noexcept :
    KernelCommon(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::~Kernel() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
argSize() const noexcept
{
  return numOfArgs();
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::destroy() noexcept
{
  destroyData();
  destroyObject();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
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
std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
dimensionSize() const noexcept
{
  return dimension();
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initialize(ZivcObject::SharedPtr&& parent,
           WeakPtr&& own,
           const Params& params)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initData(params);

  ZivcObject::setNameIfEmpty("Kernel");
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
makeOptions() const noexcept -> LaunchOptions
{
  return LaunchOptions{};
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
numOfArgs() noexcept
{
  const std::size_t size = sizeof...(Args);
  return size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::array<uint32b, 3> Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
expandWorkSize(const std::array<uint32b, kDim>& work_size) noexcept
{
  std::array<uint32b, 3> work_size_3d{{1, 1, 1}};
  for (std::size_t i = 0; i < kDim; ++i)
    work_size_3d[i] = work_size[i];
  return work_size_3d;
}

} // namespace zivc

#endif // ZIVC_KERNEL_INL_HPP
