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
#include <algorithm>
#include <array>
#include <initializer_list>
#include <limits>
#include <numbers>
#include <random>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_atomic.hpp"

namespace {

constexpr std::size_t nLoop() noexcept {return 10;}

} /* namespace */

TEST(ClCppTest, AtomicExchangeTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;

  // Initialize inputs
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(::nLoop() * n + 1);
  ztest::fillDeviceBuffer(static_cast<cl_int>(0), buffer_out1.get());
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(::nLoop() * n + 1);
  ztest::fillDeviceBuffer(static_cast<cl_uint>(0), buffer_out2.get());

  const zivc::SharedBuffer buffer_index1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int> v = {static_cast<cl_int>(buffer_out1->size() - 1)};
    ztest::setDeviceBuffer(*device, v, buffer_index1.get());
  }
  const zivc::SharedBuffer buffer_index2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint> v = {static_cast<cl_uint>(buffer_out2->size() - 1)};
    ztest::setDeviceBuffer(*device, v, buffer_index2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicExchangeTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicExchangeTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_index1, *buffer_index2,
                                          *buffer_out1, *buffer_out2,
                                          n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    const auto expected = static_cast<cl_int>(mem.size() - 1);
    cl_int c = 0;
    std::for_each(mem.begin(), mem.end(), [&c](const cl_int& v){c += v;});
    ASSERT_EQ(expected, c) << "atomic_xchg of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    const auto expected = static_cast<cl_uint>(mem.size() - 1);
    cl_uint c = 0;
    std::for_each(mem.begin(), mem.end(), [&c](const cl_int& v){c += v;});
    ASSERT_EQ(expected, c) << "atomic_xchg of int failed.";
  }
}

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
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::int32b expected = ::nLoop() * n;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::uint32b expected = ::nLoop() * n;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
}

TEST(ClCppTest, AtomicFetchAddLocalTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchAddLocalTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchAddLocalTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::int32b expected = ::nLoop() * n;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::uint32b expected = ::nLoop() * n;
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
}

TEST(ClCppTest, AtomicLocalTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  const zivc::DeviceInfo& info = device->deviceInfo();

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 0b01 << 14;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n / info.workGroupSize());
  {
    zivc::BufferLaunchOptions options = buffer_out1->createOptions();
    options.requestFence(true);
    zivc::LaunchResult result = zivc::fill(0, buffer_out1.get(), options);
    result.fence().wait();
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n / info.workGroupSize());
  {
    zivc::BufferLaunchOptions options = buffer_out2->createOptions();
    options.requestFence(true);
    zivc::LaunchResult result = zivc::fill(0, buffer_out2.get(), options);
    result.fence().wait();
  }

  device->setFenceSize(2);
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicLocalTest1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicLocalTest2Kernel, 1);
  const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel2->argSize()) << "Wrong kernel property.";

  // Launch the kernel1
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicLocalTest1Kernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, n, ::nLoop(), launch_options);

  // Launch the kernel2
  zivc::KernelLaunchOptions launch_options2 = kernel2->createOptions();
  launch_options2.setQueueIndex(0);
  launch_options2.setWorkSize({{n}});
  launch_options2.requestFence(true);
  launch_options2.setLabel("atomicLocalTest2Kernel");
  ASSERT_EQ(1, launch_options2.dimension());
  ASSERT_EQ(3, launch_options2.numOfArgs());
  ASSERT_EQ(0, launch_options2.queueIndex());
  ASSERT_EQ(n, launch_options2.workSize()[0]);
  ASSERT_TRUE(launch_options2.isFenceRequested());
  zivc::LaunchResult result2 = kernel2->run(*buffer_out2, n, ::nLoop(), launch_options2);

  device->waitForCompletion(result.fence());
  device->waitForCompletion(result2.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    const auto expected = static_cast<zivc::int32b>(info.workGroupSize() * ::nLoop());
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of local int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    const auto expected = static_cast<zivc::uint32b>(info.workGroupSize() * ::nLoop());
    ASSERT_EQ(expected, mem[0]) << "atomic_fetch_add of int failed.";
  }
}

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

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int> v = {::nLoop() * n};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint> v = {::nLoop() * n};
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
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, n, ::nLoop(), launch_options);
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

TEST(ClCppTest, AtomicFetchAndTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;
  static_assert(((::nLoop() * n) % (8 * sizeof(cl_uint))) == 0);
  constexpr std::size_t buffer_size = (::nLoop() * n) / (8 * sizeof(cl_uint));

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(buffer_size);
  ztest::fillDeviceBuffer((std::numeric_limits<cl_uint>::max)(), buffer_out1.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchAndTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchAndTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      constexpr cl_uint expected = 0;
      ASSERT_EQ(expected, mem[i]) << "atomic_fetch_and of uint failed at mem[" << i << "]";
    }
  }
}

TEST(ClCppTest, AtomicFetchOrTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;
  static_assert(((::nLoop() * n) % (8 * sizeof(cl_uint))) == 0);
  constexpr std::size_t buffer_size = (::nLoop() * n) / (8 * sizeof(cl_uint));

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(buffer_size);
  ztest::fillDeviceBuffer(static_cast<cl_uint>(0), buffer_out1.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchOrTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchOrTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      constexpr cl_uint expected = (std::numeric_limits<cl_uint>::max)();
      ASSERT_EQ(expected, mem[i]) << "atomic_fetch_and of uint failed at mem[" << i << "]";
    }
  }
}
TEST(ClCppTest, AtomicFetchXorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;
  static_assert(((::nLoop() * n) % (8 * sizeof(cl_uint))) == 0);
  constexpr std::size_t buffer_size = (::nLoop() * n) / (8 * sizeof(cl_uint));

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(buffer_size);
  ztest::fillDeviceBuffer((std::numeric_limits<cl_uint>::max)(), buffer_out1.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchXorTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchXorTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      constexpr cl_uint expected = 0;
      ASSERT_EQ(expected, mem[i]) << "atomic_fetch_xor of uint failed at mem[" << i << "]";
    }
  }
}

TEST(ClCppTest, AtomicFetchMinTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;

  //
  cl_int expected1 = (std::numeric_limits<cl_int>::max)();
  cl_uint expected2 = (std::numeric_limits<cl_uint>::max)();

  // Initialize inputs
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    std::vector<cl_uint> v{};
    v.resize(::nLoop() * n);
    std::mt19937 engine{123'456'789};
    for (std::size_t i = 0; i < v.size(); ++i) {
      const cl_uint u = engine();
      expected1 = (std::min)(expected1, zisc::bit_cast<cl_int>(u));
      expected2 = (std::min)(expected2, u);
      v[i] = u;
    }
    {
      const auto* v1 = reinterpret_cast<const std::vector<cl_int>*>(&v);
      const std::span<const cl_int> l{v1->begin(), v1->end()};
      ztest::setDeviceBuffer(*device, l, buffer_in1.get());
    }
    {
      const std::span<const cl_uint> l{v.begin(), v.end()};
      ztest::setDeviceBuffer(*device, l, buffer_in2.get());
    }
  }

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int m = (std::numeric_limits<cl_int>::max)();
    const std::initializer_list<cl_int> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_uint m = (std::numeric_limits<cl_uint>::max)();
    const std::initializer_list<cl_uint> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchMinTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchMinTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2,
                                          *buffer_out1, *buffer_out2,
                                          n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected1, mem[0]) << "atomic_fetch_min of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected2, mem[0]) << "atomic_fetch_min of int failed.";
  }
}

TEST(ClCppTest, AtomicFetchMinLocalTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;

  //
  cl_int expected1 = (std::numeric_limits<cl_int>::max)();
  cl_uint expected2 = (std::numeric_limits<cl_uint>::max)();

  // Initialize inputs
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    std::vector<cl_uint> v{};
    v.resize(::nLoop() * n);
    std::mt19937 engine{123'456'789};
    for (std::size_t i = 0; i < v.size(); ++i) {
      const cl_uint u = engine();
      expected1 = (std::min)(expected1, zisc::bit_cast<cl_int>(u));
      expected2 = (std::min)(expected2, u);
      v[i] = u;
    }
    {
      const auto* v1 = reinterpret_cast<const std::vector<cl_int>*>(&v);
      const std::span<const cl_int> l{v1->begin(), v1->end()};
      ztest::setDeviceBuffer(*device, l, buffer_in1.get());
    }
    {
      const std::span<const cl_uint> l{v.begin(), v.end()};
      ztest::setDeviceBuffer(*device, l, buffer_in2.get());
    }
  }

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int m = (std::numeric_limits<cl_int>::max)();
    const std::initializer_list<cl_int> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_uint m = (std::numeric_limits<cl_uint>::max)();
    const std::initializer_list<cl_uint> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchMinLocalTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchMinLocalTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2,
                                          *buffer_out1, *buffer_out2,
                                          n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected1, mem[0]) << "atomic_fetch_min of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected2, mem[0]) << "atomic_fetch_min of int failed.";
  }
}

TEST(ClCppTest, AtomicFetchMaxTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;

  //
  cl_int expected1 = (std::numeric_limits<cl_int>::min)();
  cl_uint expected2 = (std::numeric_limits<cl_uint>::min)();

  // Initialize inputs
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    std::vector<cl_uint> v{};
    v.resize(::nLoop() * n);
    std::mt19937 engine{123'456'789};
    for (std::size_t i = 0; i < v.size(); ++i) {
      const cl_uint u = engine();
      expected1 = (std::max)(expected1, zisc::bit_cast<cl_int>(u));
      expected2 = (std::max)(expected2, u);
      v[i] = u;
    }
    {
      const auto* v1 = reinterpret_cast<const std::vector<cl_int>*>(&v);
      const std::span<const cl_int> l{v1->begin(), v1->end()};
      ztest::setDeviceBuffer(*device, l, buffer_in1.get());
    }
    {
      const std::span<const cl_uint> l{v.begin(), v.end()};
      ztest::setDeviceBuffer(*device, l, buffer_in2.get());
    }
  }

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int m = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_uint m = (std::numeric_limits<cl_uint>::min)();
    const std::initializer_list<cl_uint> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchMaxTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchMaxTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2,
                                          *buffer_out1, *buffer_out2,
                                          n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected1, mem[0]) << "atomic_fetch_max of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected2, mem[0]) << "atomic_fetch_max of int failed.";
  }
}

TEST(ClCppTest, AtomicFetchMaxLocalTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;

  constexpr std::size_t n = 2560 * 1440;

  //
  cl_int expected1 = (std::numeric_limits<cl_int>::min)();
  cl_uint expected2 = (std::numeric_limits<cl_uint>::min)();

  // Initialize inputs
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    std::vector<cl_uint> v{};
    v.resize(::nLoop() * n);
    std::mt19937 engine{123'456'789};
    for (std::size_t i = 0; i < v.size(); ++i) {
      const cl_uint u = engine();
      expected1 = (std::max)(expected1, zisc::bit_cast<cl_int>(u));
      expected2 = (std::max)(expected2, u);
      v[i] = u;
    }
    {
      const auto* v1 = reinterpret_cast<const std::vector<cl_int>*>(&v);
      const std::span<const cl_int> l{v1->begin(), v1->end()};
      ztest::setDeviceBuffer(*device, l, buffer_in1.get());
    }
    {
      const std::span<const cl_uint> l{v.begin(), v.end()};
      ztest::setDeviceBuffer(*device, l, buffer_in2.get());
    }
  }

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int m = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_uint m = (std::numeric_limits<cl_uint>::min)();
    const std::initializer_list<cl_uint> v = {m};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicFetchMaxLocalTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicFetchMaxLocalTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2,
                                          *buffer_out1, *buffer_out2,
                                          n, ::nLoop(), launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected1, mem[0]) << "atomic_fetch_max of int failed.";
  }
  // output2
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_EQ(expected2, mem[0]) << "atomic_fetch_max of int failed.";
  }
}

TEST(ClCppTest, AtomicCompareExchangeTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  constexpr std::size_t n = 2560 * 1440;
  constexpr float scale = 0.9999999f;
  static_assert(scale < 1.0f);

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  ztest::setDeviceBuffer(*device, {0}, buffer_out1.get());

  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  ztest::setDeviceBuffer(*device, {1.0f}, buffer_out2.get());
  zivc::ReinterpBuffer reinterp_out2 = buffer_out2->reinterp<cl_uint>();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_atomic, atomicCompareAndExchangeTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("atomicCompareAndExchangeTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, reinterp_out2, n, ::nLoop(), scale, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    constexpr zivc::int32b expected = ::nLoop() * n;
    ASSERT_EQ(expected, mem[0]) << "atomic_cmpxchg of int failed.";
  }
  // output2
  {
    const auto get_expected = []() noexcept
    {
      float e = 1.0f;
      for (std::size_t i = 0; i < (::nLoop() * n); ++i)
        e *= scale;
      return e;
    };
    const float expected = get_expected();
    ASSERT_NE(0.0f, expected) << "The expected value is zero.";

    const zivc::SharedBuffer tmp = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    ASSERT_FLOAT_EQ(expected, mem[0]) << "atomic_cmpxchg of float failed.";
  }
}
