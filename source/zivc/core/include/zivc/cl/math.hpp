/*!
  \file math.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_MATH_HPP
#define ZIVC_CL_MATH_HPP

// Zivc
#include "types.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class Math
{
 public:
  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept;

  /*!
    \brief No brief description

    No detailed description.
    */
  struct Native
  {
    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  struct Fallback
  {
    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;

   private:
    template <typename Float>
    struct Util;

    template <typename Float>
    struct Impl;
  };

  // Type aliases
  using Impl = Native;
};

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN x) noexcept;

} /* namespace zivc */

#include "math-inl.hpp"

#endif /* ZIVC_CL_MATH_HPP */
