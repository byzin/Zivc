/*!
  \file kernel_test2.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST2_CL
#define ZIVC_TEST_KERNEL_TEST2_CL

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

/*!
  \details No detailed description

  \param [in,out] values No description.
  \param [in] resolution No description.
  */
__kernel void invocation1Kernel(zivc::GlobalPtr<uint32b> values,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  volatile uint32b v = values[index];
  for (size_t i = 0; i < 1024; ++i) {
    v = v + 10;
  }
  values[index] = v;
}

/*!
  \details No detailed description

  \param [in,out] values No description.
  \param [in] resolution No description.
  */
__kernel void invocation2Kernel(zivc::GlobalPtr<uint32b> values,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  values[index] *= 5;
}

/*!
  \details No detailed description

  \param [in,out] values No description.
  \param [in] resolution No description.
  */
__kernel void invocation3Kernel(zivc::GlobalPtr<uint32b> values,
                                const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  volatile uint32b v = values[index];
  v = v / 2;
  for (size_t i = 0; i < 1024; ++i) {
    v = v + 3;
  }
  values[index] = v;
}

/*!
  \details No detailed description

  \param [in,out] inout No description.
  \param [in] resolution No description.
  */
__kernel void reinterpTestKernel(zivc::GlobalPtr<float> inout,
                                 const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  inout[index] *= 3.0f;
}

namespace inner {

struct GlobalTestStruct
{
  void set(const int32b value) noexcept
  {
    zivc::GenericPtr<int32b> ptr = value_;
    for (size_t i = 0; i < 4; ++i)
      ptr[i] = value + static_cast<int32b>(i);
  }

  int32b sum() const noexcept
  {
    zivc::ConstGenericPtr<int32b> ptr = value_;
    int32b sum = 0;
    for (size_t i = 0; i < 4; ++i)
      sum += ptr[i];
    return sum;
  }

  int32b value_[4];
};

static_assert(sizeof(GlobalTestStruct) % 16 == 0);

} // inner

__kernel void testGlobalStructKernel(zivc::GlobalPtr<inner::GlobalTestStruct> inout)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
  //! \todo Resolve the compile error
//    {
//      const int32b sum = inout[0].sum();
//      inout[0].set(sum);
//    }
//    {
//      inner::GlobalTestStruct s = inout[1];
//      const int32b sum = s.sum();
//      s.set(sum);
//      inout[1] = s;
//    }
  }
}

#endif // ZIVC_TEST_KERNEL_TEST2_CL
