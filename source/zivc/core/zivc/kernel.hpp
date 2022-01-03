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
#include <cstddef>
#include <memory>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "kernel_common.hpp"
#include "kernel_set.hpp"
#include "utility/kernel_launch_options.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename...> class KernelArgParser;
template <typename, typename...> class Kernel;

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
  static_assert(zisc::Algorithm::isInBounds(kDim, 1u, 4u),
                "The kDim must be 1, 2 or 3.");

 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Kernel>;
  using WeakPtr = std::weak_ptr<Kernel>;
  using ArgParser = KernelArgParser<FuncArgs...>;
  using Params = KernelInitParams<kDim, KSet, FuncArgs...>;
  using LaunchOptions = KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>,
                                            Args...>;


  //! Initialize the kernel
  Kernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~Kernel() noexcept override;


  //! Return the number of kernel arguments
  std::size_t argSize() const noexcept override;

  //! Destroy the buffer
  void destroy() noexcept;

  //! Return the work-group dimension
  static constexpr std::size_t dimension() noexcept;

  //! Return the work-group dimension
  std::size_t dimensionSize() const noexcept override;

  //! Initialize the kernel
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const Params& params);

  //! Make launch options
  LaunchOptions makeOptions() const noexcept
  {
    //! \note Separate declaration and definition cause a build error on visual studio
    return LaunchOptions{};
  }

  //! Return the number of kernel arguments required
  static constexpr std::size_t numOfArgs() noexcept;

  //! Return the number of buffer arguments required
  static constexpr std::size_t numOfBuffers() noexcept;

  //! Return the number of pod arguments required
  static constexpr std::size_t numOfPods() noexcept;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  virtual LaunchResult run(Args... args, const LaunchOptions& launch_options) = 0;

 protected:
  //! Clear the contents of the kernel
  virtual void destroyData() noexcept = 0;

  //! Expand the given work size to 3d work size array
  static std::array<uint32b, 3> expandWorkSize(
      const std::array<uint32b, kDim>& work_size,
      const uint32b fill_value) noexcept;

  //! Initialize the kernel 
  virtual void initData(const Params& params) = 0;
};

} // namespace zivc

#include "kernel-inl.hpp"

#endif // ZIVC_KERNEL_HPP
