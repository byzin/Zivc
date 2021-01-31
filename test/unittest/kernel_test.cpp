/*!
  \file kernel_test.cpp
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
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"
#include "zivc/kernel_set/kernel_set-kernel_test.hpp"

TEST(KernelTest, SimpleKernelTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  // Allocate buffers
  auto buffer = device->makeBuffer<zivc::int32b>(zivc::BufferUsage::kDeviceOnly);
  buffer->setSize(1);

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(kernel_test, simpleKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.setExternalSyncMode(false);
  launch_options.setLabel("SimpleKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_FALSE(launch_options.isExternalSyncMode());
  auto result = kernel->run(*buffer, launch_options);
  device->waitForCompletion();
}
