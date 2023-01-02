/*!
  \file env_variable.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ENV_VARIABLE_HPP
#define ZIVC_ENV_VARIABLE_HPP

// Standard C++ library
#include <string_view>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc::internal {

//! Return the integer identifying the value of the environment variable
int64b getEnvNumber(const std::string_view env_variable) noexcept;

//! Return the string identifying the value of the environment variable
std::string_view getEnvString(const std::string_view env_variable) noexcept;

} // namespace zivc::internal

#endif // ZIVC_ENV_VARIABLE_HPP
