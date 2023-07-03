/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_MATH_HPP
#define ZIVC_CPUCL_MATH_HPP

// Standard C++ library
#include <climits>
#include <cmath>
#include <cstddef>
#include <concepts>
#include <type_traits>
// Zivc
#include "address_space_pointer.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

// Float constants
#if !defined(MAXFLOAT)
inline constexpr float MAXFLOAT = std::numeric_limits<float>::max();
#endif // MAXFLOAT
#if !defined(INFINITY)
inline constexpr float INFINITY = std::numeric_limits<float>::infinity();
#endif // INFINITY
#if !defined(NAN)
inline constexpr float NAN = std::numeric_limits<float>::quiet_NaN();
#endif // NAN 
#if !defined(FLT_DIG)
inline constexpr size_t FLT_DIG = 6;
#endif // FLT_DIG
#if !defined(FLT_MANT_DIG)
inline constexpr size_t FLT_MANT_DIG = 24;
#endif // FLT_MANT_DIG
#if !defined(FLT_MAX_10_EXP)
inline constexpr int FLT_MAX_10_EXP = +38;
#endif // FLT_MAX_10_EXP
#if !defined(FLT_MAX_EXP)
inline constexpr int FLT_MAX_EXP = +128;
#endif // FLT_MAX_EXP 
#if !defined(FLT_MIN_10_EXP)
inline constexpr int FLT_MIN_10_EXP = -37;
#endif // FLT_MIN_10_EXP
#if !defined(FLT_MIN_EXP)
inline constexpr int FLT_MIN_EXP = -125;
#endif // FLT_MIN_EXP
#if !defined(FLT_RADIX)
inline constexpr size_t FLT_RADIX = 2;
#endif // FLT_RADIX
#if !defined(FLT_MAX)
inline constexpr float FLT_MAX = 0x1.fffffep127f;
#endif // FLT_MAX
#if !defined(FLT_MIN)
inline constexpr float FLT_MIN = 0x1.0p-126f;
#endif // FLT_MIN
#if !defined(FLT_EPSILON)
inline constexpr float FLT_EPSILON = 0x1.0p-23f;
#endif // FLT_EPSILON
#if !defined(DBL_DIG)
inline constexpr size_t DBL_DIG = 15;
#endif // DBL_DIG
#if !defined(DBL_MANT_DIG)
inline constexpr size_t DBL_MANT_DIG = 53;
#endif // DBL_MANT_DIG
#if !defined(DBL_MAX_10_EXP)
inline constexpr int DBL_MAX_10_EXP = +308;
#endif // DBL_MAX_10_EXP
#if !defined(DBL_MAX_EXP)
inline constexpr int DBL_MAX_EXP = +1024;
#endif // DBL_MAX_EXP 
#if !defined(DBL_MIN_10_EXP)
inline constexpr int DBL_MIN_10_EXP = -307;
#endif // DBL_MIN_10_EXP
#if !defined(DBL_MIN_EXP)
inline constexpr int DBL_MIN_EXP = -1021;
#endif // DBL_MIN_EXP
#if !defined(DBL_MAX)
inline constexpr double DBL_MAX = 0x1.fffffffffffffp1023;
#endif // DBL_MAX
#if !defined(DBL_MIN)
inline constexpr double DBL_MIN = 0x1.0p-1022;
#endif // DBL_MIN
#if !defined(DBL_EPSILON)
inline constexpr double DBL_EPSILON = 0x1.0p-52;
#endif // DBL_EPSILON

// Math constants
#if !defined(M_E_F)
inline constexpr float M_E_F = 2.718281746e+00f;
#endif // M_E_F
#if !defined(M_LOG2E_F)
inline constexpr float M_LOG2E_F = 1.442695022e+00f;
#endif // M_LOG2E_F
#if !defined(M_LOG10E_F)
inline constexpr float M_LOG10E_F = 4.342944920e-01f;
#endif // M_LOG10E_F
#if !defined(M_LN2_F)
inline constexpr float M_LN2_F = 6.931471825e-01f;
#endif // M_LN2_F
#if !defined(M_LN10_F)
inline constexpr float M_LN10_F = 2.302585125e+00f;
#endif // M_LN10_F
#if !defined(M_PI_F)
inline constexpr float M_PI_F = 3.141592741e+00f;
#endif // M_PI_F
#if !defined(M_PI_2_F)
inline constexpr float M_PI_2_F = M_PI_F / 2.0f;
#endif // M_PI_2_F
#if !defined(M_PI_4_F)
inline constexpr float M_PI_4_F = M_PI_F / 4.0f;
#endif // M_PI_4_F
#if !defined(M_1_PI_F)
inline constexpr float M_1_PI_F = 1.0f / M_PI_F;
#endif // M_1_PI_F
#if !defined(M_2_PI_F)
inline constexpr float M_2_PI_F = 2.0f / M_PI_F;
#endif // M_2_PI_F
#if !defined(M_2_SQRTPI_F)
inline constexpr float M_2_SQRTPI_F = 1.128379226e+00f;
#endif // M_2_SQRTPI_F
#if !defined(M_SQRT2_F)
inline constexpr float M_SQRT2_F = 1.414213538e+00f;
#endif // M_SQRT2_F
#if !defined(M_SQRT1_2_F)
inline constexpr float M_SQRT1_2_F = 1.0f / M_SQRT2_F;
#endif // M_SQRT1_2_F
#if !defined(M_E)
inline constexpr double M_E = 2.71828182845904523536;
#endif // M_E
#if !defined(M_LOG2E)
inline constexpr double M_LOG2E = 1.44269504088896340736;
#endif // M_LOG2E
#if !defined(M_LOG10E)
inline constexpr double M_LOG10E = 0.434294481903251827651;
#endif // M_LOG10E
#if !defined(M_LN2)
inline constexpr double M_LN2 = 0.693147180559945309417;
#endif // M_LN2
#if !defined(M_LN10)
inline constexpr double M_LN10 = 2.30258509299404568402;
#endif // M_LN10
#if !defined(M_PI)
inline constexpr double M_PI = 3.14159265358979323846;
#endif // M_PI
#if !defined(M_PI_2)
inline constexpr double M_PI_2 = M_PI / 2.0;
#endif // M_PI_2
#if !defined(M_PI_4)
inline constexpr double M_PI_4 = M_PI / 4.0;
#endif // M_PI_4
#if !defined(M_1_PI)
inline constexpr double M_1_PI = 1.0 / M_PI;
#endif // M_1_PI
#if !defined(M_2_PI)
inline constexpr double M_2_PI = 2.0 / M_PI;
#endif // M_2_PI
#if !defined(M_2_SQRTPI)
inline constexpr double M_2_SQRTPI = 1.12837916709551257390;
#endif // M_2_SQRTPI
#if !defined(M_SQRT2)
inline constexpr double M_SQRT2 = 1.41421356237309504880;
#endif // M_SQRT2
#if !defined(M_SQRT1_2)
inline constexpr double M_SQRT1_2 = 1.0 / M_SQRT2;
#endif // M_SQRT1_2

/*!
  \brief No brief description

  No detailed description.
  */
class Math
{
 public:
  //! Invert the given value
  template <std::floating_point Float>
  static constexpr Float invert(const Float& x) noexcept;

  //! Invert the given value
  template <std::floating_point Float, std::size_t kN>
  static constexpr Vector<Float, kN> invert(const Vector<Float, kN>& x) noexcept;

  // Basic operations

  //! Compute absolute value of a floating-point number.
  template <std::floating_point Float>
  static Float fabs(const Float& x) noexcept;

  //! Compute absolute value of a floating-point number.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fabs(const Vector<Float, kN>& x) noexcept;

  //! Modulus. Returns x - y * trunc(x/y).
  template <std::floating_point Float>
  static Float fmod(const Float& x, const Float& y) noexcept;

  //! Modulus. Returns x - y * trunc(x/y).
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fmod(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
  template <std::floating_point Float>
  static Float remainder(const Float& x, const Float& y) noexcept;

  //! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> remainder(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
  template <std::floating_point Float> 
  static Float remquo(const Float& x,
                      const Float& y,
                      int32b* quo) noexcept;

  //! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
  template <std::floating_point Float, AddressSpaceType kASpaceType> 
  static Float remquo(const Float& x,
                      const Float& y,
                      AddressSpacePointer<kASpaceType, int32b> quo) noexcept;

  //! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
  template <std::floating_point Float, std::size_t kN> 
  static Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                                  const Vector<Float, kN>& y,
                                  Vector<int32b, kN>* quo) noexcept;

  //! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
  template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> 
  static Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                                  const Vector<Float, kN>& y,
                                  AddressSpacePointer<kASpaceType, Vector<int32b, kN>> quo) noexcept;

  //! Compute a * b + c.
  template <std::floating_point Float>
  static Float mad(const Float& a, const Float& b, const Float& c) noexcept;

  //! Compute a * b + c.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> mad(const Vector<Float, kN>& a, const Vector<Float, kN>& b, const Vector<Float, kN>& c) noexcept;

  //! Returns the correctly rounded floating-point representation of the sum of c with the infinitely precise product of a and b.
  template <std::floating_point Float>
  static Float fma(const Float& a, const Float& b, const Float& c) noexcept;

  //! Returns the correctly rounded floating-point representation of the sum of c with the infinitely precise product of a and b.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fma(const Vector<Float, kN>& a, const Vector<Float, kN>& b, const Vector<Float, kN>& c) noexcept;

  //! Returns y if x < y, otherwise it returns x.
  template <std::floating_point Float>
  static Float fmax(const Float& x, const Float& y) noexcept;

  //! Returns y if x < y, otherwise it returns x.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fmax(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Returns y if y < x, otherwise it returns x.
  template <std::floating_point Float>
  static Float fmin(const Float& x, const Float& y) noexcept;

  //! Returns y if y < x, otherwise it returns x.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fmin(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
  template <std::floating_point Float>
  static Float maxmag(const Float& x, const Float& y) noexcept;

  //! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> maxmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
  template <std::floating_point Float>
  static Float minmag(const Float& x, const Float& y) noexcept;

  //! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> minmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! x - y if x > y, +0 if x is less than or equal to y.
  template <std::floating_point Float>
  static Float fdim(const Float& x, const Float& y) noexcept;

  //! x - y if x > y, +0 if x is less than or equal to y.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fdim(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  // Exponential functions

  //! Compute the base-e exponential of x.
  template <std::floating_point Float>
  static Float exp(const Float& x) noexcept;

  //! Compute the base-e exponential of x.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> exp(const Vector<Float, kN>& x) noexcept;

  //! Exponential base 2 function.
  template <std::floating_point Float>
  static Float exp2(const Float& x) noexcept;

  //! Exponential base 2 function.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> exp2(const Vector<Float, kN>& x) noexcept;

//  //! Exponential base 10 function.
//  template <std::floating_point Float>
//  static Float exp10(const Float& x) noexcept;
//
//  //! Exponential base 10 function.
//  template <std::floating_point Float, std::size_t kN>
//  static Vector<Float, kN> exp10(const Vector<Float, kN>& x) noexcept;

  //! Compute e^x - 1.0.
  template <std::floating_point Float>
  static Float expm1(const Float& x) noexcept;

  //! Compute e^x - 1.0.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> expm1(const Vector<Float, kN>& x) noexcept;

  //! Compute natural logarithm.
  template <std::floating_point Float>
  static Float log(const Float& x) noexcept;

  //! Compute natural logarithm.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> log(const Vector<Float, kN>& x) noexcept;

  //! Compute a base 2 logarithm.
  template <std::floating_point Float>
  static Float log2(const Float& x) noexcept;

  //! Compute a base 2 logarithm.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> log2(const Vector<Float, kN>& x) noexcept;

  //! Compute a base 10 logarithm.
  template <std::floating_point Float>
  static Float log10(const Float& x) noexcept;

  //! Compute a base 10 logarithm.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> log10(const Vector<Float, kN>& x) noexcept;

  //! Compute log_e(1.0 + x).
  template <std::floating_point Float>
  static Float log1p(const Float& x) noexcept;

  //! Compute log_e(1.0 + x).
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> log1p(const Vector<Float, kN>& x) noexcept;

  // Power functions

  //! Compute x to the power y
  template <std::floating_point Float>
  static Float pow(const Float& x, const Float& y) noexcept;

  //! Compute x to the power y
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> pow(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Compute x to the power y
  template <std::floating_point Float>
  static Float pown(const Float& x, const int32b y) noexcept;

  //! Compute x to the power y
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> pown(const Vector<Float, kN>& x, const Vector<int32b, kN>& y) noexcept;

  //! Compute square root
  template <std::floating_point Float>
  static Float sqrt(const Float& x) noexcept;

  //! Compute square root
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> sqrt(const Vector<Float, kN>& x) noexcept;

  //! Compute inverse square root
  template <std::floating_point Float>
  static Float rsqrt(const Float& x) noexcept;

  //! Compute inverse square root
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> rsqrt(const Vector<Float, kN>& x) noexcept;

  //! Compute cube root
  template <std::floating_point Float>
  static Float cbrt(const Float& x) noexcept;

  //! Compute cube root
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> cbrt(const Vector<Float, kN>& x) noexcept;

  //! Compute the value of the square root of x + y without undue overflow or underflow.
  template <std::floating_point Float>
  static Float hypot(const Float& x, const Float& y) noexcept;

  //! Compute the value of the square root of x + y without undue overflow or underflow.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> hypot(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  // Trigonometric functions

  //! Compute sine, where x is an angle in radians.
  template <std::floating_point Float>
  static Float sin(const Float& x) noexcept;

  //! Compute sine, where x is an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> sin(const Vector<Float, kN>& x) noexcept;

  //! Compute cosine, where x is an angle in radians.
  template <std::floating_point Float>
  static Float cos(const Float& x) noexcept;

  //! Compute cosine, where x is an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> cos(const Vector<Float, kN>& x) noexcept;

  //! Compute tangent, where x is an angle in radians.
  template <std::floating_point Float>
  static Float tan(const Float& x) noexcept;

  //! Compute tangent, where x is an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> tan(const Vector<Float, kN>& x) noexcept;

  //! Arc sine function. Returns an angle in radians.
  template <std::floating_point Float>
  static Float asin(const Float& x) noexcept;

  //! Arc sine function. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> asin(const Vector<Float, kN>& x) noexcept;

  //! Arc cosine function. Returns an angle in radians.
  template <std::floating_point Float>
  static Float acos(const Float& x) noexcept;

  //! Arc cosine function. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> acos(const Vector<Float, kN>& x) noexcept;

  //! Arc tangent function. Returns an angle in radians.
  template <std::floating_point Float>
  static Float atan(const Float& x) noexcept;

  //! Arc tangent function. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> atan(const Vector<Float, kN>& x) noexcept;

  //! Arc tangent of y / x. Returns an angle in radians.
  template <std::floating_point Float>
  static Float atan2(const Float& y, const Float& x) noexcept;

  //! Arc tangent of y / x. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> atan2(const Vector<Float, kN>& y, const Vector<Float, kN>& x) noexcept;

  // Hyperbolic functions

  //! Compute hyperbolic sine, where x is an angle in radians
  template <std::floating_point Float>
  static Float sinh(const Float& x) noexcept;

  //! Compute hyperbolic sine, where x is an angle in radians
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> sinh(const Vector<Float, kN>& x) noexcept;

  //! Compute hyperbolic cosine, where x is an angle in radians.
  template <std::floating_point Float>
  static Float cosh(const Float& x) noexcept;

  //! Compute hyperbolic cosine, where x is an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> cosh(const Vector<Float, kN>& x) noexcept;

  //! Compute hyperbolic tangent, where x is an angle in radians.
  template <std::floating_point Float>
  static Float tanh(const Float& x) noexcept;

  //! Compute hyperbolic tangent, where x is an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> tanh(const Vector<Float, kN>& x) noexcept;

  //! Inverse hyperbolic sine. Returns an angle in radians.
  template <std::floating_point Float>
  static Float asinh(const Float& x) noexcept;

  //! Inverse hyperbolic sine. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> asinh(const Vector<Float, kN>& x) noexcept;

  //! Inverse hyperbolic cosine. Returns an angle in radians.
  template <std::floating_point Float>
  static Float acosh(const Float& x) noexcept;

  //! Inverse hyperbolic cosine. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> acosh(const Vector<Float, kN>& x) noexcept;

  //! Hyperbolic arc tangent. Returns an angle in radians.
  template <std::floating_point Float>
  static Float atanh(const Float& x) noexcept;

  //! Hyperbolic arc tangent. Returns an angle in radians.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> atanh(const Vector<Float, kN>& x) noexcept;

  // Error and gamma functions

  //! Error function encountered in integrating the normal distribution.
  template <std::floating_point Float>
  static Float erf(const Float& x) noexcept;

  //! Error function encountered in integrating the normal distribution.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> erf(const Vector<Float, kN>& x) noexcept;

  //! Complementary error function.
  template <std::floating_point Float>
  static Float erfc(const Float& x) noexcept;

  //! Complementary error function.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> erfc(const Vector<Float, kN>& x) noexcept;

  //! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
  template <std::floating_point Float>
  static Float lgamma(const Float& x) noexcept;

  //! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> lgamma(const Vector<Float, kN>& x) noexcept;

  //! Compute the gamma function.
  template <std::floating_point Float>
  static Float tgamma(const Float& x) noexcept;

  //! Compute the gamma function.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> tgamma(const Vector<Float, kN>& x) noexcept;

  // Nearest integer floating point operations

  //! Round to integral value using the round to positive infinity rounding mode
  template <std::floating_point Float>
  static Float ceil(const Float& x) noexcept;

  //! Round to integral value using the round to positive infinity rounding mode
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> ceil(const Vector<Float, kN>& x) noexcept;

  //! Round to integral value using the round to negative infinity rounding mode
  template <std::floating_point Float>
  static Float floor(const Float& x) noexcept;

  //! Round to integral value using the round to negative infinity rounding mode
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> floor(const Vector<Float, kN>& x) noexcept;

  //! Round to integral value using the round to zero rounding mode
  template <std::floating_point Float>
  static Float trunc(const Float& x) noexcept;

  //! Round to integral value using the round to zero rounding mode
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> trunc(const Vector<Float, kN>& x) noexcept;

  //! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
  template <std::floating_point Float>
  static Float round(const Float& x) noexcept;

  //! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> round(const Vector<Float, kN>& x) noexcept;

  //! Round to integral value (using round to nearest even rounding mode) in floating point format.
  template <std::floating_point Float>
  static Float rint(const Float& x) noexcept;

  //! Round to integral value (using round to nearest even rounding mode) in floating point format.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> rint(const Vector<Float, kN>& x) noexcept;

  // Floating point manipulation functions

  //! Extract mantissa and exponent from x.
  template <std::floating_point Float>
  static Float frexp(const Float& x, int32b* exp) noexcept;

  //! Extract mantissa and exponent from x.
  template <std::floating_point Float, AddressSpaceType kASpaceType>
  static Float frexp(const Float& x, AddressSpacePointer<kASpaceType, int32b> exp) noexcept;

  //! Extract mantissa and exponent from x.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> frexp(const Vector<Float, kN>& x, Vector<int32b, kN>* exp) noexcept;

  //! Extract mantissa and exponent from x.
  template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
  static Vector<Float, kN> frexp(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<int32b, kN>> exp) noexcept;

  //! Multiply x by 2 to the power k.
  template <std::floating_point Float>
  static Float ldexp(const Float& x, const int32b k) noexcept;

  //! Multiply x by 2 to the power k.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> ldexp(const Vector<Float, kN>& x, const Vector<int32b, kN>& k) noexcept;

  //! Decompose a floating-point number.
  template <std::floating_point Float>
  static Float modf(const Float& x, Float* iptr) noexcept;

  //! Decompose a floating-point number.
  template <std::floating_point Float, AddressSpaceType kASpaceType>
  static Float modf(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept;

  //! Decompose a floating-point number.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> modf(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept;

  //! Decompose a floating-point number.
  template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
  static Vector<Float, kN> modf(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept;

  //! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
  template <std::floating_point Float>
  static Float fract(const Float& x, Float* iptr) noexcept;

  //! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
  template <std::floating_point Float, AddressSpaceType kASpaceType>
  static Float fract(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept;

  //! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> fract(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept;

  //! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
  template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
  static Vector<Float, kN> fract(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept;

  //! Return the exponent as an integer value.
  template <std::floating_point Float>
  static int32b ilogb(const Float& x) noexcept;

  //! Return the exponent as an integer value.
  template <std::floating_point Float, std::size_t kN>
  static Vector<int32b, kN> ilogb(const Vector<Float, kN>& x) noexcept;

  //! Compute the exponent of x, which is the integral part of logr(|x|).
  template <std::floating_point Float>
  static Float logb(const Float& x) noexcept;

  //! Compute the exponent of x, which is the integral part of logr(|x|).
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> logb(const Vector<Float, kN>& x) noexcept;

  //! Computes the next representable single-precision floating-point value following x in the direction of y.
  template <std::floating_point Float>
  static Float nextafter(const Float& x, const Float& y) noexcept;

  //! Computes the next representable single-precision floating-point value following x in the direction of y.
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> nextafter(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

  //! Return x with its sign changed to match the sign of y
  template <std::floating_point Float>
  static Float copysign(const Float& x, const Float& y) noexcept;

  //! Return x with its sign changed to match the sign of y
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> copysign(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  struct Impl
  {
  };
};

// Basic operations

//! Compute absolute value of a floating-point number.
template <std::floating_point Float>
Float fabs(const Float& x) noexcept;

//! Compute absolute value of a floating-point number.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fabs(const Vector<Float, kN>& x) noexcept;

//! Modulus. Returns x - y * trunc(x/y).
template <std::floating_point Float>
Float fmod(const Float& x, const Float& y) noexcept;

//! Modulus. Returns x - y * trunc(x/y).
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fmod(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
template <std::floating_point Float>
Float remainder(const Float& x, const Float& y) noexcept;

//! Compute the value r such that r = x - n*y, where n is the integer nearest the exact value of x/y.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> remainder(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
template <std::floating_point Float> 
Float remquo(const Float& x,
             const Float& y,
             int32b* quo) noexcept;

//! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
template <std::floating_point Float, AddressSpaceType kASpaceType> 
Float remquo(const Float& x,
             const Float& y,
             AddressSpacePointer<kASpaceType, int32b> quo) noexcept;

//! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
template <std::floating_point Float, std::size_t kN> 
Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y,
                         Vector<int32b, kN>* quo) noexcept;

//! The remquo function computes the value r such that r = x - k*y, where k is the integer nearest the exact value of x/y.
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> 
Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y,
                         AddressSpacePointer<kASpaceType, Vector<int32b, kN>> quo) noexcept;

//! Compute a * b + c.
template <std::floating_point Float>
Float mad(const Float& a, const Float& b, const Float& c) noexcept;

//! Compute a * b + c.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> mad(const Vector<Float, kN>& a, const Vector<Float, kN>& b, const Vector<Float, kN>& c) noexcept;

//! Returns y if x < y, otherwise it returns x.
template <std::floating_point Float>
Float fmax(const Float& x, const Float& y) noexcept;

//! Returns the correctly rounded floating-point representation of the sum of c with the infinitely precise product of a and b.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fma(const Vector<Float, kN>& a, const Vector<Float, kN>& b, const Vector<Float, kN>& c) noexcept;

//! Returns y if x < y, otherwise it returns x.
template <std::floating_point Float>
Float fmax(const Float& x, const Float& y) noexcept;

//! Returns y if x < y, otherwise it returns x.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fmax(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Returns y if y < x, otherwise it returns x.
template <std::floating_point Float>
Float fmin(const Float& x, const Float& y) noexcept;

//! Returns y if y < x, otherwise it returns x.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fmin(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
template <std::floating_point Float>
Float maxmag(const Float& x, const Float& y) noexcept;

//! Returns x if |x| > |y|, y if |y| > |x|, otherwise fmax(x, y).
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> maxmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
template <std::floating_point Float>
Float minmag(const Float& x, const Float& y) noexcept;

//! Returns x if |x| < |y|, y if |y| < |x|, otherwise fmin(x, y).
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> minmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! x - y if x > y, +0 if x is less than or equal to y.
template <std::floating_point Float>
Float fdim(const Float& x, const Float& y) noexcept;

//! x - y if x > y, +0 if x is less than or equal to y.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fdim(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

// Exponential functions

//! Compute the base-e exponential of x.
template <std::floating_point Float>
Float exp(const Float& x) noexcept;

//! Compute the base-e exponential of x.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> exp(const Vector<Float, kN>& x) noexcept;

//! Exponential base 2 function.
template <std::floating_point Float>
Float exp2(const Float& x) noexcept;

//! Exponential base 2 function.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> exp2(const Vector<Float, kN>& x) noexcept;

////! Exponential base 10 function.
//template <std::floating_point Float>
//Float exp10(const Float& x) noexcept;
//
////! Exponential base 10 function.
//template <std::floating_point Float, std::size_t kN>
//Vector<Float, kN> exp10(const Vector<Float, kN>& x) noexcept;

//! Compute e^x - 1.0.
template <std::floating_point Float>
Float expm1(const Float& x) noexcept;

//! Compute e^x - 1.0.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> expm1(const Vector<Float, kN>& x) noexcept;

//! Compute natural logarithm.
template <std::floating_point Float>
Float log(const Float& x) noexcept;

//! Compute natural logarithm.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> log(const Vector<Float, kN>& x) noexcept;

//! Compute a base 2 logarithm.
template <std::floating_point Float>
Float log2(const Float& x) noexcept;

//! Compute a base 2 logarithm.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> log2(const Vector<Float, kN>& x) noexcept;

//! Compute a base 10 logarithm.
template <std::floating_point Float>
Float log10(const Float& x) noexcept;

//! Compute a base 10 logarithm.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> log10(const Vector<Float, kN>& x) noexcept;

//! Compute log_e(1.0 + x).
template <std::floating_point Float>
Float log1p(const Float& x) noexcept;

//! Compute log_e(1.0 + x).
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> log1p(const Vector<Float, kN>& x) noexcept;

// Power functions

//! Compute x to the power y
template <std::floating_point Float>
Float pow(const Float& x, const Float& y) noexcept;

//! Compute x to the power y
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> pow(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Compute x to the power y
template <std::floating_point Float>
Float pown(const Float& x, const int32b y) noexcept;

//! Compute x to the power y
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> pown(const Vector<Float, kN>& x, const Vector<int32b, kN>& y) noexcept;

//! Compute square root
template <std::floating_point Float>
Float sqrt(const Float& x) noexcept;

//! Compute square root
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> sqrt(const Vector<Float, kN>& x) noexcept;

//! Compute inverse square root
template <std::floating_point Float>
Float rsqrt(const Float& x) noexcept;

//! Compute inverse square root
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> rsqrt(const Vector<Float, kN>& x) noexcept;

//! Compute cube root
template <std::floating_point Float>
Float cbrt(const Float& x) noexcept;

//! Compute cube root
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> cbrt(const Vector<Float, kN>& x) noexcept;

//! Compute the value of the square root of x + y without undue overflow or underflow.
template <std::floating_point Float>
Float hypot(const Float& x, const Float& y) noexcept;

//! Compute the value of the square root of x + y without undue overflow or underflow.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> hypot(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

// Trigonometric functions

//! Compute sine, where x is an angle in radians.
template <std::floating_point Float>
Float sin(const Float& x) noexcept;

//! Compute sine, where x is an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> sin(const Vector<Float, kN>& x) noexcept;

//! Compute cosine, where x is an angle in radians.
template <std::floating_point Float>
Float cos(const Float& x) noexcept;

//! Compute cosine, where x is an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> cos(const Vector<Float, kN>& x) noexcept;

//! Compute tangent, where x is an angle in radians.
template <std::floating_point Float>
Float tan(const Float& x) noexcept;

//! Compute tangent, where x is an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> tan(const Vector<Float, kN>& x) noexcept;

//! Arc sine function. Returns an angle in radians.
template <std::floating_point Float>
Float asin(const Float& x) noexcept;

//! Arc sine function. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> asin(const Vector<Float, kN>& x) noexcept;

//! Arc cosine function. Returns an angle in radians.
template <std::floating_point Float>
Float acos(const Float& x) noexcept;

//! Arc cosine function. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> acos(const Vector<Float, kN>& x) noexcept;

//! Arc tangent function. Returns an angle in radians.
template <std::floating_point Float>
Float atan(const Float& x) noexcept;

//! Arc tangent function. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> atan(const Vector<Float, kN>& x) noexcept;

//! Arc tangent of y / x. Returns an angle in radians.
template <std::floating_point Float>
Float atan2(const Float& y, const Float& x) noexcept;

//! Arc tangent of y / x. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> atan2(const Vector<Float, kN>& y, const Vector<Float, kN>& x) noexcept;

// Hyperbolic functions

//! Compute hyperbolic sine, where x is an angle in radians
template <std::floating_point Float>
Float sinh(const Float& x) noexcept;

//! Compute hyperbolic sine, where x is an angle in radians
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> sinh(const Vector<Float, kN>& x) noexcept;

//! Compute hyperbolic cosine, where x is an angle in radians.
template <std::floating_point Float>
Float cosh(const Float& x) noexcept;

//! Compute hyperbolic cosine, where x is an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> cosh(const Vector<Float, kN>& x) noexcept;

//! Compute hyperbolic tangent, where x is an angle in radians.
template <std::floating_point Float>
Float tanh(const Float& x) noexcept;

//! Compute hyperbolic tangent, where x is an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> tanh(const Vector<Float, kN>& x) noexcept;

//! Inverse hyperbolic sine. Returns an angle in radians.
template <std::floating_point Float>
Float asinh(const Float& x) noexcept;

//! Inverse hyperbolic sine. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> asinh(const Vector<Float, kN>& x) noexcept;

//! Inverse hyperbolic cosine. Returns an angle in radians.
template <std::floating_point Float>
Float acosh(const Float& x) noexcept;

//! Inverse hyperbolic cosine. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> acosh(const Vector<Float, kN>& x) noexcept;

//! Hyperbolic arc tangent. Returns an angle in radians.
template <std::floating_point Float>
Float atanh(const Float& x) noexcept;

//! Hyperbolic arc tangent. Returns an angle in radians.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> atanh(const Vector<Float, kN>& x) noexcept;

// Error and gamma functions

//! Error function encountered in integrating the normal distribution.
template <std::floating_point Float>
Float erf(const Float& x) noexcept;

//! Error function encountered in integrating the normal distribution.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> erf(const Vector<Float, kN>& x) noexcept;

//! Complementary error function.
template <std::floating_point Float>
Float erfc(const Float& x) noexcept;

//! Complementary error function.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> erfc(const Vector<Float, kN>& x) noexcept;

//! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
template <std::floating_point Float>
Float lgamma(const Float& x) noexcept;

//! Log gamma function. Returns the natural logarithm of the absolute value of the gamma function.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> lgamma(const Vector<Float, kN>& x) noexcept;

//! Compute the gamma function.
template <std::floating_point Float>
Float tgamma(const Float& x) noexcept;

//! Compute the gamma function.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> tgamma(const Vector<Float, kN>& x) noexcept;

// Nearest integer floating point operations

//! Round to integral value using the round to positive infinity rounding mode
template <std::floating_point Float>
Float ceil(const Float& x) noexcept;

//! Round to integral value using the round to positive infinity rounding mode
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> ceil(const Vector<Float, kN>& x) noexcept;

//! Round to integral value using the round to negative infinity rounding mode
template <std::floating_point Float>
Float floor(const Float& x) noexcept;

//! Round to integral value using the round to negative infinity rounding mode
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> floor(const Vector<Float, kN>& x) noexcept;

//! Round to integral value using the round to zero rounding mode
template <std::floating_point Float>
Float trunc(const Float& x) noexcept;

//! Round to integral value using the round to zero rounding mode
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> trunc(const Vector<Float, kN>& x) noexcept;

//! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
template <std::floating_point Float>
Float round(const Float& x) noexcept;

//! Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding direction
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> round(const Vector<Float, kN>& x) noexcept;

//! Round to integral value (using round to nearest even rounding mode) in floating point format.
template <std::floating_point Float>
Float rint(const Float& x) noexcept;

//! Round to integral value (using round to nearest even rounding mode) in floating point format.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> rint(const Vector<Float, kN>& x) noexcept;

// Floating point manipulation functions

//! Extract mantissa and exponent from x.
template <std::floating_point Float>
Float frexp(const Float& x, int32b* exp) noexcept;

//! Extract mantissa and exponent from x.
template <std::floating_point Float, AddressSpaceType kASpaceType>
Float frexp(const Float& x, AddressSpacePointer<kASpaceType, int32b> exp) noexcept;

//! Extract mantissa and exponent from x.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> frexp(const Vector<Float, kN>& x, Vector<int32b, kN>* exp) noexcept;

//! Extract mantissa and exponent from x.
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
Vector<Float, kN> frexp(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<int32b, kN>> exp) noexcept;

//! Multiply x by 2 to the power k.
template <std::floating_point Float>
Float ldexp(const Float& x, const int32b k) noexcept;

//! Multiply x by 2 to the power k.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> ldexp(const Vector<Float, kN>& x, const Vector<int32b, kN>& k) noexcept;

//! Decompose a floating-point number.
template <std::floating_point Float>
Float modf(const Float& x, Float* iptr) noexcept;

//! Decompose a floating-point number.
template <std::floating_point Float, AddressSpaceType kASpaceType>
Float modf(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept;

//! Decompose a floating-point number.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> modf(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept;

//! Decompose a floating-point number.
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
Vector<Float, kN> modf(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept;

//! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
template <std::floating_point Float>
Float fract(const Float& x, Float* iptr) noexcept;

//! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
template <std::floating_point Float, AddressSpaceType kASpaceType>
Float fract(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept;

//! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> fract(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept;

//! Return fmin(x - floor(x), 0x1.fffffep-1f). floor(x) is returned in iptr.
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN>
Vector<Float, kN> fract(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept;

//! Return the exponent as an integer value.
template <std::floating_point Float>
int32b ilogb(const Float& x) noexcept;

//! Return the exponent as an integer value.
template <std::floating_point Float, std::size_t kN>
Vector<int32b, kN> ilogb(const Vector<Float, kN>& x) noexcept;

//! Compute the exponent of x, which is the integral part of logr(|x|).
template <std::floating_point Float>
Float logb(const Float& x) noexcept;

//! Compute the exponent of x, which is the integral part of logr(|x|).
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> logb(const Vector<Float, kN>& x) noexcept;

//! Computes the next representable single-precision floating-point value following x in the direction of y.
template <std::floating_point Float>
Float nextafter(const Float& x, const Float& y) noexcept;

//! Computes the next representable single-precision floating-point value following x in the direction of y.
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> nextafter(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

//! Return x with its sign changed to match the sign of y
template <std::floating_point Float>
Float copysign(const Float& x, const Float& y) noexcept;

//! Return x with its sign changed to match the sign of y
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> copysign(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept;

} // namespace zivc::cl

#include "math-inl.hpp"

#endif // ZIVC_CPUCL_MATH_HPP
