/*!
  \file context_options_test.cpp
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
#include <string_view>
// Zivc
#include "zivc/context_options.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/googletest.hpp"

TEST(ContextOptionsTest, SetterGetterTest)
{
  zivc::ContextOptions options{nullptr};

  // Context name
  const std::string_view context_name{"ZivcUnitTest"};
  options.setContextName(context_name);
  ASSERT_STREQ(context_name.data(), options.contextName().data());
  // Platfrom version
  options.setContextVersionMajor(zivc::Config::versionMajor());
  options.setContextVersionMinor(zivc::Config::versionMinor());
  options.setContextVersionPatch(zivc::Config::versionPatch());
  ASSERT_EQ(zivc::Config::versionMajor(), options.contextVersionMajor());
  ASSERT_EQ(zivc::Config::versionMinor(), options.contextVersionMinor());
  ASSERT_EQ(zivc::Config::versionPatch(), options.contextVersionPatch());
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
