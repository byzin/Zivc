/*!
  \file zivc_cl_config.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ZIVC_CL_CONFIG_HPP
#define ZIVC_CL_ZIVC_CL_CONFIG_HPP

namespace zivc {

/*!
  \brief Configuration of zivc
  */
class Config
{
 public:
  // Math library config

  //! Check if the precision math implementation is used instead of the built-in math as default
  static constexpr bool isPrecisionMathUsed() noexcept;
};

} // namespace zivc

#include "zivc_cl_config-inl.hpp"

#endif /* ZIVC_CL_ZIVC_CL_CONFIG_HPP */
