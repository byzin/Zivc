/*!
  \file env_variable-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ENV_VARIABLE_INL_HPP
#define ZIVC_ENV_VARIABLE_INL_HPP

// Standard C++ library
#include <charconv>
#include <cstdlib>
#include <string_view>
// Zivc
#include "zivc/zivc_config.hpp"

#include "env_variable.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] env_variable No description.
  \return No description
  */
inline
int64b getEnvNumber(const std::string_view env_variable) noexcept
{
  std::string_view str = getEnvString(env_variable);
  int64b number = 0;
  if (0 < str.size())
    std::from_chars(&str[0], &str[0] + str.size(), number);
  return number;
}

/*!
  \details No detailed description

  \param [in] env_variable No description.
  \return No description
  */
inline
std::string_view getEnvString(const std::string_view env_variable) noexcept
{
  char* value = std::getenv(env_variable.data());
  std::string_view result = value ? value : "";
  return result;
}

} // namespace zivc

#endif // ZIVC_ENV_VARIABLE_INL_HPP
