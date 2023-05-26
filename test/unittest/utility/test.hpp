/*!
  \file test.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_TEST_HPP
#define ZIVC_TEST_TEST_HPP

// Standard C++ library
#include <concepts>
#include <initializer_list>
#include <span>
#include <string_view>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"

namespace ztest {

/*!
  \brief No brief description

  No detailed description.
  */
struct MathTestResult
{
  //! Check any error
  void checkError(const std::string_view name) const;

  //! Return the average ULPs diff
  double getAverageUlpDiff() const noexcept;

  //! Return the average ULPs diff
  double getAverageOutlierUlpDiff() const noexcept;

  //! Print the test result
  void print() const noexcept;


  std::size_t num_of_trials_ = 0;
  std::size_t num_of_outliers_ = 0;
  std::size_t total_ulp_diff_ = 0;
  std::size_t total_outlier_ulp_diff_ = 0;
  std::size_t max_ulp_diff_ = 0;
  std::size_t ulp_outlier_tolerance_ = 16;
  bool fatal_outlier_ = false;
  bool fatal_inf_ = false;
  bool fatal_nan_ = false;
};

//!
template <std::floating_point Float>
Float calcRoughUlpDistance(const Float lhs, const Float rhs) noexcept;

//!
template <std::floating_point Float>
std::size_t calcUlpDistance(const Float lhs, const Float rhs) noexcept;

//! Create a zivc platform for unit test
zivc::SharedContext createContext();

//! Fill a device buffer with the give value
template <typename Type>
void fillDeviceBuffer(const Type& value, zivc::Buffer<Type>* buffer);

//! Initialize a device buffer with the given initializer list
template <typename Type>
void setDeviceBuffer(zivc::Device& device,
                     const std::initializer_list<Type>& init_list,
                     zivc::Buffer<Type>* buffer);

//! Initialize a device buffer with the given list
template <typename Type>
void setDeviceBuffer(zivc::Device& device,
                     const std::span<const Type>& init_list,
                     zivc::Buffer<Type>* buffer);

//! Copy buffer data
template <typename Type>
void copyBuffer(const zivc::Buffer<Type>& source, zivc::Buffer<Type>* dest);

//! Generate a random normal floating point number from a [-1, 1] value
float makeNormal(const float x) noexcept;

//! Generate a random subnormal floating point number from a [-1, 1] value
float makeSubnormal(const float x) noexcept;

//! Compare the given value with the expected and check the ULPs diff
template <std::floating_point Float>
bool test(const Float expected,
          const Float value,
          MathTestResult* result) noexcept;

} // namespace ztest

#include "test-inl.hpp"

#endif // ZIVC_TEST_TEST_HPP
