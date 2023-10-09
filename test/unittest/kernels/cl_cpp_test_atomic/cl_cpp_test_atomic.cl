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

#ifndef ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_CL
#define ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_CL

// Zivc
#include "zivc/cl/atomic.hpp"
#include "zivc/cl/bit.hpp"
#include "zivc/cl/limits.hpp"
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
  zivc::uint32b pad_[2];
};

static_assert(sizeof(Storage1) == 16);

} /* namespace inner */

__kernel void atomicLoadStoreTestKernel(zivc::GlobalPtr<int32b> index1,
                                        zivc::ConstGlobalPtr<uint32b> index2,
                                        zivc::GlobalPtr<int32b> out1,
                                        zivc::GlobalPtr<uint32b> out2,
                                        const uint32b resolution)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  {
    const size_t i = 2 * index;
    const int32b v = zivc::atomic_load(&index1[i]);
    zivc::atomic_store(&out1[i], v);
  }
  {
    const size_t i = 2 * index + 1;
    zivc::GlobalPtr<int32b> iptr = index1 + i;
    zivc::GlobalPtr<int32b> optr = out1 + i;
    const int32b v = zivc::atomic_load(iptr);
    zivc::atomic_store(optr, v);
  }
  {
    const size_t i = 2 * index;
    const uint32b v = zivc::atomic_load(&index2[i]);
    zivc::atomic_store(&out2[i], v);
  }
  {
    const size_t i = 2 * index + 1;
    zivc::ConstGlobalPtr<uint32b> iptr = index2 + i;
    zivc::GlobalPtr<uint32b> optr = out2 + i;
    const uint32b v = zivc::atomic_load(iptr);
    zivc::atomic_store(optr, v);
  }
}

__kernel void atomicExchangeTestKernel(zivc::GlobalPtr<int32b> index1,
                                       zivc::GlobalPtr<uint32b> index2,
                                       zivc::GlobalPtr<int32b> out1,
                                       zivc::GlobalPtr<uint32b> out2,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  for (size_t i = 0; i < loop; ++i) {
    const size_t c = index + i * resolution;
    {
      const int32b id = zivc::atomic_exchange(index1, static_cast<int32b>(c));
      out1[static_cast<size_t>(id)] = 1;
    }
    {
      const uint32b id = zivc::atomic_exchange(&index2[0], static_cast<uint32b>(c));
      out2[static_cast<size_t>(id)] = 1;
    }
  }
}

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
    zivc::atomic_store(&storage[0].u_, 0u);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i) {
    zivc::atomic_fetch_inc(&storage[0].i_);
    zivc::atomic_fetch_inc(&storage[0].u_);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    {
      const int32b v = zivc::atomic_load(&storage[0].i_);
      zivc::atomic_fetch_add(out1, v);
    }
    {
      const uint32b v = zivc::atomic_load(&storage[0].u_);
      zivc::atomic_fetch_add(&out2[0], v);
    }
  }
}

__kernel void atomicLocalTest1Kernel(zivc::GlobalPtr<int32b> out,
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
    zivc::atomic_fetch_add(storage, 1);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    const size_t i = index / zivc::getLocalSizeX();
    const int32b v = zivc::atomic_load(storage);
    zivc::atomic_store(out + i, v);
  }
}

__kernel void atomicLocalTest2Kernel(zivc::GlobalPtr<uint32b> out,
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
    zivc::atomic_fetch_add(&storage[0], 1u);
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    const size_t i = index / zivc::getLocalSizeX();
    const uint32b v = zivc::atomic_load(&storage[0]);
    zivc::atomic_store(&out[i], v);
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
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  constexpr size_t s = 8 * sizeof(uint32b);
  const uint32b v = 0b01u << (index % s);
  const uint32b bit = ~v;
  for (size_t i = 0; i < loop; ++i) {
    const size_t offset = (index + i * resolution) / s;
    zivc::GlobalPtr<uint32b> o = out + offset;
    zivc::atomic_fetch_and(o, bit);
  }
}

__kernel void atomicFetchOrTestKernel(zivc::GlobalPtr<uint32b> out,
                                      const uint32b resolution,
                                      const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  constexpr size_t s = 8 * sizeof(uint32b);
  const uint32b bit = 0b01u << (index % s);
  for (size_t i = 0; i < loop; ++i) {
    const size_t offset = (index + i * resolution) / s;
    zivc::GlobalPtr<uint32b> o = out + offset;
    zivc::atomic_fetch_or(o, bit);
  }
}

__kernel void atomicFetchXorTestKernel(zivc::GlobalPtr<uint32b> out,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  constexpr size_t s = 8 * sizeof(uint32b);
  const uint32b bit = 0b01u << (index % s);
  for (size_t i = 0; i < loop; ++i) {
    const size_t offset = (index + i * resolution) / s;
    zivc::GlobalPtr<uint32b> o = out + offset;
    zivc::atomic_fetch_xor(o, bit);
  }
}

__kernel void atomicFetchMinTestKernel(zivc::ConstGlobalPtr<int32b> in1,
                                       zivc::ConstGlobalPtr<uint32b> in2,
                                       zivc::GlobalPtr<int32b> out1,
                                       zivc::GlobalPtr<uint32b> out2,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  for (size_t i = 0; i < loop; ++i) {
    const size_t c = loop * index + i;
    zivc::atomic_fetch_min(out1, in1[c]);
    zivc::atomic_fetch_min(&out2[0], in2[c]);
  }
}

__kernel void atomicFetchMinLocalTestKernel(zivc::ConstGlobalPtr<int32b> in1,
                                            zivc::ConstGlobalPtr<uint32b> in2,
                                            zivc::GlobalPtr<int32b> out1,
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
    zivc::atomic_store(&storage[0].i_, zivc::NumericLimits<int32b>::max());
    zivc::atomic_store(&storage[0].u_, zivc::NumericLimits<uint32b>::max());
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i) {
    const size_t c = loop * index + i;
    zivc::atomic_fetch_min(&storage[0].i_, in1[c]);
    zivc::atomic_fetch_min(&storage[0].u_, in2[c]);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    {
      const int32b v = zivc::atomic_load(&storage[0].i_);
      zivc::atomic_fetch_min(out1, v);
    }
    {
      const uint32b v = zivc::atomic_load(&storage[0].u_);
      zivc::atomic_fetch_min(&out2[0], v);
    }
  }
}

__kernel void atomicFetchMaxTestKernel(zivc::ConstGlobalPtr<int32b> in1,
                                       zivc::ConstGlobalPtr<uint32b> in2,
                                       zivc::GlobalPtr<int32b> out1,
                                       zivc::GlobalPtr<uint32b> out2,
                                       const uint32b resolution,
                                       const uint32b loop)
{
  const size_t index = zivc::getGlobalLinearId();
  if (resolution <= index)
    return;

  for (size_t i = 0; i < loop; ++i) {
    const size_t c = loop * index + i;
    zivc::atomic_fetch_max(out1, in1[c]);
    zivc::atomic_fetch_max(&out2[0], in2[c]);
  }
}

__kernel void atomicFetchMaxLocalTestKernel(zivc::ConstGlobalPtr<int32b> in1,
                                            zivc::ConstGlobalPtr<uint32b> in2,
                                            zivc::GlobalPtr<int32b> out1,
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
    zivc::atomic_store(&storage[0].i_, zivc::NumericLimits<int32b>::min());
    zivc::atomic_store(&storage[0].u_, zivc::NumericLimits<uint32b>::min());
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  for (size_t i = 0; i < loop; ++i) {
    const size_t c = loop * index + i;
    zivc::atomic_fetch_max(&storage[0].i_, in1[c]);
    zivc::atomic_fetch_max(&storage[0].u_, in2[c]);
  }
  zivc::barrier(CLK_LOCAL_MEM_FENCE);

  if (id == 0) {
    {
      const int32b v = zivc::atomic_load(&storage[0].i_);
      zivc::atomic_fetch_max(out1, v);
    }
    {
      const uint32b v = zivc::atomic_load(&storage[0].u_);
      zivc::atomic_fetch_max(&out2[0], v);
    }
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST_ATOMIC_CL */
