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
#include <initializer_list>
#include <limits>
#include <span>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
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

//! \attention macOS doesn't support double
#if !defined(Z_MAC)

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

#endif // Z_MAC

TEST(ClCppTest, UtilityClampIntTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int> v = {1, -1, 20};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                              cl_int2{20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int3> v = {cl_int3{1, -1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_int8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int8> v = {cl_int8{1, -1, 20, -20, 1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_int16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int16> v = {cl_int16{1, -1, 20, -20, 1, -1, 20, -20,
                                                        1, -1, 20, -20, 1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityClampIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityClampIntTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(1, mem[0]) << "The clamp for int failed.";
    EXPECT_EQ(0, mem[1]) << "The clamp for int failed.";
    EXPECT_EQ(10, mem[2]) << "The clamp for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int2 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clamp for int2 failed.";
    EXPECT_EQ(10, mem[1].x) << "The clamp for int2 failed.";
    EXPECT_EQ(0, mem[1].y) << "The clamp for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int3 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clamp for int3 failed.";
    EXPECT_EQ(10, mem[0].z) << "The clamp for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int4 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clamp for int4 failed.";
    EXPECT_EQ(10, mem[0].z) << "The clamp for int4 failed.";
    EXPECT_EQ(0, mem[0].w) << "The clamp for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The clamp for int8 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s2) << "The clamp for int8 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The clamp for int8 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The clamp for int8 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s6) << "The clamp for int8 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The clamp for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s2) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s6) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s8) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].s9) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].sa) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].sb) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].sc) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].sd) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].se) << "The clamp for int16 failed.";
    EXPECT_EQ(0, mem[0].sf) << "The clamp for int16 failed.";
  }
}

TEST(ClCppTest, UtilityMaxMinIntTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int> v = {1, -1, 20};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int2> v = {cl_int2{1, -1},
                                              cl_int2{20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int3> v = {cl_int3{1, -1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int4> v = {cl_int4{1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_int8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int8> v = {cl_int8{1, -1, 20, -20, 1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_int16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int16> v = {cl_int16{1, -1, 20, -20, 1, -1, 20, -20,
                                                        1, -1, 20, -20, 1, -1, 20, -20}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityMaxMinIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityMaxMinIntTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(1, mem[0]) << "The maxmin for int failed.";
    EXPECT_EQ(0, mem[1]) << "The maxmin for int failed.";
    EXPECT_EQ(10, mem[2]) << "The maxmin for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int2 failed.";
    EXPECT_EQ(0, mem[0].y) << "The maxmin for int2 failed.";
    EXPECT_EQ(10, mem[1].x) << "The maxmin for int2 failed.";
    EXPECT_EQ(0, mem[1].y) << "The maxmin for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int3 failed.";
    EXPECT_EQ(0, mem[0].y) << "The maxmin for int3 failed.";
    EXPECT_EQ(10, mem[0].z) << "The maxmin for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int4 failed.";
    EXPECT_EQ(0, mem[0].y) << "The maxmin for int4 failed.";
    EXPECT_EQ(10, mem[0].z) << "The maxmin for int4 failed.";
    EXPECT_EQ(0, mem[0].w) << "The maxmin for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The maxmin for int8 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s2) << "The maxmin for int8 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The maxmin for int8 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The maxmin for int8 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s6) << "The maxmin for int8 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The maxmin for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s2) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s6) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s8) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].s9) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].sa) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].sb) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].sc) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].sd) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].se) << "The maxmin for int16 failed.";
    EXPECT_EQ(0, mem[0].sf) << "The maxmin for int16 failed.";
  }
}

TEST(ClCppTest, UtilityUpsampleIntTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_int>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_int> v = {0b0101'1010'0101'1010};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_int2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(1);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_int3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(1);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_int4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(1);
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_int8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(1);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_int16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(1);

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityUpsampleIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityUpsampleIntTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  constexpr cl_int hi = 0b0101'1010'0101'1010;
  constexpr cl_int lo = 0b0000'1111'0000'1111;
  constexpr cl_int expected = (hi << 16) | lo;
  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(expected, mem[0]) << "The upsample for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int2 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int3 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int3 failed.";
    EXPECT_EQ(expected, mem[0].z) << "The upsample for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].z) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].w) << "The upsample for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(expected, mem[0].s0) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s1) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s2) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s3) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s4) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s5) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s6) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s7) << "The upsample for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(expected, mem[0].s0) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s1) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s2) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s3) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s4) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s5) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s6) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s7) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s8) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s9) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sa) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sb) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sc) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sd) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].se) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sf) << "The upsample for int16 failed.";
  }
}

TEST(ClCppTest, UtilityClampUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {1, 20};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint2> v = {cl_uint2{1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint3> v = {cl_uint3{1, 20, 1}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint4> v = {cl_uint4{1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint8> v = {cl_uint8{1, 20, 1, 20, 1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint16> v = {cl_uint16{1, 20, 1, 20, 1, 20, 1, 20,
                                                          1, 20, 1, 20, 1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityClampUintTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityClampUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(1, mem[0]) << "The clamp for int failed.";
    EXPECT_EQ(10, mem[1]) << "The clamp for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int2 failed.";
    EXPECT_EQ(10, mem[0].y) << "The clamp for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int3 failed.";
    EXPECT_EQ(10, mem[0].y) << "The clamp for int3 failed.";
    EXPECT_EQ(1, mem[0].z) << "The clamp for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The clamp for int4 failed.";
    EXPECT_EQ(10, mem[0].y) << "The clamp for int4 failed.";
    EXPECT_EQ(1, mem[0].z) << "The clamp for int4 failed.";
    EXPECT_EQ(10, mem[0].w) << "The clamp for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s1) << "The clamp for int8 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s3) << "The clamp for int8 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s5) << "The clamp for int8 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The clamp for int8 failed.";
    EXPECT_EQ(10, mem[0].s7) << "The clamp for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s1) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s3) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s5) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s7) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].s8) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].s9) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].sa) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].sb) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].sc) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].sd) << "The clamp for int16 failed.";
    EXPECT_EQ(1, mem[0].se) << "The clamp for int16 failed.";
    EXPECT_EQ(10, mem[0].sf) << "The clamp for int16 failed.";
  }
}

TEST(ClCppTest, UtilityMaxMinUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {1, 20};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint2> v = {cl_uint2{1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint3> v = {cl_uint3{1, 20, 1}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint4> v = {cl_uint4{1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint8> v = {cl_uint8{1, 20, 1, 20, 1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint16> v = {cl_uint16{1, 20, 1, 20, 1, 20, 1, 20,
                                                          1, 20, 1, 20, 1, 20, 1, 20}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityMaxMinUintTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityMaxMinUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(1, mem[0]) << "The maxmin for int failed.";
    EXPECT_EQ(10, mem[1]) << "The maxmin for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int2 failed.";
    EXPECT_EQ(10, mem[0].y) << "The maxmin for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int3 failed.";
    EXPECT_EQ(10, mem[0].y) << "The maxmin for int3 failed.";
    EXPECT_EQ(1, mem[0].z) << "The maxmin for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The maxmin for int4 failed.";
    EXPECT_EQ(10, mem[0].y) << "The maxmin for int4 failed.";
    EXPECT_EQ(1, mem[0].z) << "The maxmin for int4 failed.";
    EXPECT_EQ(10, mem[0].w) << "The maxmin for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s1) << "The maxmin for int8 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s3) << "The maxmin for int8 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s5) << "The maxmin for int8 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The maxmin for int8 failed.";
    EXPECT_EQ(10, mem[0].s7) << "The maxmin for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s1) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s3) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s5) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s7) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].s8) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].s9) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].sa) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].sb) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].sc) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].sd) << "The maxmin for int16 failed.";
    EXPECT_EQ(1, mem[0].se) << "The maxmin for int16 failed.";
    EXPECT_EQ(10, mem[0].sf) << "The maxmin for int16 failed.";
  }
}

TEST(ClCppTest, UtilityAbsUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(2);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(1);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(1);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(1);
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(1);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(1);

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityAbsUintTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityAbsUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(1, mem[0]) << "The abs for int failed.";
    EXPECT_EQ(1, mem[1]) << "The abs for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The abs for int2 failed.";
    EXPECT_EQ(1, mem[0].y) << "The abs for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The abs for int3 failed.";
    EXPECT_EQ(1, mem[0].y) << "The abs for int3 failed.";
    EXPECT_EQ(20, mem[0].z) << "The abs for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(1, mem[0].x) << "The abs for int4 failed.";
    EXPECT_EQ(1, mem[0].y) << "The abs for int4 failed.";
    EXPECT_EQ(20, mem[0].z) << "The abs for int4 failed.";
    EXPECT_EQ(20, mem[0].w) << "The abs for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The abs for int8 failed.";
    EXPECT_EQ(1, mem[0].s1) << "The abs for int8 failed.";
    EXPECT_EQ(20, mem[0].s2) << "The abs for int8 failed.";
    EXPECT_EQ(20, mem[0].s3) << "The abs for int8 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The abs for int8 failed.";
    EXPECT_EQ(1, mem[0].s5) << "The abs for int8 failed.";
    EXPECT_EQ(20, mem[0].s6) << "The abs for int8 failed.";
    EXPECT_EQ(20, mem[0].s7) << "The abs for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(1, mem[0].s0) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].s1) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].s2) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].s3) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].s4) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].s5) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].s6) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].s7) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].s8) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].s9) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].sa) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].sb) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].sc) << "The abs for int16 failed.";
    EXPECT_EQ(1, mem[0].sd) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].se) << "The abs for int16 failed.";
    EXPECT_EQ(20, mem[0].sf) << "The abs for int16 failed.";
  }
}

TEST(ClCppTest, UtilityClzUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {0b00001111'00001111'00001111'00001111u,
                                              0b11110000'11110000'11110000'11110000u};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint2> v = {
        cl_uint2{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint3> v = {
        cl_uint3{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint4> v = {
        cl_uint4{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint8> v = {
        cl_uint8{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u,
                 0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint16> v = {
        cl_uint16{0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityClzUIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityClzUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(4, mem[0]) << "The clz for int failed.";
    EXPECT_EQ(0, mem[1]) << "The clz for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(4, mem[0].x) << "The clz for int2 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clz for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(4, mem[0].x) << "The clz for int3 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clz for int3 failed.";
    EXPECT_EQ(1, mem[0].z) << "The clz for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(4, mem[0].x) << "The clz for int4 failed.";
    EXPECT_EQ(0, mem[0].y) << "The clz for int4 failed.";
    EXPECT_EQ(1, mem[0].z) << "The clz for int4 failed.";
    EXPECT_EQ(0, mem[0].w) << "The clz for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(4, mem[0].s0) << "The clz for int8 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The clz for int8 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The clz for int8 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The clz for int8 failed.";
    EXPECT_EQ(4, mem[0].s4) << "The clz for int8 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The clz for int8 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The clz for int8 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The clz for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(4, mem[0].s0) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].s1) << "The clz for int16 failed.";
    EXPECT_EQ(1, mem[0].s2) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].s3) << "The clz for int16 failed.";
    EXPECT_EQ(4, mem[0].s4) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].s5) << "The clz for int16 failed.";
    EXPECT_EQ(1, mem[0].s6) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].s7) << "The clz for int16 failed.";
    EXPECT_EQ(4, mem[0].s8) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].s9) << "The clz for int16 failed.";
    EXPECT_EQ(1, mem[0].sa) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].sb) << "The clz for int16 failed.";
    EXPECT_EQ(4, mem[0].sc) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].sd) << "The clz for int16 failed.";
    EXPECT_EQ(1, mem[0].se) << "The clz for int16 failed.";
    EXPECT_EQ(0, mem[0].sf) << "The clz for int16 failed.";
  }
}

TEST(ClCppTest, UtilityPopcountUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {0b00001111'00001111'00001111'00001111u,
                                              0b11110000'11110000'11110000'11110000u};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint2> v = {
        cl_uint2{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint3> v = {
        cl_uint3{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint4> v = {
        cl_uint4{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint8> v = {
        cl_uint8{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u,
                 0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint16> v = {
        cl_uint16{0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityPopcountUIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityPopcountUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(16, mem[0]) << "The popcount for int failed.";
    EXPECT_EQ(16, mem[1]) << "The popcount for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(16, mem[0].x) << "The popcount for int2 failed.";
    EXPECT_EQ(16, mem[0].y) << "The popcount for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(16, mem[0].x) << "The popcount for int3 failed.";
    EXPECT_EQ(16, mem[0].y) << "The popcount for int3 failed.";
    EXPECT_EQ(16, mem[0].z) << "The popcount for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(16, mem[0].x) << "The popcount for int4 failed.";
    EXPECT_EQ(16, mem[0].y) << "The popcount for int4 failed.";
    EXPECT_EQ(16, mem[0].z) << "The popcount for int4 failed.";
    EXPECT_EQ(16, mem[0].w) << "The popcount for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(16, mem[0].s0) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s1) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s2) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s3) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s4) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s5) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s6) << "The popcount for int8 failed.";
    EXPECT_EQ(16, mem[0].s7) << "The popcount for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(16, mem[0].s0) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s1) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s2) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s3) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s4) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s5) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s6) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s7) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s8) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].s9) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].sa) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].sb) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].sc) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].sd) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].se) << "The popcount for int16 failed.";
    EXPECT_EQ(16, mem[0].sf) << "The popcount for int16 failed.";
  }
}

TEST(ClCppTest, UtilityRotateUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {0b00001111'00001111'00001111'00001111u,
                                              0b11110000'11110000'11110000'11110000u};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint2> v = {
        cl_uint2{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint3> v = {
        cl_uint3{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint4> v = {
        cl_uint4{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint8> v = {
        cl_uint8{0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u,
                 0b00001111'00001111'00001111'00001111u,
                 0b11110000'11110000'11110000'11110000u,
                 0b01010101'01010101'01010101'01010101u,
                 0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint16> v = {
        cl_uint16{0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u,
                  0b00001111'00001111'00001111'00001111u,
                  0b11110000'11110000'11110000'11110000u,
                  0b01010101'01010101'01010101'01010101u,
                  0b10101010'10101010'10101010'10101010u}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityRotateUIntTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityRotateUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  constexpr cl_uint e1 = 0b11110000'11110000'11110000'11110000u;
  constexpr cl_uint e2 = 0b000111100'00'111100'00111100'00111100u;
  constexpr cl_uint e3 = 0b01010101'01010101'01010101'01010101u;
  constexpr cl_uint e4 = 0b01010101'01010101'01010101'01010101u;

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(e1, mem[0]) << "The rotate for int failed.";
    EXPECT_EQ(e2, mem[1]) << "The rotate for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(e1, mem[0].x) << "The rotate for int2 failed.";
    EXPECT_EQ(e2, mem[0].y) << "The rotate for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(e1, mem[0].x) << "The rotate for int3 failed.";
    EXPECT_EQ(e2, mem[0].y) << "The rotate for int3 failed.";
    EXPECT_EQ(e3, mem[0].z) << "The rotate for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(e1, mem[0].x) << "The rotate for int4 failed.";
    EXPECT_EQ(e2, mem[0].y) << "The rotate for int4 failed.";
    EXPECT_EQ(e3, mem[0].z) << "The rotate for int4 failed.";
    EXPECT_EQ(e4, mem[0].w) << "The rotate for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(e1, mem[0].s0) << "The rotate for int8 failed.";
    EXPECT_EQ(e2, mem[0].s1) << "The rotate for int8 failed.";
    EXPECT_EQ(e3, mem[0].s2) << "The rotate for int8 failed.";
    EXPECT_EQ(e4, mem[0].s3) << "The rotate for int8 failed.";
    EXPECT_EQ(e1, mem[0].s4) << "The rotate for int8 failed.";
    EXPECT_EQ(e2, mem[0].s5) << "The rotate for int8 failed.";
    EXPECT_EQ(e3, mem[0].s6) << "The rotate for int8 failed.";
    EXPECT_EQ(e4, mem[0].s7) << "The rotate for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(e1, mem[0].s0) << "The rotate for int16 failed.";
    EXPECT_EQ(e2, mem[0].s1) << "The rotate for int16 failed.";
    EXPECT_EQ(e3, mem[0].s2) << "The rotate for int16 failed.";
    EXPECT_EQ(e4, mem[0].s3) << "The rotate for int16 failed.";
    EXPECT_EQ(e1, mem[0].s4) << "The rotate for int16 failed.";
    EXPECT_EQ(e2, mem[0].s5) << "The rotate for int16 failed.";
    EXPECT_EQ(e3, mem[0].s6) << "The rotate for int16 failed.";
    EXPECT_EQ(e4, mem[0].s7) << "The rotate for int16 failed.";
    EXPECT_EQ(e1, mem[0].s8) << "The rotate for int16 failed.";
    EXPECT_EQ(e2, mem[0].s9) << "The rotate for int16 failed.";
    EXPECT_EQ(e3, mem[0].sa) << "The rotate for int16 failed.";
    EXPECT_EQ(e4, mem[0].sb) << "The rotate for int16 failed.";
    EXPECT_EQ(e1, mem[0].sc) << "The rotate for int16 failed.";
    EXPECT_EQ(e2, mem[0].sd) << "The rotate for int16 failed.";
    EXPECT_EQ(e3, mem[0].se) << "The rotate for int16 failed.";
    EXPECT_EQ(e4, mem[0].sf) << "The rotate for int16 failed.";
  }
}

TEST(ClCppTest, UtilityUpsampleUintTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_uint>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_uint> v = {0b1010'0101'1010'0101u};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_uint2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(1);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_uint3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(1);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_uint4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(1);
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_uint8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(1);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_uint16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(1);

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityUpsampleUintTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityUpsampleUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  constexpr cl_uint hi = 0b1010'0101'1010'0101u;
  constexpr cl_uint lo = 0b0000'1111'0000'1111u;
  constexpr cl_uint expected = (hi << 16) | lo;
  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_EQ(expected, mem[0]) << "The upsample for int failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int2 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int3 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int3 failed.";
    EXPECT_EQ(expected, mem[0].z) << "The upsample for int3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_EQ(expected, mem[0].x) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].y) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].z) << "The upsample for int4 failed.";
    EXPECT_EQ(expected, mem[0].w) << "The upsample for int4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_EQ(expected, mem[0].s0) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s1) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s2) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s3) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s4) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s5) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s6) << "The upsample for int8 failed.";
    EXPECT_EQ(expected, mem[0].s7) << "The upsample for int8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_EQ(expected, mem[0].s0) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s1) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s2) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s3) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s4) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s5) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s6) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s7) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s8) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].s9) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sa) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sb) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sc) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sd) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].se) << "The upsample for int16 failed.";
    EXPECT_EQ(expected, mem[0].sf) << "The upsample for int16 failed.";
  }
}

TEST(ClCppTest, UtilityAbsFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  using LimitT = std::numeric_limits<cl_float>;
  constexpr cl_float maxf = (LimitT::max)();
  constexpr cl_float minf = (LimitT::min)();
  constexpr cl_float inf = LimitT::infinity();

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float> v = {1.5f, -1.5f};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.5f, -1.5f}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.5f, -1.5f, maxf}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.5f, -1.5f, maxf, -maxf}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                   1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityAbsFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityAbsFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0]) << "The abs for float failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[1]) << "The abs for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The abs for float2 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].y) << "The abs for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The abs for float3 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].y) << "The abs for float3 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].z) << "The abs for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The abs for float4 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].y) << "The abs for float4 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].z) << "The abs for float4 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].w) << "The abs for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s1) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].s2) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].s3) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s5) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].s6) << "The abs for float8 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].s7) << "The abs for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s1) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].s2) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].s3) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s5) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].s6) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].s7) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s8) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s9) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].sa) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(maxf, mem[0].sb) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].sc) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].sd) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].se) << "The abs for float16 failed.";
    EXPECT_FLOAT_EQ(inf, mem[0].sf) << "The abs for float16 failed.";
  }
}

TEST(ClCppTest, UtilityClampFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  using LimitT = std::numeric_limits<cl_float>;
  constexpr cl_float maxf = (LimitT::max)();
  constexpr cl_float minf = (LimitT::min)();
  constexpr cl_float inf = LimitT::infinity();

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float> v = {1.5f, -1.5f, 20.5f};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.5f, -1.5f},
                                                cl_float2{20.5f, -20.5f}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.5f, -1.5f, 20.5f}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.5f, -1.5f, maxf, -maxf}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                   1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityClampFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityClampFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0]) << "The clamp for float failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1]) << "The clamp for float failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[2]) << "The clamp for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The clamp for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The clamp for float2 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[1].x) << "The clamp for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1].y) << "The clamp for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The clamp for float3 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The clamp for float3 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].z) << "The clamp for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The clamp for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The clamp for float4 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].z) << "The clamp for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].w) << "The clamp for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s2) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s6) << "The clamp for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The clamp for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s2) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s6) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s8) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s9) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].sa) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sb) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].sc) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sd) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].se) << "The clamp for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sf) << "The clamp for float16 failed.";
  }
}

TEST(ClCppTest, UtilityMaxMinFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  using LimitT = std::numeric_limits<cl_float>;
  constexpr cl_float maxf = (LimitT::max)();
  constexpr cl_float minf = (LimitT::min)();
  constexpr cl_float inf = LimitT::infinity();

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float> v = {1.5f, -1.5f, 20.5f};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.5f, -1.5f},
                                                cl_float2{20.5f, -20.5f}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.5f, -1.5f, 20.5f}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.5f, -1.5f, maxf, -maxf}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf,
                   1.5f, -1.5f, maxf, -maxf, minf, -minf, inf, -inf}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityMaxMinFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityMaxMinFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0]) << "The maxmin for float failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1]) << "The maxmin for float failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[2]) << "The maxmin for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The maxmin for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The maxmin for float2 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[1].x) << "The maxmin for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1].y) << "The maxmin for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The maxmin for float3 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The maxmin for float3 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].z) << "The maxmin for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].x) << "The maxmin for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The maxmin for float4 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].z) << "The maxmin for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].w) << "The maxmin for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s2) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s6) << "The maxmin for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The maxmin for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(1.5f, mem[0].s0) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s2) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].s4) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].s6) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(1.5f, mem[0].s8) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s9) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].sa) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sb) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(minf, mem[0].sc) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sd) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(10.0f, mem[0].se) << "The maxmin for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sf) << "The maxmin for float16 failed.";
  }
}

TEST(ClCppTest, UtilityDegreesRadiansFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float> v = {1.0f, 2.0f,
                                               1.0f, 2.0f};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float2> v = {cl_float2{1.0f, 2.0f},
                                                cl_float2{1.0f, 2.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float3> v = {cl_float3{1.0f, 2.0f, -1.0f},
                                                cl_float3{1.0f, 2.0f, -1.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float4> v = {cl_float4{1.0f, 2.0f, -1.0f, -2.0f},
                                                cl_float4{1.0f, 2.0f, -1.0f, -2.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float8> v = {cl_float8{1.0f, 2.0f, -1.0f, -2.0f,
                                                          10.0f, 20.0f, -10.0f, -20.0f},
                                                cl_float8{1.0f, 2.0f, -1.0f, -2.0f,
                                                          10.0f, 20.0f, -10.0f, -20.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float16> v = {cl_float16{1.0f, 2.0f, -1.0f, -2.0f,
                                                            10.0f, 20.0f, -10.0f, -20.0f,
                                                            1.0f, 2.0f, -1.0f, -2.0f,
                                                            10.0f, 20.0f, -10.0f, -20.0f},
                                                 cl_float16{1.0f, 2.0f, -1.0f, -2.0f,
                                                            10.0f, 20.0f, -10.0f, -20.0f,
                                                            1.0f, 2.0f, -1.0f, -2.0f,
                                                            10.0f, 20.0f, -10.0f, -20.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityDegreesRadiansFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityDegreesRadiansFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  const float e1 = zivc::cl::degrees(1.0f);
  const float e2 = zivc::cl::radians(e1);
  const float e3 = zivc::cl::degrees(2.0f);
  const float e4 = zivc::cl::radians(e3);
  const float e5 = zivc::cl::degrees(-1.0f);
  const float e6 = zivc::cl::radians(e5);
  const float e7 = zivc::cl::degrees(-2.0f);
  const float e8 = zivc::cl::radians(e7);
  const float e9 = zivc::cl::degrees(10.0f);
  const float e10 = zivc::cl::radians(e9);
  const float e11 = zivc::cl::degrees(20.0f);
  const float e12 = zivc::cl::radians(e11);
  const float e13 = zivc::cl::degrees(-10.0f);
  const float e14 = zivc::cl::radians(e13);
  const float e15 = zivc::cl::degrees(-20.0f);
  const float e16 = zivc::cl::radians(e15);

  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0]) << "The degreesradians for float failed.";
    EXPECT_FLOAT_EQ(e2, mem[1]) << "The degreesradians for float failed.";
    EXPECT_FLOAT_EQ(e3, mem[2]) << "The degreesradians for float failed.";
    EXPECT_FLOAT_EQ(e4, mem[3]) << "The degreesradians for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0].x) << "The degreesradians for float2 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].y) << "The degreesradians for float2 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].x) << "The degreesradians for float2 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].y) << "The degreesradians for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0].x) << "The degreesradians for float3 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].y) << "The degreesradians for float3 failed.";
    EXPECT_FLOAT_EQ(e5, mem[0].z) << "The degreesradians for float3 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].x) << "The degreesradians for float3 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].y) << "The degreesradians for float3 failed.";
    EXPECT_FLOAT_EQ(e6, mem[1].z) << "The degreesradians for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0].x) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].y) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e5, mem[0].z) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e7, mem[0].w) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].x) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].y) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e6, mem[1].z) << "The degreesradians for float4 failed.";
    EXPECT_FLOAT_EQ(e8, mem[1].w) << "The degreesradians for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0].s0) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].s1) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e5, mem[0].s2) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e7, mem[0].s3) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e9, mem[0].s4) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e11, mem[0].s5) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e13, mem[0].s6) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e15, mem[0].s7) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].s0) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].s1) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e6, mem[1].s2) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e8, mem[1].s3) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e10, mem[1].s4) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e12, mem[1].s5) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e14, mem[1].s6) << "The degreesradians for float8 failed.";
    EXPECT_FLOAT_EQ(e16, mem[1].s7) << "The degreesradians for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(e1, mem[0].s0) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].s1) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e5, mem[0].s2) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e7, mem[0].s3) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e9, mem[0].s4) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e11, mem[0].s5) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e13, mem[0].s6) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e15, mem[0].s7) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e1, mem[0].s8) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e3, mem[0].s9) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e5, mem[0].sa) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e7, mem[0].sb) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e9, mem[0].sc) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e11, mem[0].sd) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e13, mem[0].se) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e15, mem[0].sf) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].s0) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].s1) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e6, mem[1].s2) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e8, mem[1].s3) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e10, mem[1].s4) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e12, mem[1].s5) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e14, mem[1].s6) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e16, mem[1].s7) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e2, mem[1].s8) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e4, mem[1].s9) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e6, mem[1].sa) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e8, mem[1].sb) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e10, mem[1].sc) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e12, mem[1].sd) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e14, mem[1].se) << "The degreesradians for float16 failed.";
    EXPECT_FLOAT_EQ(e16, mem[1].sf) << "The degreesradians for float16 failed.";
  }
}

TEST(ClCppTest, UtilitySignFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  using LimitT = std::numeric_limits<cl_float>;
  constexpr cl_float maxf = (LimitT::max)();
  constexpr cl_float minf = (LimitT::min)();
  constexpr cl_float inf = LimitT::infinity();
  const cl_float nan = LimitT::quiet_NaN();

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float> v = {2.0f, -2.0f, 0.0f, nan};
    ztest::setDeviceBuffer(*device, v, buffer_out1.get());
  }
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float2> v = {cl_float2{2.0f, -2.0f},
                                                cl_float2{0.0f, nan}};
    ztest::setDeviceBuffer(*device, v, buffer_out2.get());
  }
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float3> v = {cl_float3{2.0f, -2.0f, 2.0f}};
    ztest::setDeviceBuffer(*device, v, buffer_out3.get());
  }
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float4> v = {cl_float4{2.0f, -2.0f, 0.0f, nan}};
    ztest::setDeviceBuffer(*device, v, buffer_out4.get());
  }
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float8> v = {
        cl_float8{2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, nan}};
    ztest::setDeviceBuffer(*device, v, buffer_out8.get());
  }
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  {
    const std::initializer_list<cl_float16> v = {
        cl_float16{2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, nan,
                   2.0f, -2.0f, 0.0f, -0.0f, maxf, -maxf, inf, nan}};
    ztest::setDeviceBuffer(*device, v, buffer_out16.get());
  }

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilitySignFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilitySignFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0]) << "The sign for float failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[1]) << "The sign for float failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[2]) << "The sign for float failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[3]) << "The sign for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "The sign for float2 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "The sign for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1].x) << "The sign for float2 failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[1].y) << "The sign for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "The sign for float3 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "The sign for float3 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].z) << "The sign for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].x) << "The sign for float4 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].y) << "The sign for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].z) << "The sign for float4 failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[0].w) << "The sign for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].s0) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].s1) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s2) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s4) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].s5) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s6) << "The sign for float8 failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The sign for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(1.0f, mem[0].s0) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].s1) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s2) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s4) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].s5) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s6) << "The sign for float16 failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s8) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].s9) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sa) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sb) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].sc) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(-1.0f, mem[0].sd) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].se) << "The sign for float16 failed.";
    //! \attention NaN won't be processed in macOS sign
    //EXPECT_FLOAT_EQ(0.0f, mem[0].sf) << "The sign for float16 failed.";
  }
}

TEST(ClCppTest, UtilitySmoothstepFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(3);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(2);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(1);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(1);
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(1);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(1);

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilitySmoothstepFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilitySmoothstepFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  //! \todo Check the results
  // output
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out1->mapMemory();
  }
  // output2
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out2->mapMemory();
  }
  // output3
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out3->mapMemory();
  }
  // output4
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out4->mapMemory();
  }
  // output8
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out8->mapMemory();
  }
  // output16
  {
    [[maybe_unused]] const zivc::MappedMemory mem = buffer_out16->mapMemory();
  }
}

TEST(ClCppTest, UtilityStepFloatTest)
{
  const zivc::SharedContext context = ztest::createContext();
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());

  // Allocate buffers
  using zivc::cl_int;
  using zivc::cl_int2;
  using zivc::cl_int3;
  using zivc::cl_int4;
  using zivc::cl_int8;
  using zivc::cl_int16;
  using zivc::cl_uint;
  using zivc::cl_uint2;
  using zivc::cl_uint3;
  using zivc::cl_uint4;
  using zivc::cl_uint8;
  using zivc::cl_uint16;
  using zivc::cl_float;
  using zivc::cl_float2;
  using zivc::cl_float3;
  using zivc::cl_float4;
  using zivc::cl_float8;
  using zivc::cl_float16;

  const zivc::SharedBuffer buffer_out1 = device->createBuffer<cl_float>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out1->setSize(3);
  const zivc::SharedBuffer buffer_out2 = device->createBuffer<cl_float2>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out2->setSize(2);
  const zivc::SharedBuffer buffer_out3 = device->createBuffer<cl_float3>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out3->setSize(1);
  const zivc::SharedBuffer buffer_out4 = device->createBuffer<cl_float4>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out4->setSize(1);
  const zivc::SharedBuffer buffer_out8 = device->createBuffer<cl_float8>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out8->setSize(1);
  const zivc::SharedBuffer buffer_out16 = device->createBuffer<cl_float16>({zivc::BufferUsage::kPreferDevice, zivc::BufferFlag::kRandomAccessible});
  buffer_out16->setSize(1);

  device->waitForCompletion();

  // Make a kernel
  const zivc::KernelInitParams kernel_params = ZIVC_CREATE_KERNEL_INIT_PARAMS(cl_cpp_test1, utilityStepFloatTest, 1);
  const zivc::SharedKernel kernel = device->createKernel(kernel_params);
  ASSERT_EQ(1, kernel->dimensionSize()) << "Wrong kernel property.";
  ASSERT_EQ(6, kernel->argSize()) << "Wrong kernel property.";

  // Launch the kernel
  zivc::KernelLaunchOptions launch_options = kernel->createOptions();
  launch_options.setQueueIndex(0);
  launch_options.setWorkSize({{1}});
  launch_options.requestFence(true);
  launch_options.setLabel("UtilityStepFloatUintTest");
  ASSERT_EQ(1, launch_options.dimension());
  ASSERT_EQ(6, launch_options.numOfArgs());
  ASSERT_EQ(0, launch_options.queueIndex());
  ASSERT_EQ(1, launch_options.workSize()[0]);
  ASSERT_TRUE(launch_options.isFenceRequested());
  zivc::LaunchResult result = kernel->run(*buffer_out1, *buffer_out2, *buffer_out3,
                                          *buffer_out4, *buffer_out8, *buffer_out16,
                                          launch_options);
  device->waitForCompletion(result.fence());

  // output
  {
    const zivc::MappedMemory mem = buffer_out1->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0]) << "The sign for float failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1]) << "The sign for float failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[2]) << "The sign for float failed.";
  }
  // output2
  {
    const zivc::MappedMemory mem = buffer_out2->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0].x) << "The sign for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The sign for float2 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[1].x) << "The sign for float2 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[1].y) << "The sign for float2 failed.";
  }
  // output3
  {
    const zivc::MappedMemory mem = buffer_out3->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0].x) << "The sign for float3 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The sign for float3 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].z) << "The sign for float3 failed.";
  }
  // output4
  {
    const zivc::MappedMemory mem = buffer_out4->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0].x) << "The sign for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].y) << "The sign for float4 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].z) << "The sign for float4 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].w) << "The sign for float4 failed.";
  }
  // output8
  {
    const zivc::MappedMemory mem = buffer_out8->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0].s0) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s2) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s4) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s6) << "The sign for float8 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The sign for float8 failed.";
  }
  // output16
  {
    const zivc::MappedMemory mem = buffer_out16->mapMemory();
    EXPECT_FLOAT_EQ(0.0f, mem[0].s0) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s1) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s2) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s3) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s4) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s5) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].s6) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s7) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s8) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].s9) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].sa) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sb) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sc) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sd) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(1.0f, mem[0].se) << "The sign for float16 failed.";
    EXPECT_FLOAT_EQ(0.0f, mem[0].sf) << "The sign for float16 failed.";
  }
}
