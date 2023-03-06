/*!
  \file opencl_cpp_test_atomic.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC
#define ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC

// Zivc
#include "zivc/cl/atomic.hpp"
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

namespace inner {

struct Storage1
{
  zivc::int32b i_;
  zivc::uint32b u_;
  zivc::int32b pad_[2];
};

static_assert(sizeof(Storage1) == 16);

} /* namespace inner */

__kernel void atomicFetchAddTestKernel(zivc::GlobalPtr<int32b> out1,
                                       zivc::GlobalPtr<uint32b> out2,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  for (size_t i = 0; i < loop; ++i) {
    zivc::atomic_fetch_inc(out1);
    zivc::atomic_fetch_inc(&out2[0]);
  }
}

__kernel void atomicFetchAddLocalTestKernel(zivc::GlobalPtr<int32b> out1,
                                            zivc::GlobalPtr<uint32b> out2,
                                            const uint32b resolution,
                                            const uint32b loop,
                                            zivc::LocalPtr<inner::Storage1> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const size_t id = zivc::getLocalLinearId();
  if (id == 0) {
    zivc::atomic_store(&storage[0].i_, 0);
    //! \todo Resolve the compile error
    //zivc::atomic_store(&storage[0].u_, 0u);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i) {
    zivc::atomic_fetch_inc(&storage[0].i_);
    //! \todo Resolve the compile error
    //zivc::atomic_fetch_inc(&storage[0].u_);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    const size_t i = index / zivc::getLocalSizeX();
    {
      const int32b v = zivc::atomic_load(&storage[0].i_);
      zivc::atomic_store(out1 + i, v);
    }
    //! \todo Resolve the compile error
    //{
    //  const uint32b v = zivc::atomic_load(&storage[0].u_);
    //  zivc::atomic_store(&out2[i], v);
    //}
  }
}

__kernel void atomicFetchAddLocalTest1Kernel(zivc::GlobalPtr<int32b> out,
                                            zivc::LocalPtr<int32b> storage,
                                            const uint32b resolution,
                                            const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const size_t id = zivc::getLocalLinearId();
  if (id == 0)
    zivc::atomic_store(storage, 0);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i)
    zivc::atomic_fetch_inc(storage);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    const size_t i = index / zivc::getLocalSizeX();
    {
      const int32b v = zivc::atomic_load(storage);
      zivc::atomic_store(out + i, v);
    }
  }
}

__kernel void atomicFetchAddLocalTest2Kernel(zivc::GlobalPtr<uint32b> out,
                                             zivc::LocalPtr<uint32b> storage,
                                             const uint32b resolution,
                                             const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  const size_t id = zivc::getLocalLinearId();
  if (id == 0)
    zivc::atomic_store(&storage[0], 0u);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i)
    zivc::atomic_fetch_inc(&storage[0]);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    const size_t i = index / zivc::getLocalSizeX();
    {
      const uint32b v = zivc::atomic_load(&storage[0]);
      zivc::atomic_store(&out[i], v);
    }
  }
}

__kernel void atomicFetchDecTestKernel(zivc::GlobalPtr<int32b> out1,
                                       zivc::GlobalPtr<uint32b> out2,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  for (size_t i = 0; i < loop; ++i) {
    zivc::atomic_fetch_dec(out1);
    zivc::atomic_fetch_dec(&out2[0]);
  }
}

__kernel void atomicFetchAndTestKernel(zivc::GlobalPtr<uint32b> out,
                                       const uint32b resolution,
                                       const uint32b loop,
                                       const uint32b offset)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  constexpr size_t s = 8 * sizeof(uint32b);
  const uint32b v = 0b01u << (index % s);
  const uint32b bit = ~v;
  for (size_t i = 0; i < loop; ++i) {
    zivc::GlobalPtr<uint32b> o = out + (i * offset) + (index / s);
    zivc::atomic_fetch_and(o, bit);
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC */
