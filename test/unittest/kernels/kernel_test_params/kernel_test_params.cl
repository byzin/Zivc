/*!
  \file kernel_test_params.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST_PARAMS_CL
#define ZIVC_TEST_KERNEL_TEST_PARAMS_CL

// Zivc
#include "zivc/cl/atomic.hpp"
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

struct ParamTest
{
  int32b i_;
  uint32b u_;
  float f_;
};

/*!
  \details No detailed description
  */
__kernel void largeNumOfParametersKernel(
    zivc::GlobalPtr<int8b> o1,
    zivc::GlobalPtr<int16b> o2,
    zivc::GlobalPtr<int32b> o3,
    zivc::GlobalPtr<uint8b> o4,
    zivc::GlobalPtr<uint16b> o5,
    zivc::GlobalPtr<uint32b> o6,
    zivc::GlobalPtr<float> o7,
    zivc::GlobalPtr<ParamTest> o8,
    const int32b s1,
    zivc::GlobalPtr<int8b> o9,
    zivc::GlobalPtr<int16b> o10,
    zivc::GlobalPtr<int32b> o11,
    zivc::GlobalPtr<uint8b> o12,
    zivc::GlobalPtr<uint16b> o13,
    zivc::GlobalPtr<uint32b> o14,
    zivc::GlobalPtr<float> o15,
    zivc::GlobalPtr<ParamTest> o16,
    const uint32b s2,
    zivc::GlobalPtr<int8b> o17,
    zivc::GlobalPtr<int16b> o18,
    zivc::GlobalPtr<int32b> o19,
    zivc::GlobalPtr<uint8b> o20,
    zivc::GlobalPtr<uint16b> o21,
    zivc::GlobalPtr<uint32b> o22,
    zivc::GlobalPtr<float> o23,
    zivc::GlobalPtr<ParamTest> o24,
    const float s3,
    zivc::GlobalPtr<int8b> o25,
    zivc::GlobalPtr<int16b> o26,
    zivc::GlobalPtr<int32b> o27,
    zivc::GlobalPtr<uint8b> o28,
#if !defined(Z_MAC)
    zivc::GlobalPtr<uint16b> o29,
    zivc::GlobalPtr<uint32b> o30,
    zivc::GlobalPtr<float> o31,
    zivc::GlobalPtr<ParamTest> o32,
    const int32b s4,
    zivc::GlobalPtr<int8b> o33,
    zivc::GlobalPtr<int16b> o34,
    zivc::GlobalPtr<int32b> o35,
    zivc::GlobalPtr<uint8b> o36,
    zivc::GlobalPtr<uint16b> o37,
    zivc::GlobalPtr<uint32b> o38,
    zivc::GlobalPtr<float> o39,
    zivc::GlobalPtr<ParamTest> o40,
    const uint32b s5,
    zivc::GlobalPtr<int8b> o41,
    zivc::GlobalPtr<int16b> o42,
    zivc::GlobalPtr<int32b> o43,
    zivc::GlobalPtr<uint8b> o44,
    zivc::GlobalPtr<uint16b> o45,
    zivc::GlobalPtr<uint32b> o46,
    zivc::GlobalPtr<float> o47,
    zivc::GlobalPtr<ParamTest> o48,
    const float s6,
    zivc::GlobalPtr<int8b> o49,
    zivc::GlobalPtr<int16b> o50,
    zivc::GlobalPtr<int32b> o51,
    zivc::GlobalPtr<uint8b> o52,
    zivc::GlobalPtr<uint16b> o53,
    zivc::GlobalPtr<uint32b> o54,
    zivc::GlobalPtr<float> o55,
    zivc::GlobalPtr<ParamTest> o56,
    const int32b s7,
    zivc::GlobalPtr<int8b> o57,
    zivc::GlobalPtr<int16b> o58,
    zivc::GlobalPtr<int32b> o59,
    zivc::GlobalPtr<uint8b> o60,
    zivc::GlobalPtr<uint16b> o61,
    zivc::GlobalPtr<uint32b> o62,
    zivc::GlobalPtr<float> o63,
    zivc::GlobalPtr<ParamTest> o64,
#endif // Z_MAC
    const uint32b s8)
{
  const size_t index = zivc::getGlobalLinearId();
  if (0 < index)
    return;

  o1[0] = 1;
  o2[0] = 1;
  o3[0] = 1;
  o4[0] = 1;
  o5[0] = 1;
  o6[0] = 1;
  o7[0] = 1.0f;
  o8[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s1);

  o9[0] = 1;
  o10[0] = 1;
  o11[0] = 1;
  o12[0] = 1;
  o13[0] = 1;
  o14[0] = 1;
  o15[0] = 1.0f;
  o16[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s2);

  o17[0] = 1;
  o18[0] = 1;
  o19[0] = 1;
  o20[0] = 1;
  o21[0] = 1;
  o22[0] = 1;
  o23[0] = 1.0f;
  o24[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s3);

  o25[0] = 1;
  o26[0] = 1;
  o27[0] = 1;
  o28[0] = 1;
#if !defined(Z_MAC)
  o29[0] = 1;
  o30[0] = 1;
  o31[0] = 1.0f;
  o32[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s4);

  o33[0] = 1;
  o34[0] = 1;
  o35[0] = 1;
  o36[0] = 1;
  o37[0] = 1;
  o38[0] = 1;
  o39[0] = 1.0f;
  o40[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s5);

  o41[0] = 1;
  o42[0] = 1;
  o43[0] = 1;
  o44[0] = 1;
  o45[0] = 1;
  o46[0] = 1;
  o47[0] = 1.0f;
  o48[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s6);

  o49[0] = 1;
  o50[0] = 1;
  o51[0] = 1;
  o52[0] = 1;
  o53[0] = 1;
  o54[0] = 1;
  o55[0] = 1.0f;
  o56[0] = ParamTest{1, 1, 1.0f};
  static_cast<void>(s7);

  o57[0] = 1;
  o58[0] = 1;
  o59[0] = 1;
  o60[0] = 1;
  o61[0] = 1;
  o62[0] = 1;
  o63[0] = 1.0f;
  o64[0] = ParamTest{1, 1, 1.0f};
#endif // Z_MAC
  static_cast<void>(s8);
}

#endif // ZIVC_TEST_KERNEL_TEST_PARAMS_CL
