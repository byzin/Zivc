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
  const float y = zivc::pow(base, x);
  out[index] = y;
}

__kernel void powV1PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::precision::pow(base, x);
  out[index] = y;
}

__kernel void powV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution,
                              const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::pow(zivc::makeFloat2(base), x);
  zivc::vstore2(y, index, out);
}

__kernel void powV2PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::precision::pow(zivc::makeFloat2(base), x);
  zivc::vstore2(y, index, out);
}

__kernel void powV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution,
                              const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::pow(zivc::makeFloat3(base), x);
  zivc::vstore3(y, index, out);
}

__kernel void powV3PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::precision::pow(zivc::makeFloat3(base), x);
  zivc::vstore3(y, index, out);
}

__kernel void powV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution,
                              const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::pow(zivc::makeFloat4(base), x);
  zivc::vstore4(y, index, out);
}

__kernel void powV4PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::precision::pow(zivc::makeFloat4(base), x);
  zivc::vstore4(y, index, out);
}

__kernel void powV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution,
                              const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::pow(zivc::makeFloat8(base), x);
  zivc::vstore8(y, index, out);
}

__kernel void powV8PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::precision::pow(zivc::makeFloat8(base), x);
  zivc::vstore8(y, index, out);
}

__kernel void powV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::pow(zivc::makeFloat16(base), x);
  zivc::vstore16(y, index, out);
}

__kernel void powV16PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution,
                                        const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::precision::pow(zivc::makeFloat16(base), x);
  zivc::vstore16(y, index, out);
}

__kernel void pownV1TestKernel(zivc::ConstGlobalPtr<int32b> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int32b x = in[index];
  const float y = zivc::pown(base, x);
  out[index] = y;
}

__kernel void pownV1PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                       zivc::GlobalPtr<float> out,
                                       const uint32b resolution,
                                       const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int32b x = in[index];
  const float y = zivc::precision::pown(base, x);
  out[index] = y;
}

__kernel void pownV2TestKernel(zivc::ConstGlobalPtr<int32b> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int2 x = zivc::vload2(index, in);
  const float2 y = zivc::pown(zivc::makeFloat2(base), x);
  zivc::vstore2(y, index, out);
}

__kernel void pownV2PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution,
                                        const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int2 x = zivc::vload2(index, in);
  const float2 y = zivc::precision::pown(zivc::makeFloat2(base), x);
  zivc::vstore2(y, index, out);
}

__kernel void pownV3TestKernel(zivc::ConstGlobalPtr<int32b> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int3 x = zivc::vload3(index, in);
  const float3 y = zivc::pown(zivc::makeFloat3(base), x);
  zivc::vstore3(y, index, out);
}

__kernel void pownV3PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution,
                                        const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int3 x = zivc::vload3(index, in);
  const float3 y = zivc::precision::pown(zivc::makeFloat3(base), x);
  zivc::vstore3(y, index, out);
}

__kernel void pownV4TestKernel(zivc::ConstGlobalPtr<int32b> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int4 x = zivc::vload4(index, in);
  const float4 y = zivc::pown(zivc::makeFloat4(base), x);
  zivc::vstore4(y, index, out);
}

__kernel void pownV4PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution,
                                        const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int4 x = zivc::vload4(index, in);
  const float4 y = zivc::precision::pown(zivc::makeFloat4(base), x);
  zivc::vstore4(y, index, out);
}

__kernel void pownV8TestKernel(zivc::ConstGlobalPtr<int32b> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution,
                               const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int8 x = zivc::vload8(index, in);
  const float8 y = zivc::pown(zivc::makeFloat8(base), x);
  zivc::vstore8(y, index, out);
}

__kernel void pownV8PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution,
                                        const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int8 x = zivc::vload8(index, in);
  const float8 y = zivc::precision::pown(zivc::makeFloat8(base), x);
  zivc::vstore8(y, index, out);
}

__kernel void pownV16TestKernel(zivc::ConstGlobalPtr<int32b> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution,
                                const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int16 x = zivc::vload16(index, in);
  const float16 y = zivc::pown(zivc::makeFloat16(base), x);
  zivc::vstore16(y, index, out);
}

__kernel void pownV16PrecisionTestKernel(zivc::ConstGlobalPtr<int32b> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution,
                                         const float base)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const int16 x = zivc::vload16(index, in);
  const float16 y = zivc::precision::pown(zivc::makeFloat16(base), x);
  zivc::vstore16(y, index, out);
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

__kernel void sqrtV1PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::precision::sqrt(x);
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

__kernel void sqrtV2PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::precision::sqrt(x);
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

__kernel void sqrtV3PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::precision::sqrt(x);
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

__kernel void sqrtV4PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::precision::sqrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void sqrtV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::sqrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void sqrtV8PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::precision::sqrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void sqrtV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::sqrt(x);
  zivc::vstore16(y, index, out);
}

__kernel void sqrtV16PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::precision::sqrt(x);
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

__kernel void rsqrtV1PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::precision::rsqrt(x);
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

__kernel void rsqrtV2PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::precision::rsqrt(x);
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

__kernel void rsqrtV3PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::precision::rsqrt(x);
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

__kernel void rsqrtV4PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::precision::rsqrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void rsqrtV8TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::rsqrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void rsqrtV8PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::precision::rsqrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void rsqrtV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::rsqrt(x);
  zivc::vstore16(y, index, out);
}

__kernel void rsqrtV16PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                          zivc::GlobalPtr<float> out,
                                          const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::precision::rsqrt(x);
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
  const float y = zivc::cbrt(x);
  out[index] = y;
}

__kernel void cbrtV1PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::precision::cbrt(x);
  out[index] = y;
}

__kernel void cbrtV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::cbrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void cbrtV2PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::precision::cbrt(x);
  zivc::vstore2(y, index, out);
}

__kernel void cbrtV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::cbrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void cbrtV3PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::precision::cbrt(x);
  zivc::vstore3(y, index, out);
}

__kernel void cbrtV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::cbrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void cbrtV4PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::precision::cbrt(x);
  zivc::vstore4(y, index, out);
}

__kernel void cbrtV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::cbrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void cbrtV8PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                        zivc::GlobalPtr<float> out,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::precision::cbrt(x);
  zivc::vstore8(y, index, out);
}

__kernel void cbrtV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::cbrt(x);
  zivc::vstore16(y, index, out);
}

__kernel void cbrtV16PrecisionTestKernel(zivc::ConstGlobalPtr<float> in,
                                         zivc::GlobalPtr<float> out,
                                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::precision::cbrt(x);
  zivc::vstore16(y, index, out);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH_POWER_CL */
