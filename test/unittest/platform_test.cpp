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
#include <limits>
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
  const auto* const info = platform->deviceInfoList()[config.deviceId()];
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
  const auto& heap_info_list = info->heapInfoList();
  EXPECT_GT(heap_info_list.size(), 0) << "Heap size is zero.";
  std::cout << "## Num of heaps: " << heap_info_list.size() << std::endl;
  // Memory
  for (std::size_t i = 0; i < heap_info_list.size(); ++i) {
    const auto& heap_info = heap_info_list[i];
    std::cout << "    Heap[" << i << "]:          device local: "
              << heap_info.isDeviceLocal() << std::endl;
    EXPECT_GT(heap_info.totalSize(), 0)
        << "Heap[" << i << "]: total memory size isn't available.";
    std::cout << "    Heap[" << i << "]:     total memory size: "
              << to_mb(heap_info.totalSize()) << " MB." << std::endl;
    EXPECT_GT(heap_info.availableSize(), 0)
        << "Heap[" << i << "]: available memory size isn't available.";
    std::cout << "    Heap[" << i << "]: available memory size: "
              << to_mb(heap_info.availableSize()) << " MB." << std::endl;
  }
  // Allocation size
  {
    const std::size_t max_alloc_size = info->maxAllocationSize();
    EXPECT_GT(max_alloc_size, 0) << "Max allocation size isn't available.";
    std::cout << "## Max allocation size: "
              << to_mb(max_alloc_size) << " MB." << std::endl;
  }
  {
    const std::size_t max_buffers = info->maxNumOfBuffersPerKernel();
    EXPECT_GT(max_buffers, 0) << "The max num of buffers isn't available.";
    std::cout << "## Max num of buffers per kernel: "
              << max_buffers << std::endl;
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

TEST(PlatformTest, MakeDeviceExceptionTest)
{
  auto platform = ztest::makePlatform();
  constexpr std::size_t device_index = (std::numeric_limits<std::size_t>::max)();
  ASSERT_THROW(auto d = platform->makeDevice(device_index), zivc::SystemError)
      << "The bounds check of device index isn't performed.";
  const auto& info_list = platform->deviceInfoList();
  ASSERT_THROW(auto d = platform->makeDevice(info_list.size()), zivc::SystemError)
      << "The bounds check of device index isn't performed.";
}

TEST(PlatformTest, MakeDeviceTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  ASSERT_TRUE(device) << "Device creation failed.";
}
