/*!
  \file math_test.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_MATH_TEST_HPP
#define ZIVC_TEST_MATH_TEST_HPP

// Standard C++ library
#include <array>
#include <algorithm>
#include <cmath>
#include <concepts>
#include <functional>
#include <fstream>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/binary_serializer.hpp"
#include "zisc/concepts.hpp"
#include "zisc/function_reference.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"
#include "test.hpp"

namespace ztest {



template <typename Type>
struct V2
{
  Type x_;
  Type y_;
};

template <typename Type>
struct V3
{
  Type x_;
  Type y_;
  Type z_;
};

template <std::floating_point Float> inline
bool isSubnormal(const Float x) noexcept
{
  constexpr auto zero = static_cast<Float>(0);
  const bool flag = std::equal_to<Float>{}(x, zero) ||
                    std::isnormal(x) ||
                    std::isinf(x) ||
                    std::isnan(x);
  return !flag;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] file_path No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadXList(const std::string_view file_path) noexcept
{
  std::ifstream reference_file{file_path.data(), std::ios_base::binary};

  zivc::int32b n = 0;
  zisc::BSerializer::read(&n, &reference_file);

  std::vector<Float> x_list{};
  x_list.resize(n);
  zisc::BSerializer::read(x_list.data(), &reference_file, sizeof(Float) * n);

  return x_list;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] file_path No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadExpectedList(const std::string_view file_path, const std::size_t n) noexcept
{
  std::ifstream reference_file{file_path.data(), std::ios_base::binary};

  std::vector<Float> expected_list{};
  expected_list.resize(n);
  zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(Float) * n);

  return expected_list;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadAllXList() noexcept
{
  auto file_path = (sizeof(Float) == 4)
      ? std::string_view{"resources/math_xallf_reference.bin"}
      : std::string_view{"resources/math_xalld_reference.bin"};
  return loadXList<Float>(file_path);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadOneXList() noexcept
{
  auto file_path = (sizeof(Float) == 4)
      ? std::string_view{"resources/math_xonef_reference.bin"}
      : std::string_view{"resources/math_xoned_reference.bin"};
  return loadXList<Float>(file_path);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadPositiveXList() noexcept
{
  auto file_path = (sizeof(Float) == 4)
      ? std::string_view{"resources/math_xpositivef_reference.bin"}
      : std::string_view{"resources/math_xpositived_reference.bin"};
  return loadXList<Float>(file_path);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadPowXList() noexcept
{
  auto file_path = (sizeof(Float) == 4)
      ? std::string_view{"resources/math_xpowf_reference.bin"}
      : std::string_view{"resources/math_xpowd_reference.bin"};
  return loadXList<Float>(file_path);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF1(const KernelInitParam kernel_params,
            const zisc::FunctionReference<std::vector<Float> ()> x_list_loader_func,
            const std::string_view reference_file,
            const std::string_view label,
            const std::string_view func_name,
            const zisc::FunctionReference<Float (Float)> x_converter = [](const Float v){return v;},
            const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector x_list = x_list_loader_func();
  std::transform(x_list.cbegin(), x_list.cend(), x_list.begin(), x_converter);
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
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

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector expected_list = ztest::loadExpectedList<Float>(reference_file, x_list.size());
    std::transform(expected_list.cbegin(), expected_list.cend(), expected_list.begin(), reference_converter);

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF1Func(const KernelInitParam kernel_params,
                const zisc::FunctionReference<std::vector<Float> ()> x_list_loader_func,
                const zisc::FunctionReference<Float (Float)> reference_func,
                const std::string_view label,
                const std::string_view func_name,
                const zisc::FunctionReference<Float (Float)> x_converter = [](const Float v){return v;},
                const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector x_list = x_list_loader_func();
  std::transform(x_list.cbegin(), x_list.cend(), x_list.begin(), x_converter);
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
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

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector<Float> expected_list{};
    expected_list.resize(x_list.size());
    std::transform(x_list.cbegin(), x_list.cend(), expected_list.begin(),
    [&reference_func, &reference_converter](const Float x) noexcept
    {
      return reference_converter(reference_func(x));
    });

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF1Out(const KernelInitParam kernel_params,
               const zisc::FunctionReference<std::vector<Float> ()> x_list_loader_func,
               const zisc::FunctionReference<Float (Float, Float*)> reference_func,
               const std::string_view label,
               const std::string_view func_name,
               const zisc::FunctionReference<Float (Float)> x_converter = [](const Float v){return v;},
               const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  std::vector x_list = x_list_loader_func();
  std::transform(x_list.cbegin(), x_list.cend(), x_list.begin(), x_converter);
  const zivc::uint32b n = x_list.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.begin(), x_list.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  buffer_out2->setSize(vector_size * n);

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
  zivc::LaunchResult result = kernel->run(*buffer_in, *buffer_out, *buffer_out2, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    const zivc::SharedBuffer tmp2 = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    ztest::copyBuffer(*buffer_out2, tmp2.get());
    const zivc::MappedMemory mem = tmp->mapMemory();
    const zivc::MappedMemory mem2 = tmp2->mapMemory();

    // 
    std::vector<Float> expected_list{};
    expected_list.resize(x_list.size());
    std::vector<Float> expected2_list{};
    expected2_list.resize(x_list.size());
    for (std::size_t i = 0; i < x_list.size(); ++i) {
      const Float x = x_list[i];
      expected_list[i] = reference_converter(reference_func(x, &expected2_list[i]));
    }

    ztest::MathTestResult result{};
    ztest::MathTestResult result2{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
      if (isSubnormal(expected2_list[i])) continue;
      ztest::test(expected2_list[i], mem2[i], &result2);
    }
    result.print();
    result.checkError(func_name);
    std::cout << std::endl;
    result2.print();
    result2.checkError(func_name);
  }
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF2(const KernelInitParam kernel_params,
            const zisc::FunctionReference<V2<std::vector<Float>> ()> x_list_loader_func,
            const std::string_view reference_file,
            const std::string_view label,
            const std::string_view func_name,
            const zisc::FunctionReference<V2<Float> (V2<Float>)> x_converter = [](const V2<Float> v){return v;},
            const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  V2<std::vector<Float>> x_list = x_list_loader_func();
  for (std::size_t i = 0; i < x_list.x_.size(); ++i) {
    V2<Float> v{x_list.x_[i], x_list.y_[i]};
    v = x_converter(v);
    x_list.x_[i] = v.x_;
    x_list.y_[i] = v.y_;
  }
  const zivc::uint32b n = x_list.x_.size() / vector_size;


  // Initialize buffers
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.x_.begin(), x_list.x_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.y_.begin(), x_list.y_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
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

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector expected_list = ztest::loadExpectedList<Float>(reference_file, x_list.size());
    std::transform(expected_list.cbegin(), expected_list.cend(), expected_list.begin(), reference_converter);

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF2Func(const KernelInitParam kernel_params,
                const zisc::FunctionReference<V2<std::vector<Float>> ()> x_list_loader_func,
                const zisc::FunctionReference<Float (Float, Float)> reference_func,
                const std::string_view label,
                const std::string_view func_name,
                const zisc::FunctionReference<V2<Float> (V2<Float>)> x_converter = [](const V2<Float> v){return v;},
                const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  V2<std::vector<Float>> x_list = x_list_loader_func();
  for (std::size_t i = 0; i < x_list.x_.size(); ++i) {
    V2<Float> v{x_list.x_[i], x_list.y_[i]};
    v = x_converter(v);
    x_list.x_[i] = v.x_;
    x_list.y_[i] = v.y_;
  }
  const zivc::uint32b n = x_list.x_.size() / vector_size;

  // Initialize buffers
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.x_.begin(), x_list.x_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.y_.begin(), x_list.y_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
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

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector<Float> expected_list{};
    expected_list.resize(x_list.x_.size());
    for (std::size_t i = 0; i < x_list.x_.size(); ++i)
      expected_list[i] = reference_converter(reference_func(x_list.x_[i], x_list.y_[i]));

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam Float No description.
  \tparam KernelInitParam No description.
  \param [in] kernel_params No description.
  \param [in] x_list_loader_func No description.
  \param [in] reference_file No description.
  \param [in] label No description.
  \param [in] func_name No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <std::size_t kN,
          std::floating_point Float,
          typename KernelInitParam> inline
void testF3Func(const KernelInitParam kernel_params,
                const zisc::FunctionReference<V3<std::vector<Float>> ()> x_list_loader_func,
                const zisc::FunctionReference<Float (Float, Float, Float)> reference_func,
                const std::string_view label,
                const std::string_view func_name,
                const zisc::FunctionReference<V3<Float> (V3<Float>)> x_converter = [](const V3<Float> v){return v;},
                const zisc::FunctionReference<Float (Float)> reference_converter = [](const Float v){return v;})
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Get the test input
  constexpr std::size_t vector_size = kN;
  V3<std::vector<Float>> x_list = x_list_loader_func();
  for (std::size_t i = 0; i < x_list.x_.size(); ++i) {
    V3<Float> v{x_list.x_[i], x_list.y_[i], x_list.z_[i]};
    v = x_converter(v);
    x_list.x_[i] = v.x_;
    x_list.y_[i] = v.y_;
    x_list.z_[i] = v.z_;
  }
  const zivc::uint32b n = x_list.x_.size() / vector_size;


  // Initialize buffers
  const zivc::SharedBuffer buffer_in1 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.x_.begin(), x_list.x_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in1.get());
  }
  const zivc::SharedBuffer buffer_in2 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.y_.begin(), x_list.y_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in2.get());
  }
  const zivc::SharedBuffer buffer_in3 = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  {
    const std::span<const Float> l{x_list.z_.begin(), x_list.z_.end()};
    ztest::setDeviceBuffer(*device, l, buffer_in3.get());
  }
  const zivc::SharedBuffer buffer_out = device->createBuffer<Float>(zivc::BufferUsage::kPreferDevice);
  buffer_out->setSize(vector_size * n);

  // Make a kernel
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(5, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{n}});
  launch_options.requestFence(true);
  launch_options.setLabel(label);
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(5, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(n, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_in1, *buffer_in2, *buffer_in3, *buffer_out, n, launch_options);
  device->waitForCompletion(result.fence());

  // output1
  {
    const zivc::SharedBuffer tmp = device->createBuffer<Float>({zivc::BufferUsage::kPreferHost, zivc::BufferFlag::kRandomAccessible});
    ztest::copyBuffer(*buffer_out, tmp.get());
    const zivc::MappedMemory mem = tmp->mapMemory();

    // 
    std::vector<Float> expected_list{};
    expected_list.resize(x_list.x_.size());
    for (std::size_t i = 0; i < x_list.x_.size(); ++i)
      expected_list[i] = reference_converter(reference_func(x_list.x_[i], x_list.y_[i], x_list.z_[i]));

    ztest::MathTestResult result{};
    for (std::size_t i = 0; i < mem.size(); ++i) {
      if (isSubnormal(expected_list[i])) continue;
      ztest::test(expected_list[i], mem[i], &result);
    }
    result.print();
    result.checkError(func_name);
  }
}

} /* namespace ztest */

#endif /* ZIVC_TEST_MATH_TEST_HPP */
