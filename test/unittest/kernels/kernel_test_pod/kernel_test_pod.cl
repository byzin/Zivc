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

#endif // ZIVC_TEST_KERNEL_TEST_POD_CL
