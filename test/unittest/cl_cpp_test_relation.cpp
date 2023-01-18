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

TEST(ClCppTest, RelationIsNotEqualTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isNotEqualTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isNotEqualTestKernel");
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
    constexpr cl_int tl = zivc::cl::kSTrue;
    constexpr cl_int fl = zivc::cl::kSFalse;
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    ASSERT_EQ(fl, mem[0]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[5]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[6]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[7]) << "isnotequl failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(fl, mem[8]) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[9]) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[10]) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[11]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[12]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[13]) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[14]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[15]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[16]) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[17]) << "isnotequl failed.";
  }
  // output2
  {
    constexpr cl_int tl = zivc::cl::kVTrue;
    constexpr cl_int fl = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(fl, mem[0].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].y) << "isnotequl failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(fl, mem[6].x) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].y) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].x) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].y) << "isnotequl failed.";
  }
  // output3
  {
    constexpr cl_int tl = zivc::cl::kVTrue;
    constexpr cl_int fl = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(fl, mem[0].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].z) << "isnotequl failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(fl, mem[6].x) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].y) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].z) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].x) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].y) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].z) << "isnotequl failed.";
  }
  // output4
  {
    constexpr cl_int tl = zivc::cl::kVTrue;
    constexpr cl_int fl = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(fl, mem[0].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[0].w) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[1].w) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[2].w) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[3].w) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[4].w) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[5].w) << "isnotequl failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(fl, mem[6].x) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].y) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].z) << "isnotequl failed.";
    //ASSERT_EQ(fl, mem[6].w) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].x) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].y) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].z) << "isnotequl failed.";
    //ASSERT_EQ(tl, mem[7].w) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].x) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].y) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].z) << "isnotequl failed.";
    ASSERT_EQ(fl, mem[8].w) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[9].w) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].x) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].y) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].z) << "isnotequl failed.";
    ASSERT_EQ(tl, mem[10].w) << "isnotequl failed.";
  }
}

TEST(ClCppTest, RelationIsGreaterTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isGreaterTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isGreaterTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "isgreater failed.";
    ASSERT_EQ(f, mem[1]) << "isgreater failed.";
    ASSERT_EQ(f, mem[2]) << "isgreater failed.";
    ASSERT_EQ(f, mem[3]) << "isgreater failed.";
    ASSERT_EQ(f, mem[4]) << "isgreater failed.";
    ASSERT_EQ(f, mem[5]) << "isgreater failed.";
    ASSERT_EQ(f, mem[6]) << "isgreater failed.";
    ASSERT_EQ(f, mem[7]) << "isgreater failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[8]) << "isgreater failed.";
    //ASSERT_EQ(f, mem[9]) << "isgreater failed.";
    //ASSERT_EQ(t, mem[10]) << "isgreater failed.";
    //ASSERT_EQ(t, mem[11]) << "isgreater failed.";
    ASSERT_EQ(f, mem[12]) << "isgreater failed.";
    ASSERT_EQ(f, mem[13]) << "isgreater failed.";
    ASSERT_EQ(f, mem[14]) << "isgreater failed.";
    ASSERT_EQ(f, mem[15]) << "isgreater failed.";
    ASSERT_EQ(t, mem[16]) << "isgreater failed.";
    ASSERT_EQ(f, mem[17]) << "isgreater failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreater failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "isgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreater failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[2].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[5].z) << "isgreater failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].z) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].z) << "isgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreater failed.";
    ASSERT_EQ(t, mem[9].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].z) << "isgreater failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[0].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[1].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[2].z) << "isgreater failed.";
    ASSERT_EQ(t, mem[2].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[3].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[4].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[5].z) << "isgreater failed.";
    ASSERT_EQ(t, mem[5].w) << "isgreater failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].z) << "isgreater failed.";
    //ASSERT_EQ(f, mem[6].w) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].z) << "isgreater failed.";
    //ASSERT_EQ(t, mem[7].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[8].w) << "isgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreater failed.";
    ASSERT_EQ(t, mem[9].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[9].w) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].z) << "isgreater failed.";
    ASSERT_EQ(f, mem[10].w) << "isgreater failed.";
  }
}

TEST(ClCppTest, RelationIsGreaterEqualTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isGreaterEqualTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isGreaterEqualTestKernel");
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
    ASSERT_EQ(t, mem[0]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1]) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[5]) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[6]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[7]) << "isgreaterequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[8]) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[9]) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[10]) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[11]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[12]) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[13]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[14]) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[15]) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[16]) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[17]) << "isgreaterequal failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreaterequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreaterequal failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].z) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].z) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[5].z) << "isgreaterequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[9].z) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].z) << "isgreaterequal failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[0].w) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[1].w) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[2].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[2].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[2].w) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[3].w) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[4].w) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[5].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[5].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[5].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[5].w) << "isgreaterequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[6].w) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].x) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].y) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].z) << "isgreaterequal failed.";
    //ASSERT_EQ(t, mem[7].w) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].x) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].z) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[8].w) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[9].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[9].y) << "isgreaterequal failed.";
    ASSERT_EQ(t, mem[9].z) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[9].w) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].x) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].y) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].z) << "isgreaterequal failed.";
    ASSERT_EQ(f, mem[10].w) << "isgreaterequal failed.";
  }
}

TEST(ClCppTest, RelationIsLessTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "isgreater failed.";
    ASSERT_EQ(f, mem[1]) << "isgreater failed.";
    ASSERT_EQ(t, mem[2]) << "isgreater failed.";
    ASSERT_EQ(f, mem[3]) << "isgreater failed.";
    ASSERT_EQ(f, mem[4]) << "isgreater failed.";
    ASSERT_EQ(f, mem[5]) << "isgreater failed.";
    ASSERT_EQ(t, mem[6]) << "isless failed.";
    ASSERT_EQ(f, mem[7]) << "isless failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[8]) << "isless failed.";
    //ASSERT_EQ(f, mem[9]) << "isless failed.";
    //ASSERT_EQ(f, mem[10]) << "isless failed.";
    //ASSERT_EQ(f, mem[11]) << "isless failed.";
    ASSERT_EQ(f, mem[12]) << "isless failed.";
    ASSERT_EQ(t, mem[13]) << "isless failed.";
    ASSERT_EQ(f, mem[14]) << "isless failed.";
    ASSERT_EQ(t, mem[15]) << "isless failed.";
    ASSERT_EQ(f, mem[16]) << "isless failed.";
    ASSERT_EQ(f, mem[17]) << "isless failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isless failed.";
    ASSERT_EQ(f, mem[0].y) << "isless failed.";
    ASSERT_EQ(f, mem[1].x) << "isless failed.";
    ASSERT_EQ(f, mem[1].y) << "isless failed.";
    ASSERT_EQ(t, mem[2].x) << "isless failed.";
    ASSERT_EQ(f, mem[2].y) << "isless failed.";
    ASSERT_EQ(f, mem[3].x) << "isless failed.";
    ASSERT_EQ(f, mem[3].y) << "isless failed.";
    ASSERT_EQ(f, mem[4].x) << "isless failed.";
    ASSERT_EQ(f, mem[4].y) << "isless failed.";
    ASSERT_EQ(t, mem[5].x) << "isless failed.";
    ASSERT_EQ(f, mem[5].y) << "isless failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isless failed.";
    //ASSERT_EQ(f, mem[6].y) << "isless failed.";
    //ASSERT_EQ(f, mem[7].x) << "isless failed.";
    //ASSERT_EQ(f, mem[7].y) << "isless failed.";
    ASSERT_EQ(f, mem[8].x) << "isless failed.";
    ASSERT_EQ(f, mem[8].y) << "isless failed.";
    ASSERT_EQ(f, mem[9].x) << "isless failed.";
    ASSERT_EQ(t, mem[9].y) << "isless failed.";
    ASSERT_EQ(f, mem[10].x) << "isless failed.";
    ASSERT_EQ(f, mem[10].y) << "isless failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isless failed.";
    ASSERT_EQ(f, mem[0].y) << "isless failed.";
    ASSERT_EQ(f, mem[0].z) << "isless failed.";
    ASSERT_EQ(f, mem[1].x) << "isless failed.";
    ASSERT_EQ(f, mem[1].y) << "isless failed.";
    ASSERT_EQ(f, mem[1].z) << "isless failed.";
    ASSERT_EQ(t, mem[2].x) << "isless failed.";
    ASSERT_EQ(f, mem[2].y) << "isless failed.";
    ASSERT_EQ(t, mem[2].z) << "isless failed.";
    ASSERT_EQ(f, mem[3].x) << "isless failed.";
    ASSERT_EQ(f, mem[3].y) << "isless failed.";
    ASSERT_EQ(f, mem[3].z) << "isless failed.";
    ASSERT_EQ(f, mem[4].x) << "isless failed.";
    ASSERT_EQ(f, mem[4].y) << "isless failed.";
    ASSERT_EQ(f, mem[4].z) << "isless failed.";
    ASSERT_EQ(t, mem[5].x) << "isless failed.";
    ASSERT_EQ(f, mem[5].y) << "isless failed.";
    ASSERT_EQ(t, mem[5].z) << "isless failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isless failed.";
    //ASSERT_EQ(f, mem[6].y) << "isless failed.";
    //ASSERT_EQ(f, mem[6].z) << "isless failed.";
    //ASSERT_EQ(f, mem[7].x) << "isless failed.";
    //ASSERT_EQ(f, mem[7].y) << "isless failed.";
    //ASSERT_EQ(f, mem[7].z) << "isless failed.";
    ASSERT_EQ(f, mem[8].x) << "isless failed.";
    ASSERT_EQ(f, mem[8].y) << "isless failed.";
    ASSERT_EQ(f, mem[8].z) << "isless failed.";
    ASSERT_EQ(f, mem[9].x) << "isless failed.";
    ASSERT_EQ(t, mem[9].y) << "isless failed.";
    ASSERT_EQ(f, mem[9].z) << "isless failed.";
    ASSERT_EQ(f, mem[10].x) << "isless failed.";
    ASSERT_EQ(f, mem[10].y) << "isless failed.";
    ASSERT_EQ(f, mem[10].z) << "isless failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isless failed.";
    ASSERT_EQ(f, mem[0].y) << "isless failed.";
    ASSERT_EQ(f, mem[0].z) << "isless failed.";
    ASSERT_EQ(f, mem[0].w) << "isless failed.";
    ASSERT_EQ(f, mem[1].x) << "isless failed.";
    ASSERT_EQ(f, mem[1].y) << "isless failed.";
    ASSERT_EQ(f, mem[1].z) << "isless failed.";
    ASSERT_EQ(f, mem[1].w) << "isless failed.";
    ASSERT_EQ(t, mem[2].x) << "isless failed.";
    ASSERT_EQ(f, mem[2].y) << "isless failed.";
    ASSERT_EQ(t, mem[2].z) << "isless failed.";
    ASSERT_EQ(f, mem[2].w) << "isless failed.";
    ASSERT_EQ(f, mem[3].x) << "isless failed.";
    ASSERT_EQ(f, mem[3].y) << "isless failed.";
    ASSERT_EQ(f, mem[3].z) << "isless failed.";
    ASSERT_EQ(f, mem[3].w) << "isless failed.";
    ASSERT_EQ(f, mem[4].x) << "isless failed.";
    ASSERT_EQ(f, mem[4].y) << "isless failed.";
    ASSERT_EQ(f, mem[4].z) << "isless failed.";
    ASSERT_EQ(f, mem[4].w) << "isless failed.";
    ASSERT_EQ(t, mem[5].x) << "isless failed.";
    ASSERT_EQ(f, mem[5].y) << "isless failed.";
    ASSERT_EQ(t, mem[5].z) << "isless failed.";
    ASSERT_EQ(f, mem[5].w) << "isless failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "isless failed.";
    //ASSERT_EQ(f, mem[6].y) << "isless failed.";
    //ASSERT_EQ(f, mem[6].z) << "isless failed.";
    //ASSERT_EQ(f, mem[6].w) << "isless failed.";
    //ASSERT_EQ(f, mem[7].x) << "isless failed.";
    //ASSERT_EQ(f, mem[7].y) << "isless failed.";
    //ASSERT_EQ(f, mem[7].z) << "isless failed.";
    //ASSERT_EQ(f, mem[7].w) << "isless failed.";
    ASSERT_EQ(f, mem[8].x) << "isless failed.";
    ASSERT_EQ(f, mem[8].y) << "isless failed.";
    ASSERT_EQ(f, mem[8].z) << "isless failed.";
    ASSERT_EQ(f, mem[8].w) << "isless failed.";
    ASSERT_EQ(f, mem[9].x) << "isless failed.";
    ASSERT_EQ(t, mem[9].y) << "isless failed.";
    ASSERT_EQ(f, mem[9].z) << "isless failed.";
    ASSERT_EQ(t, mem[9].w) << "isless failed.";
    ASSERT_EQ(f, mem[10].x) << "isless failed.";
    ASSERT_EQ(f, mem[10].y) << "isless failed.";
    ASSERT_EQ(f, mem[10].z) << "isless failed.";
    ASSERT_EQ(f, mem[10].w) << "isless failed.";
  }
}

TEST(ClCppTest, RelationIsLessEqualTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessEqualTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessEqualTestKernel");
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
    ASSERT_EQ(t, mem[0]) << "islessequal failed.";
    ASSERT_EQ(t, mem[1]) << "islessequal failed.";
    ASSERT_EQ(t, mem[2]) << "islessequal failed.";
    ASSERT_EQ(t, mem[3]) << "islessequal failed.";
    ASSERT_EQ(t, mem[4]) << "islessequal failed.";
    ASSERT_EQ(t, mem[5]) << "islessequal failed.";
    ASSERT_EQ(t, mem[6]) << "islessequal failed.";
    ASSERT_EQ(t, mem[7]) << "islessequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[8]) << "islessequal failed.";
    //ASSERT_EQ(t, mem[9]) << "islessequal failed.";
    //ASSERT_EQ(f, mem[10]) << "islessequal failed.";
    //ASSERT_EQ(f, mem[11]) << "islessequal failed.";
    ASSERT_EQ(t, mem[12]) << "islessequal failed.";
    ASSERT_EQ(t, mem[13]) << "islessequal failed.";
    ASSERT_EQ(t, mem[14]) << "islessequal failed.";
    ASSERT_EQ(t, mem[15]) << "islessequal failed.";
    ASSERT_EQ(f, mem[16]) << "islessequal failed.";
    ASSERT_EQ(f, mem[17]) << "islessequal failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[2].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[2].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[5].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[5].y) << "islessequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[9].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[9].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].y) << "islessequal failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[2].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[2].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[2].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[5].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[5].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[5].z) << "islessequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].z) << "islessequal failed.";
    ASSERT_EQ(f, mem[9].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[9].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[9].z) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].z) << "islessequal failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[0].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[1].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[2].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[2].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[2].z) << "islessequal failed.";
    ASSERT_EQ(f, mem[2].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[3].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[4].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[5].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[5].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[5].z) << "islessequal failed.";
    ASSERT_EQ(f, mem[5].w) << "islessequal failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6].x) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].y) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].z) << "islessequal failed.";
    //ASSERT_EQ(t, mem[6].w) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].x) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].y) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].z) << "islessequal failed.";
    //ASSERT_EQ(f, mem[7].w) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].y) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[8].w) << "islessequal failed.";
    ASSERT_EQ(f, mem[9].x) << "islessequal failed.";
    ASSERT_EQ(t, mem[9].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[9].z) << "islessequal failed.";
    ASSERT_EQ(t, mem[9].w) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].x) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].y) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].z) << "islessequal failed.";
    ASSERT_EQ(f, mem[10].w) << "islessequal failed.";
  }
}

TEST(ClCppTest, RelationIsFiniteTest)
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
  buffer_out1->setSize(11);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_float2>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(7);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_float3>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(7);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_float4>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(7);

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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isFiniteTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isFiniteTestKernel");
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
    ASSERT_EQ(t, mem[0]) << "isfinite failed.";
    ASSERT_EQ(t, mem[1]) << "isfinite failed.";
    ASSERT_EQ(t, mem[2]) << "isfinite failed.";
    ASSERT_EQ(t, mem[3]) << "isfinite failed.";
    ASSERT_EQ(t, mem[4]) << "isfinite failed.";
    ASSERT_EQ(t, mem[5]) << "isfinite failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[6]) << "isfinite failed.";
    //ASSERT_EQ(t, mem[7]) << "isfinite failed.";
    ASSERT_EQ(f, mem[8]) << "isfinite failed.";
    ASSERT_EQ(f, mem[9]) << "isfinite failed.";
    ASSERT_EQ(f, mem[10]) << "isfinite failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].y) << "isfinite failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[4].x) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].y) << "isfinite failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].z) << "isfinite failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[4].x) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].y) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].z) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].z) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].z) << "isfinite failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(t, mem[0].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[0].w) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[1].w) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[2].w) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].x) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].y) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].z) << "isfinite failed.";
    ASSERT_EQ(t, mem[3].w) << "isfinite failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(t, mem[4].x) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].y) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].z) << "isfinite failed.";
    //ASSERT_EQ(t, mem[4].w) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].z) << "isfinite failed.";
    ASSERT_EQ(f, mem[5].w) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].x) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].y) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].z) << "isfinite failed.";
    ASSERT_EQ(f, mem[6].w) << "isfinite failed.";
  }
}

TEST(ClCppTest, RelationIsInfTest)
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
  buffer_out1->setSize(11);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_float2>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(7);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_float3>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(7);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_float4>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(7);

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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isInfTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isInfTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "isinf failed.";
    ASSERT_EQ(f, mem[1]) << "isinf failed.";
    ASSERT_EQ(f, mem[2]) << "isinf failed.";
    ASSERT_EQ(f, mem[3]) << "isinf failed.";
    ASSERT_EQ(f, mem[4]) << "isinf failed.";
    ASSERT_EQ(f, mem[5]) << "isinf failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6]) << "isinf failed.";
    //ASSERT_EQ(f, mem[7]) << "isinf failed.";
    ASSERT_EQ(t, mem[8]) << "isinf failed.";
    ASSERT_EQ(t, mem[9]) << "isinf failed.";
    ASSERT_EQ(f, mem[10]) << "isinf failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isinf failed.";
    ASSERT_EQ(f, mem[0].y) << "isinf failed.";
    ASSERT_EQ(f, mem[1].x) << "isinf failed.";
    ASSERT_EQ(f, mem[1].y) << "isinf failed.";
    ASSERT_EQ(f, mem[2].x) << "isinf failed.";
    ASSERT_EQ(f, mem[2].y) << "isinf failed.";
    ASSERT_EQ(f, mem[3].x) << "isinf failed.";
    ASSERT_EQ(f, mem[3].y) << "isinf failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].y) << "isinf failed.";
    ASSERT_EQ(t, mem[5].x) << "isinf failed.";
    ASSERT_EQ(t, mem[5].y) << "isinf failed.";
    ASSERT_EQ(f, mem[6].x) << "isinf failed.";
    ASSERT_EQ(f, mem[6].y) << "isinf failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isinf failed.";
    ASSERT_EQ(f, mem[0].y) << "isinf failed.";
    ASSERT_EQ(f, mem[0].z) << "isinf failed.";
    ASSERT_EQ(f, mem[1].x) << "isinf failed.";
    ASSERT_EQ(f, mem[1].y) << "isinf failed.";
    ASSERT_EQ(f, mem[1].z) << "isinf failed.";
    ASSERT_EQ(f, mem[2].x) << "isinf failed.";
    ASSERT_EQ(f, mem[2].y) << "isinf failed.";
    ASSERT_EQ(f, mem[2].z) << "isinf failed.";
    ASSERT_EQ(f, mem[3].x) << "isinf failed.";
    ASSERT_EQ(f, mem[3].y) << "isinf failed.";
    ASSERT_EQ(f, mem[3].z) << "isinf failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].y) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].z) << "isinf failed.";
    ASSERT_EQ(t, mem[5].x) << "isinf failed.";
    ASSERT_EQ(t, mem[5].y) << "isinf failed.";
    ASSERT_EQ(t, mem[5].z) << "isinf failed.";
    ASSERT_EQ(f, mem[6].x) << "isinf failed.";
    ASSERT_EQ(f, mem[6].y) << "isinf failed.";
    ASSERT_EQ(f, mem[6].z) << "isinf failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isinf failed.";
    ASSERT_EQ(f, mem[0].y) << "isinf failed.";
    ASSERT_EQ(f, mem[0].z) << "isinf failed.";
    ASSERT_EQ(f, mem[0].w) << "isinf failed.";
    ASSERT_EQ(f, mem[1].x) << "isinf failed.";
    ASSERT_EQ(f, mem[1].y) << "isinf failed.";
    ASSERT_EQ(f, mem[1].z) << "isinf failed.";
    ASSERT_EQ(f, mem[1].w) << "isinf failed.";
    ASSERT_EQ(f, mem[2].x) << "isinf failed.";
    ASSERT_EQ(f, mem[2].y) << "isinf failed.";
    ASSERT_EQ(f, mem[2].z) << "isinf failed.";
    ASSERT_EQ(f, mem[2].w) << "isinf failed.";
    ASSERT_EQ(f, mem[3].x) << "isinf failed.";
    ASSERT_EQ(f, mem[3].y) << "isinf failed.";
    ASSERT_EQ(f, mem[3].z) << "isinf failed.";
    ASSERT_EQ(f, mem[3].w) << "isinf failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].y) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].z) << "isinf failed.";
    //ASSERT_EQ(f, mem[4].w) << "isinf failed.";
    ASSERT_EQ(t, mem[5].x) << "isinf failed.";
    ASSERT_EQ(t, mem[5].y) << "isinf failed.";
    ASSERT_EQ(t, mem[5].z) << "isinf failed.";
    ASSERT_EQ(t, mem[5].w) << "isinf failed.";
    ASSERT_EQ(f, mem[6].x) << "isinf failed.";
    ASSERT_EQ(f, mem[6].y) << "isinf failed.";
    ASSERT_EQ(f, mem[6].z) << "isinf failed.";
    ASSERT_EQ(f, mem[6].w) << "isinf failed.";
  }
}

TEST(ClCppTest, RelationIsNanTest)
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
  buffer_out1->setSize(11);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_float2>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(7);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_float3>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(7);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_float4>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(7);

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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isNanTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isNanTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "isnan failed.";
    ASSERT_EQ(f, mem[1]) << "isnan failed.";
    ASSERT_EQ(f, mem[2]) << "isnan failed.";
    ASSERT_EQ(f, mem[3]) << "isnan failed.";
    ASSERT_EQ(f, mem[4]) << "isnan failed.";
    ASSERT_EQ(f, mem[5]) << "isnan failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6]) << "isnan failed.";
    //ASSERT_EQ(f, mem[7]) << "isnan failed.";
    ASSERT_EQ(f, mem[8]) << "isnan failed.";
    ASSERT_EQ(f, mem[9]) << "isnan failed.";
    ASSERT_EQ(t, mem[10]) << "isnan failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isnan failed.";
    ASSERT_EQ(f, mem[0].y) << "isnan failed.";
    ASSERT_EQ(f, mem[1].x) << "isnan failed.";
    ASSERT_EQ(f, mem[1].y) << "isnan failed.";
    ASSERT_EQ(f, mem[2].x) << "isnan failed.";
    ASSERT_EQ(f, mem[2].y) << "isnan failed.";
    ASSERT_EQ(f, mem[3].x) << "isnan failed.";
    ASSERT_EQ(f, mem[3].y) << "isnan failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].y) << "isnan failed.";
    ASSERT_EQ(f, mem[5].x) << "isnan failed.";
    ASSERT_EQ(f, mem[5].y) << "isnan failed.";
    ASSERT_EQ(t, mem[6].x) << "isnan failed.";
    ASSERT_EQ(t, mem[6].y) << "isnan failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isnan failed.";
    ASSERT_EQ(f, mem[0].y) << "isnan failed.";
    ASSERT_EQ(f, mem[0].z) << "isnan failed.";
    ASSERT_EQ(f, mem[1].x) << "isnan failed.";
    ASSERT_EQ(f, mem[1].y) << "isnan failed.";
    ASSERT_EQ(f, mem[1].z) << "isnan failed.";
    ASSERT_EQ(f, mem[2].x) << "isnan failed.";
    ASSERT_EQ(f, mem[2].y) << "isnan failed.";
    ASSERT_EQ(f, mem[2].z) << "isnan failed.";
    ASSERT_EQ(f, mem[3].x) << "isnan failed.";
    ASSERT_EQ(f, mem[3].y) << "isnan failed.";
    ASSERT_EQ(f, mem[3].z) << "isnan failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].y) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].z) << "isnan failed.";
    ASSERT_EQ(f, mem[5].x) << "isnan failed.";
    ASSERT_EQ(f, mem[5].y) << "isnan failed.";
    ASSERT_EQ(f, mem[5].z) << "isnan failed.";
    ASSERT_EQ(t, mem[6].x) << "isnan failed.";
    ASSERT_EQ(t, mem[6].y) << "isnan failed.";
    ASSERT_EQ(t, mem[6].z) << "isnan failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "isnan failed.";
    ASSERT_EQ(f, mem[0].y) << "isnan failed.";
    ASSERT_EQ(f, mem[0].z) << "isnan failed.";
    ASSERT_EQ(f, mem[0].w) << "isnan failed.";
    ASSERT_EQ(f, mem[1].x) << "isnan failed.";
    ASSERT_EQ(f, mem[1].y) << "isnan failed.";
    ASSERT_EQ(f, mem[1].z) << "isnan failed.";
    ASSERT_EQ(f, mem[1].w) << "isnan failed.";
    ASSERT_EQ(f, mem[2].x) << "isnan failed.";
    ASSERT_EQ(f, mem[2].y) << "isnan failed.";
    ASSERT_EQ(f, mem[2].z) << "isnan failed.";
    ASSERT_EQ(f, mem[2].w) << "isnan failed.";
    ASSERT_EQ(f, mem[3].x) << "isnan failed.";
    ASSERT_EQ(f, mem[3].y) << "isnan failed.";
    ASSERT_EQ(f, mem[3].z) << "isnan failed.";
    ASSERT_EQ(f, mem[3].w) << "isnan failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].y) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].z) << "isnan failed.";
    //ASSERT_EQ(f, mem[4].w) << "isnan failed.";
    ASSERT_EQ(f, mem[5].x) << "isnan failed.";
    ASSERT_EQ(f, mem[5].y) << "isnan failed.";
    ASSERT_EQ(f, mem[5].z) << "isnan failed.";
    ASSERT_EQ(f, mem[5].w) << "isnan failed.";
    ASSERT_EQ(t, mem[6].x) << "isnan failed.";
    ASSERT_EQ(t, mem[6].y) << "isnan failed.";
    ASSERT_EQ(t, mem[6].z) << "isnan failed.";
    ASSERT_EQ(t, mem[6].w) << "isnan failed.";
  }
}

TEST(ClCppTest, RelationSignbitTest)
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
  buffer_out1->setSize(11);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_float2>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(7);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_float3>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(7);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_float4>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(7);

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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, signbitTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("signbitTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "signbit failed.";
    ASSERT_EQ(f, mem[1]) << "signbit failed.";
    ASSERT_EQ(t, mem[2]) << "signbit failed.";
    ASSERT_EQ(f, mem[3]) << "signbit failed.";
    ASSERT_EQ(t, mem[4]) << "signbit failed.";
    ASSERT_EQ(f, mem[5]) << "signbit failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6]) << "signbit failed.";
    //ASSERT_EQ(f, mem[7]) << "signbit failed.";
    ASSERT_EQ(f, mem[8]) << "signbit failed.";
    ASSERT_EQ(t, mem[9]) << "signbit failed.";
    ASSERT_EQ(f, mem[10]) << "signbit failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "signbit failed.";
    ASSERT_EQ(t, mem[0].y) << "signbit failed.";
    ASSERT_EQ(f, mem[1].x) << "signbit failed.";
    ASSERT_EQ(t, mem[1].y) << "signbit failed.";
    ASSERT_EQ(f, mem[2].x) << "signbit failed.";
    ASSERT_EQ(t, mem[2].y) << "signbit failed.";
    ASSERT_EQ(f, mem[3].x) << "signbit failed.";
    ASSERT_EQ(t, mem[3].y) << "signbit failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].y) << "signbit failed.";
    ASSERT_EQ(f, mem[5].x) << "signbit failed.";
    ASSERT_EQ(t, mem[5].y) << "signbit failed.";
    ASSERT_EQ(f, mem[6].x) << "signbit failed.";
    ASSERT_EQ(f, mem[6].y) << "signbit failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "signbit failed.";
    ASSERT_EQ(t, mem[0].y) << "signbit failed.";
    ASSERT_EQ(f, mem[0].z) << "signbit failed.";
    ASSERT_EQ(f, mem[1].x) << "signbit failed.";
    ASSERT_EQ(t, mem[1].y) << "signbit failed.";
    ASSERT_EQ(f, mem[1].z) << "signbit failed.";
    ASSERT_EQ(f, mem[2].x) << "signbit failed.";
    ASSERT_EQ(t, mem[2].y) << "signbit failed.";
    ASSERT_EQ(f, mem[2].z) << "signbit failed.";
    ASSERT_EQ(f, mem[3].x) << "signbit failed.";
    ASSERT_EQ(t, mem[3].y) << "signbit failed.";
    ASSERT_EQ(f, mem[3].z) << "signbit failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].y) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].z) << "signbit failed.";
    ASSERT_EQ(f, mem[5].x) << "signbit failed.";
    ASSERT_EQ(t, mem[5].y) << "signbit failed.";
    ASSERT_EQ(f, mem[5].z) << "signbit failed.";
    ASSERT_EQ(f, mem[6].x) << "signbit failed.";
    ASSERT_EQ(f, mem[6].y) << "signbit failed.";
    ASSERT_EQ(f, mem[6].z) << "signbit failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "signbit failed.";
    ASSERT_EQ(t, mem[0].y) << "signbit failed.";
    ASSERT_EQ(f, mem[0].z) << "signbit failed.";
    ASSERT_EQ(t, mem[0].w) << "signbit failed.";
    ASSERT_EQ(f, mem[1].x) << "signbit failed.";
    ASSERT_EQ(t, mem[1].y) << "signbit failed.";
    ASSERT_EQ(f, mem[1].z) << "signbit failed.";
    ASSERT_EQ(t, mem[1].w) << "signbit failed.";
    ASSERT_EQ(f, mem[2].x) << "signbit failed.";
    ASSERT_EQ(t, mem[2].y) << "signbit failed.";
    ASSERT_EQ(f, mem[2].z) << "signbit failed.";
    ASSERT_EQ(t, mem[2].w) << "signbit failed.";
    ASSERT_EQ(f, mem[3].x) << "signbit failed.";
    ASSERT_EQ(t, mem[3].y) << "signbit failed.";
    ASSERT_EQ(f, mem[3].z) << "signbit failed.";
    ASSERT_EQ(t, mem[3].w) << "signbit failed.";
    //! \todo subnormal won't work on GPU
    //ASSERT_EQ(f, mem[4].x) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].y) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].z) << "signbit failed.";
    //ASSERT_EQ(f, mem[4].w) << "signbit failed.";
    ASSERT_EQ(f, mem[5].x) << "signbit failed.";
    ASSERT_EQ(t, mem[5].y) << "signbit failed.";
    ASSERT_EQ(f, mem[5].z) << "signbit failed.";
    ASSERT_EQ(t, mem[5].w) << "signbit failed.";
    ASSERT_EQ(f, mem[6].x) << "signbit failed.";
    ASSERT_EQ(f, mem[6].y) << "signbit failed.";
    ASSERT_EQ(f, mem[6].z) << "signbit failed.";
    ASSERT_EQ(f, mem[6].w) << "signbit failed.";
  }
}
