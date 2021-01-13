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

TEST(BufferTest, DeviceOnlyBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceOnly);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kDeviceOnly, buffer->usage()) << "Buffer initialization failed.";

  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }

  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";

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

  EXPECT_TRUE(buffer->isDeviceLocal()) << "DeviceOnly buffer isn't device local.";
}

TEST(BufferTest, HostOnlyBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kHostOnly, buffer->usage()) << "Buffer initialization failed.";

  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }

  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";

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

  EXPECT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
}

TEST(BufferTest, HostToDeviceBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostToDevice);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kHostToDevice, buffer->usage()) << "Buffer initialization failed.";

  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }

  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";

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

  EXPECT_TRUE(buffer->isHostVisible()) << "HostOnly buffer isn't host visible.";
}

TEST(BufferTest, DeviceToHostBufferInitializationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceToHost);
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kDeviceToHost, buffer->usage()) << "Buffer initialization failed.";

  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }

  buffer->clear();
  ASSERT_EQ(0, buffer->size()) << "Buffer 'clear' failed.";

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
}

TEST(BufferTest, DeviceBufferMaxAllocationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kDeviceOnly);

  const auto& info = device->deviceInfo();
  {
    const std::size_t s = info.maxAllocationSize() / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Max alocation failed.";
  }
}

TEST(BufferTest, HostBufferBigSizeAllocationTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);

  {
    constexpr std::size_t alloc_size = 8ull * 1024ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_EQ(s, buffer->size()) << "Big size alocation failed.";
  }
}

TEST(BufferTest, HostBufferMemoryMappingTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);

  // Bit size allocation test
  {
    constexpr std::size_t alloc_size = 4ull * 1024ull * 1024ull * 1024ull;
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
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
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
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_host2->size() - 2 * offset);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
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
    auto result = zivc::copy(*buffer_host, buffer_host2.get(), options);
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
    auto options = buffer_host2->makeOptions();
    options.setLabel("HostToHostCopy");
    options.setExternalSyncMode(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    auto result = zivc::copy(*buffer_host2, buffer_host.get(), options);
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
