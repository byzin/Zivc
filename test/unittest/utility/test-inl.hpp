/*!
  \file test-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_TEST_INL_HPP
#define ZIVC_TEST_TEST_INL_HPP

#include "test.hpp"

// Standard C++ library
#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <initializer_list>
// Zisc
#include "zisc/bit.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"

namespace ztest {

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
Float calcRoughUlpDistance(const Float lhs, const Float rhs) noexcept
{
  constexpr Float eps = std::numeric_limits<Float>::epsilon();
  constexpr Float zero = static_cast<Float>(0);
  const Float b = eps * std::abs(lhs + rhs);
  const Float ulps = std::abs(lhs - rhs) / ((b == zero) ? eps : b);
  return ulps;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
std::size_t calcUlpDistance(const Float lhs, const Float rhs) noexcept
{
  using BitT = std::conditional_t<sizeof(Float) == 4, zivc::uint32b, zivc::uint64b>;
  auto l = zisc::bit_cast<BitT>(std::abs(lhs));
  auto r = zisc::bit_cast<BitT>(std::abs(rhs));
  if (l < r)
    std::swap(l, r);
  const auto ulps = static_cast<std::size_t>(l - r);
  return ulps;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] value No description.
  \param [out] buffer No description.
  */
template <typename Type> inline
void fillDeviceBuffer(const Type& value, zivc::Buffer<Type>* buffer)
{
  auto options = buffer->createOptions();
  options.setSize(buffer->size());
  options.requestFence(true);
  auto result = zivc::fill(value, buffer, options);
  result.fence().wait();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] device No description.
  \param [in] init_list No description.
  \param [out] buffer No description.
  */
template <typename Type> inline
void setDeviceBuffer(zivc::Device& device,
                     const std::initializer_list<Type>& init_list,
                     zivc::Buffer<Type>* buffer)
{
  buffer->setSize(init_list.size());
  auto tmp_buffer = device.createBuffer<Type>({zivc::BufferUsage::kPreferHost,
                                               zivc::BufferFlag::kRandomAccessible});
  tmp_buffer->setSize(init_list.size());
  {
    auto mem = tmp_buffer->mapMemory();
    std::copy_n(init_list.begin(), init_list.size(), mem.begin());
  }
  auto options = tmp_buffer->createOptions();
  options.setSize(init_list.size());
  options.requestFence(true);
  auto result = zivc::copy(*tmp_buffer, buffer, options);
  result.fence().wait();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] source No description.
  \param [out] dest No description.
  */
template <typename Type> inline
void copyBuffer(const zivc::Buffer<Type>& source, zivc::Buffer<Type>* dest)
{
  dest->setSize(source.size());
  auto options = source.createOptions();
  options.setSize(source.size());
  options.requestFence(true);
  auto result = zivc::copy(source, dest, options);
  result.fence().wait();
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] expected No description.
  \param [in] value No description.
  \param [in,out] result No description.
  \return No description
  */
template <std::floating_point Float> inline
bool test(const Float expected,
          const Float value,
          MathTestResult* result) noexcept
{
  ++result->num_of_trials_;
  // Check NaN
  if (std::isnan(expected) && std::isnan(value))
    return true;
  // Check if either expected or value is NaN
  if (std::isnan(expected) || std::isnan(value)) {
    result->fatal_nan_ = true;
    return false;
  }

  // Check sign
  {
    constexpr auto o = static_cast<Float>(1.0);
    const Float sign = std::copysign(o, expected) * std::copysign(o, value);
    if (sign != o) {
      result->fatal_outlier_ = true;
      return false;
    }
  }

  // Check Inf
  if (std::isinf(expected) && std::isinf(value))
    return true;
  // Check if either expected or value is inf
  if (std::isinf(expected) || std::isinf(value)) {
    result->fatal_inf_ = true;
    return false;
  }

  // Check finite number
  if ((expected + value) == static_cast<Float>(0))
    return true;
  // Compute roug ULP distance first
  const Float rough_ulps = calcRoughUlpDistance(expected, value);
  const std::size_t rough_tolerance = 4 * (std::max)(
      static_cast<std::size_t>(std::numeric_limits<Float>::digits),
      result->ulp_outlier_tolerance_);
  float flag = rough_ulps <= static_cast<Float>(rough_tolerance);
  if (flag) {
    const std::size_t ulps = calcUlpDistance(expected, value);
    result->total_ulp_diff_ += ulps;
    flag = ulps <= result->ulp_outlier_tolerance_;
    if (!flag) {
      ++result->num_of_outliers_;
      result->total_outlier_ulp_diff_ += ulps;
    }
    result->max_ulp_diff_ = (std::max)(result->max_ulp_diff_, ulps);
  }
  else {
    ++result->num_of_outliers_;
    result->total_outlier_ulp_diff_ += static_cast<std::size_t>(rough_ulps);
    result->max_ulp_diff_ = (std::max)(result->max_ulp_diff_,
                                       static_cast<std::size_t>(rough_ulps));
    result->fatal_outlier_ = true;
  }
  return flag;
}

} // namespace ztest

#endif /* ZIVC_TEST_TEST_INL_HPP */
