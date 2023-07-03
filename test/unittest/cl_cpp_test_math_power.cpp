/*!
  \file cl_cpp_test_math_power.cpp
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
#include "zivc/kernel_set/kernel_set-cl_cpp_test_math_power.hpp"

namespace {

template <std::size_t kN, typename KernelInitParam>
void testPow(const KernelInitParam& kernel_params,
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

  std::ifstream reference_file{"resources/math_powf_reference.bin", std::ios_base::binary};
  zivc::uint32b n_num = 0;
  zisc::BSerializer::read(&n_num, &reference_file);
  zivc::uint32b n_base = 0;
  zisc::BSerializer::read(&n_base, &reference_file);

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector<cl_float> x_list;
  x_list.resize(n_num);
  zisc::BSerializer::read(x_list.data(), &reference_file, sizeof(cl_float) * n_num);
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
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  for (zivc::uint32b base_index = 0; base_index < n_base; ++base_index) {
    cl_float base = 0.0f;
    zisc::BSerializer::read(&base, &reference_file);

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
    zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, base, launch_options);
    device->waitForCompletion(result.fence());

    std::vector<cl_float> expected_list{};
    expected_list.resize(x_list.size());
    zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(cl_float) * n_num);

    // output1
    {
      const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      ztest::copyBuffer(*buffer_out, tmp.get());
      const zivc::MappedMemory mem = tmp->mapMemory();

      ztest::MathTestResult result{};
      for (std::size_t i = 0; i < mem.size(); ++i) {
        if (ztest::isSubnormal(expected_list[i])) continue;
        ztest::test(expected_list[i], mem[i], &result);
      }
      std::cout << "base=" << std::scientific << base << std::endl;
      result.print();
      result.checkError(func_name);
      std::cout << std::endl;
    }
  }
}

template <std::size_t kN, typename KernelInitParam>
void testPown(const KernelInitParam& kernel_params,
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

  std::ifstream reference_file{"resources/math_powintf_reference.bin", std::ios_base::binary};
  zivc::uint32b n_num = 0;
  zisc::BSerializer::read(&n_num, &reference_file);
  zivc::uint32b n_base = 0;
  zisc::BSerializer::read(&n_base, &reference_file);

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector<cl_float> x_list_f;
  x_list_f.resize(n_num);
  zisc::BSerializer::read(x_list_f.data(), &reference_file, sizeof(cl_float) * n_num);
  std::vector<cl_int> x_list;
  x_list.resize(n_num);
  std::transform(x_list_f.cbegin(), x_list_f.cend(), x_list.begin(),
  [](const cl_float x) noexcept
  {
    return static_cast<cl_int>(std::rint(x));
  });
  const zivc::uint32b n = x_list_f.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const cl_int> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  for (zivc::uint32b base_index = 0; base_index < n_base; ++base_index) {
    cl_float base = 0.0f;
    zisc::BSerializer::read(&base, &reference_file);

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
    zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, base, launch_options);
    device->waitForCompletion(result.fence());

    std::vector<cl_float> expected_list{};
    expected_list.resize(x_list.size());
    zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(cl_float) * n_num);

    // output1
    {
      const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
      ztest::copyBuffer(*buffer_out, tmp.get());
      const zivc::MappedMemory mem = tmp->mapMemory();

      ztest::MathTestResult result{};
      for (std::size_t i = 0; i < mem.size(); ++i)
        ztest::test(expected_list[i], mem[i], &result);
      std::cout << "base=" << std::scientific << base << std::endl;
      result.print();
      result.checkError(func_name);
      std::cout << std::endl;
    }
  }
}

template <std::size_t kN, typename KernelInitParam>
void testHypot(const KernelInitParam& kernel_params,
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

  std::ifstream reference_file{"resources/math_hypotf_reference.bin", std::ios_base::binary};
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

} /* namespace */

TEST(ClCppTest, MathImplExpV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expf_reference.bin",
                          "expV1TestKernel",
                          "expV1");
}

TEST(ClCppTest, MathImplExpV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expf_reference.bin",
                          "expV2TestKernel",
                          "expV2");
}

TEST(ClCppTest, MathImplExpV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expf_reference.bin",
                          "expV3TestKernel",
                          "expV3");
}

TEST(ClCppTest, MathImplExpV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expf_reference.bin",
                          "expV4TestKernel",
                          "expV4");
}

TEST(ClCppTest, MathImplExpV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expf_reference.bin",
                          "expV8TestKernel",
                          "expV8");
}

TEST(ClCppTest, MathImplExpV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPowXList<float>,
                           "resources/math_expf_reference.bin",
                           "expV16TestKernel",
                           "expV16");
}

TEST(ClCppTest, MathImplExp2V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_exp2f_reference.bin",
                          "exp2V1TestKernel",
                          "exp2V1");
}

TEST(ClCppTest, MathImplExp2V2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_exp2f_reference.bin",
                          "exp2V2TestKernel",
                          "exp2V2");
}

TEST(ClCppTest, MathImplExp2V3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_exp2f_reference.bin",
                          "exp2V3TestKernel",
                          "exp2V3");
}

TEST(ClCppTest, MathImplExp2V4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_exp2f_reference.bin",
                          "exp2V4TestKernel",
                          "exp2V4");
}

TEST(ClCppTest, MathImplExp2V8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_exp2f_reference.bin",
                          "exp2V8TestKernel",
                          "exp2V8");
}

TEST(ClCppTest, MathImplExp2V16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPowXList<float>,
                           "resources/math_exp2f_reference.bin",
                           "exp2V16TestKernel",
                           "exp2V16");
}

TEST(ClCppTest, MathImplExpm1V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expm1f_reference.bin",
                          "expm1V1TestKernel",
                          "expm1V1");
}

TEST(ClCppTest, MathImplExpm1V2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expm1f_reference.bin",
                          "expm1V2TestKernel",
                          "expm1V2");
}

TEST(ClCppTest, MathImplExpm1V3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expm1f_reference.bin",
                          "expm1V3TestKernel",
                          "expm1V3");
}

TEST(ClCppTest, MathImplExpm1V4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expm1f_reference.bin",
                          "expm1V4TestKernel",
                          "expm1V4");
}

TEST(ClCppTest, MathImplExpm1V8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_expm1f_reference.bin",
                          "expm1V8TestKernel",
                          "expm1V8");
}

TEST(ClCppTest, MathImplExpm1V16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPowXList<float>,
                           "resources/math_expm1f_reference.bin",
                           "expm1V16TestKernel",
                           "expm1V16");
}

TEST(ClCppTest, MathImplLogV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV1TestKernel",
                          "logV1");
}

TEST(ClCppTest, MathImplLogV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV2TestKernel",
                          "logV2");
}

TEST(ClCppTest, MathImplLogV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV3TestKernel",
                          "logV3");
}

TEST(ClCppTest, MathImplLogV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV4TestKernel",
                          "logV4");
}

TEST(ClCppTest, MathImplLogV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV8TestKernel",
                          "logV8");
}

TEST(ClCppTest, MathImplLogV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_logf_reference.bin",
                          "logV16TestKernel",
                          "logV16");
}

TEST(ClCppTest, MathImplLog2V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V1TestKernel",
                          "log2V1");
}

TEST(ClCppTest, MathImplLog2V2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V2TestKernel",
                          "log2V2");
}

TEST(ClCppTest, MathImplLog2V3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V3TestKernel",
                          "log2V3");
}

TEST(ClCppTest, MathImplLog2V4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V4TestKernel",
                          "log2V4");
}

TEST(ClCppTest, MathImplLog2V8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V8TestKernel",
                          "log2V8");
}

TEST(ClCppTest, MathImplLog2V16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log2f_reference.bin",
                          "log2V16TestKernel",
                          "log2V16");
}

TEST(ClCppTest, MathImplLog10V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V1TestKernel",
                          "log10V1");
}

TEST(ClCppTest, MathImplLog10V2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V2TestKernel",
                          "log10V2");
}

TEST(ClCppTest, MathImplLog10V3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V3TestKernel",
                          "log10V3");
}

TEST(ClCppTest, MathImplLog10V4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V4TestKernel",
                          "log10V4");
}

TEST(ClCppTest, MathImplLog10V8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V8TestKernel",
                          "log10V8");
}

TEST(ClCppTest, MathImplLog10V16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log10f_reference.bin",
                          "log10V16TestKernel",
                          "log10V16");
}

TEST(ClCppTest, MathImplLog1pV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV1TestKernel",
                          "log1pV1");
}

TEST(ClCppTest, MathImplLog1pV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV2TestKernel",
                          "log1pV2");
}

TEST(ClCppTest, MathImplLog1pV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV3TestKernel",
                          "log1pV3");
}

TEST(ClCppTest, MathImplLog1pV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV4TestKernel",
                          "log1pV4");
}

TEST(ClCppTest, MathImplLog1pV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV8TestKernel",
                          "log1pV8");
}

TEST(ClCppTest, MathImplLog1pV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_log1pf_reference.bin",
                          "log1pV16TestKernel",
                          "log1pV16");
}

TEST(ClCppTest, MathImplPowV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV1TestKernel, 1);
  ::testPow<1>(kernel_params, "powV1TestKernel", "powV1");
}

TEST(ClCppTest, MathPrecisionPowV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV1PrecisionTestKernel, 1);
  ::testPow<1>(kernel_params, "powV1PrecisionTestKernel", "powV1Precision");
}

TEST(ClCppTest, MathImplPowV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV2TestKernel, 1);
  ::testPow<2>(kernel_params, "powV2TestKernel", "powV2");
}

TEST(ClCppTest, MathPrecisionPowV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV2PrecisionTestKernel, 1);
  ::testPow<2>(kernel_params, "powV2PrecisionTestKernel", "powV2Precision");
}

TEST(ClCppTest, MathImplPowV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV3TestKernel, 1);
  ::testPow<3>(kernel_params, "powV3TestKernel", "powV3");
}

TEST(ClCppTest, MathPrecisionPowV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV3PrecisionTestKernel, 1);
  ::testPow<3>(kernel_params, "powV3PrecisionTestKernel", "powV3Precision");
}

TEST(ClCppTest, MathImplPowV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV4TestKernel, 1);
  ::testPow<4>(kernel_params, "powV4TestKernel", "powV4");
}

TEST(ClCppTest, MathPrecisionPowV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV4PrecisionTestKernel, 1);
  ::testPow<4>(kernel_params, "powV4PrecisionTestKernel", "powV4Precision");
}

TEST(ClCppTest, MathImplPowV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV8TestKernel, 1);
  ::testPow<8>(kernel_params, "powV8TestKernel", "powV8");
}

TEST(ClCppTest, MathPrecisionPowV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV8PrecisionTestKernel, 1);
  ::testPow<8>(kernel_params, "powV8PrecisionTestKernel", "powV8Precision");
}

TEST(ClCppTest, MathImplPowV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV16TestKernel, 1);
  ::testPow<16>(kernel_params, "powV16TestKernel", "powV16");
}

TEST(ClCppTest, MathPrecisionPowV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, powV16PrecisionTestKernel, 1);
  ::testPow<16>(kernel_params, "powV16PrecisionTestKernel", "powV16Precision");
}

TEST(ClCppTest, MathImplPownV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV1TestKernel, 1);
  ::testPown<1>(kernel_params, "pownV1TestKernel", "pownV1");
}

TEST(ClCppTest, MathPrecisionPownV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV1PrecisionTestKernel, 1);
  ::testPown<1>(kernel_params, "pownV1PrecisionTestKernel", "pownV1Precision");
}

TEST(ClCppTest, MathImplPownV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV2TestKernel, 1);
  ::testPown<2>(kernel_params, "pownV2TestKernel", "pownV2");
}

TEST(ClCppTest, MathPrecisionPownV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV2PrecisionTestKernel, 1);
  ::testPown<2>(kernel_params, "pownV2PrecisionTestKernel", "pownV2Precision");
}

TEST(ClCppTest, MathImplPownV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV3TestKernel, 1);
  ::testPown<3>(kernel_params, "pownV3TestKernel", "pownV3");
}

TEST(ClCppTest, MathPrecisionPownV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV3PrecisionTestKernel, 1);
  ::testPown<3>(kernel_params, "pownV3PrecisionTestKernel", "pownV3Precision");
}

TEST(ClCppTest, MathImplPownV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV4TestKernel, 1);
  ::testPown<4>(kernel_params, "pownV4TestKernel", "pownV4");
}

TEST(ClCppTest, MathPrecisionPownV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV4PrecisionTestKernel, 1);
  ::testPown<4>(kernel_params, "pownV4PrecisionTestKernel", "pownV4Precision");
}

TEST(ClCppTest, MathImplPownV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV8TestKernel, 1);
  ::testPown<8>(kernel_params, "pownV8TestKernel", "pownV8");
}

TEST(ClCppTest, MathPrecisionPownV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV8PrecisionTestKernel, 1);
  ::testPown<8>(kernel_params, "pownV8PrecisionTestKernel", "pownV8Precision");
}

TEST(ClCppTest, MathImplPownV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV16TestKernel, 1);
  ::testPown<16>(kernel_params, "pownV16TestKernel", "pownV16");
}

TEST(ClCppTest, MathPrecisionPownV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, pownV16PrecisionTestKernel, 1);
  ::testPown<16>(kernel_params, "pownV16PrecisionTestKernel", "pownV16Precision");
}

TEST(ClCppTest, MathImplSqrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV1TestKernel",
                          "sqrtV1");
}

TEST(ClCppTest, MathPrecisionSqrtV1Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV1PrecisionTestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV1PrecisionTestKernel",
                          "sqrtV1Precision");

}

TEST(ClCppTest, MathImplSqrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV2TestKernel",
                          "sqrtV2");
}

TEST(ClCppTest, MathPrecisionSqrtV2Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV2PrecisionTestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV2PrecisionTestKernel",
                          "sqrtV2Precision");
}

TEST(ClCppTest, MathImplSqrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV3TestKernel",
                          "sqrtV3");
}

TEST(ClCppTest, MathPrecisionSqrtV3Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV3PrecisionTestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV3PrecisionTestKernel",
                          "sqrtV3Precision");
}

TEST(ClCppTest, MathImplSqrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV4TestKernel",
                          "sqrtV4");
}

TEST(ClCppTest, MathPrecisionSqrtV4Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV4PrecisionTestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV4PrecisionTestKernel",
                          "sqrtV4Precision");
}

TEST(ClCppTest, MathImplSqrtV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV8TestKernel",
                          "sqrtV8");
}

TEST(ClCppTest, MathPrecisionSqrtV8Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV8PrecisionTestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV8PrecisionTestKernel",
                          "sqrtV8Precision");
}

TEST(ClCppTest, MathImplSqrtV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPositiveXList<float>,
                           "resources/math_sqrtf_reference.bin",
                           "sqrtV16TestKernel",
                           "sqrtV16");
}

TEST(ClCppTest, MathPrecisionSqrtV16Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV16PrecisionTestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPositiveXList<float>,
                           "resources/math_sqrtf_reference.bin",
                           "sqrtV16PrecisionTestKernel",
                           "sqrtV16Precision");
}

TEST(ClCppTest, MathImplRsqrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV1TestKernel",
                          "rsqrtV1",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV1Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV1PrecisionTestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV1PrecisionTestKernel",
                          "rsqrtV1Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplRsqrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV2TestKernel",
                          "rsqrtV2",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV2Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV2PrecisionTestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV2PrecisionTestKernel",
                          "rsqrtV2Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplRsqrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV3TestKernel",
                          "rsqrtV3",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV3Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV3PrecisionTestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV3PrecisionTestKernel",
                          "rsqrtV3Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplRsqrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV4TestKernel",
                          "rsqrtV4",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV4Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV4PrecisionTestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV4PrecisionTestKernel",
                          "rsqrtV4Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplRsqrtV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV8TestKernel",
                          "rsqrtV8",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV8Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV8PrecisionTestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "rsqrtV8PrecisionTestKernel",
                          "rsqrtV8Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplRsqrtV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPositiveXList<float>,
                           "resources/math_sqrtf_reference.bin",
                           "rsqrtV16TestKernel",
                           "rsqrtV16",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathPrecisionRsqrtV16Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV16PrecisionTestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                           ztest::loadPositiveXList<float>,
                           "resources/math_sqrtf_reference.bin",
                           "rsqrtV16PrecisionTestKernel",
                           "rsqrtV16Precision",
                          [](const float v){return v;},
                          [](const float v){return 1.0f / v;});
}

TEST(ClCppTest, MathImplCbrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV1TestKernel",
                          "cbrtV1");
}

TEST(ClCppTest, MathPrecisionCbrtV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV1PrecisionTestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV1PrecisionTestKernel",
                          "cbrtV1Precision");
}

TEST(ClCppTest, MathImplCbrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV2TestKernel",
                          "cbrtV2");
}

TEST(ClCppTest, MathPrecisionCbrtV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV2PrecisionTestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV2PrecisionTestKernel",
                          "cbrtV2Precision");
}

TEST(ClCppTest, MathImplCbrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV3TestKernel",
                          "cbrtV3");
}

TEST(ClCppTest, MathPrecisionCbrtV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV3PrecisionTestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV3PrecisionTestKernel",
                          "cbrtV3Precision");
}

TEST(ClCppTest, MathImplCbrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV4TestKernel",
                          "cbrtV4");
}

TEST(ClCppTest, MathPrecisionCbrtV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV4PrecisionTestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV4PrecisionTestKernel",
                          "cbrtV4Precision");
}

TEST(ClCppTest, MathImplCbrtV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV8TestKernel",
                          "cbrtV8");
}

TEST(ClCppTest, MathPrecisionCbrtV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV8PrecisionTestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV8PrecisionTestKernel",
                          "cbrtV8Precision");
}

TEST(ClCppTest, MathImplCbrtV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV16TestKernel",
                          "cbrtV16");
}

TEST(ClCppTest, MathPrecisionCbrtV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, cbrtV16PrecisionTestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_cbrtf_reference.bin",
                          "cbrtV16PrecisionTestKernel",
                          "cbrtV16Precision");
}

TEST(ClCppTest, MathImplHypotV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV1TestKernel, 1);
  ::testHypot<1>(kernel_params, "hypotV1TestKernel", "hypotV1");
}

TEST(ClCppTest, MathImplHypotV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV2TestKernel, 1);
  ::testHypot<2>(kernel_params, "hypotV2TestKernel", "hypotV2");
}

TEST(ClCppTest, MathImplHypotV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV3TestKernel, 1);
  ::testHypot<3>(kernel_params, "hypotV3TestKernel", "hypotV3");
}

TEST(ClCppTest, MathImplHypotV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV4TestKernel, 1);
  ::testHypot<4>(kernel_params, "hypotV4TestKernel", "hypotV4");
}

TEST(ClCppTest, MathImplHypotV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV8TestKernel, 1);
  ::testHypot<8>(kernel_params, "hypotV8TestKernel", "hypotV8");
}

TEST(ClCppTest, MathImplHypotV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, hypotV16TestKernel, 1);
  ::testHypot<16>(kernel_params, "hypotV16TestKernel", "hypotV16");
}
