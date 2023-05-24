/*!
  \file opencl_cpp_test1.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_OPENCL_CPP_TEST1_CL
#define ZIVC_TEST_OPENCL_CPP_TEST1_CL

// Zivc
#include "zivc/cl/boolean.hpp"
#include "zivc/cl/limits.hpp"
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"

namespace inner {

template <typename Type> inline
void setNumericLimit(zivc::GlobalPtr<zivc::Boolean> out_bool,
                     zivc::GlobalPtr<zivc::int32b> out_int,
                     zivc::GlobalPtr<Type> out_type) noexcept
{
  using LimitT = zivc::NumericLimits<Type>;
  size_t index = 0;
  out_bool[index++] = LimitT::isSpecialized();
  out_bool[index++] = LimitT::isSigned();
  out_bool[index++] = LimitT::isInteger();
  out_bool[index++] = LimitT::isExact();
  out_bool[index++] = LimitT::hasInfinity();
  out_bool[index++] = LimitT::hasQuietNan();
  index = 0;
  out_int[index++] = LimitT::digits();
  out_int[index++] = LimitT::digits10();
  out_int[index++] = LimitT::maxDigits10();
  out_int[index++] = LimitT::radix();
  index = 0;
  out_type[index++] = LimitT::min();
  out_type[index++] = LimitT::lowest();
  out_type[index++] = LimitT::max();
  out_type[index++] = LimitT::epsilon();
  out_type[index++] = LimitT::infinity();
  out_type[index++] = LimitT::quietNan();
}

} /* namespace inner */

__kernel void limitInt8Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                            zivc::GlobalPtr<zivc::int32b> out_int,
                            zivc::GlobalPtr<zivc::int8b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int8b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint8Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::uint8b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint8b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt16Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int16b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int16b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint16Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint16b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint16b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt32Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int32b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int32b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint32Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint32b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint32b>(out_bool, out_int, out_type);
  }
}

__kernel void limitInt64Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<zivc::int64b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::int64b>(out_bool, out_int, out_type);
  }
}

__kernel void limitUint64Test(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<zivc::uint64b> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<zivc::uint64b>(out_bool, out_int, out_type);
  }
}

__kernel void limitFloatTest(zivc::GlobalPtr<zivc::Boolean> out_bool,
                             zivc::GlobalPtr<zivc::int32b> out_int,
                             zivc::GlobalPtr<float> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<float>(out_bool, out_int, out_type);
  }
}

__kernel void limitDoubleTest(zivc::GlobalPtr<zivc::Boolean> out_bool,
                              zivc::GlobalPtr<zivc::int32b> out_int,
                              zivc::GlobalPtr<double> out_type)
{
  const size_t index = zivc::getGlobalIdX();
  if (index == 0) {
    inner::setNumericLimit<double>(out_bool, out_int, out_type);
  }
}

#endif /* ZIVC_TEST_OPENCL_CPP_TEST1_CL */
