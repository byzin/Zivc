/*!
  \file test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "config.hpp"

namespace ztest {

/*!
  \details No detailed description

  \return No description
  */
zivc::UniquePlatform makePlatform()
{
  auto& config = Config::globalConfig();
  zisc::pmr::memory_resource* mem_resource = config.memoryResource();

  zivc::PlatformOptions options{mem_resource};
  options.setPlatformName("UnitTest");
  options.setPlatformVersionMajor(zivc::Config::versionMajor());
  options.setPlatformVersionMinor(zivc::Config::versionMinor());
  options.setPlatformVersionPatch(zivc::Config::versionPatch());
  options.enableVulkanSubPlatform(0 < config.deviceId());
  options.enableDebugMode(config.isDebugMode());

  zivc::UniquePlatform platform = zivc::makePlatform(mem_resource, options);
  return platform;
}

} // namespace ztest
