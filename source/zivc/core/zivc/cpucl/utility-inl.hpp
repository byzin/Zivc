/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_UTILITY_INL_HPP
#define ZIVC_CPUCL_UTILITY_INL_HPP

#include "utility.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <numbers>
#include <stdexcept>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t WorkItem::getEnqueuedLocalSize([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t WorkItem::getGlobalId(const uint32b dimension) noexcept
{
  const size_t id = getWorkGroupId(dimension) + getGlobalIdOffset(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t WorkItem::getGlobalLinearId() noexcept
{
  const uint32b dim = getDimension();
  // 1d
  const size_t gx = getGlobalId(0) - getGlobalIdOffset(0);
  size_t id = gx;
  // 2d
  if (2 <= dim) {
    const size_t n = getGlobalSize(0);
    const size_t gy = getGlobalId(1) - getGlobalIdOffset(1);
    id = id + n * gy;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = getGlobalSize(0) * getGlobalSize(1);
    const size_t gz = getGlobalId(2) - getGlobalIdOffset(2);
    id = id + n * gz;
  }
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t WorkItem::getGlobalSize(const uint32b dimension) noexcept
{
  return getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t WorkItem::getLocalId([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t WorkItem::getLocalLinearId() noexcept
{
  const uint32b dim = getDimension();
  // 1d
  const size_t lx = getLocalId(0);
  size_t id = lx;
  // 2d
  if (2 <= dim) {
    const size_t n = getLocalSize(0);
    const size_t ly = getLocalId(1);
    id = id + n * ly;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = getLocalSize(0) * getLocalSize(1);
    const size_t lz = getLocalId(2);
    id = id + n * lz;
  }
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t WorkItem::getLocalSize([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] condition No description.
  \param [in] format No description.
  \param [in] args No description.
  */
template <typename... Types> inline
void assertIfFalse(const bool condition, const char* format, const Types&... args)
{
  if (!condition) {
    std::fprintf(stderr, format, args...);
    std::exit(EXIT_FAILURE);
  }
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] args No description.
  */
template <typename... Types> inline
int printf(const char* format, const Types&... args)
{
  //! \todo Implement type-safe printf
  return std::printf(format, args...);
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_enqueued_local_size(const uint32b dimension) noexcept
{
  return WorkItem::getEnqueuedLocalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_id(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalId(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_global_linear_id() noexcept
{
  return WorkItem::getGlobalLinearId();
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_offset(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalIdOffset(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_size(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_group_id(const uint32b dimension) noexcept
{
  return WorkItem::getWorkGroupId(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_id(const uint32b dimension) noexcept
{
  return WorkItem::getLocalId(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_local_linear_id() noexcept
{
  return WorkItem::getLocalLinearId();
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_size(const uint32b dimension) noexcept
{
  return WorkItem::getLocalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_num_groups(const uint32b dimension) noexcept
{
  return WorkItem::getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b get_work_dim() noexcept
{
  return WorkItem::getDimension();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::signed_integral Type> inline
auto Utility::abs(const Type& x) noexcept
{
  using UnsignedT = std::make_unsigned_t<Type>;
  const Type result = isNegative(x) ? -x : x;
  return static_cast<UnsignedT>(result);
}

  /*!
    \details No detailed description

    \tparam Type No description.
    \param [in] x No description.
    \return No description
    */
template <std::unsigned_integral Type> inline
Type Utility::abs(const Type& x) noexcept
{
  return x;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::signed_integral Type, std::size_t kN> inline
Vector<std::make_unsigned_t<Type>, kN> Utility::abs(const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  using UnsignedT = std::make_unsigned_t<Type>;
  const auto func = [](const Type& v) noexcept -> UnsignedT
  {
    return Utility::abs(v);
  };
  const Vector<UnsignedT, kN> result = VectorT::template apply<UnsignedT>(func, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::unsigned_integral Type, std::size_t kN> inline
Vector<Type, kN> Utility::abs(const Vector<Type, kN>& x) noexcept
{
  return x;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::fabs(const Type& x) noexcept
{
  const Type result = isNegative(x) ? -x : x;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::fabs(const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::fabs(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] lo No description.
  \param [in] hi No description.
  \return No description
  */
template <Arithmetic Type> inline
Type Utility::clamp(const Type& x, const Type& lo, const Type& hi) noexcept
{
  return std::clamp(x, lo, hi);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] lo No description.
  \param [in] hi No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> Utility::clamp(const Vector<Type, kN>& x,
                                const Vector<Type, kN>& lo,
                                const Vector<Type, kN>& hi) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::clamp<Type>, x, lo, hi);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type> inline
Type Utility::clz(const Type& x) noexcept
{
  const int result = std::countl_zero(x);
  return static_cast<Type>(result);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> Utility::clz(const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::clz(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] radians No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::degrees(const Type& radians) noexcept
{
  constexpr Type k = static_cast<Type>(180.0) / std::numbers::pi_v<Type>;
  const Type result = k * radians;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] radians No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::degrees(const Vector<Type, kN>& radians) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::degrees(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, radians);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic Type> inline
constexpr bool Utility::isNegative(const Type& x) noexcept
{
  const bool result = std::is_signed_v<Type> && (x < static_cast<Type>(0));
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type> inline
Type Utility::max(const Type& x, const Type& y) noexcept
{
  return (std::max)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> Utility::max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& lhs, const Type& rhs) noexcept -> Type
  {
    return (Utility::max)(lhs, rhs);
  };
  const VectorT result = VectorT::template apply<Type, Type>(func, x, y);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type> inline
Type Utility::min(const Type& x, const Type& y) noexcept
{
  return (std::min)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> Utility::min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& lhs, const Type& rhs) noexcept -> Type
  {
    return (Utility::min)(lhs, rhs);
  };
  const VectorT result = VectorT::template apply<Type, Type>(func, x, y);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type> inline
Type Utility::popcount(const Type& x) noexcept
{
  const int result = std::popcount(x);
  return static_cast<Type>(result);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> Utility::popcount(const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::popcount(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] degrees No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::radians(const Type& degrees) noexcept
{
  constexpr Type k = std::numbers::pi_v<Type> / static_cast<Type>(180.0);
  const Type result = k * degrees;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] degrees No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::radians(const Vector<Type, kN>& degrees) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::radians(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, degrees);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] s No description.
  \return No description
  */
template <std::integral Type> inline
Type Utility::rotate(const Type& x, const Type& s) noexcept
{
  const Type result = std::rotl(x, static_cast<int>(s));
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] s No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> Utility::rotate(const Vector<Type, kN>& x,
                                 const Vector<Type, kN>& s) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v, const Type& i) noexcept -> Type
  {
    return Utility::rotate(v, i);
  };
  const VectorT result = VectorT::template apply<Type, Type>(func, x, s);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::sign(const Type& x) noexcept
{
  const Type result = std::isnan(x)  ? static_cast<Type>(0.0) :
                      isNegative(x)  ? static_cast<Type>(-1.0) :
                      isNegative(-x) ? static_cast<Type>(1.0)
                                     : x;
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::sign(const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& v) noexcept -> Type
  {
    return Utility::sign(v);
  };
  const VectorT result = VectorT::template apply<Type>(func, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] edge0 No description.
  \param [in] edge1 No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::smoothstep(const Type& edge0, const Type& edge1, const Type& x) noexcept
{
  constexpr auto v0 = static_cast<Type>(0.0);
  constexpr auto v1 = static_cast<Type>(1.0);
  constexpr auto v2 = static_cast<Type>(2.0);
  constexpr auto v3 = static_cast<Type>(3.0);
  const Type t = std::clamp((x - edge0) / (edge1 - edge0), v0, v1);
  const Type result = t * t * (v3 - v2 * t);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] edge0 No description.
  \param [in] edge1 No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::smoothstep(const Vector<Type, kN>& edge0,
                                     const Vector<Type, kN>& edge1,
                                     const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& e0, const Type& e1, const Type& v) noexcept -> Type
  {
    return Utility::smoothstep(e0, e1, v);
  };
  const VectorT result = VectorT::template apply<Type, Type>(func, edge0, edge1, x);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] edge No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type Utility::step(const Type& edge, const Type& x) noexcept
{
  const Type result = (x < edge) ? static_cast<Type>(0.0) : static_cast<Type>(1.0);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] edge No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> Utility::step(const Vector<Type, kN>& edge,
                               const Vector<Type, kN>& x) noexcept
{
  using VectorT = Vector<Type, kN>;
  const auto func = [](const Type& e, const Type& v) noexcept -> Type
  {
    return Utility::step(e, v);
  };
  const VectorT result = VectorT::template apply<Type, Type>(func, edge, x);
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int16b Utility::upsample(const int8b hi, const uint8b lo) noexcept
{
  const int16b h = static_cast<int16b>(hi) << 8;
  const int16b l = static_cast<int16b>(lo);
  const int16b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint16b Utility::upsample(const uint8b hi, const uint8b lo) noexcept
{
  const uint16b h = static_cast<uint16b>(hi) << 8;
  const uint16b l = static_cast<uint16b>(lo);
  const uint16b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int16b, kN> Utility::upsample(const Vector<int8b, kN>& hi,
                                     const Vector<uint8b, kN> lo) noexcept
{
  using VectorT = Vector<int8b, kN>;
  using DstVecT = Vector<int16b, kN>;
  const auto func = [](const int8b h, const uint8b l) noexcept -> int16b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<int16b, uint8b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint16b, kN> Utility::upsample(const Vector<uint8b, kN>& hi,
                                      const Vector<uint8b, kN> lo) noexcept
{
  using VectorT = Vector<uint8b, kN>;
  using DstVecT = Vector<uint16b, kN>;
  const auto func = [](const uint8b h, const uint8b l) noexcept -> uint16b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<uint16b, uint8b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int32b Utility::upsample(const int16b hi, const uint16b lo) noexcept
{
  const int32b h = static_cast<int32b>(hi) << 16;
  const int32b l = static_cast<int32b>(lo);
  const int32b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint32b Utility::upsample(const uint16b hi, const uint16b lo) noexcept
{
  const uint32b h = static_cast<uint32b>(hi) << 16;
  const uint32b l = static_cast<uint32b>(lo);
  const uint32b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int32b, kN> Utility::upsample(const Vector<int16b, kN>& hi,
                                     const Vector<uint16b, kN> lo) noexcept
{
  using VectorT = Vector<int16b, kN>;
  using DstVecT = Vector<int32b, kN>;
  const auto func = [](const int16b h, const uint16b l) noexcept -> int32b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<int32b, uint16b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint32b, kN> Utility::upsample(const Vector<uint16b, kN>& hi,
                                      const Vector<uint16b, kN> lo) noexcept
{
  using VectorT = Vector<uint16b, kN>;
  using DstVecT = Vector<uint32b, kN>;
  const auto func = [](const uint16b h, const uint16b l) noexcept -> uint32b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<uint32b, uint16b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int64b Utility::upsample(const int32b hi, const uint32b lo) noexcept
{
  const int64b h = static_cast<int64b>(hi) << 32;
  const int64b l = static_cast<int64b>(lo);
  const int64b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint64b Utility::upsample(const uint32b hi, const uint32b lo) noexcept
{
  const uint64b h = static_cast<uint64b>(hi) << 32;
  const uint64b l = static_cast<uint64b>(lo);
  const uint64b result = h | l;
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int64b, kN> Utility::upsample(const Vector<int32b, kN>& hi,
                                     const Vector<uint32b, kN> lo) noexcept
{
  using VectorT = Vector<int32b, kN>;
  using DstVecT = Vector<int64b, kN>;
  const auto func = [](const int32b h, const uint32b l) noexcept -> int64b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<int64b, uint32b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint64b, kN> Utility::upsample(const Vector<uint32b, kN>& hi,
                                      const Vector<uint32b, kN> lo) noexcept
{
  using VectorT = Vector<uint32b, kN>;
  using DstVecT = Vector<uint64b, kN>;
  const auto func = [](const uint32b h, const uint32b l) noexcept -> uint64b
  {
    return Utility::upsample(h, l);
  };
  const DstVecT result = VectorT::template apply<uint64b, uint32b>(func, hi, lo);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type> inline
auto abs(const Type& x) noexcept
{
  return Utility::abs(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<std::make_unsigned_t<Type>, kN> abs(const Vector<Type, kN>& x) noexcept
{
  return Utility::abs(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type fabs(const Type& x) noexcept
{
  return Utility::fabs(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> fabs(const Vector<Type, kN>& x) noexcept
{
  return Utility::fabs(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] lo No description.
  \param [in] hi No description.
  \return No description
  */
template <Arithmetic Type> inline
Type clamp(const Type& x, const Type& lo, const Type& hi) noexcept
{
  return Utility::clamp(x, lo, hi);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] lo No description.
  \param [in] hi No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> clamp(const Vector<Type, kN>& x,
                       const Vector<Type, kN>& lo,
                       const Vector<Type, kN>& hi) noexcept
{
  return Utility::clamp(x, lo, hi);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type> inline
Type clz(const Type& x) noexcept
{
  return Utility::clz(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> clz(const Vector<Type, kN>& x) noexcept
{
  return Utility::clz(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] radians No description.
  \return No description
  */
template <std::floating_point Type> inline
Type degrees(const Type& radians) noexcept
{
  return Utility::degrees(radians);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] radians No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> degrees(const Vector<Type, kN>& radians) noexcept
{
  return Utility::degrees(radians);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type> inline
Type max(const Type& x, const Type& y) noexcept
{
  return (Utility::max)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  return (Utility::max)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type> inline
Type min(const Type& x, const Type& y) noexcept
{
  return (Utility::min)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic Type, std::size_t kN> inline
Vector<Type, kN> min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  return (Utility::min)(x, y);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type> inline
Type popcount(const Type& x) noexcept
{
  return Utility::popcount(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> popcount(const Vector<Type, kN>& x) noexcept
{
  return Utility::popcount(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] degrees No description.
  \return No description
  */
template <std::floating_point Type> inline
Type radians(const Type& degrees) noexcept
{
  return Utility::radians(degrees);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] degrees No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> radians(const Vector<Type, kN>& degrees) noexcept
{
  return Utility::radians(degrees);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \param [in] s No description.
  \return No description
  */
template <std::integral Type> inline
Type rotate(const Type& x, const Type& s) noexcept
{
  return Utility::rotate(x, s);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] s No description.
  \return No description
  */
template <std::integral Type, std::size_t kN> inline
Vector<Type, kN> rotate(const Vector<Type, kN>& x,
                        const Vector<Type, kN>& s) noexcept
{
  return Utility::rotate(x, s);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type sign(const Type& x) noexcept
{
  return Utility::sign(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> sign(const Vector<Type, kN>& x) noexcept
{
  return Utility::sign(x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] edge0 No description.
  \param [in] edge1 No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type smoothstep(const Type& edge0, const Type& edge1, const Type& x) noexcept
{
  return Utility::smoothstep(edge0, edge1, x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] edge0 No description.
  \param [in] edge1 No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> smoothstep(const Vector<Type, kN>& edge0,
                            const Vector<Type, kN>& edge1,
                            const Vector<Type, kN>& x) noexcept
{
  return Utility::smoothstep(edge0, edge1, x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] edge No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type> inline
Type step(const Type& edge, const Type& x) noexcept
{
  return Utility::step(edge, x);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] edge No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Type, std::size_t kN> inline
Vector<Type, kN> step(const Vector<Type, kN>& edge,
                      const Vector<Type, kN>& x) noexcept
{
  return Utility::step(edge, x);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int16b upsample(const int8b hi, const uint8b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint16b upsample(const uint8b hi, const uint8b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int16b, kN> upsample(const Vector<int8b, kN>& hi,
                            const Vector<uint8b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint16b, kN> upsample(const Vector<uint8b, kN>& hi,
                             const Vector<uint8b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int32b upsample(const int16b hi, const uint16b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint32b upsample(const uint16b hi, const uint16b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int32b, kN> upsample(const Vector<int16b, kN>& hi,
                            const Vector<uint16b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint32b, kN> upsample(const Vector<uint16b, kN>& hi,
                             const Vector<uint16b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
int64b upsample(const int32b hi, const uint32b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
inline
uint64b upsample(const uint32b hi, const uint32b lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<int64b, kN> upsample(const Vector<int32b, kN>& hi,
                            const Vector<uint32b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] hi No description.
  \param [in] lo No description.
  \return No description
  */
template <std::size_t kN> inline
Vector<uint64b, kN> upsample(const Vector<uint32b, kN>& hi,
                             const Vector<uint32b, kN> lo) noexcept
{
  return Utility::upsample(hi, lo);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_UTILITY_INL_HPP
