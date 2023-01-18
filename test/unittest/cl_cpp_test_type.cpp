/*!
  \file cl_cpp_test_vector.cpp
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
#include "zivc/kernel_set/kernel_set-cl_cpp_test_type.hpp"

TEST(ClCppTest, VectorComponentAccessTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  const zivc::SharedBuffer buffer_invec2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.0f, 2.0f},
                                                cl_float2{3.0f, 4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec2.get());
  }
  const zivc::SharedBuffer buffer_invec3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.0f, 2.0f, 3.0f},
                                                cl_float3{4.0f, 5.0f, 6.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec3.get());
  }
  const zivc::SharedBuffer buffer_invec4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.0f, 2.0f, 3.0f, 4.0f},
                                                cl_float4{5.0f, 6.0f, 7.0f, 8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec4.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(18);
  {
    const cl_float v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(2);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(2);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(2);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComponentAccessKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(7, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorComponentAccessTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(7, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(
      *buffer_invec2, *buffer_invec3, *buffer_invec4,
      *buffer_out1, *buffer_out2, *buffer_out3, *buffer_out4,
      launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    ASSERT_EQ(1.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(2.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(3.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(4.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(1.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(2.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(3.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(4.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(5.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(6.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(1.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(2.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(3.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(4.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(5.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(6.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(7.0f, mem[index++]) << "Vector access failed.";
    ASSERT_EQ(8.0f, mem[index++]) << "Vector access failed.";
  }
  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float2 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float2 access failed.";
    ASSERT_EQ(3.0f, mem[1].x) << "Vector float2 access failed.";
    ASSERT_EQ(4.0f, mem[1].y) << "Vector float2 access failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float3 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float3 access failed.";
    ASSERT_EQ(3.0f, mem[0].z) << "Vector float3 access failed.";
    ASSERT_EQ(4.0f, mem[1].x) << "Vector float3 access failed.";
    ASSERT_EQ(5.0f, mem[1].y) << "Vector float3 access failed.";
    ASSERT_EQ(6.0f, mem[1].z) << "Vector float3 access failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float4 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float4 access failed.";
    ASSERT_EQ(3.0f, mem[0].z) << "Vector float4 access failed.";
    ASSERT_EQ(4.0f, mem[0].w) << "Vector float4 access failed.";
    ASSERT_EQ(5.0f, mem[1].x) << "Vector float4 access failed.";
    ASSERT_EQ(6.0f, mem[1].y) << "Vector float4 access failed.";
    ASSERT_EQ(7.0f, mem[1].z) << "Vector float4 access failed.";
    ASSERT_EQ(8.0f, mem[1].w) << "Vector float4 access failed.";
  }
}

TEST(ClCppTest, VectorConstructionTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(5);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(9);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConstructionKernel, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorComponentAccessTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out2, *buffer_out3, *buffer_out4, launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(0, mem[0].x) << "Vector int2 construction failed.";
    ASSERT_EQ(1, mem[0].y) << "Vector int2 construction failed.";
    ASSERT_EQ(2, mem[1].x) << "Vector int2 construction failed.";
    ASSERT_EQ(2, mem[1].y) << "Vector int2 construction failed.";
    ASSERT_EQ(3, mem[2].x) << "Vector int2 construction failed.";
    ASSERT_EQ(4, mem[2].y) << "Vector int2 construction failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(0, mem[0].x) << "Vector int3 construction failed.";
    ASSERT_EQ(1, mem[0].y) << "Vector int3 construction failed.";
    ASSERT_EQ(2, mem[0].z) << "Vector int3 construction failed.";
    ASSERT_EQ(3, mem[1].x) << "Vector int3 construction failed.";
    ASSERT_EQ(4, mem[1].y) << "Vector int3 construction failed.";
    ASSERT_EQ(5, mem[1].z) << "Vector int3 construction failed.";
    ASSERT_EQ(5, mem[2].x) << "Vector int3 construction failed.";
    ASSERT_EQ(3, mem[2].y) << "Vector int3 construction failed.";
    ASSERT_EQ(4, mem[2].z) << "Vector int3 construction failed.";
    ASSERT_EQ(6, mem[3].x) << "Vector int3 construction failed.";
    ASSERT_EQ(6, mem[3].y) << "Vector int3 construction failed.";
    ASSERT_EQ(6, mem[3].z) << "Vector int3 construction failed.";
    ASSERT_EQ(7, mem[4].x) << "Vector int3 construction failed.";
    ASSERT_EQ(8, mem[4].y) << "Vector int3 construction failed.";
    ASSERT_EQ(9, mem[4].z) << "Vector int3 construction failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    ASSERT_EQ(0, mem[0].x) << "Vector int4 construction failed.";
    ASSERT_EQ(1, mem[0].y) << "Vector int4 construction failed.";
    ASSERT_EQ(2, mem[0].z) << "Vector int4 construction failed.";
    ASSERT_EQ(3, mem[0].w) << "Vector int4 construction failed.";
    ASSERT_EQ(4, mem[1].x) << "Vector int4 construction failed.";
    ASSERT_EQ(5, mem[1].y) << "Vector int4 construction failed.";
    ASSERT_EQ(6, mem[1].z) << "Vector int4 construction failed.";
    ASSERT_EQ(7, mem[1].w) << "Vector int4 construction failed.";
    ASSERT_EQ(6, mem[2].x) << "Vector int4 construction failed.";
    ASSERT_EQ(4, mem[2].y) << "Vector int4 construction failed.";
    ASSERT_EQ(5, mem[2].z) << "Vector int4 construction failed.";
    ASSERT_EQ(7, mem[2].w) << "Vector int4 construction failed.";
    ASSERT_EQ(6, mem[3].x) << "Vector int4 construction failed.";
    ASSERT_EQ(7, mem[3].y) << "Vector int4 construction failed.";
    ASSERT_EQ(4, mem[3].z) << "Vector int4 construction failed.";
    ASSERT_EQ(5, mem[3].w) << "Vector int4 construction failed.";
    ASSERT_EQ(4, mem[4].x) << "Vector int4 construction failed.";
    ASSERT_EQ(5, mem[4].y) << "Vector int4 construction failed.";
    ASSERT_EQ(4, mem[4].z) << "Vector int4 construction failed.";
    ASSERT_EQ(5, mem[4].w) << "Vector int4 construction failed.";
    ASSERT_EQ(8, mem[5].x) << "Vector int4 construction failed.";
    ASSERT_EQ(9, mem[5].y) << "Vector int4 construction failed.";
    ASSERT_EQ(10, mem[5].z) << "Vector int4 construction failed.";
    ASSERT_EQ(11, mem[5].w) << "Vector int4 construction failed.";
    ASSERT_EQ(11, mem[6].x) << "Vector int4 construction failed.";
    ASSERT_EQ(8, mem[6].y) << "Vector int4 construction failed.";
    ASSERT_EQ(9, mem[6].z) << "Vector int4 construction failed.";
    ASSERT_EQ(10, mem[6].w) << "Vector int4 construction failed.";
    ASSERT_EQ(12, mem[7].x) << "Vector int4 construction failed.";
    ASSERT_EQ(12, mem[7].y) << "Vector int4 construction failed.";
    ASSERT_EQ(12, mem[7].z) << "Vector int4 construction failed.";
    ASSERT_EQ(12, mem[7].w) << "Vector int4 construction failed.";
    ASSERT_EQ(13, mem[8].x) << "Vector int4 construction failed.";
    ASSERT_EQ(14, mem[8].y) << "Vector int4 construction failed.";
    ASSERT_EQ(15, mem[8].z) << "Vector int4 construction failed.";
    ASSERT_EQ(16, mem[8].w) << "Vector int4 construction failed.";
  }
}

TEST(ClCppTest, VectorArithmeticOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                              cl_int2{10, -10},
                                              cl_int2{1, 5},
                                              cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                              cl_int3{10, -10, -4},
                                              cl_int3{1, 5, 4},
                                              cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                              cl_int4{10, -10, -4, -8},
                                              cl_int4{1, 5, 4, 12},
                                              cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const zivc::SharedBuffer buffer_inf2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.0f, -1.0f},
                                                cl_float2{10.0f, -10.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf2.get());
  }
  const zivc::SharedBuffer buffer_inf3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.0f, -1.0f, 4.0f},
                                                cl_float3{10.0f, -10.0f, -4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf3.get());
  }
  const zivc::SharedBuffer buffer_inf4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.0f, -1.0f, 4.0f, 12.0f},
                                                cl_float4{10.0f, -10.0f, -4.0f, -8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf4.get());
  }
  const std::size_t n = 14;
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n + 3);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_outf2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_outf2->setSize(n);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf2.get());
  }
  const zivc::SharedBuffer buffer_outf3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_outf3->setSize(n);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf3.get());
  }
  const zivc::SharedBuffer buffer_outf4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_outf4->setSize(n + 6);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorArithmeticOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(12, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                                          *buffer_inf2, *buffer_inf3, *buffer_inf4,
                                          *buffer_out2, *buffer_out3, *buffer_out4,
                                          *buffer_outf2, *buffer_outf3, *buffer_outf4,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "Vector int2 unary plus failed.";
    EXPECT_EQ(-1, mem[0].y) << "Vector int2 unary plus failed.";
    EXPECT_EQ(-1, mem[1].x) << "Vector int2 unary minus failed.";
    EXPECT_EQ(1, mem[1].y) << "Vector int2 unary minus failed.";
    EXPECT_EQ(11, mem[2].x) << "Vector int2 addition failed.";
    EXPECT_EQ(-11, mem[2].y) << "Vector int2 addition failed.";
    EXPECT_EQ(11, mem[3].x) << "Vector int2 addition failed.";
    EXPECT_EQ(-9, mem[3].y) << "Vector int2 addition failed.";
    EXPECT_EQ(11, mem[4].x) << "Vector int2 addition failed.";
    EXPECT_EQ(9, mem[4].y) << "Vector int2 addition failed.";
    EXPECT_EQ(-9, mem[5].x) << "Vector int2 subtraction failed.";
    EXPECT_EQ(9, mem[5].y) << "Vector int2 subtraction failed.";
    EXPECT_EQ(-9, mem[6].x) << "Vector int2 subtraction failed.";
    EXPECT_EQ(11, mem[6].y) << "Vector int2 subtraction failed.";
    EXPECT_EQ(-9, mem[7].x) << "Vector int2 subtraction failed.";
    EXPECT_EQ(-11, mem[7].y) << "Vector int2 subtraction failed.";
    EXPECT_EQ(10, mem[8].x) << "Vector int2 multiplication failed.";
    EXPECT_EQ(10, mem[8].y) << "Vector int2 multiplication failed.";
    EXPECT_EQ(10, mem[9].x) << "Vector int2 multiplication failed.";
    EXPECT_EQ(-10, mem[9].y) << "Vector int2 multiplication failed.";
    EXPECT_EQ(10, mem[10].x) << "Vector int2 multiplication failed.";
    EXPECT_EQ(-10, mem[10].y) << "Vector int2 multiplication failed.";
    EXPECT_EQ(0, mem[11].x) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[11].y) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[12].x) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[12].y) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[13].x) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[13].y) << "Vector int2 division failed.";
    EXPECT_EQ(1, mem[14].x) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[14].y) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[15].x) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[15].y) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[16].x) << "Vector int2 modulo failed.";
    EXPECT_EQ(5, mem[16].y) << "Vector int2 modulo failed.";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_outf2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "Vector float2 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "Vector float2 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[1].x) << "Vector float2 unary minus failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[1].y) << "Vector float2 unary minus failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[2].x) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[2].y) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[3].x) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[3].y) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[4].x) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[4].y) << "Vector float2 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[5].x) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[5].y) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[6].x) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[6].y) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[7].x) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[7].y) << "Vector float2 subtraction failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].x) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].y) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[9].x) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[9].y) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[10].x) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[10].y) << "Vector float2 multiplication failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].x) << "Vector float2 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].y) << "Vector float2 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[12].x) << "Vector float2 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[12].y) << "Vector float2 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[13].x) << "Vector float2 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[13].y) << "Vector float2 division failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "Vector int3 unary plus failed.";
    EXPECT_EQ(-1, mem[0].y) << "Vector int3 unary plus failed.";
    EXPECT_EQ(4, mem[0].z) << "Vector int3 unary plus failed.";
    EXPECT_EQ(-1, mem[1].x) << "Vector int3 unary minus failed.";
    EXPECT_EQ(1, mem[1].y) << "Vector int3 unary minus failed.";
    EXPECT_EQ(-4, mem[1].z) << "Vector int3 unary minus failed.";
    EXPECT_EQ(11, mem[2].x) << "Vector int3 addition failed.";
    EXPECT_EQ(-11, mem[2].y) << "Vector int3 addition failed.";
    EXPECT_EQ(0, mem[2].z) << "Vector int3 addition failed.";
    EXPECT_EQ(11, mem[3].x) << "Vector int3 addition failed.";
    EXPECT_EQ(-9, mem[3].y) << "Vector int3 addition failed.";
    EXPECT_EQ(-3, mem[3].z) << "Vector int3 addition failed.";
    EXPECT_EQ(11, mem[4].x) << "Vector int3 addition failed.";
    EXPECT_EQ(9, mem[4].y) << "Vector int3 addition failed.";
    EXPECT_EQ(14, mem[4].z) << "Vector int3 addition failed.";
    EXPECT_EQ(-9, mem[5].x) << "Vector int3 subtraction failed.";
    EXPECT_EQ(9, mem[5].y) << "Vector int3 subtraction failed.";
    EXPECT_EQ(8, mem[5].z) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-9, mem[6].x) << "Vector int3 subtraction failed.";
    EXPECT_EQ(11, mem[6].y) << "Vector int3 subtraction failed.";
    EXPECT_EQ(5, mem[6].z) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-9, mem[7].x) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-11, mem[7].y) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-6, mem[7].z) << "Vector int3 subtraction failed.";
    EXPECT_EQ(10, mem[8].x) << "Vector int3 multiplication failed.";
    EXPECT_EQ(10, mem[8].y) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-16, mem[8].z) << "Vector int3 multiplication failed.";
    EXPECT_EQ(10, mem[9].x) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-10, mem[9].y) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-4, mem[9].z) << "Vector int3 multiplication failed.";
    EXPECT_EQ(10, mem[10].x) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-10, mem[10].y) << "Vector int3 multiplication failed.";
    EXPECT_EQ(40, mem[10].z) << "Vector int3 multiplication failed.";
    EXPECT_EQ(0, mem[11].x) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[11].y) << "Vector int3 division failed.";
    EXPECT_EQ(-1, mem[11].z) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[12].x) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[12].y) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[12].z) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[13].x) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[13].y) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[13].z) << "Vector int3 division failed.";
    EXPECT_EQ(1, mem[14].x) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[14].y) << "Vector int3 modulo failed.";
    EXPECT_EQ(4, mem[14].z) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[15].x) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[15].y) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[15].z) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[16].x) << "Vector int3 modulo failed.";
    EXPECT_EQ(5, mem[16].y) << "Vector int3 modulo failed.";
    EXPECT_EQ(4, mem[16].z) << "Vector int3 modulo failed.";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_outf3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "Vector float3 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "Vector float3 unary plus failed.";
    EXPECT_FLOAT_EQ(4.0f, mem[0].z) << "Vector float3 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[1].x) << "Vector float3 unary minus failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[1].y) << "Vector float3 unary minus failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[1].z) << "Vector float3 unary minus failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[2].x) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[2].y) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[2].z) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[3].x) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[3].y) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(-3.0f, mem[3].z) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[4].x) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[4].y) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(14.0f, mem[4].z) << "Vector float3 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[5].x) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[5].y) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(8.0f, mem[5].z) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[6].x) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[6].y) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(5.0f, mem[6].z) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[7].x) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[7].y) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(-6.0f, mem[7].z) << "Vector float3 subtraction failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].x) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].y) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(-16.0f, mem[8].z) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[9].x) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[9].y) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[9].z) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[10].x) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[10].y) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(40.0f, mem[10].z) << "Vector float3 multiplication failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].x) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].y) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[11].z) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[12].x) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[12].y) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(-0.25f, mem[12].z) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[13].x) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[13].y) << "Vector float3 division failed.";
    EXPECT_FLOAT_EQ(0.4f, mem[13].z) << "Vector float3 division failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "Vector int4 unary plus failed.";
    EXPECT_EQ(-1, mem[0].y) << "Vector int4 unary plus failed.";
    EXPECT_EQ(4, mem[0].z) << "Vector int4 unary plus failed.";
    EXPECT_EQ(12, mem[0].w) << "Vector int4 unary plus failed.";
    EXPECT_EQ(-1, mem[1].x) << "Vector int4 unary minus failed.";
    EXPECT_EQ(1, mem[1].y) << "Vector int4 unary minus failed.";
    EXPECT_EQ(-4, mem[1].z) << "Vector int4 unary minus failed.";
    EXPECT_EQ(-12, mem[1].w) << "Vector int4 unary minus failed.";
    EXPECT_EQ(11, mem[2].x) << "Vector int4 addition failed.";
    EXPECT_EQ(-11, mem[2].y) << "Vector int4 addition failed.";
    EXPECT_EQ(0, mem[2].z) << "Vector int4 addition failed.";
    EXPECT_EQ(4, mem[2].w) << "Vector int4 addition failed.";
    EXPECT_EQ(11, mem[3].x) << "Vector int4 addition failed.";
    EXPECT_EQ(-9, mem[3].y) << "Vector int4 addition failed.";
    EXPECT_EQ(-3, mem[3].z) << "Vector int4 addition failed.";
    EXPECT_EQ(-7, mem[3].w) << "Vector int4 addition failed.";
    EXPECT_EQ(11, mem[4].x) << "Vector int4 addition failed.";
    EXPECT_EQ(9, mem[4].y) << "Vector int4 addition failed.";
    EXPECT_EQ(14, mem[4].z) << "Vector int4 addition failed.";
    EXPECT_EQ(22, mem[4].w) << "Vector int4 addition failed.";
    EXPECT_EQ(-9, mem[5].x) << "Vector int4 subtraction failed.";
    EXPECT_EQ(9, mem[5].y) << "Vector int4 subtraction failed.";
    EXPECT_EQ(8, mem[5].z) << "Vector int4 subtraction failed.";
    EXPECT_EQ(20, mem[5].w) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-9, mem[6].x) << "Vector int4 subtraction failed.";
    EXPECT_EQ(11, mem[6].y) << "Vector int4 subtraction failed.";
    EXPECT_EQ(5, mem[6].z) << "Vector int4 subtraction failed.";
    EXPECT_EQ(9, mem[6].w) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-9, mem[7].x) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-11, mem[7].y) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-6, mem[7].z) << "Vector int4 subtraction failed.";
    EXPECT_EQ(2, mem[7].w) << "Vector int4 subtraction failed.";
    EXPECT_EQ(10, mem[8].x) << "Vector int4 multiplication failed.";
    EXPECT_EQ(10, mem[8].y) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-16, mem[8].z) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-96, mem[8].w) << "Vector int4 multiplication failed.";
    EXPECT_EQ(10, mem[9].x) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-10, mem[9].y) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-4, mem[9].z) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-8, mem[9].w) << "Vector int4 multiplication failed.";
    EXPECT_EQ(10, mem[10].x) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-10, mem[10].y) << "Vector int4 multiplication failed.";
    EXPECT_EQ(40, mem[10].z) << "Vector int4 multiplication failed.";
    EXPECT_EQ(120, mem[10].w) << "Vector int4 multiplication failed.";
    EXPECT_EQ(0, mem[11].x) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[11].y) << "Vector int4 division failed.";
    EXPECT_EQ(-1, mem[11].z) << "Vector int4 division failed.";
    EXPECT_EQ(-1, mem[11].w) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[12].x) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[12].y) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[12].z) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[12].w) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[13].x) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[13].y) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[13].z) << "Vector int4 division failed.";
    EXPECT_EQ(1, mem[13].w) << "Vector int4 division failed.";
    EXPECT_EQ(1, mem[14].x) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[14].y) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[14].z) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[14].w) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[15].x) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[15].y) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[15].z) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[15].w) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[16].x) << "Vector int4 modulo failed.";
    EXPECT_EQ(5, mem[16].y) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[16].z) << "Vector int4 modulo failed.";
    EXPECT_EQ(2, mem[16].w) << "Vector int4 modulo failed.";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_outf4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "Vector float4 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "Vector float4 unary plus failed.";
    EXPECT_FLOAT_EQ(4.0f, mem[0].z) << "Vector float4 unary plus failed.";
    EXPECT_FLOAT_EQ(12.0f, mem[0].w) << "Vector float4 unary plus failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[1].x) << "Vector float4 unary minus failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[1].y) << "Vector float4 unary minus failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[1].z) << "Vector float4 unary minus failed.";
    EXPECT_FLOAT_EQ(-12.0f, mem[1].w) << "Vector float4 unary minus failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[2].x) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[2].y) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[2].z) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(4.0f, mem[2].w) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[3].x) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[3].y) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(-3.0f, mem[3].z) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(-7.0f, mem[3].w) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[4].x) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[4].y) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(14.0f, mem[4].z) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(22.0f, mem[4].w) << "Vector float4 addition failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[5].x) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[5].y) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(8.0f, mem[5].z) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(20.0f, mem[5].w) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[6].x) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(11.0f, mem[6].y) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(5.0f, mem[6].z) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(9.0f, mem[6].w) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(-9.0f, mem[7].x) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(-11.0f, mem[7].y) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(-6.0f, mem[7].z) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(2.0f, mem[7].w) << "Vector float4 subtraction failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].x) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[8].y) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-16.0f, mem[8].z) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-96.0f, mem[8].w) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[9].x) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[9].y) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[9].z) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-8.0f, mem[9].w) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[10].x) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(-10.0f, mem[10].y) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(40.0f, mem[10].z) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(120.0f, mem[10].w) << "Vector float4 multiplication failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].x) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[11].y) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[11].z) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-1.5f, mem[11].w) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[12].x) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[12].y) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-0.25f, mem[12].z) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-0.125f, mem[12].w) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(0.1f, mem[13].x) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(-0.1f, mem[13].y) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(0.4f, mem[13].z) << "Vector float4 division failed.";
    EXPECT_FLOAT_EQ(1.2f, mem[13].w) << "Vector float4 division failed.";

    // Constexpr test
    EXPECT_FLOAT_EQ(0.0f, mem[14].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[14].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(2.0f, mem[14].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(3.0f, mem[14].w) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-0.0f, mem[15].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[15].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-2.0f, mem[15].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-3.0f, mem[15].w) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(4.0f, mem[16].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(6.0f, mem[16].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(8.0f, mem[16].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[16].w) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[17].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[17].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[17].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(-4.0f, mem[17].w) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[18].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(5.0f, mem[18].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(12.0f, mem[18].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(21.0f, mem[18].w) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[19].x) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(0.2f, mem[19].y) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(1.0f / 3.0f, mem[19].z) << "Vector constexpr test failed.";
    EXPECT_FLOAT_EQ(3.0f / 7.0f, mem[19].w) << "Vector constexpr test failed.";
  }
}

TEST(ClCppTest, VectorArithmeticAssignmentOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;

  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                              cl_int2{10, -10},
                                              cl_int2{1, 5},
                                              cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                              cl_int3{10, -10, -4},
                                              cl_int3{1, 5, 4},
                                              cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                              cl_int4{10, -10, -4, -8},
                                              cl_int4{1, 5, 4, 12},
                                              cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 10;
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n + 3 + 4);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticAssignmentOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorArithmeticAssignmentOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                                          *buffer_out2, *buffer_out3, *buffer_out4,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(11, mem[0].x) << "Vector int2 addition failed.";
    EXPECT_EQ(-11, mem[0].y) << "Vector int2 addition failed.";
    EXPECT_EQ(11, mem[1].x) << "Vector int2 addition failed.";
    EXPECT_EQ(9, mem[1].y) << "Vector int2 addition failed.";
    EXPECT_EQ(-9, mem[2].x) << "Vector int2 subtraction failed.";
    EXPECT_EQ(9, mem[2].y) << "Vector int2 subtraction failed.";
    EXPECT_EQ(-9, mem[3].x) << "Vector int2 subtraction failed.";
    EXPECT_EQ(-11, mem[3].y) << "Vector int2 subtraction failed.";
    EXPECT_EQ(10, mem[4].x) << "Vector int2 multiplication failed.";
    EXPECT_EQ(10, mem[4].y) << "Vector int2 multiplication failed.";
    EXPECT_EQ(10, mem[5].x) << "Vector int2 multiplication failed.";
    EXPECT_EQ(-10, mem[5].y) << "Vector int2 multiplication failed.";
    EXPECT_EQ(0, mem[6].x) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[6].y) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[7].x) << "Vector int2 division failed.";
    EXPECT_EQ(0, mem[7].y) << "Vector int2 division failed.";
    EXPECT_EQ(1, mem[8].x) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[8].y) << "Vector int2 modulo failed.";
    EXPECT_EQ(1, mem[9].x) << "Vector int2 modulo failed.";
    EXPECT_EQ(5, mem[9].y) << "Vector int2 modulo failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(11, mem[0].x) << "Vector int3 addition failed.";
    EXPECT_EQ(-11, mem[0].y) << "Vector int3 addition failed.";
    EXPECT_EQ(0, mem[0].z) << "Vector int3 addition failed.";
    EXPECT_EQ(11, mem[1].x) << "Vector int3 addition failed.";
    EXPECT_EQ(9, mem[1].y) << "Vector int3 addition failed.";
    EXPECT_EQ(14, mem[1].z) << "Vector int3 addition failed.";
    EXPECT_EQ(-9, mem[2].x) << "Vector int3 subtraction failed.";
    EXPECT_EQ(9, mem[2].y) << "Vector int3 subtraction failed.";
    EXPECT_EQ(8, mem[2].z) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-9, mem[3].x) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-11, mem[3].y) << "Vector int3 subtraction failed.";
    EXPECT_EQ(-6, mem[3].z) << "Vector int3 subtraction failed.";
    EXPECT_EQ(10, mem[4].x) << "Vector int3 multiplication failed.";
    EXPECT_EQ(10, mem[4].y) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-16, mem[4].z) << "Vector int3 multiplication failed.";
    EXPECT_EQ(10, mem[5].x) << "Vector int3 multiplication failed.";
    EXPECT_EQ(-10, mem[5].y) << "Vector int3 multiplication failed.";
    EXPECT_EQ(40, mem[5].z) << "Vector int3 multiplication failed.";
    EXPECT_EQ(0, mem[6].x) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[6].y) << "Vector int3 division failed.";
    EXPECT_EQ(-1, mem[6].z) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[7].x) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[7].y) << "Vector int3 division failed.";
    EXPECT_EQ(0, mem[7].z) << "Vector int3 division failed.";
    EXPECT_EQ(1, mem[8].x) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[8].y) << "Vector int3 modulo failed.";
    EXPECT_EQ(4, mem[8].z) << "Vector int3 modulo failed.";
    EXPECT_EQ(1, mem[9].x) << "Vector int3 modulo failed.";
    EXPECT_EQ(5, mem[9].y) << "Vector int3 modulo failed.";
    EXPECT_EQ(4, mem[9].z) << "Vector int3 modulo failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(11, mem[0].x) << "Vector int4 addition failed.";
    EXPECT_EQ(-11, mem[0].y) << "Vector int4 addition failed.";
    EXPECT_EQ(0, mem[0].z) << "Vector int4 addition failed.";
    EXPECT_EQ(4, mem[0].w) << "Vector int4 addition failed.";
    EXPECT_EQ(11, mem[1].x) << "Vector int4 addition failed.";
    EXPECT_EQ(9, mem[1].y) << "Vector int4 addition failed.";
    EXPECT_EQ(14, mem[1].z) << "Vector int4 addition failed.";
    EXPECT_EQ(22, mem[1].w) << "Vector int4 addition failed.";
    EXPECT_EQ(-9, mem[2].x) << "Vector int4 subtraction failed.";
    EXPECT_EQ(9, mem[2].y) << "Vector int4 subtraction failed.";
    EXPECT_EQ(8, mem[2].z) << "Vector int4 subtraction failed.";
    EXPECT_EQ(20, mem[2].w) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-9, mem[3].x) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-11, mem[3].y) << "Vector int4 subtraction failed.";
    EXPECT_EQ(-6, mem[3].z) << "Vector int4 subtraction failed.";
    EXPECT_EQ(2, mem[3].w) << "Vector int4 subtraction failed.";
    EXPECT_EQ(10, mem[4].x) << "Vector int4 multiplication failed.";
    EXPECT_EQ(10, mem[4].y) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-16, mem[4].z) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-96, mem[4].w) << "Vector int4 multiplication failed.";
    EXPECT_EQ(10, mem[5].x) << "Vector int4 multiplication failed.";
    EXPECT_EQ(-10, mem[5].y) << "Vector int4 multiplication failed.";
    EXPECT_EQ(40, mem[5].z) << "Vector int4 multiplication failed.";
    EXPECT_EQ(120, mem[5].w) << "Vector int4 multiplication failed.";
    EXPECT_EQ(0, mem[6].x) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[6].y) << "Vector int4 division failed.";
    EXPECT_EQ(-1, mem[6].z) << "Vector int4 division failed.";
    EXPECT_EQ(-1, mem[6].w) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[7].x) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[7].y) << "Vector int4 division failed.";
    EXPECT_EQ(0, mem[7].z) << "Vector int4 division failed.";
    EXPECT_EQ(1, mem[7].w) << "Vector int4 division failed.";
    EXPECT_EQ(1, mem[8].x) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[8].y) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[8].z) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[8].w) << "Vector int4 modulo failed.";
    EXPECT_EQ(1, mem[9].x) << "Vector int4 modulo failed.";
    EXPECT_EQ(5, mem[9].y) << "Vector int4 modulo failed.";
    EXPECT_EQ(4, mem[9].z) << "Vector int4 modulo failed.";
    EXPECT_EQ(2, mem[9].w) << "Vector int4 modulo failed.";

    // Constexpr test
    EXPECT_EQ(2, mem[10].x) << "Vector constexpr test failed.";
    EXPECT_EQ(7, mem[10].y) << "Vector constexpr test failed.";
    EXPECT_EQ(12, mem[10].z) << "Vector constexpr test failed.";
    EXPECT_EQ(17, mem[10].w) << "Vector constexpr test failed.";
    EXPECT_EQ(-2, mem[11].x) << "Vector constexpr test failed.";
    EXPECT_EQ(1, mem[11].y) << "Vector constexpr test failed.";
    EXPECT_EQ(4, mem[11].z) << "Vector constexpr test failed.";
    EXPECT_EQ(7, mem[11].w) << "Vector constexpr test failed.";
    EXPECT_EQ(0, mem[12].x) << "Vector constexpr test failed.";
    EXPECT_EQ(12, mem[12].y) << "Vector constexpr test failed.";
    EXPECT_EQ(32, mem[12].z) << "Vector constexpr test failed.";
    EXPECT_EQ(60, mem[12].w) << "Vector constexpr test failed.";
    EXPECT_EQ(0, mem[13].x) << "Vector constexpr test failed.";
    EXPECT_EQ(1, mem[13].y) << "Vector constexpr test failed.";
    EXPECT_EQ(2, mem[13].z) << "Vector constexpr test failed.";
    EXPECT_EQ(2, mem[13].w) << "Vector constexpr test failed.";
  }
}

TEST(ClCppTest, VectorBitwiseOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::uint32b;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;

  constexpr uint32b mask01 = std::numeric_limits<uint32b>::max();
  constexpr uint32b mask02 = 0b0101'0101'1010'1010'1100'0011'0000'1111u;
  constexpr uint32b mask03 = 0b0101'0101'0101'0101'0101'0101'0101'0101u;
  constexpr uint32b mask04 = 0b0000'0000'0000'0000'1111'1111'1111'1111u;
  constexpr uint32b mask05 = 0b1111'1111'1111'1111'0000'0000'0000'0000u;

  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                               cl_uint2{mask02, mask03},
                                               cl_uint2{mask04, mask05},
                                               cl_uint2{32u, 64u},
                                               cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                               cl_uint3{mask02, mask03, mask02},
                                               cl_uint3{mask04, mask05, mask04},
                                               cl_uint3{32u, 64u, 128u},
                                               cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                               cl_uint4{mask02, mask03, mask02, mask03},
                                               cl_uint4{mask04, mask05, mask04, mask05},
                                               cl_uint4{32u, 64u, 128u, 256u},
                                               cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 6);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorBitwiseOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                                          *buffer_out2, *buffer_out3, *buffer_out4,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(compl mask02, mem[0].x) << "Vector uint2 bitwise NOT failed.";
    EXPECT_EQ(compl mask03, mem[0].y) << "Vector uint2 bitwise NOT failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[1].y) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[2].x) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[2].y) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].x) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].y) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[4].x) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[4].y) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[5].x) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask05, mem[5].y) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[6].x) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[6].y) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[7].x) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[7].y) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[8].x) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask05, mem[8].y) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[9].x) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[9].y) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[10].x) << "Vector uint2 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[10].y) << "Vector uint2 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[11].x) << "Vector uint2 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[11].y) << "Vector uint2 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[12].x) << "Vector uint2 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[12].y) << "Vector uint2 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[13].x) << "Vector uint2 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[13].y) << "Vector uint2 right shift failed.";

    // Constexpr test
    EXPECT_EQ(~0b01u, mem[14].x) << "Vector constexpr test failed.";
    EXPECT_EQ(~0b10u, mem[14].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b01u, mem[15].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u, mem[15].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b11u, mem[16].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b11u, mem[16].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u, mem[17].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b01u, mem[17].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b01u << 3u, mem[18].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u << 3u, mem[18].y) << "Vector constexpr test failed.";
    EXPECT_EQ(3u >> 0b01u, mem[19].x) << "Vector constexpr test failed.";
    EXPECT_EQ(3u >> 0b10u, mem[19].y) << "Vector constexpr test failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(compl mask02, mem[0].x) << "Vector uint3 bitwise NOT failed.";
    EXPECT_EQ(compl mask03, mem[0].y) << "Vector uint3 bitwise NOT failed.";
    EXPECT_EQ(compl mask02, mem[0].z) << "Vector uint3 bitwise NOT failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[1].y) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].z) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[2].x) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[2].y) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[2].z) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].x) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].y) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].z) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[4].x) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[4].y) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[4].z) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[5].x) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask05, mem[5].y) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[5].z) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[6].x) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[6].y) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[6].z) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[7].x) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[7].y) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[7].z) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[8].x) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask05, mem[8].y) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[8].z) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[9].x) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[9].y) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[9].z) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[10].x) << "Vector uint3 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[10].y) << "Vector uint3 left shift failed.";
    EXPECT_EQ(128u << 6u, mem[10].z) << "Vector uint3 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[11].x) << "Vector uint3 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[11].y) << "Vector uint3 left shift failed.";
    EXPECT_EQ(128u << 2u, mem[11].z) << "Vector uint3 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[12].x) << "Vector uint3 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[12].y) << "Vector uint3 right shift failed.";
    EXPECT_EQ(128u >> 6u, mem[12].z) << "Vector uint3 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[13].x) << "Vector uint3 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[13].y) << "Vector uint3 right shift failed.";
    EXPECT_EQ(128u >> 2u, mem[13].z) << "Vector uint3 right shift failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(compl mask02, mem[0].x) << "Vector uint4 bitwise NOT failed.";
    EXPECT_EQ(compl mask03, mem[0].y) << "Vector uint4 bitwise NOT failed.";
    EXPECT_EQ(compl mask02, mem[0].z) << "Vector uint4 bitwise NOT failed.";
    EXPECT_EQ(compl mask03, mem[0].w) << "Vector uint4 bitwise NOT failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[1].y) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].z) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[1].w) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[2].x) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[2].y) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[2].z) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[2].w) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].x) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].y) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].z) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[3].w) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[4].x) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[4].y) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[4].z) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[4].w) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[5].x) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask05, mem[5].y) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[5].z) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask05, mem[5].w) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[6].x) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[6].y) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[6].z) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[6].w) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[7].x) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[7].y) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[7].z) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[7].w) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[8].x) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask05, mem[8].y) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[8].z) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask05, mem[8].w) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[9].x) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[9].y) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[9].z) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[9].w) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[10].x) << "Vector uint4 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[10].y) << "Vector uint4 left shift failed.";
    EXPECT_EQ(128u << 6u, mem[10].z) << "Vector uint4 left shift failed.";
    EXPECT_EQ(256u << 8u, mem[10].w) << "Vector uint4 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[11].x) << "Vector uint4 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[11].y) << "Vector uint4 left shift failed.";
    EXPECT_EQ(128u << 2u, mem[11].z) << "Vector uint4 left shift failed.";
    EXPECT_EQ(256u << 2u, mem[11].w) << "Vector uint4 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[12].x) << "Vector uint4 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[12].y) << "Vector uint4 right shift failed.";
    EXPECT_EQ(128u >> 6u, mem[12].z) << "Vector uint4 right shift failed.";
    EXPECT_EQ(256u >> 8u, mem[12].w) << "Vector uint4 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[13].x) << "Vector uint4 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[13].y) << "Vector uint4 right shift failed.";
    EXPECT_EQ(128u >> 2u, mem[13].z) << "Vector uint4 right shift failed.";
    EXPECT_EQ(256u >> 2u, mem[13].w) << "Vector uint4 right shift failed.";
  }
}

TEST(ClCppTest, VectorBitwiseAssignmentOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::uint32b;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;

  constexpr uint32b mask01 = std::numeric_limits<uint32b>::max();
  constexpr uint32b mask02 = 0b0101'0101'1010'1010'1100'0011'0000'1111u;
  constexpr uint32b mask03 = 0b0101'0101'0101'0101'0101'0101'0101'0101u;
  constexpr uint32b mask04 = 0b0000'0000'0000'0000'1111'1111'1111'1111u;
  constexpr uint32b mask05 = 0b1111'1111'1111'1111'0000'0000'0000'0000u;

  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                               cl_uint2{mask02, mask03},
                                               cl_uint2{mask04, mask05},
                                               cl_uint2{32u, 64u},
                                               cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                               cl_uint3{mask02, mask03, mask02},
                                               cl_uint3{mask04, mask05, mask04},
                                               cl_uint3{32u, 64u, 128u},
                                               cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                               cl_uint4{mask02, mask03, mask02, mask03},
                                               cl_uint4{mask04, mask05, mask04, mask05},
                                               cl_uint4{32u, 64u, 128u, 256u},
                                               cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 5);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseAssignmentOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorBitwiseAssignmentOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                                          *buffer_out2, *buffer_out3, *buffer_out4,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(mask01 bitand mask02, mem[0].x) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[0].y) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].y) << "Vector uint2 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[2].x) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[2].y) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[3].x) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[3].y) << "Vector uint2 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[4].x) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[4].y) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[5].x) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[5].y) << "Vector uint2 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[6].x) << "Vector uint2 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[6].y) << "Vector uint2 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[7].x) << "Vector uint2 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[7].y) << "Vector uint2 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[8].x) << "Vector uint2 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[8].y) << "Vector uint2 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[9].x) << "Vector uint2 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[9].y) << "Vector uint2 right shift failed.";

    // Constexpr test
    EXPECT_EQ(0b01u, mem[10].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u, mem[10].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b11u, mem[11].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b11u, mem[11].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u, mem[12].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b01u, mem[12].y) << "Vector constexpr test failed.";
    EXPECT_EQ(0b01u << 3u, mem[13].x) << "Vector constexpr test failed.";
    EXPECT_EQ(0b10u << 3u, mem[13].y) << "Vector constexpr test failed.";
    EXPECT_EQ(3u >> 0b01u, mem[14].x) << "Vector constexpr test failed.";
    EXPECT_EQ(3u >> 0b10u, mem[14].y) << "Vector constexpr test failed.";
  }
  // output3
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(mask01 bitand mask02, mem[0].x) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[0].y) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[0].z) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].y) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].z) << "Vector uint3 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[2].x) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[2].y) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[2].z) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[3].x) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[3].y) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[3].z) << "Vector uint3 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[4].x) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[4].y) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[4].z) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[5].x) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[5].y) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[5].z) << "Vector uint3 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[6].x) << "Vector uint3 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[6].y) << "Vector uint3 left shift failed.";
    EXPECT_EQ(128u << 6u, mem[6].z) << "Vector uint3 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[7].x) << "Vector uint3 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[7].y) << "Vector uint3 left shift failed.";
    EXPECT_EQ(128u << 2u, mem[7].z) << "Vector uint3 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[8].x) << "Vector uint3 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[8].y) << "Vector uint3 right shift failed.";
    EXPECT_EQ(128u >> 6u, mem[8].z) << "Vector uint3 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[9].x) << "Vector uint3 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[9].y) << "Vector uint3 right shift failed.";
    EXPECT_EQ(128u >> 2u, mem[9].z) << "Vector uint3 right shift failed.";
  }
  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ(mask01 bitand mask02, mem[0].x) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[0].y) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[0].z) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask03, mem[0].w) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].x) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].y) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].z) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask01 bitand mask02, mem[1].w) << "Vector uint4 bitwise AND failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[2].x) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[2].y) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[2].z) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask05, mem[2].w) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[3].x) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[3].y) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 bitor mask04, mem[3].z) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask03 bitor mask04, mem[3].w) << "Vector uint4 bitwise OR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[4].x) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[4].y) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[4].z) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask05, mem[4].w) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[5].x) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[5].y) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask02 xor mask04, mem[5].z) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(mask03 xor mask04, mem[5].w) << "Vector uint4 bitwise XOR failed.";
    EXPECT_EQ(32u << 2u, mem[6].x) << "Vector uint4 left shift failed.";
    EXPECT_EQ(64u << 4u, mem[6].y) << "Vector uint4 left shift failed.";
    EXPECT_EQ(128u << 6u, mem[6].z) << "Vector uint4 left shift failed.";
    EXPECT_EQ(256u << 8u, mem[6].w) << "Vector uint4 left shift failed.";
    EXPECT_EQ(32u << 2u, mem[7].x) << "Vector uint4 left shift failed.";
    EXPECT_EQ(64u << 2u, mem[7].y) << "Vector uint4 left shift failed.";
    EXPECT_EQ(128u << 2u, mem[7].z) << "Vector uint4 left shift failed.";
    EXPECT_EQ(256u << 2u, mem[7].w) << "Vector uint4 left shift failed.";
    EXPECT_EQ(32u >> 2u, mem[8].x) << "Vector uint4 right shift failed.";
    EXPECT_EQ(64u >> 4u, mem[8].y) << "Vector uint4 right shift failed.";
    EXPECT_EQ(128u >> 6u, mem[8].z) << "Vector uint4 right shift failed.";
    EXPECT_EQ(256u >> 8u, mem[8].w) << "Vector uint4 right shift failed.";
    EXPECT_EQ(32u >> 2u, mem[9].x) << "Vector uint4 right shift failed.";
    EXPECT_EQ(64u >> 2u, mem[9].y) << "Vector uint4 right shift failed.";
    EXPECT_EQ(128u >> 2u, mem[9].z) << "Vector uint4 right shift failed.";
    EXPECT_EQ(256u >> 2u, mem[9].w) << "Vector uint4 right shift failed.";
  }
}

TEST(ClCppTest, VectorIncrementDecrementTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  const zivc::SharedBuffer buffer_inout4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{0, 1, 2, 3},
                                              cl_int4{0, 0, 0, 0},
                                              cl_int4{0, 1, 2, 3},
                                              cl_int4{0, 0, 0, 0},
                                              cl_int4{0, 1, 2, 3},
                                              cl_int4{0, 0, 0, 0},
                                              cl_int4{0, 1, 2, 3},
                                              cl_int4{0, 0, 0, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_inout4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorIncrementDecrementTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorIncrementDecrementTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_inout4, launch_options);
  device->waitForCompletion(result.fence());

  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_inout4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    EXPECT_EQ( 1, mem[0].x) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 2, mem[0].y) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 3, mem[0].z) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 4, mem[0].w) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 0, mem[1].x) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 1, mem[1].y) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 2, mem[1].z) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 3, mem[1].w) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 1, mem[2].x) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 2, mem[2].y) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 3, mem[2].z) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 4, mem[2].w) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 1, mem[3].x) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 2, mem[3].y) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 3, mem[3].z) << "Vector int4 incremenet failed.";
    EXPECT_EQ( 4, mem[3].w) << "Vector int4 incremenet failed.";
    EXPECT_EQ(-1, mem[4].x) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 0, mem[4].y) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 1, mem[4].z) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 2, mem[4].w) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 0, mem[5].x) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 1, mem[5].y) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 2, mem[5].z) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 3, mem[5].w) << "Vector int4 decremenet failed.";
    EXPECT_EQ(-1, mem[6].x) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 0, mem[6].y) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 1, mem[6].z) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 2, mem[6].w) << "Vector int4 decremenet failed.";
    EXPECT_EQ(-1, mem[7].x) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 0, mem[7].y) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 1, mem[7].z) << "Vector int4 decremenet failed.";
    EXPECT_EQ( 2, mem[7].w) << "Vector int4 decremenet failed.";
  }
}

TEST(ClCppTest, VectorConditionalOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  const zivc::SharedBuffer buffer_inout4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, 0, 0, 0},
                                              cl_int4{1, 2, 3, 4},
                                              cl_int4{-1, -2, -3, -4},
                                              cl_int4{0, 0, 0, 0},
                                              cl_int4{0, 0, 0, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_inout4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConditionalOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorConditionalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_inout4, launch_options);
  device->waitForCompletion(result.fence());

  // output4
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_inout4, buffer.get());
    const auto mem = buffer->mapMemory();
    EXPECT_EQ( 1, mem[3].x) << "Vector int4 conditional operator failed.";
    EXPECT_EQ( 2, mem[3].y) << "Vector int4 conditional operator failed.";
    EXPECT_EQ( 3, mem[3].z) << "Vector int4 conditional operator failed.";
    EXPECT_EQ( 4, mem[3].w) << "Vector int4 conditional operator failed.";
    EXPECT_EQ(-1, mem[4].x) << "Vector int4 conditional operator failed.";
    EXPECT_EQ(-2, mem[4].y) << "Vector int4 conditional operator failed.";
    EXPECT_EQ(-3, mem[4].z) << "Vector int4 conditional operator failed.";
    EXPECT_EQ(-4, mem[4].w) << "Vector int4 conditional operator failed.";
  }
}

TEST(ClCppTest, VectorComparisonOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, 1, 1, 1},
                                              cl_int4{-1, 0, 1, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(72 + 24);
  {
    cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComparisonOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorConditionalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(2, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in4, *buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 not equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 less than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 greater than or equal to failed.";

    // Constexpr test
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
  }
}

TEST(ClCppTest, VectorComparisonResultTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_uchar;
  using zivc::cl_char2;
  using zivc::cl_ushort3;
  using zivc::cl_uint4;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_uchar>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_char2>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_ushort3>(zivc::BufferUsage::kPreferDevice);
  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_uchar> v = {cl_uchar{1},
                                               cl_uchar{0}};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  {
    const std::initializer_list<cl_char2> v = {cl_char2{1, 0},
                                               cl_char2{0, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  {
    const std::initializer_list<cl_ushort3> v = {cl_ushort3{1, 0, 1},
                                                 cl_ushort3{0, 0, 1}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  {
    const std::initializer_list<cl_uint4> v = {cl_uint4{1, 0, 1, 0},
                                               cl_uint4{0, 0, 1, 1}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<zivc::cl_CompResult<cl_uchar>>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(6);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<zivc::cl_CompResult<cl_char2>>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(6);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<zivc::cl_CompResult<cl_ushort3>>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(6);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<zivc::cl_CompResult<cl_uint4>>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(6);

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComparisonResultTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(8, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorConditionalResultTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(8, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_in4, *buffer_out1, *buffer_out2, *buffer_out3, *buffer_out4, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    using ResultT = zivc::cl_CompResult<cl_uchar>;
    const zivc::SharedBuffer buffer = device->createBuffer<ResultT>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    //! \note scalar operation doesn't return 1 or 0 on GPU?
    //const zivc::MappedMemory mem = buffer->mapMemory();
    //ASSERT_EQ(zivc::cl::kSFalse, mem[0]) << "Scalar equal operation failed.";
    //ASSERT_EQ(zivc::cl::kSTrue, mem[1]) << "Scalar not equal operation failed.";
    //ASSERT_EQ(zivc::cl::kSTrue, mem[2]) << "Scalar greater operation failed.";
    //ASSERT_EQ(zivc::cl::kSTrue, mem[3]) << "Scalar greater or equal operation failed.";
    //ASSERT_EQ(zivc::cl::kSFalse, mem[4]) << "Scalar less operation failed.";
    //ASSERT_EQ(zivc::cl::kSFalse, mem[5]) << "Scalar less or equal operation failed.";
  }
  {
    using ResultT = zivc::cl_CompResult<cl_char2>;
    const zivc::SharedBuffer buffer = device->createBuffer<ResultT>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    constexpr auto t = static_cast<ResultT::Type>(zivc::cl::kVTrue);
    constexpr auto f = static_cast<ResultT::Type>(zivc::cl::kVFalse);
    ASSERT_EQ(f, mem[0].x) << "vector2 equal operation failed.";
    ASSERT_EQ(t, mem[0].y) << "vector2 equal operation failed.";
    ASSERT_EQ(t, mem[1].x) << "vector2 not equal operation failed.";
    ASSERT_EQ(f, mem[1].y) << "vector2 not equal operation failed.";
    ASSERT_EQ(t, mem[2].x) << "vector2 greater operation failed.";
    ASSERT_EQ(f, mem[2].y) << "vector2 greater operation failed.";
    ASSERT_EQ(t, mem[3].x) << "vector2 greater or equal operation failed.";
    ASSERT_EQ(t, mem[3].y) << "vector2 greater or equal operation failed.";
    ASSERT_EQ(f, mem[4].x) << "vector2 less operation failed.";
    ASSERT_EQ(f, mem[4].y) << "vector2 less operation failed.";
    ASSERT_EQ(f, mem[5].x) << "vector2 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].y) << "vector2 less or equal operation failed.";
  }
  {
    using ResultT = zivc::cl_CompResult<cl_ushort3>;
    const zivc::SharedBuffer buffer = device->createBuffer<ResultT>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    constexpr auto t = static_cast<ResultT::Type>(zivc::cl::kVTrue);
    constexpr auto f = static_cast<ResultT::Type>(zivc::cl::kVFalse);
    ASSERT_EQ(f, mem[0].x) << "vector3 equal operation failed.";
    ASSERT_EQ(t, mem[0].y) << "vector3 equal operation failed.";
    ASSERT_EQ(t, mem[0].z) << "vector3 equal operation failed.";
    ASSERT_EQ(t, mem[1].x) << "vector3 not equal operation failed.";
    ASSERT_EQ(f, mem[1].y) << "vector3 not equal operation failed.";
    ASSERT_EQ(f, mem[1].z) << "vector3 not equal operation failed.";
    ASSERT_EQ(t, mem[2].x) << "vector3 greater operation failed.";
    ASSERT_EQ(f, mem[2].y) << "vector3 greater operation failed.";
    ASSERT_EQ(f, mem[2].z) << "vector3 greater operation failed.";
    ASSERT_EQ(t, mem[3].x) << "vector3 greater or equal operation failed.";
    ASSERT_EQ(t, mem[3].y) << "vector3 greater or equal operation failed.";
    ASSERT_EQ(t, mem[3].z) << "vector3 greater or equal operation failed.";
    ASSERT_EQ(f, mem[4].x) << "vector3 less operation failed.";
    ASSERT_EQ(f, mem[4].y) << "vector3 less operation failed.";
    ASSERT_EQ(f, mem[4].z) << "vector3 less operation failed.";
    ASSERT_EQ(f, mem[5].x) << "vector3 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].y) << "vector3 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].z) << "vector3 less or equal operation failed.";
  }
  {
    using ResultT = zivc::cl_CompResult<cl_uint4>;
    const zivc::SharedBuffer buffer = device->createBuffer<ResultT>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    constexpr auto t = static_cast<ResultT::Type>(zivc::cl::kVTrue);
    constexpr auto f = static_cast<ResultT::Type>(zivc::cl::kVFalse);
    ASSERT_EQ(f, mem[0].x) << "vector4 equal operation failed.";
    ASSERT_EQ(t, mem[0].y) << "vector4 equal operation failed.";
    ASSERT_EQ(t, mem[0].z) << "vector4 equal operation failed.";
    ASSERT_EQ(f, mem[0].w) << "vector4 equal operation failed.";
    ASSERT_EQ(t, mem[1].x) << "vector4 not equal operation failed.";
    ASSERT_EQ(f, mem[1].y) << "vector4 not equal operation failed.";
    ASSERT_EQ(f, mem[1].z) << "vector4 not equal operation failed.";
    ASSERT_EQ(t, mem[1].w) << "vector4 not equal operation failed.";
    ASSERT_EQ(t, mem[2].x) << "vector4 greater operation failed.";
    ASSERT_EQ(f, mem[2].y) << "vector4 greater operation failed.";
    ASSERT_EQ(f, mem[2].z) << "vector4 greater operation failed.";
    ASSERT_EQ(f, mem[2].w) << "vector4 greater operation failed.";
    ASSERT_EQ(t, mem[3].x) << "vector4 greater or equal operation failed.";
    ASSERT_EQ(t, mem[3].y) << "vector4 greater or equal operation failed.";
    ASSERT_EQ(t, mem[3].z) << "vector4 greater or equal operation failed.";
    ASSERT_EQ(f, mem[3].w) << "vector4 greater or equal operation failed.";
    ASSERT_EQ(f, mem[4].x) << "vector4 less operation failed.";
    ASSERT_EQ(f, mem[4].y) << "vector4 less operation failed.";
    ASSERT_EQ(f, mem[4].z) << "vector4 less operation failed.";
    ASSERT_EQ(t, mem[4].w) << "vector4 less operation failed.";
    ASSERT_EQ(f, mem[5].x) << "vector4 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].y) << "vector4 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].z) << "vector4 less or equal operation failed.";
    ASSERT_EQ(t, mem[5].w) << "vector4 less or equal operation failed.";
  }
}

TEST(ClCppTest, VectorLogicalOperatorTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, 1, 1, 1},
                                              cl_int4{1, 0, 1, 0},
                                              cl_int4{0, 0, 1, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(28 + 12);
  {
    cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLogicalOperatorTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLogicalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(2, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in4, *buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_FALSE(mem[index++]) << "Vector int4 negation failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 negation failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 negation failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 negation failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 AND failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_TRUE(mem[index++]) << "Vector int4 OR failed.";
    EXPECT_FALSE(mem[index++]) << "Vector int4 OR failed.";

    // Constexpr test
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_TRUE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
    EXPECT_FALSE(mem[index++]) << "Vector constexpr test failed.";
  }
}

TEST(ClCppTest, ScalarCastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  const zivc::SharedBuffer buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }

  const auto int_n = static_cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = static_cast<zivc::uint32b>(buffer_in1->size());
  const std::size_t n = int_n + float_n;

  const zivc::SharedBuffer buffer_out0 = device->createBuffer<cl_char>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uchar>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_short>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_ushort>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out5 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  const zivc::SharedBuffer buffer_out6 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, scalarCastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(11, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("ScalarCastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(11, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in0, *buffer_in1,
                                          *buffer_out0, *buffer_out1, *buffer_out2,
                                          *buffer_out3, *buffer_out4, *buffer_out5,
                                          *buffer_out6,
                                          int_n, float_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_char>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(-1, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(0, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(0, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int8b cast failed";
    EXPECT_EQ(3, mem[index++]) << "int8b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uchar>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uchar vmax = (std::numeric_limits<cl_uchar>::max)();
    const cl_uchar vmin = (std::numeric_limits<cl_uchar>::min)();
    EXPECT_EQ(0, mem[index++]) << "uint8b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint8b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint8b cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index++]) << "uint8b cast failed";
    EXPECT_EQ(vmin, mem[index++]) << "uint8b cast failed";
    EXPECT_EQ(0, mem[index++]) << "uint8b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint8b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint8b cast failed";
    index++;
    EXPECT_EQ(3, mem[index++]) << "uint8b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_short>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(-1, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(0, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(0, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int16b cast failed";
    EXPECT_EQ(3, mem[index++]) << "int16b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_ushort>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_ushort vmax = (std::numeric_limits<cl_ushort>::max)();
    const cl_ushort vmin = (std::numeric_limits<cl_ushort>::min)();
    EXPECT_EQ(0, mem[index++]) << "uint16b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint16b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint16b cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index++]) << "uint16b cast failed";
    EXPECT_EQ(vmin, mem[index++]) << "uint16b cast failed";
    EXPECT_EQ(0, mem[index++]) << "uint16b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint16b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint16b cast failed";
    index++;
    EXPECT_EQ(3, mem[index++]) << "uint16b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_int vmax = (std::numeric_limits<cl_int>::max)();
    const cl_int vmin = (std::numeric_limits<cl_int>::min)();
    EXPECT_EQ(0, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(vmax, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(vmin, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(0, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(100, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(-100, mem[index++]) << "int32b cast failed";
    EXPECT_EQ(3, mem[index++]) << "int32b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uint vmax = (std::numeric_limits<cl_uint>::max)();
    //const cl_uint vmin = (std::numeric_limits<cl_uint>::min)();
    EXPECT_EQ(0, mem[index++]) << "uint32b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint32b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint32b cast failed";
    index++;
    EXPECT_EQ(vmax / 2, mem[index++]) << "uint32b cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index++]) << "uint32b cast failed";
    EXPECT_EQ(0, mem[index++]) << "uint32b cast failed";
    EXPECT_EQ(100, mem[index++]) << "uint32b cast failed";
    //EXPECT_EQ(0, mem[index++]) << "uint32b cast failed";
    index++;
    EXPECT_EQ(3, mem[index++]) << "uint32b cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const auto vmax = static_cast<cl_float>((std::numeric_limits<cl_int>::max)());
    const auto vmin = static_cast<cl_float>((std::numeric_limits<cl_int>::min)());
    EXPECT_FLOAT_EQ(0.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index++]) << "float cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index++]) << "float cast failed";
  }
}

TEST(ClCppTest, ScalarBoolCastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  const zivc::SharedBuffer buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {0, 1, -1, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }

  const auto int_n = static_cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = static_cast<zivc::uint32b>(buffer_in1->size());
  const std::size_t n = int_n + float_n;

  const zivc::SharedBuffer buffer_out0 = device->createBuffer<zivc::cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    zivc::cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, scalarBoolCastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("ScalarBoolCastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in0, *buffer_in1, *buffer_out0,
                                          int_n, float_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<zivc::cl_Boolean>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_FALSE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_FALSE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
    EXPECT_TRUE(mem[index++]) << "bool cast failed";
  }
}

TEST(ClCppTest, Vector2CastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_char2;
  using zivc::cl_uchar2;
  using zivc::cl_short2;
  using zivc::cl_ushort2;
  using zivc::cl_int2;
  using zivc::cl_uint2;
  using zivc::cl_float2;

  const zivc::SharedBuffer buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int2> v = {cl_int2{0, 100},
                                              cl_int2{vmax, vmin}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float2> v = {cl_float2{0.0f, 100.0f},
                                                cl_float2{-100.0f, 3.14f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = static_cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = static_cast<zivc::uint32b>(buffer_in1->size());
  const auto int2_n = static_cast<zivc::uint32b>(buffer_in2->size());
  const auto float2_n = static_cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int2_n + float2_n;

  const zivc::SharedBuffer buffer_out0 = device->createBuffer<cl_char2>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uchar2>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_short2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_ushort2>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out5 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  const zivc::SharedBuffer buffer_out6 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float2 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector2CastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector2CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(
      *buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
      *buffer_out0, *buffer_out1, *buffer_out2,
      *buffer_out3, *buffer_out4, *buffer_out5,
      *buffer_out6, 
      int_n, float_n, int2_n, float2_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_char2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "char2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uchar2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uchar vmax = (std::numeric_limits<cl_uchar>::max)();
    const cl_uchar vmin = (std::numeric_limits<cl_uchar>::min)();
    EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "uchar2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uchar2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_short2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "short2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_ushort2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_ushort vmax = (std::numeric_limits<cl_ushort>::max)();
    const cl_ushort vmin = (std::numeric_limits<cl_ushort>::min)();
    EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "ushort2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "ushort2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_int vmax = (std::numeric_limits<cl_int>::max)();
    const cl_int vmin = (std::numeric_limits<cl_int>::min)();
    EXPECT_EQ(0, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(vmax, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(vmin, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(vmax, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int2 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "int2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uint vmax = (std::numeric_limits<cl_uint>::max)();
    //const cl_uint vmin = (std::numeric_limits<cl_uint>::min)();
    EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint2 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint2 cast failed";
    ++index;
    //EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(vmax / 2, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(vmax / 2 + 1, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    //EXPECT_EQ(0, mem[index]) << "uint2 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint2 cast failed";
    ++index;
    EXPECT_EQ(vmax / 2, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].y) << "uint2 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint2 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uint2 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uint2 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const auto vmax = static_cast<cl_float>((std::numeric_limits<cl_int>::max)());
    const auto vmin = static_cast<cl_float>((std::numeric_limits<cl_int>::min)());
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmax, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmin, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(3.14f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmax, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float2 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float2 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].y) << "float2 cast failed";
  }
}

TEST(ClCppTest, Vector3CastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_char3;
  using zivc::cl_uchar3;
  using zivc::cl_short3;
  using zivc::cl_ushort3;
  using zivc::cl_int3;
  using zivc::cl_uint3;
  using zivc::cl_float3;

  const zivc::SharedBuffer buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    const std::initializer_list<cl_int3> v = {cl_int3{0, 100, vmax}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float3> v = {cl_float3{0.0f, 100.0f, -100.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = static_cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = static_cast<zivc::uint32b>(buffer_in1->size());
  const auto int3_n = static_cast<zivc::uint32b>(buffer_in2->size());
  const auto float3_n = static_cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int3_n + float3_n;

  const zivc::SharedBuffer buffer_out0 = device->createBuffer<cl_char3>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uchar3>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_short3>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_ushort3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out5 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  const zivc::SharedBuffer buffer_out6 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float3 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector3CastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector3CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(
      *buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
      *buffer_out0, *buffer_out1, *buffer_out2,
      *buffer_out3, *buffer_out4, *buffer_out5,
      *buffer_out6, 
      int_n, float_n, int3_n, float3_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_char3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "char3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uchar3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uchar vmax = (std::numeric_limits<cl_uchar>::max)();
    const cl_uchar vmin = (std::numeric_limits<cl_uchar>::min)();
    EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uchar3 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uchar3 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "uchar3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_short3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "short3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_ushort3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_ushort vmax = (std::numeric_limits<cl_ushort>::max)();
    const cl_ushort vmin = (std::numeric_limits<cl_ushort>::min)();
    EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "ushort3 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "ushort3 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "ushort3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_int vmax = (std::numeric_limits<cl_int>::max)();
    const cl_int vmin = (std::numeric_limits<cl_int>::min)();
    EXPECT_EQ(0, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(vmax, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(vmin, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "int3 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int3 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uint vmax = (std::numeric_limits<cl_uint>::max)();
    //const cl_uint vmin = (std::numeric_limits<cl_uint>::min)();
    EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uint3 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uint3 cast failed";
    ++index;
    //EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uint3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(vmax / 2, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(vmax / 2 + 1, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uint3 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uint3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(3, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint3 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].z) << "uint3 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint3 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint3 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint3 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const auto vmax = static_cast<cl_float>((std::numeric_limits<cl_int>::max)());
    const auto vmin = static_cast<cl_float>((std::numeric_limits<cl_int>::min)());
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmax, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmin, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(3.14f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].z) << "float3 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float3 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float3 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float3 cast failed";
  }
}

TEST(ClCppTest, Vector4CastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;
  using zivc::cl_char4;
  using zivc::cl_uchar4;
  using zivc::cl_short4;
  using zivc::cl_ushort4;
  using zivc::cl_int4;
  using zivc::cl_uint4;
  using zivc::cl_float4;

  const zivc::SharedBuffer buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    const std::initializer_list<cl_int4> v = {cl_int4{0, 100, vmax, vmin}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    const std::initializer_list<cl_float4> v = {cl_float4{0.0f, 100.0f, -100.0f, 3.14f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = static_cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = static_cast<zivc::uint32b>(buffer_in1->size());
  const auto int4_n = static_cast<zivc::uint32b>(buffer_in2->size());
  const auto float4_n = static_cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int4_n + float4_n;

  const zivc::SharedBuffer buffer_out0 = device->createBuffer<cl_char4>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uchar4>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_short4>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_ushort4>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out5 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  const zivc::SharedBuffer buffer_out6 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float4 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector4CastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector4CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(
      *buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
      *buffer_out0, *buffer_out1, *buffer_out2,
      *buffer_out3, *buffer_out4, *buffer_out5,
      *buffer_out6, 
      int_n, float_n, int4_n, float4_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_char4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(-1, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "char4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "char4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "char4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "char4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "char4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uchar4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uchar vmax = (std::numeric_limits<cl_uchar>::max)();
    const cl_uchar vmin = (std::numeric_limits<cl_uchar>::min)();
    EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "uchar4 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(vmax, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "uchar4 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "uchar4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uchar4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uchar4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uchar4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "uchar4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_short4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    EXPECT_EQ(0, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(-1, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(-1, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(-1, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(-1, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "short4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "short4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "short4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "short4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "short4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_ushort4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_ushort vmax = (std::numeric_limits<cl_ushort>::max)();
    const cl_ushort vmin = (std::numeric_limits<cl_ushort>::min)();
    EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "ushort4 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(vmax, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(vmax, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(vmin, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "ushort4 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "ushort4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "ushort4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "ushort4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "ushort4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "ushort4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_int vmax = (std::numeric_limits<cl_int>::max)();
    const cl_int vmin = (std::numeric_limits<cl_int>::min)();
    EXPECT_EQ(0, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(vmax, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(vmax, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(vmax, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(vmin, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(vmin, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(vmin, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(-100, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(3, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(vmax, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(vmin, mem[index].w) << "int4 cast failed";
    ++index;
    EXPECT_EQ(0, mem[index].x) << "int4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "int4 cast failed";
    EXPECT_EQ(-100, mem[index].z) << "int4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "int4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const cl_uint vmax = (std::numeric_limits<cl_uint>::max)();
    //const cl_uint vmin = (std::numeric_limits<cl_uint>::min)();
    EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "uint4 cast failed";
    ++index;
    EXPECT_EQ(100, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "uint4 cast failed";
    ++index;
    //EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(vmax / 2, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(vmax / 2 + 1, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(0, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(100, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].w) << "uint4 cast failed";
    index++;
    //EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].y) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(3, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(3, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(3, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(vmax / 2 + 1, mem[index].w) << "uint4 cast failed";
    index++;
    EXPECT_EQ(0, mem[index].x) << "uint4 cast failed";
    EXPECT_EQ(100, mem[index].y) << "uint4 cast failed";
    //EXPECT_EQ(0, mem[index].z) << "uint4 cast failed";
    EXPECT_EQ(3, mem[index].w) << "uint4 cast failed";
  }
  {
    const zivc::SharedBuffer buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const zivc::MappedMemory mem = buffer->mapMemory();
    std::size_t index = 0;
    const auto vmax = static_cast<cl_float>((std::numeric_limits<cl_int>::max)());
    const auto vmin = static_cast<cl_float>((std::numeric_limits<cl_int>::min)());
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmax, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(vmin, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(0.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(100.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(-100.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(3.14f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmax, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(vmin, mem[index].w) << "float4 cast failed";
    ++index;
    EXPECT_FLOAT_EQ(0.0f, mem[index].x) << "float4 cast failed";
    EXPECT_FLOAT_EQ(100.0f, mem[index].y) << "float4 cast failed";
    EXPECT_FLOAT_EQ(-100.0f, mem[index].z) << "float4 cast failed";
    EXPECT_FLOAT_EQ(3.14f, mem[index].w) << "float4 cast failed";
  }
}

TEST(ClCppTest, BitCastTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_char2;
  using zivc::cl_char3;
  using zivc::cl_char4;
  using zivc::cl_uchar2;
  using zivc::cl_uchar3;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short3;
  using zivc::cl_short4;
  using zivc::cl_ushort2;
  using zivc::cl_ushort3;
  using zivc::cl_ushort4;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  constexpr cl_int ei = zisc::bit_cast<cl_int>(std::numbers::pi_v<float>);
  const zivc::SharedBuffer inout_int = device->createBuffer<cl_int>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_int->setSize(1);
  ztest::setDeviceBuffer(*device, {ei}, inout_int.get());

  constexpr float ef = std::numbers::e_v<float>;
  const zivc::SharedBuffer inout_float = device->createBuffer<float>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_float->setSize(1);
  ztest::setDeviceBuffer(*device, {ef}, inout_float.get());

  constexpr cl_uint2 eu2 = cl_uint2{zisc::bit_cast<cl_uint>(std::numbers::pi_v<float>),
                                    zisc::bit_cast<cl_uint>(std::numbers::e_v<float>)};
  const zivc::SharedBuffer inout_uint2 = device->createBuffer<cl_uint2>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_uint2->setSize(1);
  ztest::setDeviceBuffer(*device, {eu2}, inout_uint2.get());

  constexpr cl_float2 ef2 = cl_float2{std::numbers::e_v<float>,
                                      std::numbers::pi_v<float>};
  const zivc::SharedBuffer inout_float2 = device->createBuffer<cl_float2>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_float2->setSize(1);
  ztest::setDeviceBuffer(*device, {ef2}, inout_float2.get());

  constexpr cl_char3 ec3 = cl_char3{1, 2, 3};
  const zivc::SharedBuffer inout_char3 = device->createBuffer<cl_char3>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_char3->setSize(1);
  ztest::setDeviceBuffer(*device, {ec3}, inout_char3.get());

  constexpr cl_uchar4 euc4 = cl_uchar4{4, 5, 6, 7};
  const zivc::SharedBuffer inout_uchar4 = device->createBuffer<cl_uchar4>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_uchar4->setSize(1);
  ztest::setDeviceBuffer(*device, {euc4}, inout_uchar4.get());

  constexpr cl_ushort3 eus3 = cl_ushort3{1, 2, 3};
  const zivc::SharedBuffer inout_ushort3 = device->createBuffer<cl_ushort3>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_ushort3->setSize(1);
  ztest::setDeviceBuffer(*device, {eus3}, inout_ushort3.get());

  constexpr cl_short4 es4 = cl_short4{4, 5, 6, 7};
  const zivc::SharedBuffer inout_short4 = device->createBuffer<cl_short4>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_short4->setSize(1);
  ztest::setDeviceBuffer(*device, {es4}, inout_short4.get());

  constexpr cl_int3 ei3 = cl_int3{zisc::bit_cast<cl_int>(std::numbers::pi_v<float>),
                                  zisc::bit_cast<cl_int>(std::numbers::e_v<float>),
                                  zisc::bit_cast<cl_int>(std::numbers::ln2_v<float>)};
  const zivc::SharedBuffer inout_int3 = device->createBuffer<cl_int3>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_int3->setSize(3);
  ztest::setDeviceBuffer(*device, {ei3, ei3, ei3}, inout_int3.get());

  constexpr cl_int4 ei4 = cl_int4{zisc::bit_cast<cl_int>(std::numbers::pi_v<float>),
                                  zisc::bit_cast<cl_int>(std::numbers::e_v<float>),
                                  zisc::bit_cast<cl_int>(std::numbers::ln2_v<float>),
                                  zisc::bit_cast<cl_int>(std::numbers::sqrt2_v<float>)};
  const zivc::SharedBuffer inout_int4 = device->createBuffer<cl_int4>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_int4->setSize(3);
  ztest::setDeviceBuffer(*device, {ei4, ei4, ei4}, inout_int4.get());

  constexpr cl_float3 ef3 = cl_float3{std::numbers::ln2_v<float>,
                                      std::numbers::pi_v<float>,
                                      std::numbers::e_v<float>};
  const zivc::SharedBuffer inout_float3 = device->createBuffer<cl_float3>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_float3->setSize(3);
  ztest::setDeviceBuffer(*device, {ef3, ef3, ef3}, inout_float3.get());

  constexpr cl_float4 ef4 = cl_float4{std::numbers::sqrt2_v<float>,
                                      std::numbers::ln2_v<float>,
                                      std::numbers::e_v<float>,
                                      std::numbers::pi_v<float>};
  const zivc::SharedBuffer inout_float4 = device->createBuffer<cl_float4>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  inout_float4->setSize(3);
  ztest::setDeviceBuffer(*device, {ef4, ef4, ef4}, inout_float4.get());

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, bitCastTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("BitCastTestKernel");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(12, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(
      *inout_int, *inout_float, *inout_uint2, *inout_float2,
      *inout_char3, *inout_uchar4, *inout_ushort3, *inout_short4,
      *inout_int3, *inout_int4, *inout_float3, *inout_float4,
      launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = inout_int->mapMemory();
    constexpr auto expected = zisc::bit_cast<cl_int>(ef);
    ASSERT_EQ(expected, mem[0]) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_float->mapMemory();
    constexpr auto expected = zisc::bit_cast<float>(ei);
    ASSERT_FLOAT_EQ(expected, mem[0]) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_uint2->mapMemory();
    const auto expected = zisc::bit_cast<cl_uint2>(ef2);
    ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_float2->mapMemory();
    const auto expected = zisc::bit_cast<cl_float2>(eu2);
    ASSERT_FLOAT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_FLOAT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_char3->mapMemory();
    const auto expected = zisc::bit_cast<cl_char3>(euc4);
    ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
    ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_uchar4->mapMemory();
    const auto expected = zisc::bit_cast<cl_uchar4>(ec3);
    ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
    ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_ushort3->mapMemory();
    const auto expected = zisc::bit_cast<cl_ushort3>(es4);
    ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
    ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_short4->mapMemory();
    const auto expected = zisc::bit_cast<cl_short4>(eus3);
    ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
    ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
    ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
  }
  {
    const zivc::MappedMemory mem = inout_int3->mapMemory();
    {
      const auto expected = zisc::bit_cast<cl_int3>(ei4);
      ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_int3>(ef3);
      ASSERT_EQ(expected.x, mem[1].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[1].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[1].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_int3>(ef4);
      ASSERT_EQ(expected.x, mem[2].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[2].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[2].z) << "Bit cast test failed.";
    }
  }
  {
    const zivc::MappedMemory mem = inout_int4->mapMemory();
    {
      const auto expected = zisc::bit_cast<cl_int4>(ei3);
      ASSERT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_int4>(ef3);
      ASSERT_EQ(expected.x, mem[1].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[1].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[1].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_int4>(ef4);
      ASSERT_EQ(expected.x, mem[2].x) << "Bit cast test failed.";
      ASSERT_EQ(expected.y, mem[2].y) << "Bit cast test failed.";
      ASSERT_EQ(expected.z, mem[2].z) << "Bit cast test failed.";
      ASSERT_EQ(expected.w, mem[2].w) << "Bit cast test failed.";
    }
  }
  {
    const zivc::MappedMemory mem = inout_float3->mapMemory();
    {
      const auto expected = zisc::bit_cast<cl_float3>(ei3);
      ASSERT_FLOAT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_float3>(ei4);
      ASSERT_FLOAT_EQ(expected.x, mem[1].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[1].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[1].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_float3>(ef4);
      ASSERT_FLOAT_EQ(expected.x, mem[2].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[2].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[2].z) << "Bit cast test failed.";
    }
  }
  {
    const zivc::MappedMemory mem = inout_float4->mapMemory();
    {
      const auto expected = zisc::bit_cast<cl_float4>(ei3);
      ASSERT_FLOAT_EQ(expected.x, mem[0].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[0].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[0].z) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_float4>(ei4);
      ASSERT_FLOAT_EQ(expected.x, mem[1].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[1].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[1].z) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.w, mem[1].w) << "Bit cast test failed.";
    }
    {
      const auto expected = zisc::bit_cast<cl_float4>(ef3);
      ASSERT_FLOAT_EQ(expected.x, mem[2].x) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.y, mem[2].y) << "Bit cast test failed.";
      ASSERT_FLOAT_EQ(expected.z, mem[2].z) << "Bit cast test failed.";
    }
  }
}

TEST(ClCppTest, VectorLoadStoreTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_char2;
  using zivc::cl_char3;
  using zivc::cl_char4;
  using zivc::cl_uchar2;
  using zivc::cl_uchar3;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short3;
  using zivc::cl_short4;
  using zivc::cl_ushort2;
  using zivc::cl_ushort3;
  using zivc::cl_ushort4;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_half;
  using zivc::cl_half2;
  using zivc::cl_half3;
  using zivc::cl_half4;

  const std::initializer_list<cl_char> ei8 = {1, 2, 1, 2,
                                              1, 2, 3, 1, 2, 3,
                                              1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_i8 = device->createBuffer<cl_char>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ei8, inout_i8.get());

  const std::initializer_list<cl_ushort> eu16 = {1, 2, 1, 2,
                                                 1, 2, 3, 1, 2, 3,
                                                 1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_u16 = device->createBuffer<cl_ushort>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, eu16, inout_u16.get());

  const std::initializer_list<cl_int> ei32 = {1, 2, 1, 2,
                                              1, 2, 3, 1, 2, 3,
                                              1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_i32 = device->createBuffer<cl_int>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ei32, inout_i32.get());

  const std::initializer_list<float> ef = {1.0f, 2.0f, 1.0f, 2.0f,
                                           1.0f, 2.0f ,4.0f, 1.0f, 2.0f, 4.0f,
                                           1.0f, 2.0f, 4.0f, 8.0f, 1.0f, 2.0f, 4.0f, 8.0f};
  const zivc::SharedBuffer inout_f = device->createBuffer<float>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ef, inout_f.get());

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLoadStoreTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLoadStoreTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*inout_i8, *inout_u16, *inout_i32,
                                          *inout_f, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const std::span<const cl_char> e{ei8.begin(), ei8.end()};
    const zivc::MappedMemory mem = inout_i8->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }
  {
    const std::span<const cl_ushort> e{eu16.begin(), eu16.end()};
    const zivc::MappedMemory mem = inout_u16->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }
  {
    const std::span<const cl_int> e{ei32.begin(), ei32.end()};
    const zivc::MappedMemory mem = inout_i32->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }

  {
    const std::span<const float> e{ef.begin(), ef.end()};
    const zivc::MappedMemory mem = inout_f->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
  }
}

TEST(ClCppTest, VectorLoadStoreClTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_char2;
  using zivc::cl_char3;
  using zivc::cl_char4;
  using zivc::cl_uchar2;
  using zivc::cl_uchar3;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short3;
  using zivc::cl_short4;
  using zivc::cl_ushort2;
  using zivc::cl_ushort3;
  using zivc::cl_ushort4;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_half;
  using zivc::cl_half2;
  using zivc::cl_half3;
  using zivc::cl_half4;

  const std::initializer_list<cl_char> ei8 = {1, 2, 1, 2,
                                              1, 2, 3, 1, 2, 3,
                                              1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_i8 = device->createBuffer<cl_char>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ei8, inout_i8.get());

  const std::initializer_list<cl_ushort> eu16 = {1, 2, 1, 2,
                                                 1, 2, 3, 1, 2, 3,
                                                 1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_u16 = device->createBuffer<cl_ushort>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, eu16, inout_u16.get());

  const std::initializer_list<cl_int> ei32 = {1, 2, 1, 2,
                                              1, 2, 3, 1, 2, 3,
                                              1, 2, 3, 4, 1, 2, 3, 4};
  const zivc::SharedBuffer inout_i32 = device->createBuffer<cl_int>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ei32, inout_i32.get());

  const std::initializer_list<float> ef = {1.0f, 2.0f, 1.0f, 2.0f,
                                           1.0f, 2.0f ,4.0f, 1.0f, 2.0f, 4.0f,
                                           1.0f, 2.0f, 4.0f, 8.0f, 1.0f, 2.0f, 4.0f, 8.0f};
  const zivc::SharedBuffer inout_f = device->createBuffer<float>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, ef, inout_f.get());

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLoadStoreClTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(4, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLoadStoreClTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(4, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*inout_i8, *inout_u16, *inout_i32,
                                          *inout_f, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const std::span<const cl_char> e{ei8.begin(), ei8.end()};
    const zivc::MappedMemory mem = inout_i8->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }
  {
    const std::span<const cl_ushort> e{eu16.begin(), eu16.end()};
    const zivc::MappedMemory mem = inout_u16->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }
  {
    const std::span<const cl_int> e{ei32.begin(), ei32.end()};
    const zivc::MappedMemory mem = inout_i32->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(2 * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_EQ(4 * e[index], mem[index]) << "Vector load store failed.";
  }

  {
    const std::span<const float> e{ef.begin(), ef.end()};
    const zivc::MappedMemory mem = inout_f->mapMemory();
    std::size_t index = 0;
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 2; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 3; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_FLOAT_EQ(2.0f * e[index], mem[index]) << "Vector load store failed.";
    for (std::size_t i = 0; i < 4; ++i, ++index)
      ASSERT_FLOAT_EQ(4.0f * e[index], mem[index]) << "Vector load store failed.";
  }
}

TEST(ClCppTest, VectorLoadStoreHalfTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_char2;
  using zivc::cl_char3;
  using zivc::cl_char4;
  using zivc::cl_uchar2;
  using zivc::cl_uchar3;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short3;
  using zivc::cl_short4;
  using zivc::cl_ushort2;
  using zivc::cl_ushort3;
  using zivc::cl_ushort4;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_half;
  using zivc::cl_half2;
  using zivc::cl_half3;
  using zivc::cl_half4;

  constexpr auto h1 = static_cast<cl_half>(1.0f);
  constexpr auto h2 = static_cast<cl_half>(2.0f);
  constexpr auto h3 = static_cast<cl_half>(4.0f);
  constexpr auto h4 = static_cast<cl_half>(8.0f);
  const std::initializer_list<cl_half> eh = {h1, h1,
                                             h1, h2, h1, h2,
                                             h1, h2, h3, h1, h2, h3,
                                             h1, h2, h3, h4, h1, h2, h3, h4};
  const zivc::SharedBuffer inout_h = device->createBuffer<cl_half>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, eh, inout_h.get());

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLoadStoreHalfTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLoadStoreHalfTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*inout_h, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const std::span<const cl_half> e{eh.begin(), eh.end()};
    const zivc::MappedMemory mem = inout_h->mapMemory();
    std::size_t index = 0;
    {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index++]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index++]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 2; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 2; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 3; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 3; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 4; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 4; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
  }
}

TEST(ClCppTest, VectorLoadStoreHalfClTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_char2;
  using zivc::cl_char3;
  using zivc::cl_char4;
  using zivc::cl_uchar2;
  using zivc::cl_uchar3;
  using zivc::cl_uchar4;
  using zivc::cl_short2;
  using zivc::cl_short3;
  using zivc::cl_short4;
  using zivc::cl_ushort2;
  using zivc::cl_ushort3;
  using zivc::cl_ushort4;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_half;
  using zivc::cl_half2;
  using zivc::cl_half3;
  using zivc::cl_half4;

  constexpr auto h1 = static_cast<cl_half>(1.0f);
  constexpr auto h2 = static_cast<cl_half>(2.0f);
  constexpr auto h3 = static_cast<cl_half>(4.0f);
  constexpr auto h4 = static_cast<cl_half>(8.0f);
  const std::initializer_list<cl_half> eh = {h1, h1,
                                             h1, h2, h1, h2,
                                             h1, h2, h3, h1, h2, h3,
                                             h1, h2, h3, h4, h1, h2, h3, h4};
  const zivc::SharedBuffer inout_h = device->createBuffer<cl_half>(
      {zivc::BufferUsage::kPreferDevice,
       zivc::BufferFlag::kRandomAccessible});
  ztest::setDeviceBuffer(*device, eh, inout_h.get());

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLoadStoreHalfClTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLoadStoreHalfClTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*inout_h, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const std::span<const cl_half> e{eh.begin(), eh.end()};
    const zivc::MappedMemory mem = inout_h->mapMemory();
    std::size_t index = 0;
    {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index++]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index++]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 2; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 2; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 3; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 3; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 4; ++i, ++index) {
      const float expected = 2.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
    for (std::size_t i = 0; i < 4; ++i, ++index) {
      const float expected = 4.0f * static_cast<float>(e[index]);
      const float value = static_cast<float>(mem[index]);
      EXPECT_FLOAT_EQ(expected, value) << "Vector load store failed.";
    }
  }
}
