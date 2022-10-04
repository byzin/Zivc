/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include "kernel_arg_type_info.hpp"
#include "type_pack.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getArgInfoList() noexcept
    -> ArgInfoListT<kNumOfArgs>
{
  ArgInfoListT<kNumOfArgs> info_list{};
  ImplT::setArgInfo(0, 0, 0, info_list.data());
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
  ArgInfoListT<kNumOfLocalArgs> info_list{};
  ImplT::setLocalArgInfo(0, 0, 0, info_list.data());
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
  ArgInfoListT<kNumOfPodArgs> info_list{};
  ImplT::setPodArgInfo(0, 0, 0, info_list.data());
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
  ArgInfoListT<kNumOfBufferArgs> info_list{};
  ImplT::setBufferArgInfo(0, 0, 0, info_list.data());
  return info_list;
}

/*!
  \brief Type information of kernel arguments

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
class KernelArgParserImpl
{
 public:
  // The parsing results

  //!
  using ArgTypePackT = TypePack<>;


  static constexpr std::size_t kNumOfArgs = 0; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = 0; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = 0; //!< The number of locals
  static constexpr std::size_t kNumOfConstantArgs = 0; //!< The number of constants
  static constexpr std::size_t kNumOfPodArgs = 0; //!< The number of PODs
  static constexpr std::size_t kNumOfBufferArgs = 0; //!< The number of buffers
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Arg No description.
  \tparam RestArgs No description.
  */
template <typename Arg, typename ...RestArgs>
class KernelArgParserImpl<Arg, RestArgs...>
{
  // Type aliases
  using ArgTypeInfoT = KernelArgTypeInfo<Arg>;
  using NextParserT = KernelArgParserImpl<RestArgs...>;


  //! Make a new kernel argument type pack
  template <typename ...Args>
  static constexpr auto makeArgTypePack(const TypePack<Args...>& pack)
  {
    if constexpr (ArgTypeInfoT::kIsLocal) {
      return pack;
    }
    else {
      using ElementT = typename ArgTypeInfoT::ElementT;
      using ArgT = std::conditional_t<ArgTypeInfoT::kIsPod,
          std::add_const_t<ElementT>,
          std::add_lvalue_reference_t<Buffer<ElementT>>>;
      return TypePack<ArgT, Args...>{};
    }
  }

  //! Make an arg info
  static constexpr KernelArgInfo makeArgInfo() noexcept
  {
    return KernelArgInfo{ArgTypeInfoT::kIsGlobal,
                         ArgTypeInfoT::kIsLocal,
                         ArgTypeInfoT::kIsConstant,
                         ArgTypeInfoT::kIsPod,
                         ArgTypeInfoT::kIsBuffer};
  }

 public:
  // The parsing results

  //!
  using ArgTypePackT = decltype(makeArgTypePack(typename NextParserT::ArgTypePackT{}));


  static constexpr std::size_t kNumOfArgs = NextParserT::kNumOfArgs + 1;
  static constexpr std::size_t kNumOfGlobalArgs = ArgTypeInfoT::kIsGlobal
      ? NextParserT::kNumOfGlobalArgs + 1
      : NextParserT::kNumOfGlobalArgs;
  static constexpr std::size_t kNumOfLocalArgs = ArgTypeInfoT::kIsLocal
      ? NextParserT::kNumOfLocalArgs + 1
      : NextParserT::kNumOfLocalArgs;
  static constexpr std::size_t kNumOfConstantArgs = ArgTypeInfoT::kIsConstant
      ? NextParserT::kNumOfConstantArgs + 1
      : NextParserT::kNumOfConstantArgs;
  static constexpr std::size_t kNumOfPodArgs = ArgTypeInfoT::kIsPod
      ? NextParserT::kNumOfPodArgs + 1
      : NextParserT::kNumOfPodArgs;
  static constexpr std::size_t kNumOfBufferArgs = ArgTypeInfoT::kIsBuffer
      ? NextParserT::kNumOfBufferArgs + 1
      : NextParserT::kNumOfBufferArgs;


  //! Set a kernel arg info by the given index
  static constexpr void setArgInfo(const std::size_t position,
                                   std::size_t index,
                                   std::size_t local_offset,
                                   KernelArgInfo* info_list) noexcept
  {
    KernelArgInfo info = makeArgInfo();
    info.setIndex(position);
    info.setLocalOffset(local_offset);
    info_list[index] = info;
    index = index + 1;
    local_offset = ArgTypeInfoT::kIsLocal ? local_offset + 1 : local_offset;
    if constexpr (1 <= NextParserT::kNumOfArgs)
      NextParserT::setArgInfo(position + 1, index, local_offset, info_list);
  }

  //! Set a local kernel arg info by the given index
  static constexpr void setLocalArgInfo(const std::size_t position,
                                        std::size_t index,
                                        std::size_t local_offset,
                                        KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfoT::kIsLocal) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info_list[index] = info;
      index = index + 1;
      local_offset = local_offset + 1;
    }
    if constexpr (1 <= NextParserT::kNumOfLocalArgs)
      NextParserT::setLocalArgInfo(position + 1, index, local_offset, info_list);
  }

  //! Set a POD kernel arg info by the given index
  static constexpr void setPodArgInfo(const std::size_t position,
                                      std::size_t index,
                                      std::size_t local_offset,
                                      KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfoT::kIsPod) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info_list[index] = info;
      index = index + 1;
    }
    local_offset = ArgTypeInfoT::kIsLocal ? local_offset + 1 : local_offset;
    if constexpr (1 <= NextParserT::kNumOfPodArgs)
      NextParserT::setPodArgInfo(position + 1, index, local_offset, info_list);
  }

  //! Set a buffer kernel arg info by the given index
  static constexpr void setBufferArgInfo(const std::size_t position,
                                         std::size_t index,
                                         std::size_t local_offset,
                                         KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfoT::kIsBuffer) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info.setLocalOffset(local_offset);
      info_list[index] = info;
      index = index + 1;
    }
    local_offset = ArgTypeInfoT::kIsLocal ? local_offset + 1 : local_offset;
    if constexpr (1 <= NextParserT::kNumOfBufferArgs)
      NextParserT::setBufferArgInfo(position + 1, index, local_offset, info_list);
  }
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
