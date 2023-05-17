/*!
  \file kernel_test2.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <algorithm>
#include <cstddef>
#include <limits>
#include <memory>
#include <numbers>
#include <numeric>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test3.hpp"

TEST(KernelTest, KernelReinterpArrayTest)
{
  using zivc::cl_uchar4;
  using zivc::cl_ushort2;
  using zivc::cl_short4;
  using zivc::cl_ushort4;
  using zivc::cl_half;
  using zivc::cl_half2;
  using zivc::cl_float2;
  using zivc::cl_float4;
  using zivc::cl_int4;
  using zivc::cl_uint4;
  using zivc::uint8b;
  using zivc::int16b;
  using zivc::uint16b;
  using zivc::int32b;
  using zivc::uint32b;
  using zivc::uint64b;

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using Header = zivc::cl::inner::TestHeader;
  using Data = zivc::cl::inner::TestData;
  constexpr std::size_t data1_size = (sizeof(cl_int4) * 10) / sizeof(cl_uchar4);
  constexpr std::size_t data2_size = (sizeof(cl_int4) * 10) / sizeof(cl_ushort2);
  constexpr std::size_t data3_size = (sizeof(cl_int4) * 10) / sizeof(cl_short4);
  constexpr std::size_t data4_size = (sizeof(cl_int4) * 10) / sizeof(cl_half);
  constexpr std::size_t data5_size = (sizeof(cl_int4) * 10) / sizeof(float);
  constexpr std::size_t data6_size = (sizeof(cl_int4) * 10) / sizeof(cl_float2);
  constexpr std::size_t data7_size = (sizeof(cl_int4) * 10) / sizeof(cl_float4);
  constexpr std::size_t data8_size = (sizeof(cl_int4) * 10) / sizeof(Data);

  // Allocate buffers
  constexpr std::size_t n = 80 + sizeof(Header) / sizeof(cl_int4);
  const zivc::SharedBuffer buff_h = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_h->setSize(n);
  const zivc::SharedBuffer buff_d = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n);

  // Initialize buffer
  {
    zivc::MappedMemory mem = buff_h->mapMemory();
    auto* header = reinterpret_cast<Header*>(mem.data());
    {
      std::size_t address = sizeof(Header) / sizeof(cl_int4);
      header->setData1Size(data1_size);
      header->setData1Address(address);
      address += (data1_size * sizeof(cl_uchar4)) / sizeof(cl_int4);
      header->setData2Size(data2_size);
      header->setData2Address(address);
      address += (data2_size * sizeof(cl_ushort2)) / sizeof(cl_int4);
      header->setData3Size(data3_size);
      header->setData3Address(address);
      address += (data3_size * sizeof(cl_short4)) / sizeof(cl_int4);
      header->setData4Size(data4_size);
      header->setData4Address(address);
      address += (data4_size * sizeof(cl_half)) / sizeof(cl_int4);
      header->setData5Size(data5_size);
      header->setData5Address(address);
      address += (data5_size * sizeof(float)) / sizeof(cl_int4);
      header->setData6Size(data6_size);
      header->setData6Address(address);
      address += (data6_size * sizeof(cl_float2)) / sizeof(cl_int4);
      header->setData7Size(data7_size);
      header->setData7Address(address);
      address += (data7_size * sizeof(cl_float4)) / sizeof(cl_int4);
      header->setData8Size(data8_size);
      header->setData8Address(address);
    }
    // char2
    {
      auto ptr = Header::getData1(*header, mem.data());
      for (std::size_t i = 0; i < data1_size; ++i) {
        const cl_uchar4 value{static_cast<uint8b>(i)};
        ptr[i] = value;
      }
    }
    // ushort2
    {
      auto ptr = Header::getData2(*header, mem.data());
      for (std::size_t i = 0; i < data2_size; ++i) {
        const cl_ushort2 value{static_cast<uint16b>(i)};
        ptr[i] = value;
      }
    }
    // ushort3
    {
      auto ptr = Header::getData3(*header, mem.data());
      for (std::size_t i = 0; i < data3_size; ++i) {
        const cl_short4 value{static_cast<int16b>(i)};
        ptr[i] = value;
      }
    }
    // half
    {
      auto ptr = Header::getData4(*header, mem.data());
      for (std::size_t i = 0; i < data4_size; ++i) {
        const auto v = static_cast<cl_half>(static_cast<float>(i));
        const cl_half value{v};
        ptr[i] = value;
      }
    }
    // float
    {
      auto ptr = Header::getData5(*header, mem.data());
      for (std::size_t i = 0; i < data5_size; ++i) {
        const auto value = static_cast<float>(i);
        ptr[i] = value;
      }
    }
    // float2
    {
      auto ptr = Header::getData6(*header, mem.data());
      for (std::size_t i = 0; i < data6_size; ++i) {
        const cl_float2 value{static_cast<float>(i)};
        ptr[i] = value;
      }
    }
    // float4
    {
      auto ptr = Header::getData7(*header, mem.data());
      for (std::size_t i = 0; i < data7_size; ++i) {
        const cl_float4 value{static_cast<float>(i)};
        ptr[i] = value;
      }
    }
    // TestData
    {
      auto ptr = Header::getData8(*header, mem.data());
      for (std::size_t i = 0; i < data8_size; ++i) {
        Data value{};
        value.value1_ = cl_int4{static_cast<int32b>(i)};
        value.value2_ = cl_float4{static_cast<float>(i)};
        value.value3_ = cl_ushort4{static_cast<uint16b>(i)};
        value.value4_[0] = cl_half{static_cast<cl_half>(static_cast<float>(2 * i + 0))};
        value.value4_[1] = cl_half{static_cast<cl_half>(static_cast<float>(2 * i + 1))};
        value.value5_ = cl_uchar4{static_cast<uint8b>(i)};
        ptr[i] = value;
      }
    }
  }
  {
    zivc::BufferLaunchOptions options = buff_h->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_h, buff_d.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test3, reinterpArrayTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(true);
    launch_options.setLabel("ReinterpArrayTestKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, launch_options);
    device->waitForCompletion(result.fence());
  }

  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Check the result
  {
    zivc::MappedMemory mem = buff_h->mapMemory();
    const auto* header = reinterpret_cast<const Header*>(mem.data());
    // Header check
    {
      std::size_t address = sizeof(Header) / sizeof(cl_int4);
      ASSERT_EQ(data1_size, header->data1Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data1Address()) << "Header was corrupted.";
      address += (data1_size * sizeof(cl_uchar4)) / sizeof(cl_int4);
      ASSERT_EQ(data2_size, header->data2Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data2Address()) << "Header was corrupted.";
      address += (data2_size * sizeof(cl_ushort2)) / sizeof(cl_int4);
      ASSERT_EQ(data3_size, header->data3Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data3Address()) << "Header was corrupted.";
      address += (data3_size * sizeof(cl_short4)) / sizeof(cl_int4);
      ASSERT_EQ(data4_size, header->data4Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data4Address()) << "Header was corrupted.";
      address += (data4_size * sizeof(cl_half)) / sizeof(cl_int4);
      ASSERT_EQ(data5_size, header->data5Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data5Address()) << "Header was corrupted.";
      address += (data5_size * sizeof(float)) / sizeof(cl_int4);
      ASSERT_EQ(data6_size, header->data6Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data6Address()) << "Header was corrupted.";
      address += (data6_size * sizeof(cl_float2)) / sizeof(cl_int4);
      ASSERT_EQ(data7_size, header->data7Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data7Address()) << "Header was corrupted.";
      address += (data7_size * sizeof(cl_float4)) / sizeof(cl_int4);
      ASSERT_EQ(data8_size, header->data8Size()) << "Header was corrupted.";
      ASSERT_EQ(address, header->data8Address()) << "Header was corrupted.";
    }
    // uchar4
    {
      const auto ptr = Header::getData1(*header, mem.data());
      for (std::size_t i = 0; i < data1_size; ++i) {
        const cl_uchar4 value{static_cast<uint8b>(2 * i)};
        ASSERT_EQ(value.x, ptr[i].x) << "Data1 was corrupted.";
        ASSERT_EQ(value.y, ptr[i].y) << "Data1 was corrupted.";
        ASSERT_EQ(value.z, ptr[i].z) << "Data1 was corrupted.";
        ASSERT_EQ(value.w, ptr[i].w) << "Data1 was corrupted.";
      }
    }
    // ushort2
    {
      const auto ptr = Header::getData2(*header, mem.data());
      for (std::size_t i = 0; i < data2_size; ++i) {
        const cl_ushort2 value{static_cast<uint16b>(2 * i)};
        ASSERT_EQ(value.x, ptr[i].x) << "Data2 was corrupted.";
        ASSERT_EQ(value.y, ptr[i].y) << "Data2 was corrupted.";
      }
    }
    // short4
    {
      const auto ptr = Header::getData3(*header, mem.data());
      for (std::size_t i = 0; i < data3_size; ++i) {
        const cl_short4 value{static_cast<int16b>(2 * i)};
        ASSERT_EQ(value.x, ptr[i].x) << "Data3 was corrupted.";
        ASSERT_EQ(value.y, ptr[i].y) << "Data3 was corrupted.";
        ASSERT_EQ(value.z, ptr[i].z) << "Data3 was corrupted.";
        ASSERT_EQ(value.w, ptr[i].w) << "Data3 was corrupted.";
      }
    }
    // half2
    {
      const auto ptr = Header::getData4(*header, mem.data());
      for (std::size_t i = 0; i < data4_size; ++i) {
        const cl_half value{static_cast<cl_half>(static_cast<float>(2 * i))};
        ASSERT_FLOAT_EQ(static_cast<float>(value), static_cast<float>(ptr[i]))
            << "Data4 was corrupted.";
      }
    }
    // float
    {
      const auto ptr = Header::getData5(*header, mem.data());
      for (std::size_t i = 0; i < data5_size; ++i) {
        const auto value = static_cast<float>(2 * i);
        ASSERT_FLOAT_EQ(value, ptr[i]) << "Data5 was corrupted.";
      }
    }
    // float2
    {
      const auto ptr = Header::getData6(*header, mem.data());
      for (std::size_t i = 0; i < data6_size; ++i) {
        const cl_float2 value{static_cast<float>(2 * i)};
        ASSERT_FLOAT_EQ(value.x, ptr[i].x) << "Data6 was corrupted.";
        ASSERT_FLOAT_EQ(value.y, ptr[i].y) << "Data6 was corrupted.";
      }
    }
    // float4
    {
      const auto ptr = Header::getData7(*header, mem.data());
      for (std::size_t i = 0; i < data7_size; ++i) {
        const cl_float4 value{static_cast<float>(2 * i)};
        ASSERT_FLOAT_EQ(value.x, ptr[i].x) << "Data6 was corrupted.";
        ASSERT_FLOAT_EQ(value.y, ptr[i].y) << "Data6 was corrupted.";
        ASSERT_FLOAT_EQ(value.z, ptr[i].z) << "Data6 was corrupted.";
        ASSERT_FLOAT_EQ(value.w, ptr[i].w) << "Data6 was corrupted.";
      }
    }
    // TestData3
    {
      const auto ptr = Header::getData8(*header, mem.data());
      for (std::size_t i = 0; i < data8_size; ++i) {
        Data value{};
        value.value1_ = cl_int4{static_cast<int32b>(2 * i)};
        value.value2_ = cl_float4{static_cast<float>(2 * i)};
        value.value3_ = cl_ushort4{static_cast<uint16b>(2 * i)};
        value.value4_[0] = cl_half{static_cast<cl_half>(static_cast<float>(2 * (2 * i + 0)))};
        value.value4_[1] = cl_half{static_cast<cl_half>(static_cast<float>(2 * (2 * i + 1)))};
        value.value5_ = cl_uchar4{static_cast<uint8b>(2 * i)};

        ASSERT_EQ(value.value1_.x, ptr[i].value1_.x) << "Data8 was corrupted.";
        ASSERT_EQ(value.value1_.y, ptr[i].value1_.y) << "Data8 was corrupted.";
        ASSERT_EQ(value.value1_.z, ptr[i].value1_.z) << "Data8 was corrupted.";
        ASSERT_EQ(value.value1_.w, ptr[i].value1_.w) << "Data8 was corrupted.";

        ASSERT_FLOAT_EQ(value.value2_.x, ptr[i].value2_.x) << "Data8 was corrupted.";
        ASSERT_FLOAT_EQ(value.value2_.y, ptr[i].value2_.y) << "Data8 was corrupted.";
        ASSERT_FLOAT_EQ(value.value2_.z, ptr[i].value2_.z) << "Data8 was corrupted.";
        ASSERT_FLOAT_EQ(value.value2_.w, ptr[i].value2_.w) << "Data8 was corrupted.";

        ASSERT_EQ(value.value3_.x, ptr[i].value3_.x) << "Data8 was corrupted.";
        ASSERT_EQ(value.value3_.y, ptr[i].value3_.y) << "Data8 was corrupted.";
        ASSERT_EQ(value.value3_.z, ptr[i].value3_.z) << "Data8 was corrupted.";
        ASSERT_EQ(value.value3_.w, ptr[i].value3_.w) << "Data8 was corrupted.";

        ASSERT_FLOAT_EQ(static_cast<float>(value.value4_[0]),
                        static_cast<float>(ptr[i].value4_[0])) << "Data8 was corrupted.";
        ASSERT_FLOAT_EQ(static_cast<float>(value.value4_[1]),
                        static_cast<float>(ptr[i].value4_[1])) << "Data8 was corrupted.";

        ASSERT_EQ(value.value5_.x, ptr[i].value5_.x) << "Data8 was corrupted.";
        ASSERT_EQ(value.value5_.y, ptr[i].value5_.y) << "Data8 was corrupted.";
        ASSERT_EQ(value.value5_.z, ptr[i].value5_.z) << "Data8 was corrupted.";
        ASSERT_EQ(value.value5_.w, ptr[i].value5_.w) << "Data8 was corrupted.";
      }
    }
  }
}
