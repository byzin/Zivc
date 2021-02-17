/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class Kernel;
template <typename...> class KernelArgParserImpl;
template <typename ...> struct KernelArgTypePack;

/*!
  \brief Information of a pod kernel argument

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class KernelArgTypeInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = false;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = true;
  static constexpr bool kIsBuffer = false;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The Type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The Type is reference.");
  static_assert(std::is_standard_layout_v<ElementType>,
                "The POD type isn't standard layout type.");
  static_assert(std::is_trivially_copyable_v<ElementType>,
                "The POD type isn't trivially copyable type.");
  static_assert(zisc::EqualityComparable<ElementType>,
                "The POD type isn't equality comparable.");
};

/*!
  \brief No brief description

  No detailed description.  */
class KernelArgInfo
{
 public:
  //! Initialize a parse result
  constexpr KernelArgInfo() noexcept;

  //! Initialize a parse result
  constexpr KernelArgInfo(const bool is_global,
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
  [[maybe_unused]] std::array<uint8b, 3> padding_{0, 0, 0};
};

/*!
  \brief Parse kernel arguments and provide kernel information

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
class KernelArgParser 
{
  using Impl = KernelArgParserImpl<Args...>;


 public:
  //! Make a kernel helper
  template <template<typename, typename...> typename KernelT,
            std::size_t kDim, DerivedKSet KSet, typename ...KArgs>
  static auto makeKernelType(const KernelArgTypePack<KArgs...>&) noexcept
  {
    using InitParamsT = KernelInitParams<kDim, KSet, Args...>;
    struct Helper
    {
      using Type = KernelT<InitParamsT, KArgs...>;
    };
    return Helper{};
  }

  //!
  template <template<typename, typename...> typename KernelT,
            std::size_t kDim, DerivedKSet KSet>
  using Helper = decltype(makeKernelType<KernelT, kDim, KSet>(typename Impl::ArgTypePack{}));

  // Type aliases
  template <std::size_t kSize>
  using ArgInfoList = std::array<KernelArgInfo, kSize>;
  template <template<typename, typename...> typename KernelT,
            std::size_t kDim, DerivedKSet KSet>
  using KernelType = typename Helper<KernelT, kDim, KSet>::Type;


  // The number of kernel arguments
  static constexpr std::size_t kNumOfArgs = Impl::kNumOfArgs; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = Impl::kNumOfGlobalArgs; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = Impl::kNumOfLocalArgs; //!< The number of locals
  static constexpr std::size_t kNumOfConstantArgs = Impl::kNumOfConstantArgs; //!< The number of constants
  static constexpr std::size_t kNumOfPodArgs = Impl::kNumOfPodArgs; //!< The number of PODs
  static constexpr std::size_t kNumOfBufferArgs = Impl::kNumOfBufferArgs; //!< The number of buffers


  //! Return the info of arguments
  static constexpr ArgInfoList<kNumOfArgs> getArgInfoList() noexcept;

  //! Return the info of local arguments
  static constexpr ArgInfoList<kNumOfLocalArgs> getLocalArgInfoList() noexcept;

  //! Return the info of POD arguments
  static constexpr ArgInfoList<kNumOfPodArgs> getPodArgInfoList() noexcept;

  //! Return the info of buffer arguments
  static constexpr ArgInfoList<kNumOfBufferArgs> getBufferArgInfoList() noexcept;
};

// Type aliases
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
using SharedKernel = std::shared_ptr<typename KernelArgParser<Args...>::
                                         template KernelType<Kernel, kDim, KSet>>;
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
using WeakKernel = std::weak_ptr<typename KernelArgParser<Args...>::
                                     template KernelType<Kernel, kDim, KSet>>;

} // namespace zivc

#include "kernel_arg_parser-inl.hpp"

#endif // ZIVC_KERNEL_ARG_PARSER_HPP
