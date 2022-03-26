/*!
  \file buffer_test1.cpp
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
#include <numeric>
#include <utility>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"

TEST(BufferTest, DeviceLocalBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferDevice});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferDevice, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kNone, buffer->flag()) << "Buffer initialization failed.";
  EXPECT_TRUE(buffer->isDeviceLocal()) << "The buffer isn't device local.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    EXPECT_TRUE(buffer->isDeviceLocal()) << "The buffer isn't device local.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  EXPECT_TRUE(buffer->isDeviceLocal()) << "The buffer isn't device local.";
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
  ASSERT_TRUE(buffer->isDeviceLocal()) << "The buffer isn't device local.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, HostWritableDeviceBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferDevice,
                                           zivc::BufferFlag::kSequentialWritable});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferDevice, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kSequentialWritable, buffer->flag()) << "Buffer initialization failed.";
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
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
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, HostReadableDeviceBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferDevice,
                                           zivc::BufferFlag::kRandomAccessible});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferDevice, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kRandomAccessible, buffer->flag()) << "Buffer initialization failed.";
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
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
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, HostBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferHost});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferHost, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kNone, buffer->flag()) << "Buffer initialization failed.";
  EXPECT_TRUE(buffer->isHostVisible()) << "The buffer isn't host visible.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    EXPECT_TRUE(buffer->isHostVisible()) << "The buffer isn't host visible.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear
  buffer->clear();
  EXPECT_TRUE(buffer->isHostVisible()) << "The buffer isn't host visible.";
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
  ASSERT_TRUE(buffer->isHostVisible()) << "The buffer isn't host visible.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, HostWritableHostBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferHost,
                                           zivc::BufferFlag::kSequentialWritable});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferHost, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kSequentialWritable, buffer->flag()) << "Buffer initialization failed.";
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
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
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, HostReadableHostBufferInitializationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());
  // Init test
  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferHost,
                                           zivc::BufferFlag::kRandomAccessible});
  ASSERT_EQ(0, buffer->size()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferUsage::kPreferHost, buffer->usage()) << "Buffer initialization failed.";
  ASSERT_EQ(zivc::BufferFlag::kRandomAccessible, buffer->flag()) << "Buffer initialization failed.";
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
  // Allocation test
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
  // Clear test
  buffer->clear();
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
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
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
  {
    constexpr std::size_t n = 1;
    buffer->setSize(n);
    ASSERT_EQ(n, buffer->size()) << "Buffer 'setSize(" << n << ")' failed.";
  }
}

TEST(BufferTest, DeviceBufferMaxAllocationTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferDevice});
  EXPECT_TRUE(buffer->isDeviceLocal()) << "The buffer isn't device local.";

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
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferHost});
  EXPECT_TRUE(buffer->isHostVisible()) << "The buffer isn't host visible.";

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

TEST(BufferTest, HostRandomAccessibleDeviceBufferMemoryMappingTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferDevice,
                                           zivc::BufferFlag::kRandomAccessible});
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";

  // Empty mapped memory test
  {
    zivc::MappedMemory<int> mapped_mem{nullptr};
    ASSERT_FALSE(mapped_mem) << "Memory mapping initialization failed.";
    ASSERT_EQ(0, mapped_mem.size()) << "Memory mapping initialization failed.";
  }

  using zisc::cast;
  {
    const auto& info = device->deviceInfo();
    constexpr std::size_t max_alloc = 1ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  {
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = cast<int>(i);
  }
  {
    const zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      ASSERT_EQ(cast<int>(i), mapped_mem[i]) << "Mapped memory operation failed.";
  }
  {
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem.set(i, cast<int>(i));
  }
  {
    const zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      ASSERT_EQ(cast<int>(i), mapped_mem[i]) << "Mapped memory operation failed.";
  }
}

TEST(BufferTest, HostRandomAccessibleHostBufferMemoryMappingTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<int>({zivc::BufferUsage::kPreferHost,
                                           zivc::BufferFlag::kRandomAccessible});
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
  ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";

  // Empty mapped memory test
  {
    zivc::MappedMemory<int> mapped_mem{nullptr};
    ASSERT_FALSE(mapped_mem) << "Memory mapping initialization failed.";
    ASSERT_EQ(0, mapped_mem.size()) << "Memory mapping initialization failed.";
  }

  using zisc::cast;
  {
    const auto& info = device->deviceInfo();
    constexpr std::size_t max_alloc = 1ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(int);
    buffer->setSize(s);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_TRUE(buffer->isHostReadable()) << "The buffer isn't host readable.";
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  {
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = cast<int>(i);
  }
  {
    const zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      ASSERT_EQ(cast<int>(i), mapped_mem[i]) << "Mapped memory operation failed.";
  }
  {
    zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem.set(i, cast<int>(i));
  }
  {
    const zivc::MappedMemory<int> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      ASSERT_EQ(cast<int>(i), mapped_mem[i]) << "Mapped memory operation failed.";
  }
}

TEST(BufferTest, HostWritableDeviceBufferMemoryMappingTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<float>({zivc::BufferUsage::kPreferDevice,
                                             zivc::BufferFlag::kSequentialWritable});
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";

  using zisc::cast;
  {
    const auto& info = device->deviceInfo();
    constexpr std::size_t max_alloc = 128ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(float);
    buffer->setSize(s);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    zivc::MappedMemory<float> mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  {
    zivc::MappedMemory<float> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = cast<float>(i + 1);
  }

  auto buffer2 = device->createBuffer<float>({zivc::BufferUsage::kPreferHost,
                                              zivc::BufferFlag::kRandomAccessible});
  buffer2->setSize(buffer->size());
  {
    auto options = buffer->createOptions();
    options.setLabel("MemoryMappingTest");
    options.requestFence(true);
    auto result = zivc::copy(*buffer, buffer2.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<float> mapped_mem = buffer2->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const float value = cast<float>(i + 1);
      ASSERT_FLOAT_EQ(value, mapped_mem[i]) << "Mapped memory operation failed.";
    }
  }
}

TEST(BufferTest, HostWritableHostBufferMemoryMappingTest)
{
  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer = device->createBuffer<float>({zivc::BufferUsage::kPreferHost,
                                             zivc::BufferFlag::kSequentialWritable});
  ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";

  using zisc::cast;
  {
    const auto& info = device->deviceInfo();
    constexpr std::size_t max_alloc = 1ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(float);
    buffer->setSize(s);
    ASSERT_TRUE(buffer->isHostWritable()) << "The buffer isn't host writable.";
    ASSERT_EQ(s, buffer->size()) << "Host buffer allocation failed.";
    zivc::MappedMemory<float> mapped_mem = buffer->mapMemory();
    ASSERT_TRUE(mapped_mem) << "Memory mapping failed.";
    ASSERT_EQ(s, mapped_mem.size()) << "Memory mapping failed.";
  }
  {
    zivc::MappedMemory<float> mapped_mem = buffer->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = cast<float>(i + 1);
  }

  auto buffer2 = device->createBuffer<float>({zivc::BufferUsage::kPreferHost,
                                              zivc::BufferFlag::kRandomAccessible});
  buffer2->setSize(buffer->size());
  {
    auto options = buffer->createOptions();
    options.setLabel("MemoryMappingTest");
    options.requestFence(true);
    auto result = zivc::copy(*buffer, buffer2.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<float> mapped_mem = buffer2->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const float value = cast<float>(i + 1);
      ASSERT_FLOAT_EQ(value, mapped_mem[i]) << "Mapped memory operation failed.";
    }
  }
}

TEST(BufferTest, CopyBufferTest)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  // Copy from host to device
  {
    auto options = buffer_d->createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyMaxAllocBufferTest)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>(zivc::BufferUsage::kPreferDevice);

  const auto& info = device->deviceInfo();
  // Allocate memories
  {
    constexpr std::size_t max_alloc = 4ull * 1024ull * 1024ull * 1024ull;
    const std::size_t alloc_size = (std::min)(info.maxAllocationSize(), max_alloc);
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  // Copy from host to device
  {
    auto options = buffer_h->createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const auto expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyBufferRangeTest)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_h->createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed.";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed.";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    const std::size_t s = buffer_h2->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyBufferRangeTest2)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_d->createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - offset);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - offset);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < buffer_h2->size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i - offset);
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyHostBufferTest)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  // Copy from host to device
  {
    auto options = buffer_h->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyHostBufferRangeTest)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_h->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_h->size() - 2 * offset);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    const std::size_t s = buffer_h2->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i);
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyHostBufferRangeTest2)
{
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
  auto buffer_h2 = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_h2->setSize(s);
    buffer_d->setSize(s);
  }
  // Initialize the source buffer
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    constexpr uint64b start = 0;
    std::iota(mapped_mem.begin(), mapped_mem.end(), start);
  }
  constexpr std::size_t offset = 10;
  constexpr uint64b v = 16;
  // Copy from host to device
  {
    auto options = buffer_h->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    options.setDestOffset(offset);
    options.setSize(buffer_h->size() - offset);
    auto result = zivc::copy(*buffer_h, buffer_d.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), v);
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("HostToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(offset);
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - offset);
    auto result = zivc::copy(*buffer_d, buffer_h2.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h2->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < buffer_h2->size(); ++i) {
      const uint64b expected = zisc::cast<uint64b>(i - offset);
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferFastInt8Test)
{
  using zisc::int8b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int8b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<int8b>({zivc::BufferUsage::kPreferHost,
                                               zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int8b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int8b v = 24;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    auto result = buffer_d->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<int8b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferFastInt16Test)
{
  using zisc::int16b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int16b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<int16b>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int16b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int16b v = 24;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    auto result = buffer_d->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<int16b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferFastInt32Test)
{
  using zisc::int32b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int32b v = 24;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    auto result = buffer_d->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<int32b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferFastRangeTest)
{
  using zisc::int32b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int32b v = 24;
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    {
      auto result = buffer_d->fill(0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      auto result = buffer_d->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillHostBufferTest)
{
  using zisc::int32b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost});
  auto buffer_h = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int32b v = 24;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    auto result = buffer_d->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<int32b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillHostBufferRangeTest)
{
  using zisc::int32b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost});
  auto buffer_h = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    static_assert(alloc_size % 4 == 0);
    const std::size_t s = alloc_size / sizeof(int32b);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  constexpr int32b v = 24;
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_d->createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    {
      auto result = buffer_d->fill(0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      auto result = buffer_d->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
  }
  // Copy from device to host
  {
    auto options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(*buffer_d, buffer_h.get(), options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
      ASSERT_TRUE(result.fence()) << "The async fill operation failed";
      result.fence().wait();
    }
  }
  {
    auto mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyBufferReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  auto reinterp_h = buffer_h->reinterp<uint16b>();
  {
    ASSERT_EQ(buffer_h->capacityInBytes(), reinterp_h.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->getParent(), reinterp_h.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->hasParent(), reinterp_h.hasParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->getOwn(), reinterp_h.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->heapIndex(), reinterp_h.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->isDebugMode(), reinterp_h.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->isDeviceLocal(), reinterp_h.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->isHostCached(), reinterp_h.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->isHostCoherent(), reinterp_h.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->isHostVisible(), reinterp_h.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->memoryResource(), reinterp_h.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->sizeInBytes(), reinterp_h.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->type(), reinterp_h.type())
        << " Reinterp adopter is wrong.";
    //ASSERT_EQ(buffer_h->id(), reinterp_h.id())
    //    << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->usage(), reinterp_h.usage())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_h->flag(), reinterp_h.flag())
        << " Reinterp adopter is wrong.";
  }
  auto reinterp_d = buffer_d->reinterp<uint16b>();
  {
    ASSERT_EQ(buffer_d->capacityInBytes(), reinterp_d.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->getParent(), reinterp_d.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->hasParent(), reinterp_d.hasParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->getOwn(), reinterp_d.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->heapIndex(), reinterp_d.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isDebugMode(), reinterp_d.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isDeviceLocal(), reinterp_d.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostCached(), reinterp_d.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostCoherent(), reinterp_d.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostVisible(), reinterp_d.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->memoryResource(), reinterp_d.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->sizeInBytes(), reinterp_d.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->type(), reinterp_d.type())
        << " Reinterp adopter is wrong.";
    //ASSERT_EQ(buffer_d->id(), reinterp_d.id())
    //    << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->usage(), reinterp_d.usage())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->flag(), reinterp_d.flag())
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
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  // Copy from host to device
  {
    auto options = reinterp_h.createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_h, &reinterp_d, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < buffer_h->size(); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyBufferReinterpReinterpTest)
{
  using zivc::uint32b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  auto reinterp_h = buffer_h->reinterp<float>();
  auto reinterp_d = buffer_d->reinterp<uint32b>();
  auto reinterp_d2 = reinterp_d.reinterp<float>();
  // Reinterp-reinterp test
  {
    ASSERT_EQ(buffer_d->capacityInBytes(), reinterp_d2.capacityInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->getParent(), reinterp_d2.getParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->hasParent(), reinterp_d2.hasParent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->getOwn(), reinterp_d2.getOwn())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->heapIndex(), reinterp_d2.heapIndex())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isDebugMode(), reinterp_d2.isDebugMode())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isDeviceLocal(), reinterp_d2.isDeviceLocal())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostCached(), reinterp_d2.isHostCached())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostCoherent(), reinterp_d2.isHostCoherent())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->isHostVisible(), reinterp_d2.isHostVisible())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->memoryResource(), reinterp_d2.memoryResource())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->sizeInBytes(), reinterp_d2.sizeInBytes())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->type(), reinterp_d2.type())
        << " Reinterp adopter is wrong.";
    //ASSERT_EQ(buffer_d->id(), reinterp_d2.id())
    //    << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->usage(), reinterp_d2.usage())
        << " Reinterp adopter is wrong.";
    ASSERT_EQ(buffer_d->flag(), reinterp_d2.flag())
        << " Reinterp adopter is wrong.";
  }

  // Initialize the source buffer
  {
    zivc::MappedMemory<float> mapped_mem = reinterp_h.mapMemory();
    std::iota(mapped_mem.begin(), mapped_mem.end(), 0.0f);
  }
  // Copy from host to device
  {
    auto options = reinterp_h.createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_h, &reinterp_d2, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = reinterp_d2.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_d2, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<float> mapped_mem = reinterp_h.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const float expected = zisc::cast<float>(i);
      ASSERT_FLOAT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < buffer_h->size(); ++i) {
      const float v0 = zisc::cast<float>(2 * i + 0);
      const float v1 = zisc::cast<float>(2 * i + 1);
      const uint64b expected =
          (zisc::cast<uint64b>(zisc::bit_cast<uint32b>(v0)) << 0) |
          (zisc::cast<uint64b>(zisc::bit_cast<uint32b>(v1)) << 32);
      ASSERT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyBufferRangeReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto reinterp_h = buffer_h->reinterp<uint16b>();
  auto reinterp_d = buffer_d->reinterp<uint16b>();

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
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  constexpr std::size_t offset = 10;
  // Copy from host to device
  {
    auto options = reinterp_h.createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(reinterp_h.size() - 2 * k * offset);
    auto result = zivc::copy(reinterp_h, &reinterp_d, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(reinterp_d.size() - 2 * k * offset);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    const std::size_t s = buffer_h->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferRangeReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferDevice});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto reinterp_h = buffer_h->reinterp<uint16b>();
  auto reinterp_d = buffer_d->reinterp<uint16b>();

  constexpr uint16b v16 = 0b0101'0101'0101'0101;
  constexpr uint64b v64 = (zisc::cast<uint64b>(v16) << 0) |
                          (zisc::cast<uint64b>(v16) << 16) |
                          (zisc::cast<uint64b>(v16) << 32) |
                          (zisc::cast<uint64b>(v16) << 48);

  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    {
      auto result = reinterp_d.fill(0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
    options.setDestOffset(k * offset);
    options.setSize(reinterp_d.size() - 2 * k * offset);
    {
      auto result = reinterp_d.fill(v16, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    const std::size_t s = buffer_h->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyHostBufferReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  auto reinterp_h = buffer_h->reinterp<uint16b>();
  auto reinterp_d = buffer_d->reinterp<uint16b>();

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
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  // Copy from host to device
  {
    auto options = reinterp_h.createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_h, &reinterp_d, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < buffer_h->size(); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, CopyHostBufferRangeReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto reinterp_h = buffer_h->reinterp<uint16b>();
  auto reinterp_d = buffer_d->reinterp<uint16b>();

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
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i)
      mapped_mem[i] = v16[i % v16.size()];
  }
  constexpr std::size_t offset = 10;
  // Copy from host to device
  {
    auto options = reinterp_h.createOptions();
    options.setLabel("HostToDeviceCopy");
    options.requestFence(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(reinterp_h.size() - 2 * k * offset);
    auto result = zivc::copy(reinterp_h, &reinterp_d, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    zivc::MappedMemory<uint16b> mapped_mem = reinterp_h.mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    options.setSourceOffset(k * offset);
    options.setDestOffset(k * offset);
    options.setSize(reinterp_d.size() - 2 * k * offset);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    const std::size_t s = buffer_h->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range copy operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillHostBufferRangeReinterpTest)
{
  using zivc::uint8b;
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost});
  auto buffer_h = device->createBuffer<uint64b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(uint64b);
    buffer_h->setSize(s);
    buffer_d->setSize(s);
  }

  constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
  auto reinterp_h = buffer_h->reinterp<uint16b>();
  auto reinterp_d = buffer_d->reinterp<uint16b>();

  constexpr uint16b v16 = 0b0101'0101'0101'0101;
  constexpr uint64b v64 = (zisc::cast<uint64b>(v16) << 0) |
                          (zisc::cast<uint64b>(v16) << 16) |
                          (zisc::cast<uint64b>(v16) << 32) |
                          (zisc::cast<uint64b>(v16) << 48);

  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("FillBuffer");
    options.requestFence(true);
    {
      auto result = reinterp_d.fill(0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
    options.setDestOffset(k * offset);
    options.setSize(reinterp_d.size() - 2 * k * offset);
    {
      auto result = reinterp_d.fill(v16, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.hasFence()) << "The async fill operation failed";
        ASSERT_TRUE(result.fence()) << "The async fill operation failed";
        result.fence().wait();
      }
    }
  }
  // Copy from device to host
  {
    auto options = reinterp_d.createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    auto result = zivc::copy(reinterp_d, &reinterp_h, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.hasFence()) << "The async copy operation failed";
      ASSERT_TRUE(result.fence()) << "The async copy operation failed";
      result.fence().wait();
    }
  }
  {
    const zivc::MappedMemory<uint64b> mapped_mem = buffer_h->mapMemory();
    const std::size_t s = buffer_h->size();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = offset; i < (s - offset); ++i) {
      constexpr uint64b expected = v64;
      ASSERT_EQ(expected, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(0, mapped_mem[i]) << "Range fill operation failed: mem[" << i << "].";
    }
  }
}

TEST(BufferTest, ResizeBufferReinterpTest)
{
  using zivc::uint16b;
  using zivc::uint64b;

  auto context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = context->queryDevice(config.deviceId());

  auto buffer_d = device->createBuffer<uint64b>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
    constexpr std::size_t n = 100;
    constexpr std::size_t s = sizeof(uint16b) * n;

    auto reinterp_d = buffer_d->reinterp<uint16b>();
    reinterp_d.setSize(n);
    ASSERT_EQ(reinterp_d.size(), n) << "Reinterp adapter is wrong.";
    ASSERT_EQ(reinterp_d.sizeInBytes(), s) << "Reinterp adapter is wrong.";
    ASSERT_EQ(buffer_d->size(), n / k) << "Reinterp adapter is wrong.";
    ASSERT_EQ(buffer_d->sizeInBytes(), s) << "Reinterp adapter is wrong.";
  }
  buffer_d->setSize(1);
  {
    constexpr std::size_t k = sizeof(uint64b) / sizeof(uint16b);
    constexpr std::size_t n = 200;
    constexpr std::size_t s = sizeof(uint16b) * n;

    auto reinterp_d = buffer_d->reinterp<uint16b>();
    reinterp_d.setSize(n);
    ASSERT_EQ(reinterp_d.size(), n) << "Reinterp adapter is wrong.";
    ASSERT_EQ(reinterp_d.sizeInBytes(), s) << "Reinterp adapter is wrong.";
    ASSERT_EQ(buffer_d->size(), n / k) << "Reinterp adapter is wrong.";
    ASSERT_EQ(buffer_d->sizeInBytes(), s) << "Reinterp adapter is wrong.";
  }
}
