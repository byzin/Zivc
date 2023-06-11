/*!
  \file opencl_cpp_test_math.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_MATH
#define ZIVC_TEST_OPENCL_CPP_TEST_MATH

// Zivc
//#include "zivc/cl/math.hpp"
#include "zivc/cl/numbers.hpp"
#include "zivc/cl/types.hpp"
#include "zivc/cl/type_traits.hpp"
#include "zivc/cl/utility.hpp"

using zivc::int8b;
using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint8b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

__kernel void mathConstantFloatKernel(zivc::GlobalPtr<float> out)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  size_t i = 0;
  {
    constexpr float v = zivc::numbers::kE<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kLog2E<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kLog10E<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kLn2<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kLn10<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kPi<float>;
    out[i++] = v;
  }
  {
    constexpr float v = M_PI_2_F;
    out[i++] = v;
  }
  {
    constexpr float v = M_PI_4_F;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kInvPi<float>;
    out[i++] = v;
  }
  {
    constexpr float v = M_2_PI_F;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kInvSqrtPi<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kSqrt2<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kInvSqrt2<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kSqrt3<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kInvSqrt3<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kEgamma<float>;
    out[i++] = v;
  }
  {
    constexpr float v = zivc::numbers::kPhi<float>;
    out[i++] = v;
  }
}

#if !defined(Z_MAC)

__kernel void mathConstantDoubleKernel(zivc::GlobalPtr<double> out)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  size_t i = 0;
  {
    constexpr double v = zivc::numbers::kE<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kLog2E<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kLog10E<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kLn2<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kLn10<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kPi<double>;
    out[i++] = v;
  }
  {
    constexpr double v = M_PI_2;
    out[i++] = v;
  }
  {
    constexpr double v = M_PI_4;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kInvPi<double>;
    out[i++] = v;
  }
  {
    constexpr double v = M_2_PI;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kInvSqrtPi<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kSqrt2<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kInvSqrt2<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kSqrt3<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kInvSqrt3<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kEgamma<double>;
    out[i++] = v;
  }
  {
    constexpr double v = zivc::numbers::kPhi<double>;
    out[i++] = v;
  }
}

#endif // Z_MAC

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH */
