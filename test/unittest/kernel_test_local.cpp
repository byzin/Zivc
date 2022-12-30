/*!
  \file kernel_test_local.cpp
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
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test_local.hpp"

namespace {

} // namespace 

TEST(KernelTest, Local1Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(2);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(2);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, local1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint32b expected = 0b01010101'10101010'11110000'00001111u;
  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(true);
    launch_options.setLabel("Local1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, expected, launch_options);
    device->waitForCompletion(result.fence());
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    EXPECT_EQ(expected, mem[0]) << "Using local value failed.";
    EXPECT_EQ(1, mem[1]) << "Setting work dimension failed.";
  }
}

//! \todo Remove the commentout. Enable the test
//TEST(KernelTest, Local2Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();
//
//  using zivc::int32b;
//  using zivc::uint32b;
//
//  // Allocate buffers
//  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
//  buff_device->setSize(2);
//  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  buff_host->setSize(2);
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, local2Kernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";
//
//  constexpr int32b value1 = 10;
//  constexpr float value2 = 5.0f;
//  constexpr uint32b value3 = 20;
//  constexpr uint32b expected = static_cast<uint32b>(value1) +
//                               static_cast<uint32b>(value2) +
//                               value3;
//  // Launch the kernel
//  {
//    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//    launch_options.setWorkSize({{1}});
//    launch_options.requestFence(true);
//    launch_options.setLabel("Local2Kernel");
//    const zivc::LaunchResult result = kernel->run(*buff_device, value1, value2, value3, launch_options);
//    device->waitForCompletion(result.fence());
//  }
//
//  // Check the outputs
//  {
//    zivc::BufferLaunchOptions options = buff_device->createOptions();
//    options.requestFence(true);
//    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
//    device->waitForCompletion(result.fence());
//
//    const zivc::MappedMemory mem = buff_host->mapMemory();
//    EXPECT_EQ(expected, mem[0]) << "Using local value failed.";
//    EXPECT_EQ(1, mem[1]) << "Setting work dimension failed.";
//  }
//}

TEST(KernelTest, LocalSumTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t resolution = 2560 * 1440;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(resolution);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(buff_device->size());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, localSumKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  constexpr uint32b offset = 10;
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{resolution}});
    launch_options.requestFence(true);
    launch_options.setLabel("LocalSumKernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, offset, resolution, launch_options);
    device->waitForCompletion(result.fence());
  }
  std::cout << "  workgroup size: " << info.workGroupSize() << std::endl;
  const uint32b sum = info.workGroupSize() * (info.workGroupSize() + 1) / 2 + offset;
  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      ASSERT_EQ(sum, mem[i]) << "Summation of local values at " << i << " failed.";
    }
  }
}

TEST(KernelTest, LocalStruct1Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t resolution = 2560 * 1440;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(resolution);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(buff_device->size());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, localStruct1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{resolution}});
    launch_options.requestFence(true);
    launch_options.setLabel("LocalStruct1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, resolution, launch_options);
    device->waitForCompletion(result.fence());
  }
  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const uint32b expected = 4 * i + 6;
      ASSERT_EQ(expected, mem[i]) << "Using local struct at " << i << " failed.";
    }
  }
}

TEST(KernelTest, LocalStruct2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t resolution = 2560 * 1440;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(resolution);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(buff_device->size());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, localStruct2Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{resolution}});
    launch_options.requestFence(true);
    launch_options.setLabel("LocalStruct2Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, resolution, launch_options);
    device->waitForCompletion(result.fence());
  }
  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const uint32b expected = 4 * i + 6;
      ASSERT_EQ(expected, mem[i]) << "Using local struct at " << i << " failed.";
    }
  }
}

//! \todo Remove the commentout. Enable the test
//TEST(KernelTest, LocalStruct2CopyTest)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();
//
//  using zivc::int32b;
//  using zivc::uint32b;
//
//  constexpr std::size_t resolution = 0b01 << 15;
//
//  // Allocate buffers
//  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
//  buff_device->setSize(resolution);
//  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  buff_host->setSize(buff_device->size());
//
//  using StorageT = zivc::cl::inner::Storage2;
//  const zivc::SharedBuffer buff_inputs = device->createBuffer<StorageT>(zivc::BufferUsage::kPreferDevice);
//  buff_inputs->setSize(resolution);
//  {
//    const zivc::SharedBuffer inputs_host = device->createBuffer<StorageT>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    inputs_host->setSize(buff_inputs->size());
//    {
//      zivc::MappedMemory mem = inputs_host->mapMemory();
//      for (std::size_t i = 0; i < mem.size(); ++i)
//        mem[i].set(i);
//    }
//    zivc::BufferLaunchOptions options = inputs_host->createOptions();
//    options.requestFence(true);
//    const zivc::LaunchResult result = zivc::copy(*inputs_host, buff_inputs.get(), options);
//    device->waitForCompletion(result.fence());
//  }
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, localStruct2CopyKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  {
//    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//    launch_options.setWorkSize({{resolution}});
//    launch_options.requestFence(true);
//    launch_options.setLabel("LocalStruct2CopyKernel");
//    const zivc::LaunchResult result = kernel->run(*buff_inputs, *buff_device, resolution, launch_options);
//    device->waitForCompletion(result.fence());
//  }
//  // Check the outputs
//  {
//    zivc::BufferLaunchOptions options = buff_device->createOptions();
//    options.requestFence(true);
//    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
//    device->waitForCompletion(result.fence());
//
//    const zivc::MappedMemory mem = buff_host->mapMemory();
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const uint32b s = info.workGroupSize();
//      const uint32b index = (i / s) * s + (i + 1) % s;
//      const uint32b expected = 4 * index + 6;
//      ASSERT_EQ(expected, mem[i]) << "Using local struct at " << i << " failed.";
//    }
//  }
//}

TEST(KernelTest, LocalStruct3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t resolution = 2560 * 1440;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(resolution);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(buff_device->size());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_local, localStruct3Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{resolution}});
    launch_options.requestFence(true);
    launch_options.setLabel("LocalStruct3Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, resolution, launch_options);
    device->waitForCompletion(result.fence());
  }
  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const uint32b expected = 4 * i + 6;
      ASSERT_EQ(expected, mem[i]) << "Using local struct at " << i << " failed.";
    }
  }
}
