/*!
  \file kernel_test_params.cpp
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
#include "zivc/kernel_set/kernel_set-kernel_test_params.hpp"

namespace {

//! Initialize a scalar buffer
template <zisc::Scalar Scalar>
void initScalarBuffer(const zivc::Device& device, zivc::Buffer<Scalar>& buffer)
{
  zivc::BufferLaunchOptions options = buffer.createOptions();
  options.requestFence(true);
  const zivc::LaunchResult result = buffer.fill(zisc::cast<Scalar>(0), options);
  device.waitForCompletion(result.fence());
}

//! Check the value of scalar buffer
template <zisc::Scalar Scalar>
testing::AssertionResult testScalarBuffer(zivc::Device& device,
                                          const zivc::Buffer<Scalar>& buffer)
{
  const zivc::SharedBuffer buff_host = device.createBuffer<Scalar>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  buff_host->setSize(1);
  {
    zivc::BufferLaunchOptions options = buffer.createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(buffer, buff_host.get(), options);
    device.waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<Scalar> mem = buff_host->mapMemory();
    const testing::AssertionResult result = zisc::equal(mem[0], zisc::cast<Scalar>(1))
        ? testing::AssertionSuccess()
        : testing::AssertionFailure();
    return result;
  }
}

} // namespace 

TEST(KernelTest, LargeNumOfParametersTest)
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
  using ParamTest = zivc::cl::ParamTest;

  // Allocate buffers
  const zivc::SharedBuffer buff_device1 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device1->setSize(1);
  const zivc::SharedBuffer buff_device2 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device2->setSize(1);
  const zivc::SharedBuffer buff_device3 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device3->setSize(1);
  const zivc::SharedBuffer buff_device4 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device4->setSize(1);
  const zivc::SharedBuffer buff_device5 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device5->setSize(1);
  const zivc::SharedBuffer buff_device6 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device6->setSize(1);
  const zivc::SharedBuffer buff_device7 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device7->setSize(1);
  const zivc::SharedBuffer buff_device8 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device8->setSize(1);

  const zivc::SharedBuffer buff_device9 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device9->setSize(1);
  const zivc::SharedBuffer buff_device10 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device10->setSize(1);
  const zivc::SharedBuffer buff_device11 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device11->setSize(1);
  const zivc::SharedBuffer buff_device12 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device12->setSize(1);
  const zivc::SharedBuffer buff_device13 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device13->setSize(1);
  const zivc::SharedBuffer buff_device14 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device14->setSize(1);
  const zivc::SharedBuffer buff_device15 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device15->setSize(1);
  const zivc::SharedBuffer buff_device16 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device16->setSize(1);

  const zivc::SharedBuffer buff_device17 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device17->setSize(1);
  const zivc::SharedBuffer buff_device18 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device18->setSize(1);
  const zivc::SharedBuffer buff_device19 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device19->setSize(1);
  const zivc::SharedBuffer buff_device20 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device20->setSize(1);
  const zivc::SharedBuffer buff_device21 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device21->setSize(1);
  const zivc::SharedBuffer buff_device22 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device22->setSize(1);
  const zivc::SharedBuffer buff_device23 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device23->setSize(1);
  const zivc::SharedBuffer buff_device24 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device24->setSize(1);

  const zivc::SharedBuffer buff_device25 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device25->setSize(1);
  const zivc::SharedBuffer buff_device26 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device26->setSize(1);
  const zivc::SharedBuffer buff_device27 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device27->setSize(1);
  const zivc::SharedBuffer buff_device28 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device28->setSize(1);
#if !defined(Z_MAC)
  const zivc::SharedBuffer buff_device29 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device29->setSize(1);
  const zivc::SharedBuffer buff_device30 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device30->setSize(1);
  const zivc::SharedBuffer buff_device31 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device31->setSize(1);
  const zivc::SharedBuffer buff_device32 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device32->setSize(1);

  const zivc::SharedBuffer buff_device33 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device33->setSize(1);
  const zivc::SharedBuffer buff_device34 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device34->setSize(1);
  const zivc::SharedBuffer buff_device35 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device35->setSize(1);
  const zivc::SharedBuffer buff_device36 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device36->setSize(1);
  const zivc::SharedBuffer buff_device37 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device37->setSize(1);
  const zivc::SharedBuffer buff_device38 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device38->setSize(1);
  const zivc::SharedBuffer buff_device39 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device39->setSize(1);
  const zivc::SharedBuffer buff_device40 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device40->setSize(1);

  const zivc::SharedBuffer buff_device41 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device41->setSize(1);
  const zivc::SharedBuffer buff_device42 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device42->setSize(1);
  const zivc::SharedBuffer buff_device43 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device43->setSize(1);
  const zivc::SharedBuffer buff_device44 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device44->setSize(1);
  const zivc::SharedBuffer buff_device45 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device45->setSize(1);
  const zivc::SharedBuffer buff_device46 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device46->setSize(1);
  const zivc::SharedBuffer buff_device47 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device47->setSize(1);
  const zivc::SharedBuffer buff_device48 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device48->setSize(1);

  const zivc::SharedBuffer buff_device49 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device49->setSize(1);
  const zivc::SharedBuffer buff_device50 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device50->setSize(1);
  const zivc::SharedBuffer buff_device51 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device51->setSize(1);
  const zivc::SharedBuffer buff_device52 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device52->setSize(1);
  const zivc::SharedBuffer buff_device53 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device53->setSize(1);
  const zivc::SharedBuffer buff_device54 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device54->setSize(1);
  const zivc::SharedBuffer buff_device55 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device55->setSize(1);
  const zivc::SharedBuffer buff_device56 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device56->setSize(1);

  const zivc::SharedBuffer buff_device57 = device->createBuffer<int8b>(zivc::BufferUsage::kPreferDevice);
  buff_device57->setSize(1);
  const zivc::SharedBuffer buff_device58 = device->createBuffer<int16b>(zivc::BufferUsage::kPreferDevice);
  buff_device58->setSize(1);
  const zivc::SharedBuffer buff_device59 = device->createBuffer<int32b>(zivc::BufferUsage::kPreferDevice);
  buff_device59->setSize(1);
  const zivc::SharedBuffer buff_device60 = device->createBuffer<uint8b>(zivc::BufferUsage::kPreferDevice);
  buff_device60->setSize(1);
  const zivc::SharedBuffer buff_device61 = device->createBuffer<uint16b>(zivc::BufferUsage::kPreferDevice);
  buff_device61->setSize(1);
  const zivc::SharedBuffer buff_device62 = device->createBuffer<uint32b>(zivc::BufferUsage::kPreferDevice);
  buff_device62->setSize(1);
  const zivc::SharedBuffer buff_device63 = device->createBuffer<float>(zivc::BufferUsage::kPreferDevice);
  buff_device63->setSize(1);
  const zivc::SharedBuffer buff_device64 = device->createBuffer<ParamTest>(zivc::BufferUsage::kPreferDevice);
  buff_device64->setSize(1);
#endif // Z_MAC

  auto init_test_buffer = [](zivc::Device& d, zivc::Buffer<ParamTest>& buffer)
  {
    const zivc::SharedBuffer buff_host = d.createBuffer<ParamTest>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kSequentialWritable});
    buff_host->setSize(1);
    {
      zivc::MappedMemory<ParamTest> mem = buff_host->mapMemory();
      mem[0] = ParamTest{0, 0, 0.0f};
    }
    zivc::BufferLaunchOptions options = buffer.createOptions();
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buff_host, std::addressof(buffer), options);
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
  ::initScalarBuffer(*device, *buff_device17);
  ::initScalarBuffer(*device, *buff_device18);
  ::initScalarBuffer(*device, *buff_device19);
  ::initScalarBuffer(*device, *buff_device20);
  ::initScalarBuffer(*device, *buff_device21);
  ::initScalarBuffer(*device, *buff_device22);
  ::initScalarBuffer(*device, *buff_device23);
  init_test_buffer(*device, *buff_device24);
  ::initScalarBuffer(*device, *buff_device25);
  ::initScalarBuffer(*device, *buff_device26);
  ::initScalarBuffer(*device, *buff_device27);
  ::initScalarBuffer(*device, *buff_device28);
#if !defined(Z_MAC)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_test_params,
                                                      largeNumOfParametersKernel,
                                                      1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  constexpr std::size_t num_of_args =
#if !defined(Z_MAC)
      72;
#else // Z_MAC
      32;
#endif // Z_MAC
  ASSERT_EQ(num_of_args, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  {
    zivc::KernelLaunchOptions launch_options = kernel->createOptions();
    launch_options.setWorkSize({{1}});
    launch_options.requestFence(false);
    launch_options.setLabel("LargeNumOfParametersKernel");
    const zivc::LaunchResult result = kernel->run(
                              *buff_device1, *buff_device2, *buff_device3,
                              *buff_device4, *buff_device5, *buff_device6,
                              *buff_device7, *buff_device8, 0,
                              *buff_device9, *buff_device10, *buff_device11,
                              *buff_device12, *buff_device13, *buff_device14,
                              *buff_device15, *buff_device16, 1,
                              *buff_device17, *buff_device18, *buff_device19,
                              *buff_device20, *buff_device21, *buff_device22,
                              *buff_device23, *buff_device24, 2.0f,
                              *buff_device25, *buff_device26, *buff_device27,
                              *buff_device28,
#if !defined(Z_MAC)
                              *buff_device29, *buff_device30,
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
    const zivc::SharedBuffer buff_host = d.createBuffer<ParamTest>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});
    buff_host->setSize(1);
    {
      zivc::BufferLaunchOptions options = buffer.createOptions();
      options.requestFence(true);
      const zivc::LaunchResult result = zivc::copy(buffer, buff_host.get(), options);
      d.waitForCompletion(result.fence());
    }
    {
      const zivc::MappedMemory mem = buff_host->mapMemory();
      const testing::AssertionResult result = ((mem[0].i_ == 1) && (mem[0].u_ == 1) && (mem[0].f_ == 1.0f))
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
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device17));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device18));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device19));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device20));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device21));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device22));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device23));
  ASSERT_TRUE(test_test_buffer(*device, *buff_device24));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device25));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device26));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device27));
  ASSERT_TRUE(::testScalarBuffer(*device, *buff_device28));
#if !defined(Z_MAC)
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
