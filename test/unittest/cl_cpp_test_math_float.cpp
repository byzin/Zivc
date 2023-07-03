/*!
  \file cl_cpp_test_math_float.cpp
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
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <numbers>
#include <random>
#include <span>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/binary_serializer.hpp"
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/math_test.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_math_float.hpp"

namespace {

template <std::size_t kN, typename KernelInitParam>
void testFmod(const KernelInitParam& kernel_params,
              const std::string_view label,
              const std::string_view func_name) 
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  std::ifstream reference_file{"resources/math_fmodf_reference.bin", std::ios_base::binary};
  zivc::uint32b n_num = 0;
  zisc::BSerializer::read(&n_num, &reference_file);

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector<cl_float> x1_list;
  x1_list.resize(n_num);
  std::vector<cl_float> x2_list;
  x2_list.resize(n_num);
  {
    std::vector<ztest::V2<float>> tmp{};
    tmp.resize(n_num);
    zisc::BSerializer::read(tmp.data(), &reference_file, sizeof(tmp[0]) * n_num);
    for (std::size_t i = 0; i < tmp.size(); ++i) {
      x1_list[i] = tmp[i].x_;
      x2_list[i] = tmp[i].y_;
    }
  }
  const zivc::uint32b n = x1_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x1_list.begin(), x1_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x2_list.begin(), x2_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  std::vector<cl_float> expected_list{};
  expected_list.resize(x1_list.size());
  zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(cl_float) * n_num);

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testFma(const KernelInitParam& kernel_params,
             const std::string_view label,
             const std::string_view func_name) 
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  std::ifstream reference_file{"resources/math_fmaf_reference.bin", std::ios_base::binary};
  zivc::uint32b n_num = 0;
  zisc::BSerializer::read(&n_num, &reference_file);

  const auto is_subnormal = [](const float x) noexcept
  {
    return !(std::isnormal(x) || std::isinf(x) || std::isnan(x));
  };

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector<cl_float> x1_list;
  x1_list.resize(n_num);
  std::vector<cl_float> x2_list;
  x2_list.resize(n_num);
  std::vector<cl_float> x3_list;
  x3_list.resize(n_num);
  {
    std::vector<ztest::V3<float>> tmp{};
    tmp.resize(n_num);
    zisc::BSerializer::read(tmp.data(), &reference_file, sizeof(tmp[0]) * n_num);
    for (std::size_t i = 0; i < tmp.size(); ++i) {
      x1_list[i] = tmp[i].x_;
      x2_list[i] = tmp[i].y_;
      x3_list[i] = tmp[i].z_;
    }
  }
  const zivc::uint32b n = x1_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x1_list.begin(), x1_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x2_list.begin(), x2_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x3_list.begin(), x3_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  std::vector<cl_float> expected_list{};
  expected_list.resize(x1_list.size());
  zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(cl_float) * n_num);

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (is_subnormal(expected_list[i]))
        continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testFrLdexp(const KernelInitParam& kernel_params,
                 const std::string_view label,
                 const std::string_view func_name) 
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadAllXList<cl_float>();
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(vector_size * n);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(vector_size * n);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out1, *buffer_out2, *buffer_out3, n, launch_options);
  device->waitForCompletion(result.fence());

  struct V3
  {
    cl_float x_;
    cl_int y_;
    cl_float z_;
  };

  std::vector<V3> expected_list{};
  expected_list.resize(x_list.size());
  std::ifstream reference_file{"resources/math_frldexpf_reference.bin", std::ios_base::binary};
  zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(V3) * n);

  // output1
  {
    const zivc::SharedBuffer tmp1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, tmp1.get());
    const zivc::MappedMemory mem1 = tmp1->mapMemory();

    const zivc::SharedBuffer tmp2 = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, tmp2.get());
    const zivc::MappedMemory mem2 = tmp2->mapMemory();

    const zivc::SharedBuffer tmp3 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, tmp3.get());
    const zivc::MappedMemory mem3 = tmp3->mapMemory();

    ztest::MathTestResult result1{};
    ztest::MathTestResult result3{};
    for (std::size_t i = 0; i < mem1.size(); ++i) {
      ztest::test(expected_list[i].x_, mem1[i], &result1);
      ASSERT_EQ(expected_list[i].y_, mem2[i]);
      ztest::test(expected_list[i].z_, mem3[i], &result3);
    }
    result1.print();
    result1.checkError(func_name);
    std::cout << std::endl;
    result3.print();
    result3.checkError(func_name);
  }
}

ztest::V2<std::vector<float>> generateF2List()
{
  ztest::V2<std::vector<float>> v{};
  constexpr std::size_t n = 1000'000;
  v.x_.resize(n);
  v.y_.resize(n);
  std::mt19937_64 engine{123'456'789};
  constexpr auto k = static_cast<float>(std::numeric_limits<zivc::uint16b>::max());
  std::uniform_real_distribution<float> sampler{-k, k};
  for (std::size_t i = 0; i < n; ++i) {
    v.x_[i] = sampler(engine);
    v.y_[i] = sampler(engine);
  }
  return v;
}

ztest::V3<std::vector<float>> generateF3List()
{
  ztest::V3<std::vector<float>> v{};
  constexpr std::size_t n = 1000'000;
  v.x_.resize(n);
  v.y_.resize(n);
  v.z_.resize(n);
  std::mt19937_64 engine{123'456'789};
  constexpr auto k = static_cast<float>(std::numeric_limits<zivc::uint16b>::max());
  std::uniform_real_distribution<float> sampler{-k, k};
  for (std::size_t i = 0; i < n; ++i) {
    v.x_[i] = sampler(engine);
    v.y_[i] = sampler(engine);
    v.z_[i] = sampler(engine);
  }
  return v;
}

} /* namespace */

TEST(ClCppTest, MathImplFabsV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV1TestKernel, 1);
  ztest::testF1Func<1, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV1TestKernel",
                              "fabsV1");
}

TEST(ClCppTest, MathImplFabsV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV2TestKernel, 1);
  ztest::testF1Func<2, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV2TestKernel",
                              "fabsV2");
}

TEST(ClCppTest, MathImplFabsV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV3TestKernel, 1);
  ztest::testF1Func<3, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV3TestKernel",
                              "fabsV3");
}

TEST(ClCppTest, MathImplFabsV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV4TestKernel, 1);
  ztest::testF1Func<4, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV4TestKernel",
                              "fabsV4");
}

TEST(ClCppTest, MathImplFabsV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV8TestKernel, 1);
  ztest::testF1Func<8, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV8TestKernel",
                              "fabsV8");
}

TEST(ClCppTest, MathImplFabsV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fabsV16TestKernel, 1);
  ztest::testF1Func<16, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::abs(x);},
                              "fabsV16TestKernel",
                              "fabsV16");
}

TEST(ClCppTest, MathImplFmodV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV1TestKernel, 1);
  ::testFmod<1>(kernel_params, "fmodV1TestKernel", "fmodV1");
}

TEST(ClCppTest, MathImplFmodV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV2TestKernel, 1);
  ::testFmod<2>(kernel_params, "fmodV2TestKernel", "fmodV2");
}

TEST(ClCppTest, MathImplFmodV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV3TestKernel, 1);
  ::testFmod<3>(kernel_params, "fmodV3TestKernel", "fmodV3");
}

TEST(ClCppTest, MathImplFmodV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV4TestKernel, 1);
  ::testFmod<4>(kernel_params, "fmodV4TestKernel", "fmodV4");
}

TEST(ClCppTest, MathImplFmodV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV8TestKernel, 1);
  ::testFmod<8>(kernel_params, "fmodV8TestKernel", "fmodV8");
}

TEST(ClCppTest, MathImplFmodV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmodV16TestKernel, 1);
  ::testFmod<16>(kernel_params, "fmodV16TestKernel", "fmodV16");
}

TEST(ClCppTest, MathImplRemainderV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV1TestKernel",
                              "remainderV1");
}

TEST(ClCppTest, MathImplRemainderV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV2TestKernel",
                              "remainderV2");
}

TEST(ClCppTest, MathImplRemainderV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV3TestKernel",
                              "remainderV3");
}

TEST(ClCppTest, MathImplRemainderV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV4TestKernel",
                              "remainderV4");
}

TEST(ClCppTest, MathImplRemainderV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV8TestKernel",
                              "remainderV8");
}

TEST(ClCppTest, MathImplRemainderV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, remainderV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::remainder(x, y);},
                              "remainderV16TestKernel",
                              "remainderV16");
}

TEST(ClCppTest, MathImplMadV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV1TestKernel, 1);
  ztest::testF3Func<1, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV1TestKernel",
                              "madV1");
}

TEST(ClCppTest, MathImplMadV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV2TestKernel, 1);
  ztest::testF3Func<2, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV2TestKernel",
                              "madV2");
}

TEST(ClCppTest, MathImplMadV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV3TestKernel, 1);
  ztest::testF3Func<3, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV3TestKernel",
                              "madV3");
}

TEST(ClCppTest, MathImplMadV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV4TestKernel, 1);
  ztest::testF3Func<4, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV4TestKernel",
                              "madV4");
}

TEST(ClCppTest, MathImplMadV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV8TestKernel, 1);
  ztest::testF3Func<8, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV8TestKernel",
                              "madV8");
}

TEST(ClCppTest, MathImplMadV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, madV16TestKernel, 1);
  ztest::testF3Func<16, float>(kernel_params,
                              ::generateF3List,
                              [](const float x, const float y, const float z){return x * y + z;},
                              "madV16TestKernel",
                              "madV16");
}

TEST(ClCppTest, MathImplFmaV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV1TestKernel, 1);
  ::testFma<1>(kernel_params, "fmaV1TestKernel", "fmaV1");
}

TEST(ClCppTest, MathImplFmaV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV2TestKernel, 1);
  ::testFma<2>(kernel_params, "fmaV2TestKernel", "fmaV2");
}

TEST(ClCppTest, MathImplFmaV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV3TestKernel, 1);
  ::testFma<3>(kernel_params, "fmaV3TestKernel", "fmaV3");
}

TEST(ClCppTest, MathImplFmaV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV4TestKernel, 1);
  ::testFma<4>(kernel_params, "fmaV4TestKernel", "fmaV4");
}

TEST(ClCppTest, MathImplFmaV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV8TestKernel, 1);
  ::testFma<8>(kernel_params, "fmaV8TestKernel", "fmaV8");
}

TEST(ClCppTest, MathImplFmaV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaV16TestKernel, 1);
  ::testFma<16>(kernel_params, "fmaV16TestKernel", "fmaV16");
}

TEST(ClCppTest, MathImplFmaxV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV1TestKernel",
                              "fmaxV1");
}

TEST(ClCppTest, MathImplFmaxV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV2TestKernel",
                              "fmaxV2");
}

TEST(ClCppTest, MathImplFmaxV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV3TestKernel",
                              "fmaxV3");
}

TEST(ClCppTest, MathImplFmaxV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV4TestKernel",
                              "fmaxV4");
}

TEST(ClCppTest, MathImplFmaxV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV8TestKernel",
                              "fmaxV8");
}

TEST(ClCppTest, MathImplFmaxV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fmaxV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmax(x, y);},
                              "fmaxV16TestKernel",
                              "fmaxV16");
}

TEST(ClCppTest, MathImplFminV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV1TestKernel",
                              "fminV1");
}

TEST(ClCppTest, MathImplFminV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV2TestKernel",
                              "fminV2");
}

TEST(ClCppTest, MathImplFminV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV3TestKernel",
                              "fminV3");
}

TEST(ClCppTest, MathImplFminV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV4TestKernel",
                              "fminV4");
}

TEST(ClCppTest, MathImplFminV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV8TestKernel",
                              "fminV8");
}

TEST(ClCppTest, MathImplFminV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fminV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fmin(x, y);},
                              "fminV16TestKernel",
                              "fminV16");
}

TEST(ClCppTest, MathImplMaxmagV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV1TestKernel",
                              "maxmagV1");
}

TEST(ClCppTest, MathImplMaxmagV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV2TestKernel",
                              "maxmagV2");
}

TEST(ClCppTest, MathImplMaxmagV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV3TestKernel",
                              "maxmagV3");
}

TEST(ClCppTest, MathImplMaxmagV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV4TestKernel",
                              "maxmagV4");
}

TEST(ClCppTest, MathImplMaxmagV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV8TestKernel",
                              "maxmagV8");
}

TEST(ClCppTest, MathImplMaxmagV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, maxmagV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::maxmag(x, y);},
                              "maxmagV16TestKernel",
                              "maxmagV16");
}

TEST(ClCppTest, MathImplMinmagV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV1TestKernel",
                              "minmagV1");
}

TEST(ClCppTest, MathImplMinmagV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV2TestKernel",
                              "minmagV2");
}

TEST(ClCppTest, MathImplMinmagV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV3TestKernel",
                              "minmagV3");
}

TEST(ClCppTest, MathImplMinmagV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV4TestKernel",
                              "minmagV4");
}

TEST(ClCppTest, MathImplMinmagV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV8TestKernel",
                              "minmagV8");
}

TEST(ClCppTest, MathImplMinmagV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, minmagV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return zivc::cl::minmag(x, y);},
                              "minmagV16TestKernel",
                              "minmagV16");
}

TEST(ClCppTest, MathImplFdimV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV1TestKernel",
                              "fdimV1");
}

TEST(ClCppTest, MathImplFdimV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV2TestKernel",
                              "fdimV2");
}

TEST(ClCppTest, MathImplFdimV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV3TestKernel",
                              "fdimV3");
}

TEST(ClCppTest, MathImplFdimV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV4TestKernel",
                              "fdimV4");
}

TEST(ClCppTest, MathImplFdimV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV8TestKernel",
                              "fdimV8");
}

TEST(ClCppTest, MathImplFdimV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, fdimV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::fdim(x, y);},
                              "fdimV16TestKernel",
                              "fdimV16");
}

TEST(ClCppTest, MathImplCeilV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_ceilf_reference.bin",
                          "ceilV1TestKernel",
                          "ceilV1");
}

TEST(ClCppTest, MathImplCeilV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_ceilf_reference.bin",
                          "ceilV2TestKernel",
                          "ceilV2");
}

TEST(ClCppTest, MathImplCeilV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_ceilf_reference.bin",
                          "ceilV3TestKernel",
                          "ceilV3");
}

TEST(ClCppTest, MathImplCeilV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_ceilf_reference.bin",
                          "ceilV4TestKernel",
                          "ceilV4");
}

TEST(ClCppTest, MathImplCeilV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_ceilf_reference.bin",
                          "ceilV8TestKernel",
                          "ceilV8");
}

TEST(ClCppTest, MathImplCeilV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ceilV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadAllXList<float>,
                           "resources/math_ceilf_reference.bin",
                           "ceilV16TestKernel",
                           "ceilV16");
}

TEST(ClCppTest, MathImplFloorV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_floorf_reference.bin",
                          "floorV1TestKernel",
                          "floorV1");
}

TEST(ClCppTest, MathImplFloorV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_floorf_reference.bin",
                          "floorV2TestKernel",
                          "floorV2");
}

TEST(ClCppTest, MathImplFloorV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_floorf_reference.bin",
                          "floorV3TestKernel",
                          "floorV3");
}

TEST(ClCppTest, MathImplFloorV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_floorf_reference.bin",
                          "floorV4TestKernel",
                          "floorV4");
}

TEST(ClCppTest, MathImplFloorV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_floorf_reference.bin",
                          "floorV8TestKernel",
                          "floorV8");
}

TEST(ClCppTest, MathImplFloorV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, floorV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadAllXList<float>,
                           "resources/math_floorf_reference.bin",
                           "floorV16TestKernel",
                           "floorV16");
}

TEST(ClCppTest, MathImplTruncV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_truncf_reference.bin",
                          "truncV1TestKernel",
                          "truncV1");
}

TEST(ClCppTest, MathImplTruncV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_truncf_reference.bin",
                          "truncV2TestKernel",
                          "truncV2");
}

TEST(ClCppTest, MathImplTruncV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_truncf_reference.bin",
                          "truncV3TestKernel",
                          "truncV3");
}

TEST(ClCppTest, MathImplTruncV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_truncf_reference.bin",
                          "truncV4TestKernel",
                          "truncV4");
}

TEST(ClCppTest, MathImplTruncV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_truncf_reference.bin",
                          "truncV8TestKernel",
                          "truncV8");
}

TEST(ClCppTest, MathImplTruncV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, truncV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadAllXList<float>,
                           "resources/math_truncf_reference.bin",
                           "truncV16TestKernel",
                           "truncV16");
}

TEST(ClCppTest, MathImplRoundV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_roundf_reference.bin",
                          "roundV1TestKernel",
                          "roundV1");
}

TEST(ClCppTest, MathImplRoundV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_roundf_reference.bin",
                          "roundV2TestKernel",
                          "roundV2");
}

TEST(ClCppTest, MathImplRoundV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_roundf_reference.bin",
                          "roundV3TestKernel",
                          "roundV3");
}

TEST(ClCppTest, MathImplRoundV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_roundf_reference.bin",
                          "roundV4TestKernel",
                          "roundV4");
}

TEST(ClCppTest, MathImplRoundV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_roundf_reference.bin",
                          "roundV8TestKernel",
                          "roundV8");
}

TEST(ClCppTest, MathImplRoundV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, roundV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadAllXList<float>,
                           "resources/math_roundf_reference.bin",
                           "roundV16TestKernel",
                           "roundV16");
}

TEST(ClCppTest, MathImplRintV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_rintf_reference.bin",
                          "rintV1TestKernel",
                          "rintV1");
}

TEST(ClCppTest, MathImplRintV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_rintf_reference.bin",
                          "rintV2TestKernel",
                          "rintV2");
}

TEST(ClCppTest, MathImplRintV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_rintf_reference.bin",
                          "rintV3TestKernel",
                          "rintV3");
}

TEST(ClCppTest, MathImplRintV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_rintf_reference.bin",
                          "rintV4TestKernel",
                          "rintV4");
}

TEST(ClCppTest, MathImplRintV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_rintf_reference.bin",
                          "rintV8TestKernel",
                          "rintV8");
}

TEST(ClCppTest, MathImplRintV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, rintV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadAllXList<float>,
                           "resources/math_rintf_reference.bin",
                           "rintV16TestKernel",
                           "rintV16");
}

//TEST(ClCppTest, MathImplFrLdexpV1Test)
//{
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, frLdexpV1TestKernel, 1);
//  ::testFrLdexp<1>(kernel_params, "frldexpV1TestKernel", "frldexpV1");
//}
//
//TEST(ClCppTest, MathImplFrLdexpV2Test)
//{
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, frLdexpV2TestKernel, 1);
//  ::testFrLdexp<2>(kernel_params, "frldexpV2TestKernel", "frldexpV2");
//}

TEST(ClCppTest, MathImplIlogbV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV1TestKernel, 1);
  ztest::testF1Func<1, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV1TestKernel",
                              "ilogbV1");
}

TEST(ClCppTest, MathImplIlogbV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV2TestKernel, 1);
  ztest::testF1Func<2, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV2TestKernel",
                              "ilogbV2");
}

TEST(ClCppTest, MathImplIlogbV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV3TestKernel, 1);
  ztest::testF1Func<3, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV3TestKernel",
                              "ilogbV3");
}

TEST(ClCppTest, MathImplIlogbV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV4TestKernel, 1);
  ztest::testF1Func<4, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV4TestKernel",
                              "ilogbV4");
}

TEST(ClCppTest, MathImplIlogbV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV8TestKernel, 1);
  ztest::testF1Func<8, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV8TestKernel",
                              "ilogbV8");
}

TEST(ClCppTest, MathImplIlogbV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, ilogbV16TestKernel, 1);
  ztest::testF1Func<16, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "ilogbV16TestKernel",
                              "ilogbV16");
}

TEST(ClCppTest, MathImplLobgV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV1TestKernel, 1);
  ztest::testF1Func<1, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV1TestKernel",
                              "logbV1");
}

TEST(ClCppTest, MathImplLobgV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV2TestKernel, 1);
  ztest::testF1Func<2, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV2TestKernel",
                              "logbV2");
}

TEST(ClCppTest, MathImplLobgV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV3TestKernel, 1);
  ztest::testF1Func<3, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV3TestKernel",
                              "logbV3");
}

TEST(ClCppTest, MathImplLobgV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV4TestKernel, 1);
  ztest::testF1Func<4, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV4TestKernel",
                              "logbV4");
}

TEST(ClCppTest, MathImplLobgV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV8TestKernel, 1);
  ztest::testF1Func<8, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV8TestKernel",
                              "logbV8");
}

TEST(ClCppTest, MathImplLobgV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, logbV16TestKernel, 1);
  ztest::testF1Func<16, float>(kernel_params,
                              ztest::loadAllXList<float>,
                              [](const float x){return std::logb(x);},
                              "logbV16TestKernel",
                              "logbV16");
}

TEST(ClCppTest, MathImplNextafterV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV1TestKernel",
                              "nextafterV1");
}

TEST(ClCppTest, MathImplNextafterV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV2TestKernel",
                              "nextafterV2");
}

TEST(ClCppTest, MathImplNextafterV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV3TestKernel",
                              "nextafterV3");
}

TEST(ClCppTest, MathImplNextafterV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV4TestKernel",
                              "nextafterV4");
}

TEST(ClCppTest, MathImplNextafterV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV8TestKernel",
                              "nextafterV8");
}

TEST(ClCppTest, MathImplNextafterV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, nextafterV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::nextafter(x, y);},
                              "nextafterV16TestKernel",
                              "nextafterV16");
}

TEST(ClCppTest, MathImplCopysignV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV1TestKernel, 1);
  ztest::testF2Func<1, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV1TestKernel",
                              "copysignV1");
}

TEST(ClCppTest, MathImplCopysignV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV2TestKernel, 1);
  ztest::testF2Func<2, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV2TestKernel",
                              "copysignV2");
}

TEST(ClCppTest, MathImplCopysignV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV3TestKernel, 1);
  ztest::testF2Func<3, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV3TestKernel",
                              "copysignV3");
}

TEST(ClCppTest, MathImplCopysignV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV4TestKernel, 1);
  ztest::testF2Func<4, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV4TestKernel",
                              "copysignV4");
}

TEST(ClCppTest, MathImplCopysignV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV8TestKernel, 1);
  ztest::testF2Func<8, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV8TestKernel",
                              "copysignV8");
}

TEST(ClCppTest, MathImplCopysignV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_float, copysignV16TestKernel, 1);
  ztest::testF2Func<16, float>(kernel_params,
                              ::generateF2List,
                              [](const float x, const float y){return std::copysign(x, y);},
                              "copysignV16TestKernel",
                              "copysignV16");
}
