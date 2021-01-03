/*!
  \file config.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "config.hpp"
// Standard C++ library
#include <memory>
// Zivc
#include "zivc/zivc_config.hpp"

namespace ztest {

/*!
  \details No detailed description
  */
Config::~Config() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
zivc::uint32b Config::deviceId() const noexcept
{
  return device_id_;
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
void Config::setDeviceId(const zivc::uint32b id) noexcept
{
  device_id_ = id;
}

/*!
  \details No detailed description

  \return No description
  */
Config& Config::globalConfig() noexcept
{
  if (!global_config_) {
    global_config_.reset(new Config{});
  }
  return *global_config_;
}

/*!
  \details No detailed description
  */
Config::Config() noexcept
{
}

// Declaration of static member
std::unique_ptr<Config> Config::global_config_;

} // namespace ztest
