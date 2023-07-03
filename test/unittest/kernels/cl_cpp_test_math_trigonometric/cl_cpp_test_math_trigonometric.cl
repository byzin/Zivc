/*!
  \file opencl_cpp_test_math_trigonometric.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_MATH_TRIGONOMETRIC_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_MATH_TRIGONOMETRIC_CL

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

__kernel void sinV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::sin(x);
  out[index] = y;
}

__kernel void sinV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::sin(x);
  zivc::vstore2(y, index, out);
}

__kernel void sinV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::sin(x);
  zivc::vstore3(y, index, out);
}

__kernel void sinV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::sin(x);
  zivc::vstore4(y, index, out);
}

__kernel void sinV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::sin(x);
  zivc::vstore8(y, index, out);
}

__kernel void sinV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::sin(x);
  zivc::vstore16(y, index, out);
}

__kernel void cosV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::cos(x);
  out[index] = y;
}

__kernel void cosV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::cos(x);
  zivc::vstore2(y, index, out);
}

__kernel void cosV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::cos(x);
  zivc::vstore3(y, index, out);
}

__kernel void cosV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::cos(x);
  zivc::vstore4(y, index, out);
}

__kernel void cosV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::cos(x);
  zivc::vstore8(y, index, out);
}

__kernel void cosV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::cos(x);
  zivc::vstore16(y, index, out);
}

__kernel void tanV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::tan(x);
  out[index] = y;
}

__kernel void tanV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::tan(x);
  zivc::vstore2(y, index, out);
}

__kernel void tanV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::tan(x);
  zivc::vstore3(y, index, out);
}

__kernel void tanV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::tan(x);
  zivc::vstore4(y, index, out);
}

__kernel void tanV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::tan(x);
  zivc::vstore8(y, index, out);
}

__kernel void tanV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::tan(x);
  zivc::vstore16(y, index, out);
}

__kernel void asinV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::asin(x);
  out[index] = y;
}

__kernel void asinV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::asin(x);
  zivc::vstore2(y, index, out);
}

__kernel void asinV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::asin(x);
  zivc::vstore3(y, index, out);
}

__kernel void asinV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::asin(x);
  zivc::vstore4(y, index, out);
}

__kernel void asinV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::asin(x);
  zivc::vstore8(y, index, out);
}

__kernel void asinV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::asin(x);
  zivc::vstore16(y, index, out);
}

__kernel void acosV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::acos(x);
  out[index] = y;
}

__kernel void acosV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::acos(x);
  zivc::vstore2(y, index, out);
}

__kernel void acosV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::acos(x);
  zivc::vstore3(y, index, out);
}

__kernel void acosV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::acos(x);
  zivc::vstore4(y, index, out);
}

__kernel void acosV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::acos(x);
  zivc::vstore8(y, index, out);
}

__kernel void acosV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::acos(x);
  zivc::vstore16(y, index, out);
}

__kernel void atanV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::atan(x);
  out[index] = y;
}

__kernel void atanV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::atan(x);
  zivc::vstore2(y, index, out);
}

__kernel void atanV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::atan(x);
  zivc::vstore3(y, index, out);
}

__kernel void atanV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::atan(x);
  zivc::vstore4(y, index, out);
}

__kernel void atanV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::atan(x);
  zivc::vstore8(y, index, out);
}

__kernel void atanV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::atan(x);
  zivc::vstore16(y, index, out);
}

__kernel void atan2V1TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float y = in1[index];
  const float x = in2[index];
  const float z = zivc::atan2(y, x);
  out[index] = z;
}

__kernel void atan2V2TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 y = zivc::vload2(index, in1);
  const float2 x = zivc::vload2(index, in2);
  const float2 z = zivc::atan2(y, x);
  zivc::vstore2(z, index, out);
}

__kernel void atan2V3TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 y = zivc::vload3(index, in1);
  const float3 x = zivc::vload3(index, in2);
  const float3 z = zivc::atan2(y, x);
  zivc::vstore3(z, index, out);
}

__kernel void atan2V4TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 y = zivc::vload4(index, in1);
  const float4 x = zivc::vload4(index, in2);
  const float4 z = zivc::atan2(y, x);
  zivc::vstore4(z, index, out);
}

__kernel void atan2V8TestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float> in2,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 y = zivc::vload8(index, in1);
  const float8 x = zivc::vload8(index, in2);
  const float8 z = zivc::atan2(y, x);
  zivc::vstore8(z, index, out);
}

__kernel void atan2V16TestKernel(zivc::ConstGlobalPtr<float> in1,
                                 zivc::ConstGlobalPtr<float> in2,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 y = zivc::vload16(index, in1);
  const float16 x = zivc::vload16(index, in2);
  const float16 z = zivc::atan2(y, x);
  zivc::vstore16(z, index, out);
}

__kernel void sinhV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::sinh(x);
  out[index] = y;
}

__kernel void sinhV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::sinh(x);
  zivc::vstore2(y, index, out);
}

__kernel void sinhV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::sinh(x);
  zivc::vstore3(y, index, out);
}

__kernel void sinhV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::sinh(x);
  zivc::vstore4(y, index, out);
}

__kernel void sinhV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::sinh(x);
  zivc::vstore8(y, index, out);
}

__kernel void sinhV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::sinh(x);
  zivc::vstore16(y, index, out);
}

__kernel void coshV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::cosh(x);
  out[index] = y;
}

__kernel void coshV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::cosh(x);
  zivc::vstore2(y, index, out);
}

__kernel void coshV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::cosh(x);
  zivc::vstore3(y, index, out);
}

__kernel void coshV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::cosh(x);
  zivc::vstore4(y, index, out);
}

__kernel void coshV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::cosh(x);
  zivc::vstore8(y, index, out);
}

__kernel void coshV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::cosh(x);
  zivc::vstore16(y, index, out);
}

__kernel void tanhV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::tanh(x);
  out[index] = y;
}

__kernel void tanhV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::tanh(x);
  zivc::vstore2(y, index, out);
}

__kernel void tanhV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::tanh(x);
  zivc::vstore3(y, index, out);
}

__kernel void tanhV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::tanh(x);
  zivc::vstore4(y, index, out);
}

__kernel void tanhV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::tanh(x);
  zivc::vstore8(y, index, out);
}

__kernel void tanhV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::tanh(x);
  zivc::vstore16(y, index, out);
}

__kernel void asinhV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::asinh(x);
  out[index] = y;
}

__kernel void asinhV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::asinh(x);
  zivc::vstore2(y, index, out);
}

__kernel void asinhV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::asinh(x);
  zivc::vstore3(y, index, out);
}

__kernel void asinhV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::asinh(x);
  zivc::vstore4(y, index, out);
}

__kernel void asinhV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::asinh(x);
  zivc::vstore8(y, index, out);
}

__kernel void asinhV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::asinh(x);
  zivc::vstore16(y, index, out);
}

__kernel void acoshV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::acosh(x);
  out[index] = y;
}

__kernel void acoshV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::acosh(x);
  zivc::vstore2(y, index, out);
}

__kernel void acoshV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::acosh(x);
  zivc::vstore3(y, index, out);
}

__kernel void acoshV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::acosh(x);
  zivc::vstore4(y, index, out);
}

__kernel void acoshV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::acosh(x);
  zivc::vstore8(y, index, out);
}

__kernel void acoshV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::acosh(x);
  zivc::vstore16(y, index, out);
}

__kernel void atanhV1TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float x = in[index];
  const float y = zivc::atanh(x);
  out[index] = y;
}

__kernel void atanhV2TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float2 x = zivc::vload2(index, in);
  const float2 y = zivc::atanh(x);
  zivc::vstore2(y, index, out);
}

__kernel void atanhV3TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float3 x = zivc::vload3(index, in);
  const float3 y = zivc::atanh(x);
  zivc::vstore3(y, index, out);
}

__kernel void atanhV4TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float4 x = zivc::vload4(index, in);
  const float4 y = zivc::atanh(x);
  zivc::vstore4(y, index, out);
}

__kernel void atanhV8TestKernel(zivc::ConstGlobalPtr<float> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float8 x = zivc::vload8(index, in);
  const float8 y = zivc::atanh(x);
  zivc::vstore8(y, index, out);
}

__kernel void atanhV16TestKernel(zivc::ConstGlobalPtr<float> in,
                               zivc::GlobalPtr<float> out,
                               const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const float16 x = zivc::vload16(index, in);
  const float16 y = zivc::atanh(x);
  zivc::vstore16(y, index, out);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_MATH_TRIGONOMETRIC_CL */
