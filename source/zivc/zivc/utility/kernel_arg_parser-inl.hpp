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
  \brief Global or Local type info

  No detailed description.

  \tparam kAddressType No description.
  \tparam Type No description.
  */
template <cl::AddressSpaceType kAddressType, zisc::TriviallyCopyable Type>
class AddressSpaceInfo<cl::AddressSpacePointer<kAddressType, Type>>
{
  using ASpaceType = cl::AddressSpaceType;

 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = kAddressType == ASpaceType::kGlobal;
  static constexpr bool kIsLocal = kAddressType == ASpaceType::kLocal;
  static constexpr bool kIsConstant = kAddressType == ASpaceType::kConstant;
  static constexpr bool kIsPod = false;
  static_assert(kIsGlobal || kIsLocal || kIsConstant, "The address space is wrong.");

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
  static_assert(std::is_trivially_copyable_v<ElementType>,
                "The element type isn't trivially copyable.");
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <zisc::TriviallyCopyable Type>
class AddressSpaceInfo<Buffer<Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = false;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = false;
};

/*!
  \details No detailed description
  */
inline
constexpr KernelArgParseResult::KernelArgParseResult() noexcept :
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
  \param [in] is_pod No description.
  \param [in] is_constant No description.
  */
inline
constexpr KernelArgParseResult::KernelArgParseResult(const bool is_global,
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
constexpr bool KernelArgParseResult::isGlobal() const noexcept
{
  return is_global_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgParseResult::isLocal() const noexcept
{
  return is_local_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgParseResult::isConstant() const noexcept
{
  return is_constant_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgParseResult::isPod() const noexcept
{
  return is_pod_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgParseResult::isBuffer() const noexcept
{
  return is_buffer_;
}
/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t KernelArgParseResult::index() const noexcept
{
  return index_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
inline
constexpr void KernelArgParseResult::setIndex(const std::size_t index) noexcept
{
  index_ = index;
}

/*!
  \brief No brief description

  No detailed description.

  \tparam Arg No description.
  \tparam RestArgs No description.
  */
template <typename Arg, typename ...RestArgs>
class KernelArgParser<Arg, RestArgs...>
{
  // Type aliases
  using ArgInfo = KernelArgInfo<Arg>;
  template <std::size_t kDim, DerivedKSet KSet>
  using Params = KernelParams<kDim, KSet, Arg, RestArgs...>;
  using NextParser = KernelArgParser<RestArgs...>;
  template <std::size_t kDim, DerivedKSet KSet>
  using NextParams = KernelParams<kDim, KSet, RestArgs...>;
  template <std::size_t kDim, DerivedKSet KSet>
  using NextKernel = typename NextParser::template KernelType<kDim, KSet>;

  template <typename> struct KernelT;
  /*!
    \brief No brief description

    No detailed description.

    \tparam kDim No description.
    \tparam KSet No description.
    \tparam Args No description.
    */
  template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
  struct KernelT<Kernel<NextParams<kDim, KSet>, Args...>>
  {
    using Type = Kernel<Params<kDim, KSet>, Args...>;
    template <template<typename, typename...> typename Derived>
    using DerivedType = Derived<Params<kDim, KSet>, Args...>;
  };

  template <typename> struct ExtendedKernelT;
  /*!
    \brief No brief description

    No detailed description.

    \tparam kDim No description.
    \tparam KSet No description.
    \tparam Args No description.
    */
  template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
  struct ExtendedKernelT<Kernel<NextParams<kDim, KSet>, Args...>>
  {
    using ArgT = std::conditional_t<ArgInfo::kIsPod,
        std::add_const_t<typename ArgInfo::ElementType>,
        std::add_lvalue_reference_t<Buffer<typename ArgInfo::ElementType>>>;
    using Type = Kernel<Params<kDim, KSet>, ArgT, Args...>;
    template <template<typename, typename...> typename Derived>
    using DerivedType = Derived<Params<kDim, KSet>, ArgT, Args...>;
  };

  template <std::size_t kDim, DerivedKSet KSet>
  using KernelTypeHelper = std::conditional_t<ArgInfo::kIsLocal,
      KernelT<NextKernel<kDim, KSet>>,
      ExtendedKernelT<NextKernel<kDim, KSet>>>;

  //! Make the parse result of the Arg
  static constexpr KernelArgParseResult makeArgParseResult() noexcept
  {
    return KernelArgParseResult{ArgInfo::kIsGlobal,
                                ArgInfo::kIsLocal,
                                ArgInfo::kIsConstant,
                                ArgInfo::kIsPod,
                                ArgInfo::kIsBuffer};
  }

 public:
  // Type aliases
  template <std::size_t kSize>
  using ResultList = std::array<KernelArgParseResult, kSize>;
  template <std::size_t kDim, DerivedKSet KSet>
  using KernelType = typename KernelTypeHelper<kDim, KSet>::Type;
  template <template<typename, typename...> typename Derived,
            std::size_t kDim,
            DerivedKSet KSet>
  using DerivedKernelType = typename KernelTypeHelper<kDim, KSet>::
                                template DerivedType<Derived>;


  static constexpr std::size_t kNumOfArgs = NextParser::kNumOfArgs + 1;
  static constexpr std::size_t kNumOfGlobalArgs = ArgInfo::kIsGlobal
      ? NextParser::kNumOfGlobalArgs + 1
      : NextParser::kNumOfGlobalArgs;
  static constexpr std::size_t kNumOfLocalArgs = ArgInfo::kIsLocal
      ? NextParser::kNumOfLocalArgs + 1
      : NextParser::kNumOfLocalArgs;
  static constexpr std::size_t kNumOfConstantArgs = ArgInfo::kIsConstant
      ? NextParser::kNumOfConstantArgs + 1
      : NextParser::kNumOfConstantArgs;
  static constexpr std::size_t kNumOfPodArgs = ArgInfo::kIsPod
      ? NextParser::kNumOfPodArgs + 1
      : NextParser::kNumOfPodArgs;
  static constexpr std::size_t kNumOfBufferArgs = ArgInfo::kIsBuffer
      ? NextParser::kNumOfBufferArgs + 1
      : NextParser::kNumOfBufferArgs;


  //! Return the info of arguments
  static constexpr ResultList<kNumOfArgs> getArgInfoList() noexcept
  {
    ResultList<kNumOfArgs> result_list{};
    if constexpr (0u < NextParser::kNumOfArgs) {
      auto parent_list = NextParser::getArgInfoList();
      for (std::size_t i = 0; i < NextParser::kNumOfArgs; ++i) {
        result_list[i + 1] = parent_list[i];
        result_list[i + 1].setIndex(i + 1);
      }
    }
    result_list[0] = makeArgParseResult();
    result_list[0].setIndex(0);
    return result_list;
  }

  //! Return the info of local arguments
  static constexpr ResultList<kNumOfLocalArgs> getLocalArgInfoList() noexcept
  {
    ResultList<kNumOfLocalArgs> result_list{};
    const std::size_t offset = ArgInfo::kIsLocal ? 1u : 0u;
    if constexpr (0u < NextParser::kNumOfLocalArgs) {
      auto parent_list = NextParser::getLocalArgInfoList();
      for (std::size_t i = 0; i < NextParser::kNumOfLocalArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }
    if constexpr (offset) {
      result_list[0] = makeArgParseResult();
      result_list[0].setIndex(0);
    }
    return result_list;
  }

  //! Return the info of POD arguments
  static constexpr ResultList<kNumOfPodArgs> getPodArgInfoList() noexcept
  {
    ResultList<kNumOfPodArgs> result_list{};
    const std::size_t offset = ArgInfo::kIsPod ? 1u : 0u;
    if constexpr (0u < NextParser::kNumOfPodArgs) {
      auto parent_list = NextParser::getPodArgInfoList();
      for (std::size_t i = 0; i < NextParser::kNumOfPodArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }
    if constexpr (offset) {
      result_list[0] = makeArgParseResult();
      result_list[0].setIndex(0);
    }
    return result_list;
  }

  //! Return the info of buffer arguments
  static constexpr ResultList<kNumOfBufferArgs> getBufferArgInfoList() noexcept
  {
    ResultList<kNumOfBufferArgs> result_list{};
    const std::size_t offset = ArgInfo::kIsBuffer ? 1u : 0u;
    if constexpr (0u < NextParser::kNumOfBufferArgs) {
      auto parent_list = NextParser::getBufferArgInfoList();
      for (std::size_t i = 0; i < NextParser::kNumOfBufferArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }
    if constexpr (offset) {
      result_list[0] = makeArgParseResult();
      result_list[0].setIndex(0);
    }
    return result_list;
  }


  //! Check if there are const local arguments in the kernel arguments
  static constexpr bool hasConstLocal() noexcept
  {
    const bool is_const_local = std::is_const_v<Arg> && ArgInfo::kIsLocal;
    return is_const_local || NextParser::hasConstLocal();
  }
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
