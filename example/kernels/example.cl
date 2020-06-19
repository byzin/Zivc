/*!
  \file example.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
    zivc::LocalPtr<uint32b> cache,
    zivc::GlobalPtr<float> outputs,
    const int16b hash,
    const uint32b resolution,
    const int16b hash1,
    const int64b hash12,
    const int4 hash2
    );

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [in] cache No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void testKernel(zivc::ConstGlobalPtr<int32b> inputs,
    zivc::LocalPtr<uint32b> cache,
    zivc::GlobalPtr<float> outputs,
    const int16b hash,
    const uint32b resolution,
    const int16b hash1,
    const int64b hash12,
    const int4 hash2
    )
{
  const uint32b index = zivc::getGlobalIdX();
  static_cast<void>(cache);
  if (index < resolution) {
    outputs[index] = zivc::cast<float>(inputs[index]);
  }
}

#endif // ZIVC_EXAMPLE_CL
