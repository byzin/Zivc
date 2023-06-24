/*!
  \file relational-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_RELATIONAL_INL_HPP
#define ZIVC_CL_RELATIONAL_INL_HPP

#include "relational.hpp"
// Zivc
#include "types.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isEqual(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isequal(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isNotEqual(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isnotequal(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isGreater(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isgreater(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isGreaterEqual(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isLess(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isless(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isLessEqual(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::islessequal(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isLessGreater(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::islessgreater(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isFinite(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isfinite(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isInf(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isinf(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isNan(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isnan(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isNormal(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::isnormal(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> Relation::isSignBitSet(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = ZIVC_CL_GLOBAL_NAMESPACE::signbit(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam IntegerN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename IntegerN> inline
int32b Relation::hasAnyMsb(const IntegerN arg) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const int32b result = ZIVC_CL_GLOBAL_NAMESPACE::any(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam IntegerN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename IntegerN> inline
int32b Relation::hasAllMsb(const IntegerN arg) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const int32b result = ZIVC_CL_GLOBAL_NAMESPACE::all(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam TypeN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename TypeN> inline
TypeN Relation::selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept
{
  const TypeN result = ZIVC_CL_GLOBAL_NAMESPACE::bitselect(a, b, c);
  return result;
}

/*!
  \details No detailed description

  \tparam TypeN No description.
  \tparam IntegerN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename TypeN, typename IntegerN> inline
TypeN Relation::selectValue(const TypeN a,
                            const TypeN b,
                            const IntegerN c) noexcept
{
  static_assert(kIsBoolean<IntegerN> || kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const TypeN result = ZIVC_CL_GLOBAL_NAMESPACE::select(a, b, c);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isequal(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isEqual(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isnotequal(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isNotEqual(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isgreater(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isGreater(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isgreaterequal(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isGreaterEqual(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isless(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isLess(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> islessequal(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isLessEqual(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> islessgreater(const FloatN lhs, const FloatN rhs) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isLessGreater(lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isfinite(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isFinite(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isinf(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isInf(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isnan(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isNan(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> isnormal(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isNormal(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename FloatN> inline
CompResult<FloatN> signbit(const FloatN arg) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const CompResult<FloatN> result = Relation::isSignBitSet(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam IntegerN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename IntegerN> inline
int32b any(const IntegerN arg) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer.");
  const int32b result = Relation::hasAnyMsb(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam IntegerN No description.
  \param [in] arg No description.
  \return No description
  */
template <typename IntegerN> inline
int32b all(const IntegerN arg) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer.");
  const int32b result = Relation::hasAllMsb(arg);
  return result;
}

/*!
  \details No detailed description

  \tparam TypeN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename TypeN> inline
TypeN bitselect(const TypeN a, const TypeN b, const TypeN c) noexcept
{
  const TypeN result = Relation::selectBit(a, b, c);
  return result;
}

/*!
  \details No detailed description

  \tparam TypeN No description.
  \tparam IntegerN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename TypeN, typename IntegerN> inline
TypeN select(const TypeN a, const TypeN b, const IntegerN c) noexcept
{
  static_assert(kIsBoolean<IntegerN> || kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const TypeN result = Relation::selectValue(a, b, c);
  return result;
}

} // namespace zivc

#endif /* ZIVC_CL_RELATIONAL_INL_HPP */
