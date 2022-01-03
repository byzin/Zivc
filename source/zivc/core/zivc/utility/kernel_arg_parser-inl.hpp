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
// Zisc
#include "zisc/concepts.hpp"
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
    -> ArgInfoList<kNumOfArgs>
{
  ArgInfoList<kNumOfArgs> info_list{};
  Impl::setArgInfo(0, 0, info_list.data());
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getLocalArgInfoList() noexcept
    -> ArgInfoList<kNumOfLocalArgs>
{
  ArgInfoList<kNumOfLocalArgs> info_list{};
  Impl::setLocalArgInfo(0, 0, info_list.data());
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getPodArgInfoList() noexcept
    -> ArgInfoList<kNumOfPodArgs>
{
  ArgInfoList<kNumOfPodArgs> info_list{};
  Impl::setPodArgInfo(0, 0, info_list.data());
  return info_list;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Args> inline
constexpr auto KernelArgParser<Args...>::getBufferArgInfoList() noexcept
    -> ArgInfoList<kNumOfBufferArgs>
{
  ArgInfoList<kNumOfBufferArgs> info_list{};
  Impl::setBufferArgInfo(0, 0, info_list.data());
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
  using ArgTypePack = TypePack<>;


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
  using ArgTypeInfo = KernelArgTypeInfo<Arg>;
  using NextParser = KernelArgParserImpl<RestArgs...>;


  //! Make a new kernel argument type pack
  template <typename ...Args>
  static constexpr auto makeArgTypePack(const TypePack<Args...>& pack)
  {
    if constexpr (ArgTypeInfo::kIsLocal) {
      return pack;
    }
    else {
      using ElementT = typename ArgTypeInfo::ElementType;
      using ArgT = std::conditional_t<ArgTypeInfo::kIsPod,
          std::add_const_t<ElementT>,
          std::add_lvalue_reference_t<Buffer<ElementT>>>;
      return TypePack<ArgT, Args...>{};
    }
  }

  //! Make an arg info
  static constexpr KernelArgInfo makeArgInfo() noexcept
  {
    return KernelArgInfo{ArgTypeInfo::kIsGlobal,
                         ArgTypeInfo::kIsLocal,
                         ArgTypeInfo::kIsConstant,
                         ArgTypeInfo::kIsPod,
                         ArgTypeInfo::kIsBuffer};
  }

 public:
  // The parsing results

  //!
  using ArgTypePack = decltype(makeArgTypePack(typename NextParser::ArgTypePack{}));


  static constexpr std::size_t kNumOfArgs = NextParser::kNumOfArgs + 1;
  static constexpr std::size_t kNumOfGlobalArgs = ArgTypeInfo::kIsGlobal
      ? NextParser::kNumOfGlobalArgs + 1
      : NextParser::kNumOfGlobalArgs;
  static constexpr std::size_t kNumOfLocalArgs = ArgTypeInfo::kIsLocal
      ? NextParser::kNumOfLocalArgs + 1
      : NextParser::kNumOfLocalArgs;
  static constexpr std::size_t kNumOfConstantArgs = ArgTypeInfo::kIsConstant
      ? NextParser::kNumOfConstantArgs + 1
      : NextParser::kNumOfConstantArgs;
  static constexpr std::size_t kNumOfPodArgs = ArgTypeInfo::kIsPod
      ? NextParser::kNumOfPodArgs + 1
      : NextParser::kNumOfPodArgs;
  static constexpr std::size_t kNumOfBufferArgs = ArgTypeInfo::kIsBuffer
      ? NextParser::kNumOfBufferArgs + 1
      : NextParser::kNumOfBufferArgs;


  //! Set a kernel arg info by the given index
  static constexpr void setArgInfo(const std::size_t position,
                                   std::size_t index,
                                   KernelArgInfo* info_list) noexcept
  {
    KernelArgInfo info = makeArgInfo();
    info.setIndex(position);
    info_list[index] = info;
    index = index + 1;
    if constexpr (1 <= NextParser::kNumOfArgs)
      NextParser::setArgInfo(position + 1, index, info_list);
  }

  //! Set a local kernel arg info by the given index
  static constexpr void setLocalArgInfo(const std::size_t position,
                                        std::size_t index,
                                        KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfo::kIsLocal) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info_list[index] = info;
      index = index + 1;
    }
    if constexpr (1 <= NextParser::kNumOfLocalArgs)
      NextParser::setLocalArgInfo(position + 1, index, info_list);
  }

  //! Set a POD kernel arg info by the given index
  static constexpr void setPodArgInfo(const std::size_t position,
                                      std::size_t index,
                                      KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfo::kIsPod) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info_list[index] = info;
      index = index + 1;
    }
    if constexpr (1 <= NextParser::kNumOfPodArgs)
      NextParser::setPodArgInfo(position + 1, index, info_list);
  }

  //! Set a buffer kernel arg info by the given index
  static constexpr void setBufferArgInfo(const std::size_t position,
                                         std::size_t index,
                                         KernelArgInfo* info_list) noexcept
  {
    if constexpr (ArgTypeInfo::kIsBuffer) {
      KernelArgInfo info = makeArgInfo();
      info.setIndex(position);
      info_list[index] = info;
      index = index + 1;
    }
    if constexpr (1 <= NextParser::kNumOfBufferArgs)
      NextParser::setBufferArgInfo(position + 1, index, info_list);
  }
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
