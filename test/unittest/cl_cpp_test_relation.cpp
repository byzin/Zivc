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
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test_relation.hpp"

#define ZIVC_TEST_V8(v0, v1, v2, v3, mem, test_name) \
    static_assert(std::remove_cvref_t<decltype(mem)>::size() == 8); \
    ASSERT_EQ((v0), (mem).s0) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).s1) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).s2) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).s3) << (test_name) << " failed."; \
    ASSERT_EQ((v0), (mem).s4) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).s5) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).s6) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).s7) << (test_name) << " failed."

#define ZIVC_TEST_V16(v0, v1, v2, v3, mem, test_name) \
    static_assert(std::remove_cvref_t<decltype(mem)>::size() == 16); \
    ASSERT_EQ((v0), (mem).s0) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).s1) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).s2) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).s3) << (test_name) << " failed."; \
    ASSERT_EQ((v0), (mem).s4) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).s5) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).s6) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).s7) << (test_name) << " failed."; \
    ASSERT_EQ((v0), (mem).s8) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).s9) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).sa) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).sb) << (test_name) << " failed."; \
    ASSERT_EQ((v0), (mem).sc) << (test_name) << " failed."; \
    ASSERT_EQ((v1), (mem).sd) << (test_name) << " failed."; \
    ASSERT_EQ((v2), (mem).se) << (test_name) << " failed."; \
    ASSERT_EQ((v3), (mem).sf) << (test_name) << " failed."


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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsEqualLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();

  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isEqualLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isEqualLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(t, t, t, t, mem[0], "isequal");
    ZIVC_TEST_V8(t, t, t, t, mem[1], "isequal");
    ZIVC_TEST_V8(f, f, f, f, mem[2], "isequal");
    ZIVC_TEST_V8(t, t, t, t, mem[3], "isequal");
    ZIVC_TEST_V8(t, t, t, t, mem[4], "isequal");
    ZIVC_TEST_V8(f, f, f, f, mem[5], "isequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(t, t, t, t, mem[6], "isequal");
    //ZIVC_TEST_V8(f, f, f, f, mem[7], "isequal");
    ZIVC_TEST_V8(t, t, t, t, mem[8], "isequal");
    ZIVC_TEST_V8(f, f, f, f, mem[9], "isequal");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "isequal");
  }
  // output16
  {
    constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(t, t, t, t, mem[0], "isequal");
    ZIVC_TEST_V16(t, t, t, t, mem[1], "isequal");
    ZIVC_TEST_V16(f, f, f, f, mem[2], "isequal");
    ZIVC_TEST_V16(t, t, t, t, mem[3], "isequal");
    ZIVC_TEST_V16(t, t, t, t, mem[4], "isequal");
    ZIVC_TEST_V16(f, f, f, f, mem[5], "isequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(t, t, t, t, mem[6], "isequal");
    //ZIVC_TEST_V16(f, f, f, f, mem[7], "isequal");
    ZIVC_TEST_V16(t, t, t, t, mem[8], "isequal");
    ZIVC_TEST_V16(f, f, f, f, mem[9], "isequal");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "isequal");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsNotEqualLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isNotEqualLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isNotEqualLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    constexpr cl_int tl = zivc::cl::kVTrue;
    constexpr cl_int fl = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(fl, fl, fl, fl, mem[0], "isnotequal");
    ZIVC_TEST_V8(fl, fl, fl, fl, mem[1], "isnotequal");
    ZIVC_TEST_V8(tl, tl, tl, tl, mem[2], "isnotequal");
    ZIVC_TEST_V8(fl, fl, fl, fl, mem[3], "isnotequal");
    ZIVC_TEST_V8(fl, fl, fl, fl, mem[4], "isnotequal");
    ZIVC_TEST_V8(tl, tl, tl, tl, mem[5], "isnotequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(fl, fl, fl, fl, mem[6], "isnotequal");
    //ZIVC_TEST_V8(tl, tl, tl, tl, mem[7], "isnotequal");
    ZIVC_TEST_V8(fl, fl, fl, fl, mem[8], "isnotequal");
    ZIVC_TEST_V8(tl, tl, tl, tl, mem[9], "isnotequal");
    ZIVC_TEST_V8(tl, tl, tl, tl, mem[10], "isnotequal");
  }
  // output16
  {
    constexpr cl_int tl = zivc::cl::kVTrue;
    constexpr cl_int fl = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(fl, fl, fl, fl, mem[0], "isnotequal");
    ZIVC_TEST_V16(fl, fl, fl, fl, mem[1], "isnotequal");
    ZIVC_TEST_V16(tl, tl, tl, tl, mem[2], "isnotequal");
    ZIVC_TEST_V16(fl, fl, fl, fl, mem[3], "isnotequal");
    ZIVC_TEST_V16(fl, fl, fl, fl, mem[4], "isnotequal");
    ZIVC_TEST_V16(tl, tl, tl, tl, mem[5], "isnotequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(fl, fl, fl, fl, mem[6], "isnotequal");
    //ZIVC_TEST_V16(tl, tl, tl, tl, mem[7], "isnotequal");
    ZIVC_TEST_V16(fl, fl, fl, fl, mem[8], "isnotequal");
    ZIVC_TEST_V16(tl, tl, tl, tl, mem[9], "isnotequal");
    ZIVC_TEST_V16(tl, tl, tl, tl, mem[10], "isnotequal");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsGreaterLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isGreaterLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isGreaterLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, f, f, f, mem[0], "isgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[1], "isgreater");
    ZIVC_TEST_V8(f, t, f, t, mem[2], "isgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[3], "isgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[4], "isgreater");
    ZIVC_TEST_V8(f, t, f, t, mem[5], "isgreater");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[6], "isgreater");
    //ZIVC_TEST_V8(t, t, t, t, mem[7], "isgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[8], "isgreater");
    ZIVC_TEST_V8(t, f, t, f, mem[9], "isgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "isgreater");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, f, f, f, mem[0], "isgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[1], "isgreater");
    ZIVC_TEST_V16(f, t, f, t, mem[2], "isgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[3], "isgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[4], "isgreater");
    ZIVC_TEST_V16(f, t, f, t, mem[5], "isgreater");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[6], "isgreater");
    //ZIVC_TEST_V16(t, t, t, t, mem[7], "isgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[8], "isgreater");
    ZIVC_TEST_V16(t, f, t, f, mem[9], "isgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "isgreater");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsGreaterLongVecEqualTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isGreaterEqualLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isGreaterEqualLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(t, t, t, t, mem[0], "isgreaterequal");
    ZIVC_TEST_V8(t, t, t, t, mem[1], "isgreaterequal");
    ZIVC_TEST_V8(f, t, f, t, mem[2], "isgreaterequal");
    ZIVC_TEST_V8(t, t, t, t, mem[3], "isgreaterequal");
    ZIVC_TEST_V8(t, t, t, t, mem[4], "isgreaterequal");
    ZIVC_TEST_V8(f, t, f, t, mem[5], "isgreaterequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(t, t, t, t, mem[6], "isgreaterequal");
    //ZIVC_TEST_V8(t, t, t, t, mem[7], "isgreaterequal");
    ZIVC_TEST_V8(t, t, t, t, mem[8], "isgreaterequal");
    ZIVC_TEST_V8(t, f, t, f, mem[9], "isgreaterequal");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "isgreaterequal");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(t, t, t, t, mem[0], "isgreaterequal");
    ZIVC_TEST_V16(t, t, t, t, mem[1], "isgreaterequal");
    ZIVC_TEST_V16(f, t, f, t, mem[2], "isgreaterequal");
    ZIVC_TEST_V16(t, t, t, t, mem[3], "isgreaterequal");
    ZIVC_TEST_V16(t, t, t, t, mem[4], "isgreaterequal");
    ZIVC_TEST_V16(f, t, f, t, mem[5], "isgreaterequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(t, t, t, t, mem[6], "isgreaterequal");
    //ZIVC_TEST_V16(t, t, t, t, mem[7], "isgreaterequal");
    ZIVC_TEST_V16(t, t, t, t, mem[8], "isgreaterequal");
    ZIVC_TEST_V16(t, f, t, f, mem[9], "isgreaterequal");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "isgreaterequal");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsLessLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, f, f, f, mem[0], "isless");
    ZIVC_TEST_V8(f, f, f, f, mem[1], "isless");
    ZIVC_TEST_V8(t, f, t, f, mem[2], "isless");
    ZIVC_TEST_V8(f, f, f, f, mem[3], "isless");
    ZIVC_TEST_V8(f, f, f, f, mem[4], "isless");
    ZIVC_TEST_V8(t, f, t, f, mem[5], "isless");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[6], "isless");
    //ZIVC_TEST_V8(f, f, f, f, mem[7], "isless");
    ZIVC_TEST_V8(f, f, f, f, mem[8], "isless");
    ZIVC_TEST_V8(f, t, f, t, mem[9], "isless");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "isless");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, f, f, f, mem[0], "isless");
    ZIVC_TEST_V16(f, f, f, f, mem[1], "isless");
    ZIVC_TEST_V16(t, f, t, f, mem[2], "isless");
    ZIVC_TEST_V16(f, f, f, f, mem[3], "isless");
    ZIVC_TEST_V16(f, f, f, f, mem[4], "isless");
    ZIVC_TEST_V16(t, f, t, f, mem[5], "isless");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[6], "isless");
    //ZIVC_TEST_V16(f, f, f, f, mem[7], "isless");
    ZIVC_TEST_V16(f, f, f, f, mem[8], "isless");
    ZIVC_TEST_V16(f, t, f, t, mem[9], "isless");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "isless");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsLessEqualLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessEqualLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessEqualLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(t, t, t, t, mem[0], "islessequal");
    ZIVC_TEST_V8(t, t, t, t, mem[1], "islessequal");
    ZIVC_TEST_V8(t, f, t, f, mem[2], "islessequal");
    ZIVC_TEST_V8(t, t, t, t, mem[3], "islessequal");
    ZIVC_TEST_V8(t, t, t, t, mem[4], "islessequal");
    ZIVC_TEST_V8(t, f, t, f, mem[5], "islessequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(t, t, t, t, mem[6], "islessequal");
    //ZIVC_TEST_V8(f, f, f, f, mem[7], "islessequal");
    ZIVC_TEST_V8(t, t, t, t, mem[8], "islessequal");
    ZIVC_TEST_V8(f, t, f, t, mem[9], "islessequal");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "islessequal");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(t, t, t, t, mem[0], "islessequal");
    ZIVC_TEST_V16(t, t, t, t, mem[1], "islessequal");
    ZIVC_TEST_V16(t, f, t, f, mem[2], "islessequal");
    ZIVC_TEST_V16(t, t, t, t, mem[3], "islessequal");
    ZIVC_TEST_V16(t, t, t, t, mem[4], "islessequal");
    ZIVC_TEST_V16(t, f, t, f, mem[5], "islessequal");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(t, t, t, t, mem[6], "islessequal");
    //ZIVC_TEST_V16(f, f, f, f, mem[7], "islessequal");
    ZIVC_TEST_V16(t, t, t, t, mem[8], "islessequal");
    ZIVC_TEST_V16(f, t, f, t, mem[9], "islessequal");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "islessequal");
  }
}

TEST(ClCppTest, RelationIsLessGreaterTest)
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
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessGreaterTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessGreaterTestKernel");
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
    ASSERT_EQ(f, mem[0]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1]) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[5]) << "islessgreater failed.";
    ASSERT_EQ(t, mem[6]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[7]) << "islessgreater failed.";
    //! \attention subnormal won't work on GPU
    //ASSERT_EQ(f, mem[8]) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[9]) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[10]) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[11]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[12]) << "islessgreater failed.";
    ASSERT_EQ(t, mem[13]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[14]) << "islessgreater failed.";
    ASSERT_EQ(t, mem[15]) << "islessgreater failed.";
    ASSERT_EQ(t, mem[16]) << "islessgreater failed.";
    ASSERT_EQ(f, mem[17]) << "islessgreater failed.";
  }
  // output2
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "islessgreater failed.";
    //! \attention subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "islessgreater failed.";
  }
  // output3
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].z) << "islessgreater failed.";
    //! \attention subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].z) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].z) << "islessgreater failed.";
  }
  // output4
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    ASSERT_EQ(f, mem[0].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[0].w) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[1].w) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[2].w) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[3].w) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[4].w) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[5].w) << "islessgreater failed.";
    //! \attention subnormal won't work on GPU
    //ASSERT_EQ(f, mem[6].x) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].y) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].z) << "islessgreater failed.";
    //ASSERT_EQ(f, mem[6].w) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].x) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].y) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].z) << "islessgreater failed.";
    //ASSERT_EQ(t, mem[7].w) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[8].w) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].x) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].y) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].z) << "islessgreater failed.";
    ASSERT_EQ(t, mem[9].w) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].x) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].y) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].z) << "islessgreater failed.";
    ASSERT_EQ(f, mem[10].w) << "islessgreater failed.";
  }
}

TEST(ClCppTest, RelationIsLessGreaterLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(11);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(11);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isLessGreaterLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isLessGreaterLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, f, f, f, mem[0], "islessgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[1], "islessgreater");
    ZIVC_TEST_V8(t, t, t, t, mem[2], "islessgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[3], "islessgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[4], "islessgreater");
    ZIVC_TEST_V8(t, t, t, t, mem[5], "islessgreater");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[6], "islessgreater");
    //ZIVC_TEST_V8(t, t, t, t, mem[7], "islessgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[8], "islessgreater");
    ZIVC_TEST_V8(t, t, t, t, mem[9], "islessgreater");
    ZIVC_TEST_V8(f, f, f, f, mem[10], "islessgreater");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, f, f, f, mem[0], "islessgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[1], "islessgreater");
    ZIVC_TEST_V16(t, t, t, t, mem[2], "islessgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[3], "islessgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[4], "islessgreater");
    ZIVC_TEST_V16(t, t, t, t, mem[5], "islessgreater");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[6], "islessgreater");
    //ZIVC_TEST_V16(t, t, t, t, mem[7], "islessgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[8], "islessgreater");
    ZIVC_TEST_V16(t, t, t, t, mem[9], "islessgreater");
    ZIVC_TEST_V16(f, f, f, f, mem[10], "islessgreater");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsFiniteLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(7);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(7);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isFiniteLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isFiniteLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(t, t, t, t, mem[0], "isfinite");
    ZIVC_TEST_V8(t, t, t, t, mem[1], "isfinite");
    ZIVC_TEST_V8(t, t, t, t, mem[2], "isfinite");
    ZIVC_TEST_V8(t, t, t, t, mem[3], "isfinite");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(t, t, t, t, mem[4], "isfinite");
    ZIVC_TEST_V8(f, f, f, f, mem[5], "isfinite");
    ZIVC_TEST_V8(f, f, f, f, mem[6], "isfinite");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(t, t, t, t, mem[0], "isfinite");
    ZIVC_TEST_V16(t, t, t, t, mem[1], "isfinite");
    ZIVC_TEST_V16(t, t, t, t, mem[2], "isfinite");
    ZIVC_TEST_V16(t, t, t, t, mem[3], "isfinite");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(t, t, t, t, mem[4], "isfinite");
    ZIVC_TEST_V16(f, f, f, f, mem[5], "isfinite");
    ZIVC_TEST_V16(f, f, f, f, mem[6], "isfinite");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsInfLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(7);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(7);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isInfLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isInfLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, f, f, f, mem[0], "isinf");
    ZIVC_TEST_V8(f, f, f, f, mem[1], "isinf");
    ZIVC_TEST_V8(f, f, f, f, mem[2], "isinf");
    ZIVC_TEST_V8(f, f, f, f, mem[3], "isinf");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[4], "isinf");
    ZIVC_TEST_V8(t, t, t, t, mem[5], "isinf");
    ZIVC_TEST_V8(f, f, f, f, mem[6], "isinf");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, f, f, f, mem[0], "isinf");
    ZIVC_TEST_V16(f, f, f, f, mem[1], "isinf");
    ZIVC_TEST_V16(f, f, f, f, mem[2], "isinf");
    ZIVC_TEST_V16(f, f, f, f, mem[3], "isinf");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[4], "isinf");
    ZIVC_TEST_V16(t, t, t, t, mem[5], "isinf");
    ZIVC_TEST_V16(f, f, f, f, mem[6], "isinf");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationIsNanLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(7);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(7);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, isNanLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("isNanLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, f, f, f, mem[0], "isnan");
    ZIVC_TEST_V8(f, f, f, f, mem[1], "isnan");
    ZIVC_TEST_V8(f, f, f, f, mem[2], "isnan");
    ZIVC_TEST_V8(f, f, f, f, mem[3], "isnan");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[4], "isnan");
    ZIVC_TEST_V8(f, f, f, f, mem[5], "isnan");
    ZIVC_TEST_V8(t, t, t, t, mem[6], "isnan");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, f, f, f, mem[0], "isnan");
    ZIVC_TEST_V16(f, f, f, f, mem[1], "isnan");
    ZIVC_TEST_V16(f, f, f, f, mem[2], "isnan");
    ZIVC_TEST_V16(f, f, f, f, mem[3], "isnan");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[4], "isnan");
    ZIVC_TEST_V16(f, f, f, f, mem[5], "isnan");
    ZIVC_TEST_V16(t, t, t, t, mem[6], "isnan");
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
  using zivc::cl_float8;
  using zivc::cl_float16;

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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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
    //! \attention subnormal won't work on GPU
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

TEST(ClCppTest, RelationSignbitLongVecTest)
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

  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<zivc::cl_CompResult<cl_float8>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(7);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<zivc::cl_CompResult<cl_float16>>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(7);

  constexpr cl_float fmax = (std::numeric_limits<cl_float>::max)();
  constexpr cl_float fmin = (std::numeric_limits<cl_float>::min)();
  constexpr cl_float subnormal_min = std::numeric_limits<cl_float>::denorm_min();
  constexpr cl_float subnormal_max = fmin - subnormal_min;
  constexpr cl_float finf = std::numeric_limits<cl_float>::infinity();
  const cl_float fnan = std::numeric_limits<cl_float>::quiet_NaN();
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{0.0f, -0.0f, 0.0f, -0.0f,
                  0.0f, -0.0f, 0.0f, -0.0f},
        cl_float8{1.0f, -1.0f, 1.0f, -1.0f,
                  1.0f, -1.0f, 1.0f, -1.0f},
        cl_float8{fmax, -fmax, fmax, -fmax,
                  fmax, -fmax, fmax, -fmax},
        cl_float8{fmin, -fmin, fmin, -fmin,
                  fmin, -fmin, fmin, -fmin},
        cl_float8{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                  subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float8{finf, -finf, finf, -finf,
                  finf, -finf, finf, -finf},
        cl_float8{fnan, fnan, fnan, fnan,
                  fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
  }
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f,
                   0.0f, -0.0f, 0.0f, -0.0f},
        cl_float16{1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f,
                   1.0f, -1.0f, 1.0f, -1.0f},
        cl_float16{fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax,
                   fmax, -fmax, fmax, -fmax},
        cl_float16{fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin,
                   fmin, -fmin, fmin, -fmin},
        cl_float16{subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min,
                   subnormal_max, subnormal_min, subnormal_max, subnormal_min},
        cl_float16{finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf,
                   finf, -finf, finf, -finf},
        cl_float16{fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan,
                   fnan, fnan, fnan, fnan}};
    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, signbitLongVecTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("signbitLongVecTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out8, *buffer_out16, launch_options);
  device->waitForCompletion(result.fence());

  using zivc::cl_int;

  // output8
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    ZIVC_TEST_V8(f, t, f, t, mem[0], "signbit");
    ZIVC_TEST_V8(f, t, f, t, mem[1], "signbit");
    ZIVC_TEST_V8(f, t, f, t, mem[2], "signbit");
    ZIVC_TEST_V8(f, t, f, t, mem[3], "signbit");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V8(f, f, f, f, mem[4], "signbit");
    ZIVC_TEST_V8(f, t, f, t, mem[5], "signbit");
    ZIVC_TEST_V8(f, f, f, f, mem[6], "signbit");
  }
  // output16
  {
    [[maybe_unused]] constexpr cl_int t = zivc::cl::kVTrue;
    constexpr cl_int f = zivc::cl::kVFalse;
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    ZIVC_TEST_V16(f, t, f, t, mem[0], "signbit");
    ZIVC_TEST_V16(f, t, f, t, mem[1], "signbit");
    ZIVC_TEST_V16(f, t, f, t, mem[2], "signbit");
    ZIVC_TEST_V16(f, t, f, t, mem[3], "signbit");
    //! \attention subnormal won't work on GPU
    //ZIVC_TEST_V16(f, f, f, f, mem[4], "signbit");
    ZIVC_TEST_V16(f, t, f, t, mem[5], "signbit");
    ZIVC_TEST_V16(f, f, f, f, mem[6], "signbit");
  }
}

TEST(ClCppTest, RelationAnyTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_char;
  using zivc::cl_short;
  using zivc::cl_short2;
  using zivc::cl_int;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;

  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_char>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_short2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::int32b>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(15);

  {
    constexpr cl_char imax = (std::numeric_limits<cl_char>::max)();
    constexpr cl_char imin = (std::numeric_limits<cl_char>::min)();
    const std::initializer_list<cl_char> v = {
        0u,
        imax,
        imin 
    };
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  {
    constexpr cl_short imax = (std::numeric_limits<cl_short>::max)();
    constexpr cl_short imin = (std::numeric_limits<cl_short>::min)();
    const std::initializer_list<cl_short2> v = {
        cl_short2{0, 0},
        cl_short2{imax, imax},
        cl_short2{-1, 0},
        cl_short2{imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  {
    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int3> v = {
        cl_int3{0, 0, 0},
        cl_int3{imax, imax, imax},
        cl_int3{0, -1, 0},
        cl_int3{imin, imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  {
    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int4> v = {
        cl_int4{0, 0, 0, 0},
        cl_int4{imax, imax, imax, imax},
        cl_int4{0, 0, 0, -1},
        cl_int4{imin, imin, imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, anyTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("anyTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_in4, *buffer_out1, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    constexpr cl_int t = zivc::cl::kSTrue;
    constexpr cl_int f = zivc::cl::kSFalse;
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    // scalar
    ASSERT_EQ(f, mem[0]) << "any failed.";
    ASSERT_EQ(f, mem[1]) << "any failed.";
    ASSERT_EQ(t, mem[2]) << "any failed.";
    // vector2
    ASSERT_EQ(f, mem[3]) << "any failed.";
    ASSERT_EQ(f, mem[4]) << "any failed.";
    ASSERT_EQ(t, mem[5]) << "any failed.";
    ASSERT_EQ(t, mem[6]) << "any failed.";
    // vector3
    ASSERT_EQ(f, mem[7]) << "any failed.";
    ASSERT_EQ(f, mem[8]) << "any failed.";
    ASSERT_EQ(t, mem[9]) << "any failed.";
    ASSERT_EQ(t, mem[10]) << "any failed.";
    // vector4
    ASSERT_EQ(f, mem[11]) << "any failed.";
    ASSERT_EQ(f, mem[12]) << "any failed.";
    ASSERT_EQ(t, mem[13]) << "any failed.";
    ASSERT_EQ(t, mem[14]) << "any failed.";
  }
}

//! \todo Resolve the cl assertion
//TEST(ClCppTest, RelationAnyLongVecTest)
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
//  using zivc::cl_char;
//  using zivc::cl_short;
//  using zivc::cl_short2;
//  using zivc::cl_int;
//  using zivc::cl_int3;
//  using zivc::cl_int4;
//  using zivc::cl_int8;
//  using zivc::cl_int16;
//
//  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_int8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_int16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::int32b>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  buffer_out1->setSize(8);
//
//  {
//    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
//    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
//    const std::initializer_list<cl_int8> v = {
//        cl_int8{0, 0, 0, 0,
//                0, 0, 0, 0},
//        cl_int8{imax, imax, imax, imax,
//                imax, imax, imax, imax},
//        cl_int8{0, 0, 0, -1,
//                0, 0, 0, -1},
//        cl_int8{imin, imin, imin, imin,
//                imin, imin, imin, imin}
//    };
//    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
//  }
//  {
//    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
//    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
//    const std::initializer_list<cl_int16> v = {
//        cl_int16{0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0},
//        cl_int16{imax, imax, imax, imax,
//                 imax, imax, imax, imax,
//                 imax, imax, imax, imax,
//                 imax, imax, imax, imax},
//        cl_int16{0, 0, 0, -1,
//                 0, 0, 0, -1,
//                 0, 0, 0, -1,
//                 0, 0, 0, -1},
//        cl_int16{imin, imin, imin, imin,
//                 imin, imin, imin, imin,
//                 imin, imin, imin, imin,
//                 imin, imin, imin, imin}
//    };
//    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
//  }
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, anyLongVecTestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{1}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("anyLongVecTestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(1, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out1, launch_options);
//  device->waitForCompletion(result.fence());
//
//  using zivc::cl_int;
//
//  // output1
//  {
//    constexpr cl_int t = zivc::cl::kSTrue;
//    constexpr cl_int f = zivc::cl::kSFalse;
//    const zivc::MappedMemory mem = buffer_out1->mapMemory();
//    // vector8
//    ASSERT_EQ(f, mem[0]) << "any failed.";
//    ASSERT_EQ(f, mem[1]) << "any failed.";
//    ASSERT_EQ(t, mem[2]) << "any failed.";
//    ASSERT_EQ(t, mem[3]) << "any failed.";
//    // vector16
//    ASSERT_EQ(f, mem[4]) << "any failed.";
//    ASSERT_EQ(f, mem[5]) << "any failed.";
//    ASSERT_EQ(t, mem[6]) << "any failed.";
//    ASSERT_EQ(t, mem[7]) << "any failed.";
//  }
//}

TEST(ClCppTest, RelationAllTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_char;
  using zivc::cl_short;
  using zivc::cl_short2;
  using zivc::cl_int;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;

  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_char>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_short2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::int32b>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(23);

  {
    constexpr cl_char imax = (std::numeric_limits<cl_char>::max)();
    constexpr cl_char imin = (std::numeric_limits<cl_char>::min)();
    const std::initializer_list<cl_char> v = {
        0u,
        imax,
        imin 
    };
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  {
    constexpr cl_short imax = (std::numeric_limits<cl_short>::max)();
    constexpr cl_short imin = (std::numeric_limits<cl_short>::min)();
    const std::initializer_list<cl_short2> v = {
        cl_short2{0, 0},
        cl_short2{imax, imax},
        cl_short2{-1, 0},
        cl_short2{imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  {
    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int3> v = {
        cl_int3{0, 0, 0},
        cl_int3{imax, imax, imax},
        cl_int3{0, -1, 0},
        cl_int3{imin, imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  {
    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int4> v = {
        cl_int4{0, 0, 0, 0},
        cl_int4{imax, imax, imax, imax},
        cl_int4{0, 0, 0, -1},
        cl_int4{imin, imin, imin, imin}
    };
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, allTestKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("allTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_in4, *buffer_out1, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    constexpr cl_int t = zivc::cl::kSTrue;
    constexpr cl_int f = zivc::cl::kSFalse;
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    // scalar
    ASSERT_EQ(f, mem[0]) << "all failed.";
    ASSERT_EQ(f, mem[1]) << "all failed.";
    ASSERT_EQ(t, mem[2]) << "all failed.";
    // vector2
    ASSERT_EQ(f, mem[3]) << "all failed.";
    ASSERT_EQ(f, mem[4]) << "all failed.";
    ASSERT_EQ(f, mem[5]) << "all failed.";
    ASSERT_EQ(t, mem[6]) << "all failed.";
    // vector3
    ASSERT_EQ(f, mem[7]) << "all failed.";
    ASSERT_EQ(f, mem[8]) << "all failed.";
    ASSERT_EQ(f, mem[9]) << "all failed.";
    ASSERT_EQ(t, mem[10]) << "all failed.";
    // vector4
    ASSERT_EQ(f, mem[11]) << "all failed.";
    ASSERT_EQ(f, mem[12]) << "all failed.";
    ASSERT_EQ(f, mem[13]) << "all failed.";
    ASSERT_EQ(t, mem[14]) << "all failed.";
  }
}

//! \todo Resolve the cl assertion
//TEST(ClCppTest, RelationAllLongVecTest)
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
//  using zivc::cl_char;
//  using zivc::cl_short;
//  using zivc::cl_short2;
//  using zivc::cl_int;
//  using zivc::cl_int3;
//  using zivc::cl_int4;
//  using zivc::cl_int8;
//  using zivc::cl_int16;
//
//  const zivc::SharedBuffer buffer_in8 = device->createBuffer<cl_int8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  const zivc::SharedBuffer buffer_in16 = device->createBuffer<cl_int16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::int32b>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
//  buffer_out1->setSize(8);
//
//  {
//    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
//    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
//    const std::initializer_list<cl_int8> v = {
//        cl_int8{0, 0, 0, 0,
//                0, 0, 0, 0},
//        cl_int8{imax, imax, imax, imax,
//                imax, imax, imax, imax},
//        cl_int8{0, 0, 0, -1,
//                0, 0, 0, -1},
//        cl_int8{imin, imin, imin, imin,
//                imin, imin, imin, imin}
//    };
//    ztest::setDeviceBuffer(*device, v, buffer_in8.get());
//  }
//  {
//    constexpr cl_int imax = (std::numeric_limits<cl_int>::max)();
//    constexpr cl_int imin = (std::numeric_limits<cl_int>::min)();
//    const std::initializer_list<cl_int16> v = {
//        cl_int16{0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0},
//        cl_int16{imax, imax, imax, imax,
//                 imax, imax, imax, imax,
//                 imax, imax, imax, imax,
//                 imax, imax, imax, imax},
//        cl_int16{0, 0, 0, -1,
//                 0, 0, 0, -1,
//                 0, 0, 0, -1,
//                 0, 0, 0, -1},
//        cl_int16{imin, imin, imin, imin,
//                 imin, imin, imin, imin,
//                 imin, imin, imin, imin,
//                 imin, imin, imin, imin}
//    };
//    ztest::setDeviceBuffer(*device, v, buffer_in16.get());
//  }
//
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_relation, allLongVecTestKernel, 1);
//  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
//  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
//  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";
//
//  // Launch the kernel
//  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
//  launch_options.setQueueIndex(0);
//  launch_options.setWorkSize({{1}});
//  launch_options.requestFence(true);
//  launch_options.setLabel("allLongVecTestKernel");
//  ASSERT_EQ(1, launch_options.dimension());
//  ASSERT_EQ(3, launch_options.numOfArgs());
//  ASSERT_EQ(0, launch_options.queueIndex());
//  ASSERT_EQ(1, launch_options.workSize()[0]);
//  ASSERT_TRUE(launch_options.isFenceRequested());
//  zivc::LaunchResult result = kernel->run(*buffer_in8, *buffer_in16, *buffer_out1, launch_options);
//  device->waitForCompletion(result.fence());
//
//  using zivc::cl_int;
//
//  // output1
//  {
//    constexpr cl_int t = zivc::cl::kSTrue;
//    constexpr cl_int f = zivc::cl::kSFalse;
//    const zivc::MappedMemory mem = buffer_out1->mapMemory();
//    // vector8
//    ASSERT_EQ(f, mem[0]) << "all failed.";
//    ASSERT_EQ(f, mem[1]) << "all failed.";
//    ASSERT_EQ(f, mem[2]) << "all failed.";
//    ASSERT_EQ(t, mem[3]) << "all failed.";
//    // vector16
//    ASSERT_EQ(f, mem[4]) << "all failed.";
//    ASSERT_EQ(f, mem[5]) << "all failed.";
//    ASSERT_EQ(f, mem[6]) << "all failed.";
//    ASSERT_EQ(t, mem[7]) << "all failed.";
//  }
//}
