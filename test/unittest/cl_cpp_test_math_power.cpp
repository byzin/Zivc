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
void testExp(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPowXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::exp(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_expf_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testExp2(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPowXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::exp2(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_exp2f_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testExpm1(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPowXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::expm1(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_expm1f_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

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

    // tmp
    std::vector<cl_float> results{};
    results.reserve(x_list.size());
    for (const cl_float expt : x_list)
      results.push_back(std::pow(base, expt));
    {
      const std::span<const cl_float> l{results.begin(), results.end()};
      ztest::setDeviceBuffer(*device, l, buffer_out.get());
    }

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

    // tmp
    std::vector<cl_float> results{};
    results.reserve(x_list.size());
    for (const cl_int expt : x_list)
      results.push_back(std::pow(base, expt));
    {
      const std::span<const cl_float> l{results.begin(), results.end()};
      ztest::setDeviceBuffer(*device, l, buffer_out.get());
    }

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
void testLog(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::log(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_logf_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testLog2(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::log2(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_log2f_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testLog10(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::log10(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_log10f_reference.bin", x_list.size());

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i)
      ztest::test(expected_list[i], mem[i], &result);
    result.print();
    result.checkError(func_name);
  }
}

template <std::size_t kN, typename KernelInitParam>
void testLog1p(const KernelInitParam& kernel_params,
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

  // Get the test input
  constexpr std::size_t vector_size = kN;
  const std::vector x_list = ztest::loadPositiveXList<cl_float>();
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
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // tmp
  std::vector<cl_float> results{};
  results.reserve(x_list.size());
  for (const cl_float x : x_list)
    results.push_back(std::log1p(x));
  {
    const std::span<const cl_float> l{results.begin(), results.end()};
    ztest::setDeviceBuffer(*device, l, buffer_out.get());
  }

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    const std::vector expected_list = ztest::loadExpectedList<cl_float>("resources/math_log1pf_reference.bin", x_list.size());

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
  ::testExp<1>(kernel_params, "expV1TestKernel", "expV1");
}

TEST(ClCppTest, MathImplExp2V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, exp2V1TestKernel, 1);
  ::testExp2<1>(kernel_params, "exp2V1TestKernel", "exp2V1");
}

TEST(ClCppTest, MathImplExpm1V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, expm1V1TestKernel, 1);
  ::testExpm1<1>(kernel_params, "expm1V1TestKernel", "expm1V1");
}

TEST(ClCppTest, MathImplLogV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, logV1TestKernel, 1);
  ::testLog<1>(kernel_params, "logV1TestKernel", "logV1");
}

TEST(ClCppTest, MathImplLog2V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log2V1TestKernel, 1);
  ::testLog2<1>(kernel_params, "log2V1TestKernel", "log2V1");
}

TEST(ClCppTest, MathImplLog10V1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log10V1TestKernel, 1);
  ::testLog10<1>(kernel_params, "log10V1TestKernel", "log10V1");
}

TEST(ClCppTest, MathImplLog1pV1Test)
{
  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, log1pV1TestKernel, 1);
  ::testLog1p<1>(kernel_params, "log1pV1TestKernel", "log1pV1");
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

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplSqrtV8Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV8TestKernel, 1);
//  ztest::testF1<8, float>(kernel_params,
//                          ztest::loadPositiveXList<float>,
//                          "resources/math_sqrtf_reference.bin",
//                          "sqrtV8TestKernel",
//                          "sqrtV8");
//}

TEST(ClCppTest, MathPrecisionSqrtV8Test)
{
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV8PrecisionTestKernel, 1);
  ztest::testF1<8, float>(kernel_params,
                          ztest::loadPositiveXList<float>,
                          "resources/math_sqrtf_reference.bin",
                          "sqrtV8PrecisionTestKernel",
                          "sqrtV8Precision");
}

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplSqrtV16Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, sqrtV16TestKernel, 1);
//  ztest::testF1<16, float>(kernel_params,
//                           ztest::loadPositiveXList<float>,
//                           "resources/math_sqrtf_reference.bin",
//                           "sqrtV16TestKernel",
//                           "sqrtV16");
//}

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

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplRsqrtV8Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV8TestKernel, 1);
//  ztest::testF1<8, float>(kernel_params,
//                          ztest::loadPositiveXList<float>,
//                          "resources/math_sqrtf_reference.bin",
//                          "rsqrtV8TestKernel",
//                          "rsqrtV8",
//                          [](const float v){return v;},
//                          [](const float v){return 1.0f / v;});
//}

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

//! \todo Resolve the compile error using clspv
//TEST(ClCppTest, MathImplRsqrtV16Test)
//{
//  // Make a kernel
//  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test_math_power, rsqrtV16TestKernel, 1);
//  ztest::testF1<16, float>(kernel_params,
//                           ztest::loadPositiveXList<float>,
//                           "resources/math_sqrtf_reference.bin",
//                           "rsqrtV16TestKernel",
//                           "rsqrtV16",
//                          [](const float v){return v;},
//                          [](const float v){return 1.0f / v;});
//}

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
