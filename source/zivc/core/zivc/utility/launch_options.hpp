/*!
  \file launch_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_LAUNCH_OPTIONS_HPP
#define ZIVC_LAUNCH_OPTIONS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/zisc_config.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class LaunchOptions
{
 public:
  //! Initialize the launch info
  LaunchOptions() noexcept;

  //! Initialize the launch info
  LaunchOptions(const uint32b queue_index) noexcept;


  //! Check whether external sync mode is required
  bool isExternalSyncMode() const noexcept;

  //! Return the label of the launching
  std::string_view label() const noexcept;

  //! Return the color of the label
  const std::array<float, 4>& labelColor() const noexcept;

  //! Return the queue index
  uint32b queueIndex() const noexcept;

  //! Set external sync mode
  void setExternalSyncMode(const bool is_active) noexcept;

  //! Set the queue index which is used for a kernel execution
  void setQueueIndex(const uint32b queue_index) noexcept;

  //! Set the label of the launching
  void setLabel(const std::string_view launch_label) noexcept;

  //! Set the color of the label
  void setLabelColor(const std::array<float, 4>& label_color) noexcept;

 private:
  //! Initialize the options
  void initialize() noexcept;


  IdData::NameType label_;
  std::array<float, 4> label_color_{1.0f, 1.0f, 1.0f, 1.0f};
  uint32b queue_index_ = 0;
  uint8b is_external_sync_mode_ = zisc::kFalse;
  [[maybe_unused]] Padding<3> pad_;
};

} // namespace zivc

#include "launch_options-inl.hpp"

#endif // ZIVC_LAUNCH_OPTIONS_HPP
