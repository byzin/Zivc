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

// Standard C++ library
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
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

TEST(PlatformTest, DeviceInfoTest)
{
  const auto& config = ztest::Config::globalConfig();
  auto platform = ztest::makePlatform();
  const auto info = platform->deviceInfoList()[config.deviceId()];
  // Device name
  {
    const std::string_view name = info->name();
    EXPECT_FALSE(name.empty()) << "Device name isn't set.";
    EXPECT_STRNE(zivc::DeviceInfo::invalidName().data(), name.data())
        << "Device name isn't set.";
    std::cout << "## Device name: " << name << std::endl;
  }
  // Vendor name
  {
    const std::string_view name = info->vendorName();
    EXPECT_FALSE(name.empty()) << "Vendor name isn't set.";
    EXPECT_STRNE(zivc::DeviceInfo::invalidName().data(), name.data())
        << "Vendor name isn't set.";
    std::cout << "## Vendor name: " << name << std::endl;
  }

  const auto to_mb = [](const std::size_t size) noexcept
  {
    const zisc::ByteUnit bytes{zisc::cast<zivc::int64b>(size)};
    const zisc::MebiUnit mega_bytes{bytes};
    return zisc::cast<double>(mega_bytes.value());
  };

  // Heap
  const std::size_t n_heap = info->numOfHeaps();
  EXPECT_GT(n_heap, 0) << "Heap size is zero.";
  std::cout << "## Num of heaps: " << n_heap << std::endl;
  // Memory
  for (std::size_t i = 0; i < n_heap; ++i) {
    const std::size_t total_mem_size = info->totalMemory(i);
    EXPECT_GT(total_mem_size, 0)
        << "Heap[" << i << "]: total memory size isn't available.";
    std::cout << "    Heap[" << i << "]:     total memory size: "
              << to_mb(total_mem_size) << " MB." << std::endl;
    const std::size_t avail_mem_size = info->availableMemory(i);
    EXPECT_GT(avail_mem_size, 0)
        << "Heap[" << i << "]: available memory size isn't available.";
    std::cout << "    Heap[" << i << "]: available memory size: "
              << to_mb(avail_mem_size) << " MB." << std::endl;
  }
  // Allocation size
  {
    const std::size_t max_alloc_size = info->maxAllocationSize();
    EXPECT_GT(max_alloc_size, 0) << "Max allocation size isn't available.";
    std::cout << "## Max allocation size: "
              << to_mb(max_alloc_size) << " MB." << std::endl;
  }
  // Work group
  {
    const std::size_t group_size = info->workGroupSize();
    EXPECT_GT(group_size, 0) << "Work group size isn't available.";
    std::cout << "## Work group size: " << group_size << std::endl;
  }
  // Max work group count
  {
    const std::array<zivc::uint32b, 3> group_count = info->maxWorkGroupCount();
    for (std::size_t i = 0; i < group_count.size(); ++i) {
      EXPECT_GT(group_count[i], 0)
          << "Max work group count at " << i << "dim isn't available.";
    }
    std::cout << "## Max work group count: (" << group_count[0] << ", "
                                              << group_count[1] << ", "
                                              << group_count[2] << ")."
                                              << std::endl;
  }
}
