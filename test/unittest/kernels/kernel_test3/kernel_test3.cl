/*!
  \file kernel_test3.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_KERNEL_TEST3_CL
#define ZIVC_TEST_KERNEL_TEST3_CL

// Zivc
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"
#include "zivc/cl/vector_data.hpp"

using zivc::int8b;
using zivc::int16b;
using zivc::int32b;
using zivc::int64b;
using zivc::uint8b;
using zivc::uint16b;
using zivc::uint32b;
using zivc::uint64b;

namespace inner {

struct TestData
{
  int4 value1_;
  float4 value2_;
  ushort4 value3_;
  half2 value4_;
  uchar4 value5_;
  int4 pad_[2];
};

static_assert(sizeof(TestData) % 16 == 0);

struct TestHeader
{
  size_t data1Address() const noexcept
  {
    return static_cast<size_t>(address_[0].x);
  }

  size_t data1Size() const noexcept
  {
    return static_cast<size_t>(size_[0].x);
  }

  size_t data2Address() const noexcept
  {
    return static_cast<size_t>(address_[0].y);
  }

  size_t data2Size() const noexcept
  {
    return static_cast<size_t>(size_[0].y);
  }

  size_t data3Address() const noexcept
  {
    return static_cast<size_t>(address_[0].z);
  }

  size_t data3Size() const noexcept
  {
    return static_cast<size_t>(size_[0].z);
  }

  size_t data4Address() const noexcept
  {
    return static_cast<size_t>(address_[0].w);
  }

  size_t data4Size() const noexcept
  {
    return static_cast<size_t>(size_[0].w);
  }

  size_t data5Address() const noexcept
  {
    return static_cast<size_t>(address_[1].x);
  }

  size_t data5Size() const noexcept
  {
    return static_cast<size_t>(size_[1].x);
  }

  size_t data6Address() const noexcept
  {
    return static_cast<size_t>(address_[1].y);
  }

  size_t data6Size() const noexcept
  {
    return static_cast<size_t>(size_[1].y);
  }

  size_t data7Address() const noexcept
  {
    return static_cast<size_t>(address_[1].z);
  }

  size_t data7Size() const noexcept
  {
    return static_cast<size_t>(size_[1].z);
  }

  size_t data8Address() const noexcept
  {
    return static_cast<size_t>(address_[1].w);
  }

  size_t data8Size() const noexcept
  {
    return static_cast<size_t>(size_[1].w);
  }

  static zivc::GlobalPtr<uchar4> getData1(const TestHeader& header,
                                          zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data1Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<uchar4>>(h);
    return p;
  }

  static zivc::GlobalPtr<ushort2> getData2(const TestHeader& header,
                                           zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data2Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<ushort2>>(h);
    return p;
  }

  static zivc::GlobalPtr<short4> getData3(const TestHeader& header,
                                          zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data3Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<short4>>(h);
    return p;
  }

  static zivc::GlobalPtr<half2> getData4(const TestHeader& header,
                                         zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data4Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<half2>>(h);
    return p;
  }

  static zivc::GlobalPtr<float> getData5(const TestHeader& header,
                                         zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data5Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<float>>(h);
    return p;
  }

  static zivc::GlobalPtr<float2> getData6(const TestHeader& header,
                                          zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data6Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<float2>>(h);
    return p;
  }

  static zivc::GlobalPtr<float4> getData7(const TestHeader& header,
                                          zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data7Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<float4>>(h);
    return p;
  }

  static zivc::GlobalPtr<TestData> getData8(const TestHeader& header,
                                            zivc::GlobalPtr<int4> ptr) noexcept
  {
    const size_t address = header.data8Address();
    zivc::GlobalPtr<int4> h = &ptr[address];
    auto p = zivc::castPointer<zivc::GlobalPtr<TestData>>(h);
    return p;
  }

  static TestHeader getHeader(zivc::ConstGlobalPtr<int4> ptr) noexcept
  {
    auto p = zivc::castPointer<zivc::ConstGlobalPtr<TestHeader>>(ptr);
    return p[0];
  }

  void setData1Address(const size_t address) noexcept
  {
    address_[0].x = static_cast<uint>(address);
  }

  void setData1Size(const size_t size) noexcept
  {
    size_[0].x = static_cast<uint>(size);
  }

  void setData2Address(const size_t address) noexcept
  {
    address_[0].y = static_cast<uint>(address);
  }

  void setData2Size(const size_t size) noexcept
  {
    size_[0].y = static_cast<uint>(size);
  }

  void setData3Address(const size_t address) noexcept
  {
    address_[0].z = static_cast<uint>(address);
  }

  void setData3Size(const size_t size) noexcept
  {
    size_[0].z = static_cast<uint>(size);
  }

  void setData4Address(const size_t address) noexcept
  {
    address_[0].w = static_cast<uint>(address);
  }

  void setData4Size(const size_t size) noexcept
  {
    size_[0].w = static_cast<uint>(size);
  }

  void setData5Address(const size_t address) noexcept
  {
    address_[1].x = static_cast<uint>(address);
  }

  void setData5Size(const size_t size) noexcept
  {
    size_[1].x = static_cast<uint>(size);
  }

  void setData6Address(const size_t address) noexcept
  {
    address_[1].y = static_cast<uint>(address);
  }

  void setData6Size(const size_t size) noexcept
  {
    size_[1].y = static_cast<uint>(size);
  }

  void setData7Address(const size_t address) noexcept
  {
    address_[1].z = static_cast<uint>(address);
  }

  void setData7Size(const size_t size) noexcept
  {
    size_[1].z = static_cast<uint>(size);
  }

  void setData8Address(const size_t address) noexcept
  {
    address_[1].w = static_cast<uint>(address);
  }

  void setData8Size(const size_t size) noexcept
  {
    size_[1].w = static_cast<uint>(size);
  }


  uint4 address_[2];
  uint4 size_[2];
};

static_assert(sizeof(TestHeader) % 16 == 0);

} // inner

__kernel void reinterpArrayTestKernel(zivc::GlobalPtr<int4> inout)
{
  const size_t index = zivc::getGlobalLinearId();
  if (index == 0) {
    const inner::TestHeader header = inner::TestHeader::getHeader(inout);
    {
      zivc::GlobalPtr<uchar4> p1 = inner::TestHeader::getData1(header, inout);
      for (size_t i = 0; i < header.data1Size(); ++i)
        p1[i] *= static_cast<uint8b>(2);
    }
    {
      zivc::GlobalPtr<ushort2> p2 = inner::TestHeader::getData2(header, inout);
      for (size_t i = 0; i < header.data2Size(); ++i)
        p2[i] *= static_cast<uint16b>(2);
    }
    {
      zivc::GlobalPtr<short4> p3 = inner::TestHeader::getData3(header, inout);
      for (size_t i = 0; i < header.data3Size(); ++i)
        p3[i] *= static_cast<int16b>(2);
    }
    {
      //! \todo Remove the comment out
//      zivc::GlobalPtr<half2> p4 = inner::TestHeader::getData4(header, inout);
//      for (size_t i = 0; i < header.data4Size(); ++i) {
//        float2 v = zivc::vload_half2(i, p4);
//        v *= 2.0f;
//        zivc::vstore_half2(v, i, p4);
//      }
    }
    {
      zivc::GlobalPtr<float> p5 = inner::TestHeader::getData5(header, inout);
      for (size_t i = 0; i < header.data5Size(); ++i)
        p5[i] *= 2.0f;
    }
    {
      zivc::GlobalPtr<float2> p6 = inner::TestHeader::getData6(header, inout);
      for (size_t i = 0; i < header.data6Size(); ++i)
        p6[i] *= 2.0f;
    }
    {
      zivc::GlobalPtr<float4> p7 = inner::TestHeader::getData7(header, inout);
      for (size_t i = 0; i < header.data7Size(); ++i)
        p7[i] *= 2.0f;
    }
    {
      //! \todo Remove the comment out
//      zivc::GlobalPtr<inner::TestData> p8 = inner::TestHeader::getData8(header, inout);
//      for (size_t i = 0; i < header.data8Size(); ++i) {
//        p8[i].value1_ *= 2;
//        p8[i].value2_ *= 2.0f;
//        p8[i].value3_ *= static_cast<uint16b>(2);
//        {
//          float2 v = zivc::vload_half2(0, &p8[i].value4_);
//          v *= 2.0f;
//          zivc::vstore_half2(v, 0, &p8[i].value4_);
//        }
//        p8[i].value5_ *= static_cast<uint8b>(2);
//      }
    }
  }
}

#endif // ZIVC_TEST_KERNEL_TEST3_CL
