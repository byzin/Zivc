/*!
  \file opencl_cpp_test_type.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_TYPE_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_TYPE_CL

// Zivc
#include "zivc/cl/bit.hpp"
#include "zivc/cl/boolean.hpp"
#include "zivc/cl/functional.hpp"
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

/*!
  \def ZIVC_CHECK_TYPE_SIZE
  \brief No brief description

  No detailed description.

  \param [in] type No description.
  \param [in] expected_size No description.
  */
#define ZIVC_CHECK_TYPE_SIZE(type, expected_size) \
  static_assert(sizeof( type ) == expected_size , \
                "The size of '" #type "' isn't correct."); \
  static_assert(alignof( type ) == expected_size , \
                "The alignment of '" #type "' isn't correct.")

ZIVC_CHECK_TYPE_SIZE(signed char, 1);
ZIVC_CHECK_TYPE_SIZE(char, 1);
ZIVC_CHECK_TYPE_SIZE(zivc::int8b, 1);
ZIVC_CHECK_TYPE_SIZE(char2, 2);
ZIVC_CHECK_TYPE_SIZE(char3, 4);
ZIVC_CHECK_TYPE_SIZE(char4, 4);
ZIVC_CHECK_TYPE_SIZE(char8, 8);
ZIVC_CHECK_TYPE_SIZE(char16, 16);
ZIVC_CHECK_TYPE_SIZE(unsigned char, 1);
ZIVC_CHECK_TYPE_SIZE(uchar, 1);
ZIVC_CHECK_TYPE_SIZE(zivc::uint8b, 1);
ZIVC_CHECK_TYPE_SIZE(uchar2, 2);
ZIVC_CHECK_TYPE_SIZE(uchar3, 4);
ZIVC_CHECK_TYPE_SIZE(uchar4, 4);
ZIVC_CHECK_TYPE_SIZE(uchar8, 8);
ZIVC_CHECK_TYPE_SIZE(uchar16, 16);
ZIVC_CHECK_TYPE_SIZE(signed short, 2);
ZIVC_CHECK_TYPE_SIZE(short, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::int16b, 2);
ZIVC_CHECK_TYPE_SIZE(short2, 4);
ZIVC_CHECK_TYPE_SIZE(short3, 8);
ZIVC_CHECK_TYPE_SIZE(short4, 8);
ZIVC_CHECK_TYPE_SIZE(short8, 16);
ZIVC_CHECK_TYPE_SIZE(short16, 32);
ZIVC_CHECK_TYPE_SIZE(unsigned short, 2);
ZIVC_CHECK_TYPE_SIZE(ushort, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::uint16b, 2);
ZIVC_CHECK_TYPE_SIZE(ushort2, 4);
ZIVC_CHECK_TYPE_SIZE(ushort3, 8);
ZIVC_CHECK_TYPE_SIZE(ushort4, 8);
ZIVC_CHECK_TYPE_SIZE(ushort8, 16);
ZIVC_CHECK_TYPE_SIZE(ushort16, 32);
ZIVC_CHECK_TYPE_SIZE(signed int, 4);
ZIVC_CHECK_TYPE_SIZE(int, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::int32b, 4);
ZIVC_CHECK_TYPE_SIZE(int2, 8);
ZIVC_CHECK_TYPE_SIZE(int3, 16);
ZIVC_CHECK_TYPE_SIZE(int4, 16);
ZIVC_CHECK_TYPE_SIZE(int8, 32);
ZIVC_CHECK_TYPE_SIZE(int16, 64);
ZIVC_CHECK_TYPE_SIZE(unsigned int, 4);
ZIVC_CHECK_TYPE_SIZE(uint, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::uint32b, 4);
ZIVC_CHECK_TYPE_SIZE(uint2, 8);
ZIVC_CHECK_TYPE_SIZE(uint3, 16);
ZIVC_CHECK_TYPE_SIZE(uint4, 16);
ZIVC_CHECK_TYPE_SIZE(uint8, 32);
ZIVC_CHECK_TYPE_SIZE(uint16, 64);
//ZIVC_CHECK_TYPE_SIZE(signed long, 8); //!< \note long is 4 bytes in msvc
//ZIVC_CHECK_TYPE_SIZE(long, 8); //!< \note long is 4 bytes in msvc
ZIVC_CHECK_TYPE_SIZE(zivc::int64b, 8);
ZIVC_CHECK_TYPE_SIZE(long2, 16);
ZIVC_CHECK_TYPE_SIZE(long3, 32);
ZIVC_CHECK_TYPE_SIZE(long4, 32);
ZIVC_CHECK_TYPE_SIZE(long8, 64);
//ZIVC_CHECK_TYPE_SIZE(long16, 128); //!< \note MSVC doesn't support 128 byte alignment
//ZIVC_CHECK_TYPE_SIZE(unsigned long, 8); //!< \note long is 4 bytes in msvc
ZIVC_CHECK_TYPE_SIZE(ulong, 8);
ZIVC_CHECK_TYPE_SIZE(zivc::uint64b, 8);
ZIVC_CHECK_TYPE_SIZE(ulong2, 16);
ZIVC_CHECK_TYPE_SIZE(ulong3, 32);
ZIVC_CHECK_TYPE_SIZE(ulong4, 32);
ZIVC_CHECK_TYPE_SIZE(ulong8, 64);
//ZIVC_CHECK_TYPE_SIZE(ulong16, 128); //!< \note MSVC doesn't support 128 byte alignment
ZIVC_CHECK_TYPE_SIZE(half, 2);
ZIVC_CHECK_TYPE_SIZE(half2, 4);
ZIVC_CHECK_TYPE_SIZE(half3, 8);
ZIVC_CHECK_TYPE_SIZE(half4, 8);
ZIVC_CHECK_TYPE_SIZE(half8, 16);
ZIVC_CHECK_TYPE_SIZE(half16, 32);
ZIVC_CHECK_TYPE_SIZE(float, 4);
ZIVC_CHECK_TYPE_SIZE(float2, 8);
ZIVC_CHECK_TYPE_SIZE(float3, 16);
ZIVC_CHECK_TYPE_SIZE(float4, 16);
ZIVC_CHECK_TYPE_SIZE(float8, 32);
ZIVC_CHECK_TYPE_SIZE(float16, 64);
ZIVC_CHECK_TYPE_SIZE(double, 8);
ZIVC_CHECK_TYPE_SIZE(double2, 16);
ZIVC_CHECK_TYPE_SIZE(double3, 32);
ZIVC_CHECK_TYPE_SIZE(double4, 32);
ZIVC_CHECK_TYPE_SIZE(double8, 64);
//ZIVC_CHECK_TYPE_SIZE(double16, 128); //!< \note MSVC doesn't support 128 byte alignment

ZIVC_CHECK_TYPE_SIZE(zivc::GlobalPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::ConstGlobalPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::LocalPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::ConstLocalPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::ConstantPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::PrivatePtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::ConstPrivatePtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::GenericPtr<int>, sizeof(void*));
ZIVC_CHECK_TYPE_SIZE(zivc::ConstGenericPtr<int>, sizeof(void*));

__kernel void vectorComponentAccessKernel(zivc::ConstGlobalPtr<float2> in_vec2,
                                          zivc::ConstGlobalPtr<float3> in_vec3,
                                          zivc::ConstGlobalPtr<float4> in_vec4,
                                          zivc::GlobalPtr<float> output1,
                                          zivc::GlobalPtr<float2> output2,
                                          zivc::GlobalPtr<float3> output3,
                                          zivc::GlobalPtr<float4> output4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  // Read value from vector
  size_t index = 0;
  {
    const float2 v0 = in_vec2[0];
    output1[index++] = v0.x;
    output1[index++] = v0.y;
    const zivc::ConstGlobalPtr<float2> v1 = &in_vec2[1];
    output1[index++] = v1->x;
    output1[index++] = v1->y;
  }
  {
    const float3 v0 = in_vec3[0];
    output1[index++] = v0.x;
    output1[index++] = v0.y;
    output1[index++] = v0.z;
    const zivc::ConstGlobalPtr<float3> v1 = &in_vec3[1];
    output1[index++] = v1->x;
    output1[index++] = v1->y;
    output1[index++] = v1->z;
  }
  {
    const float4 v0 = in_vec4[0];
    output1[index++] = v0.x;
    output1[index++] = v0.y;
    output1[index++] = v0.z;
    output1[index++] = v0.w;
    const zivc::ConstGlobalPtr<float4> v1 = &in_vec4[1];
    output1[index++] = v1->x;
    output1[index++] = v1->y;
    output1[index++] = v1->z;
    output1[index++] = v1->w;
  }
  // Write value into vector
  {
    float2 v0{0.0f, 0.0f};
    v0.x = 1.0f;
    v0.y = 2.0f;
    output2[0] = v0;
    zivc::GlobalPtr<float2> v1 = &output2[1];
    v1->x = 3.0f;
    v1->y = 4.0f;
  }
  {
    float3 v0{0.0f, 0.0f, 0.0f};
    v0.x = 1.0f;
    v0.y = 2.0f;
    v0.z = 3.0f;
    output3[0] = v0;
    zivc::GlobalPtr<float3> v1 = &output3[1];
    v1->x = 4.0f;
    v1->y = 5.0f;
    v1->z = 6.0f;
  }
  {
    float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
    v0.x = 1.0f;
    v0.y = 2.0f;
    v0.z = 3.0f;
    v0.w = 4.0f;
    output4[0] = v0;
    zivc::GlobalPtr<float4> v1 = &output4[1];
    v1->x = 5.0f;
    v1->y = 6.0f;
    v1->z = 7.0f;
    v1->w = 8.0f;
  }
}

__kernel void vectorComponentAccessLongVecKernel(zivc::ConstGlobalPtr<float8> in_vec8,
                                                 zivc::ConstGlobalPtr<float16> in_vec16,
                                                 zivc::GlobalPtr<float> output1,
                                                 zivc::GlobalPtr<float8> output8,
                                                 zivc::GlobalPtr<float16> output16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  // Read value from vector
  size_t index = 0;
  {
    const float8 v0 = in_vec8[0];
    output1[index++] = v0.s0;
    output1[index++] = v0.s1;
    output1[index++] = v0.s2;
    output1[index++] = v0.s3;
    output1[index++] = v0.s4;
    output1[index++] = v0.s5;
    output1[index++] = v0.s6;
    output1[index++] = v0.s7;
    const zivc::ConstGlobalPtr<float8> v1 = &in_vec8[1];
    output1[index++] = v1->s0;
    output1[index++] = v1->s1;
    output1[index++] = v1->s2;
    output1[index++] = v1->s3;
    output1[index++] = v1->s4;
    output1[index++] = v1->s5;
    output1[index++] = v1->s6;
    output1[index++] = v1->s7;
  }
  {
    const float16 v0 = in_vec16[0];
    output1[index++] = v0.s0;
    output1[index++] = v0.s1;
    output1[index++] = v0.s2;
    output1[index++] = v0.s3;
    output1[index++] = v0.s4;
    output1[index++] = v0.s5;
    output1[index++] = v0.s6;
    output1[index++] = v0.s7;
    output1[index++] = v0.s8;
    output1[index++] = v0.s9;
    output1[index++] = v0.sa;
    output1[index++] = v0.sb;
    output1[index++] = v0.sc;
    output1[index++] = v0.sd;
    output1[index++] = v0.se;
    output1[index++] = v0.sf;
    const zivc::ConstGlobalPtr<float16> v1 = &in_vec16[1];
    output1[index++] = v1->s0;
    output1[index++] = v1->s1;
    output1[index++] = v1->s2;
    output1[index++] = v1->s3;
    output1[index++] = v1->s4;
    output1[index++] = v1->s5;
    output1[index++] = v1->s6;
    output1[index++] = v1->s7;
    output1[index++] = v1->s8;
    output1[index++] = v1->s9;
    output1[index++] = v1->sa;
    output1[index++] = v1->sb;
    output1[index++] = v1->sc;
    output1[index++] = v1->sd;
    output1[index++] = v1->se;
    output1[index++] = v1->sf;
  }
  // Write value into vector
  {
    float8 v0{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v0.s0 = 1.0f;
    v0.s1 = 2.0f;
    v0.s2 = 3.0f;
    v0.s3 = 4.0f;
    v0.s4 = 5.0f;
    v0.s5 = 6.0f;
    v0.s6 = 7.0f;
    v0.s7 = 8.0f;
    output8[0] = v0;
    zivc::GlobalPtr<float8> v1 = &output8[1];
    v1->s0 = 9.0f;
    v1->s1 = 10.0f;
    v1->s2 = 11.0f;
    v1->s3 = 12.0f;
    v1->s4 = 13.0f;
    v1->s5 = 14.0f;
    v1->s6 = 15.0f;
    v1->s7 = 16.0f;
  }
  {
    float16 v0{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v0.s0 = 1.0f;
    v0.s1 = 2.0f;
    v0.s2 = 3.0f;
    v0.s3 = 4.0f;
    v0.s4 = 5.0f;
    v0.s5 = 6.0f;
    v0.s6 = 7.0f;
    v0.s7 = 8.0f;
    v0.s8 = 9.0f;
    v0.s9 = 10.0f;
    v0.sa = 11.0f;
    v0.sb = 12.0f;
    v0.sc = 13.0f;
    v0.sd = 14.0f;
    v0.se = 15.0f;
    v0.sf = 16.0f;
    output16[0] = v0;
    zivc::GlobalPtr<float16> v1 = &output16[1];
    v1->s0 = 17.0f;
    v1->s1 = 18.0f;
    v1->s2 = 19.0f;
    v1->s3 = 20.0f;
    v1->s4 = 21.0f;
    v1->s5 = 22.0f;
    v1->s6 = 23.0f;
    v1->s7 = 24.0f;
    v1->s8 = 25.0f;
    v1->s9 = 26.0f;
    v1->sa = 27.0f;
    v1->sb = 28.0f;
    v1->sc = 29.0f;
    v1->sd = 30.0f;
    v1->se = 31.0f;
    v1->sf = 32.0f;
  }
}

__kernel void vectorConstructionKernel(zivc::GlobalPtr<int2> output_vec2,
                                       zivc::GlobalPtr<int3> output_vec3,
                                       zivc::GlobalPtr<int4> output_vec4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t index = 0;
  // vector2
  {
    const int2 v = zivc::makeInt2(0, 1);
    output_vec2[index++] = v;
  }
  {
    const int2 v = zivc::makeInt2(2);
    output_vec2[index++] = v;
  }
  {
    const int2 s = zivc::makeInt2(3, 4);
    const int2 v{s};
    output_vec2[index++] = v;
  }

  index = 0;
  // vector3
  {
    const int3 v = zivc::makeInt3(0, 1, 2);
    output_vec3[index++] = v;
  }
  {
    const int2 s{3, 4};
    const int3 v0 = zivc::makeInt3(s, 5);
    output_vec3[index++] = v0;
    const int3 v1 = zivc::makeInt3(5, s);
    output_vec3[index++] = v1;
  }
  {
    const int3 v = zivc::makeInt3(6);
    output_vec3[index++] = v;
  }
  {
    const int3 s = zivc::makeInt3(7, 8, 9);
    const int3 v{s};
    output_vec3[index++] = v;
  }

  index = 0;
  // vector4
  {
    const int4 v = zivc::makeInt4(0, 1, 2, 3);
    output_vec4[index++] = v;
  }
  {
    const int2 s{4, 5};
    const int4 v0 = zivc::makeInt4(s, 6, 7);
    output_vec4[index++] = v0;
    const int4 v1 = zivc::makeInt4(6, s, 7);
    output_vec4[index++] = v1;
    const int4 v2 = zivc::makeInt4(6, 7, s);
    output_vec4[index++] = v2;
    const int4 v3 = zivc::makeInt4(s, s);
    output_vec4[index++] = v3;
  }
  {
    const int3 s{8, 9, 10};
    const int4 v0 = zivc::makeInt4(s, 11);
    output_vec4[index++] = v0;
    const int4 v1 = zivc::makeInt4(11, s);
    output_vec4[index++] = v1;
  }
  {
    const int4 v = zivc::makeInt4(12);
    output_vec4[index++] = v;
  }
  {
    const int4 s = zivc::makeInt4(13, 14, 15, 16);
    const int4 v{s};
    output_vec4[index++] = v;
  }

  //
#define ZIVC_CHECK_MAKING_VECTOR_TYPE(vector_type, vector_func, v0, v1, v2, v3) \
  { \
    using VectorType = zivc::Private< vector_type ## 2 >; \
    { \
      auto v = zivc::make ## vector_func ## 2( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 2( v0, v1 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
  } \
  { \
    using Vec2 = vector_type ## 2; \
    using VectorType = zivc::Private< vector_type ## 3 >; \
    { \
      auto v = zivc::make ## vector_func ## 3( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 3( v0, v1, v2 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 3( Vec2{v0, v1}, v2 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 3( v0, Vec2{v1, v2} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
  } \
  { \
    using Vec2 = vector_type ## 2; \
    using Vec3 = vector_type ## 3; \
    using VectorType = zivc::Private< vector_type ## 4 >; \
    { \
      auto v = zivc::make ## vector_func ## 4( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( v0, v1, v2, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( Vec2{v0, v1}, v2, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( v0, Vec2{v1, v2}, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( v0, v1, Vec2{v2, v3} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( Vec2{v0, v1}, Vec2{v2, v3} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( Vec3{v0, v1, v2}, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make ## vector_func ## 4( v0, Vec3{v1, v2, v3} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
  }

  const auto c = [](const zivc::int32b i) noexcept
  {
    return static_cast<zivc::int8b>(i);
  };
  const auto uc = [](const zivc::uint32b u) noexcept
  {
    return static_cast<zivc::uint8b>(u);
  };
  const auto s = [](const zivc::int32b i) noexcept
  {
    return static_cast<zivc::int16b>(i);
  };
  const auto us = [](const zivc::uint32b u) noexcept
  {
    return static_cast<zivc::uint16b>(u);
  };
  ZIVC_CHECK_MAKING_VECTOR_TYPE(char, Char, c(0), c(1), c(2), c(3))
  ZIVC_CHECK_MAKING_VECTOR_TYPE(uchar, UChar, uc(0u), uc(1u), uc(2u), uc(3u))
  ZIVC_CHECK_MAKING_VECTOR_TYPE(short, Short, s(0), s(1), s(2), s(3))
  ZIVC_CHECK_MAKING_VECTOR_TYPE(ushort, UShort, us(0u), us(1u), us(2u), us(3u))
  ZIVC_CHECK_MAKING_VECTOR_TYPE(int, Int, 0, 1, 2, 3)
  ZIVC_CHECK_MAKING_VECTOR_TYPE(uint, UInt, 0u, 1u, 2u, 3u)
  ZIVC_CHECK_MAKING_VECTOR_TYPE(long, Long, 0l, 1l, 2l, 3l)
  ZIVC_CHECK_MAKING_VECTOR_TYPE(ulong, ULong, 0ul, 1ul, 2ul, 3ul)
  ZIVC_CHECK_MAKING_VECTOR_TYPE(float, Float, 0.0f, 1.0f, 2.0f, 3.0f)
  ZIVC_CHECK_MAKING_VECTOR_TYPE(double, Double, 0.0, 1.0, 2.0, 3.0)
}

__kernel void vectorArithmeticOperatorTest(
    zivc::ConstGlobalPtr<int2> in_int2,
    zivc::ConstGlobalPtr<int3> in_int3,
    zivc::ConstGlobalPtr<int4> in_int4,
    zivc::ConstGlobalPtr<float2> in_float2,
    zivc::ConstGlobalPtr<float3> in_float3,
    zivc::ConstGlobalPtr<float4> in_float4,
    zivc::GlobalPtr<int2> out_int2,
    zivc::GlobalPtr<int3> out_int3,
    zivc::GlobalPtr<int4> out_int4,
    zivc::GlobalPtr<float2> out_float2,
    zivc::GlobalPtr<float3> out_float3,
    zivc::GlobalPtr<float4> out_float4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // Unary plus
  {
    const int2 v = in_int2[iindex];
    out_int2[oindex] = +v;
  }
  {
    const int3 v = in_int3[iindex];
    out_int3[oindex] = +v;
  }
  {
    const int4 v = in_int4[iindex];
    out_int4[oindex] = +v;
  }
  {
    const float2 v = in_float2[iindex];
    out_float2[oindex] = +v;
  }
  {
    const float3 v = in_float3[iindex];
    out_float3[oindex] = +v;
  }
  {
    const float4 v = in_float4[iindex];
    out_float4[oindex] = +v;
  }
  ++oindex;
  // Unary minus
  {
    out_int2[oindex] = zivc::Negate<int2>{}(in_int2[iindex]);
  }
  {
    out_int3[oindex] = zivc::Negate<int3>{}(in_int3[iindex]);
  }
  {
    out_int4[oindex] = zivc::Negate<int4>{}(in_int4[iindex]);
  }
  {
    out_float2[oindex] = -in_float2[iindex];
  }
  {
    out_float3[oindex] = -in_float3[iindex];
  }
  {
    out_float4[oindex] = -in_float4[iindex];
  }
  ++oindex;
  // Addition
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = zivc::Plus<int2>{}(v0, v1);
    out_int2[oindex + 1] = v0.x + v1;
    out_int2[oindex + 2] = v0 + v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = zivc::Plus<int3>{}(v0, v1);
    out_int3[oindex + 1] = v0.x + v1;
    out_int3[oindex + 2] = v0 + v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = zivc::Plus<int4>{}(v0, v1);
    out_int4[oindex + 1] = v0.x + v1;
    out_int4[oindex + 2] = v0 + v1.x;
  }
  {
    const float2 v0 = in_float2[iindex];
    const float2 v1 = in_float2[iindex + 1];
    out_float2[oindex] = v0 + v1;
    out_float2[oindex + 1] = v0.x + v1;
    out_float2[oindex + 2] = v0 + v1.x;
  }
  {
    const float3 v0 = in_float3[iindex];
    const float3 v1 = in_float3[iindex + 1];
    out_float3[oindex] = v0 + v1;
    out_float3[oindex + 1] = v0.x + v1;
    out_float3[oindex + 2] = v0 + v1.x;
  }
  {
    const float4 v0 = in_float4[iindex];
    const float4 v1 = in_float4[iindex + 1];
    out_float4[oindex] = v0 + v1;
    out_float4[oindex + 1] = v0.x + v1;
    out_float4[oindex + 2] = v0 + v1.x;
  }
  oindex += 3;
  // Subtraction
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = zivc::Minus<int2>{}(v0, v1);
    out_int2[oindex + 1] = v0.x - v1;
    out_int2[oindex + 2] = v0 - v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = zivc::Minus<int3>{}(v0, v1);
    out_int3[oindex + 1] = v0.x - v1;
    out_int3[oindex + 2] = v0 - v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = zivc::Minus<int4>{}(v0, v1);
    out_int4[oindex + 1] = v0.x - v1;
    out_int4[oindex + 2] = v0 - v1.x;
  }
  {
    const float2 v0 = in_float2[iindex];
    const float2 v1 = in_float2[iindex + 1];
    out_float2[oindex] = v0 - v1;
    out_float2[oindex + 1] = v0.x - v1;
    out_float2[oindex + 2] = v0 - v1.x;
  }
  {
    const float3 v0 = in_float3[iindex];
    const float3 v1 = in_float3[iindex + 1];
    out_float3[oindex] = v0 - v1;
    out_float3[oindex + 1] = v0.x - v1;
    out_float3[oindex + 2] = v0 - v1.x;
  }
  {
    const float4 v0 = in_float4[iindex];
    const float4 v1 = in_float4[iindex + 1];
    out_float4[oindex] = v0 - v1;
    out_float4[oindex + 1] = v0.x - v1;
    out_float4[oindex + 2] = v0 - v1.x;
  }
  oindex += 3;
  // Multiplication
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = zivc::Multiply<int2>{}(v0, v1);
    out_int2[oindex + 1] = v0.x * v1;
    out_int2[oindex + 2] = v0 * v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = zivc::Multiply<int3>{}(v0, v1);
    out_int3[oindex + 1] = v0.x * v1;
    out_int3[oindex + 2] = v0 * v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = zivc::Multiply<int4>{}(v0, v1);
    out_int4[oindex + 1] = v0.x * v1;
    out_int4[oindex + 2] = v0 * v1.x;
  }
  {
    const float2 v0 = in_float2[iindex];
    const float2 v1 = in_float2[iindex + 1];
    out_float2[oindex] = v0 * v1;
    out_float2[oindex + 1] = v0.x * v1;
    out_float2[oindex + 2] = v0 * v1.x;
  }
  {
    const float3 v0 = in_float3[iindex];
    const float3 v1 = in_float3[iindex + 1];
    out_float3[oindex] = v0 * v1;
    out_float3[oindex + 1] = v0.x * v1;
    out_float3[oindex + 2] = v0 * v1.x;
  }
  {
    const float4 v0 = in_float4[iindex];
    const float4 v1 = in_float4[iindex + 1];
    out_float4[oindex] = v0 * v1;
    out_float4[oindex + 1] = v0.x * v1;
    out_float4[oindex + 2] = v0 * v1.x;
  }
  oindex += 3;
  // Division
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = zivc::Divide<int2>{}(v0, v1);
    out_int2[oindex + 1] = v0.x / v1;
    out_int2[oindex + 2] = v0 / v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = zivc::Divide<int3>{}(v0, v1);
    out_int3[oindex + 1] = v0.x / v1;
    out_int3[oindex + 2] = v0 / v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = zivc::Divide<int4>{}(v0, v1);
    out_int4[oindex + 1] = v0.x / v1;
    out_int4[oindex + 2] = v0 / v1.x;
  }
  {
    const float2 v0 = in_float2[iindex];
    const float2 v1 = in_float2[iindex + 1];
    out_float2[oindex] = v0 / v1;
    out_float2[oindex + 1] = v0.x / v1;
    out_float2[oindex + 2] = v0 / v1.x;
  }
  {
    const float3 v0 = in_float3[iindex];
    const float3 v1 = in_float3[iindex + 1];
    out_float3[oindex] = v0 / v1;
    out_float3[oindex + 1] = v0.x / v1;
    out_float3[oindex + 2] = v0 / v1.x;
  }
  {
    const float4 v0 = in_float4[iindex];
    const float4 v1 = in_float4[iindex + 1];
    out_float4[oindex] = v0 / v1;
    out_float4[oindex + 1] = v0.x / v1;
    out_float4[oindex + 2] = v0 / v1.x;
  }
  iindex += 2;
  oindex += 3;
  // Modulo
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = zivc::Modulu<int2>{}(v0, v1);
    out_int2[oindex + 1] = v0.x % v1;
    out_int2[oindex + 2] = v0 % v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = zivc::Modulu<int3>{}(v0, v1);
    out_int3[oindex + 1] = v0.x % v1;
    out_int3[oindex + 2] = v0 % v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = zivc::Modulu<int4>{}(v0, v1);
    out_int4[oindex + 1] = v0.x % v1;
    out_int4[oindex + 2] = v0 % v1.x;
  }

  // Constexpr test
  {
    constexpr float4 v = zivc::makeFloat4(0.0f, 1.0f, 2.0f, 3.0f);
    constexpr float4 r0 = +v;
    out_float4[oindex++] = r0;
    constexpr float4 r1 = -v;
    out_float4[oindex++] = r1;
  }
  {
    constexpr float4 v0 = zivc::makeFloat4(0.0f, 1.0f, 2.0f, 3.0f);
    constexpr float4 v1 = zivc::makeFloat4(4.0f, 5.0f, 6.0f, 7.0f);
    constexpr float4 r0 = zivc::Plus<float4>{}(v0, v1);
    out_float4[oindex++] = r0;
    constexpr float4 r1 = zivc::Minus<float4>{}(v0, v1);
    out_float4[oindex++] = r1;
    constexpr float4 r2 = zivc::Multiply<float4>{}(v0, v1);
    out_float4[oindex++] = r2;
    constexpr float4 r3 = zivc::Divide<float4>{}(v0, v1);
    out_float4[oindex++] = r3;
  }
}

__kernel void vectorArithmeticOperatorLongVecTest(
    zivc::ConstGlobalPtr<int8> in_int8,
    zivc::ConstGlobalPtr<int16> in_int16,
    zivc::ConstGlobalPtr<float8> in_float8,
    zivc::ConstGlobalPtr<float16> in_float16,
    zivc::GlobalPtr<int8> out_int8,
    zivc::GlobalPtr<int16> out_int16,
    zivc::GlobalPtr<float8> out_float8,
    zivc::GlobalPtr<float16> out_float16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // Unary plus
  {
    const int8 v = in_int8[iindex];
    out_int8[oindex] = +v;
  }
  {
    const int16 v = in_int16[iindex];
    out_int16[oindex] = +v;
  }
  {
    const float8 v = in_float8[iindex];
    out_float8[oindex] = +v;
  }
  {
    const float16 v = in_float16[iindex];
    out_float16[oindex] = +v;
  }
  ++oindex;
  // Unary minus
  {
    out_int8[oindex] = zivc::Negate<int8>{}(in_int8[iindex]);
  }
  {
    out_int16[oindex] = zivc::Negate<int16>{}(in_int16[iindex]);
  }
  {
    out_float8[oindex] = -in_float8[iindex];
  }
  {
    out_float16[oindex] = -in_float16[iindex];
  }
  ++oindex;
  // Addition
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = zivc::Plus<int8>{}(v0, v1);
    out_int8[oindex + 1] = v0.s0 + v1;
    out_int8[oindex + 2] = v0 + v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = zivc::Plus<int16>{}(v0, v1);
    out_int16[oindex + 1] = v0.s0 + v1;
    out_int16[oindex + 2] = v0 + v1.s0;
  }
  {
    const float8 v0 = in_float8[iindex];
    const float8 v1 = in_float8[iindex + 1];
    out_float8[oindex] = v0 + v1;
    out_float8[oindex + 1] = v0.s0 + v1;
    out_float8[oindex + 2] = v0 + v1.s0;
  }
  {
    const float16 v0 = in_float16[iindex];
    const float16 v1 = in_float16[iindex + 1];
    out_float16[oindex] = v0 + v1;
    out_float16[oindex + 1] = v0.s0 + v1;
    out_float16[oindex + 2] = v0 + v1.s0;
  }
  oindex += 3;
  // Subtraction
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = zivc::Minus<int8>{}(v0, v1);
    out_int8[oindex + 1] = v0.s0 - v1;
    out_int8[oindex + 2] = v0 - v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = zivc::Minus<int16>{}(v0, v1);
    out_int16[oindex + 1] = v0.s0 - v1;
    out_int16[oindex + 2] = v0 - v1.s0;
  }
  {
    const float8 v0 = in_float8[iindex];
    const float8 v1 = in_float8[iindex + 1];
    out_float8[oindex] = v0 - v1;
    out_float8[oindex + 1] = v0.s0 - v1;
    out_float8[oindex + 2] = v0 - v1.s0;
  }
  {
    const float16 v0 = in_float16[iindex];
    const float16 v1 = in_float16[iindex + 1];
    out_float16[oindex] = v0 - v1;
    out_float16[oindex + 1] = v0.s0 - v1;
    out_float16[oindex + 2] = v0 - v1.s0;
  }
  oindex += 3;
  // Multiplication
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = zivc::Multiply<int8>{}(v0, v1);
    out_int8[oindex + 1] = v0.s0 * v1;
    out_int8[oindex + 2] = v0 * v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = zivc::Multiply<int16>{}(v0, v1);
    out_int16[oindex + 1] = v0.s0 * v1;
    out_int16[oindex + 2] = v0 * v1.s0;
  }
  {
    const float8 v0 = in_float8[iindex];
    const float8 v1 = in_float8[iindex + 1];
    out_float8[oindex] = v0 * v1;
    out_float8[oindex + 1] = v0.s0 * v1;
    out_float8[oindex + 2] = v0 * v1.s0;
  }
  {
    const float16 v0 = in_float16[iindex];
    const float16 v1 = in_float16[iindex + 1];
    out_float16[oindex] = v0 * v1;
    out_float16[oindex + 1] = v0.s0 * v1;
    out_float16[oindex + 2] = v0 * v1.s0;
  }
  oindex += 3;
  // Division
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = zivc::Divide<int8>{}(v0, v1);
    out_int8[oindex + 1] = v0.s0 / v1;
    out_int8[oindex + 2] = v0 / v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = zivc::Divide<int16>{}(v0, v1);
    out_int16[oindex + 1] = v0.s0 / v1;
    out_int16[oindex + 2] = v0 / v1.s0;
  }
  {
    const float8 v0 = in_float8[iindex];
    const float8 v1 = in_float8[iindex + 1];
    out_float8[oindex] = v0 / v1;
    out_float8[oindex + 1] = v0.s0 / v1;
    out_float8[oindex + 2] = v0 / v1.s0;
  }
  {
    const float16 v0 = in_float16[iindex];
    const float16 v1 = in_float16[iindex + 1];
    out_float16[oindex] = v0 / v1;
    out_float16[oindex + 1] = v0.s0 / v1;
    out_float16[oindex + 2] = v0 / v1.s0;
  }
  iindex += 2;
  oindex += 3;
  // Modulo
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = zivc::Modulu<int8>{}(v0, v1);
    out_int8[oindex + 1] = v0.s0 % v1;
    out_int8[oindex + 2] = v0 % v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = zivc::Modulu<int16>{}(v0, v1);
    out_int16[oindex + 1] = v0.s0 % v1;
    out_int16[oindex + 2] = v0 % v1.s0;
  }

  // Constexpr test
  {
    constexpr float8 v = zivc::makeFloat8(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
    constexpr float8 r0 = +v;
    out_float8[oindex++] = r0;
    constexpr float8 r1 = -v;
    out_float8[oindex++] = r1;
  }
  {
    constexpr float8 v0 = zivc::makeFloat8(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
    constexpr float8 v1 = zivc::makeFloat8(8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
    constexpr float8 r0 = zivc::Plus<float8>{}(v0, v1);
    out_float8[oindex++] = r0;
    constexpr float8 r1 = zivc::Minus<float8>{}(v0, v1);
    out_float8[oindex++] = r1;
    constexpr float8 r2 = zivc::Multiply<float8>{}(v0, v1);
    out_float8[oindex++] = r2;
    constexpr float8 r3 = zivc::Divide<float8>{}(v0, v1);
    out_float8[oindex++] = r3;
  }
}

__kernel void vectorArithmeticAssignmentOperatorTest(
    zivc::ConstGlobalPtr<int2> in_int2,
    zivc::ConstGlobalPtr<int3> in_int3,
    zivc::ConstGlobalPtr<int4> in_int4,
    zivc::GlobalPtr<int2> out_int2,
    zivc::GlobalPtr<int3> out_int3,
    zivc::GlobalPtr<int4> out_int4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // Addition
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = v0;
    out_int2[oindex] += v1;
    out_int2[oindex + 1] = v0;
    out_int2[oindex + 1] += v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0;
    out_int3[oindex] += v1;
    out_int3[oindex + 1] = v0;
    out_int3[oindex + 1] += v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0;
    out_int4[oindex] += v1;
    out_int4[oindex + 1] = v0;
    out_int4[oindex + 1] += v1.x;
  }
  oindex += 2;
  // Subtraction
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = v0;
    out_int2[oindex] -= v1;
    out_int2[oindex + 1] = v0;
    out_int2[oindex + 1] -= v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0;
    out_int3[oindex] -= v1;
    out_int3[oindex + 1] = v0;
    out_int3[oindex + 1] -= v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0;
    out_int4[oindex] -= v1;
    out_int4[oindex + 1] = v0;
    out_int4[oindex + 1] -= v1.x;
  }
  oindex += 2;
  // Multiplication
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = v0;
    out_int2[oindex] *= v1;
    out_int2[oindex + 1] = v0;
    out_int2[oindex + 1] *= v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0;
    out_int3[oindex] *= v1;
    out_int3[oindex + 1] = v0;
    out_int3[oindex + 1] *= v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0;
    out_int4[oindex] *= v1;
    out_int4[oindex + 1] = v0;
    out_int4[oindex + 1] *= v1.x;
  }
  oindex += 2;
  // Division
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = v0;
    out_int2[oindex] /= v1;
    out_int2[oindex + 1] = v0;
    out_int2[oindex + 1] /= v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0;
    out_int3[oindex] /= v1;
    out_int3[oindex + 1] = v0;
    out_int3[oindex + 1] /= v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0;
    out_int4[oindex] /= v1;
    out_int4[oindex + 1] = v0;
    out_int4[oindex + 1] /= v1.x;
  }
  iindex += 2;
  oindex += 2;
  // Modulo
  {
    const int2 v0 = in_int2[iindex];
    const int2 v1 = in_int2[iindex + 1];
    out_int2[oindex] = v0;
    out_int2[oindex] %= v1;
    out_int2[oindex + 1] = v0;
    out_int2[oindex + 1] %= v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0;
    out_int3[oindex] %= v1;
    out_int3[oindex + 1] = v0;
    out_int3[oindex + 1] %= v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0;
    out_int4[oindex] %= v1;
    out_int4[oindex + 1] = v0;
    out_int4[oindex + 1] %= v1.x;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr auto get_add_f4 = []() noexcept
    {
      int4 v0 = zivc::makeInt4(0, 4, 8, 12);
      int4 v1 = zivc::makeInt4(2, 3, 4, 5);
      v0 += v1;
      return v0;
    };
    constexpr int4 r = get_add_f4();
    out_int4[oindex++] = r;
  }
  {
    constexpr auto get_sub_f4 = []() noexcept
    {
      int4 v0 = zivc::makeInt4(0, 4, 8, 12);
      int4 v1 = zivc::makeInt4(2, 3, 4, 5);
      v0 -= v1;
      return v0;
    };
    constexpr int4 r = get_sub_f4();
    out_int4[oindex++] = r;
  }
  {
    constexpr auto get_mul_f4 = []() noexcept
    {
      int4 v0 = zivc::makeInt4(0, 4, 8, 12);
      int4 v1 = zivc::makeInt4(2, 3, 4, 5);
      v0 *= v1;
      return v0;
    };
    constexpr int4 r = get_mul_f4();
    out_int4[oindex++] = r;
  }
  {
    constexpr auto get_div_f4 = []() noexcept
    {
      int4 v0 = zivc::makeInt4(0, 4, 8, 12);
      int4 v1 = zivc::makeInt4(2, 3, 4, 5);
      v0 /= v1;
      return v0;
    };
    constexpr int4 r = get_div_f4();
    out_int4[oindex++] = r;
  }
}

__kernel void vectorArithmeticAssignmentOperatorLongVecTest(
    zivc::ConstGlobalPtr<int8> in_int8,
    zivc::ConstGlobalPtr<int16> in_int16,
    zivc::GlobalPtr<int8> out_int8,
    zivc::GlobalPtr<int16> out_int16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // Addition
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = v0;
    out_int8[oindex] += v1;
    out_int8[oindex + 1] = v0;
    out_int8[oindex + 1] += v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = v0;
    out_int16[oindex] += v1;
    out_int16[oindex + 1] = v0;
    out_int16[oindex + 1] += v1.s0;
  }
  oindex += 2;
  // Subtraction
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = v0;
    out_int8[oindex] -= v1;
    out_int8[oindex + 1] = v0;
    out_int8[oindex + 1] -= v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = v0;
    out_int16[oindex] -= v1;
    out_int16[oindex + 1] = v0;
    out_int16[oindex + 1] -= v1.s0;
  }
  oindex += 2;
  // Multiplication
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = v0;
    out_int8[oindex] *= v1;
    out_int8[oindex + 1] = v0;
    out_int8[oindex + 1] *= v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = v0;
    out_int16[oindex] *= v1;
    out_int16[oindex + 1] = v0;
    out_int16[oindex + 1] *= v1.s0;
  }
  oindex += 2;
  // Division
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = v0;
    out_int8[oindex] /= v1;
    out_int8[oindex + 1] = v0;
    out_int8[oindex + 1] /= v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = v0;
    out_int16[oindex] /= v1;
    out_int16[oindex + 1] = v0;
    out_int16[oindex + 1] /= v1.s0;
  }
  iindex += 2;
  oindex += 2;
  // Modulo
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    out_int8[oindex] = v0;
    out_int8[oindex] %= v1;
    out_int8[oindex + 1] = v0;
    out_int8[oindex + 1] %= v1.s0;
  }
  {
    const int16 v0 = in_int16[iindex];
    const int16 v1 = in_int16[iindex + 1];
    out_int16[oindex] = v0;
    out_int16[oindex] %= v1;
    out_int16[oindex + 1] = v0;
    out_int16[oindex + 1] %= v1.s0;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr auto get_add_i8 = []() noexcept
    {
      int8 v0 = zivc::makeInt8(0, 4, 8, 12, 16, 20, 24, 28);
      int8 v1 = zivc::makeInt8(2, 3, 4, 5, 6, 7, 8, 9);
      v0 += v1;
      return v0;
    };
    constexpr int8 r = get_add_i8();
    out_int8[oindex++] = r;
  }
  {
    constexpr auto get_sub_i8 = []() noexcept
    {
      int8 v0 = zivc::makeInt8(0, 4, 8, 12, 16, 20, 24, 28);
      int8 v1 = zivc::makeInt8(2, 3, 4, 5, 6, 7, 8, 9);
      v0 -= v1;
      return v0;
    };
    constexpr int8 r = get_sub_i8();
    out_int8[oindex++] = r;
  }
  {
    constexpr auto get_mul_i8 = []() noexcept
    {
      int8 v0 = zivc::makeInt8(0, 4, 8, 12, 16, 20, 24, 28);
      int8 v1 = zivc::makeInt8(2, 3, 4, 5, 6, 7, 8, 9);
      v0 *= v1;
      return v0;
    };
    constexpr int8 r = get_mul_i8();
    out_int8[oindex++] = r;
  }
  {
    constexpr auto get_div_i8 = []() noexcept
    {
      int8 v0 = zivc::makeInt8(0, 4, 8, 12, 16, 20, 24, 28);
      int8 v1 = zivc::makeInt8(2, 3, 4, 5, 6, 7, 8, 9);
      v0 /= v1;
      return v0;
    };
    constexpr int8 r = get_div_i8();
    out_int8[oindex++] = r;
  }
}

__kernel void vectorBitwiseOperatorTest(
    zivc::ConstGlobalPtr<uint2> in_uint2,
    zivc::ConstGlobalPtr<uint3> in_uint3,
    zivc::ConstGlobalPtr<uint4> in_uint4,
    zivc::GlobalPtr<uint2> out_uint2,
    zivc::GlobalPtr<uint3> out_uint3,
    zivc::GlobalPtr<uint4> out_uint4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // bitwise NOT
  {
    const uint2 v = in_uint2[iindex + 1];
    out_uint2[oindex] = zivc::BitNot<uint2>{}(v);
  }
  {
    const uint3 v = in_uint3[iindex + 1];
    out_uint3[oindex] = zivc::BitNot<uint3>{}(v);
  }
  {
    const uint4 v = in_uint4[iindex + 1];
    out_uint4[oindex] = zivc::BitNot<uint4>{}(v);
  }
  ++oindex;
  // bitwise AND
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = zivc::BitAnd<uint2>{}(v0, v1);
    out_uint2[oindex + 1] = v0.x & v1;
    out_uint2[oindex + 2] = v0 & v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = zivc::BitAnd<uint3>{}(v0, v1);
    out_uint3[oindex + 1] = v0.x & v1;
    out_uint3[oindex + 2] = v0 & v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = zivc::BitAnd<uint4>{}(v0, v1);
    out_uint4[oindex + 1] = v0.x & v1;
    out_uint4[oindex + 2] = v0 & v1.x;
  }
  ++iindex;
  oindex += 3;
  // bitwise OR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = zivc::BitOr<uint2>{}(v0, v1);
    out_uint2[oindex + 1] = v0.x | v1;
    out_uint2[oindex + 2] = v0 | v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = zivc::BitOr<uint3>{}(v0, v1);
    out_uint3[oindex + 1] = v0.x | v1;
    out_uint3[oindex + 2] = v0 | v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = zivc::BitOr<uint4>{}(v0, v1);
    out_uint4[oindex + 1] = v0.x | v1;
    out_uint4[oindex + 2] = v0 | v1.x;
  }
  oindex += 3;
  // bitwise XOR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = zivc::BitXor<uint2>{}(v0, v1);
    out_uint2[oindex + 1] = v0.x ^ v1;
    out_uint2[oindex + 2] = v0 ^ v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = zivc::BitXor<uint3>{}(v0, v1);
    out_uint3[oindex + 1] = v0.x ^ v1;
    out_uint3[oindex + 2] = v0 ^ v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = zivc::BitXor<uint4>{}(v0, v1);
    out_uint4[oindex + 1] = v0.x ^ v1;
    out_uint4[oindex + 2] = v0 ^ v1.x;
  }
  iindex += 2;
  oindex += 3;
  // Left shift
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0 << v1;
    out_uint2[oindex + 1] = v0 << v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0 << v1;
    out_uint3[oindex + 1] = v0 << v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0 << v1;
    out_uint4[oindex + 1] = v0 << v1.x;
  }
  oindex += 2;
  // Right shift
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0 >> v1;
    out_uint2[oindex + 1] = v0 >> v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0 >> v1;
    out_uint3[oindex + 1] = v0 >> v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0 >> v1;
    out_uint4[oindex + 1] = v0 >> v1.x;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr uint2 v0 = zivc::makeUInt2(1, 2);
    constexpr uint2 v1 = zivc::makeUInt2(3, 3);
    constexpr uint2 r0 = zivc::BitNot<uint2>{}(v0);
    out_uint2[oindex++] = r0;
    constexpr uint2 r1 = zivc::BitAnd<uint2>{}(v0, v1);
    out_uint2[oindex++] = r1;
    constexpr uint2 r2 = zivc::BitOr<uint2>{}(v0, v1);
    out_uint2[oindex++] = r2;
    constexpr uint2 r3 = zivc::BitXor<uint2>{}(v0, v1);
    out_uint2[oindex++] = r3;
    constexpr uint2 r4 = v0 << v1;
    out_uint2[oindex++] = r4;
    constexpr uint2 r5 = v1 >> v0;
    out_uint2[oindex++] = r5;
  }
}

__kernel void vectorBitwiseOperatorLongVecTest(
    zivc::ConstGlobalPtr<uint8> in_uint8,
    zivc::ConstGlobalPtr<uint16> in_uint16,
    zivc::GlobalPtr<uint8> out_uint8,
    zivc::GlobalPtr<uint16> out_uint16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // bitwise NOT
  {
    const uint8 v = in_uint8[iindex + 1];
    out_uint8[oindex] = zivc::BitNot<uint8>{}(v);
  }
  {
    const uint16 v = in_uint16[iindex + 1];
    out_uint16[oindex] = zivc::BitNot<uint16>{}(v);
  }
  ++oindex;
  // bitwise AND
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = zivc::BitAnd<uint8>{}(v0, v1);
    out_uint8[oindex + 1] = v0.s0 & v1;
    out_uint8[oindex + 2] = v0 & v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = zivc::BitAnd<uint16>{}(v0, v1);
    out_uint16[oindex + 1] = v0.s0 & v1;
    out_uint16[oindex + 2] = v0 & v1.s0;
  }
  ++iindex;
  oindex += 3;
  // bitwise OR
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = zivc::BitOr<uint8>{}(v0, v1);
    out_uint8[oindex + 1] = v0.s0 | v1;
    out_uint8[oindex + 2] = v0 | v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = zivc::BitOr<uint16>{}(v0, v1);
    out_uint16[oindex + 1] = v0.s0 | v1;
    out_uint16[oindex + 2] = v0 | v1.s0;
  }
  oindex += 3;
  // bitwise XOR
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = zivc::BitXor<uint8>{}(v0, v1);
    out_uint8[oindex + 1] = v0.s0 ^ v1;
    out_uint8[oindex + 2] = v0 ^ v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = zivc::BitXor<uint16>{}(v0, v1);
    out_uint16[oindex + 1] = v0.s0 ^ v1;
    out_uint16[oindex + 2] = v0 ^ v1.s0;
  }
  iindex += 2;
  oindex += 3;
  // Left shift
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0 << v1;
    out_uint8[oindex + 1] = v0 << v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0 << v1;
    out_uint16[oindex + 1] = v0 << v1.s0;
  }
  oindex += 2;
  // Right shift
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0 >> v1;
    out_uint8[oindex + 1] = v0 >> v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0 >> v1;
    out_uint16[oindex + 1] = v0 >> v1.s0;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr uint8 v1 = zivc::makeUInt8(3);
    constexpr uint8 r0 = zivc::BitNot<uint8>{}(v0);
    out_uint8[oindex++] = r0;
    constexpr uint8 r1 = zivc::BitAnd<uint8>{}(v0, v1);
    out_uint8[oindex++] = r1;
    constexpr uint8 r2 = zivc::BitOr<uint8>{}(v0, v1);
    out_uint8[oindex++] = r2;
    constexpr uint8 r3 = zivc::BitXor<uint8>{}(v0, v1);
    out_uint8[oindex++] = r3;
    constexpr uint8 r4 = v0 << v1;
    out_uint8[oindex++] = r4;
    constexpr uint8 r5 = v1 >> v0;
    out_uint8[oindex++] = r5;
  }
}

__kernel void vectorBitwiseAssignmentOperatorTest(
    zivc::ConstGlobalPtr<uint2> in_uint2,
    zivc::ConstGlobalPtr<uint3> in_uint3,
    zivc::ConstGlobalPtr<uint4> in_uint4,
    zivc::GlobalPtr<uint2> out_uint2,
    zivc::GlobalPtr<uint3> out_uint3,
    zivc::GlobalPtr<uint4> out_uint4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // bitwise AND
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0;
    out_uint2[oindex] &= v1;
    out_uint2[oindex + 1] = v0;
    out_uint2[oindex + 1] &= v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0;
    out_uint3[oindex] &= v1;
    out_uint3[oindex + 1] = v0;
    out_uint3[oindex + 1] &= v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0;
    out_uint4[oindex] &= v1;
    out_uint4[oindex + 1] = v0;
    out_uint4[oindex + 1] &= v1.x;
  }
  ++iindex;
  oindex += 2;
  // bitwise OR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0;
    out_uint2[oindex] |= v1;
    out_uint2[oindex + 1] = v0;
    out_uint2[oindex + 1] |= v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0;
    out_uint3[oindex] |= v1;
    out_uint3[oindex + 1] = v0;
    out_uint3[oindex + 1] |= v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0;
    out_uint4[oindex] |= v1;
    out_uint4[oindex + 1] = v0;
    out_uint4[oindex + 1] |= v1.x;
  }
  oindex += 2;
  // bitwise XOR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0;
    out_uint2[oindex] ^= v1;
    out_uint2[oindex + 1] = v0;
    out_uint2[oindex + 1] ^= v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0;
    out_uint3[oindex] ^= v1;
    out_uint3[oindex + 1] = v0;
    out_uint3[oindex + 1] ^= v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0;
    out_uint4[oindex] ^= v1;
    out_uint4[oindex + 1] = v0;
    out_uint4[oindex + 1] ^= v1.x;
  }
  iindex += 2;
  oindex += 2;
  // Left shift
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0;
    out_uint2[oindex] <<= v1;
    out_uint2[oindex + 1] = v0;
    out_uint2[oindex + 1] <<= v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0;
    out_uint3[oindex] <<= v1;
    out_uint3[oindex + 1] = v0;
    out_uint3[oindex + 1] <<= v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0;
    out_uint4[oindex] <<= v1;
    out_uint4[oindex + 1] = v0;
    out_uint4[oindex + 1] <<= v1.x;
  }
  oindex += 2;
  // Right shift
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0;
    out_uint2[oindex] >>= v1;
    out_uint2[oindex + 1] = v0;
    out_uint2[oindex + 1] >>= v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0;
    out_uint3[oindex] >>= v1;
    out_uint3[oindex + 1] = v0;
    out_uint3[oindex + 1] >>= v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0;
    out_uint4[oindex] >>= v1;
    out_uint4[oindex + 1] = v0;
    out_uint4[oindex + 1] >>= v1.x;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr auto get_and_u2 = []() noexcept
    {
      uint2 v0 = zivc::makeUInt2(1, 2);
      uint2 v1 = zivc::makeUInt2(3, 3);
      v0 &= v1;
      return v0;
    };
    constexpr uint2 r = get_and_u2();
    out_uint2[oindex++] = r;
  }
  {
    constexpr auto get_or_u2 = []() noexcept
    {
      uint2 v0 = zivc::makeUInt2(1, 2);
      uint2 v1 = zivc::makeUInt2(3, 3);
      v0 |= v1;
      return v0;
    };
    constexpr uint2 r = get_or_u2();
    out_uint2[oindex++] = r;
  }
  {
    constexpr auto get_xor_u2 = []() noexcept
    {
      uint2 v0 = zivc::makeUInt2(1, 2);
      uint2 v1 = zivc::makeUInt2(3, 3);
      v0 ^= v1;
      return v0;
    };
    constexpr uint2 r = get_xor_u2();
    out_uint2[oindex++] = r;
  }
  {
    constexpr auto get_left_shift_u2 = []() noexcept
    {
      uint2 v0 = zivc::makeUInt2(1, 2);
      uint2 v1 = zivc::makeUInt2(3, 3);
      v0 <<= v1;
      return v0;
    };
    constexpr uint2 r = get_left_shift_u2();
    out_uint2[oindex++] = r;
  }
  {
    constexpr auto get_right_shift_u2 = []() noexcept
    {
      uint2 v0 = zivc::makeUInt2(1, 2);
      uint2 v1 = zivc::makeUInt2(3, 3);
      v1 >>= v0;
      return v1;
    };
    constexpr uint2 r = get_right_shift_u2();
    out_uint2[oindex++] = r;
  }
}

__kernel void vectorBitwiseAssignmentOperatorLongVecTest(
    zivc::ConstGlobalPtr<uint8> in_uint8,
    zivc::ConstGlobalPtr<uint16> in_uint16,
    zivc::GlobalPtr<uint8> out_uint8,
    zivc::GlobalPtr<uint16> out_uint16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  // bitwise AND
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0;
    out_uint8[oindex] &= v1;
    out_uint8[oindex + 1] = v0;
    out_uint8[oindex + 1] &= v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0;
    out_uint16[oindex] &= v1;
    out_uint16[oindex + 1] = v0;
    out_uint16[oindex + 1] &= v1.s0;
  }
  ++iindex;
  oindex += 2;
  // bitwise OR
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0;
    out_uint8[oindex] |= v1;
    out_uint8[oindex + 1] = v0;
    out_uint8[oindex + 1] |= v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0;
    out_uint16[oindex] |= v1;
    out_uint16[oindex + 1] = v0;
    out_uint16[oindex + 1] |= v1.s0;
  }
  oindex += 2;
  // bitwise XOR
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0;
    out_uint8[oindex] ^= v1;
    out_uint8[oindex + 1] = v0;
    out_uint8[oindex + 1] ^= v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0;
    out_uint16[oindex] ^= v1;
    out_uint16[oindex + 1] = v0;
    out_uint16[oindex + 1] ^= v1.s0;
  }
  iindex += 2;
  oindex += 2;
  // Left shift
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0;
    out_uint8[oindex] <<= v1;
    out_uint8[oindex + 1] = v0;
    out_uint8[oindex + 1] <<= v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0;
    out_uint16[oindex] <<= v1;
    out_uint16[oindex + 1] = v0;
    out_uint16[oindex + 1] <<= v1.s0;
  }
  oindex += 2;
  // Right shift
  {
    const uint8 v0 = in_uint8[iindex];
    const uint8 v1 = in_uint8[iindex + 1];
    out_uint8[oindex] = v0;
    out_uint8[oindex] >>= v1;
    out_uint8[oindex + 1] = v0;
    out_uint8[oindex + 1] >>= v1.s0;
  }
  {
    const uint16 v0 = in_uint16[iindex];
    const uint16 v1 = in_uint16[iindex + 1];
    out_uint16[oindex] = v0;
    out_uint16[oindex] >>= v1;
    out_uint16[oindex + 1] = v0;
    out_uint16[oindex + 1] >>= v1.s0;
  }

  oindex += 2;
  // Constexpr test
  {
    constexpr auto get_and_u8 = []() noexcept
    {
      uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      uint8 v1 = zivc::makeUInt8(3);
      v0 &= v1;
      return v0;
    };
    constexpr uint8 r = get_and_u8();
    out_uint8[oindex++] = r;
  }
  {
    constexpr auto get_or_u8 = []() noexcept
    {
      uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      uint8 v1 = zivc::makeUInt8(3);
      v0 |= v1;
      return v0;
    };
    constexpr uint8 r = get_or_u8();
    out_uint8[oindex++] = r;
  }
  {
    constexpr auto get_xor_u8 = []() noexcept
    {
      uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      uint8 v1 = zivc::makeUInt8(3);
      v0 ^= v1;
      return v0;
    };
    constexpr uint8 r = get_xor_u8();
    out_uint8[oindex++] = r;
  }
  {
    constexpr auto get_left_shift_u8 = []() noexcept
    {
      uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      uint8 v1 = zivc::makeUInt8(3);
      v0 <<= v1;
      return v0;
    };
    constexpr uint8 r = get_left_shift_u8();
    out_uint8[oindex++] = r;
  }
  {
    constexpr auto get_right_shift_u8 = []() noexcept
    {
      uint8 v0 = zivc::makeUInt8(1, 2, 3, 4, 5, 6, 7, 8);
      uint8 v1 = zivc::makeUInt8(3);
      v1 >>= v0;
      return v1;
    };
    constexpr uint8 r = get_right_shift_u8();
    out_uint8[oindex++] = r;
  }
}

__kernel void vectorIncrementDecrementTest(zivc::GlobalPtr<int4> inout_int4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inout_int4[1] = inout_int4[0]++;
  inout_int4[3] = ++inout_int4[2];
  inout_int4[5] = inout_int4[4]--;
  inout_int4[7] = --inout_int4[6];
}

__kernel void vectorIncrementDecrementLongVecTest(zivc::GlobalPtr<int16> inout_int16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inout_int16[1] = inout_int16[0]++;
  inout_int16[3] = ++inout_int16[2];
  inout_int16[5] = inout_int16[4]--;
  inout_int16[7] = --inout_int16[6];
}

__kernel void vectorConditionalOperatorTest(zivc::GlobalPtr<int4> inout_int4)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  const int4 flag = inout_int4[0];
  const int4 v0 = inout_int4[1];
  const int4 v1 = inout_int4[2];
  inout_int4[3] = static_cast<bool>(flag.x) ? v0 : v1;
  inout_int4[4] = static_cast<bool>(flag.y) ? v0 : v1;
}

__kernel void vectorConditionalOperatorLongVecTest(zivc::GlobalPtr<int16> inout_int16)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  const int16 flag = inout_int16[0];
  const int16 v0 = inout_int16[1];
  const int16 v1 = inout_int16[2];
  inout_int16[3] = static_cast<bool>(flag.s0) ? v0 : v1;
  inout_int16[4] = static_cast<bool>(flag.s1) ? v0 : v1;
}

namespace test {

template <typename ResultType> inline
void setBoolResult(const ResultType result, zivc::GlobalPtr<zivc::Boolean> out)
{
  out[0] = result.x;
  out[1] = result.y;
  out[2] = result.z;
  out[3] = result.w;
}

template <typename ResultType> inline
void setBoolResultL(const ResultType result, zivc::GlobalPtr<zivc::Boolean> out)
{
  out[0] = result.s0;
  out[1] = result.s1;
  out[2] = result.s2;
  out[3] = result.s3;
  out[4] = result.s4;
  out[5] = result.s5;
  out[6] = result.s6;
  out[7] = result.s7;
}

} /* namespace test */

__kernel void vectorComparisonOperatorTest(zivc::ConstGlobalPtr<int4> in_int4,
                                           zivc::GlobalPtr<zivc::Boolean> out)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    test::setBoolResult(v0 == v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x == v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 == v1.x, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 != v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x != v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 != v1.x, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 < v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x < v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 < v1.x, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 > v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x > v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 > v1.x, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 <= v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x <= v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 <= v1.x, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 >= v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0.x >= v1, out + oindex);
    oindex += 4;
    test::setBoolResult(v0 >= v1.x, out + oindex);
    oindex += 4;
  }

  // Constexpr test
  {
    constexpr int4 v0 = zivc::makeInt4(0, 1, 2, 3);
    constexpr int4 v1 = zivc::makeInt4(0, 1, 5, 1);
    constexpr auto r0 = zivc::EqualTo<void>{}(v0, v1);
    test::setBoolResult(r0, out + oindex);
    oindex += 4;
    constexpr auto r1 = zivc::NotEqualTo<void>{}(v0, v1);
    test::setBoolResult(r1, out + oindex);
    oindex += 4;
    constexpr auto r2 = zivc::Less<void>{}(v0, v1);
    test::setBoolResult(r2, out + oindex);
    oindex += 4;
    constexpr auto r3 = zivc::Greater<void>{}(v0, v1);
    test::setBoolResult(r3, out + oindex);
    oindex += 4;
    constexpr auto r4 = zivc::LessEqual<void>{}(v0, v1);
    test::setBoolResult(r4, out + oindex);
    oindex += 4;
    constexpr auto r5 = zivc::GreaterEqual<void>{}(v0, v1);
    test::setBoolResult(r5, out + oindex);
    oindex += 4;
  }
}

__kernel void vectorComparisonOperatorLongVecTest(zivc::ConstGlobalPtr<int8> in_int8,
                                                  zivc::GlobalPtr<zivc::Boolean> out)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    test::setBoolResultL(v0 == v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 == v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 == v1.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 != v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 != v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 != v1.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 < v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 < v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 < v1.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 > v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 > v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 > v1.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 <= v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 <= v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 <= v1.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 >= v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0.s0 >= v1, out + oindex);
    oindex += 8;
    test::setBoolResultL(v0 >= v1.s0, out + oindex);
    oindex += 8;
  }

  // Constexpr test
  {
    constexpr int8 v0 = zivc::makeInt8(0, 1, 2, 3, 4, 5, 6, 7);
    constexpr int8 v1 = zivc::makeInt8(0, 1, 5, 1, 0, 5, 1, 1);
    constexpr auto r0 = zivc::EqualTo<void>{}(v0, v1);
    test::setBoolResultL(r0, out + oindex);
    oindex += 8;
    constexpr auto r1 = zivc::NotEqualTo<void>{}(v0, v1);
    test::setBoolResultL(r1, out + oindex);
    oindex += 8;
    constexpr auto r2 = zivc::Less<void>{}(v0, v1);
    test::setBoolResultL(r2, out + oindex);
    oindex += 8;
    constexpr auto r3 = zivc::Greater<void>{}(v0, v1);
    test::setBoolResultL(r3, out + oindex);
    oindex += 8;
    constexpr auto r4 = zivc::LessEqual<void>{}(v0, v1);
    test::setBoolResultL(r4, out + oindex);
    oindex += 8;
    constexpr auto r5 = zivc::GreaterEqual<void>{}(v0, v1);
    test::setBoolResultL(r5, out + oindex);
    oindex += 8;
  }
}

__kernel void vectorComparisonResultTest(
    zivc::ConstGlobalPtr<uchar> in1,
    zivc::ConstGlobalPtr<char2> in2,
    zivc::ConstGlobalPtr<ushort3> in3,
    zivc::ConstGlobalPtr<uint4> in4,
    zivc::GlobalPtr<zivc::CompResult<uchar>> out1,
    zivc::GlobalPtr<zivc::CompResult<char2>> out2,
    zivc::GlobalPtr<zivc::CompResult<ushort3>> out3,
    zivc::GlobalPtr<zivc::CompResult<uint4>> out4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (0 < index)
    return;

  // scalar
  {
    const uchar v1 = in1[0];
    const uchar v2 = in1[1];
    out1[0] = zivc::EqualTo<uchar>{}(v1, v2);
    out1[1] = zivc::NotEqualTo<uchar>{}(v1, v2);
    out1[2] = zivc::Greater<uchar>{}(v1, v2);
    out1[3] = zivc::GreaterEqual<uchar>{}(v1, v2);
    out1[4] = zivc::Less<uchar>{}(v1, v2);
    out1[5] = zivc::LessEqual<uchar>{}(v1, v2);
  }
  // vector2
  {
    const char2 v1 = in2[0];
    const char2 v2 = in2[1];
    out2[0] = zivc::EqualTo<void>{}(v1, v2);
    out2[1] = zivc::NotEqualTo<void>{}(v1, v2);
    out2[2] = zivc::Greater<void>{}(v1, v2);
    out2[3] = zivc::GreaterEqual<void>{}(v1, v2);
    out2[4] = zivc::Less<void>{}(v1, v2);
    out2[5] = zivc::LessEqual<void>{}(v1, v2);
  }
  // vector3
  {
    const ushort3 v1 = in3[0];
    const ushort3 v2 = in3[1];
    out3[0] = v1 == v2;
    out3[1] = v1 != v2;
    out3[2] = v1 > v2;
    out3[3] = v1 >= v2;
    out3[4] = v1 < v2;
    out3[5] = v1 <= v2;
  }
  // vector4
  {
    const uint4 v1 = in4[0];
    const uint4 v2 = in4[1];
    out4[0] = v1 == v2;
    out4[1] = v1 != v2;
    out4[2] = v1 > v2;
    out4[3] = v1 >= v2;
    out4[4] = v1 < v2;
    out4[5] = v1 <= v2;
  }
}

__kernel void vectorComparisonResultLongVecTest(
    zivc::ConstGlobalPtr<ushort8> in8,
    zivc::ConstGlobalPtr<uint16> in16,
    zivc::GlobalPtr<zivc::CompResult<ushort8>> out8,
    zivc::GlobalPtr<zivc::CompResult<uint16>> out16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (0 < index)
    return;

  // vector8
  {
    const ushort8 v1 = in8[0];
    const ushort8 v2 = in8[1];
    out8[0] = v1 == v2;
    out8[1] = v1 != v2;
    out8[2] = v1 > v2;
    out8[3] = v1 >= v2;
    out8[4] = v1 < v2;
    out8[5] = v1 <= v2;
  }
  // vector16
  {
    const uint16 v1 = in16[0];
    const uint16 v2 = in16[1];
    out16[0] = v1 == v2;
    out16[1] = v1 != v2;
    out16[2] = v1 > v2;
    out16[3] = v1 >= v2;
    out16[4] = v1 < v2;
    out16[5] = v1 <= v2;
  }
}

__kernel void vectorLogicalOperatorTest(zivc::ConstGlobalPtr<int4> in_int4,
                                        zivc::GlobalPtr<zivc::Boolean> out)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    const int4 v2 = in_int4[iindex + 2];

    const auto result1 = v0 == v1;
    const auto result2 = v1 == v2;

    test::setBoolResult(!result1, out + oindex);
    oindex += 4;
    test::setBoolResult(result1 && result2, out + oindex);
    oindex += 4;
    test::setBoolResult(result1.x && result2, out + oindex);
    oindex += 4;
    test::setBoolResult(result1 && result2.x, out + oindex);
    oindex += 4;
    test::setBoolResult(result1 || result2, out + oindex);
    oindex += 4;
    test::setBoolResult(result1.x || result2, out + oindex);
    oindex += 4;
    test::setBoolResult(result1 || result2.x, out + oindex);
    oindex += 4;
  }

  // Constexpr test
  {
    constexpr int4 v0 = zivc::makeInt4(0, 1, 2, 3);
    constexpr int4 v1 = zivc::makeInt4(0, 1, 5, 1);
    constexpr int4 v2 = zivc::makeInt4(2, 2, 2, 2);
    constexpr auto result1 = v0 == v1;
    constexpr auto result2 = v1 == v2;

    constexpr auto r0 = zivc::LogicalNot<void>{}(result1);
    test::setBoolResult(r0, out + oindex);
    oindex += 4;
    constexpr auto r1 = zivc::LogicalAnd<void>{}(result1, result2);
    test::setBoolResult(r1, out + oindex);
    oindex += 4;
    constexpr auto r2 = zivc::LogicalOr<void>{}(result1, result2);
    test::setBoolResult(r2, out + oindex);
    oindex += 4;
  }
}

__kernel void vectorLogicalOperatorLongVecTest(zivc::ConstGlobalPtr<int8> in_int8,
                                               zivc::GlobalPtr<zivc::Boolean> out)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  size_t iindex = 0;
  size_t oindex = 0;
  {
    const int8 v0 = in_int8[iindex];
    const int8 v1 = in_int8[iindex + 1];
    const int8 v2 = in_int8[iindex + 2];

    const auto result1 = v0 == v1;
    const auto result2 = v1 == v2;

    test::setBoolResultL(!result1, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1 && result2, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1.s0 && result2, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1 && result2.s0, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1 || result2, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1.s0 || result2, out + oindex);
    oindex += 8;
    test::setBoolResultL(result1 || result2.s0, out + oindex);
    oindex += 8;
  }

  // Constexpr test
  {
    constexpr int8 v0 = zivc::makeInt8(0, 1, 2, 3, 4, 5, 6, 7);
    constexpr int8 v1 = zivc::makeInt8(0, 1, 5, 1, 0, 5, 1, 1);
    constexpr int8 v2 = zivc::makeInt8(2);
    constexpr auto result1 = v0 == v1;
    constexpr auto result2 = v1 == v2;

    constexpr auto r0 = !result1;
    test::setBoolResultL(r0, out + oindex);
    oindex += 8;
    constexpr auto r1 = result1 && result2;
    test::setBoolResultL(r1, out + oindex);
    oindex += 8;
    constexpr auto r2 = result1 || result2;
    test::setBoolResultL(r2, out + oindex);
    oindex += 8;
  }
}

namespace inner {

template <typename T> inline
void testScalar(zivc::ConstGlobalPtr<int32b> in0,
                zivc::ConstGlobalPtr<float> in1,
                zivc::GlobalPtr<T> out,
                const size_t int_n,
                const size_t float_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
}

} /* namespace inner */

__kernel void scalarCastTest(zivc::ConstGlobalPtr<int32b> in_int,
                             zivc::ConstGlobalPtr<float> in_float,
                             zivc::GlobalPtr<int8b> out_int8b,
                             zivc::GlobalPtr<uint8b> out_uint8b,
                             zivc::GlobalPtr<int16b> out_int16b,
                             zivc::GlobalPtr<uint16b> out_uint16b,
                             zivc::GlobalPtr<int32b> out_int32b,
                             zivc::GlobalPtr<uint32b> out_uint32b,
                             zivc::GlobalPtr<float> out_float,
                             const uint32b int_n,
                             const uint32b float_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testScalar<int8b>(in_int, in_float, out_int8b, int_n, float_n);
  inner::testScalar<uint8b>(in_int, in_float, out_uint8b, int_n, float_n);
  inner::testScalar<int16b>(in_int, in_float, out_int16b, int_n, float_n);
  inner::testScalar<uint16b>(in_int, in_float, out_uint16b, int_n, float_n);
  inner::testScalar<int32b>(in_int, in_float, out_int32b, int_n, float_n);
  inner::testScalar<uint32b>(in_int, in_float, out_uint32b, int_n, float_n);
  inner::testScalar<float>(in_int, in_float, out_float, int_n, float_n);
}

__kernel void scalarBoolCastTest(zivc::ConstGlobalPtr<int32b> in_int,
                                 zivc::ConstGlobalPtr<float> in_float,
                                 zivc::GlobalPtr<zivc::Boolean> out_bool,
                                 const uint32b int_n,
                                 const uint32b float_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  // bool
  {
    size_t index = 0;
    for (size_t i = 0; i < int_n; ++i)
      out_bool[index++] = zivc::cast<bool>(in_int[i]);
    for (size_t i = 0; i < float_n; ++i)
      out_bool[index++] = zivc::cast<bool>(in_float[i]);
  }
}

namespace inner {

template <typename T> inline
void testVec2(zivc::ConstGlobalPtr<int32b> in0,
              zivc::ConstGlobalPtr<float> in1,
              zivc::ConstGlobalPtr<int2> in2,
              zivc::ConstGlobalPtr<float2> in3,
              zivc::GlobalPtr<T> out,
              const size_t int_n,
              const size_t float_n,
              const size_t int2_n,
              const size_t float2_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
  for (size_t i = 0; i < int2_n; ++i)
    out[index++] = zivc::cast<Type>(in2[i]);
  for (size_t i = 0; i < float2_n; ++i)
    out[index++] = zivc::cast<Type>(in3[i]);
}

} /* namespace inner */

__kernel void vector2CastTest(zivc::ConstGlobalPtr<int32b> in_int,
                              zivc::ConstGlobalPtr<float> in_float,
                              zivc::ConstGlobalPtr<int2> in_int2,
                              zivc::ConstGlobalPtr<float2> in_float2,
                              zivc::GlobalPtr<char2> out_char2,
                              zivc::GlobalPtr<uchar2> out_uchar2,
                              zivc::GlobalPtr<short2> out_short2,
                              zivc::GlobalPtr<ushort2> out_ushort2,
                              zivc::GlobalPtr<int2> out_int2,
                              zivc::GlobalPtr<uint2> out_uint2,
                              zivc::GlobalPtr<float2> out_float2,
                              const uint32b int_n,
                              const uint32b float_n,
                              const uint32b int2_n,
                              const uint32b float2_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testVec2<char2>(in_int, in_float, in_int2, in_float2, out_char2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<uchar2>(in_int, in_float, in_int2, in_float2, out_uchar2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<short2>(in_int, in_float, in_int2, in_float2, out_short2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<ushort2>(in_int, in_float, in_int2, in_float2, out_ushort2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<int2>(in_int, in_float, in_int2, in_float2, out_int2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<uint2>(in_int, in_float, in_int2, in_float2, out_uint2, int_n, float_n, int2_n, float2_n);
  inner::testVec2<float2>(in_int, in_float, in_int2, in_float2, out_float2, int_n, float_n, int2_n, float2_n);
}

namespace inner {

template <typename T> inline
void testVec3 (zivc::ConstGlobalPtr<int32b> in0,
               zivc::ConstGlobalPtr<float> in1,
               zivc::ConstGlobalPtr<int3> in2,
               zivc::ConstGlobalPtr<float3> in3,
               zivc::GlobalPtr<T> out,
               const size_t int_n,
               const size_t float_n,
               const size_t int3_n,
               const size_t float3_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
  for (size_t i = 0; i < int3_n; ++i)
    out[index++] = zivc::cast<Type>(in2[i]);
  for (size_t i = 0; i < float3_n; ++i)
    out[index++] = zivc::cast<Type>(in3[i]);
}

} /* namespace inner */

__kernel void vector3CastTest(zivc::ConstGlobalPtr<int32b> in_int,
                              zivc::ConstGlobalPtr<float> in_float,
                              zivc::ConstGlobalPtr<int3> in_int3,
                              zivc::ConstGlobalPtr<float3> in_float3,
                              zivc::GlobalPtr<char3> out_char3,
                              zivc::GlobalPtr<uchar3> out_uchar3,
                              zivc::GlobalPtr<short3> out_short3,
                              zivc::GlobalPtr<ushort3> out_ushort3,
                              zivc::GlobalPtr<int3> out_int3,
                              zivc::GlobalPtr<uint3> out_uint3,
                              zivc::GlobalPtr<float3> out_float3,
                              const uint32b int_n,
                              const uint32b float_n,
                              const uint32b int3_n,
                              const uint32b float3_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testVec3<char3>(in_int, in_float, in_int3, in_float3, out_char3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<uchar3>(in_int, in_float, in_int3, in_float3, out_uchar3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<short3>(in_int, in_float, in_int3, in_float3, out_short3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<ushort3>(in_int, in_float, in_int3, in_float3, out_ushort3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<int3>(in_int, in_float, in_int3, in_float3, out_int3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<uint3>(in_int, in_float, in_int3, in_float3, out_uint3, int_n, float_n, int3_n, float3_n);
  inner::testVec3<float3>(in_int, in_float, in_int3, in_float3, out_float3, int_n, float_n, int3_n, float3_n);
}

namespace inner {

template <typename T> inline
void testVec4(zivc::ConstGlobalPtr<int32b> in0,
              zivc::ConstGlobalPtr<float> in1,
              zivc::ConstGlobalPtr<int4> in2,
              zivc::ConstGlobalPtr<float4> in3,
              zivc::GlobalPtr<T> out,
              const size_t int_n,
              const size_t float_n,
              const size_t int4_n,
              const size_t float4_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
  for (size_t i = 0; i < int4_n; ++i)
    out[index++] = zivc::cast<Type>(in2[i]);
  for (size_t i = 0; i < float4_n; ++i)
    out[index++] = zivc::cast<Type>(in3[i]);
}

} /* namespace inner */

__kernel void vector4CastTest(zivc::ConstGlobalPtr<int32b> in_int,
                              zivc::ConstGlobalPtr<float> in_float,
                              zivc::ConstGlobalPtr<int4> in_int4,
                              zivc::ConstGlobalPtr<float4> in_float4,
                              zivc::GlobalPtr<char4> out_char4,
                              zivc::GlobalPtr<uchar4> out_uchar4,
                              zivc::GlobalPtr<short4> out_short4,
                              zivc::GlobalPtr<ushort4> out_ushort4,
                              zivc::GlobalPtr<int4> out_int4,
                              zivc::GlobalPtr<uint4> out_uint4,
                              zivc::GlobalPtr<float4> out_float4,
                              const uint32b int_n,
                              const uint32b float_n,
                              const uint32b int4_n,
                              const uint32b float4_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testVec4<char4>(in_int, in_float, in_int4, in_float4, out_char4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<uchar4>(in_int, in_float, in_int4, in_float4, out_uchar4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<short4>(in_int, in_float, in_int4, in_float4, out_short4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<ushort4>(in_int, in_float, in_int4, in_float4, out_ushort4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<int4>(in_int, in_float, in_int4, in_float4, out_int4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<uint4>(in_int, in_float, in_int4, in_float4, out_uint4, int_n, float_n, int4_n, float4_n);
  inner::testVec4<float4>(in_int, in_float, in_int4, in_float4, out_float4, int_n, float_n, int4_n, float4_n);
}

namespace inner {

template <typename T> inline
void testVec8(zivc::ConstGlobalPtr<int32b> in0,
              zivc::ConstGlobalPtr<float> in1,
              zivc::ConstGlobalPtr<int8> in2,
              zivc::ConstGlobalPtr<float8> in3,
              zivc::GlobalPtr<T> out,
              const size_t int_n,
              const size_t float_n,
              const size_t int8_n,
              const size_t float8_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
  for (size_t i = 0; i < int8_n; ++i)
    out[index++] = zivc::cast<Type>(in2[i]);
  for (size_t i = 0; i < float8_n; ++i)
    out[index++] = zivc::cast<Type>(in3[i]);
}

} /* namespace inner */

__kernel void vector8CastTest(zivc::ConstGlobalPtr<int32b> in_int,
                              zivc::ConstGlobalPtr<float> in_float,
                              zivc::ConstGlobalPtr<int8> in_int8,
                              zivc::ConstGlobalPtr<float8> in_float8,
                              zivc::GlobalPtr<char8> out_char8,
                              zivc::GlobalPtr<uchar8> out_uchar8,
                              zivc::GlobalPtr<short8> out_short8,
                              zivc::GlobalPtr<ushort8> out_ushort8,
                              zivc::GlobalPtr<int8> out_int8,
                              zivc::GlobalPtr<uint8> out_uint8,
                              zivc::GlobalPtr<float8> out_float8,
                              const uint32b int_n,
                              const uint32b float_n,
                              const uint32b int8_n,
                              const uint32b float8_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testVec8<char8>(in_int, in_float, in_int8, in_float8, out_char8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<uchar8>(in_int, in_float, in_int8, in_float8, out_uchar8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<short8>(in_int, in_float, in_int8, in_float8, out_short8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<ushort8>(in_int, in_float, in_int8, in_float8, out_ushort8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<int8>(in_int, in_float, in_int8, in_float8, out_int8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<uint8>(in_int, in_float, in_int8, in_float8, out_uint8, int_n, float_n, int8_n, float8_n);
  inner::testVec8<float8>(in_int, in_float, in_int8, in_float8, out_float8, int_n, float_n, int8_n, float8_n);
}

namespace inner {

template <typename T> inline
void testVec16(zivc::ConstGlobalPtr<int32b> in0,
               zivc::ConstGlobalPtr<float> in1,
               zivc::ConstGlobalPtr<int16> in2,
               zivc::ConstGlobalPtr<float16> in3,
               zivc::GlobalPtr<T> out,
               const size_t int_n,
               const size_t float_n,
               const size_t int16_n,
               const size_t float16_n) noexcept
{
  size_t index = 0;
  using Type = zivc::RemoveCvrefAddressT<decltype(out[0])>;
  for (size_t i = 0; i < int_n; ++i)
    out[index++] = zivc::cast<Type>(in0[i]);
  for (size_t i = 0; i < float_n; ++i)
    out[index++] = zivc::cast<Type>(in1[i]);
  for (size_t i = 0; i < int16_n; ++i)
    out[index++] = zivc::cast<Type>(in2[i]);
  for (size_t i = 0; i < float16_n; ++i)
    out[index++] = zivc::cast<Type>(in3[i]);
}

} /* namespace inner */

__kernel void vector16CastTest(zivc::ConstGlobalPtr<int32b> in_int,
                               zivc::ConstGlobalPtr<float> in_float,
                               zivc::ConstGlobalPtr<int16> in_int16,
                               zivc::ConstGlobalPtr<float16> in_float16,
                               zivc::GlobalPtr<char16> out_char16,
                               zivc::GlobalPtr<uchar16> out_uchar16,
                               zivc::GlobalPtr<short16> out_short16,
                               zivc::GlobalPtr<ushort16> out_ushort16,
                               zivc::GlobalPtr<int16> out_int16,
                               zivc::GlobalPtr<uint16> out_uint16,
                               zivc::GlobalPtr<float16> out_float16,
                               const uint32b int_n,
                               const uint32b float_n,
                               const uint32b int16_n,
                               const uint32b float16_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  inner::testVec16<char16>(in_int, in_float, in_int16, in_float16, out_char16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<uchar16>(in_int, in_float, in_int16, in_float16, out_uchar16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<short16>(in_int, in_float, in_int16, in_float16, out_short16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<ushort16>(in_int, in_float, in_int16, in_float16, out_ushort16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<int16>(in_int, in_float, in_int16, in_float16, out_int16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<uint16>(in_int, in_float, in_int16, in_float16, out_uint16, int_n, float_n, int16_n, float16_n);
  inner::testVec16<float16>(in_int, in_float, in_int16, in_float16, out_float16, int_n, float_n, int16_n, float16_n);
}

__kernel void bitCastTest(zivc::GlobalPtr<int32b> inout_int,
                          zivc::GlobalPtr<float> inout_float,
                          zivc::GlobalPtr<uint2> inout_uint2,
                          zivc::GlobalPtr<float2> inout_float2,
                          zivc::GlobalPtr<char3> inout_char3,
                          zivc::GlobalPtr<uchar4> inout_uchar4,
                          zivc::GlobalPtr<ushort3> inout_ushort3,
                          zivc::GlobalPtr<short4> inout_short4,
                          zivc::GlobalPtr<int3> inout_int3,
                          zivc::GlobalPtr<int4> inout_int4,
                          zivc::GlobalPtr<float3> inout_float3,
                          zivc::GlobalPtr<float4> inout_float4)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // Scalar
    {
      const int32b a = inout_int[0];
      const float b = inout_float[0];
      inout_int[0] = zivc::castBit<int32b>(b);
      inout_float[0] = zivc::castBit<float>(a);
    }
    // Vector2
    {
      const uint2 a = inout_uint2[0];
      const float2 b = inout_float2[0];
      inout_uint2[0] = zivc::castBit<uint2>(b);
      inout_float2[0] = zivc::castBit<float2>(a);
    }
    // Vector3 
    {
      const char3 c3 = inout_char3[0];
      const uchar4 uc4 = inout_uchar4[0];
      inout_char3[0] = zivc::castBit<char3>(uc4);
      inout_uchar4[0] = zivc::castBit<uchar4>(c3);
    }
    {
      const ushort3 us3 = inout_ushort3[0];
      const short4 s4 = inout_short4[0];
      inout_ushort3[0] = zivc::castBit<ushort3>(s4);
      inout_short4[0] = zivc::castBit<short4>(us3);
    }
    {
      const int3 i3 = inout_int3[0];
      const int4 i4 = inout_int4[0];
      const float3 f3 = inout_float3[0];
      const float4 f4 = inout_float4[0];
      inout_int3[0] = zivc::castBit<int3>(i4);
      inout_int3[1] = zivc::castBit<int3>(f3);
      inout_int3[2] = zivc::castBit<int3>(f4);
      inout_int4[0] = zivc::castBit<int4>(i3);
      inout_int4[1] = zivc::castBit<int4>(f3);
      inout_int4[2] = zivc::castBit<int4>(f4);
      inout_float3[0] = zivc::castBit<float3>(i3);
      inout_float3[1] = zivc::castBit<float3>(i4);
      inout_float3[2] = zivc::castBit<float3>(f4);
      inout_float4[0] = zivc::castBit<float4>(i3);
      inout_float4[1] = zivc::castBit<float4>(i4);
      inout_float4[2] = zivc::castBit<float4>(f3);
    }
  }
}

__kernel void bitCastLongVecTest(zivc::GlobalPtr<uint8> inout_uint8,
                                 zivc::GlobalPtr<float8> inout_float8,
                                 zivc::GlobalPtr<uint16> inout_uint16,
                                 zivc::GlobalPtr<float16> inout_float16)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // Vector8
    {
      const uint8 a = inout_uint8[0];
      const float8 b = inout_float8[0];
      inout_uint8[0] = zivc::castBit<uint8>(b);
      inout_float8[0] = zivc::castBit<float8>(a);
    }
    // Vector16
    {
      const uint16 a = inout_uint16[0];
      const float16 b = inout_float16[0];
      inout_uint16[0] = zivc::castBit<uint16>(b);
      inout_float16[0] = zivc::castBit<float16>(a);
    }
  }
}

namespace inner {

struct LoadStoreStorage
{
  int8b i8_[4];
  int8b pad1_[4];
  uint16b u16_[4];
  int32b i32_[4];
  half h_[4];
  half pad2_[4];
  float f_[4];
};

static_assert(sizeof(LoadStoreStorage) % 16 == 0);

} // namespace inner

__kernel void vectorLoadStoreTest(zivc::GlobalPtr<int8b> inout_i8,
                                  zivc::GlobalPtr<uint16b> inout_u16,
                                  zivc::GlobalPtr<int32b> inout_i32,
                                  zivc::GlobalPtr<float> inout_f,
                                  zivc::LocalPtr<inner::LoadStoreStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // int8b
    {
      constexpr char k = 2;
      // vector2
      {
        const char2 v1 = zivc::vload2(0, inout_i8);
        zivc::vstore2(k * v1, 0, inout_i8);
        zivc::LocalPtr<int8b> p = &storage[0].i8_[0];
        zivc::vstore2(k * v1, 0, p);
        const char2 v2 = zivc::vload2(0, p);
        zivc::vstore2(k * v2, 1, inout_i8);
      }
      size_t offset = 4;
      // vector3
      {
        const char3 v1 = zivc::vload3(0, inout_i8 + offset);
        zivc::vstore3(k * v1, 0, inout_i8 + offset);
        zivc::vstore3(k * v1, 0, &storage[0].i8_[0]);
        const char3 v2 = zivc::vload3(0, &storage[0].i8_[0]);
        zivc::vstore3(k * v2, 1, inout_i8 + offset);
      }
      offset += 6;
      // vector4
      {
        const char4 v1 = zivc::vload4(0, inout_i8 + offset);
        zivc::vstore4(k * v1, 0, inout_i8 + offset);
        zivc::vstore4(k * v1, 0, &storage[0].i8_[0]);
        const char4 v2 = zivc::vload4(0, &storage[0].i8_[0]);
        zivc::vstore4(k * v2, 1, inout_i8 + offset);
      }
    }
    // uint16b
    {
      constexpr ushort k = 2;
      // vector2
      {
        const ushort2 v1 = zivc::vload2(0, inout_u16);
        zivc::vstore2(k * v1, 0, inout_u16);
        zivc::LocalPtr<uint16b> p = &storage[0].u16_[0];
        zivc::vstore2(k * v1, 0, p);
        const ushort2 v2 = zivc::vload2(0, p);
        zivc::vstore2(k * v2, 1, inout_u16);
      }
      size_t offset = 4;
      // vector3
      {
        const ushort3 v1 = zivc::vload3(0, inout_u16 + offset);
        zivc::vstore3(k * v1, 0, inout_u16 + offset);
        zivc::vstore3(k * v1, 0, &storage[0].u16_[0]);
        const ushort3 v2 = zivc::vload3(0, &storage[0].u16_[0]);
        zivc::vstore3(k * v2, 1, inout_u16 + offset);
      }
      offset += 6;
      // vector4
      {
        const ushort4 v1 = zivc::vload4(0, inout_u16 + offset);
        zivc::vstore4(k * v1, 0, inout_u16 + offset);
        zivc::vstore4(k * v1, 0, &storage[0].u16_[0]);
        const ushort4 v2 = zivc::vload4(0, &storage[0].u16_[0]);
        zivc::vstore4(k * v2, 1, inout_u16 + offset);
      }
    }
    // int32b
    {
      constexpr int32b k = 2;
      // vector2
      {
        const int2 v1 = zivc::vload2(0, inout_i32);
        zivc::vstore2(k * v1, 0, inout_i32);
        zivc::LocalPtr<int32b> p = &storage[0].i32_[0];
        zivc::vstore2(k * v1, 0, p);
        const int2 v2 = zivc::vload2(0, p);
        zivc::vstore2(k * v2, 1, inout_i32);
      }
      size_t offset = 4;
      // vector3
      {
        const int3 v1 = zivc::vload3(0, inout_i32 + offset);
        zivc::vstore3(k * v1, 0, inout_i32 + offset);
        zivc::vstore3(k * v1, 0, &storage[0].i32_[0]);
        const int3 v2 = zivc::vload3(0, &storage[0].i32_[0]);
        zivc::vstore3(k * v2, 1, inout_i32 + offset);
      }
      offset += 6;
      // vector4
      {
        const int4 v1 = zivc::vload4(0, inout_i32 + offset);
        zivc::vstore4(k * v1, 0, inout_i32 + offset);
        zivc::vstore4(k * v1, 0, &storage[0].i32_[0]);
        const int4 v2 = zivc::vload4(0, &storage[0].i32_[0]);
        zivc::vstore4(k * v2, 1, inout_i32 + offset);
      }
    }
    // float 
    {
      constexpr float k = 2.0f;
      // vector2
      {
        const float2 v1 = zivc::vload2(0, inout_f);
        zivc::vstore2(k * v1, 0, inout_f);
        zivc::LocalPtr<float> p = &storage[0].f_[0];
        zivc::vstore2(k * v1, 0, p);
        const float2 v2 = zivc::vload2(0, p);
        zivc::vstore2(k * v2, 1, inout_f);
      }
      size_t offset = 4;
      // vector3
      {
        const float3 v1 = zivc::vload3(0, inout_f + offset);
        zivc::vstore3(k * v1, 0, inout_f + offset);
        zivc::vstore3(k * v1, 0, &storage[0].f_[0]);
        const float3 v2 = zivc::vload3(0, &storage[0].f_[0]);
        zivc::vstore3(k * v2, 1, inout_f + offset);
      }
      offset += 6;
      // vector4
      {
        const float4 v1 = zivc::vload4(0, inout_f + offset);
        zivc::vstore4(k * v1, 0, inout_f + offset);
        zivc::vstore4(k * v1, 0, &storage[0].f_[0]);
        const float4 v2 = zivc::vload4(0, &storage[0].f_[0]);
        zivc::vstore4(k * v2, 1, inout_f + offset);
      }
    }
  }
}

__kernel void vectorLoadStoreClTest(zivc::GlobalPtr<int8b> inout_i8,
                                    zivc::GlobalPtr<uint16b> inout_u16,
                                    zivc::GlobalPtr<int32b> inout_i32,
                                    zivc::GlobalPtr<float> inout_f,
                                    zivc::LocalPtr<inner::LoadStoreStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // int8b
    {
      constexpr char k = 2;
      // vector2
      {
        const char2 v1 = vload2(0, inout_i8);
        vstore2(k * v1, 0, inout_i8);
        zivc::LocalPtr<int8b> p = &storage[0].i8_[0];
        vstore2(k * v1, 0, p);
        const char2 v2 = vload2(0, p);
        vstore2(k * v2, 1, inout_i8);
      }
      size_t offset = 4;
      // vector3
      {
        const char3 v1 = vload3(0, inout_i8 + offset);
        vstore3(k * v1, 0, inout_i8 + offset);
        vstore3(k * v1, 0, &storage[0].i8_[0]);
        const char3 v2 = vload3(0, &storage[0].i8_[0]);
        vstore3(k * v2, 1, inout_i8 + offset);
      }
      offset += 6;
      // vector4
      {
        const char4 v1 = vload4(0, inout_i8 + offset);
        vstore4(k * v1, 0, inout_i8 + offset);
        vstore4(k * v1, 0, &storage[0].i8_[0]);
        const char4 v2 = vload4(0, &storage[0].i8_[0]);
        vstore4(k * v2, 1, inout_i8 + offset);
      }
    }
    // uint16b
    {
      constexpr ushort k = 2;
      // vector2
      {
        const ushort2 v1 = vload2(0, inout_u16);
        vstore2(k * v1, 0, inout_u16);
        zivc::LocalPtr<uint16b> p = &storage[0].u16_[0];
        vstore2(k * v1, 0, p);
        const ushort2 v2 = vload2(0, p);
        vstore2(k * v2, 1, inout_u16);
      }
      size_t offset = 4;
      // vector3
      {
        const ushort3 v1 = vload3(0, inout_u16 + offset);
        vstore3(k * v1, 0, inout_u16 + offset);
        vstore3(k * v1, 0, &storage[0].u16_[0]);
        const ushort3 v2 = vload3(0, &storage[0].u16_[0]);
        vstore3(k * v2, 1, inout_u16 + offset);
      }
      offset += 6;
      // vector4
      {
        const ushort4 v1 = vload4(0, inout_u16 + offset);
        vstore4(k * v1, 0, inout_u16 + offset);
        vstore4(k * v1, 0, &storage[0].u16_[0]);
        const ushort4 v2 = vload4(0, &storage[0].u16_[0]);
        vstore4(k * v2, 1, inout_u16 + offset);
      }
    }
    // int32b
    {
      constexpr int32b k = 2;
      // vector2
      {
        const int2 v1 = vload2(0, inout_i32);
        vstore2(k * v1, 0, inout_i32);
        zivc::LocalPtr<int32b> p = &storage[0].i32_[0];
        vstore2(k * v1, 0, p);
        const int2 v2 = vload2(0, p);
        vstore2(k * v2, 1, inout_i32);
      }
      size_t offset = 4;
      // vector3
      {
        const int3 v1 = vload3(0, inout_i32 + offset);
        vstore3(k * v1, 0, inout_i32 + offset);
        vstore3(k * v1, 0, &storage[0].i32_[0]);
        const int3 v2 = vload3(0, &storage[0].i32_[0]);
        vstore3(k * v2, 1, inout_i32 + offset);
      }
      offset += 6;
      // vector4
      {
        const int4 v1 = vload4(0, inout_i32 + offset);
        vstore4(k * v1, 0, inout_i32 + offset);
        vstore4(k * v1, 0, &storage[0].i32_[0]);
        const int4 v2 = vload4(0, &storage[0].i32_[0]);
        vstore4(k * v2, 1, inout_i32 + offset);
      }
    }
    // float 
    {
      constexpr float k = 2.0f;
      // vector2
      {
        const float2 v1 = vload2(0, inout_f);
        vstore2(k * v1, 0, inout_f);
        zivc::LocalPtr<float> p = &storage[0].f_[0];
        vstore2(k * v1, 0, p);
        const float2 v2 = vload2(0, p);
        vstore2(k * v2, 1, inout_f);
      }
      size_t offset = 4;
      // vector3
      {
        const float3 v1 = vload3(0, inout_f + offset);
        vstore3(k * v1, 0, inout_f + offset);
        vstore3(k * v1, 0, &storage[0].f_[0]);
        const float3 v2 = vload3(0, &storage[0].f_[0]);
        vstore3(k * v2, 1, inout_f + offset);
      }
      offset += 6;
      // vector4
      {
        const float4 v1 = vload4(0, inout_f + offset);
        vstore4(k * v1, 0, inout_f + offset);
        vstore4(k * v1, 0, &storage[0].f_[0]);
        const float4 v2 = vload4(0, &storage[0].f_[0]);
        vstore4(k * v2, 1, inout_f + offset);
      }
    }
  }
}

namespace inner {

struct LoadStoreLongVecStorage
{
  half h_[16];
  int32b i_[16];
  float f_[16];
};

static_assert(sizeof(LoadStoreLongVecStorage) % 16 == 0);

} // namespace inner

__kernel void vectorLoadStoreLongVecTest(zivc::GlobalPtr<int32b> inout_i32,
                                         zivc::GlobalPtr<float> inout_f,
                                         zivc::LocalPtr<inner::LoadStoreLongVecStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // int32b
    {
      constexpr int32b k = 2;
      size_t offset = 0;
      // vector8
      {
        const int8 v1 = zivc::vload8(0, inout_i32 + offset);
        zivc::vstore8(k * v1, 0, inout_i32 + offset);
        zivc::LocalPtr<int32b> p = &storage[0].i_[0];
        zivc::vstore8(k * v1, 0, p);
        const int8 v2 = zivc::vload8(0, p);
        zivc::vstore8(k * v2, 1, inout_i32 + offset);
      }
      offset += 16;
      // vector16
      {
        const int16 v1 = zivc::vload16(0, inout_i32 + offset);
        zivc::vstore16(k * v1, 0, inout_i32 + offset);
        zivc::LocalPtr<int32b> p = &storage[0].i_[0];
        zivc::vstore16(k * v1, 0, p);
        const int16 v2 = zivc::vload16(0, p);
        zivc::vstore16(k * v2, 1, inout_i32 + offset);
      }
    }
    // float 
    {
      constexpr float k = 2.0f;
      size_t offset = 0;
      // vector8
      {
        const float8 v1 = zivc::vload8(0, inout_f + offset);
        zivc::vstore8(k * v1, 0, inout_f + offset);
        zivc::LocalPtr<float> p = &storage[0].f_[0];
        zivc::vstore8(k * v1, 0, p);
        const float8 v2 = zivc::vload8(0, p);
        zivc::vstore8(k * v2, 1, inout_f + offset);
      }
      offset += 16;
      // vector16
      {
        const float16 v1 = zivc::vload16(0, inout_f + offset);
        zivc::vstore16(k * v1, 0, inout_f + offset);
        zivc::LocalPtr<float> p = &storage[0].f_[0];
        zivc::vstore16(k * v1, 0, p);
        const float16 v2 = zivc::vload16(0, p);
        zivc::vstore16(k * v2, 1, inout_f + offset);
      }
    }
  }
}

__kernel void vectorLoadStoreHalfTest(zivc::GlobalPtr<half> inout_h,
                                      zivc::LocalPtr<inner::LoadStoreStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // half 
    {
      constexpr float k = 2.0f;
      // Scalar
      {
        const float v1 = zivc::vload_half(0, inout_h);
        zivc::vstore_half(k * v1, 0, inout_h);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        zivc::vstore_half(k * v1, 0, p);
        const float v2 = zivc::vload_half(0, p);
        zivc::vstore_half(k * v2, 1, inout_h);
      }
      size_t offset = 2;
      // vector2
      {
        const float2 v1 = zivc::vload_half2(0, inout_h + offset);
        zivc::vstore_half2(k * v1, 0, inout_h + offset);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        zivc::vstore_half2(k * v1, 0, p);
        const float2 v2 = zivc::vload_half2(0, p);
        zivc::vstore_half2(k * v2, 1, inout_h + offset);
      }
      offset += 4;
      // vector3
      {
        const float3 v1 = zivc::vload_half3(0, inout_h + offset);
        zivc::vstore_half3(k * v1, 0, inout_h + offset);
        zivc::vstore_half3(k * v1, 0, &storage[0].h_[0]);
        const float3 v2 = zivc::vload_half3(0, &storage[0].h_[0]);
        zivc::vstore_half3(k * v2, 1, inout_h + offset);
      }
      offset += 6;
      // vector4
      {
        const float4 v1 = zivc::vload_half4(0, inout_h + offset);
        zivc::vstore_half4(k * v1, 0, inout_h + offset);
        zivc::vstore_half4(k * v1, 0, &storage[0].h_[0]);
        const float4 v2 = zivc::vload_half4(0, &storage[0].h_[0]);
        zivc::vstore_half4(k * v2, 1, inout_h + offset);
      }
    }
  }
}

__kernel void vectorLoadStoreHalfClTest(zivc::GlobalPtr<half> inout_h,
                                        zivc::LocalPtr<inner::LoadStoreStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // half 
    {
      constexpr float k = 2.0f;
      // Scalar
      {
        const float v1 = vload_half(0, inout_h);
        vstore_half(k * v1, 0, inout_h);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        vstore_half(k * v1, 0, p);
        const float v2 = vload_half(0, p);
        vstore_half(k * v2, 1, inout_h);
      }
      size_t offset = 2;
      // vector2
      {
        const float2 v1 = vload_half2(0, inout_h + offset);
        vstore_half2(k * v1, 0, inout_h + offset);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        vstore_half2(k * v1, 0, p);
        const float2 v2 = vload_half2(0, p);
        vstore_half2(k * v2, 1, inout_h + offset);
      }
      offset += 4;
      // vector3
      {
        const float3 v1 = vload_half3(0, inout_h + offset);
        vstore_half3(k * v1, 0, inout_h + offset);
        vstore_half3(k * v1, 0, &storage[0].h_[0]);
        const float3 v2 = vload_half3(0, &storage[0].h_[0]);
        vstore_half3(k * v2, 1, inout_h + offset);
      }
      offset += 6;
      // vector4
      {
        const float4 v1 = vload_half4(0, inout_h + offset);
        vstore_half4(k * v1, 0, inout_h + offset);
        vstore_half4(k * v1, 0, &storage[0].h_[0]);
        const float4 v2 = vload_half4(0, &storage[0].h_[0]);
        vstore_half4(k * v2, 1, inout_h + offset);
      }
    }
  }
}

__kernel void vectorLoadStoreHalfLongVecTest(zivc::GlobalPtr<half> inout_h,
                                             zivc::LocalPtr<inner::LoadStoreLongVecStorage> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    // half 
    {
      constexpr float k = 2.0f;
      size_t offset = 0;
      // vector8
      {
        const float8 v1 = zivc::vload_half8(0, inout_h + offset);
        zivc::vstore_half8(k * v1, 0, inout_h + offset);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        zivc::vstore_half8(k * v1, 0, p);
        const float8 v2 = zivc::vload_half8(0, p);
        zivc::vstore_half8(k * v2, 1, inout_h + offset);
      }
      offset += 16;
      // vector16
      {
        const float16 v1 = zivc::vload_half16(0, inout_h + offset);
        zivc::vstore_half16(k * v1, 0, inout_h + offset);
        zivc::LocalPtr<half> p = &storage[0].h_[0];
        zivc::vstore_half16(k * v1, 0, p);
        const float16 v2 = zivc::vload_half16(0, p);
        zivc::vstore_half16(k * v2, 1, inout_h + offset);
      }
    }
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_TYPE_CL */
