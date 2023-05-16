/*!
  \file opencl_cpp_test_relation.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_RELATION
#define ZIVC_TEST_OPENCL_CPP_TEST_RELATION

// Zivc
#include "zivc/cl/relational.hpp"
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

__kernel void isEqualTestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float2> in2,
                                zivc::ConstGlobalPtr<float3> in3,
                                zivc::ConstGlobalPtr<float4> in4,
                                zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isequal(v0, v0);
    out1[1] = zivc::isequal(v1, v1);
    out1[2] = zivc::isequal(v0, v1);
    out1[3] = zivc::isequal(v2, v2);
    out1[4] = zivc::isequal(v3, v3);
    out1[5] = zivc::isequal(v4, v4);
    out1[6] = zivc::isequal(v4, v3);
    out1[7] = zivc::isequal(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::isequal(v6, v6);
    out1[9] = zivc::isequal(v7, v7);
    out1[10] = zivc::isequal(v5, v6);
    out1[11] = zivc::isequal(v6, v7);

    out1[12] = zivc::isequal(v8, v8);
    out1[13] = zivc::isequal(v3, v8);
    out1[14] = zivc::isequal(v9, v9);
    out1[15] = zivc::isequal(v9, v4);
    out1[16] = zivc::isequal(v8, v9);
    out1[17] = zivc::isequal(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isequal(v0, v0);
    out2[1] = zivc::isequal(v1, v1);
    out2[2] = zivc::isequal(v0, v1);
    out2[3] = zivc::isequal(v2, v2);
    out2[4] = zivc::isequal(v3, v3);
    out2[5] = zivc::isequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::isequal(v4, v4);
    out2[7] = zivc::isequal(v3, v4);

    out2[8] = zivc::isequal(v5, v5);
    out2[9] = zivc::isequal(v5, v2);
    out2[10] = zivc::isequal(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isequal(v0, v0);
    out3[1] = zivc::isequal(v1, v1);
    out3[2] = zivc::isequal(v0, v1);
    out3[3] = zivc::isequal(v2, v2);
    out3[4] = zivc::isequal(v3, v3);
    out3[5] = zivc::isequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::isequal(v4, v4);
    out3[7] = zivc::isequal(v3, v4);

    out3[8] = zivc::isequal(v5, v5);
    out3[9] = zivc::isequal(v5, v2);
    out3[10] = zivc::isequal(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isequal(v0, v0);
    out4[1] = zivc::isequal(v1, v1);
    out4[2] = zivc::isequal(v0, v1);
    out4[3] = zivc::isequal(v2, v2);
    out4[4] = zivc::isequal(v3, v3);
    out4[5] = zivc::isequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::isequal(v4, v4);
    out4[7] = zivc::isequal(v3, v4);

    out4[8] = zivc::isequal(v5, v5);
    out4[9] = zivc::isequal(v5, v2);
    out4[10] = zivc::isequal(v6, v6);
  }
}

__kernel void isEqualLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                       zivc::ConstGlobalPtr<float16> in16,
                                       zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                       zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isequal(v0, v0);
    out8[1] = zivc::isequal(v1, v1);
    out8[2] = zivc::isequal(v0, v1);
    out8[3] = zivc::isequal(v2, v2);
    out8[4] = zivc::isequal(v3, v3);
    out8[5] = zivc::isequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::isequal(v4, v4);
    out8[7] = zivc::isequal(v3, v4);

    out8[8] = zivc::isequal(v5, v5);
    out8[9] = zivc::isequal(v5, v2);
    out8[10] = zivc::isequal(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isequal(v0, v0);
    out16[1] = zivc::isequal(v1, v1);
    out16[2] = zivc::isequal(v0, v1);
    out16[3] = zivc::isequal(v2, v2);
    out16[4] = zivc::isequal(v3, v3);
    out16[5] = zivc::isequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::isequal(v4, v4);
    out16[7] = zivc::isequal(v3, v4);

    out16[8] = zivc::isequal(v5, v5);
    out16[9] = zivc::isequal(v5, v2);
    out16[10] = zivc::isequal(v6, v6);
  }
}

__kernel void isNotEqualTestKernel(zivc::ConstGlobalPtr<float> in1,
                                   zivc::ConstGlobalPtr<float2> in2,
                                   zivc::ConstGlobalPtr<float3> in3,
                                   zivc::ConstGlobalPtr<float4> in4,
                                   zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                   zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                   zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                   zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isnotequal(v0, v0);
    out1[1] = zivc::isnotequal(v1, v1);
    out1[2] = zivc::isnotequal(v0, v1);
    out1[3] = zivc::isnotequal(v2, v2);
    out1[4] = zivc::isnotequal(v3, v3);
    out1[5] = zivc::isnotequal(v4, v4);
    out1[6] = zivc::isnotequal(v4, v3);
    out1[7] = zivc::isnotequal(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::isnotequal(v6, v6);
    out1[9] = zivc::isnotequal(v7, v7);
    out1[10] = zivc::isnotequal(v5, v6);
    out1[11] = zivc::isnotequal(v6, v7);

    out1[12] = zivc::isnotequal(v8, v8);
    out1[13] = zivc::isnotequal(v3, v8);
    out1[14] = zivc::isnotequal(v9, v9);
    out1[15] = zivc::isnotequal(v9, v4);
    out1[16] = zivc::isnotequal(v8, v9);
    out1[17] = zivc::isnotequal(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isnotequal(v0, v0);
    out2[1] = zivc::isnotequal(v1, v1);
    out2[2] = zivc::isnotequal(v0, v1);
    out2[3] = zivc::isnotequal(v2, v2);
    out2[4] = zivc::isnotequal(v3, v3);
    out2[5] = zivc::isnotequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::isnotequal(v4, v4);
    out2[7] = zivc::isnotequal(v3, v4);

    out2[8] = zivc::isnotequal(v5, v5);
    out2[9] = zivc::isnotequal(v5, v2);
    out2[10] = zivc::isnotequal(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isnotequal(v0, v0);
    out3[1] = zivc::isnotequal(v1, v1);
    out3[2] = zivc::isnotequal(v0, v1);
    out3[3] = zivc::isnotequal(v2, v2);
    out3[4] = zivc::isnotequal(v3, v3);
    out3[5] = zivc::isnotequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::isnotequal(v4, v4);
    out3[7] = zivc::isnotequal(v3, v4);

    out3[8] = zivc::isnotequal(v5, v5);
    out3[9] = zivc::isnotequal(v5, v2);
    out3[10] = zivc::isnotequal(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isnotequal(v0, v0);
    out4[1] = zivc::isnotequal(v1, v1);
    out4[2] = zivc::isnotequal(v0, v1);
    out4[3] = zivc::isnotequal(v2, v2);
    out4[4] = zivc::isnotequal(v3, v3);
    out4[5] = zivc::isnotequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::isnotequal(v4, v4);
    out4[7] = zivc::isnotequal(v3, v4);

    out4[8] = zivc::isnotequal(v5, v5);
    out4[9] = zivc::isnotequal(v5, v2);
    out4[10] = zivc::isnotequal(v6, v6);
  }
}

__kernel void isNotEqualLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                          zivc::ConstGlobalPtr<float16> in16,
                                          zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                          zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isnotequal(v0, v0);
    out8[1] = zivc::isnotequal(v1, v1);
    out8[2] = zivc::isnotequal(v0, v1);
    out8[3] = zivc::isnotequal(v2, v2);
    out8[4] = zivc::isnotequal(v3, v3);
    out8[5] = zivc::isnotequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::isnotequal(v4, v4);
    out8[7] = zivc::isnotequal(v3, v4);

    out8[8] = zivc::isnotequal(v5, v5);
    out8[9] = zivc::isnotequal(v5, v2);
    out8[10] = zivc::isnotequal(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isnotequal(v0, v0);
    out16[1] = zivc::isnotequal(v1, v1);
    out16[2] = zivc::isnotequal(v0, v1);
    out16[3] = zivc::isnotequal(v2, v2);
    out16[4] = zivc::isnotequal(v3, v3);
    out16[5] = zivc::isnotequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::isnotequal(v4, v4);
    out16[7] = zivc::isnotequal(v3, v4);

    out16[8] = zivc::isnotequal(v5, v5);
    out16[9] = zivc::isnotequal(v5, v2);
    out16[10] = zivc::isnotequal(v6, v6);
  }
}

__kernel void isGreaterTestKernel(zivc::ConstGlobalPtr<float> in1,
                                  zivc::ConstGlobalPtr<float2> in2,
                                  zivc::ConstGlobalPtr<float3> in3,
                                  zivc::ConstGlobalPtr<float4> in4,
                                  zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                  zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                  zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                  zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isgreater(v0, v0);
    out1[1] = zivc::isgreater(v1, v1);
    out1[2] = zivc::isgreater(v0, v1);
    out1[3] = zivc::isgreater(v2, v2);
    out1[4] = zivc::isgreater(v3, v3);
    out1[5] = zivc::isgreater(v4, v4);
    out1[6] = zivc::isgreater(v4, v3);
    out1[7] = zivc::isgreater(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::isgreater(v6, v6);
    out1[9] = zivc::isgreater(v7, v7);
    out1[10] = zivc::isgreater(v5, v6);
    out1[11] = zivc::isgreater(v6, v7);

    out1[12] = zivc::isgreater(v8, v8);
    out1[13] = zivc::isgreater(v3, v8);
    out1[14] = zivc::isgreater(v9, v9);
    out1[15] = zivc::isgreater(v9, v4);
    out1[16] = zivc::isgreater(v8, v9);
    out1[17] = zivc::isgreater(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isgreater(v0, v0);
    out2[1] = zivc::isgreater(v1, v1);
    out2[2] = zivc::isgreater(v0, v1);
    out2[3] = zivc::isgreater(v2, v2);
    out2[4] = zivc::isgreater(v3, v3);
    out2[5] = zivc::isgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::isgreater(v4, v4);
    out2[7] = zivc::isgreater(v3, v4);

    out2[8] = zivc::isgreater(v5, v5);
    out2[9] = zivc::isgreater(v5, v2);
    out2[10] = zivc::isgreater(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isgreater(v0, v0);
    out3[1] = zivc::isgreater(v1, v1);
    out3[2] = zivc::isgreater(v0, v1);
    out3[3] = zivc::isgreater(v2, v2);
    out3[4] = zivc::isgreater(v3, v3);
    out3[5] = zivc::isgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::isgreater(v4, v4);
    out3[7] = zivc::isgreater(v3, v4);

    out3[8] = zivc::isgreater(v5, v5);
    out3[9] = zivc::isgreater(v5, v2);
    out3[10] = zivc::isgreater(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isgreater(v0, v0);
    out4[1] = zivc::isgreater(v1, v1);
    out4[2] = zivc::isgreater(v0, v1);
    out4[3] = zivc::isgreater(v2, v2);
    out4[4] = zivc::isgreater(v3, v3);
    out4[5] = zivc::isgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::isgreater(v4, v4);
    out4[7] = zivc::isgreater(v3, v4);

    out4[8] = zivc::isgreater(v5, v5);
    out4[9] = zivc::isgreater(v5, v2);
    out4[10] = zivc::isgreater(v6, v6);
  }
}

__kernel void isGreaterLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                         zivc::ConstGlobalPtr<float16> in16,
                                         zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                         zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isgreater(v0, v0);
    out8[1] = zivc::isgreater(v1, v1);
    out8[2] = zivc::isgreater(v0, v1);
    out8[3] = zivc::isgreater(v2, v2);
    out8[4] = zivc::isgreater(v3, v3);
    out8[5] = zivc::isgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::isgreater(v4, v4);
    out8[7] = zivc::isgreater(v3, v4);

    out8[8] = zivc::isgreater(v5, v5);
    out8[9] = zivc::isgreater(v5, v2);
    out8[10] = zivc::isgreater(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isgreater(v0, v0);
    out16[1] = zivc::isgreater(v1, v1);
    out16[2] = zivc::isgreater(v0, v1);
    out16[3] = zivc::isgreater(v2, v2);
    out16[4] = zivc::isgreater(v3, v3);
    out16[5] = zivc::isgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::isgreater(v4, v4);
    out16[7] = zivc::isgreater(v3, v4);

    out16[8] = zivc::isgreater(v5, v5);
    out16[9] = zivc::isgreater(v5, v2);
    out16[10] = zivc::isgreater(v6, v6);
  }
}

__kernel void isGreaterEqualTestKernel(zivc::ConstGlobalPtr<float> in1,
                                       zivc::ConstGlobalPtr<float2> in2,
                                       zivc::ConstGlobalPtr<float3> in3,
                                       zivc::ConstGlobalPtr<float4> in4,
                                       zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                       zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                       zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                       zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isgreaterequal(v0, v0);
    out1[1] = zivc::isgreaterequal(v1, v1);
    out1[2] = zivc::isgreaterequal(v0, v1);
    out1[3] = zivc::isgreaterequal(v2, v2);
    out1[4] = zivc::isgreaterequal(v3, v3);
    out1[5] = zivc::isgreaterequal(v4, v4);
    out1[6] = zivc::isgreaterequal(v4, v3);
    out1[7] = zivc::isgreaterequal(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::isgreaterequal(v6, v6);
    out1[9] = zivc::isgreaterequal(v7, v7);
    out1[10] = zivc::isgreaterequal(v5, v6);
    out1[11] = zivc::isgreaterequal(v6, v7);

    out1[12] = zivc::isgreaterequal(v8, v8);
    out1[13] = zivc::isgreaterequal(v3, v8);
    out1[14] = zivc::isgreaterequal(v9, v9);
    out1[15] = zivc::isgreaterequal(v9, v4);
    out1[16] = zivc::isgreaterequal(v8, v9);
    out1[17] = zivc::isgreaterequal(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isgreaterequal(v0, v0);
    out2[1] = zivc::isgreaterequal(v1, v1);
    out2[2] = zivc::isgreaterequal(v0, v1);
    out2[3] = zivc::isgreaterequal(v2, v2);
    out2[4] = zivc::isgreaterequal(v3, v3);
    out2[5] = zivc::isgreaterequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::isgreaterequal(v4, v4);
    out2[7] = zivc::isgreaterequal(v3, v4);

    out2[8] = zivc::isgreaterequal(v5, v5);
    out2[9] = zivc::isgreaterequal(v5, v2);
    out2[10] = zivc::isgreaterequal(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isgreaterequal(v0, v0);
    out3[1] = zivc::isgreaterequal(v1, v1);
    out3[2] = zivc::isgreaterequal(v0, v1);
    out3[3] = zivc::isgreaterequal(v2, v2);
    out3[4] = zivc::isgreaterequal(v3, v3);
    out3[5] = zivc::isgreaterequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::isgreaterequal(v4, v4);
    out3[7] = zivc::isgreaterequal(v3, v4);

    out3[8] = zivc::isgreaterequal(v5, v5);
    out3[9] = zivc::isgreaterequal(v5, v2);
    out3[10] = zivc::isgreaterequal(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isgreaterequal(v0, v0);
    out4[1] = zivc::isgreaterequal(v1, v1);
    out4[2] = zivc::isgreaterequal(v0, v1);
    out4[3] = zivc::isgreaterequal(v2, v2);
    out4[4] = zivc::isgreaterequal(v3, v3);
    out4[5] = zivc::isgreaterequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::isgreaterequal(v4, v4);
    out4[7] = zivc::isgreaterequal(v3, v4);

    out4[8] = zivc::isgreaterequal(v5, v5);
    out4[9] = zivc::isgreaterequal(v5, v2);
    out4[10] = zivc::isgreaterequal(v6, v6);
  }
}

__kernel void isGreaterEqualLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                              zivc::ConstGlobalPtr<float16> in16,
                                              zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                              zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isgreaterequal(v0, v0);
    out8[1] = zivc::isgreaterequal(v1, v1);
    out8[2] = zivc::isgreaterequal(v0, v1);
    out8[3] = zivc::isgreaterequal(v2, v2);
    out8[4] = zivc::isgreaterequal(v3, v3);
    out8[5] = zivc::isgreaterequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::isgreaterequal(v4, v4);
    out8[7] = zivc::isgreaterequal(v3, v4);

    out8[8] = zivc::isgreaterequal(v5, v5);
    out8[9] = zivc::isgreaterequal(v5, v2);
    out8[10] = zivc::isgreaterequal(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isgreaterequal(v0, v0);
    out16[1] = zivc::isgreaterequal(v1, v1);
    out16[2] = zivc::isgreaterequal(v0, v1);
    out16[3] = zivc::isgreaterequal(v2, v2);
    out16[4] = zivc::isgreaterequal(v3, v3);
    out16[5] = zivc::isgreaterequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::isgreaterequal(v4, v4);
    out16[7] = zivc::isgreaterequal(v3, v4);

    out16[8] = zivc::isgreaterequal(v5, v5);
    out16[9] = zivc::isgreaterequal(v5, v2);
    out16[10] = zivc::isgreaterequal(v6, v6);
  }
}

__kernel void isLessTestKernel(zivc::ConstGlobalPtr<float> in1,
                               zivc::ConstGlobalPtr<float2> in2,
                               zivc::ConstGlobalPtr<float3> in3,
                               zivc::ConstGlobalPtr<float4> in4,
                               zivc::GlobalPtr<zivc::CompResult<float>> out1,
                               zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                               zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                               zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isless(v0, v0);
    out1[1] = zivc::isless(v1, v1);
    out1[2] = zivc::isless(v0, v1);
    out1[3] = zivc::isless(v2, v2);
    out1[4] = zivc::isless(v3, v3);
    out1[5] = zivc::isless(v4, v4);
    out1[6] = zivc::isless(v4, v3);
    out1[7] = zivc::isless(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::isless(v6, v6);
    out1[9] = zivc::isless(v7, v7);
    out1[10] = zivc::isless(v5, v6);
    out1[11] = zivc::isless(v6, v7);

    out1[12] = zivc::isless(v8, v8);
    out1[13] = zivc::isless(v3, v8);
    out1[14] = zivc::isless(v9, v9);
    out1[15] = zivc::isless(v9, v4);
    out1[16] = zivc::isless(v8, v9);
    out1[17] = zivc::isless(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isless(v0, v0);
    out2[1] = zivc::isless(v1, v1);
    out2[2] = zivc::isless(v0, v1);
    out2[3] = zivc::isless(v2, v2);
    out2[4] = zivc::isless(v3, v3);
    out2[5] = zivc::isless(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::isless(v4, v4);
    out2[7] = zivc::isless(v3, v4);

    out2[8] = zivc::isless(v5, v5);
    out2[9] = zivc::isless(v5, v2);
    out2[10] = zivc::isless(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isless(v0, v0);
    out3[1] = zivc::isless(v1, v1);
    out3[2] = zivc::isless(v0, v1);
    out3[3] = zivc::isless(v2, v2);
    out3[4] = zivc::isless(v3, v3);
    out3[5] = zivc::isless(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::isless(v4, v4);
    out3[7] = zivc::isless(v3, v4);

    out3[8] = zivc::isless(v5, v5);
    out3[9] = zivc::isless(v5, v2);
    out3[10] = zivc::isless(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isless(v0, v0);
    out4[1] = zivc::isless(v1, v1);
    out4[2] = zivc::isless(v0, v1);
    out4[3] = zivc::isless(v2, v2);
    out4[4] = zivc::isless(v3, v3);
    out4[5] = zivc::isless(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::isless(v4, v4);
    out4[7] = zivc::isless(v3, v4);

    out4[8] = zivc::isless(v5, v5);
    out4[9] = zivc::isless(v5, v2);
    out4[10] = zivc::isless(v6, v6);
  }
}

__kernel void isLessLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                      zivc::ConstGlobalPtr<float16> in16,
                                      zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                      zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isless(v0, v0);
    out8[1] = zivc::isless(v1, v1);
    out8[2] = zivc::isless(v0, v1);
    out8[3] = zivc::isless(v2, v2);
    out8[4] = zivc::isless(v3, v3);
    out8[5] = zivc::isless(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::isless(v4, v4);
    out8[7] = zivc::isless(v3, v4);

    out8[8] = zivc::isless(v5, v5);
    out8[9] = zivc::isless(v5, v2);
    out8[10] = zivc::isless(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isless(v0, v0);
    out16[1] = zivc::isless(v1, v1);
    out16[2] = zivc::isless(v0, v1);
    out16[3] = zivc::isless(v2, v2);
    out16[4] = zivc::isless(v3, v3);
    out16[5] = zivc::isless(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::isless(v4, v4);
    out16[7] = zivc::isless(v3, v4);

    out16[8] = zivc::isless(v5, v5);
    out16[9] = zivc::isless(v5, v2);
    out16[10] = zivc::isless(v6, v6);
  }
}

__kernel void isLessEqualTestKernel(zivc::ConstGlobalPtr<float> in1,
                                    zivc::ConstGlobalPtr<float2> in2,
                                    zivc::ConstGlobalPtr<float3> in3,
                                    zivc::ConstGlobalPtr<float4> in4,
                                    zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                    zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                    zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                    zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::islessequal(v0, v0);
    out1[1] = zivc::islessequal(v1, v1);
    out1[2] = zivc::islessequal(v0, v1);
    out1[3] = zivc::islessequal(v2, v2);
    out1[4] = zivc::islessequal(v3, v3);
    out1[5] = zivc::islessequal(v4, v4);
    out1[6] = zivc::islessequal(v4, v3);
    out1[7] = zivc::islessequal(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::islessequal(v6, v6);
    out1[9] = zivc::islessequal(v7, v7);
    out1[10] = zivc::islessequal(v5, v6);
    out1[11] = zivc::islessequal(v6, v7);

    out1[12] = zivc::islessequal(v8, v8);
    out1[13] = zivc::islessequal(v3, v8);
    out1[14] = zivc::islessequal(v9, v9);
    out1[15] = zivc::islessequal(v9, v4);
    out1[16] = zivc::islessequal(v8, v9);
    out1[17] = zivc::islessequal(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::islessequal(v0, v0);
    out2[1] = zivc::islessequal(v1, v1);
    out2[2] = zivc::islessequal(v0, v1);
    out2[3] = zivc::islessequal(v2, v2);
    out2[4] = zivc::islessequal(v3, v3);
    out2[5] = zivc::islessequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::islessequal(v4, v4);
    out2[7] = zivc::islessequal(v3, v4);

    out2[8] = zivc::islessequal(v5, v5);
    out2[9] = zivc::islessequal(v5, v2);
    out2[10] = zivc::islessequal(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::islessequal(v0, v0);
    out3[1] = zivc::islessequal(v1, v1);
    out3[2] = zivc::islessequal(v0, v1);
    out3[3] = zivc::islessequal(v2, v2);
    out3[4] = zivc::islessequal(v3, v3);
    out3[5] = zivc::islessequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::islessequal(v4, v4);
    out3[7] = zivc::islessequal(v3, v4);

    out3[8] = zivc::islessequal(v5, v5);
    out3[9] = zivc::islessequal(v5, v2);
    out3[10] = zivc::islessequal(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::islessequal(v0, v0);
    out4[1] = zivc::islessequal(v1, v1);
    out4[2] = zivc::islessequal(v0, v1);
    out4[3] = zivc::islessequal(v2, v2);
    out4[4] = zivc::islessequal(v3, v3);
    out4[5] = zivc::islessequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::islessequal(v4, v4);
    out4[7] = zivc::islessequal(v3, v4);

    out4[8] = zivc::islessequal(v5, v5);
    out4[9] = zivc::islessequal(v5, v2);
    out4[10] = zivc::islessequal(v6, v6);
  }
}

__kernel void isLessEqualLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                           zivc::ConstGlobalPtr<float16> in16,
                                           zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                           zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::islessequal(v0, v0);
    out8[1] = zivc::islessequal(v1, v1);
    out8[2] = zivc::islessequal(v0, v1);
    out8[3] = zivc::islessequal(v2, v2);
    out8[4] = zivc::islessequal(v3, v3);
    out8[5] = zivc::islessequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::islessequal(v4, v4);
    out8[7] = zivc::islessequal(v3, v4);

    out8[8] = zivc::islessequal(v5, v5);
    out8[9] = zivc::islessequal(v5, v2);
    out8[10] = zivc::islessequal(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::islessequal(v0, v0);
    out16[1] = zivc::islessequal(v1, v1);
    out16[2] = zivc::islessequal(v0, v1);
    out16[3] = zivc::islessequal(v2, v2);
    out16[4] = zivc::islessequal(v3, v3);
    out16[5] = zivc::islessequal(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::islessequal(v4, v4);
    out16[7] = zivc::islessequal(v3, v4);

    out16[8] = zivc::islessequal(v5, v5);
    out16[9] = zivc::islessequal(v5, v2);
    out16[10] = zivc::islessequal(v6, v6);
  }
}

__kernel void isLessGreaterTestKernel(zivc::ConstGlobalPtr<float> in1,
                                      zivc::ConstGlobalPtr<float2> in2,
                                      zivc::ConstGlobalPtr<float3> in3,
                                      zivc::ConstGlobalPtr<float4> in4,
                                      zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                      zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                      zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                      zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::islessgreater(v0, v0);
    out1[1] = zivc::islessgreater(v1, v1);
    out1[2] = zivc::islessgreater(v0, v1);
    out1[3] = zivc::islessgreater(v2, v2);
    out1[4] = zivc::islessgreater(v3, v3);
    out1[5] = zivc::islessgreater(v4, v4);
    out1[6] = zivc::islessgreater(v4, v3);
    out1[7] = zivc::islessgreater(v5, v5);

    //! \attention subnormal won't work on GPU
    out1[8] = zivc::islessgreater(v6, v6);
    out1[9] = zivc::islessgreater(v7, v7);
    out1[10] = zivc::islessgreater(v5, v6);
    out1[11] = zivc::islessgreater(v6, v7);

    out1[12] = zivc::islessgreater(v8, v8);
    out1[13] = zivc::islessgreater(v3, v8);
    out1[14] = zivc::islessgreater(v9, v9);
    out1[15] = zivc::islessgreater(v9, v4);
    out1[16] = zivc::islessgreater(v8, v9);
    out1[17] = zivc::islessgreater(v10, v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::islessgreater(v0, v0);
    out2[1] = zivc::islessgreater(v1, v1);
    out2[2] = zivc::islessgreater(v0, v1);
    out2[3] = zivc::islessgreater(v2, v2);
    out2[4] = zivc::islessgreater(v3, v3);
    out2[5] = zivc::islessgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out2[6] = zivc::islessgreater(v4, v4);
    out2[7] = zivc::islessgreater(v3, v4);

    out2[8] = zivc::islessgreater(v5, v5);
    out2[9] = zivc::islessgreater(v5, v2);
    out2[10] = zivc::islessgreater(v6, v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::islessgreater(v0, v0);
    out3[1] = zivc::islessgreater(v1, v1);
    out3[2] = zivc::islessgreater(v0, v1);
    out3[3] = zivc::islessgreater(v2, v2);
    out3[4] = zivc::islessgreater(v3, v3);
    out3[5] = zivc::islessgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out3[6] = zivc::islessgreater(v4, v4);
    out3[7] = zivc::islessgreater(v3, v4);

    out3[8] = zivc::islessgreater(v5, v5);
    out3[9] = zivc::islessgreater(v5, v2);
    out3[10] = zivc::islessgreater(v6, v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::islessgreater(v0, v0);
    out4[1] = zivc::islessgreater(v1, v1);
    out4[2] = zivc::islessgreater(v0, v1);
    out4[3] = zivc::islessgreater(v2, v2);
    out4[4] = zivc::islessgreater(v3, v3);
    out4[5] = zivc::islessgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out4[6] = zivc::islessgreater(v4, v4);
    out4[7] = zivc::islessgreater(v3, v4);

    out4[8] = zivc::islessgreater(v5, v5);
    out4[9] = zivc::islessgreater(v5, v2);
    out4[10] = zivc::islessgreater(v6, v6);
  }
}

__kernel void isLessGreaterLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                             zivc::ConstGlobalPtr<float16> in16,
                                             zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                             zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::islessgreater(v0, v0);
    out8[1] = zivc::islessgreater(v1, v1);
    out8[2] = zivc::islessgreater(v0, v1);
    out8[3] = zivc::islessgreater(v2, v2);
    out8[4] = zivc::islessgreater(v3, v3);
    out8[5] = zivc::islessgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out8[6] = zivc::islessgreater(v4, v4);
    out8[7] = zivc::islessgreater(v3, v4);

    out8[8] = zivc::islessgreater(v5, v5);
    out8[9] = zivc::islessgreater(v5, v2);
    out8[10] = zivc::islessgreater(v6, v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::islessgreater(v0, v0);
    out16[1] = zivc::islessgreater(v1, v1);
    out16[2] = zivc::islessgreater(v0, v1);
    out16[3] = zivc::islessgreater(v2, v2);
    out16[4] = zivc::islessgreater(v3, v3);
    out16[5] = zivc::islessgreater(v3, v2);

    //! \attention subnormal won't work on GPU
    out16[6] = zivc::islessgreater(v4, v4);
    out16[7] = zivc::islessgreater(v3, v4);

    out16[8] = zivc::islessgreater(v5, v5);
    out16[9] = zivc::islessgreater(v5, v2);
    out16[10] = zivc::islessgreater(v6, v6);
  }
}

__kernel void isFiniteTestKernel(zivc::ConstGlobalPtr<float> in1,
                                 zivc::ConstGlobalPtr<float2> in2,
                                 zivc::ConstGlobalPtr<float3> in3,
                                 zivc::ConstGlobalPtr<float4> in4,
                                 zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                 zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                 zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                 zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isfinite(v0);
    out1[1] = zivc::isfinite(v1);
    out1[2] = zivc::isfinite(v2);
    out1[3] = zivc::isfinite(v3);
    out1[4] = zivc::isfinite(v4);
    out1[5] = zivc::isfinite(v5);

    //! \attention subnormal won't work on GPU
    out1[6] = zivc::isfinite(v6);
    out1[7] = zivc::isfinite(v7);

    out1[8] = zivc::isfinite(v8);
    out1[9] = zivc::isfinite(v9);
    out1[10] = zivc::isfinite(v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isfinite(v0);
    out2[1] = zivc::isfinite(v1);
    out2[2] = zivc::isfinite(v2);
    out2[3] = zivc::isfinite(v3);

    //! \attention subnormal won't work on GPU
    out2[4] = zivc::isfinite(v4);

    out2[5] = zivc::isfinite(v5);
    out2[6] = zivc::isfinite(v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isfinite(v0);
    out3[1] = zivc::isfinite(v1);
    out3[2] = zivc::isfinite(v2);
    out3[3] = zivc::isfinite(v3);

    //! \attention subnormal won't work on GPU
    out3[4] = zivc::isfinite(v4);

    out3[5] = zivc::isfinite(v5);
    out3[6] = zivc::isfinite(v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isfinite(v0);
    out4[1] = zivc::isfinite(v1);
    out4[2] = zivc::isfinite(v2);
    out4[3] = zivc::isfinite(v3);

    //! \attention subnormal won't work on GPU
    out4[4] = zivc::isfinite(v4);

    out4[5] = zivc::isfinite(v5);
    out4[6] = zivc::isfinite(v6);
  }
}

__kernel void isFiniteLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                        zivc::ConstGlobalPtr<float16> in16,
                                        zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                        zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isfinite(v0);
    out8[1] = zivc::isfinite(v1);
    out8[2] = zivc::isfinite(v2);
    out8[3] = zivc::isfinite(v3);

    //! \attention subnormal won't work on GPU
    out8[4] = zivc::isfinite(v4);

    out8[5] = zivc::isfinite(v5);
    out8[6] = zivc::isfinite(v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isfinite(v0);
    out16[1] = zivc::isfinite(v1);
    out16[2] = zivc::isfinite(v2);
    out16[3] = zivc::isfinite(v3);

    //! \attention subnormal won't work on GPU
    out16[4] = zivc::isfinite(v4);

    out16[5] = zivc::isfinite(v5);
    out16[6] = zivc::isfinite(v6);
  }
}

__kernel void isInfTestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float2> in2,
                              zivc::ConstGlobalPtr<float3> in3,
                              zivc::ConstGlobalPtr<float4> in4,
                              zivc::GlobalPtr<zivc::CompResult<float>> out1,
                              zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                              zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                              zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isinf(v0);
    out1[1] = zivc::isinf(v1);
    out1[2] = zivc::isinf(v2);
    out1[3] = zivc::isinf(v3);
    out1[4] = zivc::isinf(v4);
    out1[5] = zivc::isinf(v5);

    //! \attention subnormal won't work on GPU
    out1[6] = zivc::isinf(v6);
    out1[7] = zivc::isinf(v7);

    out1[8] = zivc::isinf(v8);
    out1[9] = zivc::isinf(v9);
    out1[10] = zivc::isinf(v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isinf(v0);
    out2[1] = zivc::isinf(v1);
    out2[2] = zivc::isinf(v2);
    out2[3] = zivc::isinf(v3);

    //! \attention subnormal won't work on GPU
    out2[4] = zivc::isinf(v4);

    out2[5] = zivc::isinf(v5);
    out2[6] = zivc::isinf(v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isinf(v0);
    out3[1] = zivc::isinf(v1);
    out3[2] = zivc::isinf(v2);
    out3[3] = zivc::isinf(v3);

    //! \attention subnormal won't work on GPU
    out3[4] = zivc::isinf(v4);

    out3[5] = zivc::isinf(v5);
    out3[6] = zivc::isinf(v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isinf(v0);
    out4[1] = zivc::isinf(v1);
    out4[2] = zivc::isinf(v2);
    out4[3] = zivc::isinf(v3);

    //! \attention subnormal won't work on GPU
    out4[4] = zivc::isinf(v4);

    out4[5] = zivc::isinf(v5);
    out4[6] = zivc::isinf(v6);
  }
}

__kernel void isInfLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                     zivc::ConstGlobalPtr<float16> in16,
                                     zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                     zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isinf(v0);
    out8[1] = zivc::isinf(v1);
    out8[2] = zivc::isinf(v2);
    out8[3] = zivc::isinf(v3);

    //! \attention subnormal won't work on GPU
    out8[4] = zivc::isinf(v4);

    out8[5] = zivc::isinf(v5);
    out8[6] = zivc::isinf(v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isinf(v0);
    out16[1] = zivc::isinf(v1);
    out16[2] = zivc::isinf(v2);
    out16[3] = zivc::isinf(v3);

    //! \attention subnormal won't work on GPU
    out16[4] = zivc::isinf(v4);

    out16[5] = zivc::isinf(v5);
    out16[6] = zivc::isinf(v6);
  }
}

__kernel void isNanTestKernel(zivc::ConstGlobalPtr<float> in1,
                              zivc::ConstGlobalPtr<float2> in2,
                              zivc::ConstGlobalPtr<float3> in3,
                              zivc::ConstGlobalPtr<float4> in4,
                              zivc::GlobalPtr<zivc::CompResult<float>> out1,
                              zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                              zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                              zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::isnan(v0);
    out1[1] = zivc::isnan(v1);
    out1[2] = zivc::isnan(v2);
    out1[3] = zivc::isnan(v3);
    out1[4] = zivc::isnan(v4);
    out1[5] = zivc::isnan(v5);

    //! \attention subnormal won't work on GPU
    out1[6] = zivc::isnan(v6);
    out1[7] = zivc::isnan(v7);

    out1[8] = zivc::isnan(v8);
    out1[9] = zivc::isnan(v9);
    out1[10] = zivc::isnan(v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::isnan(v0);
    out2[1] = zivc::isnan(v1);
    out2[2] = zivc::isnan(v2);
    out2[3] = zivc::isnan(v3);

    //! \attention subnormal won't work on GPU
    out2[4] = zivc::isnan(v4);

    out2[5] = zivc::isnan(v5);
    out2[6] = zivc::isnan(v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::isnan(v0);
    out3[1] = zivc::isnan(v1);
    out3[2] = zivc::isnan(v2);
    out3[3] = zivc::isnan(v3);

    //! \attention subnormal won't work on GPU
    out3[4] = zivc::isnan(v4);

    out3[5] = zivc::isnan(v5);
    out3[6] = zivc::isnan(v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::isnan(v0);
    out4[1] = zivc::isnan(v1);
    out4[2] = zivc::isnan(v2);
    out4[3] = zivc::isnan(v3);

    //! \attention subnormal won't work on GPU
    out4[4] = zivc::isnan(v4);

    out4[5] = zivc::isnan(v5);
    out4[6] = zivc::isnan(v6);
  }
}

__kernel void isNanLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                     zivc::ConstGlobalPtr<float16> in16,
                                     zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                     zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::isnan(v0);
    out8[1] = zivc::isnan(v1);
    out8[2] = zivc::isnan(v2);
    out8[3] = zivc::isnan(v3);

    //! \attention subnormal won't work on GPU
    out8[4] = zivc::isnan(v4);

    out8[5] = zivc::isnan(v5);
    out8[6] = zivc::isnan(v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::isnan(v0);
    out16[1] = zivc::isnan(v1);
    out16[2] = zivc::isnan(v2);
    out16[3] = zivc::isnan(v3);

    //! \attention subnormal won't work on GPU
    out16[4] = zivc::isnan(v4);

    out16[5] = zivc::isnan(v5);
    out16[6] = zivc::isnan(v6);
  }
}

__kernel void signbitTestKernel(zivc::ConstGlobalPtr<float> in1,
                                zivc::ConstGlobalPtr<float2> in2,
                                zivc::ConstGlobalPtr<float3> in3,
                                zivc::ConstGlobalPtr<float4> in4,
                                zivc::GlobalPtr<zivc::CompResult<float>> out1,
                                zivc::GlobalPtr<zivc::CompResult<float2>> out2,
                                zivc::GlobalPtr<zivc::CompResult<float3>> out3,
                                zivc::GlobalPtr<zivc::CompResult<float4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    const float v0 = in1[0];
    const float v1 = in1[1];
    const float v2 = in1[2];
    const float v3 = in1[3];
    const float v4 = in1[4];
    const float v5 = in1[5];
    const float v6 = in1[6]; // subnormal max
    const float v7 = in1[7]; // subnormal min
    const float v8 = in1[8];
    const float v9 = in1[9];
    const float v10 = in1[10];

    out1[0] = zivc::signbit(v0);
    out1[1] = zivc::signbit(v1);
    out1[2] = zivc::signbit(v2);
    out1[3] = zivc::signbit(v3);
    out1[4] = zivc::signbit(v4);
    out1[5] = zivc::signbit(v5);

    //! \attention subnormal won't work on GPU
    out1[6] = zivc::signbit(v6);
    out1[7] = zivc::signbit(v7);

    out1[8] = zivc::signbit(v8);
    out1[9] = zivc::signbit(v9);
    out1[10] = zivc::signbit(v10);
  }
  // vector2
  {
    const float2 v0 = in2[0];
    const float2 v1 = in2[1];
    const float2 v2 = in2[2];
    const float2 v3 = in2[3];
    const float2 v4 = in2[4];
    const float2 v5 = in2[5];
    const float2 v6 = in2[6];

    out2[0] = zivc::signbit(v0);
    out2[1] = zivc::signbit(v1);
    out2[2] = zivc::signbit(v2);
    out2[3] = zivc::signbit(v3);

    //! \attention subnormal won't work on GPU
    out2[4] = zivc::signbit(v4);

    out2[5] = zivc::signbit(v5);
    out2[6] = zivc::signbit(v6);
  }
  // vector3
  {
    const float3 v0 = in3[0];
    const float3 v1 = in3[1];
    const float3 v2 = in3[2];
    const float3 v3 = in3[3];
    const float3 v4 = in3[4];
    const float3 v5 = in3[5];
    const float3 v6 = in3[6];

    out3[0] = zivc::signbit(v0);
    out3[1] = zivc::signbit(v1);
    out3[2] = zivc::signbit(v2);
    out3[3] = zivc::signbit(v3);

    //! \attention subnormal won't work on GPU
    out3[4] = zivc::signbit(v4);

    out3[5] = zivc::signbit(v5);
    out3[6] = zivc::signbit(v6);
  }
  // vector4
  {
    const float4 v0 = in4[0];
    const float4 v1 = in4[1];
    const float4 v2 = in4[2];
    const float4 v3 = in4[3];
    const float4 v4 = in4[4];
    const float4 v5 = in4[5];
    const float4 v6 = in4[6];

    out4[0] = zivc::signbit(v0);
    out4[1] = zivc::signbit(v1);
    out4[2] = zivc::signbit(v2);
    out4[3] = zivc::signbit(v3);

    //! \attention subnormal won't work on GPU
    out4[4] = zivc::signbit(v4);

    out4[5] = zivc::signbit(v5);
    out4[6] = zivc::signbit(v6);
  }
}

__kernel void signbitLongVecTestKernel(zivc::ConstGlobalPtr<float8> in8,
                                       zivc::ConstGlobalPtr<float16> in16,
                                       zivc::GlobalPtr<zivc::CompResult<float8>> out8,
                                       zivc::GlobalPtr<zivc::CompResult<float16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // vector8
  {
    const float8 v0 = in8[0];
    const float8 v1 = in8[1];
    const float8 v2 = in8[2];
    const float8 v3 = in8[3];
    const float8 v4 = in8[4];
    const float8 v5 = in8[5];
    const float8 v6 = in8[6];

    out8[0] = zivc::signbit(v0);
    out8[1] = zivc::signbit(v1);
    out8[2] = zivc::signbit(v2);
    out8[3] = zivc::signbit(v3);

    //! \attention subnormal won't work on GPU
    out8[4] = zivc::signbit(v4);

    out8[5] = zivc::signbit(v5);
    out8[6] = zivc::signbit(v6);
  }
  // vector16
  {
    const float16 v0 = in16[0];
    const float16 v1 = in16[1];
    const float16 v2 = in16[2];
    const float16 v3 = in16[3];
    const float16 v4 = in16[4];
    const float16 v5 = in16[5];
    const float16 v6 = in16[6];

    out16[0] = zivc::signbit(v0);
    out16[1] = zivc::signbit(v1);
    out16[2] = zivc::signbit(v2);
    out16[3] = zivc::signbit(v3);

    //! \attention subnormal won't work on GPU
    out16[4] = zivc::signbit(v4);

    out16[5] = zivc::signbit(v5);
    out16[6] = zivc::signbit(v6);
  }
}

__kernel void anyTestKernel(zivc::ConstGlobalPtr<int8b> in1,
                            zivc::ConstGlobalPtr<short2> in2,
                            zivc::ConstGlobalPtr<int3> in3,
                            zivc::ConstGlobalPtr<int4> in4,
                            zivc::GlobalPtr<int32b> out1)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    out1[0] = zivc::any(in1[0]);
    out1[1] = zivc::any(in1[1]);
    out1[2] = zivc::any(in1[2]);
  }
  // vector2
  {
    out1[3] = zivc::any(in2[0]);
    out1[4] = zivc::any(in2[1]);
    out1[5] = zivc::any(in2[2]);
    out1[6] = zivc::any(in2[3]);
  }
  // vector3
  {
    out1[7] = zivc::any(in3[0]);
    out1[8] = zivc::any(in3[1]);
    out1[9] = zivc::any(in3[2]);
    out1[10] = zivc::any(in3[3]);
  }
  // vector4
  {
    out1[11] = zivc::any(in4[0]);
    out1[12] = zivc::any(in4[1]);
    out1[13] = zivc::any(in4[2]);
    out1[14] = zivc::any(in4[3]);
  }
}

//__kernel void anyLongVecTestKernel(zivc::ConstGlobalPtr<int8> in8,
//                                   zivc::ConstGlobalPtr<int16> in16,
//                                   zivc::GlobalPtr<int32b> out1)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (index != 0)
//    return;
//
//  // vector8
//  {
//    out1[0] = zivc::any(in8[0]);
//    out1[1] = zivc::any(in8[1]);
//    out1[2] = zivc::any(in8[2]);
//    out1[3] = zivc::any(in8[3]);
//  }
//  // vector16
//  {
//    out1[4] = zivc::any(in16[0]);
//    out1[5] = zivc::any(in16[1]);
//    out1[6] = zivc::any(in16[2]);
//    out1[7] = zivc::any(in16[3]);
//  }
//}

__kernel void allTestKernel(zivc::ConstGlobalPtr<int8b> in1,
                            zivc::ConstGlobalPtr<short2> in2,
                            zivc::ConstGlobalPtr<int3> in3,
                            zivc::ConstGlobalPtr<int4> in4,
                            zivc::GlobalPtr<int32b> out1)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index != 0)
    return;

  // scalar
  {
    out1[0] = zivc::all(in1[0]);
    out1[1] = zivc::all(in1[1]);
    out1[2] = zivc::all(in1[2]);
  }
  // vector2
  {
    out1[3] = zivc::all(in2[0]);
    out1[4] = zivc::all(in2[1]);
    out1[5] = zivc::all(in2[2]);
    out1[6] = zivc::all(in2[3]);
  }
  // vector3
  {
    out1[7] = zivc::all(in3[0]);
    out1[8] = zivc::all(in3[1]);
    out1[9] = zivc::all(in3[2]);
    out1[10] = zivc::all(in3[3]);
  }
  // vector4
  {
    out1[11] = zivc::all(in4[0]);
    out1[12] = zivc::all(in4[1]);
    out1[13] = zivc::all(in4[2]);
    out1[14] = zivc::all(in4[3]);
  }
}

//__kernel void allLongVecTestKernel(zivc::ConstGlobalPtr<int8> in8,
//                                   zivc::ConstGlobalPtr<int16> in16,
//                                   zivc::GlobalPtr<int32b> out1)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (index != 0)
//    return;
//
//  // vector8
//  {
//    out1[0] = zivc::all(in8[0]);
//    out1[1] = zivc::all(in8[1]);
//    out1[2] = zivc::all(in8[2]);
//    out1[3] = zivc::all(in8[3]);
//  }
//  // vector16
//  {
//    out1[4] = zivc::all(in16[0]);
//    out1[5] = zivc::all(in16[1]);
//    out1[6] = zivc::all(in16[2]);
//    out1[7] = zivc::all(in16[3]);
//  }
//}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_RELATION */
