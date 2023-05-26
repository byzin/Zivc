/*!
  \file cli.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cli.hpp"
// Standard C++ library
#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "config.hpp"

namespace ztest {

/*!
  \details No detailed description

  \param [in] name No description.
  \return No description
  */
zivc::uint32b getDeviceId(std::string name) noexcept
{
  zivc::uint32b id = Config::invalidDeviceId();
  auto to_lower = [](const char c) noexcept -> char
  {
    return zisc::cast<char>(std::tolower(c));
  };
  std::transform(name.begin(), name.end(), name.begin(), to_lower);
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
      ++id;
      if (e != (name.data() + name.size()))
        id = Config::invalidDeviceId();
    }
  }
  return id;
}

} // namespace ztest
