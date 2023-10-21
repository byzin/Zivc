/*!
  \file kernel_test1.cpp
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
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test1.hpp"

TEST(KernelTest, SimpleKernelTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  const zivc::SharedBuffer buffer = device->createBuffer<zivc::int32b>({zivc::BufferUsage::kPreferDevice});
  buffer->setSize(1);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, simpleKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(false);
  launch_options.setLabel("SimpleKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_FALSE(launch_options.isFenceRequested());
  const zivc::LaunchResult result = kernel->run(*buffer, launch_options);
  device->waitForCompletion();
}

TEST(KernelTest, BasicTypeSizeTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = 96;

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n);
  const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_h->setSize(n);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, basicTypeSizeKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("BasicTypeSizeKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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

    using zivc::cl_char2;
    using zivc::cl_char3;
    using zivc::cl_char4;
    using zivc::cl_short2;
    using zivc::cl_short3;
    using zivc::cl_short4;
    using zivc::cl_int2;
    using zivc::cl_int3;
    using zivc::cl_int4;
    using zivc::cl_long2;
    using zivc::cl_long3;
    using zivc::cl_long4;

    EXPECT_EQ(sizeof(cl_char2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_char2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_char3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_char3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_char4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_char4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_short2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_short2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_short3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_short3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_short4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_short4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_int2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_int2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_int3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_int3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_int4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_int4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_long2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_long2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_long3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_long3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_long4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_long4>, mem[i++]) << "Type size mismatch!.";

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

    using zivc::cl_uchar2;
    using zivc::cl_uchar3;
    using zivc::cl_uchar4;
    using zivc::cl_ushort2;
    using zivc::cl_ushort3;
    using zivc::cl_ushort4;
    using zivc::cl_uint2;
    using zivc::cl_uint3;
    using zivc::cl_uint4;
    using zivc::cl_ulong2;
    using zivc::cl_ulong3;
    using zivc::cl_ulong4;

    EXPECT_EQ(sizeof(cl_uchar2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uchar2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_uchar3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uchar3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_uchar4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uchar4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ushort2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ushort2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ushort3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ushort3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ushort4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ushort4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_uint2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uint2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_uint3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uint3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_uint4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_uint4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ulong2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ulong2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ulong3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ulong3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_ulong4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_ulong4>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl_half;

    EXPECT_EQ(sizeof(cl_half), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_half>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(float), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<float>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(double), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<double>, mem[i++]) << "Type size mismatch!.";

    using zivc::cl_half2;
    using zivc::cl_half3;
    using zivc::cl_half4;
    using zivc::cl_float2;
    using zivc::cl_float3;
    using zivc::cl_float4;
    using zivc::cl_double2;
    using zivc::cl_double3;
    using zivc::cl_double4;

    EXPECT_EQ(sizeof(cl_half2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_half2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_half3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_half3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_half4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_half4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_float2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_float2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_float3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_float3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_float4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_float4>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_double2), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_double2>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_double3), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_double3>, mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(sizeof(cl_double4), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<cl_double4>, mem[i++]) << "Type size mismatch!.";

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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(2);
  const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_h->setSize(2);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, classSize1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("ClassSize1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
    std::size_t i = 0;

    using TestType = zivc::cl::ClassSizeTest1;
    EXPECT_EQ(sizeof(TestType), mem[i++]) << "Type size mismatch!.";
    EXPECT_EQ(std::alignment_of_v<TestType>, mem[i++]) << "Type size mismatch!.";
  }
}

TEST(KernelTest, InputOutput1Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_d2 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  buff_d2->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_h = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_h->setSize(n);

  // Init buffers
  {
    {
      zivc::MappedMemory<int32b> mem = buff_h->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_h, buff_d.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, inputOutput1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{zisc::cast<uint32b>(n)}});
    launch_options.requestFence(false);
    launch_options.setLabel("InputOutput1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, *buff_d2, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_d2->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d2, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<int32b> mem = buff_h->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputHost1Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_input = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  buff_input->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_output = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost});
  buff_output->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(n);

  // Init buffers
  {
    {
      zivc::MappedMemory<int32b> mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    zivc::BufferLaunchOptions options = buff_input->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, inputOutput1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{zisc::cast<uint32b>(n)}});
    launch_options.requestFence(false);
    launch_options.setLabel("InputOutput1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_output->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<int32b> mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutputHost2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_input = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost});
  buff_input->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_output = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  buff_output->setSize(n + info.workGroupSize());
  const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(n);

  // Init buffers
  {
    {
      zivc::MappedMemory<int32b> mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    zivc::BufferLaunchOptions options = buff_input->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_host, buff_input.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, inputOutput1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{zisc::cast<uint32b>(n)}});
    launch_options.requestFence(false);
    launch_options.setLabel("InputOutput1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_input, *buff_output, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_output->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_output, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<int32b> mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const int32b expected = zisc::cast<int32b>(i);
      ASSERT_EQ(expected, mem[i])
          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
    }
  }
}

TEST(KernelTest, InputOutput2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = 256;

  using zivc::int32b;
  using zivc::uint32b;

  using TestType = zivc::cl::ClassSizeTest1;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<TestType>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n);
  const zivc::SharedBuffer buff_d2 = device->createBuffer<TestType>({zivc::BufferUsage::kPreferDevice});
  buff_d2->setSize(2 * n);
  const zivc::SharedBuffer buff_h = device->createBuffer<TestType>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_h->setSize(2 * n);

  // Init buffers
  {
    {
      zivc::MappedMemory<TestType> mem = buff_h->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i) {
        mem[i].i16_ = zisc::cast<zivc::int16b>(i);
        mem[i].u32_ = zisc::cast<zivc::uint32b>(i);
        mem[i].u8_ = zisc::cast<zivc::uint8b>(i);
        mem[i].u16_ = zisc::cast<zivc::uint16b>(i);
        mem[i].f_ = zisc::cast<float>(i);
      }
    }

    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.setSize(n);
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_h, buff_d.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, inputOutput2Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{zisc::cast<uint32b>(n)}});
    launch_options.requestFence(false);
    launch_options.setLabel("InputOutput2Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, *buff_d2, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_d2->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d2, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory<TestType> mem = buff_h->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const std::size_t v = i / 2;
      {
        const auto expected = zisc::cast<zivc::int16b>(v);
        ASSERT_EQ(expected, mem[i].i16())
            << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
      }
      {
        const auto expected = zisc::cast<zivc::uint32b>(v);
        ASSERT_EQ(expected, mem[i].u32())
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 1000;
  constexpr std::size_t n = n_dim;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n + 3);
  const zivc::SharedBuffer buff_p = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_p->setSize(16);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_d->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_p->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, workItem1dKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n_dim}});
    launch_options.requestFence(false);
    launch_options.setLabel("workItem1dKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, *buff_p, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_p->size());

    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_p, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_d->size());

    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 500;
  constexpr std::size_t n = n_dim * n_dim;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n + 3);
  const zivc::SharedBuffer buff_p = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_p->setSize(16);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_d->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_p->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, workItem2dKernel, 2);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(2, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n_dim, n_dim}});
    launch_options.requestFence(false);
    launch_options.setLabel("workItem2dKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, *buff_p, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_p->size());

    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_p, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_d->size());

    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 250;
  constexpr std::size_t n = n_dim * n_dim * n_dim;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(n + 3);
  const zivc::SharedBuffer buff_p = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_p->setSize(16);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_d->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_p->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, workItem3dKernel, 3);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(3, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n_dim, n_dim, n_dim}});
    launch_options.requestFence(false);
    launch_options.setLabel("workItem3dKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, *buff_p, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_p->size());

    zivc::BufferLaunchOptions options = buff_p->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_p, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
    const zivc::SharedBuffer buff_h = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    buff_h->setSize(buff_d->size());

    zivc::BufferLaunchOptions options = buff_d->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory<uint32b> mem = buff_h->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t n_dim = 1000;
  constexpr std::size_t n = n_dim;
  constexpr std::size_t offset_x = 19;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device1->setSize(n);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device2->setSize(n + info.workGroupSize() + offset_x);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device3->setSize(2 * n);
  const zivc::SharedBuffer buff_properties = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_properties->setSize(1);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_device1->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device1->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_device2->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device2->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_device3->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device3->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_properties->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, workItemOffset1dKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n_dim}});
    launch_options.setGlobalIdOffset({{offset_x}});
    launch_options.requestFence(false);
    launch_options.setLabel("workItemOffset1dKernel");
    const zivc::LaunchResult result = kernel->run(*buff_device1, *buff_device2, *buff_device3, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_properties->size());

    zivc::BufferLaunchOptions options = buff_properties->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(offset_x, mem[0]) << "Global offset x is wrong.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());

    zivc::BufferLaunchOptions options = buff_device1->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < n; i++)
        ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device2->size());

    zivc::BufferLaunchOptions options = buff_device2->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
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
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device3->size());

    zivc::BufferLaunchOptions options = buff_device3->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;
  using GlobalId = zivc::cl::inner::GlobalId3d;

  constexpr std::size_t n_dim = 250;
  constexpr std::size_t n = n_dim * n_dim * n_dim;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(n);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<GlobalId>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(2 * n);
  const zivc::SharedBuffer buff_properties = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_properties->setSize(3);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_device1->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device1->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<GlobalId>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device2->size());
    {
      constexpr uint32b init_v = 0;
      const GlobalId id{init_v, init_v, init_v};
      zivc::MappedMemory mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), id);
    }

    zivc::BufferLaunchOptions options = buff_device2->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_host, buff_device2.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::BufferLaunchOptions options = buff_properties->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_properties->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, workItemOffset3dKernel, 3);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(3, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint32b offset_x = 15;
  constexpr uint32b offset_y = 9;
  constexpr uint32b offset_z = 5;

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n_dim, n_dim, n_dim}});
    launch_options.setGlobalIdOffset({{offset_x, offset_y, offset_z}});
    launch_options.requestFence(false);
    launch_options.setLabel("workItemOffset3dKernel");
    const zivc::LaunchResult result = kernel->run(*buff_device1, *buff_device2, *buff_properties, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_properties->size());

    zivc::BufferLaunchOptions options = buff_properties->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_properties, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(offset_x, mem[0]) << "Global offset x is wrong.";
      ASSERT_EQ(offset_y, mem[1]) << "Global offset y is wrong.";
      ASSERT_EQ(offset_z, mem[2]) << "Global offset z is wrong.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());

    zivc::BufferLaunchOptions options = buff_device1->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      //auto mem = buff_host->mapMemory();
      //for (std::size_t i = 0; i < n; i++)
      //  ASSERT_EQ(2, mem[i]) << "Work item[" << i << "] prop isn't set properly.";
    }
  }
}

TEST(KernelTest, PrintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_d = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_d->setSize(1);
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, printKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("PrintKernel");
    const zivc::LaunchResult result = kernel->run(*buff_d, launch_options);
    device->waitForCompletion();
  }

  SUCCEED();
}

TEST(KernelTest, AssertionTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::uint32b;

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test1, assertionKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  if (device->type() == zivc::BackendType::kCpu) {
    // Launch the kernel
    ASSERT_DEATH(
    {
      uint32b data = 0;
      kernel_params.func()(&data);
    },
    "## 'assertion' with multi args. i=1, u=2, f=3, c=4");
  }
  else {
    SUCCEED();
  }
}
