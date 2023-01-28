/*!
  \file cl_cpp_test_atomic.cpp
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
#include <initializer_list>
#include <limits>
#include <numbers>
#include <random>
#include <span>
#include <type_traits>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_atomic.hpp"

TEST(ClCppTest, AtomicFetchAddTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 2560 * 1440;
  constexpr std::size_t loop = 10;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int> v = {0};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint> v = {0};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchAddTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchAddTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, n, loop, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::int32b expected = n * loop;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::uint32b expected = n * loop;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
}

//TEST(ClCppTest, AtomicFetchAddLocalTest)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//  const zivc::DeviceInfo& info = device->deviceInfo();
//
//  // Allocate buffers
//  using zivc::cl_int;
//  using zivc::cl_uint;
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//
//  constexpr std::size_t n = 0b01 << 14;
//  constexpr std::size_t loop = 10;
//
//  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
//  buffer_out1->setSize(n / info.workGroupSize());
//  {
//    zivc::LaunchResult result = zivc::fill(0, buffer_out1.get(), buffer_out1->createOptions());
//    result.fence().wait();
//  }
//  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
//  buffer_out2->setSize(n / info.workGroupSize());
//  {
//    zivc::LaunchResult result = zivc::fill(0, buffer_out2.get(), buffer_out2->createOptions());
//    result.fence().wait();
//  }
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchAddLocalTest1Kernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchAddLocalTest2Kernel, 1);
//  const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
//  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel2->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel1
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("atomicFetchAddLocalTest1Kernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_out1, n, loop, launch_options);
//
//  // Launch the kernel2
//  zivc::KernelLaunchOptions launch_options2 = kernel2->createOptions();
//  launch_options2.setQueueIndex(0);
//  launch_options2.setWorkSize({{n}});
//  launch_options2.requestFence(true);
//  launch_options2.setLabel("atomicFetchAddLocalTest2Kernel");
//  ASSERT_EQ(1, launch_options2.dimension());
//  ASSERT_EQ(3, launch_options2.numOfArgs());
//  ASSERT_EQ(0, launch_options2.queueIndex());
//  ASSERT_EQ(n, launch_options2.workSize()[0]);
//  ASSERT_TRUE(launch_options2.isFenceRequested());
//  zivc::LaunchResult result2 = kernel2->run(*buffer_out2, n, loop, launch_options2);
//
//  device->waitForCompletion(result.fence());
//  device->waitForCompletion(result2.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out1, tmp.get());
//    const zivc::MappedMemory mem = tmp->mapMemory();
//    const auto expected = static_cast<zivc::int32b>(info.workGroupSize() * loop);
//    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of local int failed.";
//  }
//  // output2
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out2, tmp.get());
//    const zivc::MappedMemory mem = tmp->mapMemory();
//    const auto expected = static_cast<zivc::uint32b>(info.workGroupSize() * loop);
//    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
//  }
//}

TEST(ClCppTest, AtomicFetchDecTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 2560 * 1440;
  constexpr std::size_t loop = 10;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int> v = {n * loop};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint> v = {n * loop};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchDecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchDecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, n, loop, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::int32b expected = 0;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_dec of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::uint32b expected = 0;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_dec of int failed.";
  }
}
