/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include "kernel_arg_info.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class Kernel;
template <typename...> class KernelArgParserImpl;
template <typename ...> struct TypePack;

/*!
  \brief Parse kernel arguments and provide kernel information

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
class KernelArgParser 
{
  //! A kernel type helper class
  template <typename> struct KernelTypeHelper;

  //! A kernel type helper class
  template <typename ...KArgs>
  struct KernelTypeHelper<TypePack<KArgs...>>
  {
    template <template<typename, typename...> typename KernelT, std::size_t kDim, DerivedKSet KSet>
    using Type = KernelT<KernelInitParams<kDim, KSet, Args...>, KArgs...>;
  };


  // Type aliases
  using Impl = KernelArgParserImpl<Args...>;
  using Helper = KernelTypeHelper<typename Impl::ArgTypePack>;

 public:
  // Type aliases
  template <std::size_t kSize>
  using ArgInfoList = std::array<KernelArgInfo, kSize>;
  template <template<typename, typename...> typename KernelT, std::size_t kDim, DerivedKSet KSet>
  using KernelType = typename Helper::template Type<KernelT, kDim, KSet>;


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
