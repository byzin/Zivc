/*!
  \file kernel_launch_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_LAUNCH_OPTIONS_HPP
#define ZIVC_KERNEL_LAUNCH_OPTIONS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "id_data.hpp"
#include "launch_options.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class KernelLaunchOptions;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam FuncArgs No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
class KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...> :
    public LaunchOptions
{
  static_assert(zisc::Algorithm::isInBounds(kDim, 1u, 4u),
                "The kDim must be 1, 2 or 3.");

 public:
  //! Initialize the launch info
  KernelLaunchOptions() noexcept;

  //! Initialize the launch info
  KernelLaunchOptions(const std::array<uint32b, kDim>& work_size) noexcept;

  //! Initialize the launch info
  KernelLaunchOptions(const std::array<uint32b, kDim>& work_size,
                      const uint32b queue_index) noexcept;


  //! Return the work-group dimension
  static constexpr std::size_t dimension() noexcept;

  //! Return the 3d global offset used in global id calculations
  const std::array<uint32b, kDim>& globalIdOffset() const noexcept;

  //! Return the number of kernel arguments
  static constexpr std::size_t numOfArgs() noexcept;

  //! Set the 3d global offset used in global id calculations
  void setGlobalIdOffset(const uint32b offset, const std::size_t dim) noexcept;

  //! Set the 3d global offset used in global id calculations
  void setGlobalIdOffset(const std::array<uint32b, kDim>& offset) noexcept;

  //! Set the work item size at the give dimension
  void setWorkSize(const uint32b work_size, const std::size_t dim) noexcept;

  //! Set the work item size
  void setWorkSize(const std::array<uint32b, kDim>& work_size) noexcept;

  //! Return the work item size
  const std::array<uint32b, kDim>& workSize() const noexcept;

 private:
  //! Initialize the options
  void initialize() noexcept;


  std::array<uint32b, kDim> work_size_;
  std::array<uint32b, kDim> global_id_offset_;
};

} // namespace zivc

#include "kernel_launch_options-inl.hpp"

#endif // ZIVC_KERNEL_LAUNCH_OPTIONS_HPP
