/*!
  \file kernel_test2.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include "zivc/kernel_set/kernel_set-kernel_test2.hpp"

namespace {

constexpr zivc::uint32b getExpectedValueForKernelMultipleInvocations() noexcept
{
  zivc::uint32b expected = 3 * 10 * 1024;
  expected *= 5;
  for (std::size_t i = 0; i < 3; ++i) {
    expected /= 2;
    expected += 3 * 1024;
  }
  return expected;
}

} // namespace 

TEST(KernelTest, KernelMultipleInvocationsTest)
{
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Init buffers
  {
    auto options = buff_device->createOptions();
    options.requestFence(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  auto kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel1->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel1->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel2->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel2->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel3->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel3->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel4->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation2Kernel");
    auto result = kernel4->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel5->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel5->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel6->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel6->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel7->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel7->run(*buff_device, n, launch_options);
  }
  device->waitForCompletion();

  // Check the outputs
  {
    auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device->size());
    {
      auto options = buff_device->createOptions();
      options.requestFence(true);
      auto result = zivc::copy(*buff_device, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Multiple kernel invocations failed.";
    }
  }
}

TEST(KernelTest, KernelBufferChangeTest)
{
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device1 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device1->setSize(n);
  auto buff_device2 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device2->setSize(n);
  auto buff_device3 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device3->setSize(n);

  // Init buffers
  {
    auto options = buff_device1->createOptions();
    options.requestFence(true);
    auto result = buff_device1->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_device2->createOptions();
    options.requestFence(true);
    auto result = buff_device2->fill(0, options);
    device->waitForCompletion(result.fence());
  }
  {
    auto options = buff_device3->createOptions();
    options.requestFence(true);
    auto result = buff_device3->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device1, n, launch_options);
    device->waitForCompletion();
  }
  {
    auto launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device2, n, launch_options);
    device->waitForCompletion();
  }
  {
    auto launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device3, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  constexpr uint32b expected = 10 * 1024;
  {
    auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());
    {
      auto options = buff_device1->createOptions();
      options.requestFence(true);
      auto result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
  {
    auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device2->size());
    {
      auto options = buff_device2->createOptions();
      options.requestFence(true);
      auto result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
  {
    auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device3->size());
    {
      auto options = buff_device3->createOptions();
      options.requestFence(true);
      auto result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
}

TEST(KernelTest, KernelQueueTest)
{
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Make a kernels
  auto kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  for (uint32b queue_index = 0; queue_index < 16; ++queue_index) {
    // Init buffers
    {
      auto options = buff_device->createOptions();
      options.requestFence(true);
      auto result = buff_device->fill(0, options);
      device->waitForCompletion(result.fence());
    }

    // Launch the kernels
    {
      auto launch_options = kernel1->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel1->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel2->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel2->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel3->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel3->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel4->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation2Kernel");
      auto result = kernel4->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel5->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel5->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel6->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel6->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel7->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel7->run(*buff_device, n, launch_options);
      device->waitForCompletion(queue_index);
    }

    // Check the outputs
    {
      auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                      zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device->size());
      {
        auto options = buff_device->createOptions();
        options.requestFence(true);
        auto result = zivc::copy(*buff_device, buff_host.get(), options);
        device->waitForCompletion(result.fence());
      }
      {
        constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
        auto mem = buff_host->mapMemory();
        for (std::size_t i = 0; i < mem.size(); ++i)
          ASSERT_EQ(expected, mem[i]) << "Queue[" << queue_index << "] failed.";
      }
    }
  }
}

TEST(KernelTest, KernelFenceTest)
{
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Init buffers
  {
    auto options = buff_device->createOptions();
    options.requestFence(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  device->setFenceSize(7);

  // Make a kernels
  auto kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel1->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(0);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel1->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel2->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(1);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel2->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel3->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(2);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel3->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel4->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(3);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation2Kernel");
    auto result = kernel4->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel5->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(4);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel5->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel6->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(5);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel6->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel7->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(6);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel7->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }

  // Check the outputs
  {
    auto buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device->size());
    {
      auto options = buff_device->createOptions();
      options.requestFence(true);
      auto result = zivc::copy(*buff_device, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
      auto mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Multiple kernel invocations failed.";
    }
  }
}

