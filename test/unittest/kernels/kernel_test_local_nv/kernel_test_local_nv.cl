/*!
  \file kernel_test_local.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST_LOCAL_CL
#define ZIVC_TEST_KERNEL_TEST_LOCAL_CL

// Zivc
#include "zivc/cl/synchronization.hpp"
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

namespace inner {

struct Storage2
{
  int32b sum() const noexcept;

  void set(const uint32b index) noexcept;

  static constexpr size_t size() noexcept;


  static constexpr size_t kSize = 4;


  int32b value_[kSize];
};

inline
int32b Storage2::sum() const noexcept
{
  int32b sum = 0;
  for (size_t i = 0; i < size(); ++i)
    sum += value_[i];
  return sum;
}

inline
void Storage2::set(const uint32b index) noexcept
{
  for (size_t i = 0; i < size(); ++i)
    value_[i] = static_cast<int32b>(index + i);
}

inline
constexpr size_t Storage2::size() noexcept
{
  return kSize;
}

static_assert(sizeof(Storage2) % 16 == 0);

} // inner

__kernel void localStruct2CopyKernel(zivc::ConstGlobalPtr<inner::Storage2> inputs,
                                     zivc::GlobalPtr<uint32b> outputs,
                                     const uint32b resolution,
                                     zivc::LocalPtr<inner::Storage2> storage)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    {
      const size_t storage_id = zivc::getLocalLinearId();
      storage[storage_id] = inputs[index];
      zivc::Synchronization::barrierLocal();
    }
    {
      const size_t storage_id = (zivc::getLocalLinearId() + 1) % zivc::getLocalSizeX();
      zivc::ConstLocalPtr<inner::Storage2> s = &storage[storage_id];
      outputs[index] = static_cast<uint32b>(s->sum());
    }
  }
}

#endif // ZIVC_TEST_KERNEL_TEST_LOCAL_CL
