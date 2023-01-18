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

    //! \todo subnormal won't work on GPU
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

    //! \todo subnormal won't work on GPU
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

    //! \todo subnormal won't work on GPU
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

    //! \todo subnormal won't work on GPU
    out4[6] = zivc::isequal(v4, v4);
    out4[7] = zivc::isequal(v3, v4);

    out4[8] = zivc::isequal(v5, v5);
    out4[9] = zivc::isequal(v5, v2);
    out4[10] = zivc::isequal(v6, v6);
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_RELATION */
