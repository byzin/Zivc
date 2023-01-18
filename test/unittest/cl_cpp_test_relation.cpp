/*!
  \file cl_cpp_test_relation.cpp
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
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_relation.hpp"

TEST(ClCppTest, RelationIsEqualTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::cl_CompResult<cl_float>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(18);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_float2>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(11);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_float3>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(11);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_float4>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float> v = {0.0f,
                                               1.0f,
                                               -1.0f,
                                               fmax,
                                               -fmax,
                                               fmin,
                                               subnormal_max,
                                               subnormal_min,
                                               finf,
                                               -finf,
                                               fnan};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  {
    const std::initializer_list<cl_float2> v = {
        cl_float2{0.0f, -0.0f},
        cl_float2{1.0f, -1.0f},
        cl_float2{fmax, -fmax},
        cl_float2{fmin, -fmin},
        cl_float2{subnormal_max, subnormal_min},
        cl_float2{finf, -finf},
        cl_float2{fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  {
    const std::initializer_list<cl_float3> v = {
        cl_float3{0.0f, -0.0f, 0.0f},
        cl_float3{1.0f, -1.0f, 1.0f},
        cl_float3{fmax, -fmax, fmax},
        cl_float3{fmin, -fmin, fmin},
        cl_float3{subnormal_max, subnormal_min, subnormal_max},
        cl_float3{finf, -finf, finf},
        cl_float3{fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  {
    const std::initializer_list<cl_float4> v = {
        cl_float4{0.0f, -0.0f, 0.0f, -0.0f},
        cl_float4{1.0f, -1.0f, 1.0f, -1.0f},
        cl_float4{fmax, -fmax, fmax, -fmax},
        cl_float4{fmin, -fmin, fmin, -fmin},
        cl_float4{subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float4{finf, -finf, finf, -finf},
        cl_float4{fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isEqualTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isEqualTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(8, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_in4, *buffer_out1, *buffer_out2, *buffer_out3, *buffer_out4, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output1
  {
    constexpr cl_int t = zivc::cl::kSTrue;
    constexpr cl_int f = zivc::cl::kSFalse;
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    ASSERT_EQ(t, mem[0]) << "isequal failed.";
    ASSERT_EQ(t, mem[1]) << "isequal failed.";
    ASSERT_EQ(f, mem[2]) << "isequal failed.";
    ASSERT_EQ(t, mem[3]) << "isequal failed.";
    ASSERT_EQ(t, mem[4]) << "isequal failed.";
    ASSERT_EQ(t, mem[5]) << "isequal failed.";
    ASSERT_EQ(f, mem[6]) << "isequal failed.";
    ASSERT_EQ(t, mem[7]) << "isequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[8]) << "isequal failed.";
    //ASSERT_EQ(t, mem[9]) << "isequal failed.";
    //ASSERT_EQ(f, mem[10]) << "isequal failed.";
    //ASSERT_EQ(f, mem[11]) << "isequal failed.";
    ASSERT_EQ(t, mem[12]) << "isequal failed.";
    ASSERT_EQ(f, mem[13]) << "isequal failed.";
    ASSERT_EQ(t, mem[14]) << "isequal failed.";
    ASSERT_EQ(f, mem[15]) << "isequal failed.";
    ASSERT_EQ(f, mem[16]) << "isequal failed.";
    ASSERT_EQ(f, mem[17]) << "isequal failed.";
  }
  // output2
  {
    constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isequal failed.";
    ASSERT_EQ(f, mem[2].y) << "isequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isequal failed.";
    ASSERT_EQ(f, mem[5].y) << "isequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "isequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isequal failed.";
    ASSERT_EQ(f, mem[9].x) << "isequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isequal failed.";
  }
  // output3
  {
    constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isequal failed.";
    ASSERT_EQ(t, mem[0].z) << "isequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isequal failed.";
    ASSERT_EQ(t, mem[1].z) << "isequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isequal failed.";
    ASSERT_EQ(f, mem[2].y) << "isequal failed.";
    ASSERT_EQ(f, mem[2].z) << "isequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isequal failed.";
    ASSERT_EQ(t, mem[3].z) << "isequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isequal failed.";
    ASSERT_EQ(t, mem[4].z) << "isequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isequal failed.";
    ASSERT_EQ(f, mem[5].y) << "isequal failed.";
    ASSERT_EQ(f, mem[5].z) << "isequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].z) << "isequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isequal failed.";
    ASSERT_EQ(t, mem[8].z) << "isequal failed.";
    ASSERT_EQ(f, mem[9].x) << "isequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isequal failed.";
    ASSERT_EQ(f, mem[9].z) << "isequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isequal failed.";
    ASSERT_EQ(f, mem[10].z) << "isequal failed.";
  }
  // output4
  {
    constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isequal failed.";
    ASSERT_EQ(t, mem[0].z) << "isequal failed.";
    ASSERT_EQ(t, mem[0].w) << "isequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isequal failed.";
    ASSERT_EQ(t, mem[1].z) << "isequal failed.";
    ASSERT_EQ(t, mem[1].w) << "isequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isequal failed.";
    ASSERT_EQ(f, mem[2].y) << "isequal failed.";
    ASSERT_EQ(f, mem[2].z) << "isequal failed.";
    ASSERT_EQ(f, mem[2].w) << "isequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isequal failed.";
    ASSERT_EQ(t, mem[3].z) << "isequal failed.";
    ASSERT_EQ(t, mem[3].w) << "isequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isequal failed.";
    ASSERT_EQ(t, mem[4].z) << "isequal failed.";
    ASSERT_EQ(t, mem[4].w) << "isequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isequal failed.";
    ASSERT_EQ(f, mem[5].y) << "isequal failed.";
    ASSERT_EQ(f, mem[5].z) << "isequal failed.";
    ASSERT_EQ(f, mem[5].w) << "isequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "isequal failed.";
    //ASSERT_EQ(t, mem[6].w) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].z) << "isequal failed.";
    //ASSERT_EQ(f, mem[7].w) << "isequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isequal failed.";
    ASSERT_EQ(t, mem[8].z) << "isequal failed.";
    ASSERT_EQ(t, mem[8].w) << "isequal failed.";
    ASSERT_EQ(f, mem[9].x) << "isequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isequal failed.";
    ASSERT_EQ(f, mem[9].z) << "isequal failed.";
    ASSERT_EQ(f, mem[9].w) << "isequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isequal failed.";
    ASSERT_EQ(f, mem[10].z) << "isequal failed.";
    ASSERT_EQ(f, mem[10].w) << "isequal failed.";
  }
}
