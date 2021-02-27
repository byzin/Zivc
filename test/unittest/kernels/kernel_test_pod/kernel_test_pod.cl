/*!
  \file kernel_test_pod.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST_POD_CL
#define ZIVC_TEST_KERNEL_TEST_POD_CL

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

static_assert(sizeof(PodAlignmentTest) == 32);
static_assert(alignof(PodAlignmentTest) == 4);

} // namespace inner

/*!
  \details No detailed description

  \param [out] output No description.
  \param [in] i16 No description.
  \param [in] i8_1 No description.
  \param [in] i8_2 No description.
  \param [in] u32 No description.
  \param [in] u8_1 No description.
  \param [in] u8_2 No description.
  \param [in] u8_3 No description.
  \param [in] u8_4 No description.
  */
__kernel void podSizeAlignmentKernel(zivc::GlobalPtr<int32b> output,
                                     const int16b i16,
                                     const int8b i8_1,
                                     const int8b i8_2,
                                     const uint32b u32,
                                     const uint8b u8_1,
                                     const uint8b u8_2,
                                     const uint8b u8_3,
                                     const uint8b u8_4,
                                     const uint32b u32_2,
                                     const int16b i16_2,
                                     const int8b i8_3,
                                     const int8b i8_4,
                                     const int8b i8_5,
                                     const int8b i8_6,
                                     const int16b i16_3)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    size_t i = 0;
    output[i++] = static_cast<int32b>(i16);
    output[i++] = static_cast<int32b>(i8_1);
    output[i++] = static_cast<int32b>(i8_2);
    output[i++] = static_cast<int32b>(u32);
    output[i++] = static_cast<int32b>(u8_1);
    output[i++] = static_cast<int32b>(u8_2);
    output[i++] = static_cast<int32b>(u8_3);
    output[i++] = static_cast<int32b>(u8_4);
    output[i++] = static_cast<int32b>(u32_2);
    output[i++] = static_cast<int32b>(i16_2);
    output[i++] = static_cast<int32b>(i8_3);
    output[i++] = static_cast<int32b>(i8_4);
    output[i++] = static_cast<int32b>(i8_5);
    output[i++] = static_cast<int32b>(i8_6);
    output[i++] = static_cast<int32b>(i16_3);
  }
}

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
__kernel void podSizeAlignment2Kernel(zivc::GlobalPtr<uint8b> output1,
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
  \brief No brief description

  No detailed description.
  */
struct PodVectorTest
{
  uchar2 u8v2_1_;
  uchar2 u8v2_2_;
  uchar4 u8v4_1_;
  uchar4 u8v4_2_;
  short2 i16v2_1_;
  short2 i16v2_2_;
  short2 padding1_;
  short4 i16v4_1_;
  short4 i16v4_2_;
  int2 i32v2_1_;
  int2 i32v2_2_;
  int2 padding2_;
  int4 i32v4_1_;
  int4 i32v4_2_;
  float2 f32v2_1_;
  float2 f32v2_2_;
  float4 f32v4_1_;
  float4 f32v4_2_;
};

//! Check if two values are equal
bool operator==(const PodVectorTest& lhs, const PodVectorTest& rhs) noexcept;

//! Check if two values aren't equal
bool operator!=(const PodVectorTest& lhs, const PodVectorTest& rhs) noexcept;

inline
bool operator==(const PodVectorTest& /* lhs */, const PodVectorTest& /* rhs */) noexcept
{
  const bool result = false;
  return result;
}

inline
bool operator!=(const PodVectorTest& lhs, const PodVectorTest& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

static_assert(sizeof(PodVectorTest) == 144);
static_assert(alignof(PodVectorTest) == 16);

} // namespace inner

__kernel void podVectorKernel(zivc::GlobalPtr<uchar2> output1,
                              zivc::GlobalPtr<uchar4> output2,
                              zivc::GlobalPtr<short2> output3,
                              zivc::GlobalPtr<short4> output4,
                              zivc::GlobalPtr<int2> output5,
                              zivc::GlobalPtr<int4> output6,
                              zivc::GlobalPtr<float2> output7,
                              zivc::GlobalPtr<float4> output8,
                              zivc::GlobalPtr<inner::PodVectorTest> output9,
                              const uchar2 u8v2_1,
                              const uchar2 u8v2_2,
                              const uchar4 u8v4_1,
                              const uchar4 u8v4_2,
                              const short2 i16v2_1,
                              const short2 i16v2_2,
                              const short4 i16v4_1,
                              const short4 i16v4_2,
                              const int2 i32v2_1,
                              const int2 i32v2_2,
                              const int4 i32v4_1,
                              const int4 i32v4_2,
                              const float2 f32v2_1,
                              const float2 f32v2_2,
                              const float4 f32v4_1,
                              const float4 f32v4_2,
                              const inner::PodVectorTest test)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    output1[0] = u8v2_1;
    output1[1] = u8v2_2;
    output1[2] = test.u8v2_1_;
    output1[3] = test.u8v2_2_;
    output2[0] = u8v4_1;
    output2[1] = u8v4_2;
    output2[2] = test.u8v4_1_;
    output2[3] = test.u8v4_2_;
    output3[0] = i16v2_1;
    output3[1] = i16v2_2;
    output3[2] = test.i16v2_1_;
    output3[3] = test.i16v2_2_;
    output4[0] = i16v4_1;
    output4[1] = i16v4_2;
    output4[2] = test.i16v4_1_;
    output4[3] = test.i16v4_2_;
    output5[0] = i32v2_1;
    output5[1] = i32v2_2;
    output5[2] = test.i32v2_1_;
    output5[3] = test.i32v2_2_;
    output6[0] = i32v4_1;
    output6[1] = i32v4_2;
    output6[2] = test.i32v4_1_;
    output6[3] = test.i32v4_2_;
    output7[0] = f32v2_1;
    output7[1] = f32v2_2;
    output7[2] = test.f32v2_1_;
    output7[3] = test.f32v2_2_;
    output8[0] = f32v4_1;
    output8[1] = f32v4_2;
    output8[2] = test.f32v4_1_;
    output8[3] = test.f32v4_2_;

    output9[0].u8v2_1_ = u8v2_1;
    output9[0].u8v2_2_ = u8v2_2;
    output9[0].u8v4_1_ = u8v4_1;
    output9[0].u8v4_2_ = u8v4_2;
    output9[0].i16v2_1_ = i16v2_1;
    output9[0].i16v2_2_ = i16v2_2;
    output9[0].i16v4_1_ = i16v4_1;
    output9[0].i16v4_2_ = i16v4_2;
    output9[0].i32v2_1_ = i32v2_1;
    output9[0].i32v2_2_ = i32v2_2;
    output9[0].i32v4_1_ = i32v4_1;
    output9[0].i32v4_2_ = i32v4_2;
    output9[0].f32v2_1_ = f32v2_1;
    output9[0].f32v2_2_ = f32v2_2;
    output9[0].f32v4_1_ = f32v4_1;
    output9[0].f32v4_2_ = f32v4_2;
    output9[1] = test;
  }
}

#endif // ZIVC_TEST_KERNEL_TEST_POD_CL
