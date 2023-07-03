/*!
  \file zivc_cl_config-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ZIVC_CL_CONFIG_INL_HPP
#define ZIVC_CL_ZIVC_CL_CONFIG_INL_HPP

// Zivc
#include "zivc_cl_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool Config::isPrecisionMathUsed() noexcept
{
#if defined(ZIVC_MATH_PRECISION) && (ZIVC_MATH_PRECISION == 1)
  const bool flag = true;
#else // ZIVC_MATH_PRECISION
  const bool flag = false;
#endif // ZIVC_MATH_PRECISION
  return flag;
}

} // namespace zivc

#endif /* ZIVC_CL_ZIVC_CL_CONFIG_INL_HPP */
