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
class KernelLaunchOptions<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>
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

  //! Check whether external sync mode is required
  bool isExternalSyncMode() const noexcept;

  //! Return the label of the launching
  std::string_view label() const noexcept;

  //! Return the color of the label
  const std::array<float, 4>& labelColor() const noexcept;

  //! Return the number of kernel arguments
  static constexpr std::size_t numOfArgs() noexcept;

  //! Return the queue index
  uint32b queueIndex() const noexcept;

  //! Set external sync mode
  void setExternalSyncMode(const bool is_active) noexcept;

  //! Set the 3d global offset used in global id calculations
  void setGlobalIdOffset(const uint32b offset, const std::size_t dim) noexcept;

  //! Set the 3d global offset used in global id calculations
  void setGlobalIdOffset(const std::array<uint32b, kDim>& offset) noexcept;

  //! Set the label of the launching
  void setLabel(const std::string_view launch_label) noexcept;

  //! Set the color of the label
  void setLabelColor(const std::array<float, 4>& label_color) noexcept;

  //! Set the queue index which is used for a kernel execution
  void setQueueIndex(const uint32b queue_index) noexcept;

  //! Set the work item size at the give dimension
  void setWorkSize(const uint32b work_size, const std::size_t dim) noexcept;

  //! Set the work item size
  void setWorkSize(const std::array<uint32b, kDim>& work_size) noexcept;

  //! Return the work item size
  const std::array<uint32b, kDim>& workSize() const noexcept;

 private:
  //! Initialize the options
  void initialize() noexcept;


  IdData::NameType label_;
  std::array<float, 4> label_color_ = {1.0f, 1.0f, 1.0f, 1.0f};
  std::array<uint32b, kDim> work_size_;
  std::array<uint32b, kDim> global_id_offset_;
  uint32b queue_index_ = 0;
  int8b is_external_sync_mode_ = zisc::kFalse;
  [[maybe_unused]] std::array<uint8b, 7> padding_;
};

} // namespace zivc

#include "kernel_launch_options-inl.hpp"

#endif // ZIVC_KERNEL_LAUNCH_OPTIONS_HPP
