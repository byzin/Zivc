/*!
  \file limits.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_LIMITS_HPP
#define ZIVC_LIMITS_HPP

// Zivc
#include "types.hpp"

namespace zivc {

/*!
  \brief Various properties of arithmetic types

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class NumericLimits
{
 public:
  //! Check if the NumericLimits<Type> is specialized
  static constexpr bool isSpecialized() noexcept;

  //! Identify signed type
  static constexpr bool isSigned() noexcept;

  //! Identify integer type
  static constexpr bool isInteger() noexcept;

  //! Identify exact type
  static constexpr bool isExact() noexcept;

  //! Identify floating-point types that can represent the special value "positive infinity"
  static constexpr bool hasInfinity() noexcept;

  //! Identify floating-point types that can represent the special value "quiet NaN"
  static constexpr bool hasQuietNan() noexcept;

  //! Return the number of radix digits that can be represented without change
  static constexpr int32b digits() noexcept;

  //! Return the number of decimal digits that can be represented without change
  static constexpr int32b digits10() noexcept;

  //! Return the number of decimal necessary to differentiate all values of type
  static constexpr int32b maxDigits10() noexcept;

  //! The radix or integer base used by the representation of the given type
  static constexpr int32b radix() noexcept;

  //! Return the smallest finite value of the Type
  static constexpr Type min() noexcept;

  //! Return the lowest finite value of the Type
  static constexpr Type lowest() noexcept;

  //! Return the largest finite value of the Type
  static constexpr Type max() noexcept;

  //! Return the difference between 1.0 and the next representable value
  static constexpr Type epsilon() noexcept;

  //! Return the positive infinity value of the Type
  static Type infinity() noexcept;

  //! Return the positive infinity value of the Type
  static Type quietNan() noexcept;
};

} // namespace zivc

#include "limits-inl.hpp"

#endif /* ZIVC_LIMITS_HPP */
