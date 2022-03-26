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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  auto buffer_invec2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float2> v = {cl_float2{1.0f, 2.0f},
                                          cl_float2{3.0f, 4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec2.get());
  }
  auto buffer_invec3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float3> v = {cl_float3{1.0f, 2.0f, 3.0f},
                                          cl_float3{4.0f, 5.0f, 6.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec3.get());
  }
  auto buffer_invec4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float4> v = {cl_float4{1.0f, 2.0f, 3.0f, 4.0f},
                                          cl_float4{5.0f, 6.0f, 7.0f, 8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_invec4.get());
  }
  auto buffer_out1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(18);
  {
    const cl_float v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(2);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(2);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(2);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComponentAccessKernel, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(7, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
    ASSERT_EQ(1.0f, mem[0].x) << "Vector float2 access failed.";
    ASSERT_EQ(2.0f, mem[0].y) << "Vector float2 access failed.";
    ASSERT_EQ(3.0f, mem[1].x) << "Vector float2 access failed.";
    ASSERT_EQ(4.0f, mem[1].y) << "Vector float2 access failed.";
  }
  // output3
  {
    auto buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  auto buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(5);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(9);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConstructionKernel, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;

  auto buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                        cl_int2{10, -10},
                                        cl_int2{1, 5},
                                        cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                        cl_int3{10, -10, -4},
                                        cl_int3{1, 5, 4},
                                        cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                        cl_int4{10, -10, -4, -8},
                                        cl_int4{1, 5, 4, 12},
                                        cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  auto buffer_inf2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float2> v = {cl_float2{1.0f, -1.0f},
                                          cl_float2{10.0f, -10.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf2.get());
  }
  auto buffer_inf3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float3> v = {cl_float3{1.0f, -1.0f, 4.0f},
                                          cl_float3{10.0f, -10.0f, -4.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf3.get());
  }
  auto buffer_inf4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float4> v = {cl_float4{1.0f, -1.0f, 4.0f, 12.0f},
                                          cl_float4{10.0f, -10.0f, -4.0f, -8.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_inf4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n + 3);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_outf2 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_outf2->setSize(n);
  {
    const cl_float2 v{0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf2.get());
  }
  auto buffer_outf3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_outf3->setSize(n);
  {
    const cl_float3 v{0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf3.get());
  }
  auto buffer_outf4 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_outf4->setSize(n + 6);
  {
    const cl_float4 v{0.0f, 0.0f, 0.0f, 0.0f};
    ztest::fillDeviceBuffer(v, buffer_outf4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(12, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;

  auto buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                        cl_int2{10, -10},
                                        cl_int2{1, 5},
                                        cl_int2{10, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int3> v = {cl_int3{1, -1, 4},
                                        cl_int3{10, -10, -4},
                                        cl_int3{1, 5, 4},
                                        cl_int3{10, 2, 7}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, -1, 4, 12},
                                        cl_int4{10, -10, -4, -8},
                                        cl_int4{1, 5, 4, 12},
                                        cl_int4{10, 2, 7, 8}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 10;
  auto buffer_out2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 3);
  {
    const cl_int2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n + 3);
  {
    const cl_int3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n + 3 + 4);
  {
    const cl_int4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorArithmeticAssignmentOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

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

  auto buffer_in2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                         cl_uint2{mask02, mask03},
                                         cl_uint2{mask04, mask05},
                                         cl_uint2{32u, 64u},
                                         cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                         cl_uint3{mask02, mask03, mask02},
                                         cl_uint3{mask04, mask05, mask04},
                                         cl_uint3{32u, 64u, 128u},
                                         cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                         cl_uint4{mask02, mask03, mask02, mask03},
                                         cl_uint4{mask04, mask05, mask04, mask05},
                                         cl_uint4{32u, 64u, 128u, 256u},
                                         cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 6);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

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

  auto buffer_in2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint2> v = {cl_uint2{mask01, mask01},
                                         cl_uint2{mask02, mask03},
                                         cl_uint2{mask04, mask05},
                                         cl_uint2{32u, 64u},
                                         cl_uint2{2u, 4u}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint3> v = {cl_uint3{mask01, mask01, mask01},
                                         cl_uint3{mask02, mask03, mask02},
                                         cl_uint3{mask04, mask05, mask04},
                                         cl_uint3{32u, 64u, 128u},
                                         cl_uint3{2u, 4u, 6u}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }
  auto buffer_in4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_uint4> v = {cl_uint4{mask01, mask01, mask01, mask01},
                                         cl_uint4{mask02, mask03, mask02, mask03},
                                         cl_uint4{mask04, mask05, mask04, mask05},
                                         cl_uint4{32u, 64u, 128u, 256u},
                                         cl_uint4{2u, 4u, 6u, 8u}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  const std::size_t n = 14;
  auto buffer_out2 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n + 5);
  {
    const cl_uint2 v{0, 0};
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_uint3 v{0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_uint4 v{0, 0, 0, 0};
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorBitwiseAssignmentOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
    auto buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  auto buffer_inout4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
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
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorIncrementDecrementTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;

  auto buffer_inout4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
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
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorConditionalOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(1, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;
  using zivc::cl_Boolean;

  auto buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, 1, 1, 1},
                                        cl_int4{-1, 0, 1, 2}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  auto buffer_out = device->createBuffer<cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(72 + 24);
  {
    cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComparisonOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
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
    auto buffer = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
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

TEST(ClCppTest, VectorLogicalOperatorTest)
{
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int4;
  using zivc::cl_Boolean;

  auto buffer_in4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_int4> v = {cl_int4{1, 1, 1, 1},
                                        cl_int4{1, 0, 1, 0},
                                        cl_int4{0, 0, 1, 0}};
    ztest::setDeviceBuffer(*device, v, buffer_in4.get());
  }
  auto buffer_out = device->createBuffer<cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(28 + 12);
  {
    cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorLogicalOperatorTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(2, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("VectorLogicalOperatorTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(2, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in4, *buffer_out, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  auto buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  auto buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }

  const auto int_n = zisc::cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = zisc::cast<zivc::uint32b>(buffer_in1->size());
  const std::size_t n = int_n + float_n;

  auto buffer_out0 = device->createBuffer<cl_char>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  auto buffer_out1 = device->createBuffer<cl_uchar>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->createBuffer<cl_short>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_ushort>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_out5 = device->createBuffer<cl_uint>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  auto buffer_out6 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, scalarCastTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(11, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("ScalarCastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(11, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in0, *buffer_in1,
                            *buffer_out0, *buffer_out1, *buffer_out2,
                            *buffer_out3, *buffer_out4, *buffer_out5,
                            *buffer_out6,
                            int_n, float_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<cl_char>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uchar>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_short>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_ushort>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferHost,
                                                zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_char;
  using zivc::cl_uchar;
  using zivc::cl_short;
  using zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_uint;
  using zivc::cl_float;

  auto buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int> v = {0, 1, -1, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  auto buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }

  const auto int_n = zisc::cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = zisc::cast<zivc::uint32b>(buffer_in1->size());
  const std::size_t n = int_n + float_n;

  auto buffer_out0 = device->createBuffer<zivc::cl_Boolean>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    zivc::cl_Boolean v;
    v.set(false);
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, scalarBoolCastTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("ScalarBoolCastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in0, *buffer_in1, *buffer_out0,
                            int_n, float_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<zivc::cl_Boolean>({zivc::BufferUsage::kPreferHost,
                                                          zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

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

  auto buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  auto buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  auto buffer_in2 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int2> v = {cl_int2{0, 100},
                                        cl_int2{vmax, vmin}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float2> v = {cl_float2{0.0f, 100.0f},
                                          cl_float2{-100.0f, 3.14f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = zisc::cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = zisc::cast<zivc::uint32b>(buffer_in1->size());
  const auto int2_n = zisc::cast<zivc::uint32b>(buffer_in2->size());
  const auto float2_n = zisc::cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int2_n + float2_n;

  auto buffer_out0 = device->createBuffer<cl_char2>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  auto buffer_out1 = device->createBuffer<cl_uchar2>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->createBuffer<cl_short2>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_ushort2>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int2>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_out5 = device->createBuffer<cl_uint2>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint2 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  auto buffer_out6 = device->createBuffer<cl_float2>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float2 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector2CastTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector2CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
                            *buffer_out0, *buffer_out1, *buffer_out2,
                            *buffer_out3, *buffer_out4, *buffer_out5,
                            *buffer_out6, 
                            int_n, float_n, int2_n, float2_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<cl_char2>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uchar2>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_short2>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_ushort2>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

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

  auto buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  auto buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  auto buffer_in2 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    std::initializer_list<cl_int3> v = {cl_int3{0, 100, vmax}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float3> v = {cl_float3{0.0f, 100.0f, -100.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = zisc::cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = zisc::cast<zivc::uint32b>(buffer_in1->size());
  const auto int3_n = zisc::cast<zivc::uint32b>(buffer_in2->size());
  const auto float3_n = zisc::cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int3_n + float3_n;

  auto buffer_out0 = device->createBuffer<cl_char3>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  auto buffer_out1 = device->createBuffer<cl_uchar3>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->createBuffer<cl_short3>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_ushort3>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int3>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_out5 = device->createBuffer<cl_uint3>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint3 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  auto buffer_out6 = device->createBuffer<cl_float3>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float3 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector3CastTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector3CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
                            *buffer_out0, *buffer_out1, *buffer_out2,
                            *buffer_out3, *buffer_out4, *buffer_out5,
                            *buffer_out6, 
                            int_n, float_n, int3_n, float3_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<cl_char3>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uchar3>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_short3>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_ushort3>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const auto mem = buffer->mapMemory();
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
  auto create = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  zivc::SharedDevice device = create->queryDevice(config.deviceId());

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

  auto buffer_in0 = device->createBuffer<cl_int>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int> v = {0, 100, -100, vmax, vmin};
    ztest::setDeviceBuffer(*device, v, buffer_in0.get());
  }
  auto buffer_in1 = device->createBuffer<cl_float>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float> v = {0.0f, 100.0f, -100.0f, 3.14f};
    ztest::setDeviceBuffer(*device, v, buffer_in1.get());
  }
  auto buffer_in2 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  {
    constexpr cl_int vmax = (std::numeric_limits<cl_int>::max)();
    constexpr cl_int vmin = (std::numeric_limits<cl_int>::min)();
    std::initializer_list<cl_int4> v = {cl_int4{0, 100, vmax, vmin}};
    ztest::setDeviceBuffer(*device, v, buffer_in2.get());
  }
  auto buffer_in3 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  {
    std::initializer_list<cl_float4> v = {cl_float4{0.0f, 100.0f, -100.0f, 3.14f}};
    ztest::setDeviceBuffer(*device, v, buffer_in3.get());
  }

  const auto int_n = zisc::cast<zivc::uint32b>(buffer_in0->size());
  const auto float_n = zisc::cast<zivc::uint32b>(buffer_in1->size());
  const auto int4_n = zisc::cast<zivc::uint32b>(buffer_in2->size());
  const auto float4_n = zisc::cast<zivc::uint32b>(buffer_in3->size());
  const std::size_t n = int_n + float_n + int4_n + float4_n;

  auto buffer_out0 = device->createBuffer<cl_char4>(zivc::BufferUsage::kPreferDevice);
  buffer_out0->setSize(n);
  {
    const cl_char4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out0.get());
  }
  auto buffer_out1 = device->createBuffer<cl_uchar4>(zivc::BufferUsage::kPreferDevice);
  buffer_out1->setSize(n);
  {
    const cl_uchar4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out1.get());
  }
  auto buffer_out2 = device->createBuffer<cl_short4>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(n);
  {
    const cl_short4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out2.get());
  }
  auto buffer_out3 = device->createBuffer<cl_ushort4>(zivc::BufferUsage::kPreferDevice);
  buffer_out3->setSize(n);
  {
    const cl_ushort4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out3.get());
  }
  auto buffer_out4 = device->createBuffer<cl_int4>(zivc::BufferUsage::kPreferDevice);
  buffer_out4->setSize(n);
  {
    const cl_int4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out4.get());
  }
  auto buffer_out5 = device->createBuffer<cl_uint4>(zivc::BufferUsage::kPreferDevice);
  buffer_out5->setSize(n);
  {
    const cl_uint4 v = 0;
    ztest::fillDeviceBuffer(v, buffer_out5.get());
  }
  auto buffer_out6 = device->createBuffer<cl_float4>(zivc::BufferUsage::kPreferDevice);
  buffer_out6->setSize(n);
  {
    const cl_float4 v = 0.0f;
    ztest::fillDeviceBuffer(v, buffer_out6.get());
  }

  device->waitForCompletion();

  // Make a kernel
  auto kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vector4CastTest, 1);
  auto kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(15, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  auto launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("Vector4CastTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(15, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  auto result = kernel->run(*buffer_in0, *buffer_in1, *buffer_in2, *buffer_in3,
                            *buffer_out0, *buffer_out1, *buffer_out2,
                            *buffer_out3, *buffer_out4, *buffer_out5,
                            *buffer_out6, 
                            int_n, float_n, int4_n, float4_n, launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    auto buffer = device->createBuffer<cl_char4>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out0, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uchar4>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out1, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_short4>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out2, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_ushort4>({zivc::BufferUsage::kPreferHost,
                                                    zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out3, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out4, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferHost,
                                                  zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out5, buffer.get());
    const auto mem = buffer->mapMemory();
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
    auto buffer = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferHost,
                                                   zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out6, buffer.get());
    const auto mem = buffer->mapMemory();
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
