/*!
  \file kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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
#include <span>
#include <string_view>
#include <type_traits>
// Zivc
#include "kernel_set.hpp"
#include "zivc_config.hpp"
#include "auxiliary/kernel_common.hpp"
#include "auxiliary/kernel_launch_options.hpp"
#include "auxiliary/id_data.hpp"
#include "auxiliary/zivc_object.hpp"
#include "internal/kernel_arg_parser.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::Kernel(IdData&& id) noexcept :
    KernelCommon(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::~Kernel() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
argSize() const noexcept
{
  return numOfArgs();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
bufferSize() const noexcept
{
  return numOfBuffers();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
createOptions() const noexcept -> LaunchOptionsT
{
  return LaunchOptionsT{};
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::destroy() noexcept
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
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
dimensionSize() const noexcept
{
  return dimension();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
hasGlobalArg() noexcept
{
  const bool result = 0 < ArgParserT::kNumOfGlobalArgs;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
hasLocalArg() noexcept
{
  const bool result = 0 < ArgParserT::kNumOfLocalArgs;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
hasPodArg() noexcept
{
  const bool result = 0 < ArgParserT::kNumOfPodArgs;
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initialize(ZivcObject::SharedPtr&& parent,
           WeakPtr&& own,
           const ParamsT& params)
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
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
localSize() const noexcept
{
  return numOfLocals();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfArgs() noexcept
{
  const std::size_t size = sizeof...(Args);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfBuffers() noexcept
{
  constexpr std::size_t size = ArgParserT::kNumOfBufferArgs;
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfLocals() noexcept
{
  constexpr std::size_t size = ArgParserT::kNumOfLocalArgs;
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfPods() noexcept
{
  constexpr std::size_t size = ArgParserT::kNumOfPodArgs;
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
podSize() const noexcept
{
  return numOfBuffers();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::array<uint32b, 3> Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
expandWorkSize(const std::span<const uint32b, kDim>& work_size,
               const uint32b fill_value) noexcept
{
  std::array<uint32b, 3> work_size_3d{{fill_value, fill_value, fill_value}};
  for (std::size_t i = 0; i < kDim; ++i)
    work_size_3d[i] = work_size[i];
  return work_size_3d;
}

} // namespace zivc

#endif // ZIVC_KERNEL_INL_HPP
