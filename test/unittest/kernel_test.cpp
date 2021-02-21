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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, simpleKernel, 1);
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

TEST(KernelTest, BasicTypeSizeTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  const std::size_t n = 96;

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, basicTypeSizeKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("BasicTypeSizeKernel");
    auto result = kernel->run(*buff_device1, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    std::size_t i = 0;

    using zivc::int8b;
    using zivc::int16b;
    using zivc::int32b;
    using zivc::int64b;

    EXPECT_EQ(sizeof(int8b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int8b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int16b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int16b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int32b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int32b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int64b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int64b>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl::char2;
    using zivc::cl::char3;
    using zivc::cl::char4;
    using zivc::cl::short2;
    using zivc::cl::short3;
    using zivc::cl::short4;
    using zivc::cl::int2;
    using zivc::cl::int3;
    using zivc::cl::int4;
    using zivc::cl::long2;
    using zivc::cl::long3;
    using zivc::cl::long4;

    EXPECT_EQ(sizeof(char2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<char2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(char3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<char3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(char4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<char4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(short2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<short2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(short3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<short3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(short4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<short4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(int4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<int4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(long2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<long2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(long3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<long3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(long4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<long4>, mem[i++]) << "Type size mismatch!.";

    using zivc::uint8b;
    using zivc::uint16b;
    using zivc::uint32b;
    using zivc::uint64b;

    EXPECT_EQ(sizeof(uint8b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint8b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint16b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint16b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint32b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint32b>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint64b), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint64b>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl::uchar2;
    using zivc::cl::uchar3;
    using zivc::cl::uchar4;
    using zivc::cl::ushort2;
    using zivc::cl::ushort3;
    using zivc::cl::ushort4;
    using zivc::cl::uint2;
    using zivc::cl::uint3;
    using zivc::cl::uint4;
    using zivc::cl::ulong2;
    using zivc::cl::ulong3;
    using zivc::cl::ulong4;

    EXPECT_EQ(sizeof(uchar2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uchar2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uchar3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uchar3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uchar4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uchar4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ushort2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ushort2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ushort3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ushort3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ushort4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ushort4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(uint4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<uint4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ulong2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ulong2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ulong3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ulong3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(ulong4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<ulong4>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl::half;

    EXPECT_EQ(sizeof(half), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<half>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(float), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<float>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(double), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<double>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl::half2;
    using zivc::cl::half3;
    using zivc::cl::half4;
    using zivc::cl::float2;
    using zivc::cl::float3;
    using zivc::cl::float4;
    using zivc::cl::double2;
    using zivc::cl::double3;
    using zivc::cl::double4;

    EXPECT_EQ(sizeof(half2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<half2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(half3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<half3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(half4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<half4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(float2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<float2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(float3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<float3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(float4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<float4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(double2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<double2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(double3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<double3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(double4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<double4>, mem[i++]) << "Type size mismatch!.";

//    using zivc::cl::size_t;
//    using zivc::cl::ptrdiff_t;
//    using zivc::cl::intptr_t;
//    using zivc::cl::uintptr_t;

//    EXPECT_EQ(sizeof(size_t), mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(std::alignment_of_v<size_t>, mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(sizeof(ptrdiff_t), mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(std::alignment_of_v<ptrdiff_t>, mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(sizeof(intptr_t), mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(std::alignment_of_v<intptr_t>, mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(sizeof(uintptr_t), mem[i++]) << "Type size mismatch!.";
//    EXPECT_EQ(std::alignment_of_v<uintptr_t>, mem[i++]) << "Type size mismatch!.";
  }
}

TEST(KernelTest, ClassSize1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(2);
  auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(2);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, classSize1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("ClassSize1Kernel");
    auto result = kernel->run(*buff_device1, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    std::size_t i = 0;

    using TestType = zivc::cl::kernel_test::ClassSizeTest1;

    EXPECT_EQ(sizeof(TestType), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<TestType>, mem[i++]) << "Type size mismatch!.";
  }
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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
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

TEST(KernelTest, InputOutputHost1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputHost2Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputHostToDevice1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kHostToDevice);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputHostToDevice2Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kHostToDevice);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputDeviceToHost1Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceToHost);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputDeviceToHost2Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  const auto& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  auto buff_input = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceToHost);
  buff_input->setSize(n + info.workGroupSize());
  auto buff_output = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_output->setSize(n + info.workGroupSize());
  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    auto options = buff_input->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput1Kernel");
    auto result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_output->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutput2Test)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  const std::size_t n = 256;

  using zivc::int32b;
  using zivc::uint32b;

  using TestType = zivc::cl::kernel_test::ClassSizeTest1;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<TestType>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_device2 = device->makeBuffer<TestType>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(2 * n);
  auto buff_host = device->makeBuffer<TestType>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(2 * n);

  // Init buffers
  {
    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i) {
        mem[i].i16_ = zisc::cast<zivc::int16b>(i);
        mem[i].u_ = zisc::cast<zivc::uint32b>(i);
        mem[i].u8_ = zisc::cast<zivc::uint8b>(i);
        mem[i].u16_ = zisc::cast<zivc::uint16b>(i);
        mem[i].f_ = zisc::cast<float>(i);
      }
    }

    auto options = buff_device1->makeOptions();
    options.setSize(n);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_device1.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, inputOutput2Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({zisc::cast<uint32b>(n)});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("InputOutput2Kernel");
    auto result = kernel->run(*buff_device1, *buff_device2, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const auto mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const std::size_t v = i / 2;
      {
        const auto expected = zisc::cast<zivc::int16b>(v);
        ASSERT_EQ(expected, mem[i].i16())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
      {
        const auto expected = zisc::cast<zivc::uint32b>(v);
        ASSERT_EQ(expected, mem[i].u())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
      {
        const auto expected = zisc::cast<zivc::uint8b>(v);
        ASSERT_EQ(expected, mem[i].u8())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
      {
        const auto expected = zisc::cast<zivc::uint16b>(v);
        ASSERT_EQ(expected, mem[i].u16())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
      {
        const auto expected = zisc::cast<float>(v);
        ASSERT_EQ(expected, mem[i].f())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
    }
  }
}

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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, pod1Kernel, 1);
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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, pod2Kernel, 1);
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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, pod3Kernel, 1);
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
  using PodTest = zivc::cl::kernel_test::inner::PodAlignmentTest;

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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, podSizeAlignmentKernel, 1);
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
  using PodTest = zivc::cl::kernel_test::inner::PodAlignmentTest;

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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, podSizeAlignmentKernel, 1);
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

namespace {

void printWorkItemProperties(const zivc::uint32b* properties) noexcept
{
  std::size_t i = 0;
  std::cout << "## dimension             : " << properties[i++] << std::endl;
  std::cout << "## global size x         : " << properties[i++] << std::endl;
  std::cout << "## global size y         : " << properties[i++] << std::endl;
  std::cout << "## global size z         : " << properties[i++] << std::endl;
  std::cout << "## local size x          : " << properties[i++] << std::endl;
  std::cout << "## local size y          : " << properties[i++] << std::endl;
  std::cout << "## local size z          : " << properties[i++] << std::endl;
  std::cout << "## enqueued local size x : " << properties[i++] << std::endl;
  std::cout << "## enqueued local size y : " << properties[i++] << std::endl;
  std::cout << "## enqueued local size z : " << properties[i++] << std::endl;
  std::cout << "## number of groups x    : " << properties[i++] << std::endl;
  std::cout << "## number of groups y    : " << properties[i++] << std::endl;
  std::cout << "## number of groups z    : " << properties[i++] << std::endl;
  std::cout << "## global offset x       : " << properties[i++] << std::endl;
  std::cout << "## global offset y       : " << properties[i++] << std::endl;
  std::cout << "## global offset z       : " << properties[i++] << std::endl;
}

} // namespace

TEST(KernelTest, WorkItem1dTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 1000;
  constexpr std::size_t n = n_dim;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n + 3);
  auto buff_properties = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_properties->setSize(16);

  // Init buffers
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, workItem1dKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n_dim});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("workItem1dKernel");
    auto result = kernel->run(*buff_device, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_properties->size());

    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      ::printWorkItemProperties(mem.data());

      ASSERT_EQ(1, mem[0])
          << "Dimension property isn't set properly.";
      const std::size_t work_group_size = mem[4] * mem[5] * mem[6];
      ASSERT_EQ(info.workGroupSize(), work_group_size)
          << "Work group size isn't set properly.";
      ASSERT_EQ(0, mem[13])
          << "Global offset x isn't set properly.";
      ASSERT_EQ(0, mem[14])
          << "Global offset y isn't set properly.";
      ASSERT_EQ(0, mem[15])
          << "Global offset z isn't set properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device->size());

    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
      const std::size_t group_size = info.workGroupSize();
      const std::size_t limit = group_size;
      ASSERT_GT(limit, mem[n]) << "Work group size isn't property set.";
      std::cout << "## Num of outliers: " << mem[n] << std::endl;
      ASSERT_EQ(group_size, mem[n + 1]) << "Work group size isn't property set.";
      ASSERT_EQ(group_size, mem[n + 2]) << "Work group size isn't property set.";
    }
  }
}

TEST(KernelTest, WorkItem2dTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 500;
  constexpr std::size_t n = n_dim * n_dim;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n + 3);
  auto buff_properties = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_properties->setSize(16);

  // Init buffers
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, workItem2dKernel, 2);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(2, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n_dim, n_dim});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("workItem2dKernel");
    auto result = kernel->run(*buff_device, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_properties->size());

    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      ::printWorkItemProperties(mem.data());

      ASSERT_EQ(2, mem[0])
          << "Dimension property isn't set properly.";
      const std::size_t work_group_size = mem[4] * mem[5] * mem[6];
      ASSERT_EQ(info.workGroupSize(), work_group_size)
          << "Work group size isn't set properly.";
      ASSERT_EQ(0, mem[13])
          << "Global offset x isn't set properly.";
      ASSERT_EQ(0, mem[14])
          << "Global offset y isn't set properly.";
      ASSERT_EQ(0, mem[15])
          << "Global offset z isn't set properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device->size());

    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
      const std::size_t group_size = info.workGroupSize();
      const std::size_t limit = group_size * group_size;
      ASSERT_GT(limit * limit, mem[n]) << "Work group size isn't property set.";
      std::cout << "## Num of outliers: " << mem[n] << std::endl;
      ASSERT_EQ(group_size, mem[n + 1]) << "Work group size isn't property set.";
      ASSERT_EQ(group_size, mem[n + 2]) << "Work group size isn't property set.";
    }
  }
}

TEST(KernelTest, WorkItem3dTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 250;
  constexpr std::size_t n = n_dim * n_dim * n_dim;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n + 3);
  auto buff_properties = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_properties->setSize(16);

  // Init buffers
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, workItem3dKernel, 3);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(3, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n_dim, n_dim, n_dim});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("workItem3dKernel");
    auto result = kernel->run(*buff_device, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_properties->size());

    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      ::printWorkItemProperties(mem.data());

      ASSERT_EQ(3, mem[0])
          << "Dimension property isn't set properly.";
      const std::size_t work_group_size = mem[4] * mem[5] * mem[6];
      ASSERT_EQ(info.workGroupSize(), work_group_size)
          << "Work group size isn't set properly.";
      ASSERT_EQ(0, mem[13])
          << "Global offset x isn't set properly.";
      ASSERT_EQ(0, mem[14])
          << "Global offset y isn't set properly.";
      ASSERT_EQ(0, mem[15])
          << "Global offset z isn't set properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device->size());

    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
      const std::size_t group_size = info.workGroupSize();
      const std::size_t limit = group_size * group_size;
      ASSERT_GT(limit * limit, mem[n]) << "Work group size isn't property set.";
      std::cout << "## Num of outliers: " << mem[n] << std::endl;
      ASSERT_EQ(group_size, mem[n + 1]) << "Work group size isn't property set.";
      ASSERT_EQ(group_size, mem[n + 2]) << "Work group size isn't property set.";
    }
  }
}

TEST(KernelTest, WorkItemOffset1dTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 1000;
  constexpr std::size_t n = n_dim;
  constexpr std::size_t offset_x = 19;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_device2 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(n + info.workGroupSize() + offset_x);
  auto buff_device3 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device3->setSize(2 * n);
  auto buff_properties = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_properties->setSize(1);

  // Init buffers
  {
    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device1->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device2->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_device3->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device3->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, workItemOffset1dKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n_dim});
    launch_options.setGlobalIdOffset({offset_x});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("workItemOffset1dKernel");
    auto result = kernel->run(*buff_device1, *buff_device2, *buff_device3, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_properties->size());

    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      ASSERT_EQ(offset_x, mem[0]) << "Global offset x is wrong.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device1->size());

    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device2->size());

    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
//      for (std::size_t i = 0; i < mem.size(); ++i) {
//        std::cout << "mem[" << i << "] = " << mem[i] << std::endl;
//      }
      for (std::size_t i = 0; i < offset_x; ++i) {
        ASSERT_EQ(0, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
      }
      for (std::size_t i = 0; i < n; ++i) {
        const std::size_t index = i + offset_x;
        ASSERT_EQ(i, mem[index]) << "Work item[" << index << "] prop isn't set properly.";
      }
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device3->size());

    auto options = buff_device3->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device3, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; ++i) {
        {
          const uint32b x = zisc::cast<uint32b>(i);
          ASSERT_EQ(x, mem[2 * i + 0]) << "Work item[" << i << "] prop isn't set properly.";
        }
        {
          const uint32b x = zisc::cast<uint32b>(i) + offset_x;
          ASSERT_EQ(x, mem[2 * i + 1]) << "Work item[" << i << "] prop isn't set properly.";
        }
      }
    }
  }
}

TEST(KernelTest, WorkItemOffset3dTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;
  using GlobalId = zivc::cl::kernel_test::inner::GlobalId3d;

  constexpr std::size_t n_dim = 250;
  constexpr std::size_t n = n_dim * n_dim * n_dim;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_device2 = device->makeBuffer<GlobalId>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(2 * n);
  auto buff_properties = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_properties->setSize(3);

  // Init buffers
  {
    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device1->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto buff_host = device->makeBuffer<GlobalId>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device2->size());
    {
      constexpr uint32b init_v = 0;
      const GlobalId id{init_v, init_v, init_v};
      auto options = buff_host->makeOptions();
      auto result = buff_host->fill(id, options);
      ASSERT_FALSE(result.isAsync());
    }

    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, buff_device2.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test, workItemOffset3dKernel, 3);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(3, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint32b offset_x = 15;
  constexpr uint32b offset_y = 9;
  constexpr uint32b offset_z = 5;

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n_dim, n_dim, n_dim});
    launch_options.setGlobalIdOffset({offset_x, offset_y, offset_z});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("workItemOffset3dKernel");
    auto result = kernel->run(*buff_device1, *buff_device2, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_properties->size());

    auto options = buff_properties->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      ASSERT_EQ(offset_x, mem[0]) << "Global offset x is wrong.";
      ASSERT_EQ(offset_y, mem[1]) << "Global offset y is wrong.";
      ASSERT_EQ(offset_z, mem[2]) << "Global offset z is wrong.";
    }
  }
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device1->size());

    auto options = buff_device1->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
    }
  }
  {
    auto buff_host = device->makeBuffer<GlobalId>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device2->size());

    auto options = buff_device2->makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      auto mem = buff_host->mapMemory();
      std::size_t index = 0;
      for (std::size_t z = 0; z < n_dim; ++z) {
        const uint32b z_id = zisc::cast<uint32b>(z) + offset_z;
        for (std::size_t y = 0; y < n_dim; ++y) {
          const uint32b y_id = zisc::cast<uint32b>(y) + offset_y;
          for (std::size_t x = 0; x < n_dim; ++x) {
            const std::size_t i = index++;
            const uint32b x_id = zisc::cast<uint32b>(x) + offset_x;
            {
              const GlobalId& id = mem[2 * i + 0];
              ASSERT_EQ(x, id.x_)
                  << "Global offset x is wrong: index=" << i 
                  << " (" << x << "," << y << "," << z << ").";
              ASSERT_EQ(y, id.y_)
                  << "Global offset y is wrong: index=" << i
                  << " (" << x << "," << y << "," << z << ").";
              ASSERT_EQ(z, id.z_)
                  << "Global offset z is wrong: index=" << i
                  << " (" << x << "," << y << "," << z << ").";
            }
            {
              const GlobalId& id = mem[2 * i + 1];
              ASSERT_EQ(x_id, id.x_)
                  << "Global offset x is wrong: index=" << i 
                  << " (" << x << "," << y << "," << z << ").";
              ASSERT_EQ(y_id, id.y_)
                  << "Global offset y is wrong: index=" << i
                  << " (" << x << "," << y << "," << z << ").";
              ASSERT_EQ(z_id, id.z_)
                  << "Global offset z is wrong: index=" << i
                  << " (" << x << "," << y << "," << z << ").";
            }
          }
        }
      }
    }
  }
}
