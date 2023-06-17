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
#include <concepts>
#include <cstddef>
#include <functional>
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

namespace {

template <std::floating_point Float>
void initDotInput(const Float x,
                  std::mt19937_64& engine,
                  std::uniform_real_distribution<Float>& sampler,
                  zivc::cl::Vector<Float, 2>* v)
{
  v->x = x * sampler(engine);
  v->y = x * sampler(engine);
}

template <std::floating_point Float>
void initDotInput(const Float x,
                  std::mt19937_64& engine,
                  std::uniform_real_distribution<Float>& sampler,
                  zivc::cl::Vector<Float, 3>* v)
{
  v->x = x * sampler(engine);
  v->y = x * sampler(engine);
  v->z = x * sampler(engine);
}

template <std::floating_point Float>
void initDotInput(const Float x,
                  std::mt19937_64& engine,
                  std::uniform_real_distribution<Float>& sampler,
                  zivc::cl::Vector<Float, 4>* v)
{
  v->x = x * sampler(engine);
  v->y = x * sampler(engine);
  v->z = x * sampler(engine);
  v->w = x * sampler(engine);
}

template <std::floating_point Float>
void initDotInput(const Float x,
                  std::mt19937_64& engine,
                  std::uniform_real_distribution<Float>& sampler,
                  zivc::cl::Vector<Float, 8>* v)
{
  v->s0 = x * sampler(engine);
  v->s1 = x * sampler(engine);
  v->s2 = x * sampler(engine);
  v->s3 = x * sampler(engine);
  v->s4 = x * sampler(engine);
  v->s5 = x * sampler(engine);
  v->s6 = x * sampler(engine);
  v->s7 = x * sampler(engine);
}

template <std::floating_point Float>
void initDotInput(const Float x,
                  std::mt19937_64& engine,
                  std::uniform_real_distribution<Float>& sampler,
                  zivc::cl::Vector<Float, 16>* v)
{
  v->s0 = x * sampler(engine);
  v->s1 = x * sampler(engine);
  v->s2 = x * sampler(engine);
  v->s3 = x * sampler(engine);
  v->s4 = x * sampler(engine);
  v->s5 = x * sampler(engine);
  v->s6 = x * sampler(engine);
  v->s7 = x * sampler(engine);
  v->s8 = x * sampler(engine);
  v->s9 = x * sampler(engine);
  v->sa = x * sampler(engine);
  v->sb = x * sampler(engine);
  v->sc = x * sampler(engine);
  v->sd = x * sampler(engine);
  v->se = x * sampler(engine);
  v->sf = x * sampler(engine);
}

template <std::floating_point Float, std::size_t kN>
zivc::cl::Vector<Float, kN> makeDotInput(const Float x,
                                         std::mt19937_64& engine,
                                         std::uniform_real_distribution<Float>& sampler)
{
  const Float t = static_cast<Float>(0.2) + static_cast<Float>(0.6) * x;
  const Float k = ztest::makeNormal(t);
  zivc::cl::Vector<Float, kN> v{};
  initDotInput(k, engine, sampler, &v);
  return v;
}

template <std::floating_point Float>
void initCrossInput(const Float x,
                    std::mt19937_64& engine,
                    std::uniform_real_distribution<Float>& sampler,
                    zivc::cl::Vector<Float, 3>* v)
{
  const Float theta = static_cast<Float>(2) * std::numbers::pi_v<float> * sampler(engine);
  const Float phi = std::acos(static_cast<Float>(1) - static_cast<Float>(2) * sampler(engine));
  v->x = x * std::sin(phi) * std::cos(theta);
  v->y = x * std::sin(phi) * std::sin(theta);
  v->z = x * std::cos(phi);
}

template <std::floating_point Float>
void initCrossInput(const Float x,
                    std::mt19937_64& engine,
                    std::uniform_real_distribution<Float>& sampler,
                    zivc::cl::Vector<Float, 4>* v)
{
  const Float theta = static_cast<Float>(2) * std::numbers::pi_v<float> * sampler(engine);
  const Float phi = std::acos(static_cast<Float>(1) - static_cast<Float>(2) * sampler(engine));
  v->x = x * std::sin(phi) * std::cos(theta);
  v->y = x * std::sin(phi) * std::sin(theta);
  v->z = x * std::cos(phi);
  v->w = static_cast<Float>(2);
}

template <std::floating_point Float, std::size_t kN>
zivc::cl::Vector<Float, kN> makeCrossInput(const Float x,
                                           std::mt19937_64& engine,
                                           std::uniform_real_distribution<Float>& sampler)
{
  const Float t = static_cast<Float>(0.3) + static_cast<Float>(0.4) * x;
  const Float k = ztest::makeNormal(t);
  zivc::cl::Vector<Float, kN> v{};
  initCrossInput(k, engine, sampler, &v);
  return v;
}

template <std::floating_point Float>
bool hasSubnormalDot(const Float v)
{
  constexpr auto z = static_cast<Float>(0);
  const bool result = !(std::equal_to<Float>{}(z, v) || std::isnormal(v) || std::isinf(v) || std::isnan(v));
  return result;
}

template <std::floating_point Float>
bool hasSubnormalDot(const Float a, const Float b)
{
  const bool result = hasSubnormalDot(a * b);
  return result;
}

template <std::floating_point Float, std::size_t kN>
bool hasSubnormalCross(const zivc::cl::Vector<Float, kN>& a, const zivc::cl::Vector<Float, kN>& b)
{
  return hasSubnormalDot(a.y, b.z) || hasSubnormalDot(a.z, b.y) ||
         hasSubnormalDot(a.z, b.x) || hasSubnormalDot(a.x, b.z) ||
         hasSubnormalDot(a.x, b.y) || hasSubnormalDot(a.y, b.x);
}

template <std::floating_point Float>
bool hasSubnormalDot(const zivc::cl::Vector<Float, 2>& a, const zivc::cl::Vector<Float, 2>& b)
{
  return hasSubnormalDot(a.x, b.x) ||
         hasSubnormalDot(a.y, b.y);
}

template <std::floating_point Float>
bool hasSubnormalDot(const zivc::cl::Vector<Float, 3>& a, const zivc::cl::Vector<Float, 3>& b)
{
  return hasSubnormalDot(a.x, b.x) ||
         hasSubnormalDot(a.y, b.y) ||
         hasSubnormalDot(a.z, b.z);
}

template <std::floating_point Float>
bool hasSubnormalDot(const zivc::cl::Vector<Float, 4>& a, const zivc::cl::Vector<Float, 4>& b)
{
  return hasSubnormalDot(a.x, b.x) ||
         hasSubnormalDot(a.y, b.y) ||
         hasSubnormalDot(a.z, b.z) ||
         hasSubnormalDot(a.w, b.w);
}

template <std::floating_point Float>
bool hasSubnormalDot(const zivc::cl::Vector<Float, 8>& a, const zivc::cl::Vector<Float, 8>& b)
{
  return hasSubnormalDot(a.s0, b.s0) ||
         hasSubnormalDot(a.s1, b.s1) ||
         hasSubnormalDot(a.s2, b.s2) ||
         hasSubnormalDot(a.s3, b.s3) ||
         hasSubnormalDot(a.s4, b.s4) ||
         hasSubnormalDot(a.s5, b.s5) ||
         hasSubnormalDot(a.s6, b.s6) ||
         hasSubnormalDot(a.s7, b.s7);
}

template <std::floating_point Float>
bool hasSubnormalDot(const zivc::cl::Vector<Float, 16>& a, const zivc::cl::Vector<Float, 16>& b)
{
  return hasSubnormalDot(a.s0, b.s0) ||
         hasSubnormalDot(a.s1, b.s1) ||
         hasSubnormalDot(a.s2, b.s2) ||
         hasSubnormalDot(a.s3, b.s3) ||
         hasSubnormalDot(a.s4, b.s4) ||
         hasSubnormalDot(a.s5, b.s5) ||
         hasSubnormalDot(a.s6, b.s6) ||
         hasSubnormalDot(a.s7, b.s7) ||
         hasSubnormalDot(a.s8, b.s8) ||
         hasSubnormalDot(a.s9, b.s9) ||
         hasSubnormalDot(a.sa, b.sa) ||
         hasSubnormalDot(a.sb, b.sb) ||
         hasSubnormalDot(a.sc, b.sc) ||
         hasSubnormalDot(a.sd, b.sd) ||
         hasSubnormalDot(a.se, b.se) ||
         hasSubnormalDot(a.sf, b.sf);
}

} /* namespace */

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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeCrossInput<float, 3>(x, engine, sampler);
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
      if (hasSubnormalCross(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeCrossInput<float, 3>(x, engine, sampler);
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
      if (hasSubnormalCross(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeCrossInput<float, 4>(x, engine, sampler);
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
      if (hasSubnormalCross(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeCrossInput<float, 4>(x, engine, sampler);
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
      if (hasSubnormalCross(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
//      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
//      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
//      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
//      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, b)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
//      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
//      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, a)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
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
      if (::hasSubnormalDot(a, a)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 2>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 3>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
//      constexpr float eps = std::numeric_limits<float>::epsilon();
//      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
//      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 8>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
//    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
//    for (std::size_t i = 0; i < mem.size(); ++i) {
//      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
//      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
//      constexpr float eps = std::numeric_limits<float>::epsilon();
//      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
//      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 16>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(in[i], in[i])) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(a, a)) continue;
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
    std::uniform_real_distribution sampler{1.0e-4f, 1.0f};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      const float x = static_cast<float>(i) / static_cast<float>(mem.size());
      mem[i] = ::makeDotInput<float, 4>(x, engine, sampler);
      constexpr float eps = std::numeric_limits<float>::epsilon();
      if (zivc::cl::dot(mem[i], mem[i]) < eps) mem[i] += eps;
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
      if (::hasSubnormalDot(a, a)) continue;
      const cl_float4 expected = zivc::cl::normalize(a);
      ztest::test(expected.x, mem[i].x, &result);
      ztest::test(expected.y, mem[i].y, &result);
      ztest::test(expected.z, mem[i].z, &result);
    }
    result.print();
    result.checkError("normalize4c3Fallback");
  }
}
