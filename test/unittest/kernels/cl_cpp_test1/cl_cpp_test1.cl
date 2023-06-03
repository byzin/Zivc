/*!
  \file opencl_cpp_test1.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST1_CL
#define ZIVC_TEST_OPENCL_CPP_TEST1_CL

// Zivc
#include "zivc/cl/boolean.hpp"
#include "zivc/cl/limits.hpp"
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"

using zivc::int8b;
using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint8b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

// Utility test

__kernel void utilityClampIntTest(zivc::GlobalPtr<int32b> out_i1,
                                  zivc::GlobalPtr<int2> out_i2,
                                  zivc::GlobalPtr<int3> out_i3,
                                  zivc::GlobalPtr<int4> out_i4,
                                  zivc::GlobalPtr<int8> out_i8,
                                  zivc::GlobalPtr<int16> out_i16)
{
  // clamp
  {
    // clamp v1
    constexpr int32b lo = 0;
    constexpr int32b hi = 10;
    {
      constexpr int32b x = 1;
      const int32b y = zivc::clamp(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr int32b x = -1;
      const int32b y = zivc::clamp(x, lo, hi);
      out_i1[1] = y;
    }
    {
      constexpr int32b x = 20;
      const int32b y = zivc::clamp(x, lo, hi);
      out_i1[2] = y;
    }
  }
  {
    // clamp v2
    constexpr int2 lo = zivc::makeInt2(0);
    constexpr int2 hi = zivc::makeInt2(10);
    {
      constexpr int2 x = zivc::makeInt2(1, 2);
      const int2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr int2 x = zivc::makeInt2(-1, -2);
      const int2 y = zivc::clamp(x, lo, hi);
      out_i2[1] = y;
    }
    {
      constexpr int2 x = zivc::makeInt2(20, 21);
      const int2 y = zivc::clamp(x, lo, hi);
      out_i2[2] = y;
    }
  }
  {
    // clamp v3
    constexpr int3 lo = zivc::makeInt3(0);
    constexpr int3 hi = zivc::makeInt3(10);
    {
      constexpr int3 x = zivc::makeInt3(1, 2, 3);
      const int3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
    {
      constexpr int3 x = zivc::makeInt3(-1, -2, -3);
      const int3 y = zivc::clamp(x, lo, hi);
      out_i3[1] = y;
    }
    {
      constexpr int3 x = zivc::makeInt3(20, 21, 22);
      const int3 y = zivc::clamp(x, lo, hi);
      out_i3[2] = y;
    }
  }
  {
    // clamp v4
    constexpr int4 lo = zivc::makeInt4(0);
    constexpr int4 hi = zivc::makeInt4(10);
    {
      constexpr int4 x = zivc::makeInt4(1, 2, 3, 4);
      const int4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
    {
      constexpr int4 x = zivc::makeInt4(-1, -2, -3, -4);
      const int4 y = zivc::clamp(x, lo, hi);
      out_i4[1] = y;
    }
    {
      constexpr int4 x = zivc::makeInt4(20, 21, 22, 23);
      const int4 y = zivc::clamp(x, lo, hi);
      out_i4[2] = y;
    }
  }
  {
    // clamp v8
    constexpr int8 lo = zivc::makeInt8(0);
    constexpr int8 hi = zivc::makeInt8(10);
    {
      constexpr int8 x = zivc::makeInt8(1, 2, 3, 4, 5, 6, 7, 8);
      const int8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
    {
      constexpr int8 x = zivc::makeInt8(-1, -2, -3, -4, -5, -6, -7, -8);
      const int8 y = zivc::clamp(x, lo, hi);
      out_i8[1] = y;
    }
    {
      constexpr int8 x = zivc::makeInt8(20, 21, 22, 23, 24, 25, 26, 27);
      const int8 y = zivc::clamp(x, lo, hi);
      out_i8[2] = y;
    }
  }
  {
    // clamp v16
    constexpr int16 lo = zivc::makeInt16(0);
    constexpr int16 hi = zivc::makeInt16(10);
    {
      constexpr int16 x = zivc::makeInt16(1, 2, 3, 4, 5, 6, 7, 8,
                                          9, 10, 11, 12, 13, 14, 15, 16);
      const int16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
    {
      constexpr int16 x = zivc::makeInt16(-1, -2, -3, -4, -5, -6, -7, -8,
                                          -9, -10, -11, -12, -13, -14, -15, -16);
      const int16 y = zivc::clamp(x, lo, hi);
      out_i16[1] = y;
    }
    {
      constexpr int16 x = zivc::makeInt16(20, 21, 22, 23, 24, 25, 26, 27,
                                          28, 29, 30, 31, 32, 33, 34, 35);
      const int16 y = zivc::clamp(x, lo, hi);
      out_i16[2] = y;
    }
  }
}

__kernel void utilityAbsUintTest(zivc::GlobalPtr<uint32b> out_i1,
                                 zivc::GlobalPtr<uint2> out_i2,
                                 zivc::GlobalPtr<uint3> out_i3,
                                 zivc::GlobalPtr<uint4> out_i4,
                                 zivc::GlobalPtr<uint8> out_i8,
                                 zivc::GlobalPtr<uint16> out_i16)
{
  // abs
  {
    // abs v1
    {
      constexpr int32b x = 1;
      const uint32b y = zivc::abs(x);
      out_i1[0] = y;
    }
    {
      constexpr int32b x = -1;
      const uint32b y = zivc::abs(x);
      out_i1[1] = y;
    }
  }
  {
    // abs v2
    {
      constexpr int2 x = zivc::makeInt2(1, 2);
      const uint2 y = zivc::abs(x);
      out_i2[0] = y;
    }
    {
      constexpr int2 x = zivc::makeInt2(-1, -2);
      const uint2 y = zivc::abs(x);
      out_i2[1] = y;
    }
  }
  {
    // abs v3
    {
      constexpr int3 x = zivc::makeInt3(1, 2, 3);
      const uint3 y = zivc::abs(x);
      out_i3[0] = y;
    }
    {
      constexpr int3 x = zivc::makeInt3(-1, -2, -3);
      const uint3 y = zivc::abs(x);
      out_i3[1] = y;
    }
  }
  {
    // abs v4
    {
      constexpr int4 x = zivc::makeInt4(1, 2, 3, 4);
      const uint4 y = zivc::abs(x);
      out_i4[0] = y;
    }
    {
      constexpr int4 x = zivc::makeInt4(-1, -2, -3, -4);
      const uint4 y = zivc::abs(x);
      out_i4[1] = y;
    }
  }
  {
    // abs v8
    {
      constexpr int8 x = zivc::makeInt8(1, 2, 3, 4, 5, 6, 7, 8);
      const uint8 y = zivc::abs(x);
      out_i8[0] = y;
    }
    {
      constexpr int8 x = zivc::makeInt8(-1, -2, -3, -4, -5, -6, -7, -8);
      const uint8 y = zivc::abs(x);
      out_i8[1] = y;
    }
  }
  {
    // abs v16
    {
      constexpr int16 x = zivc::makeInt16(1, 2, 3, 4, 5, 6, 7, 8,
                                          9, 10, 11, 12, 13, 14, 15, 16);
      const uint16 y = zivc::abs(x);
      out_i16[0] = y;
    }
    {
      constexpr int16 x = zivc::makeInt16(-1, -2, -3, -4, -5, -6, -7, -8,
                                          -9, -10, -11, -12, -13, -14, -15, -16);
      const uint16 y = zivc::abs(x);
      out_i16[1] = y;
    }
  }
}

__kernel void utilityClampUintTest(zivc::GlobalPtr<uint32b> out_i1,
                                   zivc::GlobalPtr<uint2> out_i2,
                                   zivc::GlobalPtr<uint3> out_i3,
                                   zivc::GlobalPtr<uint4> out_i4,
                                   zivc::GlobalPtr<uint8> out_i8,
                                   zivc::GlobalPtr<uint16> out_i16)
{

  // clamp
  {
    // clamp v1
    constexpr uint32b lo = 0;
    constexpr uint32b hi = 10;
    {
      constexpr uint32b x = 1;
      const uint32b y = zivc::clamp(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr uint32b x = 20;
      const uint32b y = zivc::clamp(x, lo, hi);
      out_i1[1] = y;
    }
  }
  {
    // clamp v2
    constexpr uint2 lo = zivc::makeUInt2(0);
    constexpr uint2 hi = zivc::makeUInt2(10);
    {
      constexpr uint2 x = zivc::makeUInt2(1, 2);
      const uint2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr uint2 x = zivc::makeUInt2(20, 21);
      const uint2 y = zivc::clamp(x, lo, hi);
      out_i2[1] = y;
    }
  }
  {
    // clamp v3
    constexpr uint3 lo = zivc::makeUInt3(0);
    constexpr uint3 hi = zivc::makeUInt3(10);
    {
      constexpr uint3 x = zivc::makeUInt3(1, 2, 3);
      const uint3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
    {
      constexpr uint3 x = zivc::makeUInt3(20, 21, 22);
      const uint3 y = zivc::clamp(x, lo, hi);
      out_i3[1] = y;
    }
  }
  {
    // clamp v4
    constexpr uint4 lo = zivc::makeUInt4(0);
    constexpr uint4 hi = zivc::makeUInt4(10);
    {
      constexpr uint4 x = zivc::makeUInt4(1, 2, 3, 4);
      const uint4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
    {
      constexpr uint4 x = zivc::makeUInt4(20, 21, 22, 23);
      const uint4 y = zivc::clamp(x, lo, hi);
      out_i4[1] = y;
    }
  }
  {
    // clamp v8
    constexpr uint8 lo = zivc::makeUInt8(0);
    constexpr uint8 hi = zivc::makeUInt8(10);
    {
      constexpr uint8 x = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      const uint8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
    {
      constexpr uint8 x = zivc::makeUInt8(20, 21, 22, 23, 24, 25, 26, 27);
      const uint8 y = zivc::clamp(x, lo, hi);
      out_i8[1] = y;
    }
  }
  {
    // clamp v16
    constexpr uint16 lo = zivc::makeUInt16(0);
    constexpr uint16 hi = zivc::makeUInt16(10);
    {
      constexpr uint16 x = zivc::makeUInt16(1, 2, 3, 4, 5, 6, 7, 8,
                                            9, 10, 11, 12, 13, 14, 15, 16);
      const uint16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
    {
      constexpr uint16 x = zivc::makeUInt16(20, 21, 22, 23, 24, 25, 26, 27,
                                            28, 29, 30, 31, 32, 33, 34, 35);
      const uint16 y = zivc::clamp(x, lo, hi);
      out_i16[1] = y;
    }
  }
}

__kernel void utilityAbsFloatTest(zivc::GlobalPtr<float> out_i1,
                                  zivc::GlobalPtr<float2> out_i2,
                                  zivc::GlobalPtr<float3> out_i3,
                                  zivc::GlobalPtr<float4> out_i4,
                                  zivc::GlobalPtr<float8> out_i8,
                                  zivc::GlobalPtr<float16> out_i16)
{
  // abs
  {
    // abs v1
    {
      constexpr float x = 1.0f;
      const float y = zivc::abs(x);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.0f;
      const float y = zivc::abs(x);
      out_i1[1] = y;
    }
  }
  {
    // abs v2
    {
      constexpr float2 x = zivc::makeFloat2(1.0f, 2.0f);
      const float2 y = zivc::abs(x);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(-1.0f, -2.0f);
      const float2 y = zivc::abs(x);
      out_i2[1] = y;
    }
  }
  {
    // abs v3
    {
      constexpr float3 x = zivc::makeFloat3(1.0f, 2.0f, 3.0f);
      const float3 y = zivc::abs(x);
      out_i3[0] = y;
    }
    {
      constexpr float3 x = zivc::makeFloat3(-1.0f, -2.0f, -3.0f);
      const float3 y = zivc::abs(x);
      out_i3[1] = y;
    }
  }
  {
    // abs v4
    {
      constexpr float4 x = zivc::makeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
      const float4 y = zivc::abs(x);
      out_i4[0] = y;
    }
    {
      constexpr float4 x = zivc::makeFloat4(-1.0f, -2.0f, -3.0f, -4.0f);
      const float4 y = zivc::abs(x);
      out_i4[1] = y;
    }
  }
  {
    // abs v8
    {
      constexpr float8 x = zivc::makeFloat8(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const float8 y = zivc::abs(x);
      out_i8[0] = y;
    }
    {
      constexpr float8 x = zivc::makeFloat8(-1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f);
      const float8 y = zivc::abs(x);
      out_i8[1] = y;
    }
  }
  {
    // abs v16
    {
      constexpr float16 x = zivc::makeFloat16(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
                                              9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
      const float16 y = zivc::abs(x);
      out_i16[0] = y;
    }
    {
      constexpr float16 x = zivc::makeFloat16(-1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f,
                                              -9.0f, -10.0f, -11.0f, -12.0f, -13.0f, -14.0f, -15.0f, -16.0f);
      const float16 y = zivc::abs(x);
      out_i16[1] = y;
    }
  }
}

__kernel void utilityClampFloatTest(zivc::GlobalPtr<float> out_i1,
                                    zivc::GlobalPtr<float2> out_i2,
                                    zivc::GlobalPtr<float3> out_i3,
                                    zivc::GlobalPtr<float4> out_i4,
                                    zivc::GlobalPtr<float8> out_i8,
                                    zivc::GlobalPtr<float16> out_i16)
{

  // clamp
  {
    // clamp v1
    constexpr float lo = 0.0f;
    constexpr float hi = 10.0f;
    {
      constexpr float x = 1.0f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.0f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[1] = y;
    }
    {
      constexpr float x = 20.0f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[2] = y;
    }
  }
  {
    // clamp v2
    constexpr float2 lo = zivc::makeFloat2(0.0f);
    constexpr float2 hi = zivc::makeFloat2(10.0f);
    {
      constexpr float2 x = zivc::makeFloat2(1.0f, 2.0f);
      const float2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(-1.0f, -2.0f);
      const float2 y = zivc::clamp(x, lo, hi);
      out_i2[1] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(20.0f, 21.0f);
      const float2 y = zivc::clamp(x, lo, hi);
      out_i2[2] = y;
    }
  }
  {
    // clamp v3
    constexpr float3 lo = zivc::makeFloat3(0.0f);
    constexpr float3 hi = zivc::makeFloat3(10.0f);
    {
      constexpr float3 x = zivc::makeFloat3(1.0f, 2.0f, 3.0f);
      const float3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
    {
      constexpr float3 x = zivc::makeFloat3(-1.0f, -2.0f, -3.0f);
      const float3 y = zivc::clamp(x, lo, hi);
      out_i3[1] = y;
    }
    {
      constexpr float3 x = zivc::makeFloat3(20.0f, 21.0f, 22.0f);
      const float3 y = zivc::clamp(x, lo, hi);
      out_i3[2] = y;
    }
  }
  {
    // clamp v4
    constexpr float4 lo = zivc::makeFloat4(0.0f);
    constexpr float4 hi = zivc::makeFloat4(10.0f);
    {
      constexpr float4 x = zivc::makeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
      const float4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
    {
      constexpr float4 x = zivc::makeFloat4(-1.0f, -2.0f, -3.0f, -4.0f);
      const float4 y = zivc::clamp(x, lo, hi);
      out_i4[1] = y;
    }
    {
      constexpr float4 x = zivc::makeFloat4(20.0f, 21.0f, 22.0f, 23.0f);
      const float4 y = zivc::clamp(x, lo, hi);
      out_i4[2] = y;
    }
  }
  {
    // clamp v8
    constexpr float8 lo = zivc::makeFloat8(0.0f);
    constexpr float8 hi = zivc::makeFloat8(10.0f);
    {
      constexpr float8 x = zivc::makeFloat8(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const float8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
    {
      constexpr float8 x = zivc::makeFloat8(-1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f);
      const float8 y = zivc::clamp(x, lo, hi);
      out_i8[1] = y;
    }
    {
      constexpr float8 x = zivc::makeFloat8(20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f);
      const float8 y = zivc::clamp(x, lo, hi);
      out_i8[2] = y;
    }
  }
  {
    // clamp v16
    constexpr float16 lo = zivc::makeFloat16(0.0f);
    constexpr float16 hi = zivc::makeFloat16(10.0f);
    {
      constexpr float16 x = zivc::makeFloat16(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
                                              9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
      const float16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
    {
      constexpr float16 x = zivc::makeFloat16(-1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f,
                                              -9.0f, -10.0f, -11.0f, -12.0f, -13.0f, -14.0f, -15.0f, -16.0f);
      const float16 y = zivc::clamp(x, lo, hi);
      out_i16[1] = y;
    }
    {
      constexpr float16 x = zivc::makeFloat16(20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f,
                                              28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f);
      const float16 y = zivc::clamp(x, lo, hi);
      out_i16[2] = y;
    }
  }
}

// Numeric limits test

namespace inner {

template <typename Type> inline
void setNumericLimit(zivc::GlobalPtr<zivc::Boolean> out_bool,
                     zivc::GlobalPtr<zivc::int32b> out_int,
                     zivc::GlobalPtr<Type> out_type) noexcept
{
  using LimitT = zivc::NumericLimits<Type>;
  size_t index = 0;
  out_bool[index++] = LimitT::isSpecialized();
  out_bool[index++] = LimitT::isSigned();
  out_bool[index++] = LimitT::isInteger();
  out_bool[index++] = LimitT::isExact();
  out_bool[index++] = LimitT::hasInfinity();
  out_bool[index++] = LimitT::hasQuietNan();
  index = 0;
  out_int[index++] = LimitT::digits();
  out_int[index++] = LimitT::digits10();
  out_int[index++] = LimitT::maxDigits10();
  out_int[index++] = LimitT::radix();
  index = 0;
  out_type[index++] = LimitT::min();
  out_type[index++] = LimitT::lowest();
  out_type[index++] = LimitT::max();
  out_type[index++] = LimitT::epsilon();
  out_type[index++] = LimitT::infinity();
  out_type[index++] = LimitT::quietNan();
}

} /* namespace inner */

__kernel void limitInt8Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                            zivc::GlobalPtr<zivc::int32b> out_int,
                            zivc::GlobalPtr<zivc::int8b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int8b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint8Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::uint8b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint8b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt16Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int16b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int16b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint16Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint16b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint16b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt32Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int32b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int32b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint32Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint32b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint32b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt64Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int64b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int64b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint64Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint64b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint64b>(out_bool, out_int, out_type);
  }
}

__kernel void limitFloatTest(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<float> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<float>(out_bool, out_int, out_type);
  }
}

__kernel void limitDoubleTest(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<double> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<double>(out_bool, out_int, out_type);
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST1_CL */
