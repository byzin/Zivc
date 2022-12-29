/*!
  \file kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_HPP
#define ZIVC_KERNEL_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <concepts>
#include <cstddef>
#include <memory>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
// Zisc
#include "zisc/algorithm.hpp"
// Zivc
#include "kernel_set.hpp"
#include "auxiliary/kernel_common.hpp"
#include "auxiliary/kernel_launch_options.hpp"
#include "auxiliary/id_data.hpp"
#include "auxiliary/launch_result.hpp"
#include "auxiliary/shared_kernel.hpp"
#include "auxiliary/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class Kernel;
namespace internal {
template <typename...> class KernelArgParser;
}

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam FuncArgs No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
class Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...> : public KernelCommon
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Kernel>;
  using WeakPtr = std::weak_ptr<Kernel>;
  using ParamsT = KernelInitParams<kDim, KSet, FuncArgs...>;
  using LaunchOptionsT = KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>,
                                             Args...>;


  //! Initialize the kernel
  Kernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~Kernel() noexcept override;


  //! Return the number of kernel arguments required (nBuffers + nPods)
  constexpr std::size_t argSize() const noexcept override;

  //! Return the number of buffer arguments required
  constexpr std::size_t bufferSize() const noexcept override;

  //! Create launch options
  LaunchOptionsT createOptions() const noexcept;

  //! Destroy the buffer
  void destroy() noexcept;

  //! Return the work-group dimension
  static constexpr std::size_t dimension() noexcept;

  //! Return the work-group dimension
  constexpr std::size_t dimensionSize() const noexcept override;

  //! Check if the kernel has global arg
  static constexpr bool hasGlobalArg() noexcept;

  //! Check if the kernel has local arg
  static constexpr bool hasLocalArg() noexcept;

  //! Check if the kernel has pod arg
  static constexpr bool hasPodArg() noexcept;

  //! Initialize the kernel
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const ParamsT& params);

  //! Return the number of local parameters are used
  constexpr std::size_t localSize() const noexcept override;

  //! Return the number of kernel arguments required (nBuffers + nPods)
  static constexpr std::size_t numOfArgs() noexcept;

  //! Return the number of buffer arguments required
  static constexpr std::size_t numOfBuffers() noexcept;

  //! Return the number of local parameters are used
  static constexpr std::size_t numOfLocals() noexcept;

  //! Return the number of pod arguments required
  static constexpr std::size_t numOfPods() noexcept;

  //! Return the number of pod arguments required
  constexpr std::size_t podSize() const noexcept override;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  virtual LaunchResult run(Args... args, const LaunchOptionsT& launch_options) = 0;

 protected:
  static_assert(zisc::Algorithm::isInBounds(kDim, 1u, 4u),
                "The kDim must be 1, 2 or 3.");


  // Type aliases
  using ArgParserT = internal::KernelArgParser<FuncArgs...>;
  template <std::size_t kIndex>
  using ArgT = std::tuple_element_t<kIndex, std::tuple<FuncArgs...>>;


  //! Clear the contents of the kernel
  virtual void destroyData() noexcept = 0;

  //! Expand the given work size to 3d work size array
  static std::array<uint32b, 3> expandWorkSize(
      const std::span<const uint32b, kDim>& work_size,
      const uint32b fill_value) noexcept;

  //! Initialize the kernel 
  virtual void initData(const ParamsT& params) = 0;
};

} // namespace zivc

#include "kernel-inl.hpp"

#endif // ZIVC_KERNEL_HPP
