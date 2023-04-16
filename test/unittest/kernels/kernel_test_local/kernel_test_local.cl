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

struct Storage1
{
  int32b sum() const noexcept;

  void set(const uint32b index) noexcept;

  zivc::ConstGenericPtr<int32b> data() const noexcept;

  zivc::GenericPtr<int32b> data() noexcept;

  static constexpr size_t size() noexcept;


  int32b value1_;
  int32b value2_;
  int32b value3_;
  int32b value4_;
};

inline
int32b Storage1::sum() const noexcept
{
  zivc::ConstGenericPtr<int32b> ptr = data();
  int32b sum = 0;
  for (size_t i = 0; i < size(); ++i)
    sum += ptr[i];
  return sum;
}

inline
void Storage1::set(const uint32b index) noexcept
{
  zivc::GenericPtr<int32b> ptr = data();
  for (size_t i = 0; i < size(); ++i)
    ptr[i] = static_cast<int32b>(index + i);
}

inline
zivc::ConstGenericPtr<int32b> Storage1::data() const noexcept
{
  zivc::ConstGenericPtr<int32b> ptr = reinterpret_cast<const int32b*>(&value1_);
  return ptr;
}

inline
zivc::GenericPtr<int32b> Storage1::data() noexcept
{
  zivc::GenericPtr<int32b> ptr = reinterpret_cast<int32b*>(&value1_);
  return ptr;
}

inline
constexpr size_t Storage1::size() noexcept
{
  return 4;
}

static_assert(sizeof(Storage1) % 16 == 0);

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

struct Storage3
{
  int32b sum() const noexcept;

  void set(const uint32b index) noexcept;

  zivc::ConstGenericPtr<int32b> data() const noexcept;

  zivc::GenericPtr<int32b> data() noexcept;

  static constexpr size_t size() noexcept;


  int4 value_;
};

inline
int32b Storage3::sum() const noexcept
{
  zivc::ConstGenericPtr<int32b> ptr = data();
  int32b sum = 0;
  for (size_t i = 0; i < size(); ++i)
    sum += ptr[i];
  return sum;
}

inline
void Storage3::set(const uint32b index) noexcept
{
  zivc::GenericPtr<int32b> ptr = data();
  for (size_t i = 0; i < size(); ++i)
    ptr[i] = static_cast<int32b>(index + i);
}

inline
zivc::ConstGenericPtr<int32b> Storage3::data() const noexcept
{
  zivc::ConstGenericPtr<int32b> ptr = reinterpret_cast<const int32b*>(&value_);
  return ptr;
}

inline
zivc::GenericPtr<int32b> Storage3::data() noexcept
{
  zivc::GenericPtr<int32b> ptr = reinterpret_cast<int32b*>(&value_);
  return ptr;
}

inline
constexpr size_t Storage3::size() noexcept
{
  return 4;
}

static_assert(sizeof(Storage3) % 16 == 0);

inline
uint32b accumulateArray(zivc::ConstLocalPtr<uint32b> ptr, const size_t n) noexcept
{
  uint32b sum = 0;
  for (size_t i = 0; i < n; ++i)
    sum += ptr[i];
  return sum;
}

} // inner

__kernel void local1Kernel(zivc::GlobalPtr<uint32b> outputs,
                           zivc::LocalPtr<uint32b> storage,
                           const uint32b value)
{
  if ((zivc::getGlobalLinearId() == 0) && (zivc::getLocalLinearId() == 0)) {
    storage[0] = value;
    outputs[0] = storage[0];
    outputs[1] = zivc::getWorkDim();
  }
}

__kernel void local2Kernel(zivc::GlobalPtr<uint32b> outputs,
    const int32b value1,
    const float value2,
    const uint32b value3,
    zivc::LocalPtr<int32b> storage1,
    zivc::LocalPtr<float> storage2,
    zivc::LocalPtr<uint32b> storage3)
{
  if ((zivc::getGlobalLinearId() == 0) && (zivc::getLocalLinearId() == 0)) {
    storage1[0] = value1;
    storage2[0] = value2;
    storage3[0] = value3;
    outputs[0] = static_cast<uint32b>(storage1[0]) +
                 static_cast<uint32b>(storage2[0]) +
                 storage3[0];
    outputs[1] = zivc::getWorkDim();
  }
}

__kernel void localSumKernel(zivc::GlobalPtr<uint32b> outputs,
                             zivc::LocalPtr<uint32b> storage,
                             const uint32b offset,
                             const uint32b resolution)
{
  {
    const uint32b id = zivc::getLocalLinearId();
    storage[id] = id + 1;
    zivc::Synchronization::barrierLocal();
  }

  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    const size_t n = zivc::getLocalSizeX();
    outputs[index] = inner::accumulateArray(storage, n) + offset;
  }
}

__kernel void localStruct1Kernel(zivc::GlobalPtr<uint32b> outputs,
                                 const uint32b resolution,
                                 zivc::LocalPtr<inner::Storage1> storage)
{
  const size_t storage_id = zivc::getLocalLinearId();
  zivc::Local<inner::Storage1>& s = storage[storage_id];

  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    s.set(index);
    outputs[index] = static_cast<uint32b>(s.sum());
  }
}

__kernel void localStruct2Kernel(zivc::GlobalPtr<uint32b> outputs,
                                 const uint32b resolution,
                                 zivc::LocalPtr<inner::Storage2> storage)
{
  const size_t storage_id = zivc::getLocalLinearId();
  zivc::Local<inner::Storage2>& s = storage[storage_id];

  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    s.set(index);
    outputs[index] = static_cast<uint32b>(s.sum());
  }
}

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
      const zivc::Local<inner::Storage2>& s = storage[storage_id];
      outputs[index] = static_cast<uint32b>(s.sum());
    }
  }
}

__kernel void localStruct3Kernel(zivc::GlobalPtr<uint32b> outputs,
                                 const uint32b resolution,
                                 zivc::LocalPtr<inner::Storage3> storage)
{
  const size_t storage_id = zivc::getLocalLinearId();
  zivc::Local<inner::Storage3>& s = storage[storage_id];

  const size_t index = zivc::getGlobalLinearId();
  if (index < resolution) {
    s.set(index);
    outputs[index] = static_cast<uint32b>(s.sum());
  }
}

#endif // ZIVC_TEST_KERNEL_TEST_LOCAL_CL
