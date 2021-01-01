/*!
  \file example.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_EXAMPLE_CL
#define ZIVC_EXAMPLE_CL

// Zivc
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"

using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

// Forward declaration
__kernel void testKernel(zivc::ConstGlobalPtr<int32b> inputs,
    zivc::GlobalPtr<int32b> outputs,
    const uint32b resolution);

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void testKernel(zivc::ConstGlobalPtr<int32b> inputs,
    zivc::GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zivc::getGlobalIdX();
  if (index < resolution) {
    outputs[index] = 2 * inputs[index];
  }
}

#endif // ZIVC_EXAMPLE_CL
