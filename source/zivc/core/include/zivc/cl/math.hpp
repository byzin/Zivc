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

  // Power functions

  //! Compute x to the power y
  template <typename FloatN>
  static FloatN pow(const FloatN x, const FloatN y) noexcept;

  //! Compute x to the power y
  template <typename FloatN, typename IntN>
  static FloatN pown(const FloatN x, const IntN y) noexcept;

  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept;

  //! Compute inverse square root
  template <typename FloatN>
  static FloatN rsqrt(const FloatN x) noexcept;

  //! Compute cube root
  template <typename FloatN>
  static FloatN cbrt(const FloatN x) noexcept;

  //! \todo Implement hypot

  /*!
    \brief No brief description

    No detailed description.
    */
  struct Native
  {
    // Power functions

    //! Compute x to the power y
    template <typename FloatN>
    static FloatN pow(const FloatN x, const FloatN y) noexcept;

    //! Compute x to the power y
    template <typename FloatN, typename IntN>
    static FloatN pown(const FloatN x, const IntN y) noexcept;

    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;

    //! Compute inverse square root
    template <typename FloatN>
    static FloatN rsqrt(const FloatN x) noexcept;

    //! Compute cube root
    template <typename FloatN>
    static FloatN cbrt(const FloatN x) noexcept;
  };

  /*!
    \brief The implementation is based on SLEEF Vectorized Math Llibrary (https://sleef.org/)

    No detailed description.
    */
  struct Precision
  {
    // Power functions

    //! Compute x to the power y
    template <typename FloatN>
    static FloatN pow(const FloatN x, const FloatN y) noexcept;

    //! Compute x to the power y
    template <typename FloatN, typename IntN>
    static FloatN pown(const FloatN x, const IntN y) noexcept;

    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;

    //! Compute inverse square root
    template <typename FloatN>
    static FloatN rsqrt(const FloatN x) noexcept;

    //! Compute cube root
    template <typename FloatN>
    static FloatN cbrt(const FloatN x) noexcept;

   private:
    template <typename Float>
    struct Util;

    template <typename Float>
    struct Impl;
  };

  // Type aliases
  using Impl = Native;
};

// Power functions

//! Compute x to the power y
template <typename FloatN>
FloatN pow(const FloatN x, const FloatN y) noexcept;

//! Compute x to the power y
template <typename FloatN, typename IntN>
FloatN pown(const FloatN x, const IntN y) noexcept;

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN x) noexcept;

//! Compute inverse square root
template <typename FloatN>
FloatN rsqrt(const FloatN x) noexcept;

//! Compute cube root
template <typename FloatN>
FloatN cbrt(const FloatN x) noexcept;

namespace native {

//! Compute x to the power y
template <typename FloatN>
FloatN pow(const FloatN x, const FloatN y) noexcept;

//! Compute x to the power y
template <typename FloatN, typename IntN>
FloatN pown(const FloatN x, const IntN y) noexcept;

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN x) noexcept;

//! Compute inverse square root
template <typename FloatN>
FloatN rsqrt(const FloatN x) noexcept;

//! Compute cube root
template <typename FloatN>
FloatN cbrt(const FloatN x) noexcept;

} /* namespace native */

namespace precision {

//! Compute x to the power y
template <typename FloatN>
FloatN pow(const FloatN x, const FloatN y) noexcept;

//! Compute x to the power y
template <typename FloatN, typename IntN>
FloatN pown(const FloatN x, const IntN y) noexcept;

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN x) noexcept;

//! Compute inverse square root
template <typename FloatN>
FloatN rsqrt(const FloatN x) noexcept;

//! Compute cube root
template <typename FloatN>
FloatN cbrt(const FloatN x) noexcept;

} /* namespace native */

} /* namespace zivc */

#include "math-inl.hpp"

#endif /* ZIVC_CL_MATH_HPP */
