/*!
  \file buffer_test2.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \note No notation.
  \attention No attention.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstddef>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/math/unit_multiple.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/cppcl/vector.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"

TEST(BufferTest, FillBufferInt8Test)
{
  using zivc::uint8b;
  using TestData = uint8b;
  static_assert(sizeof(TestData) == 1);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 1;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v = 0b01010101;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill8Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt16Test)
{
  using zivc::uint16b;
  using TestData = uint16b;
  static_assert(sizeof(TestData) == 2);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull + 2;
    static_assert(alloc_size % 4 != 0);
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v = 0b01010101'01010101;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill16Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt24Test)
{
  using zivc::uint8b;
  using TestData = std::array<uint8b, 3>;
  static_assert(sizeof(TestData) == 3);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101, 0b10101010, 0b11110000}};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill8Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt24RangeTest)
{
  using zivc::uint8b;
  using TestData = std::array<uint8b, 3>;
  static_assert(sizeof(TestData) == 3);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101, 0b10101010, 0b11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill8Buffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(v0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt48Test)
{
  using zivc::uint16b;
  using TestData = std::array<uint16b, 3>;
  static_assert(sizeof(TestData) == 6);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill16Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt48RangeTest)
{
  using zivc::uint16b;
  using TestData = std::array<uint16b, 3>;
  static_assert(sizeof(TestData) == 6);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill8Buffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(v0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt64Test)
{
  using zivc::int32b;
  using zivc::cl::int2;
  static_assert(sizeof(int2) == 8);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int2>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int2>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int2);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const int2 v{0b01010101'01010101, 0b10101010'10101010};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill64Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const auto result = v == mapped_mem[i];
      const bool flag = result.x && result.y;
      ASSERT_TRUE(flag) <<"Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt96Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 3>;
  static_assert(sizeof(TestData) == 12);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill32Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt96RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 3>;
  static_assert(sizeof(TestData) == 12);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000}};
  const TestData v0{0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill32Buffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(v0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt128Test)
{
  using zivc::int32b;
  using zivc::cl::int4;

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<int4>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<int4>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(int4);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const int4 v{0b01010101'01010101, 0b10101010'10101010,
               0b11110000'11110000, 0b11111111'00000000};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill128Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      const auto result = v == mapped_mem[i];
      const bool flag = result.x && result.y && result.z && result.w;
      ASSERT_TRUE(flag) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt192Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 6>;
  static_assert(sizeof(TestData) == 24);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
                    0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000}};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill64Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt192RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 6>;
  static_assert(sizeof(TestData) == 24);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
                    0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000}};
  const TestData v0{0, 0, 0, 0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill64Buffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(v0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt384Test)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 12>;
  static_assert(sizeof(TestData) == 48);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
                    0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000,
                    0b10'01010101'01010101, 0b10'10101010'10101010, 0b10'11110000'11110000,
                    0b01'01010101'01010101, 0b01'10101010'10101010, 0b01'11110000'11110000}};
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill128Buffer");
    options.setExternalSyncMode(true);
    auto result = buffer_device->fill(v, options);
    if (result.isAsync()) {
      ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
      device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < mapped_mem.size(); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}

TEST(BufferTest, FillBufferInt384RangeTest)
{
  using zivc::int32b;
  using TestData = std::array<int32b, 12>;
  static_assert(sizeof(TestData) == 48);

  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->makeDevice(config.deviceId());

  auto buffer_device = device->makeBuffer<TestData>(zivc::BufferUsage::kDeviceOnly);
  auto buffer_host = device->makeBuffer<TestData>(zivc::BufferUsage::kHostOnly);

  // Allocate memories
  {
    constexpr std::size_t alloc_size = 16ull * 1024ull * 1024ull;
    const std::size_t s = alloc_size / sizeof(TestData);
    buffer_device->setSize(s);
    buffer_host->setSize(s);
  }
  const TestData v{{0b01010101'01010101, 0b10101010'10101010, 0b11110000'11110000,
                    0b11'01010101'01010101, 0b11'10101010'10101010, 0b11'11110000'11110000,
                    0b10'01010101'01010101, 0b10'10101010'10101010, 0b10'11110000'11110000,
                    0b01'01010101'01010101, 0b01'10101010'10101010, 0b01'11110000'11110000}};
  const TestData v0{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  constexpr std::size_t offset = 10;
  // Fill buffer test
  {
    auto options = buffer_device->makeOptions();
    options.setLabel("Fill128Buffer");
    options.setExternalSyncMode(true);
    {
      auto result = buffer_device->fill(v0, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
      }
    }
    options.setDestOffset(offset);
    options.setSize(buffer_device->size() - 2 * offset);
    {
      auto result = buffer_device->fill(v, options);
      if (result.isAsync()) {
        ASSERT_TRUE(result.fence()) << "The result of the filling is wrong.";
        device->waitForCompletion(result.fence());
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
      device->waitForCompletion(result.fence());
    }
  }
  {
    auto mapped_mem = buffer_host->mapMemory();
    for (std::size_t i = 0; i < offset; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
    const std::size_t s = mapped_mem.size();
    for (std::size_t i = offset; i < (s - offset); ++i) {
      ASSERT_EQ(v, mapped_mem[i]) << "Filling buffer failed.";
    }
    for (std::size_t i = s - offset; i < s; ++i) {
      ASSERT_EQ(v0, mapped_mem[i]) << "Filling buffer failed.";
    }
  }
}
