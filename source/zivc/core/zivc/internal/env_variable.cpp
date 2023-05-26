/*!
  \file env_variable-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ENV_VARIABLE_INL_HPP
#define ZIVC_ENV_VARIABLE_INL_HPP

#include "env_variable.hpp"
// Standard C++ library
#include <charconv>
#include <cstdlib>
#include <string_view>
// Zivc
#include "../zivc_config.hpp"

namespace zivc::internal {

/*!
  \details No detailed description

  \param [in] env_variable No description.
  \return No description
  */
int64b getEnvNumber(const std::string_view env_variable) noexcept
{
  const std::string_view str = getEnvString(env_variable);
  int64b number = 0;
  if (!str.empty())
    std::from_chars(str.data(), str.data() + str.size(), number);
  return number;
}

/*!
  \details No detailed description

  \param [in] env_variable No description.
  \return No description
  */
std::string_view getEnvString(const std::string_view env_variable) noexcept
{
  const char* const value = std::getenv(env_variable.data());
  const std::string_view result = (value != nullptr) ? value : "";
  return result;
}

} // namespace zivc::internal

#endif // ZIVC_ENV_VARIABLE_INL_HPP
