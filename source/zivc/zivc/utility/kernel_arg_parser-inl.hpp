/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
#include "zisc/zisc_config.hpp"
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/cppcl/address_space_pointer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class CpuKernel;
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class VulkanKernel;

/*!
  \brief Global or Local type info

  No detailed description.

  \tparam kAddressType No description.
  \tparam Type No description.
  */
template <cl::AddressSpaceType kAddressType, typename Type>
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
};

/*!
  \details No detailed description
  */
inline
constexpr KernelArgParseResult::KernelArgParseResult() noexcept :
    is_global_{false},
    is_local_{false},
    is_constant_{false},
    is_pod_{false},
    index_{0}
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
                                                     const bool is_pod)
    noexcept :
        is_global_{is_global},
        is_local_{is_local},
        is_constant_{is_constant},
        is_pod_{is_pod},
        index_{0}
{
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
constexpr bool KernelArgParseResult::isPod() const noexcept
{
  return is_pod_;
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

  \tparam ArgType No description.
  \tparam RestTypes No description.
  */
template <typename ArgType, typename ...RestTypes>
class KernelArgParser<ArgType, RestTypes...>
{
  // Type aliases
  using Parent = KernelArgParser<RestTypes...>;
  using ArgInfo = KernelArgInfo<ArgType>;
  template <std::size_t kDimension, typename SetType>
  using ParentKernel = typename Parent::template KernelType<kDimension, SetType>;
  template <typename SetType>
  using PrevParameters = KernelParameters<SetType, RestTypes...>;
  template <typename SetType>
  using Parameters = KernelParameters<SetType, ArgType, RestTypes...>;
  template <typename ...Types> struct NewKernel;
  template <typename ...Types> struct ExtendedKernel;

  /*!
    \brief No brief description

    No detailed description.

    \tparam kDimension No description.
    \tparam SetType No description.
    \tparam Types No description.
    */
  template <std::size_t kDimension, typename SetType, typename ...Types>
  struct NewKernel<Kernel<kDimension, PrevParameters<SetType>, Types...>>
  {
    using KernelType = Kernel<kDimension, Parameters<SetType>, Types...>;
    using CpuKernelType = CpuKernel<kDimension, Parameters<SetType>, Types...>;
    using VulkanKernelType = VulkanKernel<kDimension, Parameters<SetType>, Types...>;
  };

  /*!
    \brief No brief description

    No detailed description.

    \tparam kDimension No description.
    \tparam SetType No description.
    \tparam Types No description.
    */
  template <std::size_t kDimension, typename SetType, typename ...Types>
  struct ExtendedKernel<Kernel<kDimension, PrevParameters<SetType>, Types...>>
  {
    using Arg = std::conditional_t<ArgInfo::kIsPod,
        std::add_const_t<typename ArgInfo::ElementType>,
        std::add_lvalue_reference_t<Buffer<typename ArgInfo::ElementType>>>;
    using KernelType = Kernel<kDimension, Parameters<SetType>, Arg, Types...>;
    using CpuKernelType = CpuKernel<kDimension, Parameters<SetType>, Arg, Types...>;
    using VulkanKernelType = VulkanKernel<kDimension, Parameters<SetType>, Arg, Types...>;
  };

  template <std::size_t kDimension, typename SetType>
  using KernelTypeHelper = std::conditional_t<ArgInfo::kIsLocal,
      NewKernel<ParentKernel<kDimension, SetType>>,
      ExtendedKernel<ParentKernel<kDimension, SetType>>>;

 public:
  // Type aliases
  template <std::size_t kSize>
  using ResultList = std::array<KernelArgParseResult, kSize>;
  template <std::size_t kDimension, typename SetType>
  using KernelType = typename KernelTypeHelper<kDimension, SetType>::KernelType;
  template <std::size_t kDimension, typename SetType>
  using SharedKernel = std::shared_ptr<KernelType<kDimension, SetType>>;
  template <std::size_t kDimension, typename SetType>
  using WeakKernel = std::weak_ptr<KernelType<kDimension, SetType>>;
  template <std::size_t kDimension, typename SetType>
  using CpuKernelType = typename KernelTypeHelper<kDimension, SetType>::CpuKernelType;
  template <std::size_t kDimension, typename SetType>
  using VulkanKernelType = typename KernelTypeHelper<kDimension, SetType>::VulkanKernelType;


  static constexpr std::size_t kNumOfArgs = Parent::kNumOfArgs + 1;
  static constexpr std::size_t kNumOfGlobalArgs = ArgInfo::kIsGlobal
      ? Parent::kNumOfGlobalArgs + 1
      : Parent::kNumOfGlobalArgs;
  static constexpr std::size_t kNumOfLocalArgs = ArgInfo::kIsLocal
      ? Parent::kNumOfLocalArgs + 1
      : Parent::kNumOfLocalArgs;
  static constexpr std::size_t kNumOfConstantArgs = ArgInfo::kIsConstant
      ? Parent::kNumOfConstantArgs + 1
      : Parent::kNumOfConstantArgs;
  static constexpr std::size_t kNumOfStorageBuffer =
      (ArgInfo::kIsGlobal && ArgInfo::kIsConstant)
          ? Parent::kNumOfStorageBuffer + 1
          : Parent::kNumOfStorageBuffer;
  static constexpr std::size_t kNumOfUniformBuffer = Parent::kNumOfUniformBuffer;


  //! Return the info of arguments
  static constexpr ResultList<kNumOfArgs> getArgInfoList() noexcept
  {
    ResultList<kNumOfArgs> result_list;
    if constexpr (0u < Parent::kNumOfArgs) {
      auto parent_list = Parent::getArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfArgs; ++i) {
        result_list[i + 1] = parent_list[i];
        result_list[i + 1].setIndex(i + 1);
      }
    }

    result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                          ArgInfo::kIsLocal,
                                          ArgInfo::kIsConstant,
                                          ArgInfo::kIsPod};
    result_list[0].setIndex(0);

    return result_list;
  }

  //! Return the info of global arguments
  static constexpr ResultList<kNumOfGlobalArgs> getGlobalArgInfoList() noexcept
  {
    ResultList<kNumOfGlobalArgs> result_list;
    if constexpr (0u < Parent::kNumOfGlobalArgs) {
      const std::size_t offset = ArgInfo::kIsGlobal ? 1u : 0u;
      auto parent_list = Parent::getGlobalArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfGlobalArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }

    if constexpr (ArgInfo::kIsGlobal) {
      result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                            ArgInfo::kIsLocal,
                                            ArgInfo::kIsConstant,
                                            ArgInfo::kIsPod};
      result_list[0].setIndex(0);
    }

    return result_list;
  }

  //! Return the info of local arguments
  static constexpr ResultList<kNumOfLocalArgs> getLocalArgInfoList() noexcept
  {
    ResultList<kNumOfLocalArgs> result_list;
    if constexpr (0u < Parent::kNumOfLocalArgs) {
      const std::size_t offset = ArgInfo::kIsLocal ? 1u : 0u;
      auto parent_list = Parent::getLocalArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfLocalArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }

    if constexpr (ArgInfo::kIsLocal) {
      result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                            ArgInfo::kIsLocal,
                                            ArgInfo::kIsConstant,
                                            ArgInfo::kIsPod};
      result_list[0].setIndex(0);
    }

    return result_list;
  }

  //! Check if there are const local arguments in the kernel arguments
  static constexpr bool hasConstLocal() noexcept
  {
    const bool is_const_local = std::is_const_v<ArgType> && ArgInfo::kIsLocal;
    return is_const_local || Parent::hasConstLocal();
  }
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_PARSER_INL_HPP
