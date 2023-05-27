/*!
  \file numbers-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \note No notation.
  \attention No attention.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_NUMBERS_INL_HPP
#define ZIVC_CL_NUMBERS_INL_HPP

#include "numbers.hpp"

namespace zivc::numbers {

template <> inline constexpr float kE<float> = M_E_F;
template <> inline constexpr double kE<double> = M_E;

template <> inline constexpr float kLog2E<float> = M_LOG2E_F;
template <> inline constexpr double kLog2E<double> = M_LOG2E;

template <> inline constexpr float kLog10E<float> = M_LOG10E_F;
template <> inline constexpr double kLog10E<double> = M_LOG10E;

template <> inline constexpr float kPi<float> = M_PI_F;
template <> inline constexpr double kPi<double> = M_PI;

template <> inline constexpr float kInvPi<float> = M_1_PI_F;
template <> inline constexpr double kInvPi<double> = M_1_PI;

template <> inline constexpr float kInvSqrtPi<float> = M_2_SQRTPI_F / 2.0f;
template <> inline constexpr double kInvSqrtPi<double> = M_2_SQRTPI / 2.0;

template <> inline constexpr float kLn2<float> = M_LN2_F;
template <> inline constexpr double kLn2<double> = M_LN2;

template <> inline constexpr float kLn10<float> = M_LN10_F;
template <> inline constexpr double kLn10<double> = M_LN10;

template <> inline constexpr float kSqrt2<float> = M_SQRT2_F;
template <> inline constexpr double kSqrt2<double> = M_SQRT2;

template <> inline constexpr float kSqrt3<float> = 1.732051f;
template <> inline constexpr double kSqrt3<double> = 1.732050807568877;

template <> inline constexpr float kInvSqrt2<float> = M_SQRT1_2_F;
template <> inline constexpr double kInvSqrt2<double> = M_SQRT1_2;

template <> inline constexpr float kInvSqrt3<float> = 0.5773503f;
template <> inline constexpr double kInvSqrt3<double> = 0.5773502691896257;

template <> inline constexpr float kEgamma<float> = 0.5772157f;
template <> inline constexpr double kEgamma<double> = 0.5772156649015329;

template <> inline constexpr float kPhi<float> = 1.618034f;
template <> inline constexpr double kPhi<double> = 1.618033988749895;

} /* namespace zivc::numbers */

#endif /* ZIVC_CL_NUMBERS_INL_HPP */
