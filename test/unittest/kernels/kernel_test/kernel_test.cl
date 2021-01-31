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

// Forward declaration
__kernel void simpleKernel([[maybe_unused]] zivc::ConstGlobalPtr<int32b> inputs)
{
  // Do nothing here
}

#endif // ZIVC_TEST_KERNEL_TEST_CL
