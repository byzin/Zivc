/*!
  \file kernel_test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/cppcl/types.hpp"
#include "zivc/cppcl/vector.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test_pod.hpp"

TEST(KernelTest, Pod1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(1);
  auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(1);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test_pod, pod1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint32b expected = 0b01010101'10101010'11110000'00001111u;
  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("Pod1Kernel");
    auto result = kernel->run(*buff_device, expected, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    EXPECT_EQ(expected, mem[0]) << "Copying POD value failed.";
  }
}

TEST(KernelTest, Pod2Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_device2 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(n);
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_device1.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test_pod, pod2Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    const uint32b res = zisc::cast<uint32b>(n);
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({res});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("Pod2Kernel");
    auto result = kernel->run(*buff_device1, *buff_device2, res, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    {
      const int32b expected = zisc::cast<int32b>(n);
      EXPECT_EQ(expected, mem[0]) << "Copying POD value failed.";
    }
    for (std::size_t i = 1; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, Pod3Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device_i = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device_i->setSize(3);
  auto buff_device_u = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device_u->setSize(3);
  auto buff_device_f = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device_f->setSize(3);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test_pod, pod3Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  constexpr int32b i0 = (std::numeric_limits<int32b>::min)();
  constexpr int32b i1 = 0b01010101'10101010'11110000'00001111;
  constexpr int32b i2 = (std::numeric_limits<int32b>::max)();
  constexpr uint32b u0 = (std::numeric_limits<uint32b>::min)();
  constexpr uint32b u1 = 0b01010101'10101010'11110000'00001111u;
  constexpr uint32b u2 = (std::numeric_limits<uint32b>::max)();
  constexpr float f0 = (std::numeric_limits<float>::min)();
  constexpr float f1 = std::numbers::pi_v<float>;
  constexpr float f2 = (std::numeric_limits<float>::max)();

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("Pod3Kernel");
    auto result = kernel->run(
        i0, u0, f0,
        i1, u1, f1,
        i2, u2, f2,
        *buff_device_i, *buff_device_u, *buff_device_f, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(3);

    auto options = buff_device_i->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device_i, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const auto mem = buff_host->mapMemory();
      EXPECT_EQ(i0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(i1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(i2, mem[2]) << "POD copying failed.";
    }
  }

  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(3);

    auto options = buff_device_u->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device_u, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const auto mem = buff_host->mapMemory();
      EXPECT_EQ(u0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(u1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(u2, mem[2]) << "POD copying failed.";
    }
  }

  {
    auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(3);

    auto options = buff_device_f->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device_f, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const auto mem = buff_host->mapMemory();
      EXPECT_EQ(f0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(f1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(f2, mem[2]) << "POD copying failed.";
    }
  }
}

TEST(KernelTest, PodSizeAlignmentTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using PodTest = zivc::cl::kernel_test_pod::inner::PodAlignmentTest;

  auto init_buffer = [](zivc::Device& d, auto& buffer, const auto value)
  {
    using Type = std::add_const_t<decltype(value)>;
    auto buff_host = d.makeBuffer<Type>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buffer.size());
    {
      auto mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), value);
    }
    auto options = buff_host->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_host, std::addressof(buffer), options);
    d.waitForCompletion();
  };

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(2);
  auto buff_device2 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(2);
  auto buff_device3 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device3->setSize(2);
  auto buff_device4 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device4->setSize(2);
  auto buff_device5 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device5->setSize(2);
  auto buff_device6 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device6->setSize(2);
  auto buff_device7 = device->makeBuffer<PodTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device7->setSize(2);

  init_buffer(*device, *buff_device1, zisc::cast<uint8b>(0));
  init_buffer(*device, *buff_device2, zisc::cast<float>(0));
  init_buffer(*device, *buff_device3, zisc::cast<int8b>(0));
  init_buffer(*device, *buff_device4, zisc::cast<int16b>(0));
  init_buffer(*device, *buff_device5, zisc::cast<uint16b>(0));
  init_buffer(*device, *buff_device6, zisc::cast<int32b>(0));
  init_buffer(*device, *buff_device7, PodTest{});

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test_pod, podSizeAlignmentKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(14, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint8b u8 = 0b10101010u;
  constexpr float f32 = std::numbers::pi_v<float>;
  constexpr int8b i8 = 0b01010101;
  constexpr int16b i16 = 0b01010101'10101010;
  constexpr uint16b u16 = 0b10101010'01010101u;
  constexpr int32b i32 = 0b01010101'10101010'11110000'00001111;
  constexpr PodTest test{u8, 0, 0, 0, f32, i8, 0, i16, u16, 0, i32, 0, 0, 0};

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("PodSizeAlignmentKernel");
    auto result = kernel->run(
        *buff_device1, *buff_device2, *buff_device3, *buff_device4,
        *buff_device5, *buff_device6, *buff_device7,
        u8, f32, i8, i16, u16, i32, test, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint8b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device1->size());
    auto options = buff_device1->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device2->size());
    auto options = buff_device2->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<int8b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device3->size());
    auto options = buff_device3->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device3, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<int16b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device4->size());
    auto options = buff_device4->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device4, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint16b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device5->size());
    auto options = buff_device5->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device5, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device6->size());
    auto options = buff_device6->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device6, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
//      ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<PodTest>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device7->size());
    auto options = buff_device7->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_device7, buff_host.get(), options);
    device->waitForCompletion();
    {
      const auto mem = buff_host->mapMemory();
      ASSERT_EQ(u8, mem[0].u8_) << "POD value isn't processed properly.";
      ASSERT_EQ(u8, mem[1].u8_) << "POD structure isn't processed properly.";
      ASSERT_EQ(f32, mem[0].f_) << "POD value isn't processed properly.";
      ASSERT_EQ(f32, mem[1].f_) << "POD structure isn't processed properly.";
      ASSERT_EQ(i8, mem[0].i8_) << "POD value isn't processed properly.";
      ASSERT_EQ(i8, mem[1].i8_) << "POD structure isn't processed properly.";
      ASSERT_EQ(i16, mem[0].i16_) << "POD value isn't processed properly.";
      ASSERT_EQ(i16, mem[1].i16_) << "POD structure isn't processed properly.";
      ASSERT_EQ(u16, mem[0].u16_) << "POD value isn't processed properly.";
      ASSERT_EQ(u16, mem[1].u16_) << "POD structure isn't processed properly.";
      ASSERT_EQ(i32, mem[0].i32_) << "POD value isn't processed properly.";
      ASSERT_EQ(i32, mem[1].i32_) << "POD structure isn't processed properly.";
    }
  }
}

TEST(KernelTest, PodMultipleValuesTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using PodTest = zivc::cl::kernel_test_pod::inner::PodAlignmentTest;

  auto init_buffer = [](zivc::Device& d, auto& buffer, const auto value)
  {
    using Type = std::add_const_t<decltype(value)>;
    auto buff_host = d.makeBuffer<Type>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buffer.size());
    {
      auto mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), value);
    }
    auto options = buff_host->makeOptions();
    [[maybe_unused]] auto result = zivc::copy(*buff_host, std::addressof(buffer), options);
    d.waitForCompletion();
  };

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(2);
  auto buff_device2 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(2);
  auto buff_device3 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device3->setSize(2);
  auto buff_device4 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device4->setSize(2);
  auto buff_device5 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device5->setSize(2);
  auto buff_device6 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device6->setSize(2);
  auto buff_device7 = device->makeBuffer<PodTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device7->setSize(2);

  init_buffer(*device, *buff_device1, zisc::cast<uint8b>(0));
  init_buffer(*device, *buff_device2, zisc::cast<float>(0));
  init_buffer(*device, *buff_device3, zisc::cast<int8b>(0));
  init_buffer(*device, *buff_device4, zisc::cast<int16b>(0));
  init_buffer(*device, *buff_device5, zisc::cast<uint16b>(0));
  init_buffer(*device, *buff_device6, zisc::cast<int32b>(0));
  init_buffer(*device, *buff_device7, PodTest{});

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test_pod, podSizeAlignmentKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(14, kernel->argSize()) << "Wrong kernel property.";

  // Try1
  {
    constexpr uint8b u8 = 0b10101010u;
    constexpr float f32 = std::numbers::pi_v<float>;
    constexpr int8b i8 = 0b01010101;
    constexpr int16b i16 = 0b01010101'10101010;
    constexpr uint16b u16 = 0b10101010'01010101u;
    constexpr int32b i32 = 0b01010101'10101010'11110000'00001111;
    constexpr PodTest test{u8, 0, 0, 0, f32, i8, 0, i16, u16, 0, i32, 0, 0, 0};

    // Launch the kernel
    {
      auto launch_options = kernel->makeOptions();
      launch_options.setWorkSize({1});
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("PodSizeAlignmentKernel");
      auto result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      auto buff_host = device->makeBuffer<uint8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device1->size());
      auto options = buff_device1->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device2->size());
      auto options = buff_device2->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device3->size());
      auto options = buff_device3->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device4->size());
      auto options = buff_device4->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<uint16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device5->size());
      auto options = buff_device5->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device6->size());
      auto options = buff_device6->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<PodTest>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device7->size());
      auto options = buff_device7->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0].u8_) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1].u8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(f32, mem[0].f_) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1].f_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i8, mem[0].i8_) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1].i8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i16, mem[0].i16_) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1].i16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(u16, mem[0].u16_) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1].u16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i32, mem[0].i32_) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1].i32_) << "POD structure isn't processed properly.";
      }
    }
  }
  // Try2
  {
    constexpr uint8b u8 = 1;
    constexpr float f32 = 1.0f;
    constexpr int8b i8 = 1;
    constexpr int16b i16 = 1;
    constexpr uint16b u16 = 1;
    constexpr int32b i32 = 1;
    constexpr PodTest test{u8, 0, 0, 0, f32, i8, 0, i16, u16, 0, i32, 0, 0, 0};

    // Launch the kernel
    {
      auto launch_options = kernel->makeOptions();
      launch_options.setWorkSize({1});
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("PodSizeAlignmentKernel");
      auto result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      auto buff_host = device->makeBuffer<uint8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device1->size());
      auto options = buff_device1->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device2->size());
      auto options = buff_device2->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device3->size());
      auto options = buff_device3->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device4->size());
      auto options = buff_device4->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<uint16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device5->size());
      auto options = buff_device5->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device6->size());
      auto options = buff_device6->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<PodTest>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device7->size());
      auto options = buff_device7->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0].u8_) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1].u8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(f32, mem[0].f_) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1].f_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i8, mem[0].i8_) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1].i8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i16, mem[0].i16_) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1].i16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(u16, mem[0].u16_) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1].u16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i32, mem[0].i32_) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1].i32_) << "POD structure isn't processed properly.";
      }
    }
  }
  // Try3
  {
    constexpr uint8b u8 = 10;
    constexpr float f32 = 10.0f;
    constexpr int8b i8 = 10;
    constexpr int16b i16 = 10;
    constexpr uint16b u16 = 10;
    constexpr int32b i32 = 10;
    constexpr PodTest test{u8, 0, 0, 0, f32, i8, 0, i16, u16, 0, i32, 0, 0, 0};

    // Launch the kernel
    {
      auto launch_options = kernel->makeOptions();
      launch_options.setWorkSize({1});
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("PodSizeAlignmentKernel");
      auto result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      auto buff_host = device->makeBuffer<uint8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device1->size());
      auto options = buff_device1->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device2->size());
      auto options = buff_device2->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int8b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device3->size());
      auto options = buff_device3->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device4->size());
      auto options = buff_device4->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<uint16b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device5->size());
      auto options = buff_device5->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device6->size());
      auto options = buff_device6->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      auto buff_host = device->makeBuffer<PodTest>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device7->size());
      auto options = buff_device7->makeOptions();
      [[maybe_unused]] auto result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const auto mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0].u8_) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1].u8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(f32, mem[0].f_) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1].f_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i8, mem[0].i8_) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1].i8_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i16, mem[0].i16_) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1].i16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(u16, mem[0].u16_) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1].u16_) << "POD structure isn't processed properly.";
        ASSERT_EQ(i32, mem[0].i32_) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1].i32_) << "POD structure isn't processed properly.";
      }
    }
  }
}
