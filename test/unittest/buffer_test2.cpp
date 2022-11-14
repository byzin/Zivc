/*!
  \file buffer_test2.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \note No notation.
  \attention No attention.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"

TEST(BufferTest, SharedBufferTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Creating a shared buffer
  const zivc::SharedBuffer buffer1 = device->createBuffer<int>({zivc::BufferUsage::kPreferHost});
  ASSERT_TRUE(static_cast<bool>(buffer1)) << "SharedBuffer initialization failed.";
  buffer1->setSize(1);
  ASSERT_EQ(1, buffer1->size()) << "SharedBuffer initialization failed.";
  ASSERT_EQ(1, (*buffer1).size()) << "SharedBuffer initialization failed.";
  ASSERT_EQ(1, buffer1.useCount()) << "SharedBuffer initialization failed.";

  // Casting to common type
  zivc::SharedBufferCommon common = buffer1;

  // Casting to the derived buffer type
  using SharedBufferT = std::remove_cvref_t<decltype(buffer1)>;
  const SharedBufferT buffer2{common};
  ASSERT_EQ(1, buffer2->size()) << "SharedBuffer initialization failed.";
  ASSERT_EQ(1, (*buffer2).size()) << "SharedBuffer initialization failed.";
  ASSERT_EQ(3, buffer2.useCount()) << "SharedBuffer initialization failed.";

  //! \todo Resolve the move ownership
  SharedBufferT buffer3{std::move(common)};
  ASSERT_EQ(1, buffer3->size()) << "SharedBuffer initialization failed.";
  ASSERT_EQ(1, (*buffer3).size()) << "SharedBuffer initialization failed.";
#if defined(Z_CLANG)
  constexpr std::size_t count_offset = 1;
#else // Z_CLANG
  constexpr std::size_t count_offset = 0;
#endif // Z_CLANG
  ASSERT_EQ(3 + count_offset, buffer3.useCount()) << "SharedBuffer initialization failed.";

  buffer3.reset();
  ASSERT_EQ(2 + count_offset, buffer1.useCount()) << "SharedBuffer initialization failed.";
}

TEST(BufferTest, FillBufferInt8Test)
{
  using zivc::uint8b;
  using TestData = uint8b;
  static_assert(sizeof(TestData) == 1);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v = 0b01010101;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill8Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt16Test)
{
  using zivc::uint16b;
  using TestData = uint16b;
  static_assert(sizeof(TestData) == 2);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v = 0b01010101'01010101;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill16Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), 0);
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt24Test)
{
  using zivc::uint8b;
  using TestData = std::array<uint8b, 3>;
  static_assert(sizeof(TestData) == 3);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101, 0b10101010, 0b11110000}};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill24Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), TestData{0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt24RangeTest)
{
  using zivc::uint8b;
  using TestData = std::array<uint8b, 3>;
  static_assert(sizeof(TestData) == 3);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101, 0b10101010, 0b11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill24Buffer");
    options.requestFence(true);
    {
      const zivc::LaunchResult result = buffer_d->fill(v0, options);
      device->waitForCompletion(result.fence());
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      const zivc::LaunchResult result = buffer_d->fill(v, options);
      device->waitForCompletion(result.fence());
    }
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt48Test)
{
  using zivc::uint16b;
  using TestData = std::array<uint16b, 3>;
  static_assert(sizeof(TestData) == 6);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill48Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), TestData{0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt48RangeTest)
{
  using zivc::uint16b;
  using TestData = std::array<uint16b, 3>;
  static_assert(sizeof(TestData) == 6);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill48Buffer");
    options.requestFence(true);
    {
      const zivc::LaunchResult result = buffer_d->fill(v0, options);
      device->waitForCompletion(result.fence());
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      const zivc::LaunchResult result = buffer_d->fill(v, options);
      device->waitForCompletion(result.fence());
    }
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt64Test)
{
  using zivc::int32b;
  using zivc::cl_int2;
  static_assert(sizeof(cl_int2) == 8);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(cl_int2);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const cl_int2 v{0b01010101'01010101, 0b10101010'10101010};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill64Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<cl_int2> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), cl_int2{0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<cl_int2> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt96Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 3>;
  static_assert(sizeof(TestData) == 12);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill96Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), TestData{0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt96RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 3>;
  static_assert(sizeof(TestData) == 12);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill96Buffer");
    options.requestFence(true);
    {
      const zivc::LaunchResult result = buffer_d->fill(v0, options);
      device->waitForCompletion(result.fence());
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      const zivc::LaunchResult result = buffer_d->fill(v, options);
      device->waitForCompletion(result.fence());
    }
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt128Test)
{
  using zivc::int32b;
  using zivc::cl_int4;
  static_assert(sizeof(cl_int4) == 16);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(cl_int4);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const cl_int4 v{0b01010101'01010101, 0b10101010'10101010,
                  0b11110000'11110000, 0b11111111'00000000};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill128Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<cl_int4> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), cl_int4{0, 0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<cl_int4> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt192Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 6>;
  static_assert(sizeof(TestData) == 24);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{
    {0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
     0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000}};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill192Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), TestData{0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt192RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 6>;
  static_assert(sizeof(TestData) == 24);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{
    {0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
     0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000}};
  const TestData v0{0, 0, 0, 0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill192Buffer");
    options.requestFence(true);
    {
      const zivc::LaunchResult result = buffer_d->fill(v0, options);
      device->waitForCompletion(result.fence());
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      const zivc::LaunchResult result = buffer_d->fill(v, options);
      device->waitForCompletion(result.fence());
    }
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt384Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 12>;
  static_assert(sizeof(TestData) == 48);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{
      {0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
       0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000,
       0b10'01010101'01010101, 0b10'10101010'10101010, 0b10'11110000'11110000,
       0b01'01010101'01010101, 0b01'10101010'10101010, 0b01'11110000'11110000}};
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill384Buffer");
    options.requestFence(true);
    const zivc::LaunchResult result = buffer_d->fill(v, options);
    device->waitForCompletion(result.fence());
  }
  {
    zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    std::fill_n(mapped_mem.begin(), mapped_mem.size(), TestData{0, 0, 0});
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}

TEST(BufferTest, FillBufferInt384RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 12>;
  static_assert(sizeof(TestData) == 48);

  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  const zivc::SharedBuffer buffer_d = device->createBuffer<TestData>({zivc::BufferUsage::kPreferDevice});
  const zivc::SharedBuffer buffer_h = device->createBuffer<TestData>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_d->setSize(s);
    buffer_h->setSize(s);
  }
  const TestData v{
      {0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
       0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000,
       0b10'01010101'01010101, 0b10'10101010'10101010, 0b10'11110000'11110000,
       0b01'01010101'01010101, 0b01'10101010'10101010, 0b01'11110000'11110000}};
  const TestData v0{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("Fill384Buffer");
    options.requestFence(true);
    {
      const zivc::LaunchResult result = buffer_d->fill(v0, options);
      device->waitForCompletion(result.fence());
    }
    options.setDestOffset(offset);
    options.setSize(buffer_d->size() - 2 * offset);
    {
      const zivc::LaunchResult result = buffer_d->fill(v, options);
      device->waitForCompletion(result.fence());
    }
  }
  // Copy from device to host
  {
    zivc::BufferLaunchOptions options = buffer_d->createOptions();
    options.setLabel("DeviceToHostCopy");
    options.requestFence(true);
    const zivc::LaunchResult result = zivc::copy(*buffer_d, buffer_h.get(), options);
    device->waitForCompletion(result.fence());
  }
  {
    const zivc::MappedMemory<TestData> mapped_mem = buffer_h->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Fill buffer operation failed. mem[" << i << "].";
    }
  }
}
