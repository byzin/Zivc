/*!
  \file kernel_test.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST_CL
#define ZIVC_TEST_KERNEL_TEST_CL

// Zivc
#include "zivc/cl/atomic.cl"
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"

using zivc::int8b;
using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint8b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

namespace inner {

void writeWorkItemProperties(zivc::GlobalPtr<uint32b> properties) noexcept;

}

/*!
  \details No detailed description

  \param [in] inputs No description.
  */
__kernel void simpleKernel(zivc::ConstGlobalPtr<int32b> inputs)
{
  static_cast<void>(inputs); //!< Remove me
  // Do nothing here
}

/*!
  \details No detailed description

  \param [out] outputs No description.
  */
__kernel void basicTypeSizeKernel(zivc::GlobalPtr<uint32b> outputs)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;

    outputs[i++] = sizeof(int8b);
    outputs[i++] = alignof(int8b);
    outputs[i++] = sizeof(int16b);
    outputs[i++] = alignof(int16b);
    outputs[i++] = sizeof(int32b);
    outputs[i++] = alignof(int32b);
    outputs[i++] = sizeof(int64b);
    outputs[i++] = alignof(int64b);

    outputs[i++] = sizeof(char2);
    outputs[i++] = alignof(char2);
    outputs[i++] = sizeof(char3);
    outputs[i++] = alignof(char3);
    outputs[i++] = sizeof(char4);
    outputs[i++] = alignof(char4);
    outputs[i++] = sizeof(short2);
    outputs[i++] = alignof(short2);
    outputs[i++] = sizeof(short3);
    outputs[i++] = alignof(short3);
    outputs[i++] = sizeof(short4);
    outputs[i++] = alignof(short4);
    outputs[i++] = sizeof(int2);
    outputs[i++] = alignof(int2);
    outputs[i++] = sizeof(int3);
    outputs[i++] = alignof(int3);
    outputs[i++] = sizeof(int4);
    outputs[i++] = alignof(int4);
    outputs[i++] = sizeof(long2);
    outputs[i++] = alignof(long2);
    outputs[i++] = sizeof(long3);
    outputs[i++] = alignof(long3);
    outputs[i++] = sizeof(long4);
    outputs[i++] = alignof(long4);

    outputs[i++] = sizeof(uint8b);
    outputs[i++] = alignof(uint8b);
    outputs[i++] = sizeof(uint16b);
    outputs[i++] = alignof(uint16b);
    outputs[i++] = sizeof(uint32b);
    outputs[i++] = alignof(uint32b);
    outputs[i++] = sizeof(uint64b);
    outputs[i++] = alignof(uint64b);

    outputs[i++] = sizeof(uchar2);
    outputs[i++] = alignof(uchar2);
    outputs[i++] = sizeof(uchar3);
    outputs[i++] = alignof(uchar3);
    outputs[i++] = sizeof(uchar4);
    outputs[i++] = alignof(uchar4);
    outputs[i++] = sizeof(ushort2);
    outputs[i++] = alignof(ushort2);
    outputs[i++] = sizeof(ushort3);
    outputs[i++] = alignof(ushort3);
    outputs[i++] = sizeof(ushort4);
    outputs[i++] = alignof(ushort4);
    outputs[i++] = sizeof(uint2);
    outputs[i++] = alignof(uint2);
    outputs[i++] = sizeof(uint3);
    outputs[i++] = alignof(uint3);
    outputs[i++] = sizeof(uint4);
    outputs[i++] = alignof(uint4);
    outputs[i++] = sizeof(ulong2);
    outputs[i++] = alignof(ulong2);
    outputs[i++] = sizeof(ulong3);
    outputs[i++] = alignof(ulong3);
    outputs[i++] = sizeof(ulong4);
    outputs[i++] = alignof(ulong4);

    outputs[i++] = sizeof(half);
    outputs[i++] = alignof(half);
    outputs[i++] = sizeof(float);
    outputs[i++] = alignof(float);
    outputs[i++] = sizeof(double);
    outputs[i++] = alignof(double);

    outputs[i++] = sizeof(half2);
    outputs[i++] = alignof(half2);
    outputs[i++] = sizeof(half3);
    outputs[i++] = alignof(half3);
    outputs[i++] = sizeof(half4);
    outputs[i++] = alignof(half4);
    outputs[i++] = sizeof(float2);
    outputs[i++] = alignof(float2);
    outputs[i++] = sizeof(float3);
    outputs[i++] = alignof(float3);
    outputs[i++] = sizeof(float4);
    outputs[i++] = alignof(float4);
    outputs[i++] = sizeof(double2);
    outputs[i++] = alignof(double2);
    outputs[i++] = sizeof(double3);
    outputs[i++] = alignof(double3);
    outputs[i++] = sizeof(double4);
    outputs[i++] = alignof(double4);

    outputs[i++] = sizeof(size_t);
    outputs[i++] = alignof(size_t);
    outputs[i++] = sizeof(ptrdiff_t);
    outputs[i++] = alignof(ptrdiff_t);
    outputs[i++] = sizeof(intptr_t);
    outputs[i++] = alignof(intptr_t);
    outputs[i++] = sizeof(uintptr_t);
    outputs[i++] = alignof(uintptr_t);
  }
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif // Z_GCC || Z_CLANG

/*!
  \brief No brief description

  No detailed description.
  */
class ClassSizeTest1
{
 public:
  int16b i16() const noexcept {return i16_;}
  uint32b u() const noexcept {return u_;}
  uint8b u8() const noexcept {return u8_;}
  uint16b u16() const noexcept {return u16_;}
  float f() const noexcept {return f_;}

  int16b i16_;
  uint32b u_;
  uint8b u8_;
  uint16b u16_;
  float f_;
};

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \param [out] outputs No description.
  */
__kernel void classSize1Kernel(zivc::GlobalPtr<uint32b> outputs)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;

    outputs[i++] = sizeof(ClassSizeTest1);
    outputs[i++] = alignof(ClassSizeTest1);
  }
}

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  */
__kernel void inputOutput1Kernel(zivc::ConstGlobalPtr<int32b> inputs,
                                 zivc::GlobalPtr<int32b> outputs)
{
  const size_t index = zivc::getGlobalIdX();
  outputs[index] = inputs[index];
}

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  */
__kernel void inputOutput2Kernel(zivc::ConstGlobalPtr<ClassSizeTest1> inputs,
                                 zivc::GlobalPtr<ClassSizeTest1> outputs)
{
  const size_t index = zivc::getGlobalIdX();
  if (256 <= index)
    return;

  zivc::ConstGlobalPtr<ClassSizeTest1> in = inputs + index;
  {
    zivc::GlobalPtr<ClassSizeTest1> o1 = outputs + (2 * index);
    o1->i16_ = in->i16();
    o1->u_ = in->u();
    o1->u8_ = in->u8();
    o1->u16_ = in->u16();
    o1->f_ = in->f();
  }
  {
    zivc::GlobalPtr<ClassSizeTest1> o2 = outputs + (2 * index + 1);
    o2[0] = in[0];
  }
}

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [in] value No description.
  */
__kernel void pod1Kernel(zivc::GlobalPtr<uint32b> outputs, const uint32b value)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    outputs[0] = value;
  }
}

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void pod2Kernel(zivc::ConstGlobalPtr<int32b> inputs,
                         zivc::GlobalPtr<int32b> outputs,
                         const uint32b resolution)
{
  const size_t index = zivc::getGlobalIdX();
  if (resolution <= index)
    return;

  const int32b v = (index == 0) ? static_cast<int32b>(resolution) : inputs[index];
  outputs[index] = v;
}

/*!
  \details No detailed description

  \param [in] i0 No description.
  \param [in] i1 No description.
  \param [in] i2 No description.
  \param [in] u0 No description.
  \param [in] u1 No description.
  \param [in] u2 No description.
  \param [in] f0 No description.
  \param [in] f1 No description.
  \param [in] f2 No description.
  \param [out] ioutputs No description.
  \param [out] uoutputs No description.
  \param [out] foutputs No description.
  */
__kernel void pod3Kernel(const int32b i0, const uint32b u0, const float f0,
                         const int32b i1, const uint32b u1, const float f1,
                         const int32b i2, const uint32b u2, const float f2,
                         zivc::GlobalPtr<int32b> ioutputs,
                         zivc::GlobalPtr<uint32b> uoutputs,
                         zivc::GlobalPtr<float> foutputs)
{
  const size_t index = zivc::getGlobalIdX();
  if (index != 0)
    return;

  ioutputs[0] = i0;
  ioutputs[1] = i1;
  ioutputs[2] = i2;
  uoutputs[0] = u0;
  uoutputs[1] = u1;
  uoutputs[2] = u2;
  foutputs[0] = f0;
  foutputs[1] = f1;
  foutputs[2] = f2;
}

namespace inner {

/*!
  \brief No brief description

  No detailed description.
  */
struct PodAlignmentTest
{
  uint8b u8_ = 0;
  uint8b padding1_1_ = 0;
  uint8b padding1_2_ = 0;
  uint8b padding1_3_ = 0;

  float f_ = 0.0f;

  int8b i8_ = 0;
  int8b padding2_ = 0;
  int16b i16_ = 0;

  uint16b u16_ = 0;
  uint16b padding3_ = 0;

  int32b i32_ = 0;

  int32b padding4_1_ = 0;
  int32b padding4_2_ = 0;
  int32b padding4_3_ = 0;
};

//! Check if two values are equal
bool operator==(const PodAlignmentTest& lhs, const PodAlignmentTest& rhs) noexcept;

//! Check if two values aren't equal
bool operator!=(const PodAlignmentTest& lhs, const PodAlignmentTest& rhs) noexcept;

inline
bool operator==(const PodAlignmentTest& lhs, const PodAlignmentTest& rhs) noexcept
{
  const bool result = (lhs.u8_ == rhs.u8_) && (lhs.f_ == rhs.f_) &&
                      (lhs.i8_ == rhs.i8_) && (lhs.i16_ == rhs.i16_) &&
                      (lhs.u16_ == rhs.u16_) && (lhs.i32_ == rhs.i32_);
  return result;
}

inline
bool operator!=(const PodAlignmentTest& lhs, const PodAlignmentTest& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

static_assert(alignof(PodAlignmentTest) == 4);

} // namespace inner

/*!
  \details No detailed description

  \param [out] output1 No description.
  \param [out] output2 No description.
  \param [out] output3 No description.
  \param [out] output4 No description.
  \param [out] output5 No description.
  \param [out] output6 No description.
  \param [out] output7 No description.
  \param [in] u8 No description.
  \param [in] f No description.
  \param [in] i8 No description.
  \param [in] i16 No description.
  \param [in] u16 No description.
  \param [in] i32 No description.
  \param [in] test No description.
  */
__kernel void podSizeAlignmentKernel(zivc::GlobalPtr<uint8b> output1,
                                     zivc::GlobalPtr<float> output2,
                                     zivc::GlobalPtr<int8b> output3,
                                     zivc::GlobalPtr<int16b> output4,
                                     zivc::GlobalPtr<uint16b> output5,
                                     zivc::GlobalPtr<int32b> output6,
                                     zivc::GlobalPtr<inner::PodAlignmentTest> output7,
                                     const uint8b u8,
                                     const float f,
                                     const int8b i8,
                                     const int16b i16,
                                     const uint16b u16,
                                     const int32b i32,
                                     const inner::PodAlignmentTest test)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    output1[0] = u8;
    output1[1] = test.u8_;
    output2[0] = f;
    output2[1] = test.f_;
    output3[0] = i8;
    output3[1] = test.i8_;
    output4[0] = i16;
    output4[1] = test.i16_;
    output5[0] = u16;
    output5[1] = test.u16_;
    output6[0] = i32;
    output6[1] = test.i32_;
    output7[0].u8_ = u8;
    output7[0].f_ = f;
    output7[0].i8_ = i8;
    output7[0].i16_ = i16;
    output7[0].u16_ = u16;
    output7[0].i32_ = i32;
    output7[1] = test;
  }
}


namespace inner {

/*!
  \details No detailed description

  \param [out] properties No description.
  */
inline
void writeWorkItemProperties(zivc::GlobalPtr<uint32b> properties) noexcept
{
  size_t i = 0;
  properties[i++] = zivc::getWorkDim();
  properties[i++] = static_cast<uint32b>(zivc::getGlobalSizeX());
  properties[i++] = static_cast<uint32b>(zivc::getGlobalSizeY());
  properties[i++] = static_cast<uint32b>(zivc::getGlobalSizeZ());
  properties[i++] = static_cast<uint32b>(zivc::getLocalSizeX());
  properties[i++] = static_cast<uint32b>(zivc::getLocalSizeY());
  properties[i++] = static_cast<uint32b>(zivc::getLocalSizeZ());
  properties[i++] = static_cast<uint32b>(zivc::getEnqueuedLocalSizeX());
  properties[i++] = static_cast<uint32b>(zivc::getEnqueuedLocalSizeY());
  properties[i++] = static_cast<uint32b>(zivc::getEnqueuedLocalSizeZ());
  properties[i++] = static_cast<uint32b>(zivc::getNumGroupsX());
  properties[i++] = static_cast<uint32b>(zivc::getNumGroupsY());
  properties[i++] = static_cast<uint32b>(zivc::getNumGroupsZ());
  properties[i++] = static_cast<uint32b>(zivc::getGlobalOffsetX());
  properties[i++] = static_cast<uint32b>(zivc::getGlobalOffsetY());
  properties[i++] = static_cast<uint32b>(zivc::getGlobalOffsetZ());
}

} // namespace inner

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [out] properties No description.
  \param [in] resolution No description.
  */
__kernel void workItem1dKernel(zivc::GlobalPtr<uint32b> outputs,
                               zivc::GlobalPtr<uint32b> properties,
                               const uint32b resolution)
{
  const size_t wx = zivc::getGroupIdX();

  // Global ID test
  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    if (wx == 0) {
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 1);
      zivc::atomic_inc(num_of_work_groups);
    }

    if (index == 0) {
      inner::writeWorkItemProperties(properties);
      const size_t nlx = zivc::getLocalSizeX();
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 2);
      num_of_work_groups[0] = static_cast<uint32b>(nlx);
    }

    zivc::GlobalPtr<uint32b> ptr = &outputs[index];
    zivc::atomic_inc(ptr);
  }
  else {
    zivc::GlobalPtr<uint32b> outlier = outputs + resolution;
    zivc::atomic_inc(outlier);
  }

  // Local ID test
  const size_t local_id = zivc::getLocalLinearId();
  const size_t nlx = zivc::getLocalSizeX();
  const size_t group_size = nlx;
  const size_t group_id = wx;
  const size_t index2 = local_id + group_size * group_id;
  if (index2 < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index2];
    zivc::atomic_inc(ptr);
  }
}

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [out] properties No description.
  \param [in] resolution No description.
  */
__kernel void workItem2dKernel(zivc::GlobalPtr<uint32b> outputs,
                               zivc::GlobalPtr<uint32b> properties,
                               const uint32b resolution)
{
  const size_t wx = zivc::getGroupIdX();
  const size_t wy = zivc::getGroupIdY();

  // Global ID test
  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    if ((wx + wy) == 0) {
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 1);
      zivc::atomic_inc(num_of_work_groups);
    }

    if (index == 0) {
      inner::writeWorkItemProperties(properties);
      const size_t nlx = zivc::getLocalSizeX();
      const size_t nly = zivc::getLocalSizeY();
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 2);
      num_of_work_groups[0] = static_cast<uint32b>(nlx * nly);
    }

    zivc::GlobalPtr<uint32b> ptr = &outputs[index];
    zivc::atomic_inc(ptr);
  }
  else {
    zivc::GlobalPtr<uint32b> outlier = outputs + resolution;
    zivc::atomic_inc(outlier);
  }

  // Local ID test
  const size_t local_id = zivc::getLocalLinearId();
  const size_t nlx = zivc::getLocalSizeX();
  const size_t nly = zivc::getLocalSizeY();
  const size_t group_size = nlx * nly;
  const size_t nwx = zivc::getNumGroupsX();
  const size_t group_id = wx + nwx * wy;
  const size_t index2 = local_id + group_size * group_id;
  if (index2 < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index2];
    zivc::atomic_inc(ptr);
  }
}

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [out] properties No description.
  \param [in] resolution No description.
  */
__kernel void workItem3dKernel(zivc::GlobalPtr<uint32b> outputs,
                               zivc::GlobalPtr<uint32b> properties,
                               const uint32b resolution)
{
  const size_t wx = zivc::getGroupIdX();
  const size_t wy = zivc::getGroupIdY();
  const size_t wz = zivc::getGroupIdZ();

  // Global ID test
  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    if ((wx + wy + wz) == 0) {
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 1);
      zivc::atomic_inc(num_of_work_groups);
    }

    if (index == 0) {
      inner::writeWorkItemProperties(properties);
      const size_t nlx = zivc::getLocalSizeX();
      const size_t nly = zivc::getLocalSizeY();
      const size_t nlz = zivc::getLocalSizeZ();
      zivc::GlobalPtr<uint32b> num_of_work_groups = outputs + (resolution + 2);
      num_of_work_groups[0] = static_cast<uint32b>(nlx * nly * nlz);
    }

    zivc::GlobalPtr<uint32b> ptr = &outputs[index];
    zivc::atomic_inc(ptr);
  }
  else {
    zivc::GlobalPtr<uint32b> outlier = outputs + resolution;
    zivc::atomic_inc(outlier);
  }

  // Local ID test
  const size_t local_id = zivc::getLocalLinearId();
  const size_t nlx = zivc::getLocalSizeX();
  const size_t nly = zivc::getLocalSizeY();
  const size_t nlz = zivc::getLocalSizeZ();
  const size_t group_size = nlx * nly * nlz;
  const size_t nwx = zivc::getNumGroupsX();
  const size_t nwy = zivc::getNumGroupsY();
  const size_t group_id = wx + nwx * wy + (nwx * nwy) * wz;
  const size_t index2 = local_id + group_size * group_id;
  if (index2 < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index2];
    zivc::atomic_inc(ptr);
  }
}

namespace inner {

/*!
  \brief No brief description

  No detailed description.
  */
struct GlobalId3d
{
  uint32b x_ = 0;
  uint32b y_ = 0;
  uint32b z_ = 0;
};

} // namespace inner

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [out] ids No description.
  \param [out] properties No description.
  \param [in] resolution No description.
  */
__kernel void workItemOffset1dKernel(zivc::GlobalPtr<uint32b> outputs,
                                     zivc::GlobalPtr<uint32b> outputs2,
                                     zivc::GlobalPtr<uint32b> ids,
                                     zivc::GlobalPtr<uint32b> properties,
                                     const uint32b resolution)
{
  // Global ID test
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    properties[0] = static_cast<uint32b>(zivc::getGlobalOffsetX());
  }
  if (index < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index];
    zivc::atomic_inc(ptr);
    ids[2 * index + 0] = static_cast<uint32b>(zivc::getGlobalIdX() -
                                              zivc::getGlobalOffsetX());
    ids[2 * index + 1] = static_cast<uint32b>(zivc::getGlobalIdX());
  }

  // Offset test
  {
    const size_t i = zivc::getGlobalIdX();
    outputs2[i] = static_cast<uint32b>(index);
  }

  // Local ID test
  const size_t wx = zivc::getGroupIdX();
  const size_t local_id = zivc::getLocalLinearId();
  const size_t nlx = zivc::getLocalSizeX();
  const size_t group_size = nlx;
  const size_t group_id = wx;
  const size_t index2 = local_id + group_size * group_id;
  if (index2 < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index2];
    zivc::atomic_inc(ptr);
  }
}

/*!
  \details No detailed description

  \param [out] outputs No description.
  \param [out] ids No description.
  \param [out] properties No description.
  \param [in] resolution No description.
  */
__kernel void workItemOffset3dKernel(zivc::GlobalPtr<uint32b> outputs,
                                     zivc::GlobalPtr<inner::GlobalId3d> ids,
                                     zivc::GlobalPtr<uint32b> properties,
                                     const uint32b resolution)
{
  // Global ID test
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    properties[0] = static_cast<uint32b>(zivc::getGlobalOffsetX());
    properties[1] = static_cast<uint32b>(zivc::getGlobalOffsetY());
    properties[2] = static_cast<uint32b>(zivc::getGlobalOffsetZ());
  }
  if (index < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index];
    zivc::atomic_inc(ptr);
    ids[2 * index + 0].x_ = static_cast<uint32b>(zivc::getGlobalIdX() -
                                                 zivc::getGlobalOffsetX());
    ids[2 * index + 0].y_ = static_cast<uint32b>(zivc::getGlobalIdY() -
                                                 zivc::getGlobalOffsetY());
    ids[2 * index + 0].z_ = static_cast<uint32b>(zivc::getGlobalIdZ() -
                                                 zivc::getGlobalOffsetZ());
    ids[2 * index + 1].x_ = static_cast<uint32b>(zivc::getGlobalIdX());
    ids[2 * index + 1].y_ = static_cast<uint32b>(zivc::getGlobalIdY());
    ids[2 * index + 1].z_ = static_cast<uint32b>(zivc::getGlobalIdZ());
  }

  // Local ID test
  const size_t wx = zivc::getGroupIdX();
  const size_t wy = zivc::getGroupIdY();
  const size_t wz = zivc::getGroupIdZ();
  const size_t local_id = zivc::getLocalLinearId();
  const size_t nlx = zivc::getLocalSizeX();
  const size_t nly = zivc::getLocalSizeY();
  const size_t nlz = zivc::getLocalSizeZ();
  const size_t group_size = nlx * nly * nlz;
  const size_t nwx = zivc::getNumGroupsX();
  const size_t nwy = zivc::getNumGroupsY();
  const size_t group_id = wx + nwx * wy + (nwx * nwy) * wz;
  const size_t index2 = local_id + group_size * group_id;
  if (index2 < resolution) {
    zivc::GlobalPtr<uint32b> ptr = &outputs[index2];
    zivc::atomic_inc(ptr);
  }
}

#endif // ZIVC_TEST_KERNEL_TEST_CL
