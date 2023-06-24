/*!
  \file cl_cpp_test_math.cpp
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
#include <initializer_list>
#include <iostream>
#include <limits>
#include <numbers>
#include <random>
#include <span>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/math_test.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_math.hpp"

TEST(ClCppTest, MathConstantFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_double;

  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(17);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, mathConstantFloatKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("mathConstantFloatKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    std::size_t index = 0;
    {
      constexpr float expected = std::numbers::e_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kE' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::log2e_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kLog2E' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::log10e_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kLog10E' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::ln2_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kLn2' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::ln10_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kLn10' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::pi_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kPi' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::pi_v<float> / 2.0f;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'M_PI_2' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::pi_v<float> / 4.0f;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'M_PI_4' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::inv_pi_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kInvPi' isn't correct.";
    }
    {
      constexpr float expected = 2.0f * std::numbers::inv_pi_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'M_2_PI' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::inv_sqrtpi_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kInvSqrtPi' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::sqrt2_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kSqrt2' isn't correct.";
    }
    {
      constexpr float expected = 1.0f / std::numbers::sqrt2_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kInvSqrt2' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::sqrt3_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kSqrt3' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::inv_sqrt3_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kInvSqrt3' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::egamma_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kEgamma' isn't correct.";
    }
    {
      constexpr float expected = std::numbers::phi_v<float>;
      EXPECT_FLOAT_EQ(expected, mem[index++]) << "The math constant 'kPhi' isn't correct.";
    }
  }
}

#if !defined(Z_MAC)

TEST(ClCppTest, MathConstantDoubleTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_double;

  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_double>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(17);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, mathConstantDoubleKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("mathConstantDoubleKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_double>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    std::size_t index = 0;
    {
      constexpr double expected = std::numbers::e_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kE' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::log2e_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kLog2E' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::log10e_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kLog10E' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::ln2_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kLn2' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::ln10_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kLn10' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::pi_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kPi' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::pi_v<double> / 2.0;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'M_PI_2' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::pi_v<double> / 4.0;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'M_PI_4' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::inv_pi_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kInvPi' isn't correct.";
    }
    {
      constexpr double expected = 2.0 * std::numbers::inv_pi_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'M_2_PI' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::inv_sqrtpi_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kInvSqrtPi' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::sqrt2_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kSqrt2' isn't correct.";
    }
    {
      constexpr double expected = 1.0 / std::numbers::sqrt2_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kInvSqrt2' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::sqrt3_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kSqrt3' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::inv_sqrt3_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kInvSqrt3' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::egamma_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kEgamma' isn't correct.";
    }
    {
      constexpr double expected = std::numbers::phi_v<double>;
      EXPECT_DOUBLE_EQ(expected, mem[index++]) << "The math constant 'kPhi' isn't correct.";
    }
  }
}

#endif // Z_MAC

namespace {

template <std::size_t kN, typename KernelInitParam>
void testSqrt(const KernelInitParam& kernel_params,
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
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
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
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_sqrtf_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testRsqrt(const KernelInitParam& kernel_params,
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
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
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
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_sqrtf_reference.bin", x_list.size());
    std::transform(expected_list.cbegin(), expected_list.cend(), expected_list.begin(),
    [](const cl_float& x) noexcept
    {
      return 1.0f / x;
    });

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

} /* namespace */

TEST(ClCppTest, MathImplSqrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV1TestKernel, 1);
  ::testSqrt<1>(kernel_params, "sqrtV1TestKernel", "sqrtV1");
}

TEST(ClCppTest, MathFallbackSqrtV1Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV1FallbackTestKernel, 1);
  ::testSqrt<1>(kernel_params, "sqrtV1FallbackTestKernel", "sqrtV1Fallback");
}

TEST(ClCppTest, MathImplSqrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV2TestKernel, 1);
  ::testSqrt<2>(kernel_params, "sqrtV2TestKernel", "sqrtV2");
}

TEST(ClCppTest, MathFallbackSqrtV2Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV2FallbackTestKernel, 1);
  ::testSqrt<2>(kernel_params, "sqrtV2FallbackTestKernel", "sqrtV2Fallback");
}

TEST(ClCppTest, MathImplSqrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV3TestKernel, 1);
  ::testSqrt<3>(kernel_params, "sqrtV3TestKernel", "sqrtV3");
}

TEST(ClCppTest, MathFallbackSqrtV3Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV3FallbackTestKernel, 1);
  ::testSqrt<3>(kernel_params, "sqrtV3FallbackTestKernel", "sqrtV3Fallback");
}

TEST(ClCppTest, MathImplSqrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV4TestKernel, 1);
  ::testSqrt<4>(kernel_params, "sqrtV4TestKernel", "sqrtV4");
}

TEST(ClCppTest, MathFallbackSqrtV4Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV4FallbackTestKernel, 1);
  ::testSqrt<4>(kernel_params, "sqrtV4FallbackTestKernel", "sqrtV4Fallback");
}

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplSqrtV8Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV8TestKernel, 1);
//  ::testSqrt<8>(kernel_params, "sqrtV8TestKernel", "sqrtV8");
//}

TEST(ClCppTest, MathFallbackSqrtV8Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV8FallbackTestKernel, 1);
  ::testSqrt<8>(kernel_params, "sqrtV8FallbackTestKernel", "sqrtV8Fallback");
}

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplSqrtV16Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV16TestKernel, 1);
//  ::testSqrt<16>(kernel_params, "sqrtV16TestKernel", "sqrtV16");
//}

TEST(ClCppTest, MathFallbackSqrtV16Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, sqrtV16FallbackTestKernel, 1);
  ::testSqrt<16>(kernel_params, "sqrtV16FallbackTestKernel", "sqrtV16Fallback");
}

TEST(ClCppTest, MathImplRsqrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV1TestKernel, 1);
  ::testRsqrt<1>(kernel_params, "rsqrtV1TestKernel", "rsqrtV1");
}

TEST(ClCppTest, MathFallbackRsqrtV1Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV1FallbackTestKernel, 1);
  ::testRsqrt<1>(kernel_params, "rsqrtV1FallbackTestKernel", "rsqrtV1Fallback");
}

TEST(ClCppTest, MathImplRsqrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV2TestKernel, 1);
  ::testRsqrt<2>(kernel_params, "rsqrtV2TestKernel", "rsqrtV2");
}

TEST(ClCppTest, MathFallbackRsqrtV2Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV2FallbackTestKernel, 1);
  ::testRsqrt<2>(kernel_params, "rsqrtV2FallbackTestKernel", "rsqrtV2Fallback");
}

TEST(ClCppTest, MathImplRsqrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV3TestKernel, 1);
  ::testRsqrt<3>(kernel_params, "rsqrtV3TestKernel", "rsqrtV3");
}

TEST(ClCppTest, MathFallbackRsqrtV3Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV3FallbackTestKernel, 1);
  ::testRsqrt<3>(kernel_params, "rsqrtV3FallbackTestKernel", "rsqrtV3Fallback");
}

TEST(ClCppTest, MathImplRsqrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV4TestKernel, 1);
  ::testRsqrt<4>(kernel_params, "rsqrtV4TestKernel", "rsqrtV4");
}

TEST(ClCppTest, MathFallbackRsqrtV4Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV4FallbackTestKernel, 1);
  ::testRsqrt<4>(kernel_params, "rsqrtV4FallbackTestKernel", "rsqrtV4Fallback");
}

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplRsqrtV8Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV8TestKernel, 1);
//  ::testRsqrt<8>(kernel_params, "rsqrtV8TestKernel", "rsqrtV8");
//}

TEST(ClCppTest, MathFallbackRsqrtV8Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV8FallbackTestKernel, 1);
  ::testRsqrt<8>(kernel_params, "rsqrtV8FallbackTestKernel", "rsqrtV8Fallback");
}

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplRsqrtV16Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV16TestKernel, 1);
//  ::testRsqrt<16>(kernel_params, "rsqrtV16TestKernel", "rsqrtV16");
//}

TEST(ClCppTest, MathFallbackRsqrtV16Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math, rsqrtV16FallbackTestKernel, 1);
  ::testRsqrt<16>(kernel_params, "rsqrtV16FallbackTestKernel", "rsqrtV16Fallback");
}

