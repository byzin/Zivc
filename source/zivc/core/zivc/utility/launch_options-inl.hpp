/*!
  \file launch_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_LAUNCH_OPTIONS_INL_HPP
#define ZIVC_LAUNCH_OPTIONS_INL_HPP

#include "launch_options.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
LaunchOptions::LaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
inline
LaunchOptions::LaunchOptions(const uint32b queue_index) noexcept :
    queue_index_{queue_index}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool LaunchOptions::isExternalSyncMode() const noexcept
{
  return is_external_sync_mode_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view LaunchOptions::label() const noexcept
{
  std::string_view l{label_.data()};
  return l;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const std::array<float, 4>& LaunchOptions::labelColor() const noexcept
{
  return label_color_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b LaunchOptions::queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] is_active No description.
  */
inline
void LaunchOptions::setExternalSyncMode(const bool is_active) noexcept
{
  is_external_sync_mode_ = is_active;
}

/*!
  \details No detailed description

  \param [in] launch_label No description.
  */
inline
void LaunchOptions::setLabel(const std::string_view launch_label) noexcept
{
  copyStr(launch_label, label_.data());
}

/*!
  \details No detailed description

  \param [in] label_color No description.
  */
inline
void LaunchOptions::setLabelColor(const std::array<float, 4>& label_color) noexcept
{
  label_color_ = label_color;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
inline
void LaunchOptions::setQueueIndex(const uint32b queue_index) noexcept
{
  queue_index_ = queue_index;
}

/*!
  \details No detailed description
  */
inline
void LaunchOptions::initialize() noexcept
{
  setLabel("LaunchOptions");
}

} // namespace zivc

#endif // ZIVC_LAUNCH_OPTIONS_INL_HPP
