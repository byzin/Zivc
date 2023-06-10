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

// Utility test

__kernel void utilityClampIntTest(zivc::GlobalPtr<int32b> out_i1,
                                  zivc::GlobalPtr<int2> out_i2,
                                  zivc::GlobalPtr<int3> out_i3,
                                  zivc::GlobalPtr<int4> out_i4,
                                  zivc::GlobalPtr<int8> out_i8,
                                  zivc::GlobalPtr<int16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

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
      constexpr int2 x = zivc::makeInt2(1, -1);
      const int2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr int2 x = zivc::makeInt2(20, -20);
      const int2 y = zivc::clamp(x, lo, hi);
      out_i2[1] = y;
    }
  }
  {
    // clamp v3
    constexpr int3 lo = zivc::makeInt3(0);
    constexpr int3 hi = zivc::makeInt3(10);
    {
      constexpr int3 x = zivc::makeInt3(1, -1, 20);
      const int3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr int4 lo = zivc::makeInt4(0);
    constexpr int4 hi = zivc::makeInt4(10);
    {
      constexpr int4 x = zivc::makeInt4(1, -1, 20, -20);
      const int4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr int8 lo = zivc::makeInt8(0);
    constexpr int8 hi = zivc::makeInt8(10);
    {
      constexpr int8 x = zivc::makeInt8(1, -1, 20, -20,
                                        1, -1, 20, -20);
      const int8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr int16 lo = zivc::makeInt16(0);
    constexpr int16 hi = zivc::makeInt16(10);
    {
      constexpr int16 x = zivc::makeInt16(1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20);
      const int16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
  }
}

#define ZIVC_CL_TEST_MAXMIN(x, lo, hi) zivc::min(zivc::max(x, lo), hi)

__kernel void utilityMaxMinIntTest(zivc::GlobalPtr<int32b> out_i1,
                                   zivc::GlobalPtr<int2> out_i2,
                                   zivc::GlobalPtr<int3> out_i3,
                                   zivc::GlobalPtr<int4> out_i4,
                                   zivc::GlobalPtr<int8> out_i8,
                                   zivc::GlobalPtr<int16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  // clamp
  {
    // clamp v1
    constexpr int32b lo = 0;
    constexpr int32b hi = 10;
    {
      constexpr int32b x = 1;
      const int32b y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr int32b x = -1;
      const int32b y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[1] = y;
    }
    {
      constexpr int32b x = 20;
      const int32b y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[2] = y;
    }
  }
  {
    // clamp v2
    constexpr int2 lo = zivc::makeInt2(0);
    constexpr int2 hi = zivc::makeInt2(10);
    {
      constexpr int2 x = zivc::makeInt2(1, -1);
      const int2 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr int2 x = zivc::makeInt2(20, -20);
      const int2 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i2[1] = y;
    }
  }
  {
    // clamp v3
    constexpr int3 lo = zivc::makeInt3(0);
    constexpr int3 hi = zivc::makeInt3(10);
    {
      constexpr int3 x = zivc::makeInt3(1, -1, 20);
      const int3 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr int4 lo = zivc::makeInt4(0);
    constexpr int4 hi = zivc::makeInt4(10);
    {
      constexpr int4 x = zivc::makeInt4(1, -1, 20, -20);
      const int4 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr int8 lo = zivc::makeInt8(0);
    constexpr int8 hi = zivc::makeInt8(10);
    {
      constexpr int8 x = zivc::makeInt8(1, -1, 20, -20,
                                        1, -1, 20, -20);
      const int8 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr int16 lo = zivc::makeInt16(0);
    constexpr int16 hi = zivc::makeInt16(10);
    {
      constexpr int16 x = zivc::makeInt16(1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20);
      const int16 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i16[0] = y;
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
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

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
      constexpr int2 x = zivc::makeInt2(1, -1);
      const uint2 y = zivc::abs(x);
      out_i2[0] = y;
    }
  }
  {
    // abs v3
    {
      constexpr int3 x = zivc::makeInt3(1, -1, 20);
      const uint3 y = zivc::abs(x);
      out_i3[0] = y;
    }
  }
  {
    // abs v4
    {
      constexpr int4 x = zivc::makeInt4(1, -1, 20, -20);
      const uint4 y = zivc::abs(x);
      out_i4[0] = y;
    }
  }
  {
    // abs v8
    {
      constexpr int8 x = zivc::makeInt8(1, -1, 20, -20,
                                        1, -1, 20, -20);
      const uint8 y = zivc::abs(x);
      out_i8[0] = y;
    }
  }
  {
    // abs v16
    {
      constexpr int16 x = zivc::makeInt16(1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20,
                                          1, -1, 20, -20);
      const uint16 y = zivc::abs(x);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityUpsampleIntTest(zivc::GlobalPtr<int32b> out_i1,
                                     zivc::GlobalPtr<int2> out_i2,
                                     zivc::GlobalPtr<int3> out_i3,
                                     zivc::GlobalPtr<int4> out_i4,
                                     zivc::GlobalPtr<int8> out_i8,
                                     zivc::GlobalPtr<int16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  constexpr int16b hi = 0b0101'1010'0101'1010;
  constexpr uint16b lo = 0b0000'1111'0000'1111u;
  {
    // v1
    {
      const int32b y = zivc::upsample(hi, lo);
      out_i1[0] = y;
    }
  }
  {
    // v2
    {
      constexpr short2 x = zivc::makeShort2(hi);
      constexpr ushort2 y = zivc::makeUShort2(lo);
      const int2 z = zivc::upsample(x, y);
      out_i2[0] = z;
    }
  }
  {
    // v3
    {
      constexpr short3 x = zivc::makeShort3(hi);
      constexpr ushort3 y = zivc::makeUShort3(lo);
      const int3 z = zivc::upsample(x, y);
      out_i3[0] = z;
    }
  }
  {
    // v4
    {
      constexpr short4 x = zivc::makeShort4(hi);
      constexpr ushort4 y = zivc::makeUShort4(lo);
      const int4 z = zivc::upsample(x, y);
      out_i4[0] = z;
    }
  }
  {
    // v8
    {
      constexpr short8 x = zivc::makeShort8(hi);
      constexpr ushort8 y = zivc::makeUShort8(lo);
      const int8 z = zivc::upsample(x, y);
      out_i8[0] = z;
    }
  }
  {
    // v16
    {
      constexpr short16 x = zivc::makeShort16(hi);
      constexpr ushort16 y = zivc::makeUShort16(lo);
      const int16 z = zivc::upsample(x, y);
      out_i16[0] = z;
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
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

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
      constexpr uint2 x = zivc::makeUInt2(1, 20);
      const uint2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
  }
  {
    // clamp v3
    constexpr uint3 lo = zivc::makeUInt3(0);
    constexpr uint3 hi = zivc::makeUInt3(10);
    {
      constexpr uint3 x = zivc::makeUInt3(1, 20, 1);
      const uint3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr uint4 lo = zivc::makeUInt4(0);
    constexpr uint4 hi = zivc::makeUInt4(10);
    {
      constexpr uint4 x = zivc::makeUInt4(1, 20, 1, 20);
      const uint4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr uint8 lo = zivc::makeUInt8(0);
    constexpr uint8 hi = zivc::makeUInt8(10);
    {
      constexpr uint8 x = zivc::makeUInt8(1, 20, 1, 20,
                                          1, 20, 1, 20);
      const uint8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr uint16 lo = zivc::makeUInt16(0);
    constexpr uint16 hi = zivc::makeUInt16(10);
    {
      constexpr uint16 x = zivc::makeUInt16(1, 20, 1, 20,
                                            1, 20, 1, 20,
                                            1, 20, 1, 20,
                                            1, 20, 1, 20);
      const uint16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityMaxMinUintTest(zivc::GlobalPtr<uint32b> out_i1,
                                    zivc::GlobalPtr<uint2> out_i2,
                                    zivc::GlobalPtr<uint3> out_i3,
                                    zivc::GlobalPtr<uint4> out_i4,
                                    zivc::GlobalPtr<uint8> out_i8,
                                    zivc::GlobalPtr<uint16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  // clamp
  {
    // clamp v1
    constexpr uint32b lo = 0;
    constexpr uint32b hi = 10;
    {
      constexpr uint32b x = 1;
      const uint32b y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr uint32b x = 20;
      const uint32b y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[1] = y;
    }
  }
  {
    // clamp v2
    constexpr uint2 lo = zivc::makeUInt2(0);
    constexpr uint2 hi = zivc::makeUInt2(10);
    {
      constexpr uint2 x = zivc::makeUInt2(1, 20);
      const uint2 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i2[0] = y;
    }
  }
  {
    // clamp v3
    constexpr uint3 lo = zivc::makeUInt3(0);
    constexpr uint3 hi = zivc::makeUInt3(10);
    {
      constexpr uint3 x = zivc::makeUInt3(1, 20, 1);
      const uint3 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr uint4 lo = zivc::makeUInt4(0);
    constexpr uint4 hi = zivc::makeUInt4(10);
    {
      constexpr uint4 x = zivc::makeUInt4(1, 20, 1, 20);
      const uint4 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr uint8 lo = zivc::makeUInt8(0);
    constexpr uint8 hi = zivc::makeUInt8(10);
    {
      constexpr uint8 x = zivc::makeUInt8(1, 20, 1, 20,
                                          1, 20, 1, 20);
      const uint8 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr uint16 lo = zivc::makeUInt16(0);
    constexpr uint16 hi = zivc::makeUInt16(10);
    {
      constexpr uint16 x = zivc::makeUInt16(1, 20, 1, 20,
                                            1, 20, 1, 20,
                                            1, 20, 1, 20,
                                            1, 20, 1, 20);
      const uint16 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityClzUIntTest(zivc::GlobalPtr<uint32b> out_i1,
                                 zivc::GlobalPtr<uint2> out_i2,
                                 zivc::GlobalPtr<uint3> out_i3,
                                 zivc::GlobalPtr<uint4> out_i4,
                                 zivc::GlobalPtr<uint8> out_i8,
                                 zivc::GlobalPtr<uint16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  {
    // v1
    {
      constexpr uint32b x = 0b00001111'00001111'00001111'00001111u;
      const uint32b y = zivc::clz(x);
      out_i1[0] = y;
    }
    {
      constexpr uint32b x = 0b11110000'11110000'11110000'11110000u;
      const uint32b y = zivc::clz(x);
      out_i1[1] = y;
    }
  }
  {
    // v2
    {
      constexpr uint2 x = zivc::makeUInt2(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u);
      const uint2 y = zivc::clz(x);
      out_i2[0] = y;
    }
  }
  {
    // v3
    {
      constexpr uint3 x = zivc::makeUInt3(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u);
      const uint3 y = zivc::clz(x);
      out_i3[0] = y;
    }
  }
  {
    // v4
    {
      constexpr uint4 x = zivc::makeUInt4(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      const uint4 y = zivc::clz(x);
      out_i4[0] = y;
    }
  }
  {
    // v8
    {
      constexpr uint8 x = zivc::makeUInt8(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u,
                                          0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      const uint8 y = zivc::clz(x);
      out_i8[0] = y;
    }
  }
  {
    // v16
    {
      constexpr uint16 x = zivc::makeUInt16(0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u);
      const uint16 y = zivc::clz(x);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityPopcountUIntTest(zivc::GlobalPtr<uint32b> out_i1,
                                      zivc::GlobalPtr<uint2> out_i2,
                                      zivc::GlobalPtr<uint3> out_i3,
                                      zivc::GlobalPtr<uint4> out_i4,
                                      zivc::GlobalPtr<uint8> out_i8,
                                      zivc::GlobalPtr<uint16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  {
    // v1
    {
      constexpr uint32b x = 0b00001111'00001111'00001111'00001111u;
      const uint32b y = zivc::popcount(x);
      out_i1[0] = y;
    }
    {
      constexpr uint32b x = 0b11110000'11110000'11110000'11110000u;
      const uint32b y = zivc::popcount(x);
      out_i1[1] = y;
    }
  }
  {
    // v2
    {
      constexpr uint2 x = zivc::makeUInt2(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u);
      const uint2 y = zivc::popcount(x);
      out_i2[0] = y;
    }
  }
  {
    // v3
    {
      constexpr uint3 x = zivc::makeUInt3(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u);
      const uint3 y = zivc::popcount(x);
      out_i3[0] = y;
    }
  }
  {
    // v4
    {
      constexpr uint4 x = zivc::makeUInt4(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      const uint4 y = zivc::popcount(x);
      out_i4[0] = y;
    }
  }
  {
    // v8
    {
      constexpr uint8 x = zivc::makeUInt8(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u,
                                          0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      const uint8 y = zivc::popcount(x);
      out_i8[0] = y;
    }
  }
  {
    // v16
    {
      constexpr uint16 x = zivc::makeUInt16(0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u);
      const uint16 y = zivc::popcount(x);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityRotateUIntTest(zivc::GlobalPtr<uint32b> out_i1,
                                    zivc::GlobalPtr<uint2> out_i2,
                                    zivc::GlobalPtr<uint3> out_i3,
                                    zivc::GlobalPtr<uint4> out_i4,
                                    zivc::GlobalPtr<uint8> out_i8,
                                    zivc::GlobalPtr<uint16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  {
    // v1
    {
      constexpr uint32b x = 0b00001111'00001111'00001111'00001111u;
      constexpr uint32b s = 4;
      const uint32b y = zivc::rotate(x, s);
      out_i1[0] = y;
    }
    {
      constexpr uint32b x = 0b11110000'11110000'11110000'11110000u;
      constexpr uint32b s = 6;
      const uint32b y = zivc::rotate(x, s);
      out_i1[1] = y;
    }
  }
  {
    // v2
    {
      constexpr uint2 x = zivc::makeUInt2(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u);
      constexpr uint2 s = zivc::makeUInt2(4, 6);
      const uint2 y = zivc::rotate(x, s);
      out_i2[0] = y;
    }
  }
  {
    // v3
    {
      constexpr uint3 x = zivc::makeUInt3(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u);
      constexpr uint3 s = zivc::makeUInt3(4, 6, 8);
      const uint3 y = zivc::rotate(x, s);
      out_i3[0] = y;
    }
  }
  {
    // v4
    {
      constexpr uint4 x = zivc::makeUInt4(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      constexpr uint4 s = zivc::makeUInt4(4, 6, 8, 1);
      const uint4 y = zivc::rotate(x, s);
      out_i4[0] = y;
    }
  }
  {
    // v8
    {
      constexpr uint8 x = zivc::makeUInt8(0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u,
                                          0b00001111'00001111'00001111'00001111u,
                                          0b11110000'11110000'11110000'11110000u,
                                          0b01010101'01010101'01010101'01010101u,
                                          0b10101010'10101010'10101010'10101010u);
      constexpr uint8 s = zivc::makeUInt8(4, 6, 8, 1, 4, 6, 8, 1);
      const uint8 y = zivc::rotate(x, s);
      out_i8[0] = y;
    }
  }
  {
    // v16
    {
      constexpr uint16 x = zivc::makeUInt16(0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u,
                                            0b00001111'00001111'00001111'00001111u,
                                            0b11110000'11110000'11110000'11110000u,
                                            0b01010101'01010101'01010101'01010101u,
                                            0b10101010'10101010'10101010'10101010u);
      constexpr uint16 s = zivc::makeUInt16(4, 6, 8, 1, 4, 6, 8, 1,
                                            4, 6, 8, 1, 4, 6, 8, 1);
      const uint16 y = zivc::rotate(x, s);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityUpsampleUintTest(zivc::GlobalPtr<uint32b> out_i1,
                                      zivc::GlobalPtr<uint2> out_i2,
                                      zivc::GlobalPtr<uint3> out_i3,
                                      zivc::GlobalPtr<uint4> out_i4,
                                      zivc::GlobalPtr<uint8> out_i8,
                                      zivc::GlobalPtr<uint16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  constexpr uint16b hi = 0b1010'0101'1010'0101u;
  constexpr uint16b lo = 0b0000'1111'0000'1111u;
  {
    // v1
    {
      const uint32b y = zivc::upsample(hi, lo);
      out_i1[0] = y;
    }
  }
  {
    // v2
    {
      constexpr ushort2 x = zivc::makeUShort2(hi);
      constexpr ushort2 y = zivc::makeUShort2(lo);
      const uint2 z = zivc::upsample(x, y);
      out_i2[0] = z;
    }
  }
  {
    // v3
    {
      constexpr ushort3 x = zivc::makeUShort3(hi);
      constexpr ushort3 y = zivc::makeUShort3(lo);
      const uint3 z = zivc::upsample(x, y);
      out_i3[0] = z;
    }
  }
  {
    // v4
    {
      constexpr ushort4 x = zivc::makeUShort4(hi);
      constexpr ushort4 y = zivc::makeUShort4(lo);
      const uint4 z = zivc::upsample(x, y);
      out_i4[0] = z;
    }
  }
  {
    // v8
    {
      constexpr ushort8 x = zivc::makeUShort8(hi);
      constexpr ushort8 y = zivc::makeUShort8(lo);
      const uint8 z = zivc::upsample(x, y);
      out_i8[0] = z;
    }
  }
  {
    // v16
    {
      constexpr ushort16 x = zivc::makeUShort16(hi);
      constexpr ushort16 y = zivc::makeUShort16(lo);
      const uint16 z = zivc::upsample(x, y);
      out_i16[0] = z;
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
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  constexpr float minf = LimitT::min();
  const float inf = LimitT::infinity();
  // abs
  {
    // abs v1
    {
      constexpr float x = 1.5f;
      const float y = zivc::abs(x);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.5f;
      const float y = zivc::abs(x);
      out_i1[1] = y;
    }
  }
  {
    // abs v2
    {
      constexpr float2 x = zivc::makeFloat2(1.5f, -1.5f);
      const float2 y = zivc::abs(x);
      out_i2[0] = y;
    }
  }
  {
    // abs v3
    {
      constexpr float3 x = zivc::makeFloat3(1.5f, -1.5f, maxf);
      const float3 y = zivc::abs(x);
      out_i3[0] = y;
    }
  }
  {
    // abs v4
    {
      constexpr float4 x = zivc::makeFloat4(1.5f, -1.5f, maxf, -maxf);
      const float4 y = zivc::abs(x);
      out_i4[0] = y;
    }
  }
  {
    // abs v8
    {
      const float8 x = zivc::makeFloat8(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float8 y = zivc::abs(x);
      out_i8[0] = y;
    }
  }
  {
    // abs v16
    {
      const float16 x = zivc::makeFloat16(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                                          1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float16 y = zivc::abs(x);
      out_i16[0] = y;
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
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  constexpr float minf = LimitT::min();
  const float inf = LimitT::infinity();
  // clamp
  {
    // clamp v1
    constexpr float lo = 0.0f;
    constexpr float hi = 10.0f;
    {
      constexpr float x = 1.5f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.5f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[1] = y;
    }
    {
      constexpr float x = 20.5f;
      const float y = zivc::clamp(x, lo, hi);
      out_i1[2] = y;
    }
  }
  {
    // clamp v2
    constexpr float2 lo = zivc::makeFloat2(0.0f);
    constexpr float2 hi = zivc::makeFloat2(10.0f);
    {
      constexpr float2 x = zivc::makeFloat2(1.5f, -1.5f);
      const float2 y = zivc::clamp(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(20.5f, -20.5f);
      const float2 y = zivc::clamp(x, lo, hi);
      out_i2[1] = y;
    }
  }
  {
    // clamp v3
    constexpr float3 lo = zivc::makeFloat3(0.0f);
    constexpr float3 hi = zivc::makeFloat3(10.0f);
    {
      constexpr float3 x = zivc::makeFloat3(1.5f, -1.5f, 20.0f);
      const float3 y = zivc::clamp(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr float4 lo = zivc::makeFloat4(0.0f);
    constexpr float4 hi = zivc::makeFloat4(10.0f);
    {
      constexpr float4 x = zivc::makeFloat4(1.5f, -1.5f, maxf, -maxf);
      const float4 y = zivc::clamp(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr float8 lo = zivc::makeFloat8(0.0f);
    constexpr float8 hi = zivc::makeFloat8(10.0f);
    {
      const float8 x = zivc::makeFloat8(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float8 y = zivc::clamp(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr float16 lo = zivc::makeFloat16(0.0f);
    constexpr float16 hi = zivc::makeFloat16(10.0f);
    {
      const float16 x = zivc::makeFloat16(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                                          1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float16 y = zivc::clamp(x, lo, hi);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityMaxMinFloatTest(zivc::GlobalPtr<float> out_i1,
                                     zivc::GlobalPtr<float2> out_i2,
                                     zivc::GlobalPtr<float3> out_i3,
                                     zivc::GlobalPtr<float4> out_i4,
                                     zivc::GlobalPtr<float8> out_i8,
                                     zivc::GlobalPtr<float16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  constexpr float minf = LimitT::min();
  const float inf = LimitT::infinity();
  // clamp
  {
    // clamp v1
    constexpr float lo = 0.0f;
    constexpr float hi = 10.0f;
    {
      constexpr float x = 1.5f;
      const float y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.5f;
      const float y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[1] = y;
    }
    {
      constexpr float x = 20.5f;
      const float y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i1[2] = y;
    }
  }
  {
    // clamp v2
    constexpr float2 lo = zivc::makeFloat2(0.0f);
    constexpr float2 hi = zivc::makeFloat2(10.0f);
    {
      constexpr float2 x = zivc::makeFloat2(1.5f, -1.5f);
      const float2 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(20.5f, -20.5f);
      const float2 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i2[1] = y;
    }
  }
  {
    // clamp v3
    constexpr float3 lo = zivc::makeFloat3(0.0f);
    constexpr float3 hi = zivc::makeFloat3(10.0f);
    {
      constexpr float3 x = zivc::makeFloat3(1.5f, -1.5f, 20.0f);
      const float3 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i3[0] = y;
    }
  }
  {
    // clamp v4
    constexpr float4 lo = zivc::makeFloat4(0.0f);
    constexpr float4 hi = zivc::makeFloat4(10.0f);
    {
      constexpr float4 x = zivc::makeFloat4(1.5f, -1.5f, maxf, -maxf);
      const float4 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i4[0] = y;
    }
  }
  {
    // clamp v8
    constexpr float8 lo = zivc::makeFloat8(0.0f);
    constexpr float8 hi = zivc::makeFloat8(10.0f);
    {
      const float8 x = zivc::makeFloat8(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float8 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i8[0] = y;
    }
  }
  {
    // clamp v16
    constexpr float16 lo = zivc::makeFloat16(0.0f);
    constexpr float16 hi = zivc::makeFloat16(10.0f);
    {
      const float16 x = zivc::makeFloat16(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                                          1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float16 y = ZIVC_CL_TEST_MAXMIN(x, lo, hi);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityDegreesRadiansFloatTest(zivc::GlobalPtr<float> out_i1,
                                             zivc::GlobalPtr<float2> out_i2,
                                             zivc::GlobalPtr<float3> out_i3,
                                             zivc::GlobalPtr<float4> out_i4,
                                             zivc::GlobalPtr<float8> out_i8,
                                             zivc::GlobalPtr<float16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  // clamp
  {
    // clamp v1
    {
      constexpr float x = 1.0f;
      const float y = zivc::degrees(x);
      out_i1[0] = y;
      const float z = zivc::radians(y);
      out_i1[1] = z;
    }
    {
      constexpr float x = 2.0f;
      const float y = zivc::degrees(x);
      out_i1[2] = y;
      const float z = zivc::radians(y);
      out_i1[3] = z;
    }
  }
  {
    // clamp v2
    {
      constexpr float2 x = zivc::makeFloat2(1.0f, 2.0f);
      const float2 y = zivc::degrees(x);
      out_i2[0] = y;
      const float2 z = zivc::radians(y);
      out_i2[1] = z;
    }
  }
  {
    // clamp v3
    {
      constexpr float3 x = zivc::makeFloat3(1.0f, 2.0f, -1.0f);
      const float3 y = zivc::degrees(x);
      out_i3[0] = y;
      const float3 z = zivc::radians(y);
      out_i3[1] = z;
    }
  }
  {
    // clamp v4
    {
      constexpr float4 x = zivc::makeFloat4(1.0f, 2.0f, -1.0f, -2.0f);
      const float4 y = zivc::degrees(x);
      out_i4[0] = y;
      const float4 z = zivc::radians(y);
      out_i4[1] = z;
    }
  }
  {
    // clamp v8
    {
      const float8 x = zivc::makeFloat8(1.0f, 2.0f, -1.0f, -2.0f, 10.0f, 20.0f, -10.0f, -20.0f);
      const float8 y = zivc::degrees(x);
      out_i8[0] = y;
      const float8 z = zivc::radians(y);
      out_i8[1] = z;
    }
  }
  {
    // clamp v16
    {
      const float16 x = zivc::makeFloat16(1.0f, 2.0f, -1.0f, -2.0f, 10.0f, 20.0f, -10.0f, -20.0f,
                                          1.0f, 2.0f, -1.0f, -2.0f, 10.0f, 20.0f, -10.0f, -20.0f);
      const float16 y = zivc::degrees(x);
      out_i16[0] = y;
      const float16 z = zivc::radians(y);
      out_i16[1] = z;
    }
  }
}

__kernel void utilitySignFloatTest(zivc::GlobalPtr<float> out_i1,
                                   zivc::GlobalPtr<float2> out_i2,
                                   zivc::GlobalPtr<float3> out_i3,
                                   zivc::GlobalPtr<float4> out_i4,
                                   zivc::GlobalPtr<float8> out_i8,
                                   zivc::GlobalPtr<float16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  const float inf = LimitT::infinity();
  {
    // v1
    {
      constexpr float x = 2.0f;
      const float y = zivc::sign(x);
      out_i1[0] = y;
    }
    {
      constexpr float x = -2.0f;
      const float y = zivc::sign(x);
      out_i1[1] = y;
    }
    {
      constexpr float x = 0.0f;
      const float y = zivc::sign(x);
      out_i1[2] = y;
    }
    {
      const float x = LimitT::quietNan();
      const float y = zivc::sign(x);
      out_i1[3] = y;
    }
  }
  {
    // v2
    {
      constexpr float2 x = zivc::makeFloat2(2.0f, -2.0f);
      const float2 y = zivc::sign(x);
      out_i2[0] = y;
    }
    {
      const float2 x = zivc::makeFloat2(0.0f, LimitT::quietNan());
      const float2 y = zivc::sign(x);
      out_i2[1] = y;
    }
  }
  {
    // v3
    {
      const float3 x = zivc::makeFloat3(2.0f, -2.0f, 0.0f);
      const float3 y = zivc::sign(x);
      out_i3[0] = y;
    }
  }
  {
    // v4
    {
      const float4 x = zivc::makeFloat4(2.0f, -2.0f, 0.0f, LimitT::quietNan());
      const float4 y = zivc::sign(x);
      out_i4[0] = y;
    }
  }
  {
    // v8
    {
      const float8 x = zivc::makeFloat8(
          2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, LimitT::quietNan());
      const float8 y = zivc::sign(x);
      out_i8[0] = y;
    }
  }
  {
    // v16
    {
      const float16 x = zivc::makeFloat16(
          2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, LimitT::quietNan(),
          2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, LimitT::quietNan());
      const float16 y = zivc::sign(x);
      out_i16[0] = y;
    }
  }
}

__kernel void utilitySmoothstepFloatTest(zivc::GlobalPtr<float> out_i1,
                                         zivc::GlobalPtr<float2> out_i2,
                                         zivc::GlobalPtr<float3> out_i3,
                                         zivc::GlobalPtr<float4> out_i4,
                                         zivc::GlobalPtr<float8> out_i8,
                                         zivc::GlobalPtr<float16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  constexpr float minf = LimitT::min();
  const float inf = LimitT::infinity();
  // smoothstep
  {
    // smoothstep v1
    constexpr float lo = 0.0f;
    constexpr float hi = 10.0f;
    {
      constexpr float x = 1.5f;
      const float y = zivc::smoothstep(lo, hi, x);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.5f;
      const float y = zivc::smoothstep(lo, hi, x);
      out_i1[1] = y;
    }
    {
      constexpr float x = 20.5f;
      const float y = zivc::smoothstep(lo, hi, x);
      out_i1[2] = y;
    }
  }
  {
    // smoothstep v2
    constexpr float2 lo = zivc::makeFloat2(0.0f);
    constexpr float2 hi = zivc::makeFloat2(10.0f);
    {
      constexpr float2 x = zivc::makeFloat2(1.5f, -1.5f);
      const float2 y = zivc::smoothstep(lo, hi, x);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(20.5f, -20.5f);
      const float2 y = zivc::smoothstep(lo, hi, x);
      out_i2[1] = y;
    }
  }
  {
    // smoothstep v3
    constexpr float3 lo = zivc::makeFloat3(0.0f);
    constexpr float3 hi = zivc::makeFloat3(10.0f);
    {
      constexpr float3 x = zivc::makeFloat3(1.5f, -1.5f, 20.0f);
      const float3 y = zivc::smoothstep(lo, hi, x);
      out_i3[0] = y;
    }
  }
  {
    // smoothstep v4
    constexpr float4 lo = zivc::makeFloat4(0.0f);
    constexpr float4 hi = zivc::makeFloat4(10.0f);
    {
      constexpr float4 x = zivc::makeFloat4(1.5f, -1.5f, maxf, -maxf);
      const float4 y = zivc::smoothstep(lo, hi, x);
      out_i4[0] = y;
    }
  }
  {
    // smoothstep v8
    constexpr float8 lo = zivc::makeFloat8(0.0f);
    constexpr float8 hi = zivc::makeFloat8(10.0f);
    {
      const float8 x = zivc::makeFloat8(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float8 y = zivc::smoothstep(lo, hi, x);
      out_i8[0] = y;
    }
  }
  {
    // smoothstep v16
    constexpr float16 lo = zivc::makeFloat16(0.0f);
    constexpr float16 hi = zivc::makeFloat16(10.0f);
    {
      const float16 x = zivc::makeFloat16(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                                          1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float16 y = zivc::smoothstep(lo, hi, x);
      out_i16[0] = y;
    }
  }
}

__kernel void utilityStepFloatTest(zivc::GlobalPtr<float> out_i1,
                                   zivc::GlobalPtr<float2> out_i2,
                                   zivc::GlobalPtr<float3> out_i3,
                                   zivc::GlobalPtr<float4> out_i4,
                                   zivc::GlobalPtr<float8> out_i8,
                                   zivc::GlobalPtr<float16> out_i16)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  using LimitT = zivc::NumericLimits<float>;
  constexpr float maxf = LimitT::max();
  constexpr float minf = LimitT::min();
  const float inf = LimitT::infinity();
  // smoothstep
  {
    // smoothstep v1
    constexpr float hi = 10.0f;
    {
      constexpr float x = 1.5f;
      const float y = zivc::step(hi, x);
      out_i1[0] = y;
    }
    {
      constexpr float x = -1.5f;
      const float y = zivc::step(hi, x);
      out_i1[1] = y;
    }
    {
      constexpr float x = 20.5f;
      const float y = zivc::step(hi, x);
      out_i1[2] = y;
    }
  }
  {
    // smoothstep v2
    constexpr float2 hi = zivc::makeFloat2(10.0f);
    {
      constexpr float2 x = zivc::makeFloat2(1.5f, -1.5f);
      const float2 y = zivc::step(hi, x);
      out_i2[0] = y;
    }
    {
      constexpr float2 x = zivc::makeFloat2(20.5f, -20.5f);
      const float2 y = zivc::step(hi, x);
      out_i2[1] = y;
    }
  }
  {
    // smoothstep v3
    constexpr float3 hi = zivc::makeFloat3(10.0f);
    {
      constexpr float3 x = zivc::makeFloat3(1.5f, -1.5f, 20.0f);
      const float3 y = zivc::step(hi, x);
      out_i3[0] = y;
    }
  }
  {
    // smoothstep v4
    constexpr float4 hi = zivc::makeFloat4(10.0f);
    {
      constexpr float4 x = zivc::makeFloat4(1.5f, -1.5f, maxf, -maxf);
      const float4 y = zivc::step(hi, x);
      out_i4[0] = y;
    }
  }
  {
    // smoothstep v8
    constexpr float8 hi = zivc::makeFloat8(10.0f);
    {
      const float8 x = zivc::makeFloat8(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float8 y = zivc::step(hi, x);
      out_i8[0] = y;
    }
  }
  {
    // smoothstep v16
    constexpr float16 hi = zivc::makeFloat16(10.0f);
    {
      const float16 x = zivc::makeFloat16(1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                                          1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf);
      const float16 y = zivc::step(hi, x);
      out_i16[0] = y;
    }
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST1_CL */
