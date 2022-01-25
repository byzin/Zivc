/*!
  \file cl_cpp_test_vector.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <initializer_list>
#include <limits>
#include <type_traits>
// Zisc
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  auto buffer_invec2 = device->makeBuffer<cl_float2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float2> v = {cl_float2{1.0f, 2.0f},
                                          cl_float2{3.0f, 4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec2.get());
  }
  auto buffer_invec3 = device->makeBuffer<cl_float3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float3> v = {cl_float3{1.0f, 2.0f, 3.0f},
                                          cl_float3{4.0f, 5.0f, 6.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec3.get());
  }
  auto buffer_invec4 = device->makeBuffer<cl_float4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float4> v = {cl_float4{1.0f, 2.0f, 3.0f, 4.0f},
                                          cl_float4{5.0f, 6.0f, 7.0f, 8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec4.get());
  }
  auto buffer_out1 = device->makeBuffer<cl_float>(zivc::BufferUsage::kDeviceOnly);
  buffer_out1->setSize(18);
  {
    const cl_float v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->makeBuffer<cl_float2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(2);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_float3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(2);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_float4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(2);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComponentAccessKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(7, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorComponentAccessTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(7, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_invec2, *buffer_invec3, *buffer_invec4,
                            *buffer_out1, *buffer_out2, *buffer_out3, *buffer_out4,
                            launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    auto buffer = device->makeBuffer<cl_float>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_float2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float2 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float2 access failed.";
    ASSERT_EQ(3.0f, mem[1].x) << "Vector float2 access failed.";
    ASSERT_EQ(4.0f, mem[1].y) << "Vector float2 access failed.";
  }
  // output3
  {
    auto buffer = device->makeBuffer<cl_float3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float3 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float3 access failed.";
    ASSERT_EQ(3.0f, mem[0].z) << "Vector float3 access failed.";
    ASSERT_EQ(4.0f, mem[1].x) << "Vector float3 access failed.";
    ASSERT_EQ(5.0f, mem[1].y) << "Vector float3 access failed.";
    ASSERT_EQ(6.0f, mem[1].z) << "Vector float3 access failed.";
  }
  // output4
  {
    auto buffer = device->makeBuffer<cl_float4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  auto buffer_out2 = device->makeBuffer<cl_int2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_int3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(5);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(9);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConstructionKernel, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorComponentAccessTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_out2, *buffer_out3, *buffer_out4, launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    auto buffer = device->makeBuffer<cl_int2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
    ASSERT_EQ(0, mem[0].x) << "Vector int2 construction failed.";
    ASSERT_EQ(1, mem[0].y) << "Vector int2 construction failed.";
    ASSERT_EQ(2, mem[1].x) << "Vector int2 construction failed.";
    ASSERT_EQ(2, mem[1].y) << "Vector int2 construction failed.";
    ASSERT_EQ(3, mem[2].x) << "Vector int2 construction failed.";
    ASSERT_EQ(4, mem[2].y) << "Vector int2 construction failed.";
  }
  // output3
  {
    auto buffer = device->makeBuffer<cl_int3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_int4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  auto buffer_in2 = device->makeBuffer<cl_int2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                        cl_int2{10, -10},
                                        cl_int2{1, 5},
                                        cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->makeBuffer<cl_int3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                        cl_int3{10, -10, -4},
                                        cl_int3{1, 5, 4},
                                        cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                        cl_int4{10, -10, -4, -8},
                                        cl_int4{1, 5, 4, 12},
                                        cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  auto buffer_inf2 = device->makeBuffer<cl_float2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float2> v = {cl_float2{1.0f, -1.0f},
                                          cl_float2{10.0f, -10.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf2.get());
  }
  auto buffer_inf3 = device->makeBuffer<cl_float3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float3> v = {cl_float3{1.0f, -1.0f, 4.0f},
                                          cl_float3{10.0f, -10.0f, -4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf3.get());
  }
  auto buffer_inf4 = device->makeBuffer<cl_float4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_float4> v = {cl_float4{1.0f, -1.0f, 4.0f, 12.0f},
                                          cl_float4{10.0f, -10.0f, -4.0f, -8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->makeBuffer<cl_int2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_int3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(n + 3);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_outf2 = device->makeBuffer<cl_float2>(zivc::BufferUsage::kDeviceOnly);
  buffer_outf2->setSize(n);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf2.get());
  }
  auto buffer_outf3 = device->makeBuffer<cl_float3>(zivc::BufferUsage::kDeviceOnly);
  buffer_outf3->setSize(n);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf3.get());
  }
  auto buffer_outf4 = device->makeBuffer<cl_float4>(zivc::BufferUsage::kDeviceOnly);
  buffer_outf4->setSize(n);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorArithmeticOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(12, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                            *buffer_inf2, *buffer_inf3, *buffer_inf4,
                            *buffer_out2, *buffer_out3, *buffer_out4,
                            *buffer_outf2, *buffer_outf3, *buffer_outf4,
                            launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    auto buffer = device->makeBuffer<cl_int2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_float2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_outf2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_int3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_float3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_outf3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_int4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_float4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_outf4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  }
}

TEST(ClCppTest, VectorArithmeticAssignmentOperatorTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;

  auto buffer_in2 = device->makeBuffer<cl_int2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                        cl_int2{10, -10},
                                        cl_int2{1, 5},
                                        cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->makeBuffer<cl_int3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                        cl_int3{10, -10, -4},
                                        cl_int3{1, 5, 4},
                                        cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                        cl_int4{10, -10, -4, -8},
                                        cl_int4{1, 5, 4, 12},
                                        cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 10;
  auto buffer_out2 = device->makeBuffer<cl_int2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_int3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(n + 3);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticAssignmentOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorArithmeticAssignmentOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                            *buffer_out2, *buffer_out3, *buffer_out4,
                            launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    auto buffer = device->makeBuffer<cl_int2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_int3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_int4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  }
}

TEST(ClCppTest, VectorBitwiseOperatorTest)
{
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

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

  auto buffer_in2 = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                         cl_uint2{mask02, mask03},
                                         cl_uint2{mask04, mask05},
                                         cl_uint2{32u, 64u},
                                         cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                         cl_uint3{mask02, mask03, mask02},
                                         cl_uint3{mask04, mask05, mask04},
                                         cl_uint3{32u, 64u, 128u},
                                         cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                         cl_uint4{mask02, mask03, mask02, mask03},
                                         cl_uint4{mask04, mask05, mask04, mask05},
                                         cl_uint4{32u, 64u, 128u, 256u},
                                         cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(n);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorBitwiseOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                            *buffer_out2, *buffer_out3, *buffer_out4,
                            launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    auto buffer = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
  }
  // output3
  {
    auto buffer = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

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

  auto buffer_in2 = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                         cl_uint2{mask02, mask03},
                                         cl_uint2{mask04, mask05},
                                         cl_uint2{32u, 64u},
                                         cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                         cl_uint3{mask02, mask03, mask02},
                                         cl_uint3{mask04, mask05, mask04},
                                         cl_uint3{32u, 64u, 128u},
                                         cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                         cl_uint4{mask02, mask03, mask02, mask03},
                                         cl_uint4{mask04, mask05, mask04, mask05},
                                         cl_uint4{32u, 64u, 128u, 256u},
                                         cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kDeviceOnly);
  buffer_out2->setSize(n);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kDeviceOnly);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kDeviceOnly);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseAssignmentOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorBitwiseAssignmentOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in2, *buffer_in3, *buffer_in4,
                            *buffer_out2, *buffer_out3, *buffer_out4,
                            launch_options);
  device->waitForCompletion(result.fence());

  // output2
  {
    auto buffer = device->makeBuffer<cl_uint2>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
  }
  // output3
  {
    auto buffer = device->makeBuffer<cl_uint3>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->makeBuffer<cl_uint4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  auto buffer_inout4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int4> v = {cl_int4{0, 1, 2, 3},
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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorIncrementDecrementTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorIncrementDecrementTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_inout4, launch_options);
  device->waitForCompletion(result.fence());

  // output4
  {
    auto buffer = device->makeBuffer<cl_int4>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_inout4, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  auto buffer_inout4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, 0, 0, 0},
                                        cl_int4{1, 2, 3, 4},
                                        cl_int4{-1, -2, -3, -4},
                                        cl_int4{0, 0, 0, 0},
                                        cl_int4{0, 0, 0, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_inout4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConditionalOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorConditionalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(1, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_inout4, launch_options);
  device->waitForCompletion(result.fence());

  // output4
  {
    auto buffer = device->makeBuffer<cl_int4>(zivc::BufferUsage::kHostOnly);
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
  auto platform = ztest::makePlatform();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = platform->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;
  using zivc::cl_Boolean;

  auto buffer_in4 = device->makeBuffer<cl_int4>(zivc::BufferUsage::kDeviceOnly);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, 1, 1, 1},
                                        cl_int4{-1, 0, 1, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  auto buffer_out = device->makeBuffer<cl_Boolean>(zivc::BufferUsage::kDeviceOnly);
  buffer_out->setSize(72);
  {
    cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComparisonOperatorTest, 1);
  auto kernel = device->makeKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->makeOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({1});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorConditionalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(2, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in4, *buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->makeBuffer<cl_Boolean>(zivc::BufferUsage::kHostOnly);
    ztest::copyBuffer(*buffer_out, buffer.get());
    const auto mem = buffer->mapMemory();
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
  }
}
