/*!
  \file cl_cpp_test_math_trigonometric.cpp
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
#include "zivc/kernel_set/kernel_set-cl_cpp_test_math_trigonometric.hpp"

namespace {

template <std::size_t kN, typename KernelInitParam>
void testAtan2(const KernelInitParam& kernel_params,
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

  std::ifstream reference_file{"resources/math_atan2f_reference.bin", std::ios_base::binary};
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
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (ztest::isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

} /* namespace */

TEST(ClCppTest, MathImplSinV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV1TestKernel",
                          "sinV1");
}

TEST(ClCppTest, MathImplSinV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV2TestKernel",
                          "sinV2");
}

TEST(ClCppTest, MathImplSinV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV3TestKernel",
                          "sinV3");
}

TEST(ClCppTest, MathImplSinV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV4TestKernel",
                          "sinV4");
}

TEST(ClCppTest, MathImplSinV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV8TestKernel",
                          "sinV8");
}

TEST(ClCppTest, MathImplSinV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_sinf_reference.bin",
                          "sinV16TestKernel",
                          "sinV16");
}

TEST(ClCppTest, MathImplCosV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV1TestKernel",
                          "cosV1");
}

TEST(ClCppTest, MathImplCosV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV2TestKernel",
                          "cosV2");
}

TEST(ClCppTest, MathImplCosV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV3TestKernel",
                          "cosV3");
}

TEST(ClCppTest, MathImplCosV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV4TestKernel",
                          "cosV4");
}

TEST(ClCppTest, MathImplCosV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV8TestKernel",
                          "cosV8");
}

TEST(ClCppTest, MathImplCosV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, cosV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_cosf_reference.bin",
                          "cosV16TestKernel",
                          "cosV16");
}

TEST(ClCppTest, MathImplTanV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV1TestKernel",
                          "tanV1");
}

TEST(ClCppTest, MathImplTanV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV2TestKernel",
                          "tanV2");
}

TEST(ClCppTest, MathImplTanV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV3TestKernel",
                          "tanV3");
}

TEST(ClCppTest, MathImplTanV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV4TestKernel",
                          "tanV4");
}

TEST(ClCppTest, MathImplTanV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV8TestKernel",
                          "tanV8");
}

TEST(ClCppTest, MathImplTanV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadPowXList<float>,
                          "resources/math_tanf_reference.bin",
                          "tanV16TestKernel",
                          "tanV16");
}

TEST(ClCppTest, MathImplAsinV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV1TestKernel",
                          "asinV1");
}

TEST(ClCppTest, MathImplAsinV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV2TestKernel",
                          "asinV2");
}

TEST(ClCppTest, MathImplAsinV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV3TestKernel",
                          "asinV3");
}

TEST(ClCppTest, MathImplAsinV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV4TestKernel",
                          "asinV4");
}

TEST(ClCppTest, MathImplAsinV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV8TestKernel",
                          "asinV8");
}

TEST(ClCppTest, MathImplAsinV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_asinf_reference.bin",
                          "asinV16TestKernel",
                          "asinV16");
}

TEST(ClCppTest, MathImplAcosV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV1TestKernel",
                          "acosV1");
}

TEST(ClCppTest, MathImplAcosV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV2TestKernel",
                          "acosV2");
}

TEST(ClCppTest, MathImplAcosV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV3TestKernel",
                          "acosV3");
}

TEST(ClCppTest, MathImplAcosV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV4TestKernel",
                          "acosV4");
}

TEST(ClCppTest, MathImplAcosV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV8TestKernel",
                          "acosV8");
}

TEST(ClCppTest, MathImplAcosV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acosV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_acosf_reference.bin",
                          "acosV16TestKernel",
                          "acosV16");
}

TEST(ClCppTest, MathImplAtanV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV1TestKernel",
                          "atanV1");
}

TEST(ClCppTest, MathImplAtanV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV2TestKernel",
                          "atanV2");
}

TEST(ClCppTest, MathImplAtanV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV3TestKernel",
                          "atanV3");
}

TEST(ClCppTest, MathImplAtanV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV4TestKernel",
                          "atanV4");
}

TEST(ClCppTest, MathImplAtanV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV8TestKernel",
                          "atanV8");
}

TEST(ClCppTest, MathImplAtanV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadOneXList<float>,
                          "resources/math_atanf_reference.bin",
                          "atanV16TestKernel",
                          "atanV16");
}

TEST(ClCppTest, MathImplAtan2V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V1TestKernel, 1);
  ::testAtan2<1>(kernel_params, "atan2V1TestKernel", "atan2V1");
}

TEST(ClCppTest, MathImplAtan2V2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V2TestKernel, 1);
  ::testAtan2<2>(kernel_params, "atan2V2TestKernel", "atan2V2");
}

TEST(ClCppTest, MathImplAtan2V3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V3TestKernel, 1);
  ::testAtan2<3>(kernel_params, "atan2V3TestKernel", "atan2V3");
}

TEST(ClCppTest, MathImplAtan2V4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V4TestKernel, 1);
  ::testAtan2<4>(kernel_params, "atan2V4TestKernel", "atan2V4");
}

TEST(ClCppTest, MathImplAtan2V8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V8TestKernel, 1);
  ::testAtan2<8>(kernel_params, "atan2V8TestKernel", "atan2V8");
}

TEST(ClCppTest, MathImplAtan2V16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atan2V16TestKernel, 1);
  ::testAtan2<16>(kernel_params, "atan2V16TestKernel", "atan2V16");
}

TEST(ClCppTest, MathImplSinhV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV1TestKernel",
                          "sinhV1");
}

TEST(ClCppTest, MathImplSinhV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV2TestKernel",
                          "sinhV2");
}

TEST(ClCppTest, MathImplSinhV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV3TestKernel",
                          "sinhV3");
}

TEST(ClCppTest, MathImplSinhV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV4TestKernel",
                          "sinhV4");
}

TEST(ClCppTest, MathImplSinhV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV8TestKernel",
                          "sinhV8");
}

TEST(ClCppTest, MathImplSinhV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, sinhV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_sinhf_reference.bin",
                          "sinhV16TestKernel",
                          "sinhV16");
}

TEST(ClCppTest, MathImplCoshV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV1TestKernel",
                          "coshV1");
}

TEST(ClCppTest, MathImplCoshV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV2TestKernel",
                          "coshV2");
}

TEST(ClCppTest, MathImplCoshV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV3TestKernel",
                          "coshV3");
}

TEST(ClCppTest, MathImplCoshV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV4TestKernel",
                          "coshV4");
}

TEST(ClCppTest, MathImplCoshV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV8TestKernel",
                          "coshV8");
}

TEST(ClCppTest, MathImplCoshV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, coshV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_coshf_reference.bin",
                          "coshV16TestKernel",
                          "coshV16");
}

TEST(ClCppTest, MathImplTanhV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV1TestKernel",
                          "tanhV1");
}

TEST(ClCppTest, MathImplTanhV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV2TestKernel",
                          "tanhV2");
}

TEST(ClCppTest, MathImplTanhV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV3TestKernel",
                          "tanhV3");
}

TEST(ClCppTest, MathImplTanhV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV4TestKernel",
                          "tanhV4");
}

TEST(ClCppTest, MathImplTanhV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV8TestKernel",
                          "tanhV8");
}

TEST(ClCppTest, MathImplTanhV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, tanhV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_tanhf_reference.bin",
                          "tanhV16TestKernel",
                          "tanhV16");
}

TEST(ClCppTest, MathImplAsinhV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV1TestKernel",
                          "asinhV1");
}

TEST(ClCppTest, MathImplAsinhV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV2TestKernel",
                          "asinhV2");
}

TEST(ClCppTest, MathImplAsinhV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV3TestKernel",
                          "asinhV3");
}

TEST(ClCppTest, MathImplAsinhV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV4TestKernel",
                          "asinhV4");
}

TEST(ClCppTest, MathImplAsinhV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV8TestKernel",
                          "asinhV8");
}

TEST(ClCppTest, MathImplAsinhV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, asinhV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_asinhf_reference.bin",
                          "asinhV16TestKernel",
                          "asinhV16");
}

TEST(ClCppTest, MathImplAcoshV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV1TestKernel",
                          "acoshV1");
}

TEST(ClCppTest, MathImplAcoshV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV2TestKernel",
                          "acoshV2");
}

TEST(ClCppTest, MathImplAcoshV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV3TestKernel",
                          "acoshV3");
}

TEST(ClCppTest, MathImplAcoshV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV4TestKernel",
                          "acoshV4");
}

TEST(ClCppTest, MathImplAcoshV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV8TestKernel",
                          "acoshV8");
}

TEST(ClCppTest, MathImplAcoshV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, acoshV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_acoshf_reference.bin",
                          "acoshV16TestKernel",
                          "acoshV16");
}

TEST(ClCppTest, MathImplAtanhV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV1TestKernel, 1);
  ztest::testF1<1, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV1TestKernel",
                          "atanhV1");
}

TEST(ClCppTest, MathImplAtanhV2Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV2TestKernel, 1);
  ztest::testF1<2, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV2TestKernel",
                          "atanhV2");
}

TEST(ClCppTest, MathImplAtanhV3Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV3TestKernel, 1);
  ztest::testF1<3, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV3TestKernel",
                          "atanhV3");
}

TEST(ClCppTest, MathImplAtanhV4Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV4TestKernel, 1);
  ztest::testF1<4, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV4TestKernel",
                          "atanhV4");
}

TEST(ClCppTest, MathImplAtanhV8Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV8TestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV8TestKernel",
                          "atanhV8");
}

TEST(ClCppTest, MathImplAtanhV16Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_trigonometric, atanhV16TestKernel, 1);
  ztest::testF1<16, float>(kernel_params,
                          ztest::loadAllXList<float>,
                          "resources/math_atanhf_reference.bin",
                          "atanhV16TestKernel",
                          "atanhV16");
}
