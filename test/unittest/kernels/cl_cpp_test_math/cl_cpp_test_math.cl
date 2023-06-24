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
#include "zivc/cl/math.hpp"
#include "zivc/cl/numbers.hpp"
#include "zivc/cl/types.hpp"
#include "zivc/cl/type_traits.hpp"
#include "zivc/cl/utility.hpp"
#include "zivc/cl/vector_data.hpp"

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

__kernel void sqrtV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::sqrt(x);
  out[index] = y;
}

__kernel void sqrtV1FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::Math::Fallback::sqrt(x);
  out[index] = y;
}

__kernel void sqrtV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::sqrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void sqrtV2FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::Math::Fallback::sqrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void sqrtV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::sqrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void sqrtV3FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::Math::Fallback::sqrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void sqrtV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::sqrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void sqrtV4FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::Math::Fallback::sqrt(x);
  zivc::vstore4(y, index, out);
}

//! \todo Resolve the compile error using clspv
//__kernel void sqrtV8TestKernel(zivc::ConstGlobalPtr<float> in,
//                               zivc::GlobalPtr<float> out,
//                               const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float8 x = zivc::vload8(index, in);
//  const float8 y = zivc::sqrt(x);
//  zivc::vstore8(y, index, out);
//}

__kernel void sqrtV8FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::Math::Fallback::sqrt(x);
  zivc::vstore8(y, index, out);
}

//! \todo Resolve the compile error using clspv
//__kernel void sqrtV16TestKernel(zivc::ConstGlobalPtr<float> in,
//                                zivc::GlobalPtr<float> out,
//                                const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float16 x = zivc::vload16(index, in);
//  const float16 y = zivc::sqrt(x);
//  zivc::vstore16(y, index, out);
//}

__kernel void sqrtV16FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::Math::Fallback::sqrt(x);
  zivc::vstore16(y, index, out);
}

__kernel void rsqrtV1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::rsqrt(x);
  out[index] = y;
}

__kernel void rsqrtV1FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::Math::Fallback::rsqrt(x);
  out[index] = y;
}

__kernel void rsqrtV2TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::rsqrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void rsqrtV2FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::Math::Fallback::rsqrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void rsqrtV3TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::rsqrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void rsqrtV3FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::Math::Fallback::rsqrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void rsqrtV4TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::rsqrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void rsqrtV4FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::Math::Fallback::rsqrt(x);
  zivc::vstore4(y, index, out);
}

//! \todo Resolve the compile error using clspv
//__kernel void rsqrtV8TestKernel(zivc::ConstGlobalPtr<float> in,
//                                zivc::GlobalPtr<float> out,
//                                const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float8 x = zivc::vload8(index, in);
//  const float8 y = zivc::rsqrt(x);
//  zivc::vstore8(y, index, out);
//}

__kernel void rsqrtV8FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::Math::Fallback::rsqrt(x);
  zivc::vstore8(y, index, out);
}

//! \todo Resolve the compile error using clspv
//__kernel void rsqrtV16TestKernel(zivc::ConstGlobalPtr<float> in,
//                                 zivc::GlobalPtr<float> out,
//                                 const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float16 x = zivc::vload16(index, in);
//  const float16 y = zivc::rsqrt(x);
//  zivc::vstore16(y, index, out);
//}

__kernel void rsqrtV16FallbackTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::Math::Fallback::rsqrt(x);
  zivc::vstore16(y, index, out);
}

#endif // Z_MAC

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH */
