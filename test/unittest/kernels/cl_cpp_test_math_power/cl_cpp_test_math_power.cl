/*!
  \file opencl_cpp_test_math_power.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_MATH_POWER_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_MATH_POWER_CL

// Zivc
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

__kernel void expV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void exp2V1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void expm1V1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void logV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void log2V1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void log10V1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void log1pV1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x;
  out[index] = y;
}

__kernel void powV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution,
                              const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = base * x;
  out[index] = y;
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

__kernel void cbrtV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = x; //zivc::cqrt(x);
  out[index] = y;
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH_POWER_CL */
