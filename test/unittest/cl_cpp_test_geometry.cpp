/*!
  \file cl_cpp_test_geometry.cpp
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
#include <initializer_list>
#include <limits>
#include <numbers>
#include <random>
#include <span>
#include <type_traits>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_geometry.hpp"

TEST(ClCppTest, GeometryImplCrossV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, crossV3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("crossV3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float3 expected = zivc::cl::cross(a, b);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("crossv3");
  }
}

TEST(ClCppTest, GeometryFallbackCrossV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, crossV3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("crossV3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float3 expected = zivc::cl::cross(a, b);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("crossv3Fallback");
  }
}

TEST(ClCppTest, GeometryImplCrossV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        2.0f};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, crossV4TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("crossV4TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float4 expected = zivc::cl::cross(a, b);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("crossv4");
  }
}

TEST(ClCppTest, GeometryFallbackCrossV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        2.0f};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, crossV4FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("crossV4FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float4 expected = zivc::cl::cross(a, b);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("crossv4Fallback");
  }
}

TEST(ClCppTest, GeometryImplDotV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV2TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV2TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2& a = in[2 * i];
      const cl_float2& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv2");
  }
}

TEST(ClCppTest, GeometryFallbackDotV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV2FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV2FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2& a = in[2 * i];
      const cl_float2& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv2Fallback");
  }
}

TEST(ClCppTest, GeometryImplDotV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv3");
  }
}

TEST(ClCppTest, GeometryFallbackDotV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv3Fallback");
  }
}

TEST(ClCppTest, GeometryImplDotV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV4TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV4TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv4");
  }
}

TEST(ClCppTest, GeometryFallbackDotV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV4FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV4FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv4Fallback");
  }
}

//TEST(ClCppTest, GeometryImplDotV8Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(2 * n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine),
//                        k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV8TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("dotV8TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float8& a = in[2 * i];
//      const cl_float8& b = in[2 * i + 1];
//      const cl_float expected = zivc::cl::dot(a, b);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("dotv8");
//  }
//}

TEST(ClCppTest, GeometryFallbackDotV8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine),
                        k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV8FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV8FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float8& a = in[2 * i];
      const cl_float8& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv8Fallback");
  }
}

//TEST(ClCppTest, GeometryImplDotV16Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//  using zivc::cl_float16;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(2 * n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV16TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("dotV16TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float16& a = in[2 * i];
//      const cl_float16& b = in[2 * i + 1];
//      const cl_float expected = zivc::cl::dot(a, b);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("dotv16");
//  }
//}

TEST(ClCppTest, GeometryFallbackDotV16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV16FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV16FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float16& a = in[2 * i];
      const cl_float16& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv16Fallback");
  }
}

TEST(ClCppTest, GeometryImplDotV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV4C3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV4C3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[2 * i];
      a.w = 0.0f;
      cl_float4 b = in[2 * i + 1];
      b.w = 0.0f;
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv4c3");
  }
}

TEST(ClCppTest, GeometryFallbackDotV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, dotV4C3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("dotV4C3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[2 * i];
      a.w = 0.0f;
      cl_float4 b = in[2 * i + 1];
      b.w = 0.0f;
      const cl_float expected = zivc::cl::dot(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("dotv4c3Fallback");
  }
}

TEST(ClCppTest, GeometryImplDistanceV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV2TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV2TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2& a = in[2 * i];
      const cl_float2& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev2");
  }
}

TEST(ClCppTest, GeometryFallbackDistanceV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV2FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV2FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2& a = in[2 * i];
      const cl_float2& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev2Fallback");
  }
}

TEST(ClCppTest, GeometryImplDistanceV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev3");
  }
}

TEST(ClCppTest, GeometryFallbackDistanceV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3& a = in[2 * i];
      const cl_float3& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev3Fallback");
  }
}

TEST(ClCppTest, GeometryImplDistanceV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV4TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV4TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev4");
  }
}

TEST(ClCppTest, GeometryFallbackDistanceV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV4FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV4FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4& a = in[2 * i];
      const cl_float4& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev4Fallback");
  }
}

//TEST(ClCppTest, GeometryImplDistanceV8Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(2 * n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine),
//                        k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV8TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("distanceV8TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float8& a = in[2 * i];
//      const cl_float8& b = in[2 * i + 1];
//      const cl_float expected = zivc::cl::distance(a, b);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("distancev8");
//  }
//}

TEST(ClCppTest, GeometryFallbackDistanceV8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine),
                        k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV8FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV8FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float8& a = in[2 * i];
      const cl_float8& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev8Fallback");
  }
}

//TEST(ClCppTest, GeometryImplDistanceV16Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//  using zivc::cl_float16;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(2 * n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV16TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("distanceV16TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float16& a = in[2 * i];
//      const cl_float16& b = in[2 * i + 1];
//      const cl_float expected = zivc::cl::distance(a, b);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("distancev16");
//  }
//}

TEST(ClCppTest, GeometryFallbackDistanceV16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV16FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV16FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float16& a = in[2 * i];
      const cl_float16& b = in[2 * i + 1];
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev16Fallback");
  }
}

TEST(ClCppTest, GeometryImplDistanceV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV4C3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV4C3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[2 * i];
      a.w = 0.0f;
      cl_float4 b = in[2 * i + 1];
      b.w = 0.0f;
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev4c3");
  }
}

TEST(ClCppTest, GeometryFallbackDistanceV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(2 * n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, distanceV4C3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("distanceV4C3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[2 * i];
      a.w = 0.0f;
      cl_float4 b = in[2 * i + 1];
      b.w = 0.0f;
      const cl_float expected = zivc::cl::distance(a, b);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("distancev4c3Fallback");
  }
}

TEST(ClCppTest, GeometryImplLengthV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV2TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV2TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length2");
  }
}

TEST(ClCppTest, GeometryFallbackLengthV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV2FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV2FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length2Fallback");
  }
}

TEST(ClCppTest, GeometryImplLengthV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length3");
  }
}

TEST(ClCppTest, GeometryFallbackLengthV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length3Fallback");
  }
}

TEST(ClCppTest, GeometryImplLengthV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV4TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV4TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length4");
  }
}

TEST(ClCppTest, GeometryFallbackLengthV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV4FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV4FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length4Fallback");
  }
}

//TEST(ClCppTest, GeometryImplLengthV8Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine),
//                        k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV8TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("lengthV8TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float expected = zivc::cl::length(in[i]);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("length8");
//  }
//}

TEST(ClCppTest, GeometryFallbackLengthV8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine),
                        k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV8FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV8FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length8Fallback");
  }
}

//TEST(ClCppTest, GeometryImplLengthV16Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//  using zivc::cl_float16;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV16TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("lengthV16TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float expected = zivc::cl::length(in[i]);
//      ztest::test(expected, mem[i], &result);
//    }
//    result.print();
//    result.checkError("length16");
//  }
//}

TEST(ClCppTest, GeometryFallbackLengthV16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV16FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV16FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float expected = zivc::cl::length(in[i]);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length16Fallback");
  }
}

TEST(ClCppTest, GeometryImplLengthV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV4C3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV4C3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[i];
      a.w = 0.0f;
      const cl_float expected = zivc::cl::length(a);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length4c3");
  }
}

TEST(ClCppTest, GeometryFallbackLengthV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, lengthV4C3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("lengthV4C3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[i];
      a.w = 0.0f;
      const cl_float expected = zivc::cl::length(a);
      ztest::test(expected, mem[i], &result);
    }
    result.print();
    result.checkError("length4c3Fallback");
  }
}

TEST(ClCppTest, GeometryImplNormalizeV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV2TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV2TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
    }
    result.print();
    result.checkError("normalize2");
  }
}

TEST(ClCppTest, GeometryFallbackNormalizeV2Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const cl_float2 v{k * sampler(engine), k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV2FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV2FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float2 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
    }
    result.print();
    result.checkError("normalize2Fallback");
  }
}

TEST(ClCppTest, GeometryImplNormalizeV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("normalize3");
  }
}

TEST(ClCppTest, GeometryFallbackNormalizeV3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float3 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float3 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("normalize3Fallback");
  }
}

TEST(ClCppTest, GeometryImplNormalizeV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV4TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV4TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
      ztest::test(expected.w, mem[i].w, &result);
    }
    result.print();
    result.checkError("normalize4");
  }
}

TEST(ClCppTest, GeometryFallbackNormalizeV4Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV4FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV4FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float4 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
      ztest::test(expected.w, mem[i].w, &result);
    }
    result.print();
    result.checkError("normalize4Fallback");
  }
}

//TEST(ClCppTest, GeometryImplNormalizeV8Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine),
//                        k * (std::sin(phi) * std::cos(theta)),
//                        k * (std::sin(phi) * std::sin(theta)),
//                        k * std::cos(phi),
//                        k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV8TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("normalizeV8TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float8 expected = zivc::cl::normalize(in[i]);
//      ztest::test(expected.s0, mem[i].s0, &result);
//      ztest::test(expected.s1, mem[i].s1, &result);
//      ztest::test(expected.s2, mem[i].s2, &result);
//      ztest::test(expected.s3, mem[i].s3, &result);
//      ztest::test(expected.s4, mem[i].s4, &result);
//      ztest::test(expected.s5, mem[i].s5, &result);
//      ztest::test(expected.s6, mem[i].s6, &result);
//      ztest::test(expected.s7, mem[i].s7, &result);
//    }
//    result.print();
//    result.checkError("normalize8");
//  }
//}

TEST(ClCppTest, GeometryFallbackNormalizeV8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float8>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float8 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine),
                        k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV8FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV8FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float8 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.s0, mem[i].s0, &result);
      ztest::test(expected.s1, mem[i].s1, &result);
      ztest::test(expected.s2, mem[i].s2, &result);
      ztest::test(expected.s3, mem[i].s3, &result);
      ztest::test(expected.s4, mem[i].s4, &result);
      ztest::test(expected.s5, mem[i].s5, &result);
      ztest::test(expected.s6, mem[i].s6, &result);
      ztest::test(expected.s7, mem[i].s7, &result);
    }
    result.print();
    result.checkError("normalize8Fallback");
  }
}

//TEST(ClCppTest, GeometryImplNormalizeV16Test)
//{
//  const zivc::SharedContext context = ztest::createContext();
//  const ztest::Config& config = ztest::Config::globalConfig();
//  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
//
//  // Allocate buffers
//  using zivc::cl_float;
//  using zivc::cl_float2;
//  using zivc::cl_float3;
//  using zivc::cl_float4;
//  using zivc::cl_float8;
//  using zivc::cl_float16;
//
//  constexpr std::size_t n = 65535 * 64;
//
//  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
//  buffer_in->setSize(n);
//  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
//  buffer_out->setSize(n);
//  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//  {
//    host_buff->setSize(buffer_in->size());
//    zivc::MappedMemory mem = host_buff->mapMemory();
//    std::mt19937_64 engine{123'456'789};
//    std::uniform_real_distribution sampler{0.0f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
//      const float k = ztest::makeNormal(x);
//      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
//      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
//      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine),
//                         k * (std::sin(phi) * std::cos(theta)),
//                         k * (std::sin(phi) * std::sin(theta)),
//                         k * std::cos(phi),
//                         k * sampler(engine)};
//      mem[i] = v;
//    }
//  }
//  ztest::copyBuffer(*host_buff, buffer_in.get());
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV16TestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{n}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("normalizeV16TestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(n, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
//  device->waitForCompletion(result.fence());
//
//  // output1
//  {
//    const zivc::SharedBuffer tmp = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
//    ztest::copyBuffer(*buffer_out, tmp.get());
//    const zivc::MappedMemory in = host_buff->mapMemory();
//    const zivc::MappedMemory mem = tmp->mapMemory();
//
//    ztest::MathTestResult result{};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const cl_float16 expected = zivc::cl::normalize(in[i]);
//      ztest::test(expected.s0, mem[i].s0, &result);
//      ztest::test(expected.s1, mem[i].s1, &result);
//      ztest::test(expected.s2, mem[i].s2, &result);
//      ztest::test(expected.s3, mem[i].s3, &result);
//      ztest::test(expected.s4, mem[i].s4, &result);
//      ztest::test(expected.s5, mem[i].s5, &result);
//      ztest::test(expected.s6, mem[i].s6, &result);
//      ztest::test(expected.s7, mem[i].s7, &result);
//      ztest::test(expected.s8, mem[i].s8, &result);
//      ztest::test(expected.s9, mem[i].s9, &result);
//      ztest::test(expected.sa, mem[i].sa, &result);
//      ztest::test(expected.sb, mem[i].sb, &result);
//      ztest::test(expected.sc, mem[i].sc, &result);
//      ztest::test(expected.sd, mem[i].sd, &result);
//      ztest::test(expected.se, mem[i].se, &result);
//      ztest::test(expected.sf, mem[i].sf, &result);
//    }
//    result.print();
//    result.checkError("normalize16");
//  }
//}

TEST(ClCppTest, GeometryFallbackNormalizeV16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float16>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float16 v{k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine),
                         k * (std::sin(phi) * std::cos(theta)),
                         k * (std::sin(phi) * std::sin(theta)),
                         k * std::cos(phi),
                         k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV16FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV16FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const cl_float16 expected = zivc::cl::normalize(in[i]);
      ztest::test(expected.s0, mem[i].s0, &result);
      ztest::test(expected.s1, mem[i].s1, &result);
      ztest::test(expected.s2, mem[i].s2, &result);
      ztest::test(expected.s3, mem[i].s3, &result);
      ztest::test(expected.s4, mem[i].s4, &result);
      ztest::test(expected.s5, mem[i].s5, &result);
      ztest::test(expected.s6, mem[i].s6, &result);
      ztest::test(expected.s7, mem[i].s7, &result);
      ztest::test(expected.s8, mem[i].s8, &result);
      ztest::test(expected.s9, mem[i].s9, &result);
      ztest::test(expected.sa, mem[i].sa, &result);
      ztest::test(expected.sb, mem[i].sb, &result);
      ztest::test(expected.sc, mem[i].sc, &result);
      ztest::test(expected.sd, mem[i].sd, &result);
      ztest::test(expected.se, mem[i].se, &result);
      ztest::test(expected.sf, mem[i].sf, &result);
    }
    result.print();
    result.checkError("normalize16Fallback");
  }
}

TEST(ClCppTest, GeometryImplNormalizeV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV4C3TestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV4C3TestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[i];
      a.w = 0.0f;
      const cl_float4 expected = zivc::cl::normalize(a);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("normalize4c3");
  }
}

TEST(ClCppTest, GeometryFallbackNormalizeV4C3Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr std::size_t n = 65535 * 64;

  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_in->setSize(n);
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(n);
  const zivc::SharedBuffer host_buff = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
  {
    host_buff->setSize(buffer_in->size());
    zivc::MappedMemory mem = host_buff->mapMemory();
    std::mt19937_64 engine{123'456'789};
    std::uniform_real_distribution sampler{0.0f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = 0.3f + 0.4f * (static_cast<float>(i) / static_cast<float>(mem.size()));
      const float k = ztest::makeNormal(x);
      const float theta = 2.0f * std::numbers::pi_v<float> * sampler(engine);
      const float phi = std::acos(1.0f - 2.0f * sampler(engine));
      const cl_float4 v{k * (std::sin(phi) * std::cos(theta)),
                        k * (std::sin(phi) * std::sin(theta)),
                        k * std::cos(phi),
                        k * sampler(engine)};
      mem[i] = v;
    }
  }
  ztest::copyBuffer(*host_buff, buffer_in.get());

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_geometry, normalizeV4C3FallbackTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel("normalizeV4C3FallbackTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory in = host_buff->mapMemory();
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      cl_float4 a = in[i];
      a.w = 0.0f;
      const cl_float4 expected = zivc::cl::normalize(a);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("normalize4c3Fallback");
  }
}
