/*!
  \file opencl_cpp_test_atomic_compare_and_exchange.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_COMPARE_AND_EXCHANGE_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_COMPARE_AND_EXCHANGE_CL

// Zivc
#include "zivc/cl/atomic.hpp"
#include "zivc/cl/bit.hpp"
#include "zivc/cl/synchronization.hpp"
#include "zivc/cl/types.hpp"
#include "zivc/cl/type_traits.hpp"
#include "zivc/cl/utility.hpp"

using zivc::int8b;
using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint8b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

__kernel void atomicCompareAndExchangeTestKernel(zivc::GlobalPtr<int32b> out1,
                                                 zivc::GlobalPtr<uint32b> out2,
                                                 const uint32b resolution,
                                                 const uint32b loop,
                                                 const float scale)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  // Increment the input integer
  const auto func1 = [](const int32b v) noexcept
  {
    return v + 1;
  };

  // Scale the input value as float
  const auto func2 = [](const uint32b v, const float s) noexcept
  {
    const float result = zivc::castBit<float>(v) * s;
    return zivc::castBit<uint32b>(result);
  };

  for (size_t i = 0; i < loop; ++i) {
    zivc::Atomic::perform(out1, zivc::MemoryOrder::kAcqRel, func1);
    zivc::Atomic::perform(out2, zivc::MemoryOrder::kAcqRel, func2, scale);
  }
}

__kernel void atomicCompareAndExchangeLocalTestKernel(zivc::GlobalPtr<int32b> out1,
                                                      const uint32b resolution,
                                                      const uint32b loop,
                                                      zivc::LocalPtr<int32b> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const size_t id = zivc::getLocalLinearId();
  if (id == 0)
    zivc::atomic_store(storage, 0);
  zivc::Synchronization::barrierLocal();

  // Increment the input integer
  const auto func1 = [](const int32b v, const int32b c) noexcept
  {
    return v + c;
  };

  for (size_t i = 0; i < loop; ++i)
    zivc::Atomic::perform(storage, zivc::MemoryOrder::kAcqRel, func1, 1);
  zivc::Synchronization::barrierLocal();

  if (id == 0) {
    const int32b v = zivc::atomic_load(&storage[0]);
    zivc::Atomic::perform(out1, zivc::MemoryOrder::kAcqRel, func1, v);
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_COMPARE_AND_EXCHANGE_CL */
