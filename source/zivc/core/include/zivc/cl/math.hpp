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

  // Exponential functions

  //! Compute the base-e exponential of x.
  template <typename FloatN>
  static FloatN exp(const FloatN x) noexcept;

  //! Exponential base 2 function.
  template <typename FloatN>
  static FloatN exp2(const FloatN x) noexcept;

  //! Compute e^x - 1.0.
  template <typename FloatN>
  static FloatN expm1(const FloatN x) noexcept;

  //! Compute natural logarithm.
  template <typename FloatN>
  static FloatN log(const FloatN x) noexcept;

  //! Compute a base 2 logarithm.
  template <typename FloatN>
  static FloatN log2(const FloatN x) noexcept;

  //! Compute a base 10 logarithm.
  template <typename FloatN>
  static FloatN log10(const FloatN x) noexcept;

  //! Compute log_e(1.0 + x).
  template <typename FloatN>
  static FloatN log1p(const FloatN x) noexcept;

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

  //! Compute the value of the square root of x + y without undue overflow or underflow.
  template <typename FloatN>
  static FloatN hypot(const FloatN x, const FloatN y) noexcept;

  // Trigonometric functions

  //! Compute sine, where x is an angle in radians.
  template <typename FloatN>
  static FloatN sin(const FloatN x) noexcept;

  //! Compute cosine, where x is an angle in radians.
  template <typename FloatN>
  static FloatN cos(const FloatN x) noexcept;

  //! Compute tangent, where x is an angle in radians.
  template <typename FloatN>
  static FloatN tan(const FloatN x) noexcept;

  //! Arc sine function. Returns an angle in radians.
  template <typename FloatN>
  static FloatN asin(const FloatN x) noexcept;

  //! Arc cosine function. Returns an angle in radians.
  template <typename FloatN>
  static FloatN acos(const FloatN x) noexcept;

  //! Arc tangent function. Returns an angle in radians.
  template <typename FloatN>
  static FloatN atan(const FloatN x) noexcept;

  //! Arc tangent of y / x. Returns an angle in radians.
  template <typename FloatN>
  static FloatN atan2(const FloatN y, const FloatN x) noexcept;

  // Hyperbolic functions

  //! Compute hyperbolic sine, where x is an angle in radians
  template <typename FloatN>
  static FloatN sinh(const FloatN x) noexcept;

  //! Compute hyperbolic cosine, where x is an angle in radians
  template <typename FloatN>
  static FloatN cosh(const FloatN x) noexcept;

  //! Compute hyperbolic tangent, where x is an angle in radians
  template <typename FloatN>
  static FloatN tanh(const FloatN x) noexcept;

  //! Inverse hyperbolic sine. Returns an angle in radians
  template <typename FloatN>
  static FloatN asinh(const FloatN x) noexcept;

  //! Inverse hyperbolic cosine. Returns an angle in radians
  template <typename FloatN>
  static FloatN acosh(const FloatN x) noexcept;

  //! Hyperbolic arc tangent. Returns an angle in radians
  template <typename FloatN>
  static FloatN atanh(const FloatN x) noexcept;

  // Error and gamma functions

  //! Error function encountered in integrating the normal distribution.
  template <typename FloatN>
  static FloatN erf(const FloatN x) noexcept;

  //! Complementary error function.
  template <typename FloatN>
  static FloatN erfc(const FloatN x) noexcept;

  //! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
  template <typename FloatN>
  static FloatN lgamma(const FloatN x) noexcept;

  //! Compute the gamma function.
  template <typename FloatN>
  static FloatN tgamma(const FloatN x) noexcept;

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
    // Exponential functions

    //! Compute the base-e exponential of x.
    template <typename FloatN>
    static FloatN exp(const FloatN x) noexcept;

    //! Exponential base 2 function.
    template <typename FloatN>
    static FloatN exp2(const FloatN x) noexcept;

    //! Compute e^x - 1.0.
    template <typename FloatN>
    static FloatN expm1(const FloatN x) noexcept;

    //! Compute natural logarithm.
    template <typename FloatN>
    static FloatN log(const FloatN x) noexcept;

    //! Compute a base 2 logarithm.
    template <typename FloatN>
    static FloatN log2(const FloatN x) noexcept;

    //! Compute a base 10 logarithm.
    template <typename FloatN>
    static FloatN log10(const FloatN x) noexcept;

    //! Compute log_e(1.0 + x).
    template <typename FloatN>
    static FloatN log1p(const FloatN x) noexcept;

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

    //! Compute the value of the square root of x + y without undue overflow or underflow.
    template <typename FloatN>
    static FloatN hypot(const FloatN x, const FloatN y) noexcept;

    // Trigonometric functions

    //! Compute sine, where x is an angle in radians.
    template <typename FloatN>
    static FloatN sin(const FloatN x) noexcept;

    //! Compute cosine, where x is an angle in radians.
    template <typename FloatN>
    static FloatN cos(const FloatN x) noexcept;

    //! Compute tangent, where x is an angle in radians.
    template <typename FloatN>
    static FloatN tan(const FloatN x) noexcept;

    //! Arc sine function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN asin(const FloatN x) noexcept;

    //! Arc cosine function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN acos(const FloatN x) noexcept;

    //! Arc tangent function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN atan(const FloatN x) noexcept;

    //! Arc tangent of y / x. Returns an angle in radians.
    template <typename FloatN>
    static FloatN atan2(const FloatN y, const FloatN x) noexcept;

    // Hyperbolic functions

    //! Compute hyperbolic sine, where x is an angle in radians
    template <typename FloatN>
    static FloatN sinh(const FloatN x) noexcept;

    //! Compute hyperbolic cosine, where x is an angle in radians
    template <typename FloatN>
    static FloatN cosh(const FloatN x) noexcept;

    //! Compute hyperbolic tangent, where x is an angle in radians
    template <typename FloatN>
    static FloatN tanh(const FloatN x) noexcept;

    //! Inverse hyperbolic sine. Returns an angle in radians
    template <typename FloatN>
    static FloatN asinh(const FloatN x) noexcept;

    //! Inverse hyperbolic cosine. Returns an angle in radians
    template <typename FloatN>
    static FloatN acosh(const FloatN x) noexcept;

    //! Hyperbolic arc tangent. Returns an angle in radians
    template <typename FloatN>
    static FloatN atanh(const FloatN x) noexcept;

    // Error and gamma functions

    //! Error function encountered in integrating the normal distribution.
    template <typename FloatN>
    static FloatN erf(const FloatN x) noexcept;

    //! Complementary error function.
    template <typename FloatN>
    static FloatN erfc(const FloatN x) noexcept;

    //! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
    template <typename FloatN>
    static FloatN lgamma(const FloatN x) noexcept;

    //! Compute the gamma function.
    template <typename FloatN>
    static FloatN tgamma(const FloatN x) noexcept;
  };

  /*!
    \brief The implementation is based on SLEEF Vectorized Math Llibrary (https://sleef.org/)

    No detailed description.
    */
  struct Precision
  {
    // Exponential functions

    //! Compute the base-e exponential of x.
    template <typename FloatN>
    static FloatN exp(const FloatN x) noexcept;

    //! Exponential base 2 function.
    template <typename FloatN>
    static FloatN exp2(const FloatN x) noexcept;

    //! Compute e^x - 1.0.
    template <typename FloatN>
    static FloatN expm1(const FloatN x) noexcept;

    //! Compute natural logarithm.
    template <typename FloatN>
    static FloatN log(const FloatN x) noexcept;

    //! Compute a base 2 logarithm.
    template <typename FloatN>
    static FloatN log2(const FloatN x) noexcept;

    //! Compute a base 10 logarithm.
    template <typename FloatN>
    static FloatN log10(const FloatN x) noexcept;

    //! Compute log_e(1.0 + x).
    template <typename FloatN>
    static FloatN log1p(const FloatN x) noexcept;

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

    //! Compute the value of the square root of x + y without undue overflow or underflow.
    template <typename FloatN>
    static FloatN hypot(const FloatN x, const FloatN y) noexcept;

    // Trigonometric functions

    //! Compute sine, where x is an angle in radians.
    template <typename FloatN>
    static FloatN sin(const FloatN x) noexcept;

    //! Compute cosine, where x is an angle in radians.
    template <typename FloatN>
    static FloatN cos(const FloatN x) noexcept;

    //! Compute tangent, where x is an angle in radians.
    template <typename FloatN>
    static FloatN tan(const FloatN x) noexcept;

    //! Arc sine function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN asin(const FloatN x) noexcept;

    //! Arc cosine function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN acos(const FloatN x) noexcept;

    //! Arc tangent function. Returns an angle in radians.
    template <typename FloatN>
    static FloatN atan(const FloatN x) noexcept;

    //! Arc tangent of y / x. Returns an angle in radians.
    template <typename FloatN>
    static FloatN atan2(const FloatN y, const FloatN x) noexcept;

    // Hyperbolic functions

    //! Compute hyperbolic sine, where x is an angle in radians
    template <typename FloatN>
    static FloatN sinh(const FloatN x) noexcept;

    //! Compute hyperbolic cosine, where x is an angle in radians
    template <typename FloatN>
    static FloatN cosh(const FloatN x) noexcept;

    //! Compute hyperbolic tangent, where x is an angle in radians
    template <typename FloatN>
    static FloatN tanh(const FloatN x) noexcept;

    //! Inverse hyperbolic sine. Returns an angle in radians
    template <typename FloatN>
    static FloatN asinh(const FloatN x) noexcept;

    //! Inverse hyperbolic cosine. Returns an angle in radians
    template <typename FloatN>
    static FloatN acosh(const FloatN x) noexcept;

    //! Hyperbolic arc tangent. Returns an angle in radians
    template <typename FloatN>
    static FloatN atanh(const FloatN x) noexcept;

    // Error and gamma functions

    //! Error function encountered in integrating the normal distribution.
    template <typename FloatN>
    static FloatN erf(const FloatN x) noexcept;

    //! Complementary error function.
    template <typename FloatN>
    static FloatN erfc(const FloatN x) noexcept;

    //! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
    template <typename FloatN>
    static FloatN lgamma(const FloatN x) noexcept;

    //! Compute the gamma function.
    template <typename FloatN>
    static FloatN tgamma(const FloatN x) noexcept;

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

// Exponential functions

//! Compute the base-e exponential of x.
template <typename FloatN>
FloatN exp(const FloatN x) noexcept;

//! Exponential base 2 function.
template <typename FloatN>
FloatN exp2(const FloatN x) noexcept;

//! Compute e^x - 1.0.
template <typename FloatN>
FloatN expm1(const FloatN x) noexcept;

//! Compute natural logarithm.
template <typename FloatN>
FloatN log(const FloatN x) noexcept;

//! Compute a base 2 logarithm.
template <typename FloatN>
FloatN log2(const FloatN x) noexcept;

//! Compute a base 10 logarithm.
template <typename FloatN>
FloatN log10(const FloatN x) noexcept;

//! Compute log_e(1.0 + x).
template <typename FloatN>
FloatN log1p(const FloatN x) noexcept;

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

//! Compute the value of the square root of x + y without undue overflow or underflow.
template <typename FloatN>
FloatN hypot(const FloatN x, const FloatN y) noexcept;

// Trigonometric functions

//! Compute sine, where x is an angle in radians.
template <typename FloatN>
FloatN sin(const FloatN x) noexcept;

//! Compute cosine, where x is an angle in radians.
template <typename FloatN>
FloatN cos(const FloatN x) noexcept;

//! Compute tangent, where x is an angle in radians.
template <typename FloatN>
FloatN tan(const FloatN x) noexcept;

//! Arc sine function. Returns an angle in radians.
template <typename FloatN>
FloatN asin(const FloatN x) noexcept;

//! Arc cosine function. Returns an angle in radians.
template <typename FloatN>
FloatN acos(const FloatN x) noexcept;

//! Arc tangent function. Returns an angle in radians.
template <typename FloatN>
FloatN atan(const FloatN x) noexcept;

//! Arc tangent of y / x. Returns an angle in radians.
template <typename FloatN>
FloatN atan2(const FloatN y, const FloatN x) noexcept;

// Hyperbolic functions

//! Compute hyperbolic sine, where x is an angle in radians
template <typename FloatN>
FloatN sinh(const FloatN x) noexcept;

//! Compute hyperbolic cosine, where x is an angle in radians
template <typename FloatN>
FloatN cosh(const FloatN x) noexcept;

//! Compute hyperbolic tangent, where x is an angle in radians
template <typename FloatN>
FloatN tanh(const FloatN x) noexcept;

//! Inverse hyperbolic sine. Returns an angle in radians
template <typename FloatN>
FloatN asinh(const FloatN x) noexcept;

//! Inverse hyperbolic cosine. Returns an angle in radians
template <typename FloatN>
FloatN acosh(const FloatN x) noexcept;

//! Hyperbolic arc tangent. Returns an angle in radians
template <typename FloatN>
FloatN atanh(const FloatN x) noexcept;

// Error and gamma functions

//! Error function encountered in integrating the normal distribution.
template <typename FloatN>
FloatN erf(const FloatN x) noexcept;

//! Complementary error function.
template <typename FloatN>
FloatN erfc(const FloatN x) noexcept;

//! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
template <typename FloatN>
FloatN lgamma(const FloatN x) noexcept;

//! Compute the gamma function.
template <typename FloatN>
FloatN tgamma(const FloatN x) noexcept;

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

// Exponential functions

//! Compute the base-e exponential of x.
template <typename FloatN>
FloatN exp(const FloatN x) noexcept;

//! Exponential base 2 function.
template <typename FloatN>
FloatN exp2(const FloatN x) noexcept;

//! Compute e^x - 1.0.
template <typename FloatN>
FloatN expm1(const FloatN x) noexcept;

//! Compute natural logarithm.
template <typename FloatN>
FloatN log(const FloatN x) noexcept;

//! Compute a base 2 logarithm.
template <typename FloatN>
FloatN log2(const FloatN x) noexcept;

//! Compute a base 10 logarithm.
template <typename FloatN>
FloatN log10(const FloatN x) noexcept;

//! Compute log_e(1.0 + x).
template <typename FloatN>
FloatN log1p(const FloatN x) noexcept;

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

//! Compute the value of the square root of x + y without undue overflow or underflow.
template <typename FloatN>
FloatN hypot(const FloatN x, const FloatN y) noexcept;

// Trigonometric functions

//! Compute sine, where x is an angle in radians.
template <typename FloatN>
FloatN sin(const FloatN x) noexcept;

//! Compute cosine, where x is an angle in radians.
template <typename FloatN>
FloatN cos(const FloatN x) noexcept;

//! Compute tangent, where x is an angle in radians.
template <typename FloatN>
FloatN tan(const FloatN x) noexcept;

//! Arc sine function. Returns an angle in radians.
template <typename FloatN>
FloatN asin(const FloatN x) noexcept;

//! Arc cosine function. Returns an angle in radians.
template <typename FloatN>
FloatN acos(const FloatN x) noexcept;

//! Arc tangent function. Returns an angle in radians.
template <typename FloatN>
FloatN atan(const FloatN x) noexcept;

//! Arc tangent of y / x. Returns an angle in radians.
template <typename FloatN>
FloatN atan2(const FloatN y, const FloatN x) noexcept;

// Hyperbolic functions

//! Compute hyperbolic sine, where x is an angle in radians
template <typename FloatN>
FloatN sinh(const FloatN x) noexcept;

//! Compute hyperbolic cosine, where x is an angle in radians
template <typename FloatN>
FloatN cosh(const FloatN x) noexcept;

//! Compute hyperbolic tangent, where x is an angle in radians
template <typename FloatN>
FloatN tanh(const FloatN x) noexcept;

//! Inverse hyperbolic sine. Returns an angle in radians
template <typename FloatN>
FloatN asinh(const FloatN x) noexcept;

//! Inverse hyperbolic cosine. Returns an angle in radians
template <typename FloatN>
FloatN acosh(const FloatN x) noexcept;

//! Hyperbolic arc tangent. Returns an angle in radians
template <typename FloatN>
FloatN atanh(const FloatN x) noexcept;

// Error and gamma functions

//! Error function encountered in integrating the normal distribution.
template <typename FloatN>
FloatN erf(const FloatN x) noexcept;

//! Complementary error function.
template <typename FloatN>
FloatN erfc(const FloatN x) noexcept;

//! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
template <typename FloatN>
FloatN lgamma(const FloatN x) noexcept;

//! Compute the gamma function.
template <typename FloatN>
FloatN tgamma(const FloatN x) noexcept;

} /* namespace native */

namespace precision {

// Exponential functions

//! Compute the base-e exponential of x.
template <typename FloatN>
FloatN exp(const FloatN x) noexcept;

//! Exponential base 2 function.
template <typename FloatN>
FloatN exp2(const FloatN x) noexcept;

//! Compute e^x - 1.0.
template <typename FloatN>
FloatN expm1(const FloatN x) noexcept;

//! Compute natural logarithm.
template <typename FloatN>
FloatN log(const FloatN x) noexcept;

//! Compute a base 2 logarithm.
template <typename FloatN>
FloatN log2(const FloatN x) noexcept;

//! Compute a base 10 logarithm.
template <typename FloatN>
FloatN log10(const FloatN x) noexcept;

//! Compute log_e(1.0 + x).
template <typename FloatN>
FloatN log1p(const FloatN x) noexcept;

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

//! Compute the value of the square root of x + y without undue overflow or underflow.
template <typename FloatN>
FloatN hypot(const FloatN x, const FloatN y) noexcept;

// Trigonometric functions

//! Compute sine, where x is an angle in radians.
template <typename FloatN>
FloatN sin(const FloatN x) noexcept;

//! Compute cosine, where x is an angle in radians.
template <typename FloatN>
FloatN cos(const FloatN x) noexcept;

//! Compute tangent, where x is an angle in radians.
template <typename FloatN>
FloatN tan(const FloatN x) noexcept;

//! Arc sine function. Returns an angle in radians.
template <typename FloatN>
FloatN asin(const FloatN x) noexcept;

//! Arc cosine function. Returns an angle in radians.
template <typename FloatN>
FloatN acos(const FloatN x) noexcept;

//! Arc tangent function. Returns an angle in radians.
template <typename FloatN>
FloatN atan(const FloatN x) noexcept;

//! Arc tangent of y / x. Returns an angle in radians.
template <typename FloatN>
FloatN atan2(const FloatN y, const FloatN x) noexcept;

// Hyperbolic functions

//! Compute hyperbolic sine, where x is an angle in radians
template <typename FloatN>
FloatN sinh(const FloatN x) noexcept;

//! Compute hyperbolic cosine, where x is an angle in radians
template <typename FloatN>
FloatN cosh(const FloatN x) noexcept;

//! Compute hyperbolic tangent, where x is an angle in radians
template <typename FloatN>
FloatN tanh(const FloatN x) noexcept;

//! Inverse hyperbolic sine. Returns an angle in radians
template <typename FloatN>
FloatN asinh(const FloatN x) noexcept;

//! Inverse hyperbolic cosine. Returns an angle in radians
template <typename FloatN>
FloatN acosh(const FloatN x) noexcept;

//! Hyperbolic arc tangent. Returns an angle in radians
template <typename FloatN>
FloatN atanh(const FloatN x) noexcept;

// Error and gamma functions

//! Error function encountered in integrating the normal distribution.
template <typename FloatN>
FloatN erf(const FloatN x) noexcept;

//! Complementary error function.
template <typename FloatN>
FloatN erfc(const FloatN x) noexcept;

//! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
template <typename FloatN>
FloatN lgamma(const FloatN x) noexcept;

//! Compute the gamma function.
template <typename FloatN>
FloatN tgamma(const FloatN x) noexcept;

} /* namespace native */

} /* namespace zivc */

#include "math-inl.hpp"

#endif /* ZIVC_CL_MATH_HPP */