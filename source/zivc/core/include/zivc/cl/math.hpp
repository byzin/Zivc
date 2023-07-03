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
  // Basic operations

  //! Compute absolute value of a floating-point number.
  template <typename FloatN>
  static FloatN fabs(const FloatN x) noexcept;

  //! Modulus. Returns x - y * trunc(x/y).
  template <typename FloatN>
  static FloatN fmod(const FloatN x, const FloatN y) noexcept;

  //! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
  template <typename FloatN>
  static FloatN remainder(const FloatN x, const FloatN y) noexcept;

  //! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
  template <typename FloatN, typename AddressSpacePointer>
  static FloatN remquo(const FloatN x, const FloatN y, AddressSpacePointer quo) noexcept;

  //! Compute a * b + c.
  template <typename FloatN>
  static FloatN mad(const FloatN a, const FloatN b, const FloatN c) noexcept;

  //! Returns the correctly rounded floating-point representation of the sum of c with the infinitely precise product of a and b.
  template <typename FloatN>
  static FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept;

  //! Returns y if x < y, otherwise it returns x.
  template <typename FloatN>
  static FloatN fmax(const FloatN x, const FloatN y) noexcept;

  //! Returns y if y < x, otherwise it returns x.
  template <typename FloatN>
  static FloatN fmin(const FloatN x, const FloatN y) noexcept;

  //! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
  template <typename FloatN>
  static FloatN maxmag(const FloatN x, const FloatN y) noexcept;

  //! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
  template <typename FloatN>
  static FloatN minmag(const FloatN x, const FloatN y) noexcept;

  //! x - y if x > y, +0 if x is less than or equal to y.
  template <typename FloatN>
  static FloatN fdim(const FloatN x, const FloatN y) noexcept;

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

  // Nearest integer floating point operations

  //! Round to integral value using the round to positive infinity rounding mode
  template <typename FloatN>
  static FloatN ceil(const FloatN x) noexcept;

  //! Round to integral value using the round to positive infinity rounding mode
  template <typename FloatN>
  static FloatN floor(const FloatN x) noexcept;

  //! Round to integral value using the round to zero rounding mode
  template <typename FloatN>
  static FloatN trunc(const FloatN x) noexcept;

  //! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
  template <typename FloatN>
  static FloatN round(const FloatN x) noexcept;

  //! Round to integral value (using round to nearest even rounding mode) in floating point format.
  template <typename FloatN>
  static FloatN rint(const FloatN x) noexcept;

  // Floating point manipulation functions

  //! Extract mantissa and exponent from x.
  template <typename FloatN, typename AddressSpacePointer>
  static FloatN frexp(const FloatN x, AddressSpacePointer exp) noexcept;

  //! Multiply x by 2 to the power k.
  template <typename FloatN, typename IntegerN>
  static FloatN ldexp(const FloatN x, const IntegerN k) noexcept;

  //! Decompose a floating-point number
  template <typename FloatN, typename AddressSpacePointer>
  static FloatN modf(const FloatN x, AddressSpacePointer iptr) noexcept;

  //! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
  template <typename FloatN, typename AddressSpacePointer>
  static FloatN fract(const FloatN x, AddressSpacePointer iptr) noexcept;

  //! Return the exponent as an integer value.
  template <typename FloatN>
  static auto ilogb(const FloatN x) noexcept;

  //! Return the exponent as an integer value.
  template <typename FloatN>
  static FloatN logb(const FloatN x) noexcept;

  //! Computes the next representable single-precision floating-point value following x in the direction of y.
  template <typename FloatN>
  static FloatN nextafter(const FloatN x, const FloatN y) noexcept;

  //! Return x with its sign changed to match the sign of y
  template <typename FloatN>
  static FloatN copysign(const FloatN x, const FloatN y) noexcept;

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

// Basic operations

//! Compute absolute value of a floating-point number.
template <typename FloatN>
FloatN fabs(const FloatN x) noexcept;

//! Modulus. Returns x - y * trunc(x/y).
template <typename FloatN>
FloatN fmod(const FloatN x, const FloatN y) noexcept;

//! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
template <typename FloatN>
FloatN remainder(const FloatN x, const FloatN y) noexcept;

//! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
template <typename FloatN, typename AddressSpacePointer>
FloatN remquo(const FloatN x, const FloatN y, AddressSpacePointer quo) noexcept;

//! Compute a * b + c.
template <typename FloatN>
FloatN mad(const FloatN a, const FloatN b, const FloatN c) noexcept;

//! Returns the correctly rounded floating-point representation of the sum of c with the infinitely precise product of a and b.
template <typename FloatN>
FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept;

//! Returns y if x < y, otherwise it returns x.
template <typename FloatN>
FloatN fmax(const FloatN x, const FloatN y) noexcept;

//! Returns y if y < x, otherwise it returns x.
template <typename FloatN>
FloatN fmin(const FloatN x, const FloatN y) noexcept;

//! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
template <typename FloatN>
FloatN maxmag(const FloatN x, const FloatN y) noexcept;

//! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
template <typename FloatN>
FloatN minmag(const FloatN x, const FloatN y) noexcept;

//! x - y if x > y, +0 if x is less than or equal to y.
template <typename FloatN>
FloatN fdim(const FloatN x, const FloatN y) noexcept;

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

// Nearest integer floating point operations

//! Round to integral value using the round to positive infinity rounding mode
template <typename FloatN>
FloatN ceil(const FloatN x) noexcept;

//! Round to integral value using the round to positive infinity rounding mode
template <typename FloatN>
FloatN floor(const FloatN x) noexcept;

//! Round to integral value using the round to zero rounding mode
template <typename FloatN>
FloatN trunc(const FloatN x) noexcept;

//! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
template <typename FloatN>
FloatN round(const FloatN x) noexcept;

//! Round to integral value (using round to nearest even rounding mode) in floating point format.
template <typename FloatN>
FloatN rint(const FloatN x) noexcept;

// Floating point manipulation functions

//! Extract mantissa and exponent from x.
template <typename FloatN, typename AddressSpacePointer>
FloatN frexp(const FloatN x, AddressSpacePointer exp) noexcept;

//! Multiply x by 2 to the power k.
template <typename FloatN, typename IntegerN>
FloatN ldexp(const FloatN x, const IntegerN k) noexcept;

//! Decompose a floating-point number
template <typename FloatN, typename AddressSpacePointer>
FloatN modf(const FloatN x, AddressSpacePointer iptr) noexcept;

//! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
template <typename FloatN, typename AddressSpacePointer>
FloatN fract(const FloatN x, AddressSpacePointer iptr) noexcept;

//! Return the exponent as an integer value.
template <typename FloatN>
auto ilogb(const FloatN x) noexcept;

//! Return the exponent as an integer value.
template <typename FloatN>
FloatN logb(const FloatN x) noexcept;

//! Computes the next representable single-precision floating-point value following x in the direction of y.
template <typename FloatN>
FloatN nextafter(const FloatN x, const FloatN y) noexcept;

//! Return x with its sign changed to match the sign of y
template <typename FloatN>
FloatN copysign(const FloatN x, const FloatN y) noexcept;

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
