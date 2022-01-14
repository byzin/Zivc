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

__kernel void vectorComponentAccessTest(zivc::ConstGlobalPtr<float2> in_vec2,
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

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_VECTOR_CL */
