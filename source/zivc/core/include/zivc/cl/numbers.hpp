/*!
  \file numbers.hpp
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

#ifndef ZIVC_CL_NUMBERS_HPP
#define ZIVC_CL_NUMBERS_HPP

namespace zivc::numbers {

//! The mathematical constant e
template <typename Float> inline constexpr Float kE = static_cast<Float>(0);

//! log2(e)
template <typename Float> inline constexpr Float kLog2E = static_cast<Float>(0);

//! log10(e)
template <typename Float> inline constexpr Float kLog10E = static_cast<Float>(0);

//! The mathematical constant π
template <typename Float> inline constexpr Float kPi = static_cast<Float>(0);

//! 1 / π
template <typename Float> inline constexpr Float kInvPi = static_cast<Float>(0);

//! 1 / √π
template <typename Float> inline constexpr Float kInvSqrtPi = static_cast<Float>(0);

//! ln(2)
template <typename Float> inline constexpr Float kLn2 = static_cast<Float>(0);

//! ln(10)
template <typename Float> inline constexpr Float kLn10 = static_cast<Float>(0);

//! √2
template <typename Float> inline constexpr Float kSqrt2 = static_cast<Float>(0);

//! √3
template <typename Float> inline constexpr Float kSqrt3 = static_cast<Float>(0);

//! 1 / √2
template <typename Float> inline constexpr Float kInvSqrt2 = static_cast<Float>(0);

//! 1 / √3
template <typename Float> inline constexpr Float kInvSqrt3 = static_cast<Float>(0);

//! The Euler–Mascheroni constant γ
template <typename Float> inline constexpr Float kEgamma = static_cast<Float>(0);

//! the golden ratio Φ ((1 + √5) / 2)
template <typename Float> inline constexpr Float kPhi = static_cast<Float>(0);

} /* namespace zivc::numbers */

#include "numbers-inl.hpp"

#endif /* ZIVC_CL_NUMBERS_HPP */
