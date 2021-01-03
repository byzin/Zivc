/*!
  \file platform_test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"

TEST(PlatformTest, InitializationTest)
{
  auto platform = ztest::makePlatform();
  ASSERT_TRUE(platform) << "Platform isn't available.";
  // Check if platform has sub-platform
  const auto& config = ztest::Config::globalConfig();
  const auto type = (config.deviceId() == 0) ? zivc::SubPlatformType::kCpu
                                             : zivc::SubPlatformType::kVulkan;
  ASSERT_TRUE(platform->hasSubPlatform(type)) << "Platform initialization failed.";
  // Check if platform has the memory resource
  ASSERT_EQ(config.memoryResource(), platform->memoryResource())
      << "Platform doesn't have memory resource.";
}
