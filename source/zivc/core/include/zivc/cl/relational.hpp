/*!
  \file relational.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_RELATIONAL_HPP
#define ZIVC_CL_RELATIONAL_HPP

// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \brief Relational functions
  */
class Relation
{
 public:
  //! Return the component-wise compare of lhs == rhs
  template <typename FloatN>
  static CompResult<FloatN> isEqual(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of lhs != rhs 
  template <typename FloatN>
  static CompResult<FloatN> isNotEqual(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of lhs > rhs 
  template <typename FloatN>
  static CompResult<FloatN> isGreater(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of lhs >= rhs
  template <typename FloatN>
  static CompResult<FloatN> isGreaterEqual(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of lhs < rhs
  template <typename FloatN>
  static CompResult<FloatN> isLess(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of lhs <= rhs
  template <typename FloatN>
  static CompResult<FloatN> isLessEqual(const FloatN lhs, const FloatN rhs) noexcept;

  //! Return the component-wise compare of (lhs < rhs) || (lhs > rhs)
  template <typename FloatN>
  static CompResult<FloatN> isLessGreater(const FloatN lhs, const FloatN rhs) noexcept;

  //! Determin if the given floating point is a finite value
  template <typename FloatN>
  static CompResult<FloatN> isFinite(const FloatN arg) noexcept;

  //! Determin if the given floating point is a positive or negative infinity
  template <typename FloatN>
  static CompResult<FloatN> isInf(const FloatN arg) noexcept;

  //! Determin if the given floating point is a NaN
  template <typename FloatN>
  static CompResult<FloatN> isNan(const FloatN arg) noexcept;

  //! Determin if the given floating point is a normal value
  template <typename FloatN>
  static CompResult<FloatN> isNormal(const FloatN arg) noexcept;

  //! Determin if the given floating point is negative
  template <typename FloatN>
  static CompResult<FloatN> isSignBitSet(const FloatN arg) noexcept;

  //! Return 1 if the msb in any component of arg is set; otherwise return 0
  template <typename IntegerN>
  static int32b hasAnyMsb(const IntegerN arg) noexcept;

  //! Return 1 if the msb in all component of arg is set; otherwise return 0
  template <typename IntegerN>
  static int32b hasAllMsb(const IntegerN arg) noexcept;

  //! Each bit of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
  template <typename TypeN>
  static TypeN selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept;

  //! Each component of the result is set to c[i] ? b[i] : a[i]
  template <typename TypeN, typename IntegerN>
  static TypeN selectValue(const TypeN a, const TypeN b, const IntegerN c) noexcept;
};

// OpenCL function aliases

//! Return the component-wise compare of lhs == rhs
template <typename FloatN>
CompResult<FloatN> isequal(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of lhs != rhs
template <typename FloatN>
CompResult<FloatN> isnotequal(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of lhs > rhs
template <typename FloatN>
CompResult<FloatN> isgreater(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of lhs >= rhs
template <typename FloatN>
CompResult<FloatN> isgreaterequal(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of lhs < rhs
template <typename FloatN>
CompResult<FloatN> isless(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of lhs <= rhs
template <typename FloatN>
CompResult<FloatN> islessequal(const FloatN lhs, const FloatN rhs) noexcept;

//! Return the component-wise compare of (lhs < rhs) || (lhs > rhs)
template <typename FloatN>
CompResult<FloatN> islessgreater(const FloatN lhs, const FloatN rhs) noexcept;

//! Determin if the given floating point is a finite value
template <typename FloatN>
CompResult<FloatN> isfinite(const FloatN arg) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <typename FloatN>
CompResult<FloatN> isinf(const FloatN arg) noexcept;

//! Determin if the given floating point is a NaN
template <typename FloatN>
CompResult<FloatN> isnan(const FloatN arg) noexcept;

//! Determin if the given floating point is a normal value
template <typename FloatN>
CompResult<FloatN> isnormal(const FloatN arg) noexcept;

//! Determin if the given floating point is negative
template <typename FloatN>
CompResult<FloatN> signbit(const FloatN arg) noexcept;

//! Return 1 if the msb in any component of arg is set; otherwise return 0
template <typename IntegerN>
int32b any(const IntegerN arg) noexcept;

//! Return 1 if the msb in all component of arg is set; otherwise return 0
template <typename IntegerN>
int32b all(const IntegerN arg) noexcept;

//! Each bit of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <typename TypeN>
TypeN bitselect(const TypeN a, const TypeN b, const TypeN c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <typename TypeN, typename IntegerN>
TypeN select(const TypeN a, const TypeN b, const IntegerN c) noexcept;

} // namespace zivc

#include "relational-inl.hpp"

#endif /* ZIVC_CL_RELATIONAL_HPP */
