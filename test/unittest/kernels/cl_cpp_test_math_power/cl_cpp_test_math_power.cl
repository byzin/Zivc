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
  const float y = zivc::exp(x);
  out[index] = y;
}

__kernel void expV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::exp(x);
  zivc::vstore2(y, index, out);
}

__kernel void expV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::exp(x);
  zivc::vstore3(y, index, out);
}

__kernel void expV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::exp(x);
  zivc::vstore4(y, index, out);
}

__kernel void expV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::exp(x);
  zivc::vstore8(y, index, out);
}

__kernel void expV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::exp(x);
  zivc::vstore16(y, index, out);
}

__kernel void exp2V1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::exp2(x);
  out[index] = y;
}

__kernel void exp2V2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::exp2(x);
  zivc::vstore2(y, index, out);
}

__kernel void exp2V3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::exp2(x);
  zivc::vstore3(y, index, out);
}

__kernel void exp2V4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::exp2(x);
  zivc::vstore4(y, index, out);
}

__kernel void exp2V8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::exp2(x);
  zivc::vstore8(y, index, out);
}

__kernel void exp2V16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::exp2(x);
  zivc::vstore16(y, index, out);
}

__kernel void expm1V1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::expm1(x);
  out[index] = y;
}

__kernel void expm1V2TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::expm1(x);
  zivc::vstore2(y, index, out);
}

__kernel void expm1V3TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::expm1(x);
  zivc::vstore3(y, index, out);
}

__kernel void expm1V4TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::expm1(x);
  zivc::vstore4(y, index, out);
}

__kernel void expm1V8TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::expm1(x);
  zivc::vstore8(y, index, out);
}

__kernel void expm1V16TestKernel(zivc::ConstGlobalPtr<float> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::expm1(x);
  zivc::vstore16(y, index, out);
}

__kernel void logV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::log(x);
  out[index] = y;
}

__kernel void logV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::log(x);
  zivc::vstore2(y, index, out);
}

__kernel void logV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::log(x);
  zivc::vstore3(y, index, out);
}

__kernel void logV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::log(x);
  zivc::vstore4(y, index, out);
}

__kernel void logV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::log(x);
  zivc::vstore8(y, index, out);
}

__kernel void logV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::log(x);
  zivc::vstore16(y, index, out);
}

__kernel void log2V1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::log2(x);
  out[index] = y;
}

__kernel void log2V2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::log2(x);
  zivc::vstore2(y, index, out);
}

__kernel void log2V3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::log2(x);
  zivc::vstore3(y, index, out);
}

__kernel void log2V4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::log2(x);
  zivc::vstore4(y, index, out);
}

__kernel void log2V8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::log2(x);
  zivc::vstore8(y, index, out);
}

__kernel void log2V16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::log2(x);
  zivc::vstore16(y, index, out);
}

__kernel void log10V1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::log10(x);
  out[index] = y;
}

__kernel void log10V2TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::log10(x);
  zivc::vstore2(y, index, out);
}

__kernel void log10V3TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::log10(x);
  zivc::vstore3(y, index, out);
}

__kernel void log10V4TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::log10(x);
  zivc::vstore4(y, index, out);
}

__kernel void log10V8TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::log10(x);
  zivc::vstore8(y, index, out);
}

__kernel void log10V16TestKernel(zivc::ConstGlobalPtr<float> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::log10(x);
  zivc::vstore16(y, index, out);
}

__kernel void log1pV1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::log1p(x);
  out[index] = y;
}

__kernel void log1pV2TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::log1p(x);
  zivc::vstore2(y, index, out);
}

__kernel void log1pV3TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::log1p(x);
  zivc::vstore3(y, index, out);
}

__kernel void log1pV4TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::log1p(x);
  zivc::vstore4(y, index, out);
}

__kernel void log1pV8TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::log1p(x);
  zivc::vstore8(y, index, out);
}

__kernel void log1pV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::log1p(x);
  zivc::vstore16(y, index, out);
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

__kernel void hypotV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::hypot(x, y);
  out[index] = z;
}

__kernel void hypotV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::hypot(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void hypotV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::hypot(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void hypotV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::hypot(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void hypotV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::hypot(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void hypotV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                 zivc::ConstGlobalPtr<float> in2,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::hypot(x, y);
  zivc::vstore16(z, index, out);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH_POWER_CL */
