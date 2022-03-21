/*!
  \file example.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_EXAMPLE_CL
#define ZIVC_EXAMPLE_CL

// Zivc
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"

using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

// Forward declaration
__kernel void kernel1(zivc::ConstGlobalPtr<int32b> inputs1,
                      zivc::ConstGlobalPtr<int32b> inputs2,
                      zivc::GlobalPtr<int32b> outputs,
                      const uint32b resolution);
__kernel void kernel2(zivc::ConstGlobalPtr<float> inputs,
                      zivc::GlobalPtr<float> outputs,
                      const uint32b resolution);

/*!
  \details No detailed description

  \param [in] inputs1 No description.
  \param [in] inputs2 No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void kernel1(zivc::ConstGlobalPtr<int32b> inputs1,
                      zivc::ConstGlobalPtr<int32b> inputs2,
                      zivc::GlobalPtr<int32b> outputs,
                      const uint32b resolution)
{
  // Actual the number of work-items can exceed the dispatch size due to subgroup size
  const size_t gindex = zivc::getGlobalIdX();
  if (resolution <= gindex)
    return;

  outputs[gindex] = inputs1[gindex] + inputs2[gindex];
}

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void kernel2(zivc::ConstGlobalPtr<float> inputs,
                      zivc::GlobalPtr<float> outputs,
                      const uint32b resolution)
{
  // Actual the number of work-items can exceed the dispatch size due to subgroup size
  const size_t gindex = zivc::getGlobalIdX();
  if (resolution <= gindex)
    return;

  outputs[gindex] = 2.0f * inputs[gindex];
}

#endif // ZIVC_EXAMPLE_CL
