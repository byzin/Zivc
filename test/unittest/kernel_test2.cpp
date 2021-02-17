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
//#include "zivc/kernel_set/kernel_set-kernel_test2.hpp"

//TEST(KernelTest, LargeNumOfParametersTest)
//{
//  auto platform = ztest::makePlatform();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
//  [[maybe_unused]] const auto& info = device->deviceInfo();
//
//  const std::size_t n = config.testKernelWorkSize1d();
//
//  using zivc::int8b;
//  using zivc::int16b;
//  using zivc::int32b;
//  using zivc::uint8b;
//  using zivc::uint16b;
//  using zivc::uint32b;
//  using ParamTest = zivc::cl::kernel_test2::ParamTest;
//
//  // Allocate buffers
//  auto buff_device1 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device1->setSize(1);
//  auto buff_device2 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device2->setSize(1);
//  auto buff_device3 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device3->setSize(1);
//  auto buff_device4 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device4->setSize(1);
//  auto buff_device5 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device5->setSize(1);
//  auto buff_device6 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device6->setSize(1);
//  auto buff_device7 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device7->setSize(1);
//  auto buff_device8 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device8->setSize(1);
//
//  auto buff_device9 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device9->setSize(1);
//  auto buff_device10 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device10->setSize(1);
//  auto buff_device11 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device11->setSize(1);
//  auto buff_device12 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device12->setSize(1);
//  auto buff_device13 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device13->setSize(1);
//  auto buff_device14 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device14->setSize(1);
//  auto buff_device15 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device15->setSize(1);
//  auto buff_device16 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device16->setSize(1);
//
//#if defined(ZIVC_TEST_64_PARAMETERS_TEST)
//  auto buff_device17 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device17->setSize(1);
//  auto buff_device18 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device18->setSize(1);
//  auto buff_device19 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device19->setSize(1);
//  auto buff_device20 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device20->setSize(1);
//  auto buff_device21 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device21->setSize(1);
//  auto buff_device22 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device22->setSize(1);
//  auto buff_device23 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device23->setSize(1);
//  auto buff_device24 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device24->setSize(1);
//
//  auto buff_device25 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device25->setSize(1);
//  auto buff_device26 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device26->setSize(1);
//  auto buff_device27 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device27->setSize(1);
//  auto buff_device28 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device28->setSize(1);
//  auto buff_device29 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device29->setSize(1);
//  auto buff_device30 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device30->setSize(1);
//  auto buff_device31 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device31->setSize(1);
//  auto buff_device32 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device32->setSize(1);
//
//  auto buff_device33 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device33->setSize(1);
//  auto buff_device34 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device34->setSize(1);
//  auto buff_device35 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device35->setSize(1);
//  auto buff_device36 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device36->setSize(1);
//  auto buff_device37 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device37->setSize(1);
//  auto buff_device38 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device38->setSize(1);
//  auto buff_device39 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device39->setSize(1);
//  auto buff_device40 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device40->setSize(1);
//
//  auto buff_device41 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device41->setSize(1);
//  auto buff_device42 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device42->setSize(1);
//  auto buff_device43 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device43->setSize(1);
//  auto buff_device44 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device44->setSize(1);
//  auto buff_device45 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device45->setSize(1);
//  auto buff_device46 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device46->setSize(1);
//  auto buff_device47 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device47->setSize(1);
//  auto buff_device48 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device48->setSize(1);
//
//  auto buff_device49 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device49->setSize(1);
//  auto buff_device50 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device50->setSize(1);
//  auto buff_device51 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device51->setSize(1);
//  auto buff_device52 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device52->setSize(1);
//  auto buff_device53 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device53->setSize(1);
//  auto buff_device54 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device54->setSize(1);
//  auto buff_device55 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device55->setSize(1);
//  auto buff_device56 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device56->setSize(1);
//
//  auto buff_device57 = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device57->setSize(1);
//  auto buff_device58 = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device58->setSize(1);
//  auto buff_device59 = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device59->setSize(1);
//  auto buff_device60 = device->makeBuffer<uint8b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device60->setSize(1);
//  auto buff_device61 = device->makeBuffer<uint16b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device61->setSize(1);
//  auto buff_device62 = device->makeBuffer<uint32b>(zivc::BufferUsage::kDeviceOnly);
//  buff_device62->setSize(1);
//  auto buff_device63 = device->makeBuffer<float>(zivc::BufferUsage::kDeviceOnly);
//  buff_device63->setSize(1);
//  auto buff_device64 = device->makeBuffer<ParamTest>(zivc::BufferUsage::kDeviceOnly);
//  buff_device64->setSize(1);
//#endif // ZIVC_TEST_64_PARAMETERS_TEST
//
////  auto buff_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);
////  buff_host->setSize(n);
//
//  // Init buffers
////  {
////    {
////      auto mem = buff_host->mapMemory();
////      std::iota(mem.begin(), mem.end(), 0);
////    }
////    auto options = buff_device1->makeOptions();
////    options.setExternalSyncMode(true);
////    auto result = zivc::copy(*buff_host, buff_device1.get(), options);
////    device->waitForCompletion(result.fence());
////  }
//
//  // Make a kernel
//  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_test2, largeNumOfParametersKernel, 1);
////  auto kernel = device->makeKernel(kernel_params);
////  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
////  ASSERT_EQ(64, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
////  {
////    const uint32b res = zisc::cast<uint32b>(n);
////    auto launch_options = kernel->makeOptions();
////    launch_options.setWorkSize({res});
////    launch_options.setExternalSyncMode(false);
////    launch_options.setLabel("LargeNumOfParametersKernel");
//////    auto result = kernel->run(*buff_device1, *buff_device2, *buff_device3,
//////                              *buff_device4, *buff_device5, *buff_device6,
//////                              *buff_device7, *buff_device8, *buff_device9,
//////                              *buff_device10, *buff_device11, *buff_device12,
//////                              *buff_device13, *buff_device14, *buff_device15,
//////                              *buff_device16,
//////#if defined(ZIVC_TEST_64_PARAMETERS_TEST)
//////                              *buff_device17, *buff_device18, *buff_device19,
//////                              *buff_device20, *buff_device21, *buff_device22,
//////                              *buff_device23, *buff_device24, *buff_device25,
//////                              *buff_device26, *buff_device27, *buff_device28,
//////                              *buff_device29, *buff_device30, *buff_device31,
//////                              *buff_device32,
//////                              *buff_device33, *buff_device34, *buff_device35,
//////                              *buff_device36, *buff_device37, *buff_device38,
//////                              *buff_device39, *buff_device40, *buff_device41,
//////                              *buff_device42, *buff_device43, *buff_device44,
//////                              *buff_device45, *buff_device46, *buff_device47,
//////                              *buff_device48,
//////                              *buff_device49, *buff_device50, *buff_device51,
//////                              *buff_device52, *buff_device53, *buff_device54,
//////                              *buff_device55, *buff_device56, *buff_device57,
//////                              *buff_device58, *buff_device59, *buff_device60,
//////                              *buff_device61, *buff_device62, *buff_device63,
//////                              *buff_device64,
//////#endif // ZIVC_TEST_64_PARAMETERS_TEST
//////                              launch_options);
////    device->waitForCompletion();
////  }
//
//  // Check the outputs
////  {
////    auto options = buff_device2->makeOptions();
////    options.setExternalSyncMode(true);
////    auto result = zivc::copy(*buff_device2, buff_host.get(), options);
////    device->waitForCompletion(result.fence());
////
////    const auto mem = buff_host->mapMemory();
////    {
////      const int32b expected = zisc::cast<int32b>(n);
////      EXPECT_EQ(expected, mem[0]) << "Copying POD value failed.";
////    }
////    for (std::size_t i = 1; i < mem.size(); ++i) {
////      const int32b expected = zisc::cast<int32b>(i);
////      ASSERT_EQ(expected, mem[i])
////          << "Copying inputs[" << i << "] to outputs[" << i << "] failed.";
////    }
////  }
//
//}
