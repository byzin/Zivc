/*!
  \file buffer_test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \note No notation.
  \attention No attention.

  \copyright
  No copyright
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
#include "zisc/math/unit_multiple.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"

TEST(BufferTest, DeviceOnlyBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  // Init test
  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceOnly);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kDeviceOnly, buffer->usage()) << "Buffer initialization failed.";
  EXPECT_TRUE(buffer->isDeviceLocal()) << "DeviceOnly buffer isn't device local.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";
  EXPECT_TRUE(buffer->isDeviceLocal()) << "DeviceOnly buffer isn't device local.";
  // Resize test
  {
    constexpr std::size_t n = 3;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 10;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 16;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 100;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1000;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 2048;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  ASSERT_TRUE(buffer->isDeviceLocal()) << "DeviceOnly buffer isn't device local.";
}

TEST(BufferTest, HostOnlyBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  // Init test
  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kHostOnly, buffer->usage()) << "Buffer initialization failed.";
  EXPECT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear
  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";
  EXPECT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
  // Resize test
  {
    constexpr std::size_t n = 3;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 10;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 16;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 100;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1000;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 2048;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  ASSERT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
}

TEST(BufferTest, HostToDeviceBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  // Init test
  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostToDevice);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kHostToDevice, buffer->usage()) << "Buffer initialization failed.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";
  // Resize test
  {
    constexpr std::size_t n = 3;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 10;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 16;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 100;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1000;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 2048;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  EXPECT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
}

TEST(BufferTest, DeviceToHostBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());
  // Init test
  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceToHost);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kDeviceToHost, buffer->usage()) << "Buffer initialization failed.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";
  // Resize test
  {
    constexpr std::size_t n = 3;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 10;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 16;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 100;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1000;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 2048;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, DeviceBufferMaxAllocationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceOnly);

  const auto& info = device->deviceInfo();
  {
    constexpr std::size_t max_alloc = 4ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Max alocation of device buffer failed.";
  }

  {
    const auto to_mb = [](const std::size_t size) noexcept
    {
      const zisc::ByteUnit bytes{zisc::cast<zivc::int64b>(size)};
      const zisc::MebiUnit mega_bytes{bytes};
      return zisc::cast<double>(mega_bytes.value());
    };

    const std::size_t alloc_size = sizeof(int) * buffer->capacity();
    std::cout << "## Max allocation size: "
              << to_mb(alloc_size) << " MB." << std::endl;
    const auto& heap_info = info.heapInfo(buffer->heapIndex());
    const auto& usage = heap_info.usedSizeForBuffer();
    ASSERT_GT(usage.peak(), 0) << "Memory usage isn't updated.";
    std::cout << "## Used memory size: "
              << to_mb(usage.peak()) << " MB." << std::endl;
  }
}

TEST(BufferTest, HostBufferMaxAllocationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);

  const auto& info = device->deviceInfo();
  {
    constexpr std::size_t max_alloc = 4ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Max allocation of host buffer failed.";
  }

  {
    const auto to_mb = [](const std::size_t size) noexcept
    {
      const zisc::ByteUnit bytes{zisc::cast<zivc::int64b>(size)};
      const zisc::MebiUnit mega_bytes{bytes};
      return zisc::cast<double>(mega_bytes.value());
    };

    const std::size_t alloc_size = sizeof(int) * buffer->capacity();
    std::cout << "## Max allocation size: "
              << to_mb(alloc_size) << " MB." << std::endl;
    const auto& heap_info = info.heapInfo(buffer->heapIndex());
    const auto& usage = heap_info.usedSizeForBuffer();
    ASSERT_GT(usage.peak(), 0) << "Memory usage isn't updated.";
    std::cout << "## Used memory size: "
              << to_mb(usage.peak()) << " MB." << std::endl;
  }
}

TEST(BufferTest, HostBufferMemoryMappingTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);

  // Max allocation test
  {
    const auto& info = device->deviceInfo();
    constexpr std::size_t max_alloc = 4ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    auto mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<int>(i);
  }

  //
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    auto mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }

  // Test1
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<int>(i);
  }
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const int expected = zisc::cast<int>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Memory mappling failed.";
    }
  }
  // Test2
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const std::size_t value = mapped_mem.size() - i;
      mapped_mem.set(i, zisc::cast<int>(value));
    }
  }
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const int expected = zisc::cast<int>(mapped_mem.size() - i);
      ASSERT_EQ(expected, mapped_mem[i]) << "'set' failed.";
    }
  }
  // Test3
  constexpr int test3_value = 8;
  {
    auto mapped_mem = buffer->mapMemory();
    std::fill(mapped_mem.begin(), mapped_mem.end(), test3_value);
  }
  {
    auto mapped_mem = buffer->mapMemory();
    std::size_t n = 0;
    for (auto ite = mapped_mem.cbegin(); ite != mapped_mem.cend(); ++ite) {
      ASSERT_EQ(test3_value, *ite) << "iterator of MappedMemory failed.";
      ++n;
    }
    ASSERT_EQ(mapped_mem.size(), n) << "iterator of MappedMemory failed.";
  }
}

TEST(BufferTest, HostToDeviceBufferMemoryMappingTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);

  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "HostToDevice buffer allocation failed.";
    auto mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  // Test1
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<int>(i);
  }
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const int expected = zisc::cast<int>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Memory mappling failed.";
    }
  }
  // Test2
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const std::size_t value = mapped_mem.size() - i;
      mapped_mem.set(i, zisc::cast<int>(value));
    }
  }
  {
    auto mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const int expected = zisc::cast<int>(mapped_mem.size() - i);
      ASSERT_EQ(expected, mapped_mem[i]) << "'set' failed.";
    }
  }
  // Test3
  constexpr int test3_value = 8;
  {
    auto mapped_mem = buffer->mapMemory();
    std::fill(mapped_mem.begin(), mapped_mem.end(), test3_value);
  }
  {
    auto mapped_mem = buffer->mapMemory();
    std::size_t n = 0;
    for (auto ite = mapped_mem.cbegin(); ite != mapped_mem.cend(); ++ite) {
      ASSERT_EQ(test3_value, *ite) << "iterator of MappedMemory failed.";
      ++n;
    }
    ASSERT_EQ(mapped_mem.size(), n) << "iterator of MappedMemory failed.";
  }
}

TEST(BufferTest, CopyMaxAllocBufferTest)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);

  const auto& info = device->deviceInfo();
  // Allocate memories
  {
    constexpr std::size_t max_alloc = 4ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_device->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  // Copy from host to device
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("HostToDeviceCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_host, buffer_device.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
}

TEST(BufferTest, CopyBufferTest)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_device->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  // Copy from host to device
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("HostToDeviceCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_host, buffer_device.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
  }
}

TEST(BufferTest, CopyBufferRangeTest)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_device->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("HostToDeviceCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    auto result = zivc::copy(*buffer_host, buffer_device.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    const std::size_t s = buffer_host->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
  }
}

TEST(BufferTest, CopyBufferRangeTest2)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_device->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("HostToDeviceCopy");
    options.setExternalSyncMode(true);
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - offset);
    auto result = zivc::copy(*buffer_host, buffer_device.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - offset);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < buffer_host->size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i - offset);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
  }
}

TEST(BufferTest, CopyHostBufferTest)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_host2 = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_host2->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  // Copy from host to device
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_host, buffer_host2.get(), options);
    if (buffer_host2->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
    if (buffer_host->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
  }
}

TEST(BufferTest, CopyHostBufferRangeTest)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_host2 = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_host2->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_host2->size() - 2 * offset);
    auto result = zivc::copy(*buffer_host, buffer_host2.get(), options);
    if (buffer_host2->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_host2->size() - 2 * offset);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
    if (buffer_host->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    const std::size_t s = buffer_host->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
  }
}

TEST(BufferTest, CopyHostBufferRangeTest2)
{
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_host2 = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_host2->setSize(s);
  }
  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = zisc::cast<uint64b>(i);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setDestOffset(offset);
    options.setSize(buffer_host2->size() - offset);
    auto result = zivc::copy(*buffer_host, buffer_host2.get(), options);
    if (buffer_host2->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_host2->size() - offset);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
    if (buffer_host->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < buffer_host->size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i - offset);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferFastInt8Test)
{
  using zisc::int8b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int8b>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int8b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int8b);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  constexpr int8b v = 24;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferFastInt16Test)
{
  using zisc::int16b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int16b>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int16b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int16b);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  constexpr int16b v = 24;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferFastInt32Test)
{
  using zisc::int32b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  constexpr int32b v = 24;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferFastRangeTest)
{
  using zisc::int32b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int32b>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  constexpr int32b v = 24;
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        result.fence().wait();
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        result.fence().wait();
      }
    }
  }
  // Copy from device to host
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_device, buffer_host.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Filling buffer range failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer range failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Filling buffer range failed.";
    }
  }
}

TEST(BufferTest, FillHostBufferTest)
{
  using zisc::int32b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_host->setSize(s);
  }
  constexpr int32b v = 24;
  // Fill buffer test
  {
    auto options = buffer_host->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    auto result = buffer_host->fill(v, options);
    if (buffer_host->isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillHostBufferRangeTest)
{
  using zisc::int32b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<int32b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_host->setSize(s);
  }
  constexpr int32b v = 24;
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_host->makeOptions();
    options.setLabel("FillBuffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_host->fill(0, options);
      if (buffer_host->isDeviceLocal()) {
        ASSERT_TRUE(result.isAsync());
        device->waitForCompletion(result.fence());
      }
      else {
        ASSERT_FALSE(result.isAsync());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_host->size() - 2 * offset);
    {
      auto result = buffer_host->fill(v, options);
      if (buffer_host->isDeviceLocal()) {
        ASSERT_TRUE(result.isAsync());
        device->waitForCompletion(result.fence());
      }
      else {
        ASSERT_FALSE(result.isAsync());
      }
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Filling buffer range failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer range failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Filling buffer range failed.";
    }
  }
}

TEST(BufferTest, CopyBufferReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_device->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto buffer_host2 = buffer_host->reinterp<uint16b>();
  {
    ASSERT_EQ(buffer_host->capacityInBytes(), buffer_host2.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->getParent(), buffer_host2.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->getOwn(), buffer_host2.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->heapIndex(), buffer_host2.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->isDebugMode(), buffer_host2.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->isDeviceLocal(), buffer_host2.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->isHostCached(), buffer_host2.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->isHostCoherent(), buffer_host2.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->isHostVisible(), buffer_host2.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->memoryResource(), buffer_host2.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->sizeInBytes(), buffer_host2.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->type(), buffer_host2.type())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_host->type(), buffer_host2.type())
        << " Reinterp adopter is wrong.";
  }
  auto buffer_device2 = buffer_device->reinterp<uint16b>();
  {
    ASSERT_EQ(buffer_device->capacityInBytes(), buffer_device2.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->getParent(), buffer_device2.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->getOwn(), buffer_device2.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->heapIndex(), buffer_device2.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isDebugMode(), buffer_device2.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isDeviceLocal(), buffer_device2.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostCached(), buffer_device2.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostCoherent(), buffer_device2.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostVisible(), buffer_device2.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->memoryResource(), buffer_device2.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->sizeInBytes(), buffer_device2.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->type(), buffer_device2.type())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->type(), buffer_device2.type())
        << " Reinterp adopter is wrong.";
  }
  // Reinterp-reinterp test
  {
    auto buffer_device3 = buffer_device2.reinterp<uint8b>();
    ASSERT_EQ(buffer_device->capacityInBytes(), buffer_device3.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->getParent(), buffer_device3.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->getOwn(), buffer_device3.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->heapIndex(), buffer_device3.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isDebugMode(), buffer_device3.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isDeviceLocal(), buffer_device3.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostCached(), buffer_device3.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostCoherent(), buffer_device3.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->isHostVisible(), buffer_device3.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->memoryResource(), buffer_device3.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->sizeInBytes(), buffer_device3.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->type(), buffer_device3.type())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_device->type(), buffer_device3.type())
        << " Reinterp adopter is wrong.";
  }

  constexpr std::array<uint16b, 4> v16{{0b0101'0101'0101'0101,
                                        0b0011'0011'0011'0011,
                                        0b1010'1010'1010'1010,
                                        0b1100'1100'1100'1100}};
  constexpr uint64b v64 = (zisc::cast<uint64b>(v16[0]) << 0) |
                          (zisc::cast<uint64b>(v16[1]) << 16) |
                          (zisc::cast<uint64b>(v16[2]) << 32) |
                          (zisc::cast<uint64b>(v16[3]) << 48);

  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host2.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  constexpr std::size_t offset = 10;
  // Copy from host to device
  {
    auto options = buffer_device2.makeOptions();
    options.setLabel("HostToDeviceCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(buffer_device2.size() - 2 * k * offset);
    auto result = zivc::copy(buffer_host2, &buffer_device2, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host2.mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_device2.makeOptions();
    options.setLabel("DeviceToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(buffer_device2.size() - 2 * k * offset);
    auto result = zivc::copy(buffer_device2, &buffer_host2, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the copy is wrong.";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    const std::size_t s = buffer_host->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Copying buffer range failed.";
    }
  }
}

TEST(BufferTest, CopyHostBufferReinterpTest)
{
  using zivc::uint16b;
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_host = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);
  auto buffer_host2 = device->makeBuffer<uint64b>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_host->setSize(s);
    buffer_host2->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto buffer_host3 = buffer_host->reinterp<uint16b>();
  auto buffer_host4 = buffer_host2->reinterp<uint16b>();

  constexpr std::array<uint16b, 4> v16{{0b0101'0101'0101'0101,
                                        0b0011'0011'0011'0011,
                                        0b1010'1010'1010'1010,
                                        0b1100'1100'1100'1100}};
  constexpr uint64b v64 = (zisc::cast<uint64b>(v16[0]) << 0) |
                          (zisc::cast<uint64b>(v16[1]) << 16) |
                          (zisc::cast<uint64b>(v16[2]) << 32) |
                          (zisc::cast<uint64b>(v16[3]) << 48);

  // Initialize the source buffer
  {
    auto mapped_mem = buffer_host3.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  constexpr std::size_t offset = 10;
  // Copy from host to device
  {
    auto options = buffer_host3.makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(buffer_host3.size() - 2 * k * offset);
    auto result = zivc::copy(buffer_host3, &buffer_host4, options);
    if (buffer_host4.isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host3.mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_host4.makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(buffer_host3.size() - 2 * k * offset);
    auto result = zivc::copy(buffer_host4, &buffer_host3, options);
    if (buffer_host3.isDeviceLocal()) {
      ASSERT_TRUE(result.isAsync());
      device->waitForCompletion(result.fence());
    }
    else {
      ASSERT_FALSE(result.isAsync());
      ASSERT_FALSE(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    const std::size_t s = buffer_host->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Copying buffer range failed.";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Copying buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Copying buffer range failed.";
    }
  }
}

TEST(BufferTest, ResizeBufferReinterpTest)
{
  using zivc::uint16b;
  using zivc::uint64b;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<uint64b>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_device2 = buffer_device->reinterp<uint16b>();

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  constexpr std::size_t n = 100;
  constexpr std::size_t s = sizeof(uint16b) * n;

  buffer_device2.setSize(n);
  ASSERT_EQ(buffer_device2.size(), n) << "Reinterp adapter is wrong.";
  ASSERT_EQ(buffer_device2.sizeInBytes(), s) << "Reinterp adapter is wrong.";
  ASSERT_EQ(buffer_device->size(), n / k) << "Reinterp adapter is wrong.";
  ASSERT_EQ(buffer_device->sizeInBytes(), s) << "Reinterp adapter is wrong.";
}
