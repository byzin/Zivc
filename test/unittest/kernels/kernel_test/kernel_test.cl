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

  \param [in] inputs No description.
  */
__kernel void simpleKernel([[maybe_unused]] zivc::ConstGlobalPtr<int32b> inputs)
{
  // Do nothing here
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
  const int32b v = inputs[index];
  outputs[index] = v;
}

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void pod1Kernel(zivc::ConstGlobalPtr<int32b> inputs,
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
__kernel void pod2Kernel(const int32b i0, const uint32b u0, const float f0,
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

#endif // ZIVC_TEST_KERNEL_TEST_CL
