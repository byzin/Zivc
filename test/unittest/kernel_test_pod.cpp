/*!
  \file kernel_test_pod.cpp
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
#include <functional>
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
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test_pod.hpp"

namespace {

//! Check the value of scalar buffer
template <typename Type>
testing::AssertionResult testVectorBuffer(zivc::Device& device,
                                          const zivc::Buffer<Type>& buffer)
{
  const zivc::SharedBuffer buff_host = device.createBuffer<Type>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(buffer.size());
  {
    zivc::BufferLaunchOptions options = buffer.createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(buffer, buff_host.get(), options);
    device.waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory mem = buff_host->mapMemory();
    auto* ptr = std::addressof(mem[0].x);
    using ScalarT = std::remove_cvref_t<decltype(ptr[0])>;
    auto expected = static_cast<ScalarT>(1);
    const std::size_t n =  mem.size() * mem[0].size();
    for (std::size_t i = 0; i < n; ++i, ++expected) {
      if (i == n / 2)
        expected = static_cast<ScalarT>(1);
      if (std::not_equal_to<ScalarT>{}(expected, ptr[i]))
        return testing::AssertionFailure() << "mem[" << i << "] = " << ptr[i];
    }
    return testing::AssertionSuccess();
  }
}

} // namespace 

TEST(KernelTest, Pod1Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_device = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device->setSize(1);
  const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(1);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, pod1Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  constexpr uint32b expected = 0b01010101'10101010'11110000'00001111u;
  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("Pod1Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device, expected, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
    EXPECT_EQ(expected, mem[0]) << "Copying POD value failed.";
  }
}

TEST(KernelTest, Pod2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  const std::size_t n = config.testKernelWorkSize1d();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(n);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(n);
  const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(n);

  // Init buffers
  {
    {
      zivc::MappedMemory mem = buff_host->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    zivc::BufferLaunchOptions options = buff_device1->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_host, buff_device1.get(), options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, pod2Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    const uint32b res = zisc::cast<uint32b>(n);
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{res}});
    launch_options.requestFence(false);
    launch_options.setLabel("Pod2Kernel");
    const zivc::LaunchResult result = kernel->run(*buff_device1, *buff_device2, res, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    zivc::BufferLaunchOptions options = buff_device2->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    const zivc::MappedMemory mem = buff_host->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_device_i = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device_i->setSize(3);
  const zivc::SharedBuffer buff_device_u = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device_u->setSize(3);
  const zivc::SharedBuffer buff_device_f = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device_f->setSize(3);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, pod3Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
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
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("Pod3Kernel");
    const zivc::LaunchResult result = kernel->run(
        i0, u0, f0,
        i1, u1, f1,
        i2, u2, f2,
        *buff_device_i, *buff_device_u, *buff_device_f, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(3);

    zivc::BufferLaunchOptions options = buff_device_i->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device_i, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      EXPECT_EQ(i0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(i1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(i2, mem[2]) << "POD copying failed.";
    }
  }

  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(3);

    zivc::BufferLaunchOptions options = buff_device_u->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device_u, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      EXPECT_EQ(u0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(u1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(u2, mem[2]) << "POD copying failed.";
    }
  }

  {
    const zivc::SharedBuffer buff_host = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(3);

    zivc::BufferLaunchOptions options = buff_device_f->createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_device_f, buff_host.get(), options);
    device->waitForCompletion(result.fence());

    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      EXPECT_EQ(f0, mem[0]) << "POD copying failed.";
      EXPECT_EQ(f1, mem[1]) << "POD copying failed.";
      EXPECT_EQ(f2, mem[2]) << "POD copying failed.";
    }
  }
}

TEST(KernelTest, PodSizeAlignmentTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(15);
  {
    const zivc::BufferLaunchOptions options = buff_device1->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = buff_device1->fill(0, options);
    device->waitForCompletion();
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, podSizeAlignmentKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(16, kernel->argSize()) << "Wrong kernel property.";

  constexpr int16b i16 = 1;
  constexpr int8b i8_1 = 2;
  constexpr int8b i8_2 = 3;
  constexpr uint32b u32 = 4;
  constexpr uint8b u8_1 = 5;
  constexpr uint8b u8_2 = 6;
  constexpr uint8b u8_3 = 7;
  constexpr uint8b u8_4 = 8;
  constexpr uint32b u32_2 = 9;
  constexpr int16b i16_2 = 10;
  constexpr int8b i8_3 = 11;
  constexpr int8b i8_4 = 12;
  constexpr int8b i8_5 = 13;
  constexpr int8b i8_6 = 14;
  constexpr int16b i16_3 = 15;

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("PodSizeAlignmentKernel");
    const zivc::LaunchResult result = kernel->run(*buff_device1, i16, i8_1, i8_2,
                              u32, u8_1, u8_2, u8_3, u8_4,
                              u32_2, i16_2, i8_3, i8_4, i8_5, i8_6, i16_3,
                              launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());
    const zivc::BufferLaunchOptions options = buff_device1->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i) {
        const int32b expected = zisc::cast<int32b>(i + 1);
        ASSERT_EQ(expected, mem[i]) << "POD value isn't processed properly.";
      }
    }
  }
}

TEST(KernelTest, PodSizeAlignment2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using PodTest = zivc::cl::inner::PodAlignmentTest;

  auto init_buffer = [](zivc::Device& d, auto& buffer, const auto value)
  {
    using Type = std::add_const_t<decltype(value)>;
    const zivc::SharedBuffer buff_host = d.createBuffer<Type>({zivc::BufferUsage::kPreferHost,
                                           zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buffer.size());
    {
      zivc::MappedMemory mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), value);
    }
    const zivc::BufferLaunchOptions options = buff_host->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_host, std::addressof(buffer), options);
    d.waitForCompletion();
  };

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(2);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(2);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device3->setSize(2);
  const zivc::SharedBuffer buff_device4 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device4->setSize(2);
  const zivc::SharedBuffer buff_device5 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device5->setSize(2);
  const zivc::SharedBuffer buff_device6 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device6->setSize(2);
  const zivc::SharedBuffer buff_device7 = device->createBuffer<PodTest>(zivc::BufferUsage::kPreferDevice);
  buff_device7->setSize(2);

  init_buffer(*device, *buff_device1, zisc::cast<uint8b>(0));
  init_buffer(*device, *buff_device2, zisc::cast<float>(0));
  init_buffer(*device, *buff_device3, zisc::cast<int8b>(0));
  init_buffer(*device, *buff_device4, zisc::cast<int16b>(0));
  init_buffer(*device, *buff_device5, zisc::cast<uint16b>(0));
  init_buffer(*device, *buff_device6, zisc::cast<int32b>(0));
  init_buffer(*device, *buff_device7, PodTest{});

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, podSizeAlignment2Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
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
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("PodSizeAlignment2Kernel");
    const zivc::LaunchResult result = kernel->run(
        *buff_device1, *buff_device2, *buff_device3, *buff_device4,
        *buff_device5, *buff_device6, *buff_device7,
        u8, f32, i8, i16, u16, i32, test, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device1->size());
    const zivc::BufferLaunchOptions options = buff_device1->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device2->size());
    const zivc::BufferLaunchOptions options = buff_device2->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<int8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device3->size());
    const zivc::BufferLaunchOptions options = buff_device3->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<int16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device4->size());
    const zivc::BufferLaunchOptions options = buff_device4->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device4, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<uint16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device5->size());
    const zivc::BufferLaunchOptions options = buff_device5->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device5, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device6->size());
    const zivc::BufferLaunchOptions options = buff_device6->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device6, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
      ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
    }
  }
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<PodTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device7->size());
    const zivc::BufferLaunchOptions options = buff_device7->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device7, buff_host.get(), options);
    device->waitForCompletion();
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
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
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using PodTest = zivc::cl::inner::PodAlignmentTest;

  auto init_buffer = [](zivc::Device& d, auto& buffer, const auto value)
  {
    using Type = std::add_const_t<decltype(value)>;
    const zivc::SharedBuffer buff_host = d.createBuffer<Type>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buffer.size());
    {
      zivc::MappedMemory mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), value);
    }
    const zivc::BufferLaunchOptions options = buff_host->createOptions();
    [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_host, std::addressof(buffer), options);
    d.waitForCompletion();
  };

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(2);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(2);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device3->setSize(2);
  const zivc::SharedBuffer buff_device4 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device4->setSize(2);
  const zivc::SharedBuffer buff_device5 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device5->setSize(2);
  const zivc::SharedBuffer buff_device6 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device6->setSize(2);
  const zivc::SharedBuffer buff_device7 = device->createBuffer<PodTest>(zivc::BufferUsage::kPreferDevice);
  buff_device7->setSize(2);

  init_buffer(*device, *buff_device1, zisc::cast<uint8b>(0));
  init_buffer(*device, *buff_device2, zisc::cast<float>(0));
  init_buffer(*device, *buff_device3, zisc::cast<int8b>(0));
  init_buffer(*device, *buff_device4, zisc::cast<int16b>(0));
  init_buffer(*device, *buff_device5, zisc::cast<uint16b>(0));
  init_buffer(*device, *buff_device6, zisc::cast<int32b>(0));
  init_buffer(*device, *buff_device7, PodTest{});

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, podSizeAlignment2Kernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
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
      zivc::KernelLaunchOptions launch_options = kernel->createOptions();
      launch_options.setWorkSize({{1}});
      launch_options.requestFence(false);
      launch_options.setLabel("PodSizeAlignment2Kernel");
      const zivc::LaunchResult result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device1->size());
      const zivc::BufferLaunchOptions options = buff_device1->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device2->size());
      const zivc::BufferLaunchOptions options = buff_device2->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device3->size());
      const zivc::BufferLaunchOptions options = buff_device3->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device4->size());
      const zivc::BufferLaunchOptions options = buff_device4->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device5->size());
      const zivc::BufferLaunchOptions options = buff_device5->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device6->size());
      const zivc::BufferLaunchOptions options = buff_device6->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<PodTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device7->size());
      const zivc::BufferLaunchOptions options = buff_device7->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
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
      zivc::KernelLaunchOptions launch_options = kernel->createOptions();
      launch_options.setWorkSize({{1}});
      launch_options.requestFence(false);
      launch_options.setLabel("PodSizeAlignmentKernel");
      const zivc::LaunchResult result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device1->size());
      const zivc::BufferLaunchOptions options = buff_device1->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device2->size());
      const zivc::BufferLaunchOptions options = buff_device2->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device3->size());
      const zivc::BufferLaunchOptions options = buff_device3->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device4->size());
      const zivc::BufferLaunchOptions options = buff_device4->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device5->size());
      const zivc::BufferLaunchOptions options = buff_device5->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device6->size());
      const zivc::BufferLaunchOptions options = buff_device6->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<PodTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device7->size());
      const zivc::BufferLaunchOptions options = buff_device7->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
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
      zivc::KernelLaunchOptions launch_options = kernel->createOptions();
      launch_options.setWorkSize({{1}});
      launch_options.requestFence(false);
      launch_options.setLabel("PodSizeAlignmentKernel");
      const zivc::LaunchResult result = kernel->run(
          *buff_device1, *buff_device2, *buff_device3, *buff_device4,
          *buff_device5, *buff_device6, *buff_device7,
          u8, f32, i8, i16, u16, i32, test, launch_options);
      device->waitForCompletion();
    }

    // Check the outputs
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device1->size());
      const zivc::BufferLaunchOptions options = buff_device1->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device1, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device2->size());
      const zivc::BufferLaunchOptions options = buff_device2->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device2, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(f32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(f32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int8b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device3->size());
      const zivc::BufferLaunchOptions options = buff_device3->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device3, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i8, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i8, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device4->size());
      const zivc::BufferLaunchOptions options = buff_device4->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device4, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<uint16b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device5->size());
      const zivc::BufferLaunchOptions options = buff_device5->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device5, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(u16, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(u16, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device6->size());
      const zivc::BufferLaunchOptions options = buff_device6->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device6, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
        ASSERT_EQ(i32, mem[0]) << "POD value isn't processed properly.";
        ASSERT_EQ(i32, mem[1]) << "POD structure isn't processed properly.";
      }
    }
    {
      const zivc::SharedBuffer buff_host = device->createBuffer<PodTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      buff_host->setSize(buff_device7->size());
      const zivc::BufferLaunchOptions options = buff_device7->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_device7, buff_host.get(), options);
      device->waitForCompletion();
      {
        const zivc::MappedMemory mem = buff_host->mapMemory();
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

TEST(KernelTest, PodVectorTypeTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  [[maybe_unused]] const zivc::DeviceInfo& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using zivc::cl_uchar2;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short4;
  using zivc::cl_int2;
  using zivc::cl_int4;
  using zivc::cl_float2;
  using zivc::cl_float4;
  using PodTest = zivc::cl::inner::PodVectorTest;

  auto init_buffer = [](zivc::Device& d, auto& buffer, const auto value)
  {
    using Type = std::add_const_t<decltype(value)>;
    const zivc::SharedBuffer buff_host = d.createBuffer<Type>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buffer.size());
    {
      zivc::MappedMemory mem = buff_host->mapMemory();
      std::fill(mem.begin(), mem.end(), value);
    }
    {
      const zivc::BufferLaunchOptions options = buff_host->createOptions();
      [[maybe_unused]] const zivc::LaunchResult result = zivc::copy(*buff_host, std::addressof(buffer), options);
      d.waitForCompletion();
    }
  };

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<cl_uchar2>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(4);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<cl_uchar4>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(4);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<cl_short2>(zivc::BufferUsage::kPreferDevice);
  buff_device3->setSize(4);
  const zivc::SharedBuffer buff_device4 = device->createBuffer<cl_short4>(zivc::BufferUsage::kPreferDevice);
  buff_device4->setSize(4);
  const zivc::SharedBuffer buff_device5 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buff_device5->setSize(4);
  const zivc::SharedBuffer buff_device6 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buff_device6->setSize(4);
  const zivc::SharedBuffer buff_device7 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buff_device7->setSize(4);
  const zivc::SharedBuffer buff_device8 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buff_device8->setSize(4);
  const zivc::SharedBuffer buff_device9 = device->createBuffer<PodTest>(zivc::BufferUsage::kPreferDevice);
  buff_device9->setSize(2);

  init_buffer(*device, *buff_device1, cl_uchar2{0, 0});
  init_buffer(*device, *buff_device2, cl_uchar4{0, 0, 0, 0});
  init_buffer(*device, *buff_device3, cl_short2{0, 0});
  init_buffer(*device, *buff_device4, cl_short4{0, 0, 0, 0});
  init_buffer(*device, *buff_device5, cl_int2{0, 0});
  init_buffer(*device, *buff_device6, cl_int4{0, 0, 0, 0});
  init_buffer(*device, *buff_device7, cl_float2{0.0f, 0.0f});
  init_buffer(*device, *buff_device8, cl_float4{0.0f, 0.0f, 0.0f, 0.0f});

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_pod, podVectorKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(26, kernel->argSize()) << "Wrong kernel property.";

  const cl_uchar2 u8v2_1{1, 2};
  const cl_uchar2 u8v2_2{3, 4};
  const cl_uchar4 u8v4_1{1, 2, 3, 4};
  const cl_uchar4 u8v4_2{5, 6, 7, 8};
  const cl_short2 i16v2_1{1, 2};
  const cl_short2 i16v2_2{3, 4};
  const cl_short4 i16v4_1{1, 2, 3, 4};
  const cl_short4 i16v4_2{5, 6, 7, 8};
  const cl_int2 i32v2_1{1, 2};
  const cl_int2 i32v2_2{3, 4};
  const cl_int4 i32v4_1{1, 2, 3, 4};
  const cl_int4 i32v4_2{5, 6, 7, 8};
  const cl_float2 f32v2_1{1.0f, 2.0f};
  const cl_float2 f32v2_2{3.0f, 4.0f};
  const cl_float4 f32v4_1{1.0f, 2.0f, 3.0f, 4.0f};
  const cl_float4 f32v4_2{5.0f, 6.0f, 7.0f, 8.0f};
  const PodTest podtest{u8v2_1, u8v2_2, u8v4_1, u8v4_2,
                        i16v2_1, i16v2_2, i16v2_1, i16v4_1, i16v4_2,
                        i32v2_1, i32v2_2, i32v2_1, i32v4_1, i32v4_2,
                        f32v2_1, f32v2_2, f32v4_1, f32v4_2};

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("PodVectorKernel");
    const zivc::LaunchResult result = kernel->run(
        *buff_device1, *buff_device2, *buff_device3, *buff_device4,
        *buff_device5, *buff_device6, *buff_device7, *buff_device8, *buff_device9,
        u8v2_1, u8v2_2, u8v4_1, u8v4_2,
        i16v2_1, i16v2_2, i16v4_1, i16v4_2,
        i32v2_1, i32v2_2, i32v4_1, i32v4_2,
        f32v2_1, f32v2_2, f32v4_1, f32v4_2,
        podtest, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device1));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device2));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device3));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device4));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device5));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device6));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device7));
  ASSERT_TRUE(::testVectorBuffer(*device, *buff_device8));
  {
    const zivc::SharedBuffer buff_host = device->createBuffer<PodTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(buff_device9->size());
    {
      zivc::BufferLaunchOptions options = buff_device9->createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(*buff_device9, buff_host.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      zivc::MappedMemory mem = buff_host->mapMemory();

      const auto test_vec = [](const auto ptr, const std::size_t n) noexcept
      {
        using ScalarT = std::remove_cvref_t<decltype(ptr[0])>;
        ScalarT expected = static_cast<ScalarT>(1);
        for (std::size_t i = 0; i < n; ++i, ++expected) {
          if (std::not_equal_to<ScalarT>{}(expected, ptr[i]))
            return testing::AssertionFailure() << "mem[" << i << "] = " << ptr[i];
        }
        return testing::AssertionSuccess();
      };
      ASSERT_TRUE(test_vec(std::addressof(mem[0].u8v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].u8v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].i16v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].i16v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].i32v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].i32v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].f32v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[0].f32v4_1_.x), 8));

      ASSERT_TRUE(test_vec(std::addressof(mem[1].u8v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].u8v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].i16v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].i16v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].i32v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].i32v4_1_.x), 8));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].f32v2_1_.x), 4));
      ASSERT_TRUE(test_vec(std::addressof(mem[1].f32v4_1_.x), 8));
    }
  }
}
