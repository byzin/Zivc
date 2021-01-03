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
#include <algorithm>
#include <charconv>
#include <memory>
#include <string>
#include <string_view>
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
  return getDeviceId(deviceName());
}

/*!
  \details No detailed description

  \return No description
  */
std::string& Config::deviceName() noexcept
{
  return device_name_;
}

/*!
  \details No detailed description

  \return No description
  */
const std::string& Config::deviceName() const noexcept
{
  return device_name_;
}

/*!
  \details No detailed description

  \param [in] name No description.
  \return No description
  */
zivc::uint32b Config::getDeviceId(std::string name) noexcept
{
  zivc::uint32b id = invalidDeviceId();
  std::transform(name.begin(), name.end(), name.begin(), std::tolower);
  const std::string_view cpu_name{"cpu"};
  const std::string_view vulkan_name{"vulkan"};
  if (name == cpu_name) {
    id = 0;
  }
  else if (name.starts_with(vulkan_name)) {
    name.erase(name.begin(), name.begin() + vulkan_name.size());
    if (name.empty()) {
      id = 1;
    }
    else {
      auto [e, result] = std::from_chars(name.data(), name.data() + name.size(), id);
      if (e != std::addressof(*name.end()))
        id = invalidDeviceId();
    }
  }
  return id;
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
