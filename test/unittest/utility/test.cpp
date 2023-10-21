/*!
  \file test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Standard C++ library
#include <concepts>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string_view>
// Zisc
#include "zisc/ieee_754_binary.hpp"
#include "zisc/hash/fnv_1a_hash_engine.hpp"
#include "zisc/math/math.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "config.hpp"
#include "googletest.hpp"

namespace {

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float>
Float makeNormalT(const Float x) noexcept
{
  using BinaryT = zisc::BinaryFromFloat<Float>;

  constexpr auto one = static_cast<Float>(1.0);
  constexpr auto two = static_cast<Float>(2.0);
  constexpr auto bias = static_cast<int>(BinaryT::exponentBias());

  const Float k = two * std::abs(x) - one;
  const Float sign = std::copysign(one, x);
  const int exponent = std::clamp(static_cast<int>(k * static_cast<Float>(bias)),
                                  -bias + 1,
                                  bias );

  const auto h = zisc::Fnv1aHash32::hash(static_cast<zivc::uint32b>(std::abs(exponent)));
  const Float s = zisc::mapTo01<Float>(h);

  auto normal = s * static_cast<Float>(std::pow(two, exponent));
  normal = sign * std::clamp(normal,
                             std::numeric_limits<Float>::min(),
                             std::numeric_limits<Float>::max());
  return normal;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float>
Float makeSubnormalT(const Float x) noexcept
{
  constexpr Float subnormal_max = (std::numeric_limits<Float>::min)() -
                                  std::numeric_limits<Float>::denorm_min();
  static_assert(zisc::isSubnormal(subnormal_max), "The sub max isn't subnormal.");

  const Float sign = std::copysign(static_cast<Float>(1.0), x);
  Float subnormal = static_cast<Float>(std::pow(x, 4)) * subnormal_max;
  subnormal = sign * std::clamp(subnormal,
                                std::numeric_limits<Float>::denorm_min(),
                                subnormal_max);
  return subnormal;
}

} /* namespace */

namespace ztest {

/*!
  \details No detailed description
  */
void MathTestResult::checkError(const std::string_view name) const
{
  EXPECT_FALSE(fatal_nan_) << name.data() << " has unexpected NaN.";
  EXPECT_FALSE(fatal_inf_) << name.data() << " has unexpected Inf.";
  EXPECT_FALSE(fatal_outlier_) << name.data() << " has unexpected outlier.";
  EXPECT_FALSE(0 < num_of_outliers1_) << name.data() << " has outliers.";
}

/*!
  \details No detailed description

  \return No description
  */
double MathTestResult::getAverageUlpDiff() const noexcept
{
  const double average = (0 < num_of_trials_)
      ? static_cast<double>(total_ulp_diff_) / static_cast<double>(num_of_trials_)
      : 0.0;
  return average;
}

/*!
  \details No detailed description

  \return No description
  */
double MathTestResult::getAverageOutlierUlpDiff1() const noexcept
{
  const double average = (0 < num_of_outliers1_)
      ? static_cast<double>(total_outlier_ulp_diff1_) / static_cast<double>(num_of_outliers1_)
      : 0.0;
  return average;
}

/*!
  \details No detailed description

  \return No description
  */
double MathTestResult::getAverageOutlierUlpDiff2() const noexcept
{
  const double average = (0 < num_of_outliers2_)
      ? static_cast<double>(total_outlier_ulp_diff2_) / static_cast<double>(num_of_outliers2_)
      : 0.0;
  return average;
}

/*!
  \details No detailed description
  */
void MathTestResult::print() const noexcept
{
  std::cout << "-- num of trials       : " << num_of_trials_ << std::endl
            << "-- avg ulp diff        : " << std::scientific << std::setprecision(5)
                                           << getAverageUlpDiff() << std::endl
            << "-- max ulp diff        : " << max_ulp_diff_ << std::endl;
  if (0 < num_of_outliers1_) {
    std::cout << "-- ulp tolerance1      : " << ulp_outlier_tolerance1_ << std::endl
              << "--     num of outliers : " << num_of_outliers1_ << std::endl
              << "--     avg out ulp diff: " << std::scientific << std::setprecision(5)
                                                 << getAverageOutlierUlpDiff1() << std::endl;
  }
  if (0 < num_of_outliers2_) {
    std::cout << "-- ulp tolerance2      : " << ulp_outlier_tolerance2_ << std::endl
              << "--     num of outliers : " << num_of_outliers2_ << std::endl
              << "--     avg out ulp diff: " << std::scientific << std::setprecision(5)
                                                 << getAverageOutlierUlpDiff2() << std::endl;
  }

}

/*!
  \details No detailed description

  \return No description
  */
zivc::SharedContext createContext()
{
  auto& config = Config::globalConfig();
  if (!config.isGlobalContextEnabled())
    config.destroyContext();
  zivc::SharedContext context = config.queryContext();
  return context;
}

/*!
  \details No detailed description

  \param [in] x No description.
  \return No description
  */
float makeNormal(const float x) noexcept
{
  return ::makeNormalT(x);
}

/*!
  \details No detailed description

  \param [in] x No description.
  \return No description
  */
float makeSubnormal(const float x) noexcept
{
  return ::makeSubnormalT(x);
}

} // namespace ztest
