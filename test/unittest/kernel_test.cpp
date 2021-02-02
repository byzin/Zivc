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
#include <numbers>
#include <numeric>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test.hpp"

TEST(KernelTest, SimpleKernelTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  // Allocate buffers
  auto buffer = device->makeBuffer<zivc::int32b>(zivc::BufferUsage::kDeviceOnly);
  buffer->setSize(1);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(kernel_test, simpleKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.setExternalSyncMode(false);
  launch_options.setLabel("SimpleKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_FALSE(launch_options.isExternalSyncMode());
  auto result = kernel->run(*buffer, launch_options);
  device->waitForCompletion();
}

TEST(KernelTest, InputOutput1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n + info.workGroupSize());
  auto buff_device2 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_device1->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_device1.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_device1, *buff_device2, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device2->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, Pod1Test)
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
  auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(kernel_test, pod1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    const uint32b res = zisc::cast<uint32b>(n);
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({res});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("Pod1Kernel");
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

TEST(KernelTest, Pod2Test)
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
  auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(kernel_test, pod2Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  constexpr int32b i0 = (std::numeric_limits<int32b>::min)();
  constexpr int32b i1 = 0b01010101'10101010'11110000'00001111;
  constexpr int32b i2 = (std::numeric_limits<int32b>::max)();
  constexpr int32b u0 = (std::numeric_limits<uint32b>::min)();
  constexpr int32b u1 = 0b01010101'10101010'11110000'00001111u;
  constexpr int32b u2 = (std::numeric_limits<uint32b>::max)();
  constexpr float f0 = (std::numeric_limits<float>::min)();
  constexpr float f1 = std::numbers::pi_v<float>;
  constexpr float f2 = (std::numeric_limits<float>::max)();

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("Pod2Kernel");
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

    const auto mem = buff_host->mapMemory();
    EXPECT_EQ(i0, mem[0]) << "POD copying failed.";
    EXPECT_EQ(i1, mem[1]) << "POD copying failed.";
    EXPECT_EQ(i2, mem[2]) << "POD copying failed.";
  }

  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(3);

    auto options = buff_device_u->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device_u, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    EXPECT_EQ(u0, mem[0]) << "POD copying failed.";
    EXPECT_EQ(u1, mem[1]) << "POD copying failed.";
    EXPECT_EQ(u2, mem[2]) << "POD copying failed.";
  }

  {
    auto buff_host = device->makeBuffer<float>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(3);

    auto options = buff_device_f->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device_f, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    EXPECT_EQ(f0, mem[0]) << "POD copying failed.";
    EXPECT_EQ(f1, mem[1]) << "POD copying failed.";
    EXPECT_EQ(f2, mem[2]) << "POD copying failed.";
  }
}
