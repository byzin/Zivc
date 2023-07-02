/*!
  \file math-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_MATH_INL_HPP
#define ZIVC_CPUCL_MATH_INL_HPP

#include "math.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <concepts>
#include <limits>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
constexpr Float Math::invert(const Float& x) noexcept
{
  const Float y = static_cast<Float>(1.0) / x;
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
constexpr Vector<Float, kN> Math::invert(const Vector<Float, kN>& x) noexcept
{
  const Vector<Float, kN> y = static_cast<Float>(1.0) / x;
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fabs(const Float& x) noexcept
{
  const Float y = std::fabs(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fabs(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return fabs(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fmod(const Float& x, const Float& y) noexcept
{
  const Float z = std::fmod(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fmod(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return fmod(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::remainder(const Float& x, const Float& y) noexcept
{
  const Float z = std::remainder(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::remainder(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return remainder(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::remquo(const Float& x,
                   const Float& y,
                   int32b* quo) noexcept
{
  const Float z = std::remquo(x, y, quo);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float Math::remquo(const Float& x,
                   const Float& y,
                   AddressSpacePointer<kASpaceType, int32b> quo) noexcept
{
  return remquo(x, y, quo.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::remquo(const Vector<Float, kN>& x,
                               const Vector<Float, kN>& y,
                               Vector<int32b, kN>* quo) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y, int32b* quo) noexcept -> Float
  {
    return remquo(x, y, quo);
  };
  const VectorT z = VectorT::template apply<Float, int32b*>(func, x, y, quo);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> Math::remquo(const Vector<Float, kN>& x,
                               const Vector<Float, kN>& y,
                               AddressSpacePointer<kASpaceType, Vector<int32b, kN>> quo) noexcept
{
  return remquo(x, y, quo.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fma(const Float& x, const Float& y, const Float& z) noexcept
{
  const Float w = std::fma(x, y, z);
  return w;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fma(const Vector<Float, kN>& x, const Vector<Float, kN>& y, const Vector<Float, kN>& z) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y, const Float& z) noexcept -> Float
  {
    return fma(x, y, z);
  };
  const VectorT w = VectorT::template apply<Float, Float>(func, x, y, z);
  return w;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fmax(const Float& x, const Float& y) noexcept
{
  const Float z = std::fmax(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fmax(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return fmax(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fmin(const Float& x, const Float& y) noexcept
{
  const Float z = std::fmin(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fmin(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return fmin(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::maxmag(const Float& x, const Float& y) noexcept
{
  const Float abs_x = std::abs(x);
  const Float abs_y = std::abs(y);
  const Float z = (abs_y < abs_x) ? x : (abs_x < abs_y) ? y : std::fmax(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::maxmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return maxmag(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::minmag(const Float& x, const Float& y) noexcept
{
  const Float abs_x = std::abs(x);
  const Float abs_y = std::abs(y);
  const Float z = (abs_y < abs_x) ? y : (abs_x < abs_y) ? x : std::fmin(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::minmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return minmag(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fdim(const Float& x, const Float& y) noexcept
{
  const Float z = std::fdim(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fdim(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return fdim(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::pow(const Float& x, const Float& y) noexcept
{
  const Float z = std::pow(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::pow(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return pow(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::pown(const Float& x, const int32b y) noexcept
{
  const Float z = std::pow(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::pown(const Vector<Float, kN>& x, const Vector<int32b, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const int32b y) noexcept -> Float
  {
    return pown(x, y);
  };
  const VectorT z = VectorT::template apply<Float, int32b>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::sqrt(const Float& x) noexcept
{
  const Float y = std::sqrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::sqrt(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return sqrt(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::cbrt(const Float& x) noexcept
{
  const Float y = std::cbrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::cbrt(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return cbrt(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::rsqrt(const Float& x) noexcept
{
  const Float y = invert(sqrt(x));
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::rsqrt(const Vector<Float, kN>& x) noexcept
{
  const Vector<Float, kN> y = invert(sqrt(x));
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::ceil(const Float& x) noexcept
{
  const Float y = std::ceil(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::ceil(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return ceil(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::floor(const Float& x) noexcept
{
  const Float y = std::floor(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::floor(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return floor(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::trunc(const Float& x) noexcept
{
  const Float y = std::trunc(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::trunc(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return trunc(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::round(const Float& x) noexcept
{
  const Float y = std::round(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::round(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return round(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::rint(const Float& x) noexcept
{
  const Float y = std::rint(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::rint(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return rint(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::frexp(const Float& x, int32b* exp) noexcept
{
  const Float y = std::frexp(x, exp);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float Math::frexp(const Float& x, AddressSpacePointer<kASpaceType, int32b> exp) noexcept
{
  return frexp(x, exp.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::frexp(const Vector<Float, kN>& x, Vector<int32b, kN>* exp) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, int32b* exp) noexcept -> Float
  {
    return frexp(x, exp);
  };
  const VectorT y = VectorT::template apply<Float, int32b*>(func, x, exp);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> Math::frexp(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<int32b, kN>> exp) noexcept
{
  return frexp(x, exp.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::ldexp(const Float& x, const int32b k) noexcept
{
  const Float y = std::ldexp(x, k);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::ldexp(const Vector<Float, kN>& x, const Vector<int32b, kN>& k) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const int32b k) noexcept -> Float
  {
    return ldexp(x, k);
  };
  const VectorT y = VectorT::template apply<Float, int32b>(func, x, k);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::modf(const Float& x, Float* iptr) noexcept
{
  const Float y = std::modf(x, iptr);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float Math::modf(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept
{
  return modf(x, iptr.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::modf(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, Float* iptr) noexcept -> Float
  {
    return modf(x, iptr);
  };
  const VectorT y = VectorT::template apply<Float, Float*>(func, x, iptr);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> Math::modf(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept
{
  return modf(x, iptr.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::fract(const Float& x, Float* iptr) noexcept
{
  const Float floor_x = floor(x);
  const Float y = fmin(x - floor_x, 0x1.fffffep-1f);
  *iptr = floor_x;
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float Math::fract(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept
{
  return fract(x, iptr.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::fract(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, Float* iptr) noexcept -> Float
  {
    return fract(x, iptr);
  };
  const VectorT y = VectorT::template apply<Float, Float*>(func, x, iptr);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> Math::fract(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept
{
  return fract(x, iptr.get());
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
int32b Math::ilogb(const Float& x) noexcept
{
  const int32b y = std::ilogb(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<int32b, kN> Math::ilogb(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> int32b 
  {
    return ilogb(x);
  };
  const Vector<int32b, kN> y = VectorT::template apply<int32b>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::logb(const Float& x) noexcept
{
  const Float y = std::logb(x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::logb(const Vector<Float, kN>& x) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x) noexcept -> Float
  {
    return logb(x);
  };
  const VectorT y = VectorT::template apply<Float>(func, x);
  return y;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::nextafter(const Float& x, const Float& y) noexcept
{
  const Float z = std::nextafter(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::nextafter(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return nextafter(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float Math::copysign(const Float& x, const Float& y) noexcept
{
  const Float z = std::copysign(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Math::copysign(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  using VectorT = Vector<Float, kN>;
  const auto func = [](const Float& x, const Float& y) noexcept -> Float
  {
    return copysign(x, y);
  };
  const VectorT z = VectorT::template apply<Float, Float>(func, x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fabs(const Float& x) noexcept
{
  return Math::fabs(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fabs(const Vector<Float, kN>& x) noexcept
{
  return Math::fabs(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fmod(const Float& x, const Float& y) noexcept
{
  return Math::fmod(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fmod(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::fmod(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float remainder(const Float& x, const Float& y) noexcept
{
  return Math::remainder(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> remainder(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::remainder(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float> inline
Float remquo(const Float& x,
             const Float& y,
             int32b* quo) noexcept
{
  return Math::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float remquo(const Float& x,
             const Float& y,
             AddressSpacePointer<kASpaceType, int32b> quo) noexcept
{
  return Math::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y,
                         Vector<int32b, kN>* quo) noexcept
{
  return Math::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> remquo(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y,
                         AddressSpacePointer<kASpaceType, Vector<int32b, kN>> quo) noexcept
{
  return Math::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fma(const Float& x, const Float& y, const Float& z) noexcept
{
  return Math::fma(x, y, z);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fma(const Vector<Float, kN>& x, const Vector<Float, kN>& y, const Vector<Float, kN>& z) noexcept
{
  return Math::fma(x, y, z);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fmax(const Float& x, const Float& y) noexcept
{
  return Math::fmax(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fmax(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::fmax(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fmin(const Float& x, const Float& y) noexcept
{
  return Math::fmin(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fmin(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::fmin(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float maxmag(const Float& x, const Float& y) noexcept
{
  return Math::maxmag(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> maxmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::maxmag(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float minmag(const Float& x, const Float& y) noexcept
{
  return Math::minmag(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> minmag(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::minmag(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fdim(const Float& x, const Float& y) noexcept
{
  return Math::fdim(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fdim(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::fdim(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float pow(const Float& x, const Float& y) noexcept
{
  return Math::pow(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> pow(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::pow(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float pown(const Float& x, const int32b y) noexcept
{
  return Math::pown(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> pown(const Vector<Float, kN>& x, const Vector<int32b, kN>& y) noexcept
{
  return Math::pown(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float sqrt(const Float& x) noexcept
{
  return Math::sqrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> sqrt(const Vector<Float, kN>& x) noexcept
{
  return Math::sqrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float rsqrt(const Float& x) noexcept
{
  return Math::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> rsqrt(const Vector<Float, kN>& x) noexcept
{
  return Math::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float cbrt(const Float& x) noexcept
{
  return Math::cbrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> cbrt(const Vector<Float, kN>& x) noexcept
{
  return Math::cbrt(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float ceil(const Float& x) noexcept
{
  return Math::ceil(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> ceil(const Vector<Float, kN>& x) noexcept
{
  return Math::ceil(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float floor(const Float& x) noexcept
{
  return Math::floor(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> floor(const Vector<Float, kN>& x) noexcept
{
  return Math::floor(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float trunc(const Float& x) noexcept
{
  return Math::trunc(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> trunc(const Vector<Float, kN>& x) noexcept
{
  return Math::trunc(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float round(const Float& x) noexcept
{
  return Math::round(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> round(const Vector<Float, kN>& x) noexcept
{
  return Math::round(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float rint(const Float& x) noexcept
{
  return Math::rint(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> rint(const Vector<Float, kN>& x) noexcept
{
  return Math::rint(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float> inline
Float frexp(const Float& x, int32b* exp) noexcept
{
  return Math::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float frexp(const Float& x, AddressSpacePointer<kASpaceType, int32b> exp) noexcept
{
  return Math::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> frexp(const Vector<Float, kN>& x, Vector<int32b, kN>* exp) noexcept
{
  return Math::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> frexp(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<int32b, kN>> exp) noexcept
{
  return Math::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <std::floating_point Float> inline
Float ldexp(const Float& x, const int32b k) noexcept
{
  return Math::ldexp(x, k);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> ldexp(const Vector<Float, kN>& x, const Vector<int32b, kN>& k) noexcept
{
  return Math::ldexp(x, k);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float> inline
Float modf(const Float& x, Float* iptr) noexcept
{
  return Math::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float modf(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept
{
  return Math::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> modf(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept
{
  return Math::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> modf(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept
{
  return Math::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float> inline
Float fract(const Float& x, Float* iptr) noexcept
{
  return Math::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType> inline
Float fract(const Float& x, AddressSpacePointer<kASpaceType, Float> iptr) noexcept
{
  return Math::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> fract(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept
{
  return Math::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kASpaceType No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <std::floating_point Float, AddressSpaceType kASpaceType, std::size_t kN> inline
Vector<Float, kN> fract(const Vector<Float, kN>& x, AddressSpacePointer<kASpaceType, Vector<Float, kN>> iptr) noexcept
{
  return Math::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
int32b ilogb(const Float& x) noexcept
{
  return Math::ilogb(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<int32b, kN> ilogb(const Vector<Float, kN>& x) noexcept
{
  return Math::ilogb(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float> inline
Float logb(const Float& x) noexcept
{
  return Math::logb(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> logb(const Vector<Float, kN>& x) noexcept
{
  return Math::logb(x);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float nextafter(const Float& x, const Float& y) noexcept
{
  return Math::nextafter(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> nextafter(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::nextafter(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float> inline
Float copysign(const Float& x, const Float& y) noexcept
{
  return Math::copysign(x, y);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> copysign(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
{
  return Math::copysign(x, y);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_MATH_INL_HPP
