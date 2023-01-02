/*!
  \file context_test.cpp
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
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <span>
#include <string_view>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"

// Check if vulkan.hpp isn't visible from user
#if defined(VULKAN_HPP)
static_assert(false, "vulkan.hpp is visible from user.");
#endif // VULKAN_HPP

#define ZIVC_CHECK_TYPE_SIZE(type, expected_size) \
  static_assert(sizeof( type ) == ( expected_size ), \
                "The size of '" #type "' isn't correct."); \
  static_assert(std::alignment_of_v< type > == ( expected_size ), \
                "The alignment of '" #type "' isn't correct.")

// Check size of basic data types
ZIVC_CHECK_TYPE_SIZE(zivc::int8b, 1);
ZIVC_CHECK_TYPE_SIZE(zivc::uint8b, 1);
ZIVC_CHECK_TYPE_SIZE(zivc::int16b, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::uint16b, 2);
ZIVC_CHECK_TYPE_SIZE(zivc::int32b, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::uint32b, 4);
ZIVC_CHECK_TYPE_SIZE(zivc::int64b, 8);
ZIVC_CHECK_TYPE_SIZE(zivc::uint64b, 8);

TEST(ContextTest, InitializationTest)
{
  const zivc::SharedContext context = ztest::createContext();
  ASSERT_TRUE(context) << "Context isn't available.";
  // Check if context has backend
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::BackendType type = (config.deviceId() == 0) ? zivc::BackendType::kCpu
                                                          : zivc::BackendType::kVulkan;
  ASSERT_TRUE(context->hasBackend(type)) << "Context initialization failed.";
  // Check if context has the memory resource
  ASSERT_EQ(config.memoryResource(), context->memoryResource())
      << "Context doesn't have memory resource.";
  // Check capacity for kernels
  const zivc::Backend* backend = context->backend(type);
  ASSERT_EQ(2048, backend->capacityForModulesPerDevice())
        << "Capacity for modules isn't set correctly.";
  ASSERT_EQ(4096, backend->capacityForKernelsPerDevice())
        << "Capacity for kernels isn't set correctly.";
}

TEST(ContextTest, DeviceInfoTest)
{
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedContext context = ztest::createContext();
  const zivc::DeviceInfo* const info = context->deviceInfoList()[config.deviceId()];
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
  const std::span heap_info_list = info->heapInfoList();
  EXPECT_GT(heap_info_list.size(), 0) << "Heap size is zero.";
  std::cout << "## Num of heaps: " << heap_info_list.size() << std::endl;
  // Memory
  for (std::size_t i = 0; i < heap_info_list.size(); ++i) {
    const zivc::MemoryHeapInfo& heap_info = heap_info_list[i];
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

TEST(ContextTest, CreateDeviceExceptionTest)
{
  const zivc::SharedContext context = ztest::createContext();
  constexpr std::size_t device_index = (std::numeric_limits<std::size_t>::max)();
  ASSERT_THROW(auto d = context->queryDevice(device_index), zivc::SystemError)
      << "The bounds check of device index isn't performed.";
  const std::span info_list = context->deviceInfoList();
  ASSERT_THROW(auto d = context->queryDevice(info_list.size()), zivc::SystemError)
      << "The bounds check of device index isn't performed.";
}

TEST(ContextTest, CreateDeviceTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();

  const zivc::SharedDevice device1 = context->queryDevice(config.deviceId());
  ASSERT_TRUE(device1) << "Device creation failed.";
  // Second query should return the same device unless the first device is destroyed
  const zivc::SharedDevice device2 = context->queryDevice(config.deviceId());
  ASSERT_EQ(device1.get(), device2.get()) << "The device query failed.";
}
