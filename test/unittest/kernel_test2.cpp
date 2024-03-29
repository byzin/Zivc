/*!
  \file kernel_test2.cpp
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
#include "zisc/concepts.hpp"
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
#include "zivc/kernel_set/kernel_set-kernel_test2.hpp"

namespace {

//! Initialize a scalar buffer
template <zisc::Scalar Scalar>
void initScalarBuffer(const zivc::Device& device, zivc::Buffer<Scalar>& buffer)
{
  auto options = buffer.makeOptions();
  options.setExternalSyncMode(true);
  //! \todo remove me
  if constexpr (sizeof(Scalar) == 4) { 
    auto result = buffer.fill(zisc::cast<Scalar>(0), options);
    device.waitForCompletion(result.fence());
  }
}

//! Check the value of scalar buffer
template <zisc::Scalar Scalar>
testing::AssertionResult testScalarBuffer(zivc::Device& device,
                                          const zivc::Buffer<Scalar>& buffer)
{
  auto buff_host = device.makeBuffer<Scalar>(zivc::BufferUsage::kHostOnly);
  buff_host->setSize(1);
  {
    auto options = buffer.makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(buffer, buff_host.get(), options);
    device.waitForCompletion(result.fence());
  }
  {
    auto mem = buff_host->mapMemory();
    auto result = zisc::equal(mem[0], zisc::cast<Scalar>(1))
        ? testing::AssertionSuccess()
        : testing::AssertionFailure();
    return result;
  }
}

} // namespace 

TEST(KernelTest, LargeNumOfParametersTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int8b;
  using zivc::int16b;
  using zivc::int32b;
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint32b;
  using ParamTest = zivc::cl::kernel_test2::ParamTest;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(1);
  auto buff_device2 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(1);
  auto buff_device3 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device3->setSize(1);
  auto buff_device4 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device4->setSize(1);
  auto buff_device5 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device5->setSize(1);
  auto buff_device6 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device6->setSize(1);
  auto buff_device7 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device7->setSize(1);
  auto buff_device8 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device8->setSize(1);

  auto buff_device9 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device9->setSize(1);
  auto buff_device10 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device10->setSize(1);
  auto buff_device11 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device11->setSize(1);
  auto buff_device12 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device12->setSize(1);
  auto buff_device13 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device13->setSize(1);
  auto buff_device14 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device14->setSize(1);
  auto buff_device15 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device15->setSize(1);
  auto buff_device16 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device16->setSize(1);

#if !defined(ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
  auto buff_device17 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device17->setSize(1);
  auto buff_device18 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device18->setSize(1);
  auto buff_device19 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device19->setSize(1);
  auto buff_device20 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device20->setSize(1);
  auto buff_device21 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device21->setSize(1);
  auto buff_device22 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device22->setSize(1);
  auto buff_device23 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device23->setSize(1);
  auto buff_device24 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device24->setSize(1);
#endif // ZIVC_MSL_BUFFER_OUT_OF_BOUNDS

#if !defined(Z_MAC)
  auto buff_device25 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device25->setSize(1);
  auto buff_device26 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device26->setSize(1);
  auto buff_device27 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device27->setSize(1);
  auto buff_device28 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device28->setSize(1);
  auto buff_device29 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device29->setSize(1);
  auto buff_device30 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device30->setSize(1);
  auto buff_device31 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device31->setSize(1);
  auto buff_device32 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device32->setSize(1);

  auto buff_device33 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device33->setSize(1);
  auto buff_device34 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device34->setSize(1);
  auto buff_device35 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device35->setSize(1);
  auto buff_device36 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device36->setSize(1);
  auto buff_device37 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device37->setSize(1);
  auto buff_device38 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device38->setSize(1);
  auto buff_device39 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device39->setSize(1);
  auto buff_device40 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device40->setSize(1);

  auto buff_device41 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device41->setSize(1);
  auto buff_device42 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device42->setSize(1);
  auto buff_device43 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device43->setSize(1);
  auto buff_device44 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device44->setSize(1);
  auto buff_device45 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device45->setSize(1);
  auto buff_device46 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device46->setSize(1);
  auto buff_device47 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device47->setSize(1);
  auto buff_device48 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device48->setSize(1);

  auto buff_device49 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device49->setSize(1);
  auto buff_device50 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device50->setSize(1);
  auto buff_device51 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device51->setSize(1);
  auto buff_device52 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device52->setSize(1);
  auto buff_device53 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device53->setSize(1);
  auto buff_device54 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device54->setSize(1);
  auto buff_device55 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device55->setSize(1);
  auto buff_device56 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device56->setSize(1);

  auto buff_device57 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device57->setSize(1);
  auto buff_device58 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device58->setSize(1);
  auto buff_device59 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device59->setSize(1);
  auto buff_device60 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
  buff_device60->setSize(1);
  auto buff_device61 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
  buff_device61->setSize(1);
  auto buff_device62 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device62->setSize(1);
  auto buff_device63 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
  buff_device63->setSize(1);
  auto buff_device64 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
  buff_device64->setSize(1);
#endif // Z_MAC

  auto init_test_buffer = [](zivc::Device& d, zivc::Buffer<ParamTest>& buffer)
  {
    auto buff_host = d.makeBuffer<ParamTest>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(1);
    {
      auto mem = buff_host->mapMemory();
      mem[0] = ParamTest{0, 0, 0.0f};
    }
    auto options = buffer.makeOptions();
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buff_host, std::addressof(buffer), options);
    d.waitForCompletion(result.fence());
  };

  // Init buffers
  ::initScalarBuffer(*device, *buff_device1);
  ::initScalarBuffer(*device, *buff_device2);
  ::initScalarBuffer(*device, *buff_device3);
  ::initScalarBuffer(*device, *buff_device4);
  ::initScalarBuffer(*device, *buff_device5);
  ::initScalarBuffer(*device, *buff_device6);
  ::initScalarBuffer(*device, *buff_device7);
  init_test_buffer(*device, *buff_device8);
  ::initScalarBuffer(*device, *buff_device9);
  ::initScalarBuffer(*device, *buff_device10);
  ::initScalarBuffer(*device, *buff_device11);
  ::initScalarBuffer(*device, *buff_device12);
  ::initScalarBuffer(*device, *buff_device13);
  ::initScalarBuffer(*device, *buff_device14);
  ::initScalarBuffer(*device, *buff_device15);
  init_test_buffer(*device, *buff_device16);
#if !defined(ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
  ::initScalarBuffer(*device, *buff_device17);
  ::initScalarBuffer(*device, *buff_device18);
  ::initScalarBuffer(*device, *buff_device19);
  ::initScalarBuffer(*device, *buff_device20);
  ::initScalarBuffer(*device, *buff_device21);
  ::initScalarBuffer(*device, *buff_device22);
  ::initScalarBuffer(*device, *buff_device23);
  init_test_buffer(*device, *buff_device24);
#endif // ZIVC_MSL_BUFFER_OUT_OF_BOUNDS
#if !defined(Z_MAC)
  ::initScalarBuffer(*device, *buff_device25);
  ::initScalarBuffer(*device, *buff_device26);
  ::initScalarBuffer(*device, *buff_device27);
  ::initScalarBuffer(*device, *buff_device28);
  ::initScalarBuffer(*device, *buff_device29);
  ::initScalarBuffer(*device, *buff_device30);
  ::initScalarBuffer(*device, *buff_device31);
  init_test_buffer(*device, *buff_device32);
  ::initScalarBuffer(*device, *buff_device33);
  ::initScalarBuffer(*device, *buff_device34);
  ::initScalarBuffer(*device, *buff_device35);
  ::initScalarBuffer(*device, *buff_device36);
  ::initScalarBuffer(*device, *buff_device37);
  ::initScalarBuffer(*device, *buff_device38);
  ::initScalarBuffer(*device, *buff_device39);
  init_test_buffer(*device, *buff_device40);
  ::initScalarBuffer(*device, *buff_device41);
  ::initScalarBuffer(*device, *buff_device42);
  ::initScalarBuffer(*device, *buff_device43);
  ::initScalarBuffer(*device, *buff_device44);
  ::initScalarBuffer(*device, *buff_device45);
  ::initScalarBuffer(*device, *buff_device46);
  ::initScalarBuffer(*device, *buff_device47);
  init_test_buffer(*device, *buff_device48);
  ::initScalarBuffer(*device, *buff_device49);
  ::initScalarBuffer(*device, *buff_device50);
  ::initScalarBuffer(*device, *buff_device51);
  ::initScalarBuffer(*device, *buff_device52);
  ::initScalarBuffer(*device, *buff_device53);
  ::initScalarBuffer(*device, *buff_device54);
  ::initScalarBuffer(*device, *buff_device55);
  init_test_buffer(*device, *buff_device56);
  ::initScalarBuffer(*device, *buff_device57);
  ::initScalarBuffer(*device, *buff_device58);
  ::initScalarBuffer(*device, *buff_device59);
  ::initScalarBuffer(*device, *buff_device60);
  ::initScalarBuffer(*device, *buff_device61);
  ::initScalarBuffer(*device, *buff_device62);
  ::initScalarBuffer(*device, *buff_device63);
  init_test_buffer(*device, *buff_device64);
#endif // Z_MAC

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, largeNumOfParametersKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  constexpr std::size_t num_of_args =
#if !defined(Z_MAC)
      72;
#elif defined(ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
      20;
#else // Z_MAC
      28;
#endif // Z_MAC
  ASSERT_EQ(num_of_args, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("LargeNumOfParametersKernel");
    auto result = kernel->run(*buff_device1, *buff_device2, *buff_device3,
                              *buff_device4, *buff_device5, *buff_device6,
                              *buff_device7, *buff_device8, 0,
                              *buff_device9, *buff_device10, *buff_device11,
                              *buff_device12, *buff_device13, *buff_device14,
                              *buff_device15, *buff_device16, 1,
#if !defined(ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
                              *buff_device17, *buff_device18, *buff_device19,
                              *buff_device20, *buff_device21, *buff_device22,
                              *buff_device23, *buff_device24,
#endif // ZIVC_MSL_BUFFER_OUT_OF_BOUNDS
                              2.0f,
#if !defined(Z_MAC)
                              *buff_device25, *buff_device26, *buff_device27,
                              *buff_device28, *buff_device29, *buff_device30,
                              *buff_device31, *buff_device32, 3,
                              *buff_device33, *buff_device34, *buff_device35,
                              *buff_device36, *buff_device37, *buff_device38,
                              *buff_device39, *buff_device40, 4,
                              *buff_device41, *buff_device42, *buff_device43,
                              *buff_device44, *buff_device45, *buff_device46,
                              *buff_device47, *buff_device48, 5.0f,
                              *buff_device49, *buff_device50, *buff_device51,
                              *buff_device52, *buff_device53, *buff_device54,
                              *buff_device55, *buff_device56, 6,
                              *buff_device57, *buff_device58, *buff_device59,
                              *buff_device60, *buff_device61, *buff_device62,
                              *buff_device63, *buff_device64,
#endif // Z_MAC
                              7,
                              launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  auto test_test_buffer = [](zivc::Device& d, const zivc::Buffer<ParamTest>& buffer)
  {
    auto buff_host = d.makeBuffer<ParamTest>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(1);
    {
      auto options = buffer.makeOptions();
      options.setExternalSyncMode(true);
      auto result = zivc::copy(buffer, buff_host.get(), options);
      d.waitForCompletion(result.fence());
    }
    {
      auto mem = buff_host->mapMemory();
      auto result = ((mem[0].i_ == 1) && (mem[0].u_ == 1) && (mem[0].f_ == 1.0f))
          ? testing::AssertionSuccess()
          : testing::AssertionFailure();
      return result;
    }
  };

  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device1));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device2));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device3));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device4));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device5));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device6));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device7));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device8));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device9));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device10));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device11));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device12));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device13));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device14));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device15));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device16));
#if !defined(ZIVC_MSL_BUFFER_OUT_OF_BOUNDS)
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device17));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device18));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device19));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device20));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device21));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device22));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device23));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device24));
#endif // ZIVC_MSL_BUFFER_OUT_OF_BOUNDS
#if !defined(Z_MAC)
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device25));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device26));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device27));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device28));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device29));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device30));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device31));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device32));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device33));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device34));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device35));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device36));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device37));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device38));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device39));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device40));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device41));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device42));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device43));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device44));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device45));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device46));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device47));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device48));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device49));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device50));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device51));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device52));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device53));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device54));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device55));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device56));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device57));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device58));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device59));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device60));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device61));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device62));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device63));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device64));
#endif // Z_MAC
}

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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n);

  // Init buffers
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  // Make a kernels
  auto kernel_params1 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->makeKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->makeKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->makeKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->makeKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->makeKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->makeKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->makeKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel1->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel1->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel2->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel2->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel3->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel3->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel4->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation2Kernel");
    auto result = kernel4->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel5->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel5->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel6->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel6->run(*buff_device, n, launch_options);
  }
  {
    auto launch_options = kernel7->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel7->run(*buff_device, n, launch_options);
  }
  device->waitForCompletion();

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device->size());
    {
      auto options = buff_device->makeOptions();
      options.setExternalSyncMode(true);
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device1 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device1->setSize(n);
  auto buff_device2 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device2->setSize(n);
  auto buff_device3 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device3->setSize(n);

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

  // Make a kernels
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device1, n, launch_options);
    device->waitForCompletion();
  }
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device2, n, launch_options);
    device->waitForCompletion();
  }
  {
    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setExternalSyncMode(false);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel->run(*buff_device3, n, launch_options);
    device->waitForCompletion();
  }

  // Check the outputs
  constexpr uint32b expected = 10 * 1024;
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device1->size());
    {
      auto options = buff_device1->makeOptions();
      options.setExternalSyncMode(true);
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
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device2->size());
    {
      auto options = buff_device2->makeOptions();
      options.setExternalSyncMode(true);
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
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device3->size());
    {
      auto options = buff_device3->makeOptions();
      options.setExternalSyncMode(true);
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n);

  // Make a kernels
  auto kernel_params1 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->makeKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->makeKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->makeKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->makeKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->makeKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->makeKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->makeKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  for (uint32b queue_index = 0; queue_index < 16; ++queue_index) {
    // Init buffers
    {
      auto options = buff_device->makeOptions();
      options.setExternalSyncMode(true);
      auto result = buff_device->fill(0, options);
      device->waitForCompletion(result.fence());
    }

    // Launch the kernels
    {
      auto launch_options = kernel1->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel1->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel2->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel2->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel3->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation1Kernel");
      auto result = kernel3->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel4->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation2Kernel");
      auto result = kernel4->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel5->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel5->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel6->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel6->run(*buff_device, n, launch_options);
    }
    {
      auto launch_options = kernel7->makeOptions();
      launch_options.setWorkSize({n});
      launch_options.setQueueIndex(queue_index);
      launch_options.setExternalSyncMode(false);
      launch_options.setLabel("invocation3Kernel");
      auto result = kernel7->run(*buff_device, n, launch_options);
      device->waitForCompletion(queue_index);
    }

    // Check the outputs
    {
      auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
      buff_host->setSize(buff_device->size());
      {
        auto options = buff_device->makeOptions();
        options.setExternalSyncMode(true);
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());
  [[maybe_unused]] const auto& info = device->deviceInfo();

  using zivc::int32b;
  using zivc::uint32b;

  constexpr std::size_t w = 1920;
  constexpr std::size_t h = 1080;
  constexpr std::size_t n = w * h;

  // Allocate buffers
  auto buff_device = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
  buff_device->setSize(n);

  // Init buffers
  {
    auto options = buff_device->makeOptions();
    options.setExternalSyncMode(true);
    auto result = buff_device->fill(0, options);
    device->waitForCompletion(result.fence());
  }

  device->setFenceSize(7);

  // Make a kernels
  auto kernel_params1 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel1 = device->makeKernel(kernel_params1);
  ASSERT_EQ(1, kernel1->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel1->argSize()) << "Wrong kernel property.";

  auto kernel_params2 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel2 = device->makeKernel(kernel_params2);
  ASSERT_EQ(1, kernel2->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel2->argSize()) << "Wrong kernel property.";

  auto kernel_params3 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation1Kernel, 1);
  auto kernel3 = device->makeKernel(kernel_params3);
  ASSERT_EQ(1, kernel3->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel3->argSize()) << "Wrong kernel property.";

  auto kernel_params4 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation2Kernel, 1);
  auto kernel4 = device->makeKernel(kernel_params4);
  ASSERT_EQ(1, kernel4->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel4->argSize()) << "Wrong kernel property.";

  auto kernel_params5 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel5 = device->makeKernel(kernel_params5);
  ASSERT_EQ(1, kernel5->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel5->argSize()) << "Wrong kernel property.";

  auto kernel_params6 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel6 = device->makeKernel(kernel_params6);
  ASSERT_EQ(1, kernel6->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel6->argSize()) << "Wrong kernel property.";

  auto kernel_params7 = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, invocation3Kernel, 1);
  auto kernel7 = device->makeKernel(kernel_params7);
  ASSERT_EQ(1, kernel7->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel7->argSize()) << "Wrong kernel property.";

  // Launch the kernels
  {
    auto launch_options = kernel1->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(0);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel1->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel2->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(1);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel2->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel3->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(2);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation1Kernel");
    auto result = kernel3->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel4->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(3);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation2Kernel");
    auto result = kernel4->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel5->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(4);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel5->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel6->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(5);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel6->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }
  {
    auto launch_options = kernel7->makeOptions();
    launch_options.setWorkSize({n});
    launch_options.setQueueIndex(6);
    launch_options.setExternalSyncMode(true);
    launch_options.setLabel("invocation3Kernel");
    auto result = kernel7->run(*buff_device, n, launch_options);
    device->waitForCompletion(result.fence());
  }

  // Check the outputs
  {
    auto buff_host = device->makeBuffer<uint32b>(zivc::BufferUsage::kHostOnly);
    buff_host->setSize(buff_device->size());
    {
      auto options = buff_device->makeOptions();
      options.setExternalSyncMode(true);
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

