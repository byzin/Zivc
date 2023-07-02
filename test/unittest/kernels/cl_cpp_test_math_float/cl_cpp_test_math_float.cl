/*!
  \file opencl_cpp_test_math_float.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_MATH_FLOAT_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_MATH_FLOAT_CL

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

__kernel void fabsV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::fabs(x);
  out[index] = y;
}

__kernel void fabsV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::fabs(x);
  zivc::vstore2(y, index, out);
}

__kernel void fabsV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::fabs(x);
  zivc::vstore3(y, index, out);
}

__kernel void fabsV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::fabs(x);
  zivc::vstore4(y, index, out);
}

__kernel void fabsV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::fabs(x);
  zivc::vstore8(y, index, out);
}

__kernel void fabsV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::fabs(x);
  zivc::vstore16(y, index, out);
}

__kernel void fmodV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::fmod(x, y);
  out[index] = z;
}

__kernel void fmodV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::fmod(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void fmodV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::fmod(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void fmodV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::fmod(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void fmodV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::fmod(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void fmodV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::fmod(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void remainderV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::remainder(x, y);
  out[index] = z;
}

__kernel void remainderV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::remainder(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void remainderV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::remainder(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void remainderV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::remainder(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void remainderV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::remainder(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void remainderV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                     zivc::ConstGlobalPtr<float> in2,
                                     zivc::GlobalPtr<float> out,
                                     const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::remainder(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void fmaV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float> in2,
                              zivc::ConstGlobalPtr<float> in3,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = in3[index];
  const float w = zivc::fma(x, y, z);
  out[index] = w;
}

__kernel void fmaV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float> in2,
                              zivc::ConstGlobalPtr<float> in3,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::vload2(index, in3);
  const float2 w = zivc::fma(x, y, z);
  zivc::vstore2(w, index, out);
}

__kernel void fmaV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float> in2,
                              zivc::ConstGlobalPtr<float> in3,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::vload3(index, in3);
  const float3 w = zivc::fma(x, y, z);
  zivc::vstore3(w, index, out);
}

__kernel void fmaV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float> in2,
                              zivc::ConstGlobalPtr<float> in3,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::vload4(index, in3);
  const float4 w = zivc::fma(x, y, z);
  zivc::vstore4(w, index, out);
}

__kernel void fmaV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float> in2,
                              zivc::ConstGlobalPtr<float> in3,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::vload8(index, in3);
  const float8 w = zivc::fma(x, y, z);
  zivc::vstore8(w, index, out);
}

__kernel void fmaV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::ConstGlobalPtr<float> in3,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::vload16(index, in3);
  const float16 w = zivc::fma(x, y, z);
  zivc::vstore16(w, index, out);
}

__kernel void fmaxV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::fmax(x, y);
  out[index] = z;
}

__kernel void fmaxV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::fmax(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void fmaxV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::fmax(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void fmaxV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::fmax(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void fmaxV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::fmax(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void fmaxV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::fmax(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void fminV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::fmin(x, y);
  out[index] = z;
}

__kernel void fminV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::fmin(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void fminV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::fmin(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void fminV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::fmin(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void fminV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::fmin(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void fminV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::fmin(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void maxmagV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::maxmag(x, y);
  out[index] = z;
}

__kernel void maxmagV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::maxmag(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void maxmagV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::maxmag(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void maxmagV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::maxmag(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void maxmagV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::maxmag(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void maxmagV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::maxmag(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void minmagV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::minmag(x, y);
  out[index] = z;
}

__kernel void minmagV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::minmag(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void minmagV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::minmag(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void minmagV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::minmag(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void minmagV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::minmag(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void minmagV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::minmag(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void fdimV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::fdim(x, y);
  out[index] = z;
}

__kernel void fdimV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::fdim(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void fdimV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::fdim(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void fdimV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::fdim(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void fdimV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float> in2,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::fdim(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void fdimV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::fdim(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void ceilV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::ceil(x);
  out[index] = y;
}

__kernel void ceilV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::ceil(x);
  zivc::vstore2(y, index, out);
}

__kernel void ceilV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::ceil(x);
  zivc::vstore3(y, index, out);
}

__kernel void ceilV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::ceil(x);
  zivc::vstore4(y, index, out);
}

__kernel void ceilV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::ceil(x);
  zivc::vstore8(y, index, out);
}

__kernel void ceilV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::ceil(x);
  zivc::vstore16(y, index, out);
}

__kernel void floorV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::floor(x);
  out[index] = y;
}

__kernel void floorV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::floor(x);
  zivc::vstore2(y, index, out);
}

__kernel void floorV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::floor(x);
  zivc::vstore3(y, index, out);
}

__kernel void floorV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::floor(x);
  zivc::vstore4(y, index, out);
}

__kernel void floorV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::floor(x);
  zivc::vstore8(y, index, out);
}

__kernel void floorV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::floor(x);
  zivc::vstore16(y, index, out);
}

__kernel void truncV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::trunc(x);
  out[index] = y;
}

__kernel void truncV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::trunc(x);
  zivc::vstore2(y, index, out);
}

__kernel void truncV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::trunc(x);
  zivc::vstore3(y, index, out);
}

__kernel void truncV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::trunc(x);
  zivc::vstore4(y, index, out);
}

__kernel void truncV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::trunc(x);
  zivc::vstore8(y, index, out);
}

__kernel void truncV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::trunc(x);
  zivc::vstore16(y, index, out);
}

__kernel void roundV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::round(x);
  out[index] = y;
}

__kernel void roundV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::round(x);
  zivc::vstore2(y, index, out);
}

__kernel void roundV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::round(x);
  zivc::vstore3(y, index, out);
}

__kernel void roundV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::round(x);
  zivc::vstore4(y, index, out);
}

__kernel void roundV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::round(x);
  zivc::vstore8(y, index, out);
}

__kernel void roundV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::round(x);
  zivc::vstore16(y, index, out);
}

__kernel void rintV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::rint(x);
  out[index] = y;
}

__kernel void rintV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::rint(x);
  zivc::vstore2(y, index, out);
}

__kernel void rintV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::rint(x);
  zivc::vstore3(y, index, out);
}

__kernel void rintV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::rint(x);
  zivc::vstore4(y, index, out);
}

__kernel void rintV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::rint(x);
  zivc::vstore8(y, index, out);
}

__kernel void rintV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::rint(x);
  zivc::vstore16(y, index, out);
}

//__kernel void frLdexpV1TestKernel(zivc::ConstGlobalPtr<float> in,
//                                  zivc::GlobalPtr<float> out1,
//                                  zivc::GlobalPtr<int32b> out2,
//                                  zivc::GlobalPtr<float> out3,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float x = in[index];
//  out1[index] = zivc::frexp(x, out2 + index);
//  int32b iexp = 0;
//  const float y = zivc::frexp(x, &iexp);
//  const float z = zivc::ldexp(y, iexp);
//  out3[index] = z;
//}
//
//__kernel void frLdexpV2TestKernel(zivc::ConstGlobalPtr<float> in,
//                                  zivc::GlobalPtr<float> out1,
//                                  zivc::GlobalPtr<int32b> out2,
//                                  zivc::GlobalPtr<float> out3,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float2 x = zivc::vload2(index, in);
//  int2 iexp = zivc::makeInt2(0);
//  const float2 y = zivc::frexp(x, &iexp);
//  zivc::vstore2(y, index, out1);
//  zivc::vstore2(iexp, index, out2);
//  const float2 z = zivc::ldexp(y, iexp);
//  zivc::vstore2(z, index, out3);
//}
//
//__kernel void frLdexpV3TestKernel(zivc::ConstGlobalPtr<float> in,
//                                  zivc::GlobalPtr<float> out1,
//                                  zivc::GlobalPtr<int32b> out2,
//                                  zivc::GlobalPtr<float> out3,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float3 x = zivc::vload3(index, in);
//  int3 iexp = zivc::makeInt3(0);
//  const float3 y = zivc::frexp(x, &iexp);
//  zivc::vstore3(y, index, out1);
//  zivc::vstore3(iexp, index, out2);
//  const float3 z = zivc::ldexp(y, iexp);
//  zivc::vstore3(z, index, out3);
//}
//
//__kernel void frLdexpV4TestKernel(zivc::ConstGlobalPtr<float> in,
//                                  zivc::GlobalPtr<float> out1,
//                                  zivc::GlobalPtr<int32b> out2,
//                                  zivc::GlobalPtr<float> out3,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float4 x = zivc::vload4(index, in);
//  int4 iexp = zivc::makeInt4(0);
//  const float4 y = zivc::frexp(x, &iexp);
//  zivc::vstore4(y, index, out1);
//  zivc::vstore4(iexp, index, out2);
//  const float4 z = zivc::ldexp(y, iexp);
//  zivc::vstore4(z, index, out3);
//}
//
//__kernel void frLdexpV8TestKernel(zivc::ConstGlobalPtr<float> in,
//                                  zivc::GlobalPtr<float> out1,
//                                  zivc::GlobalPtr<int32b> out2,
//                                  zivc::GlobalPtr<float> out3,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float8 x = zivc::vload8(index, in);
//  int8 iexp = zivc::makeInt8(0);
//  const float8 y = zivc::frexp(x, &iexp);
//  zivc::vstore8(y, index, out1);
//  zivc::vstore8(iexp, index, out2);
//  const float8 z = zivc::ldexp(y, iexp);
//  zivc::vstore8(z, index, out3);
//}
//
//__kernel void frLdexpV16TestKernel(zivc::ConstGlobalPtr<float> in,
//                                   zivc::GlobalPtr<float> out1,
//                                   zivc::GlobalPtr<int32b> out2,
//                                   zivc::GlobalPtr<float> out3,
//                                   const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  const float16 x = zivc::vload16(index, in);
//  int16 iexp = zivc::makeInt16(0);
//  const float16 y = zivc::frexp(x, &iexp);
//  zivc::vstore16(y, index, out1);
//  zivc::vstore16(iexp, index, out2);
//  const float16 z = zivc::ldexp(y, iexp);
//  zivc::vstore16(z, index, out3);
//}

__kernel void ilogbV1TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const int32b y = zivc::ilogb(x);
  out[index] = zivc::cast<float>(y);
}

__kernel void ilogbV2TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const int2 y = zivc::ilogb(x);
  zivc::vstore2(zivc::cast<float2>(y), index, out);
}

__kernel void ilogbV3TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const int3 y = zivc::ilogb(x);
  zivc::vstore3(zivc::cast<float3>(y), index, out);
}

__kernel void ilogbV4TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const int4 y = zivc::ilogb(x);
  zivc::vstore4(zivc::cast<float4>(y), index, out);
}

__kernel void ilogbV8TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const int8 y = zivc::ilogb(x);
  zivc::vstore8(zivc::cast<float8>(y), index, out);
}

__kernel void ilogbV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const int16 y = zivc::ilogb(x);
  zivc::vstore16(zivc::cast<float16>(y), index, out);
}

__kernel void logbV1TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::logb(x);
  out[index] = y;
}

__kernel void logbV2TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::logb(x);
  zivc::vstore2(y, index, out);
}

__kernel void logbV3TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::logb(x);
  zivc::vstore3(y, index, out);
}

__kernel void logbV4TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::logb(x);
  zivc::vstore4(y, index, out);
}

__kernel void logbV8TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::logb(x);
  zivc::vstore8(y, index, out);
}

__kernel void logbV16TestKernel(zivc::ConstGlobalPtr<float> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::logb(x);
  zivc::vstore16(y, index, out);
}

__kernel void nextafterV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::nextafter(x, y);
  out[index] = z;
}

__kernel void nextafterV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::nextafter(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void nextafterV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::nextafter(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void nextafterV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::nextafter(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void nextafterV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::nextafter(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void nextafterV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                     zivc::ConstGlobalPtr<float> in2,
                                     zivc::GlobalPtr<float> out,
                                     const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::nextafter(x, y);
  zivc::vstore16(z, index, out);
}

__kernel void copysignV1TestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float> in2,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in1[index];
  const float y = in2[index];
  const float z = zivc::copysign(x, y);
  out[index] = z;
}

__kernel void copysignV2TestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float> in2,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in1);
  const float2 y = zivc::vload2(index, in2);
  const float2 z = zivc::copysign(x, y);
  zivc::vstore2(z, index, out);
}

__kernel void copysignV3TestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float> in2,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in1);
  const float3 y = zivc::vload3(index, in2);
  const float3 z = zivc::copysign(x, y);
  zivc::vstore3(z, index, out);
}

__kernel void copysignV4TestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float> in2,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in1);
  const float4 y = zivc::vload4(index, in2);
  const float4 z = zivc::copysign(x, y);
  zivc::vstore4(z, index, out);
}

__kernel void copysignV8TestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float> in2,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in1);
  const float8 y = zivc::vload8(index, in2);
  const float8 z = zivc::copysign(x, y);
  zivc::vstore8(z, index, out);
}

__kernel void copysignV16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float> in2,
                                    zivc::GlobalPtr<float> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in1);
  const float16 y = zivc::vload16(index, in2);
  const float16 z = zivc::copysign(x, y);
  zivc::vstore16(z, index, out);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH_FLOAT_CL */
