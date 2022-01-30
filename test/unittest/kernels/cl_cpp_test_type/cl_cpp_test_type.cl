/*!
  \file opencl_cpp_test_vector.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_VECTOR_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_VECTOR_CL

// Zivc
#include "zivc/cl/boolean.hpp"
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
ZIVC_CHECK_TYPE_SIZE(unsigned char, 1);
ZIVC_CHECK_TYPE_SIZE(uchar, 1);
ZIVC_CHECK_TYPE_SIZE(zivc::uint8b, 1);
ZIVC_CHECK_TYPE_SIZE(uchar2, 2);
ZIVC_CHECK_TYPE_SIZE(uchar3, 4);
ZIVC_CHECK_TYPE_SIZE(uchar4, 4);
ZIVC_CHECK_TYPE_SIZE(signed short, 2);
ZIVC_CHECK_TYPE_SIZE(short, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::int16b, 2);
ZIVC_CHECK_TYPE_SIZE(short2, 4);
ZIVC_CHECK_TYPE_SIZE(short3, 8);
ZIVC_CHECK_TYPE_SIZE(short4, 8);
ZIVC_CHECK_TYPE_SIZE(unsigned short, 2);
ZIVC_CHECK_TYPE_SIZE(ushort, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::uint16b, 2);
ZIVC_CHECK_TYPE_SIZE(ushort2, 4);
ZIVC_CHECK_TYPE_SIZE(ushort3, 8);
ZIVC_CHECK_TYPE_SIZE(ushort4, 8);
ZIVC_CHECK_TYPE_SIZE(signed int, 4);
ZIVC_CHECK_TYPE_SIZE(int, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::int32b, 4);
ZIVC_CHECK_TYPE_SIZE(int2, 8);
ZIVC_CHECK_TYPE_SIZE(int3, 16);
ZIVC_CHECK_TYPE_SIZE(int4, 16);
ZIVC_CHECK_TYPE_SIZE(unsigned int, 4);
ZIVC_CHECK_TYPE_SIZE(uint, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::uint32b, 4);
ZIVC_CHECK_TYPE_SIZE(uint2, 8);
ZIVC_CHECK_TYPE_SIZE(uint3, 16);
ZIVC_CHECK_TYPE_SIZE(uint4, 16);
ZIVC_CHECK_TYPE_SIZE(signed long, 8);
ZIVC_CHECK_TYPE_SIZE(long, 8);
ZIVC_CHECK_TYPE_SIZE(zivc::int64b, 8);
ZIVC_CHECK_TYPE_SIZE(long2, 16);
ZIVC_CHECK_TYPE_SIZE(long3, 32);
ZIVC_CHECK_TYPE_SIZE(long4, 32);
ZIVC_CHECK_TYPE_SIZE(unsigned long, 8);
ZIVC_CHECK_TYPE_SIZE(ulong, 8);
ZIVC_CHECK_TYPE_SIZE(zivc::uint64b, 8);
ZIVC_CHECK_TYPE_SIZE(ulong2, 16);
ZIVC_CHECK_TYPE_SIZE(ulong3, 32);
ZIVC_CHECK_TYPE_SIZE(ulong4, 32);
ZIVC_CHECK_TYPE_SIZE(half, 2);
ZIVC_CHECK_TYPE_SIZE(half2, 4);
ZIVC_CHECK_TYPE_SIZE(half3, 8);
ZIVC_CHECK_TYPE_SIZE(half4, 8);
ZIVC_CHECK_TYPE_SIZE(float, 4);
ZIVC_CHECK_TYPE_SIZE(float2, 8);
ZIVC_CHECK_TYPE_SIZE(float3, 16);
ZIVC_CHECK_TYPE_SIZE(float4, 16);
ZIVC_CHECK_TYPE_SIZE(double, 8);
ZIVC_CHECK_TYPE_SIZE(double2, 16);
ZIVC_CHECK_TYPE_SIZE(double3, 32);
ZIVC_CHECK_TYPE_SIZE(double4, 32);

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
    const int2 s = zivc::make<int2>(3, 4);
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
    const int3 s = zivc::make<int3>(7, 8, 9);
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
    const int4 s = zivc::make<int4>(13, 14, 15, 16);
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
    { \
      auto v = zivc::make< vector_type ## 2 >( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 2 >( v0, v1 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
  } \
  { \
    using Vec2 = zivc::Private<vector_type ## 2>; \
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
    { \
      auto v = zivc::make< vector_type ## 3 >( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 3 >( v0, v1, v2 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 3 >( Vec2{v0, v1}, v2 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 3 >( v0, Vec2{v1, v2} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
  } \
  { \
    using Vec2 = zivc::Private<vector_type ## 2>; \
    using Vec3 = zivc::Private<vector_type ## 3>; \
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
    { \
      auto v = zivc::make< vector_type ## 4 >( v0 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( v0, v1, v2, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( Vec2{v0, v1}, v2, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( v0, Vec2{v1, v2}, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( v0, v1, Vec2{v2, v3} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( Vec2{v0, v1}, Vec2{v2, v3} ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( Vec3{v0, v1, v2}, v3 ); \
      using ResultType = decltype(v); \
      static_assert(zivc::kIsSame<VectorType, ResultType>); \
    } \
    { \
      auto v = zivc::make< vector_type ## 4 >( v0, Vec3{v1, v2, v3} ); \
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
    out_int2[oindex] = -in_int2[iindex];
  }
  {
    out_int3[oindex] = -in_int3[iindex];
  }
  {
    out_int4[oindex] = -in_int4[iindex];
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
    out_int2[oindex] = v0 + v1;
    out_int2[oindex + 1] = v0.x + v1;
    out_int2[oindex + 2] = v0 + v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0 + v1;
    out_int3[oindex + 1] = v0.x + v1;
    out_int3[oindex + 2] = v0 + v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0 + v1;
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
    out_int2[oindex] = v0 - v1;
    out_int2[oindex + 1] = v0.x - v1;
    out_int2[oindex + 2] = v0 - v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0 - v1;
    out_int3[oindex + 1] = v0.x - v1;
    out_int3[oindex + 2] = v0 - v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0 - v1;
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
    out_int2[oindex] = v0 * v1;
    out_int2[oindex + 1] = v0.x * v1;
    out_int2[oindex + 2] = v0 * v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0 * v1;
    out_int3[oindex + 1] = v0.x * v1;
    out_int3[oindex + 2] = v0 * v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0 * v1;
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
    out_int2[oindex] = v0 / v1;
    out_int2[oindex + 1] = v0.x / v1;
    out_int2[oindex + 2] = v0 / v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0 / v1;
    out_int3[oindex + 1] = v0.x / v1;
    out_int3[oindex + 2] = v0 / v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0 / v1;
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
    out_int2[oindex] = v0 % v1;
    out_int2[oindex + 1] = v0.x % v1;
    out_int2[oindex + 2] = v0 % v1.x;
  }
  {
    const int3 v0 = in_int3[iindex];
    const int3 v1 = in_int3[iindex + 1];
    out_int3[oindex] = v0 % v1;
    out_int3[oindex + 1] = v0.x % v1;
    out_int3[oindex + 2] = v0 % v1.x;
  }
  {
    const int4 v0 = in_int4[iindex];
    const int4 v1 = in_int4[iindex + 1];
    out_int4[oindex] = v0 % v1;
    out_int4[oindex + 1] = v0.x % v1;
    out_int4[oindex + 2] = v0 % v1.x;
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
    out_uint2[oindex] = ~v;
  }
  {
    const uint3 v = in_uint3[iindex + 1];
    out_uint3[oindex] = ~v;
  }
  {
    const uint4 v = in_uint4[iindex + 1];
    out_uint4[oindex] = ~v;
  }
  ++oindex;
  // bitwise AND
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0 & v1;
    out_uint2[oindex + 1] = v0.x & v1;
    out_uint2[oindex + 2] = v0 & v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0 & v1;
    out_uint3[oindex + 1] = v0.x & v1;
    out_uint3[oindex + 2] = v0 & v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0 & v1;
    out_uint4[oindex + 1] = v0.x & v1;
    out_uint4[oindex + 2] = v0 & v1.x;
  }
  ++iindex;
  oindex += 3;
  // bitwise OR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0 | v1;
    out_uint2[oindex + 1] = v0.x | v1;
    out_uint2[oindex + 2] = v0 | v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0 | v1;
    out_uint3[oindex + 1] = v0.x | v1;
    out_uint3[oindex + 2] = v0 | v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0 | v1;
    out_uint4[oindex + 1] = v0.x | v1;
    out_uint4[oindex + 2] = v0 | v1.x;
  }
  oindex += 3;
  // bitwise XOR
  {
    const uint2 v0 = in_uint2[iindex];
    const uint2 v1 = in_uint2[iindex + 1];
    out_uint2[oindex] = v0 ^ v1;
    out_uint2[oindex + 1] = v0.x ^ v1;
    out_uint2[oindex + 2] = v0 ^ v1.x;
  }
  {
    const uint3 v0 = in_uint3[iindex];
    const uint3 v1 = in_uint3[iindex + 1];
    out_uint3[oindex] = v0 ^ v1;
    out_uint3[oindex + 1] = v0.x ^ v1;
    out_uint3[oindex + 2] = v0 ^ v1.x;
  }
  {
    const uint4 v0 = in_uint4[iindex];
    const uint4 v1 = in_uint4[iindex + 1];
    out_uint4[oindex] = v0 ^ v1;
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

namespace test {

template <typename ResultType> inline
void setBoolResult(const ResultType result, zivc::GlobalPtr<zivc::Boolean> out)
{
  out[0] = result.x;
  out[1] = result.y;
  out[2] = result.z;
  out[3] = result.w;
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
}

__kernel void scalarCastTest(zivc::ConstGlobalPtr<int32b> in_int,
                             zivc::ConstGlobalPtr<float> in_float,
                             zivc::GlobalPtr<int8b> out_int8b,
                             zivc::GlobalPtr<uint8b> out_uint8b,
                             zivc::GlobalPtr<int16b> out_int16b,
                             zivc::GlobalPtr<uint16b> out_uint16b,
                             zivc::GlobalPtr<int32b> out_int32b,
                             zivc::GlobalPtr<uint32b> out_uint32b,
                             zivc::GlobalPtr<float> out_float,
                             zivc::GlobalPtr<zivc::Boolean> out_bool,
                             const uint32b int_n,
                             const uint32b float_n)
{
  const size_t global_index = zivc::getGlobalIdX();
  if (global_index != 0)
    return;

  auto test_scalar = [int_n, float_n](zivc::ConstGlobalPtr<int32b> in0,
                                      zivc::ConstGlobalPtr<float> in1,
                                      auto out) noexcept
  {
    size_t index = 0;
    using Type = zivc::RemoveCvRefAddressT<decltype(out[0])>;
    for (size_t i = 0; i < int_n; ++i)
      out[index++] = zivc::cast<Type>(in0[i]);
    for (size_t i = 0; i < float_n; ++i)
      out[index++] = zivc::cast<Type>(in1[i]);
  };

  test_scalar(in_int, in_float, out_int8b);
  test_scalar(in_int, in_float, out_uint8b);
  test_scalar(in_int, in_float, out_int16b);
  test_scalar(in_int, in_float, out_uint16b);
  test_scalar(in_int, in_float, out_int32b);
  test_scalar(in_int, in_float, out_uint32b);
  test_scalar(in_int, in_float, out_float);

  // bool
  {
    size_t index = 0;
    for (size_t i = 0; i < int_n; ++i)
      out_bool[index++] = zivc::cast<bool>(in_int[i]);
    for (size_t i = 0; i < float_n; ++i)
      out_bool[index++] = zivc::cast<bool>(in_float[i]);
  }
}

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

  auto test_vec2 = [int_n, float_n, int2_n, float2_n](zivc::ConstGlobalPtr<int32b> in0,
                                                      zivc::ConstGlobalPtr<float> in1,
                                                      zivc::ConstGlobalPtr<int2> in2,
                                                      zivc::ConstGlobalPtr<float2> in3,
                                                      auto out) noexcept
  {
    size_t index = 0;
    using Type = zivc::RemoveCvRefAddressT<decltype(out[0])>;
//    for (size_t i = 0; i < int_n; ++i)
//      out[index++] = zivc::cast<Type>(in0[i]);
//    for (size_t i = 0; i < float_n; ++i)
//      out[index++] = zivc::cast<Type>(in1[i]);
    for (size_t i = 0; i < int2_n; ++i)
      out[index++] = zivc::cast<Type>(in2[i]);
    for (size_t i = 0; i < float2_n; ++i)
      out[index++] = zivc::cast<Type>(in3[i]);
  };

  test_vec2(in_int, in_float, in_int2, in_float2, out_char2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_uchar2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_short2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_ushort2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_int2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_uint2);
  test_vec2(in_int, in_float, in_int2, in_float2, out_float2);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_VECTOR_CL */
