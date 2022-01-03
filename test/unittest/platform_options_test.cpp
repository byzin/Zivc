/*!
  \file platform_options_test.cpp
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
#include <string_view>
// Zivc
#include "zivc/platform_options.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "googletest.hpp"

TEST(PlatformOptionsTest, SetterGetterTest)
{
  zivc::PlatformOptions options{nullptr};

  // Platform name
  const std::string_view platform_name{"ZivcUnitTest"};
  options.setPlatformName(platform_name);
  ASSERT_STREQ(platform_name.data(), options.platformName().data());
  // Platfrom version
  options.setPlatformVersionMajor(zivc::Config::versionMajor());
  options.setPlatformVersionMinor(zivc::Config::versionMinor());
  options.setPlatformVersionPatch(zivc::Config::versionPatch());
  ASSERT_EQ(zivc::Config::versionMajor(), options.platformVersionMajor());
  ASSERT_EQ(zivc::Config::versionMinor(), options.platformVersionMinor());
  ASSERT_EQ(zivc::Config::versionPatch(), options.platformVersionPatch());
  // CPU params
  options.setCpuNumOfThreads(1);
  options.setCpuTaskBatchSize(1);
  ASSERT_TRUE(options.cpuNumOfThreads());
  ASSERT_TRUE(options.cpuTaskBatchSize());
  options.setCpuNumOfThreads(0);
  options.setCpuTaskBatchSize(0);
  ASSERT_FALSE(options.cpuNumOfThreads());
  ASSERT_FALSE(options.cpuTaskBatchSize());
  // Debug
  options.enableDebugMode(true);
  ASSERT_TRUE(options.debugModeEnabled());
  options.enableDebugMode(false);
  ASSERT_FALSE(options.debugModeEnabled());
}
