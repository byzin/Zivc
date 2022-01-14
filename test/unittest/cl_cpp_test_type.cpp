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
  device->setFenceSize(1);

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
  auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(cl_cpp_test_type, vectorComponentAccessTest, 1);
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

