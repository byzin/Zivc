/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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
#include "../kernel_set.hpp"
#include "../zivc_config.hpp"

namespace zivc {

// Forward declaration
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class Kernel;

} // namespace zivc

namespace zivc::internal {

// Forward declaration
template <typename ...> struct KernelArgPack;

/*!
  \brief Parse kernel arguments and provide kernel information

  No detailed description.

  \tparam Args No description.
  */
template <typename ...Args>
class KernelArgParser 
{
  //! Pack kernel arguments
  static auto packArguments() noexcept;

  // Forward declaration
  template <typename ...> struct Pack;

  //!
  template <typename ...KArgs>
  struct Pack<KernelArgPack<KArgs...>>
  {
    template <template<typename, typename...> typename K, std::size_t D, DerivedKSet S>
    using T = K<KernelInitParams<D, S, Args...>, KArgs...>;
  };

  // Type aliases
  using PackT = Pack<decltype(packArguments())>;
  template <std::size_t kSize>
  using ArgInfoListImplT = std::array<KernelArgInfo, kSize>;


  static constexpr std::size_t kNumOfArgsImpl = sizeof...(Args);


  //! Return the list of kernel argument info
  static constexpr ArgInfoListImplT<kNumOfArgsImpl> getArgInfoListImpl() noexcept;

  //! Return the number of global arguments
  static constexpr std::size_t numOfGlobalArgs() noexcept;

  //! Return the number of local arguments
  static constexpr std::size_t numOfLocalArgs() noexcept;

  //! Return the number of POD arguments
  static constexpr std::size_t numOfPodArgs() noexcept;

  //! Return the number of global and constant arguments
  static constexpr std::size_t numOfBufferArgs() noexcept;

  //! Return the number of global, constant and POD arguments
  static constexpr std::size_t numOfParameterArgs() noexcept;

 public:
  // Type aliases
  template <std::size_t kSize>
  using ArgInfoListT = ArgInfoListImplT<kSize>;
  template <template<typename, typename...> typename K, std::size_t D, DerivedKSet S>
  using KernelT = typename PackT::template T<K, D, S>;


  // The number of kernel arguments
  static constexpr std::size_t kNumOfArgs = kNumOfArgsImpl;
  static constexpr std::size_t kNumOfGlobalArgs = numOfGlobalArgs();
  static constexpr std::size_t kNumOfLocalArgs = numOfLocalArgs();
  static constexpr std::size_t kNumOfPodArgs = numOfPodArgs();
  static constexpr std::size_t kNumOfBufferArgs = numOfBufferArgs();
  static constexpr std::size_t kNumOfParameterArgs = numOfParameterArgs();


  //! Return the list of kernel argument info
  static constexpr ArgInfoListT<kNumOfArgs> getArgInfoList() noexcept;

  //! Return the list of global argument info
  static constexpr ArgInfoListT<kNumOfGlobalArgs> getGlobalArgInfoList() noexcept;

  //! Return the list of local argument info
  static constexpr ArgInfoListT<kNumOfLocalArgs> getLocalArgInfoList() noexcept;

  //! Return the list of pod argument info
  static constexpr ArgInfoListT<kNumOfPodArgs> getPodArgInfoList() noexcept;

  //! Return the list of buffer argument info
  static constexpr ArgInfoListT<kNumOfBufferArgs> getBufferArgInfoList() noexcept;

  //! Return the list of parameter argument info
  static constexpr ArgInfoListT<kNumOfParameterArgs> getParameterArgInfoList() noexcept;
};

//! Create an argument info
template <typename Type>
constexpr KernelArgInfo createArgInfo() noexcept;

} // namespace zivc::internal

#include "kernel_arg_parser-inl.hpp"

#endif // ZIVC_KERNEL_ARG_PARSER_HPP
