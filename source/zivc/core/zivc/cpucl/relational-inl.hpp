/*!
  \file relational-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_RELATIONAL_INL_HPP
#define ZIVC_CPUCL_RELATIONAL_INL_HPP

#include "relational.hpp"
// Standard C++ library
#include <cmath>
#include <concepts>
#include <cstddef>
#include <functional>
#include <limits>
#include <type_traits>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isequal(const Float lhs, const Float rhs) noexcept
{
  const CompResult<Float> result = std::equal_to<Float>{}(lhs, rhs) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isequal(const Vector<Float, kN>& lhs,
                                           const Vector<Float, kN>& rhs) noexcept
{
  return lhs == rhs;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isnotequal(const Float lhs, const Float rhs) noexcept
{
  const CompResult<Float> result = std::not_equal_to<Float>{}(lhs, rhs) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isnotequal(const Vector<Float, kN>& lhs,
                                              const Vector<Float, kN>& rhs) noexcept
{
  return lhs != rhs;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isgreater(const Float lhs, const Float rhs) noexcept
{
  const CompResult<Float> result = std::greater<Float>{}(lhs, rhs) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isgreater(const Vector<Float, kN>& lhs,
                                             const Vector<Float, kN>& rhs) noexcept
{
  return rhs < lhs;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template<std::floating_point Float> inline
CompResult<Float> Relation::isgreaterequal(const Float lhs, const Float rhs) noexcept
{
  const CompResult<Float> result = std::greater_equal<Float>{}(lhs, rhs) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isgreaterequal(const Vector<Float, kN>& lhs,
                                                  const Vector<Float, kN>& rhs) noexcept
{
  return rhs <= lhs;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isless(const Float lhs, const Float rhs) noexcept
{
  return isgreater(rhs, lhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isless(const Vector<Float, kN>& lhs,
                                          const Vector<Float, kN>& rhs) noexcept
{
  return isgreater(rhs, lhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::islessequal(const Float lhs, const Float rhs) noexcept
{
  return isgreaterequal(rhs, lhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::islessequal(const Vector<Float, kN>& lhs,
                                               const Vector<Float, kN>& rhs) noexcept
{
  return isgreaterequal(rhs, lhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::islessgreater(const Float lhs, const Float rhs) noexcept
{
  const CompResult<Float> result = (std::less<Float>{}(lhs, rhs) || std::greater<Float>{}(lhs, rhs)) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::islessgreater(const Vector<Float, kN>& lhs,
                                                 const Vector<Float, kN>& rhs) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto lessgreater = [](const Float& x, const Float& y) noexcept -> IntT
  {
    const int32b result = (islessgreater(x, y) == kSTrue) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT, Float>(lessgreater, lhs, rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isfinite(const Float value) noexcept
{
  const CompResult<Float> result = std::isfinite(value) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isfinite(const Vector<Float, kN>& value) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto is_finite = [](const Float& x) noexcept -> IntT
  {
    const int32b result = std::isfinite(x) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT>(is_finite, value);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isinf(const Float value) noexcept
{
  const CompResult<Float> result = std::isinf(value) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isinf(const Vector<Float, kN>& value) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto is_inf = [](const Float& x) noexcept -> IntT
  {
    const int32b result = std::isinf(x) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT>(is_inf, value);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isnan(const Float value) noexcept
{
  const CompResult<Float> result = std::isnan(value) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isnan(const Vector<Float, kN>& value) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto is_nan = [](const Float& x) noexcept -> IntT
  {
    const int32b result = std::isnan(x) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT>(is_nan, value);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::isnormal(const Float value) noexcept
{
  const CompResult<Float> result = std::isnormal(value) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::isnormal(const Vector<Float, kN>& value) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto is_normal = [](const Float& x) noexcept -> IntT
  {
    const int32b result = std::isnormal(x) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT>(is_normal, value);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> Relation::signbit(const Float value) noexcept
{
  const CompResult<Float> result = std::signbit(value) ? kSTrue : kSFalse;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> Relation::signbit(const Vector<Float, kN>& value) noexcept
{
  using VectorT = Vector<Float, kN>;
  using ResultT = CompResultVec<Float, kN>;
  using IntT = typename ResultT::Type;
  const auto signbit = [](const Float& x) noexcept -> IntT
  {
    const int32b result = std::signbit(x) ? kVTrue : kVFalse;
    return static_cast<IntT>(result);
  };
  const ResultT result = VectorT::template apply<IntT>(signbit, value);
  return result;
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer> inline
int32b Relation::any(const Integer x) noexcept
{
  using Bit = BitT<Integer>;
  const auto bit = zisc::bit_cast<Bit>(x);
  constexpr std::size_t shift = std::numeric_limits<Bit>::digits - 1;
  const auto result = static_cast<int32b>(bit >> shift);
  return result;
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer, size_t kN> inline
int32b Relation::any(const Vector<Integer, kN>& x) noexcept
{
  using VectorT = Vector<Integer, kN>;
  const auto func = [](const Integer& v) noexcept -> int32b
  {
    return any(v);
  };
  const Vector s = VectorT::template apply<int32b>(func, x);
  const int32b result = (0 < s.sum()) ? 0b01 : 0b00;
  return result;
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer> inline
int32b Relation::all(const Integer x) noexcept
{
  return any(x);
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer, size_t kN> inline
int32b Relation::all(const Vector<Integer, kN>& x) noexcept
{
  using VectorT = Vector<Integer, kN>;
  const auto func = [](const Integer& v) noexcept -> int32b
  {
    return all(v);
  };
  const Vector s = VectorT::template apply<int32b>(func, x);
  const int32b result = (static_cast<std::size_t>(s.sum()) == s.size()) ? 0b01 : 0b00;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type> inline
Type Relation::bitselect(const Type a, const Type b, const Type c) noexcept
{
  using Bit = BitT<Type>;
  const auto x = zisc::bit_cast<Bit>(a);
  const auto y = zisc::bit_cast<Bit>(b);
  const auto z = zisc::bit_cast<Bit>(c);
  const Bit result = (x ^ z) | (y & z);
  return zisc::bit_cast<Type>(result);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, size_t kN> inline
Vector<Type, kN> Relation::bitselect(const Vector<Type, kN>& a,
                                     const Vector<Type, kN>& b,
                                     const Vector<Type, kN>& c) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto bselect = [](const Type& x, const Type& y, const Type& z) noexcept
  {
    return bitselect(x, y, z);
  };
  const VectorT result = VectorT::template apply<Type, Type>(bselect, a, b, c);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Integer No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, std::integral Integer> inline
Type Relation::select(const Type a, const Type b, const Integer c) noexcept
{
  constexpr auto zero = static_cast<Integer>(0);
  const Type result = (c == zero) ? a : b;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Integer No description.
  \tparam kN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, std::integral Integer, size_t kN> inline
Vector<Type, kN> Relation::select(const Vector<Type, kN>& a,
                                  const Vector<Type, kN>& b,
                                  const Vector<Integer, kN>& c) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto selectv = [](const Type& x, const Type& y, const Integer& z) noexcept
  {
    return select(x, y, z);
  };
  const VectorT result = VectorT::template apply<Type, Integer>(selectv, a, b, c);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isequal(const Float lhs, const Float rhs) noexcept
{
  return Relation::isequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isequal(const Vector<Float, kN>& lhs,
                                 const Vector<Float, kN>& rhs) noexcept
{
  return Relation::isequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isnotequal(const Float lhs, const Float rhs) noexcept
{
  return Relation::isnotequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isnotequal(const Vector<Float, kN>& lhs,
                                    const Vector<Float, kN>& rhs) noexcept
{
  return Relation::isnotequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isgreater(const Float lhs, const Float rhs) noexcept
{
  return Relation::isgreater(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isgreater(const Vector<Float, kN>& lhs,
                                   const Vector<Float, kN>& rhs) noexcept
{
  return Relation::isgreater(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isgreaterequal(const Float lhs, const Float rhs) noexcept
{
  return Relation::isgreaterequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isgreaterequal(const Vector<Float, kN>& lhs,
                                        const Vector<Float, kN>& rhs) noexcept
{
  return Relation::isgreaterequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isless(const Float lhs, const Float rhs) noexcept
{
  return Relation::isless(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isless(const Vector<Float, kN>& lhs,
                                const Vector<Float, kN>& rhs) noexcept
{
  return Relation::isless(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> islessequal(const Float lhs, const Float rhs) noexcept
{
  return Relation::islessequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> islessequal(const Vector<Float, kN>& lhs,
                                     const Vector<Float, kN>& rhs) noexcept
{
  return Relation::islessequal(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> islessgreater(const Float lhs, const Float rhs) noexcept
{
  return Relation::islessgreater(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> islessgreater(const Vector<Float, kN>& lhs,
                                       const Vector<Float, kN>& rhs) noexcept
{
  return Relation::islessgreater(lhs, rhs);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isfinite(const Float value) noexcept
{
  return Relation::isfinite(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isfinite(const Vector<Float, kN>& value) noexcept
{
  return Relation::isfinite(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isinf(const Float value) noexcept
{
  return Relation::isinf(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isinf(const Vector<Float, kN>& value) noexcept
{
  return Relation::isinf(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isnan(const Float value) noexcept
{
  return Relation::isnan(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isnan(const Vector<Float, kN>& value) noexcept
{
  return Relation::isnan(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> isnormal(const Float value) noexcept
{
  return Relation::isnormal(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> isnormal(const Vector<Float, kN>& value) noexcept
{
  return Relation::isnormal(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float> inline
CompResult<Float> signbit(const Float value) noexcept
{
  return Relation::signbit(value);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
CompResultVec<Float, kN> signbit(const Vector<Float, kN>& value) noexcept
{
  return Relation::signbit(value);
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer> inline
int32b any(const Integer x) noexcept
{
  return Relation::any(x);
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer, size_t kN> inline
int32b any(const Vector<Integer, kN>& x) noexcept
{
  return Relation::any(x);
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer> inline
int32b all(const Integer x) noexcept
{
  return Relation::all(x);
}

/*!
  \details No detailed description

  \tparam Integer No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Integer, size_t kN> inline
int32b all(const Vector<Integer, kN>& x) noexcept
{
  return Relation::all(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type> inline
Type bitselect(const Type a, const Type b, const Type c) noexcept
{
  return Relation::bitselect(a, b, c);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, size_t kN> inline
Vector<Type, kN> bitselect(const Vector<Type, kN>& a,
                           const Vector<Type, kN>& b,
                           const Vector<Type, kN>& c) noexcept
{
  return Relation::bitselect(a, b, c);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Integer No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, std::integral Integer> inline
Type select(const Type a, const Type b, const Integer c) noexcept
{
  return Relation::select(a, b, c);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Integer No description.
  \tparam kN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <zisc::Scalar Type, std::integral Integer, size_t kN> inline
Vector<Type, kN> select(const Vector<Type, kN>& a,
                        const Vector<Type, kN>& b,
                        const Vector<Integer, kN>& c) noexcept
{
  return Relation::select(a, b, c);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_RELATIONAL_INL_HPP
