/*!
  \file opencl_cpp_test_geometry.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_GEOMETRY
#define ZIVC_TEST_OPENCL_CPP_TEST_GEOMETRY

// Zivc
#include "zivc/cl/geometric.hpp"
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

__kernel void crossV3TestKernel(zivc::ConstGlobalPtr<float3> in,
                                zivc::GlobalPtr<float3> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::cross(in[2 * index], in[2 * index + 1]);
}

__kernel void crossV4TestKernel(zivc::ConstGlobalPtr<float4> in,
                                zivc::GlobalPtr<float4> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::cross(in[2 * index], in[2 * index + 1]);
}

__kernel void dotV2TestKernel(zivc::ConstGlobalPtr<float2> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::dot(in[2 * index], in[2 * index + 1]);
}

__kernel void dotV3TestKernel(zivc::ConstGlobalPtr<float3> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::dot(in[2 * index], in[2 * index + 1]);
}

__kernel void dotV4TestKernel(zivc::ConstGlobalPtr<float4> in,
                              zivc::GlobalPtr<float> out,
                              const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::dot(in[2 * index], in[2 * index + 1]);
}

//__kernel void dotV8TestKernel(zivc::ConstGlobalPtr<float8> in,
//                              zivc::GlobalPtr<float> out,
//                              const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::dot(in[2 * index], in[2 * index + 1]);
//}
//
//__kernel void dotV16TestKernel(zivc::ConstGlobalPtr<float16> in,
//                               zivc::GlobalPtr<float> out,
//                               const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::dot(in[2 * index], in[2 * index + 1]);
//}

__kernel void dotV4C3TestKernel(zivc::ConstGlobalPtr<float4> in,
                                zivc::GlobalPtr<float> out,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::dot3(in[2 * index], in[2 * index + 1]);
}

__kernel void distanceV2TestKernel(zivc::ConstGlobalPtr<float2> in,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::distance(in[2 * index], in[2 * index + 1]);
}

__kernel void distanceV3TestKernel(zivc::ConstGlobalPtr<float3> in,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::distance(in[2 * index], in[2 * index + 1]);
}

__kernel void distanceV4TestKernel(zivc::ConstGlobalPtr<float4> in,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::distance(in[2 * index], in[2 * index + 1]);
}

//__kernel void distanceV8TestKernel(zivc::ConstGlobalPtr<float8> in,
//                                   zivc::GlobalPtr<float> out,
//                                   const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::distance(in[2 * index], in[2 * index + 1]);
//}
//
//__kernel void distanceV16TestKernel(zivc::ConstGlobalPtr<float16> in,
//                                    zivc::GlobalPtr<float> out,
//                                    const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::distance(in[2 * index], in[2 * index + 1]);
//}

__kernel void distanceV4C3TestKernel(zivc::ConstGlobalPtr<float4> in,
                                     zivc::GlobalPtr<float> out,
                                     const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::distance3(in[2 * index], in[2 * index + 1]);
}

__kernel void lengthV2TestKernel(zivc::ConstGlobalPtr<float2> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::length(in[index]);
}

__kernel void lengthV3TestKernel(zivc::ConstGlobalPtr<float3> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::length(in[index]);
}

__kernel void lengthV4TestKernel(zivc::ConstGlobalPtr<float4> in,
                                 zivc::GlobalPtr<float> out,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::length(in[index]);
}

//__kernel void lengthV8TestKernel(zivc::ConstGlobalPtr<float8> in,
//                                 zivc::GlobalPtr<float> out,
//                                 const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::length(in[index]);
//}
//
//__kernel void lengthV16TestKernel(zivc::ConstGlobalPtr<float16> in,
//                                  zivc::GlobalPtr<float> out,
//                                  const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::length(in[index]);
//}

__kernel void lengthV4C3TestKernel(zivc::ConstGlobalPtr<float4> in,
                                   zivc::GlobalPtr<float> out,
                                   const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::length3(in[index]);
}

__kernel void normalizeV2TestKernel(zivc::ConstGlobalPtr<float2> in,
                                    zivc::GlobalPtr<float2> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::normalize(in[index]);
}

__kernel void normalizeV3TestKernel(zivc::ConstGlobalPtr<float3> in,
                                    zivc::GlobalPtr<float3> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::normalize(in[index]);
}

__kernel void normalizeV4TestKernel(zivc::ConstGlobalPtr<float4> in,
                                    zivc::GlobalPtr<float4> out,
                                    const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::normalize(in[index]);
}

//__kernel void normalizeV8TestKernel(zivc::ConstGlobalPtr<float8> in,
//                                    zivc::GlobalPtr<float8> out,
//                                    const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::normalize(in[index]);
//}
//
//__kernel void normalizeV16TestKernel(zivc::ConstGlobalPtr<float16> in,
//                                     zivc::GlobalPtr<float16> out,
//                                     const uint32b resolution)
//{
//  const size_t index = zivc::getGlobalLinearId();
//  if (resolution <= index)
//    return;
//
//  out[index] = zivc::normalize(in[index]);
//}

__kernel void normalizeV4C3TestKernel(zivc::ConstGlobalPtr<float4> in,
                                      zivc::GlobalPtr<float4> out,
                                      const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  out[index] = zivc::normalize3(in[index]);
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_GEOMETRY */
