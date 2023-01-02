/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_PARSER_INL_HPP
#define ZIVC_KERNEL_ARG_PARSER_INL_HPP

#include "kernel_arg_parser.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "kernel_arg_info.hpp"
#include "kernel_arg_pack.hpp"
#include "kernel_arg_type_info.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::internal {

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
auto KernelArgParser<Args...>::packArguments() noexcept
{
  KernelArgPack pack = (KernelArgPack<Args>{} + ... + KernelArgPack<>{});
  return pack;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getArgInfoListImpl() noexcept
    -> ArgInfoListImplT<kNumOfArgsImpl>
{
  ArgInfoListImplT<kNumOfArgsImpl> info_list{createArgInfo<Args>()...};
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t index = 0; index < info_list.size(); ++index) {
    KernelArgInfo& info = info_list[index];
    info.setIndex(index);
    info.setLocalOffset(local_offset);
    info.setPodOffset(pod_offset);
    local_offset = info.isLocal() ? local_offset + 1 : local_offset;
    pod_offset = info.isPod() ? pod_offset + 1 : pod_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr std::size_t KernelArgParser<Args...>::numOfGlobalArgs() noexcept
{
  const std::array info_list = getArgInfoListImpl();
  std::size_t n = 0;
  for (const KernelArgInfo& info : info_list)
    n = info.isGlobal() ? n + 1 : n;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr std::size_t KernelArgParser<Args...>::numOfLocalArgs() noexcept
{
  const std::array info_list = getArgInfoListImpl();
  std::size_t n = 0;
  for (const KernelArgInfo& info : info_list)
    n = info.isLocal() ? n + 1 : n;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr std::size_t KernelArgParser<Args...>::numOfPodArgs() noexcept
{
  const std::array info_list = getArgInfoListImpl();
  std::size_t n = 0;
  for (const KernelArgInfo& info : info_list)
    n = info.isPod() ? n + 1 : n;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr std::size_t KernelArgParser<Args...>::numOfBufferArgs() noexcept
{
  const std::array info_list = getArgInfoListImpl();
  std::size_t n = 0;
  for (const KernelArgInfo& info : info_list)
    n = info.isBuffer() ? n + 1 : n;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr std::size_t KernelArgParser<Args...>::numOfParameterArgs() noexcept
{
  const std::array info_list = getArgInfoListImpl();
  std::size_t n = 0;
  for (const KernelArgInfo& info : info_list)
    n = info.isParameter() ? n + 1 : n;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getArgInfoList() noexcept
    -> ArgInfoListT<kNumOfArgs>
{
  return getArgInfoListImpl();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getGlobalArgInfoList() noexcept
    -> ArgInfoListT<kNumOfGlobalArgs>
{
  const std::array arg_info_list = getArgInfoList();
  ArgInfoListT<kNumOfGlobalArgs> info_list{};
  std::size_t index = 0;
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t position = 0; position < arg_info_list.size(); ++position) {
    const KernelArgInfo& arg_info = arg_info_list[position];
    if (arg_info.isGlobal()) {
      KernelArgInfo& info = info_list[index++];
      info = arg_info;
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info.setPodOffset(pod_offset);
    }
    local_offset = arg_info.isLocal() ? local_offset + 1 : local_offset;
    pod_offset = arg_info.isPod() ? pod_offset + 1 : pod_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getLocalArgInfoList() noexcept
    -> ArgInfoListT<kNumOfLocalArgs>
{
  const std::array arg_info_list = getArgInfoList();
  ArgInfoListT<kNumOfLocalArgs> info_list{};
  std::size_t index = 0;
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t position = 0; position < arg_info_list.size(); ++position) {
    const KernelArgInfo& arg_info = arg_info_list[position];
    if (arg_info.isLocal()) {
      KernelArgInfo& info = info_list[index++];
      info = arg_info;
      info.setIndex(position);
      info.setLocalOffset(local_offset++);
      info.setPodOffset(pod_offset);
    }
    pod_offset = arg_info.isPod() ? pod_offset + 1 : pod_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getPodArgInfoList() noexcept
    -> ArgInfoListT<kNumOfPodArgs>
{
  const std::array arg_info_list = getArgInfoList();
  ArgInfoListT<kNumOfPodArgs> info_list{};
  std::size_t index = 0;
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t position = 0; position < arg_info_list.size(); ++position) {
    const KernelArgInfo& arg_info = arg_info_list[position];
    if (arg_info.isPod()) {
      KernelArgInfo& info = info_list[index++];
      info = arg_info;
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info.setPodOffset(pod_offset++);
    }
    local_offset = arg_info.isLocal() ? local_offset + 1 : local_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getBufferArgInfoList() noexcept
    -> ArgInfoListT<kNumOfBufferArgs>
{
  const std::array arg_info_list = getArgInfoList();
  ArgInfoListT<kNumOfBufferArgs> info_list{};
  std::size_t index = 0;
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t position = 0; position < arg_info_list.size(); ++position) {
    const KernelArgInfo& arg_info = arg_info_list[position];
    if (arg_info.isBuffer()) {
      KernelArgInfo& info = info_list[index++];
      info = arg_info;
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info.setPodOffset(pod_offset);
    }
    local_offset = arg_info.isLocal() ? local_offset + 1 : local_offset;
    pod_offset = arg_info.isPod() ? pod_offset + 1 : pod_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getParameterArgInfoList() noexcept
    -> ArgInfoListT<kNumOfParameterArgs>
{
  const std::array arg_info_list = getArgInfoList();
  ArgInfoListT<kNumOfParameterArgs> info_list{};
  std::size_t index = 0;
  std::size_t local_offset = 0;
  std::size_t pod_offset = 0;
  for (std::size_t position = 0; position < arg_info_list.size(); ++position) {
    const KernelArgInfo& arg_info = arg_info_list[position];
    if (arg_info.isParameter()) {
      KernelArgInfo& info = info_list[index++];
      info = arg_info;
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info.setPodOffset(pod_offset);
    }
    local_offset = arg_info.isLocal() ? local_offset + 1 : local_offset;
    pod_offset = arg_info.isPod() ? pod_offset + 1 : pod_offset;
  }
  return info_list;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \return No description
  */
template <typename Type> inline
constexpr KernelArgInfo createArgInfo() noexcept
{
  using InfoT = KernelArgTypeInfo<Type>;
  return KernelArgInfo{InfoT::kIsGlobal,
                       InfoT::kIsLocal,
                       InfoT::kIsConstant,
                       InfoT::kIsPod};
}

} // namespace zivc::internal

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
