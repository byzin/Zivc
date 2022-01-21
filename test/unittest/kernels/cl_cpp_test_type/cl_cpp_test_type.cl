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
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"

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
    const int2 v{0, 1};
    output_vec2[index++] = v;
  }
  {
//    const int2 v{2};
//    output_vec2[index++] = v;
    output_vec2[index++] = 0;
  }
  {
    const int2 s{3, 4};
    const int2 v{s};
    output_vec2[index++] = v;
  }

  index = 0;
  // vector3
  {
    const int3 v{0, 1, 2};
    output_vec3[index++] = v;
  }
  {
    const int2 s{3, 4};
    const int3 v0{s, 5};
    output_vec3[index++] = v0;
    const int3 v1{5, s};
    output_vec3[index++] = v1;
  }
  {
//    const int3 v{6};
//    output_vec3[index++] = v;
    output_vec3[index++] = 0;
  }
  {
    const int3 s{7, 8, 9};
    const int3 v{s};
    output_vec3[index++] = v;
  }

  index = 0;
  // vector4
  {
    const int4 v{0, 1, 2, 3};
    output_vec4[index++] = v;
  }
  {
    const int2 s{4, 5};
    const int4 v0{s, 6, 7};
    output_vec4[index++] = v0;
    const int4 v1{6, s, 7};
    output_vec4[index++] = v1;
    const int4 v2{6, 7, s};
    output_vec4[index++] = v2;
    const int4 v3{s, s};
    output_vec4[index++] = v3;
  }
  {
    const int3 s{8, 9, 10};
    const int4 v0{s, 11};
    output_vec4[index++] = v0;
    const int4 v1{11, s};
    output_vec4[index++] = v1;
  }
  {
//    const int4 v{12};
//    output_vec4[index++] = v;
    output_vec4[index++] = 0;
  }
  {
    const int4 s{13, 14, 15, 16};
    const int4 v{s};
    output_vec4[index++] = v;
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_VECTOR_CL */
