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

TEST(KernelTest, SharedKernelTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Creating a shared kernel
  const zivc::KernelInitParams params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2,
                                                                       invocation1Kernel,
                                                                       1);
  const zivc::SharedKernel kernel1 = device->createKernel(params);
  ASSERT_TRUE(static_cast<bool>(kernel1)) << "SharedKernel initialization failed.";
  ASSERT_EQ(1, kernel1->dimension()) << "SharedKernel initialization failed.";
  ASSERT_EQ(1, (*kernel1).dimension()) << "SharedKernel initialization failed.";
  ASSERT_EQ(1, kernel1.useCount()) << "SharedKernel initialization failed.";

  // Casting to common type
  zivc::SharedKernelCommon common = kernel1;

  // Casting to the derived kernel type
  using SharedKernelT = std::remove_cvref_t<decltype(kernel1)>;
  const SharedKernelT kernel2{common};
  ASSERT_EQ(1, kernel2->dimension()) << "SharedKernel initialization failed.";
  ASSERT_EQ(1, (*kernel2).dimension()) << "SharedKernel initialization failed.";
  ASSERT_EQ(3, kernel2.useCount()) << "SharedKernel initialization failed.";

  //! \todo Resolve the move ownership
  SharedKernelT kernel3{std::move(common)};
  ASSERT_EQ(1, kernel3->dimension()) << "SharedKernel initialization failed.";
  ASSERT_EQ(1, (*kernel3).dimension()) << "SharedKernel initialization failed.";
#if defined(Z_CLANG)
  constexpr std::size_t count_offset = 1;
#else // Z_CLANG
  constexpr std::size_t count_offset = 0;
#endif // Z_CLANG
  ASSERT_EQ(3 + count_offset, kernel3.useCount()) << "SharedKernel initialization failed.";

  kernel3.reset();
  ASSERT_EQ(2 + count_offset, kernel1.useCount()) << "SharedKernel initialization failed.";
}

TEST(KernelTest, KernelMultipleInvocationsTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  const zivc::KernelInitParams kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  const zivc::SharedKernel kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    zivc::KernelLaunchOptions launch_options = kernel1->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel1->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel2->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel2->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel3->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel3->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel4->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation2Kernel");
    const zivc::LaunchResult result = kernel4->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel5->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel5->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel6->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel6->run(*buff_device, n, launch_options);
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel7->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel7->run(*buff_device, n, launch_options);
  }
  device->waitForCompletion();

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device->size());
    {
      zivc::BufferLaunchOptions options = buff_device->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Multiple kernel invocations failed.";
    }
  }
}

TEST(KernelTest, KernelBufferChangeTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device1->setSize(n);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device2->setSize(n);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device3->setSize(n);

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

  // Make a kernels
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device1, n, launch_options);
    device->waitForCompletion();
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device2, n, launch_options);
    device->waitForCompletion();
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(false);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device3, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  constexpr uint32b expected = 10 * 1024;
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());
    {
      zivc::BufferLaunchOptions options = buff_device1->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device2->size());
    {
      zivc::BufferLaunchOptions options = buff_device2->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device3->size());
    {
      zivc::BufferLaunchOptions options = buff_device3->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Changing buffer failed.";
    }
  }
}

TEST(KernelTest, KernelQueueTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Make a kernels
  const zivc::KernelInitParams kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  const zivc::SharedKernel kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  for (uint32b queue_index = 0; queue_index < 16; ++queue_index) {
    // Init buffers
    {
      zivc::BufferLaunchOptions options = buff_device->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = buff_device->fill(0, options);
      device->waitForCompletion(result.fence());
    }

    // Launch the kernels
    {
      zivc::KernelLaunchOptions launch_options = kernel1->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      const zivc::LaunchResult result = kernel1->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel2->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      const zivc::LaunchResult result = kernel2->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel3->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation1Kernel");
      const zivc::LaunchResult result = kernel3->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel4->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation2Kernel");
      const zivc::LaunchResult result = kernel4->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel5->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      const zivc::LaunchResult result = kernel5->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel6->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      const zivc::LaunchResult result = kernel6->run(*buff_device, n, launch_options);
    }
    {
      zivc::KernelLaunchOptions launch_options = kernel7->createOptions();
      launch_options.setWorkSize({{n}});
      launch_options.setQueueIndex(queue_index);
      launch_options.requestFence(false);
      launch_options.setLabel("invocation3Kernel");
      const zivc::LaunchResult result = kernel7->run(*buff_device, n, launch_options);
      device->waitForCompletion(queue_index);
    }

    // Check the outputs
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device->size());
      {
        zivc::BufferLaunchOptions options = buff_device->createOptions();
        options.requestFence(true);
        const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
        device->waitForCompletion(result.fence());
      }
      {
        constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
        const zivc::MappedMemory mem = buff_host->mapMemory();
        for (std::size_t i = 0; i < mem.size(); ++i)
          ASSERT_EQ(expected, mem[i]) << "Queue[" << queue_index << "] failed.";
      }
    }
  }
}

TEST(KernelTest, KernelFenceTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferDevice});
  buff_device->setSize(n);

  // Init buffers
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  device->setFenceSize(7);

  // Make a kernels
  const zivc::KernelInitParams kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel1 = device->createKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params3 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  const zivc::SharedKernel kernel3 = device->createKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params4 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  const zivc::SharedKernel kernel4 = device->createKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params5 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel5 = device->createKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params6 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel6 = device->createKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  const zivc::KernelInitParams kernel_params7 = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  const zivc::SharedKernel kernel7 = device->createKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    zivc::KernelLaunchOptions launch_options = kernel1->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(0);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel1->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel2->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(1);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel2->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel3->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(2);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation1Kernel");
    const zivc::LaunchResult result = kernel3->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel4->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(3);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation2Kernel");
    const zivc::LaunchResult result = kernel4->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel5->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(4);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel5->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel6->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(5);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel6->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::KernelLaunchOptions launch_options = kernel7->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.setQueueIndex(6);
    launch_options.requestFence(true);
    launch_options.setLabel("invocation3Kernel");
    const zivc::LaunchResult result = kernel7->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device->size());
    {
      zivc::BufferLaunchOptions options = buff_device->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      constexpr uint32b expected = ::getExpectedValueForKernelMultipleInvocations();
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        ASSERT_EQ(expected, mem[i]) << "Multiple kernel invocations failed.";
    }
  }
}

TEST(KernelTest, KernelReinterpBufferTest)
{
  using zivc::int32b;
  using zivc::uint32b;
  using zivc::uint64b;

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  constexpr std::size_t w = 2560;
  constexpr std::size_t h = 1440;

  // Allocate buffers
  const zivc::SharedBuffer buff_h = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buff_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = w * h;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buff_h->setSize(s);
    buff_d->setSize(s);
  }

  // Initialize buffer
  {
    zivc::MappedMemory<float> mem = buff_h->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i)
      mem[i] = zisc::cast<float>(i + 1);
  }
  zivc::ReinterpBuffer reinterp_d = buff_d->reinterp<float>();
  {
    zivc::BufferLaunchOptions options = buff_h->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_h, &reinterp_d, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test2, reinterpTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    const auto n = zisc::cast<uint32b>(reinterp_d.size());
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{n}});
    launch_options.requestFence(true);
    launch_options.setLabel("reinterpTestKernel");
    const zivc::LaunchResult result = kernel->run(reinterp_d, n, launch_options);
    device->waitForCompletion(result.fence());
  }

  {
    zivc::BufferLaunchOptions options = buff_h->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(reinterp_d, buff_h.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Check the result
  {
    const zivc::MappedMemory<float> mem = buff_h->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float expected = 3.0f * zisc::cast<float>(i + 1);
      ASSERT_FLOAT_EQ(expected, mem[i]) << "Reinterp kernel failed. mem[" << i << "].";
    }
  }
}
