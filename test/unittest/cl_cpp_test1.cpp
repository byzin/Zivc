/*!
  \file cl_cpp_test1.cpp
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
#include <limits>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"
#include "zivc/kernel_set/kernel_set-cl_cpp_test1.hpp"

namespace {

constexpr std::size_t nLimitBool() {return 6;}
constexpr std::size_t nLimitInt() {return 4;}
constexpr std::size_t nLimitType() {return 6;}

template <typename Type>
void testTypeLimit(const zivc::Buffer<zivc::cl_Boolean>& buffer_bool,
                   const zivc::Buffer<zivc::cl_int>& buffer_int,
                   const zivc::Buffer<Type>& buffer_type)
{
  using LimitT = std::numeric_limits<Type>;
  std::size_t index = 0;
  {
    index = 0;
    zivc::MappedMemory mem = buffer_bool.mapMemory();
    if (LimitT::is_specialized) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::isSpecialized() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::isSpecialized() is wrong.";
    }
    if (LimitT::is_signed) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::isSigned() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::isSigned() is wrong.";
    }
    if (LimitT::is_integer) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::isInteger() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::isInteger() is wrong.";
    }
    if (LimitT::is_exact) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::isExact() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::isExact() is wrong.";
    }
    if (LimitT::has_infinity) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::hasInfinity() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::hasInfinity() is wrong.";
    }
    if (LimitT::has_quiet_NaN) {
      ASSERT_TRUE(mem[index++]) << "NumericLimit<>::hasQuietNan() is wrong.";
    }
    else {
      ASSERT_FALSE(mem[index++]) << "NumericLimit<>::hasQuietNan() is wrong.";
    }
  }
  {
    index = 0;
    zivc::MappedMemory mem = buffer_int.mapMemory();
    ASSERT_EQ(LimitT::digits, mem[index++]) << "NumericLimit<>::digits() is wrong.";
    ASSERT_EQ(LimitT::digits10, mem[index++]) << "NumericLimit<>::digits10() is wrong.";
    ASSERT_EQ(LimitT::max_digits10, mem[index++]) << "NumericLimit<>::maxDigits10() is wrong.";
    ASSERT_EQ(LimitT::radix, mem[index++]) << "NumericLimit<>::radix() is wrong.";
  }
  {
    index = 0;
    zivc::MappedMemory mem = buffer_type.mapMemory();
    ASSERT_EQ((LimitT::min)(), mem[index++]) << "NumericLimit<>::min() is wrong.";
    ASSERT_EQ(LimitT::lowest(), mem[index++]) << "NumericLimit<>::lowest() is wrong.";
    ASSERT_EQ((LimitT::max)(), mem[index++]) << "NumericLimit<>::max() is wrong.";
    ASSERT_EQ(LimitT::epsilon(), mem[index++]) << "NumericLimit<>::epsilon() is wrong.";
    ASSERT_EQ(LimitT::infinity(), mem[index++]) << "NumericLimit<>::infinity() is wrong.";
    //ASSERT_EQ(LimitT::quiet_NaN(), mem[index++]) << "NumericLimit<>::quietNan() is wrong.";
  }
}

} /* namespace */

TEST(ClCppTest, LimitInt8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_char;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitInt8Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitInt8Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitUint8Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_uchar;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitUint8Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitUint8Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitInt16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_short;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitInt16Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitInt16Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitUint16Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_ushort;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitUint16Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitUint16Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitInt32Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_int;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitInt32Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitInt32Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitUint32Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_uint;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitUint32Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitUint32Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitInt64Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_long;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitInt64Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitInt64Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitUint64Test)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_ulong;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitUint64Test, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitUint64Test");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_float;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitFloatTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}

TEST(ClCppTest, LimitDoubleTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using ClTypeT = zivc::cl_double;
  using zivc::cl_int;
  using zivc::cl_Boolean;

  const zivc::SharedBuffer buffer_bool = device->createBuffer<cl_Boolean>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_bool->setSize(::nLimitBool());
  const zivc::SharedBuffer buffer_int = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_int->setSize(::nLimitInt());
  const zivc::SharedBuffer buffer_type = device->createBuffer<ClTypeT>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_type->setSize(::nLimitType());
  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, limitDoubleTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(3, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("limitDoubleTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(3, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_bool, *buffer_int, *buffer_type, launch_options);
  device->waitForCompletion(result.fence());

  // output
  ::testTypeLimit<ClTypeT>(*buffer_bool, *buffer_int, *buffer_type);
}
