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
    using T = KernelT<KernelInitParams<kDim, KSet, Args...>, KArgs...>;
  };


  // Type aliases
  using ImplT = KernelArgParserImpl<Args...>;
  using HelperT = KernelTypeHelper<typename ImplT::ArgTypePackT>;

 public:
  // Type aliases
  template <std::size_t kSize>
  using ArgInfoListT = std::array<KernelArgInfo, kSize>;
  template <template<typename, typename...> typename KernelT, std::size_t kDim, DerivedKSet KSet>
  using KernelT = typename HelperT::template T<KernelT, kDim, KSet>;


  // The number of kernel arguments
  static constexpr std::size_t kNumOfArgs = ImplT::kNumOfArgs; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = ImplT::kNumOfGlobalArgs; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = ImplT::kNumOfLocalArgs; //!< The number of locals
  static constexpr std::size_t kNumOfConstantArgs = ImplT::kNumOfConstantArgs; //!< The number of constants
  static constexpr std::size_t kNumOfPodArgs = ImplT::kNumOfPodArgs; //!< The number of PODs
  static constexpr std::size_t kNumOfBufferArgs = ImplT::kNumOfBufferArgs; //!< The number of buffers


  //! Return the info of arguments
  static constexpr ArgInfoListT<kNumOfArgs> getArgInfoList() noexcept;

  //! Return the info of local arguments
  static constexpr ArgInfoListT<kNumOfLocalArgs> getLocalArgInfoList() noexcept;

  //! Return the info of POD arguments
  static constexpr ArgInfoListT<kNumOfPodArgs> getPodArgInfoList() noexcept;

  //! Return the info of buffer arguments
  static constexpr ArgInfoListT<kNumOfBufferArgs> getBufferArgInfoList() noexcept;
};

} // namespace zivc

#include "kernel_arg_parser-inl.hpp"

#endif // ZIVC_KERNEL_ARG_PARSER_HPP
