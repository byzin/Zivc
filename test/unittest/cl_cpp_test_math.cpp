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
