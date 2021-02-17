/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
#include "zisc/zisc_config.hpp"
// Zivc
#include "zivc/kernel_set.hpp"
#include "zivc/cppcl/address_space_pointer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief Information of address space kernel argument

  No detailed description.

  \tparam kASpaceType No description.
  \tparam Type No description.
  */
template <cl::AddressSpaceType kASpaceType, KernelArg Type>
class KernelArgTypeInfo<cl::AddressSpacePointer<kASpaceType, Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = kASpaceType == cl::AddressSpaceType::kGlobal;
  static constexpr bool kIsLocal = kASpaceType == cl::AddressSpaceType::kLocal;
  static constexpr bool kIsConstant = kASpaceType == cl::AddressSpaceType::kConstant;
  static constexpr bool kIsPod = false;
  static constexpr bool kIsBuffer = kIsGlobal || kIsConstant;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
  static_assert(kASpaceType != cl::AddressSpaceType::kPrivate,
                "Private address space argument isn't allowed.");
};

/*!
  \brief Information of buffer kernel argument

  No detailed description.

  \tparam Type No description.
  */
template <KernelArg Type>
class KernelArgTypeInfo<Buffer<Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = true;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = false;
  static constexpr bool kIsBuffer = kIsGlobal || kIsConstant;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
};

/*!
  \details No detailed description
  */
inline
constexpr KernelArgInfo::KernelArgInfo() noexcept :
    index_{0},
    is_global_{zisc::kFalse},
    is_local_{zisc::kFalse},
    is_constant_{zisc::kFalse},
    is_pod_{zisc::kFalse},
    is_buffer_{zisc::kFalse}
{
}

/*!
  \details No detailed description

  \param [in] is_global No description.
  \param [in] is_local No description.
  \param [in] is_constant No description.
  \param [in] is_pod No description.
  \param [in] is_buffer No description.
  */
inline
constexpr KernelArgInfo::KernelArgInfo(const bool is_global,
                                       const bool is_local,
                                       const bool is_constant,
                                       const bool is_pod,
                                       const bool is_buffer) noexcept :
    index_{0},
    is_global_{is_global ? zisc::kTrue : zisc::kFalse},
    is_local_{is_local ? zisc::kTrue : zisc::kFalse},
    is_constant_{is_constant ? zisc::kTrue : zisc::kFalse},
    is_pod_{is_pod ? zisc::kTrue : zisc::kFalse},
    is_buffer_{is_buffer ? zisc::kTrue : zisc::kFalse}
{
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isGlobal() const noexcept
{
  return is_global_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isLocal() const noexcept
{
  return is_local_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isConstant() const noexcept
{
  return is_constant_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isPod() const noexcept
{
  return is_pod_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isBuffer() const noexcept
{
  return is_buffer_;
}
/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t KernelArgInfo::index() const noexcept
{
  return index_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
inline
constexpr void KernelArgInfo::setIndex(const std::size_t index) noexcept
{
  index_ = index;
}

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
  using ArgTypePack = KernelArgTypePack<>;


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
  static constexpr auto makeArgTypePack(const KernelArgTypePack<Args...>& pack)
  {
    if constexpr (ArgTypeInfo::kIsLocal) {
      return pack;
    }
    else {
      using ElementT = typename ArgTypeInfo::ElementType;
      using ArgT = std::conditional_t<ArgTypeInfo::kIsPod,
          std::add_const_t<ElementT>,
          std::add_lvalue_reference_t<Buffer<ElementT>>>;
      return KernelArgTypePack<ArgT, Args...>{};
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

/*!
  \brief Pack kernel argument types

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
struct KernelArgTypePack
{
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
