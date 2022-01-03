/*!
  \file zivc_config.cl
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_CONFIG_INL_CL
#define ZIVC_ZIVC_CONFIG_INL_CL

// Zivc
#include "zivc_config.cl"

namespace zivc {

/*!
  */
inline
constexpr bool Config::isBuiltinMathUsed() noexcept
{
#if defined(ZIVC_MATH_BUILTIN)
  const bool flag = true;
#else // ZIVC_MATH_BUILTIN
  const bool flag = false;
#endif // ZIVC_MATH_BUILTIN
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathFrexpUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_FREXP)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_FREXP
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathLdexpUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_LDEXP)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_LDEXP
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathIlogbUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_ILOGB)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_ILOGB
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathModfUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_MODF)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_MODF
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathRoundUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_ROUND)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_ROUND
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathFmodUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_FMOD)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_FMOD
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathExpUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_EXP)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_EXP
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathLogUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_LOG)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_LOG
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathPowUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_POW)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_POW
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathSqrtUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_SQRT)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_SQRT
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathCbrtUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_CBRT)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_CBRT
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathTrigonometricUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_TRIGONOMETRIC)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_TRIGONOMETRIC
  return flag;
}

/*!
  */
inline
constexpr bool Config::isBuiltinMathInvTrigonometricUsed() noexcept
{
  bool flag = isBuiltinMathUsed();
#if defined(ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC)
  flag = true;
#endif // ZIVC_MATH_BUILTIN_INV_TRIGONOMETRIC
  return flag;
}

} // namespace zivc

#endif /* ZIVC_ZIVC_CONFIG_INL_CL */
