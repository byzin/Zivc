/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_PARSER_HPP
#define ZIVC_KERNEL_ARG_PARSER_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <std::size_t, DerivedKSet, typename...> class KernelParams;
template <typename, typename...> class Kernel;
template <typename> class Buffer;

/*!
  \brief POD type info

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class AddressSpaceInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = false;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = true;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
};

/*!
  \brief Type information of a kernel arg

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class KernelArgInfo
{
  using ASpaceInfo = AddressSpaceInfo<std::remove_cv_t<Type>>;

 public:
  using ElementType = typename ASpaceInfo::ElementType;


  static constexpr bool kIsGlobal = ASpaceInfo::kIsGlobal;
  static constexpr bool kIsLocal = ASpaceInfo::kIsLocal;
  static constexpr bool kIsConstant = ASpaceInfo::kIsConstant;
  static constexpr bool kIsPod = ASpaceInfo::kIsPod;
  static constexpr bool kIsBuffer = kIsGlobal || kIsConstant;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
};

/*!
  \brief No brief description

  No detailed description.
  */
class KernelArgParseResult
{
 public:
  //! Initialize a parse result
  constexpr KernelArgParseResult() noexcept;

  //! Initialize a parse result
  constexpr KernelArgParseResult(const bool is_global,
                                 const bool is_local,
                                 const bool is_constant,
                                 const bool is_pod,
                                 const bool is_buffer) noexcept;


  //! Check if the argument is global qualified
  constexpr bool isGlobal() const noexcept;

  //! Check if the argument is local qualified
  constexpr bool isLocal() const noexcept;

  //! Check if the argument is constant qualified
  constexpr bool isConstant() const noexcept;

  //! Check if the argument is pod
  constexpr bool isPod() const noexcept;

  //! Check if the argument is 
  constexpr bool isBuffer() const noexcept;

  //! Return the position of the argument
  constexpr std::size_t index() const noexcept;

  //! Set an index of the argument
  constexpr void setIndex(const std::size_t index) noexcept;

 private:
  std::size_t index_;
  uint8b is_global_;
  uint8b is_local_;
  uint8b is_constant_;
  uint8b is_pod_;
  uint8b is_buffer_;
  std::array<uint8b, 3> padding_{0, 0, 0};
};

/*!
  \brief Type information of kernel arguments

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
class KernelArgParser
{
 public:
  // Type aliases
  template <std::size_t kSize>
  using ResultList = std::array<KernelArgParseResult, kSize>;
  template <std::size_t kDim, DerivedKSet KSet>
  using KernelType = Kernel<KernelParams<kDim, KSet, Args...>>;


  static constexpr std::size_t kNumOfArgs = 0; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = 0; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = 0; //!< The number of locals
  static constexpr std::size_t kNumOfConstantArgs = 0; //!< The number of constants
  static constexpr std::size_t kNumOfPodArgs = 0; //!< The number of PODs
  static constexpr std::size_t kNumOfBufferArgs = 0; //!< The number of buffers


  //! Return the info of arguments
  static constexpr ResultList<kNumOfArgs> getArgInfoList() noexcept
  {
    ResultList<kNumOfArgs> result_list;
    return result_list;
  }

  //! Return the info of local arguments
  static constexpr ResultList<kNumOfLocalArgs> getLocalArgInfoList() noexcept
  {
    ResultList<kNumOfLocalArgs> result_list;
    return result_list;
  }

  //! Return the info of POD arguments
  static constexpr ResultList<kNumOfPodArgs> getPodArgInfoList() noexcept
  {
    ResultList<kNumOfPodArgs> result_list;
    return result_list;
  }

  //! Return the info of buffer arguments
  static constexpr ResultList<kNumOfBufferArgs> getBufferArgInfoList() noexcept
  {
    ResultList<kNumOfBufferArgs> result_list;
    return result_list;
  }

  //! Check if there are any const local arguments in the kernel arguments 
  static constexpr bool hasConstLocal() noexcept
  {
    return false;
  }
};

// Type aliases
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
using SharedKernel = std::shared_ptr<typename KernelArgParser<Args...>::
                                         template KernelType<kDim, KSet>>;
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
using WeakKernel = std::weak_ptr<typename KernelArgParser<Args...>::
                                     template KernelType<kDim, KSet>>;

} // namespace zivc

#include "kernel_arg_parser-inl.hpp"

#endif // ZIVC_KERNEL_ARG_PARSER_HPP
