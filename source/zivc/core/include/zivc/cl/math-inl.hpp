/*!
  \file math-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_MATH_INL_HPP
#define ZIVC_CL_MATH_INL_HPP

#include "math.hpp"
// Zivc
#include "bit.hpp"
#include "limits.hpp"
#include "relational.hpp"
#include "types.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fabs(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fabs(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fmod(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fmod(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::remainder(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::remainder(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN Math::remquo(const FloatN x, const FloatN y, AddressSpacePointer quo) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The pointer object isn't pointer.");
  static_assert(kIsInteger<typename ASpaceInfo::DataT>, "The pointer object isn't integer.");
  return ZIVC_CL_GLOBAL_NAMESPACE::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fma(const FloatN a, const FloatN b, const FloatN c) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fma(a, b, c);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fmax(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fmax(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fmin(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fmin(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::maxmag(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::maxmag(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::minmag(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::minmag(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::fdim(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fdim(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::pow(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl::pow(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN Math::pown(const FloatN x, const IntN y) noexcept
{
  using VecFInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementFT = typename VecFInfo::ElementT;
  static_assert(kIsFloat<ElementFT>, "The FloatN isn't floating point.");
  using VecIInfo = VectorTypeInfo<RemoveVolatileT<IntN>>;
  using ElementIT = typename VecIInfo::ElementT;
  static_assert(kIsInteger<ElementIT>, "The IntN isn't integer.");
  static_assert(VecFInfo::size() == VecIInfo::size(), "The FloatN and IntN have different size.");
  return Impl::pown(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::rsqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::cbrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl::cbrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::ceil(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::ceil(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::floor(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::floor(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::trunc(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::trunc(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::round(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::round(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::rint(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::rint(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN Math::frexp(const FloatN x, AddressSpacePointer exp) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The pointer object isn't pointer.");
  static_assert(kIsInteger<typename ASpaceInfo::DataT>, "The pointer object isn't integer.");
  return ZIVC_CL_GLOBAL_NAMESPACE::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntegerN No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::ldexp(const FloatN x, const IntegerN k) noexcept
{
  using VecInfo1 = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using Element1T = typename VecInfo1::ElementT;
  static_assert(kIsFloat<Element1T>, "The FloatN isn't floating point.");
  using VecInfo2 = VectorTypeInfo<RemoveVolatileT<IntegerN>>;
  using Element2T = typename VecInfo2::ElementT;
  static_assert(kIsInteger<Element2T>, "The IntegerN isn't integer.");
  return ZIVC_CL_GLOBAL_NAMESPACE::ldexp(x, k);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN Math::modf(const FloatN x, AddressSpacePointer iptr) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The pointer object isn't pointer.");
  static_assert(kIsFloat<typename ASpaceInfo::DataT>, "The pointer object isn't integer.");
  return ZIVC_CL_GLOBAL_NAMESPACE::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN Math::fract(const FloatN x, AddressSpacePointer iptr) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The pointer object isn't pointer.");
  static_assert(kIsFloat<typename ASpaceInfo::DataT>, "The pointer object isn't integer.");
  return ZIVC_CL_GLOBAL_NAMESPACE::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
auto Math::ilogb(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const Integer32VecT<VecInfo::size()> y = ZIVC_CL_GLOBAL_NAMESPACE::ilogb(x);
  return y;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::logb(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::logb(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::nextafter(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::nextafter(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::copysign(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return ZIVC_CL_GLOBAL_NAMESPACE::copysign(x, y);
}


/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Native::pow(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const FloatN z = ZIVC_CL_GLOBAL_NAMESPACE::pow(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN Math::Native::pown(const FloatN x, const IntN y) noexcept
{
  using VecFInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementFT = typename VecFInfo::ElementT;
  static_assert(kIsFloat<ElementFT>, "The FloatN isn't floating point.");
  using VecIInfo = VectorTypeInfo<RemoveVolatileT<IntN>>;
  using ElementIT = typename VecIInfo::ElementT;
  static_assert(kIsInteger<ElementIT>, "The IntN isn't integer.");
  static_assert(VecFInfo::size() == VecIInfo::size(), "The FloatN and IntN have different size.");
  const FloatN z = ZIVC_CL_GLOBAL_NAMESPACE::pown(x, y);
  return z;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Native::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const FloatN y = ZIVC_CL_GLOBAL_NAMESPACE::sqrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Native::rsqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const FloatN y = ZIVC_CL_GLOBAL_NAMESPACE::rsqrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Native::cbrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const FloatN y = ZIVC_CL_GLOBAL_NAMESPACE::cbrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Precision::pow(const FloatN x, const FloatN y) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl<ElementT>::pow(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN Math::Precision::pown(const FloatN x, const IntN y) noexcept
{
  using VecFInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementFT = typename VecFInfo::ElementT;
  static_assert(kIsFloat<ElementFT>, "The FloatN isn't floating point.");
  using VecIInfo = VectorTypeInfo<RemoveVolatileT<IntN>>;
  using ElementIT = typename VecIInfo::ElementT;
  static_assert(kIsInteger<ElementIT>, "The IntN isn't integer.");
  static_assert(VecFInfo::size() == VecIInfo::size(), "The FloatN and IntN have different size.");
  return Impl<ElementFT>::pown(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Precision::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl<ElementT>::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Precision::rsqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Util<ElementT>::invert(sqrt(x));
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Precision::cbrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl<ElementT>::cbrt(x);
}

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Math::Precision::Util<float>
{
  // Type aliases
  using LimitT = NumericLimits<float>;


  template <typename FloatN>
  struct V2
  {
    FloatN x;
    FloatN y;
  };

  template <typename FloatN>
  static FloatN invert(const FloatN x) noexcept
  {
    const FloatN y =  1.0f / x;
    return y;
  }

  template <typename FloatN>
  static V2<FloatN> df(const FloatN h, const FloatN l) noexcept
  {
    return {h, l};
  }

  template <typename FloatN>
  static FloatN abs(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using UIntN = UInteger32VecT<VecInfo::size()>;

    const FloatN y = castBit<FloatN>(castBit<UIntN>(x) & 0x7fffffffu);
    return y;
  }

  template <typename FloatN>
  static FloatN mla(const FloatN x, const FloatN y, const FloatN z) noexcept
  {
    const FloatN w = x * y + z;
    return w;
  }

  template <typename FloatN>
  static FloatN rint(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    constexpr FloatN p = 0.5f;
    const FloatN a = mulsign(p, x);
    const FloatN y = cast<FloatN>(cast<IntN>(x + a));
    return y;
  }

  template <typename FloatN>
  static CompResult<FloatN> signbit(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using UIntN = UInteger32VecT<VecInfo::size()>;

    constexpr uint32b mask = 0b01u << 31u;
    const CompResult<UIntN> result = (castBit<UIntN>(x) & mask) == mask;
    return result;
  }

  template <typename FloatN>
  static FloatN upper(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using UIntN = UInteger32VecT<VecInfo::size()>;

    const FloatN y = castBit<FloatN>(castBit<UIntN>(x) & 0xfffff000u);
    return y;
  }

  template <typename FloatN>
  static V2<FloatN> add(const FloatN x, const FloatN y) noexcept
  {
    const FloatN rx = x + y;
    const FloatN ry = x - rx + y;;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add2(const FloatN x, const FloatN y) noexcept
  {
    const FloatN rx = x + y;
    const FloatN v = rx - x;
    const FloatN ry = (x - (rx - v)) + (y - v);

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add(const V2<FloatN> x, const FloatN y) noexcept
  {
    const FloatN rx = x.x + y;;
    const FloatN ry = x.x - rx + y + x.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add2(const V2<FloatN> x, const FloatN y) noexcept
  {
    const FloatN rx = x.x + y;
    const FloatN v = rx - x.x;
    const FloatN ry = ((x.x - (rx - v)) + (y - v)) + x.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add(const FloatN x, const V2<FloatN> y) noexcept
  {
    const FloatN rx = x + y.x;
    const FloatN ry = x - rx + y.x + y.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add2(const FloatN x, const V2<FloatN> y) noexcept
  {
    const FloatN rx = x + y.x;
    const FloatN v = rx - x;
    const FloatN ry = (x - (rx - v)) + (y.x - v) + y.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add(const V2<FloatN> x, const V2<FloatN> y) noexcept
  {
    const FloatN rx = x.x + y.x;
    const FloatN ry = x.x - rx + y.x + x.y + y.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> add2(const V2<FloatN> x, const V2<FloatN> y) noexcept
  {
    const FloatN rx = x.x + y.x;
    const FloatN v = rx - x.x;
    const FloatN ry = ((x.x - (rx - v)) + (y.x - v)) + (x.y + y.y);

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> mul(const FloatN x, const FloatN y) noexcept
  {
    const FloatN xh = upper(x);
    const FloatN xl = x - xh;
    const FloatN yh = upper(y);
    const FloatN yl = y - yh;

    const FloatN rx = x * y;
    const FloatN ry = xh * yh - rx + xl * yh + xh * yl + xl * yl;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> mul(const V2<FloatN> x, const FloatN y) noexcept
  {
    const FloatN xh = upper(x.x);
    const FloatN xl = x.x - xh;
    const FloatN yh = upper(y);
    const FloatN yl = y - yh;

    const FloatN rx = x.x * y;
    const FloatN ry = xh * yh - rx + xl * yh + xh * yl + xl * yl + x.y * y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> mul(const V2<FloatN> x, const V2<FloatN> y) noexcept
  {
    const FloatN xh = upper(x.x);
    const FloatN xl = x.x - xh;
    const FloatN yh = upper(y.x);
    const FloatN yl = y.x - yh;

    const FloatN rx = x.x * y.x;
    const FloatN ry = xh * yh - rx + xl * yh + xh * yl + xl * yl + x.x * y.y + x.y * y.x;

    return {rx, ry};
  }

  template <typename FloatN>
  static FloatN mulF(const V2<FloatN> x, const V2<FloatN> y) noexcept
  {
    const FloatN xh = upper(x.x);
    const FloatN xl = x.x - xh;
    const FloatN yh = upper(y.x);
    const FloatN yl = y.x - yh;

    const FloatN z = x.y * yh + xh * y.y + xl * yl + xh * yl + xl * yh + xh * yh;
    return z;
  }

  template <typename FloatN>
  static V2<FloatN> div(const V2<FloatN> n, const V2<FloatN> d) noexcept
  {
    const FloatN t = 1.0f / d.x;
    const FloatN dh = upper(d.x);
    const FloatN dl = d.x - dh;
    const FloatN th = upper(t);
    const FloatN tl = t - th;
    const FloatN nhh = upper(n.x);
    const FloatN nhl = n.x - nhh;

    const FloatN qx = n.x * t;
    const FloatN u = -qx + nhh * th + nhh * tl + nhl * th + nhl * tl +
                     qx * (1.0f - dh * th - dh * tl - dl * th - dl * tl);
    const FloatN qy = t * (n.y - qx * d.y) + u;

    return {qx, qy};
  }

//  template <typename FloatN>
//  static V2<FloatN> frecV2(const FloatN d) noexcept
//  {
//    const FloatN t = 1.0f / d;
//    const FloatN dh = upper(d);
//    const FloatN dl = d - dh;
//    const FloatN th = upper(t);
//    const FloatN tl = t - th;
//
//    const FloatN qx = t;
//    const FloatN qy = t * (1.0f - dh * th - dh * tl - dl * th - dl * tl);
//
//    return {qx, qy};
//  }

  template <typename FloatN>
  static V2<FloatN> squ(const V2<FloatN> x) noexcept
  {
    const FloatN xh = upper(x.x);
    const FloatN xl = x.x - xh;

    const FloatN rx = x.x * x.x;
    const FloatN ry = xh * xh - rx + (xh + xh) * xl + xl * xl + x.x * (x.y + x.y);

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> normalize(const V2<FloatN> x) noexcept
  {
    const FloatN rx = x.x + x.y;
    const FloatN ry = x.x - rx + x.y;

    return {rx, ry};
  }

  template <typename FloatN>
  static V2<FloatN> scale(const V2<FloatN> d, const FloatN s) noexcept
  {
    const FloatN rx = d.x * s;
    const FloatN ry = d.y * s;

    return {rx, ry};
  }

  template <typename FloatN>
  static FloatN mulsign(const FloatN x, const FloatN y) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using UIntN = UInteger32VecT<VecInfo::size()>;

    constexpr UIntN mask = 0b01u << 31u;
    const FloatN z = castBit<FloatN>(castBit<UIntN>(x) ^ (castBit<UIntN>(y) & mask));
    return z;
  }

  template <typename FloatN>
  static FloatN exp(const V2<FloatN> x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    const FloatN q = rint((x.x + x.y) * kRLn2);
    V2<FloatN> s = add2(x, q * -kL2U);
    s = add2(s, q * -kL2L);

    s = normalize(s);

    FloatN u = 0.00136324646882712841033936f;
    u = mla<FloatN>(u, s.x, 0.00836596917361021041870117f);
    u = mla<FloatN>(u, s.x, 0.0416710823774337768554688f);
    u = mla<FloatN>(u, s.x, 0.166665524244308471679688f);
    u = mla<FloatN>(u, s.x, 0.499999850988388061523438f);

    V2<FloatN> t = add(s, mul(squ(s), u));

    constexpr FloatN one = 1.0f;
    t = add(one, t);

    u = ldexp(t.x + t.y, cast<IntN>(q));

    constexpr FloatN edge = -104.0f;
    u = u * step(edge, x.x);

    return u;
  }

  template <typename FloatN>
  static V2<FloatN> log(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    constexpr FloatN one = 1.0f;
    FloatN d = 0.0f;
    const CompResult<FloatN> o = x < LimitT::min();
    {
      constexpr uint32b k = 0b01001111100000000000000000000000u;
      const FloatN v = castBit<float>(k) * castBit<float>(k);
      d = x * select(one, v, o);
    }

    IntN e = ilogb2<IntN>(d * (1.0f / 0.75f));
    const FloatN m = ldexp3(d, -e);

    {
      constexpr IntN k0 = 0;
      constexpr IntN k1 = 64;
      e = e - select(k0, k1, o);
    }

    const V2<FloatN> a = div(add2(-one, m), add2(one, m));
    const V2<FloatN> a2 = squ(a);

    FloatN t = 0.240320354700088500976562f;
    t = mla<FloatN>(t, a2.x, 0.285112679004669189453125f);
    t = mla<FloatN>(t, a2.x, 0.400007992982864379882812f);
    const V2<FloatN> c = df<FloatN>(0.66666662693023681640625f, 3.69183861259614332084311e-09f);

    V2<FloatN> s = mul(df<FloatN>(0.69314718246459960938f, -1.904654323148236017e-09f),
                       cast<FloatN>(e));
    s = add(s, scale<FloatN>(a, 2.0f));
    s = add(s, mul(mul(a2, a), add2(mul(a2, t), c)));

    return s;
  }

  template <typename FloatN, typename IntN>
  static FloatN pow2(const IntN x) noexcept
  {
    const FloatN y = castBit<FloatN>((x + 0x7f) << 23);
    return y;
  }

  template <typename FloatN, typename IntN>
  static FloatN ldexp(const FloatN x, const IntN e) noexcept
  {
    IntN m = e >> 31;
    m = (((m + e) >> 6) - m) << 4;
    const IntN q = e - (m << 2);
    m = clamp<IntN>(m + 127, 0, 255);
    FloatN u = castBit<FloatN>(m << 23);
    const FloatN t = x * u * u * u * u;;
    u = castBit<FloatN>((q + 0x7f) << 23);
    const FloatN y = t * u;
    return y;
  }

  template <typename FloatN, typename IntN>
  static FloatN ldexp2(const FloatN x, const IntN e) noexcept
  {
    const FloatN y = x * pow2<FloatN>(e >> 1) * pow2<FloatN>(e - (e >> 1));
    return y;
  }

  template <typename FloatN, typename IntN>
  static FloatN ldexp3(const FloatN x, const IntN e) noexcept
  {
    const FloatN y = castBit<FloatN>(castBit<IntN>(x) + (e << 23));
    return y;
  }

  template <typename IntN, typename FloatN>
  static IntN ilogb(const FloatN x) noexcept
  {
    const CompResult<FloatN> m = x < 5.421010862427522e-20f;

    constexpr FloatN k1 = 1.0f;
    constexpr FloatN k2 = 1.8446744073709552e19f;
    const FloatN d = x * select(k1, k2, m);

    constexpr IntN i1 = 0x7f;
    constexpr IntN i2 = 64 + 0x7f;
    const IntN y = ((castBit<IntN>(d) >> 23) & 0xff) - select(i1, i2, m);

    return y;
  }

  template <typename IntN, typename FloatN>
  static IntN ilogb2(const FloatN x) noexcept
  {
    const IntN y = ((castBit<IntN>(x) >> 23) & 0xff) - 0x7f;
    return y;
  }

  static constexpr float kRLn2 = 1.442695040888963407359924681001892137426645954152985934135449406931f;
  static constexpr float kL2U = 0.693145751953125f;
  static constexpr float kL2L = 1.428606765330187045e-06f;
};

/*!
  \brief The math implementation for float

  No detailed description.
  */
template <>
struct Math::Precision::Impl<float>
{
  // Type aliases
  using LimitT = NumericLimits<float>;
  using UtilT = Util<float>;

  template <typename FloatN>
  static FloatN pow(const FloatN x, const FloatN y) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    constexpr float t = static_cast<float>(1u << 24u);
    const IntN y_int = cast<IntN>(y);
    const CompResult<FloatN> y_is_int = (y == cast<FloatN>(y_int)) || (t <= UtilT::abs(y));
    const CompResult<FloatN> y_is_odd = ((1 & y_int) != 0) && y_is_int && (UtilT::abs(y) < t);

    FloatN z = UtilT::exp(UtilT::mul(UtilT::log(UtilT::abs(x)), y));

    constexpr FloatN zero = 0.0f;
    constexpr FloatN one = 1.0f;
    const FloatN infinity = LimitT::infinity();
    const FloatN nan = LimitT::quietNan();

    z = select(z, infinity, isnan(z));
    z = z * select(select(nan, select(one, -one, y_is_odd), y_is_int), one, 0.0f <= x);

    const FloatN efx = UtilT::mulsign(UtilT::abs(x) - 1.0f, y);
    z = select(z, select(select(infinity, one, efx == 0.0f), zero, efx < 0.0f), isinf(y));
    z = select(z, UtilT::mulsign(select(infinity, zero, UtilT::signbit(y) != (x == 0.0f)), select(one, x, y_is_odd)), isinf(x) || (x == 0.0f));
    z = select(z, nan, isnan(x) || isnan(y));
    z = select(z, one, (x == 1.0f) || (y == 0.0f));

    return z;
  }

  template <typename FloatN, typename IntN>
  static FloatN pown(const FloatN x, const IntN y) noexcept
  {
    return pow(x, cast<FloatN>(y));
  }

  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    constexpr FloatN one = 1.0f;
    const FloatN nan = LimitT::quietNan();
    FloatN d = select(x, nan, x < 0.0f);
    FloatN q = one;
    {
      constexpr FloatN k = 1.8889465931478580e+22f;
      constexpr FloatN new_q = 7.2759576141834260e-12f;
      const CompResult<FloatN> comp = d < 5.2939559203393770e-23f;
      d = d * select(one, k, comp);
      q = select(q, new_q, comp);
    }
    {
      constexpr FloatN k = 5.4210108624275220e-20f;
      constexpr FloatN new_q = 4294967296.0f;
      const CompResult<FloatN> comp = 1.8446744073709552e+19f < d;
      d = d * select(one, k, comp);
      q = select(q, new_q, comp);
    }

    // http://en.wikipedia.org/wiki/Fast_inverse_square_root
    FloatN y = castBit<FloatN>(0x5f375a86 - (castBit<IntN>(d + 1.0e-45f) >> 1));

    for (size_t i = 0; i < 4; ++i)
      y = y * (1.5f - 0.5f * d * y * y);
    const FloatN inf = LimitT::infinity();
    y = select(y * d * q, inf, isinf(d));
    return y;
  }

  template <typename FloatN>
  static FloatN cbrt(const FloatN x) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    const IntN e = UtilT::ilogb<IntN>(UtilT::abs(x)) + 1;
    FloatN d = UtilT::ldexp2(x, -e);
    FloatN q = 1.0f;
    {
      const IntN r = (e + 6144) % 3;
      constexpr FloatN q1 = 1.2599210498948731647672106f;
      constexpr FloatN q2 = 1.5874010519681994747517056f;
      q = select(q, q1, r == 1);
      q = select(q, q2, r == 2);
    }
    q = UtilT::ldexp2(q, (e + 6144) / 3 - 2048);

    q = UtilT::mulsign(q, d);
    d = UtilT::abs(d);

    FloatN t = -0.601564466953277587890625f;
    t = UtilT::mla<FloatN>(t, d, 2.8208892345428466796875f);
    t = UtilT::mla<FloatN>(t, d, -5.532182216644287109375f);
    t = UtilT::mla<FloatN>(t, d, 5.898262500762939453125f);
    t = UtilT::mla<FloatN>(t, d, -3.8095417022705078125f);
    t = UtilT::mla<FloatN>(t, d, 2.2241256237030029296875f);

    FloatN y = d * t * t;
    y = (y - (2.0f / 3.0f) * y * (y * t - 1.0f)) * q;

    return y;
  }
};

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fabs(const FloatN x) noexcept
{
  return Math::fabs(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fmod(const FloatN x, const FloatN y) noexcept
{
  return Math::fmod(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN remainder(const FloatN x, const FloatN y) noexcept
{
  return Math::remainder(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [out] quo No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN remquo(const FloatN x, const FloatN y, AddressSpacePointer quo) noexcept
{
  return Math::remquo(x, y, quo);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] a No description.
  \param [in] b No description.
  \param [in] c No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept
{
  return Math::fma(a, b, c);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fmax(const FloatN x, const FloatN y) noexcept
{
  return Math::fmax(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fmin(const FloatN x, const FloatN y) noexcept
{
  return Math::fmin(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN maxmag(const FloatN x, const FloatN y) noexcept
{
  return Math::maxmag(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN minmag(const FloatN x, const FloatN y) noexcept
{
  return Math::minmag(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN fdim(const FloatN x, const FloatN y) noexcept
{
  return Math::fdim(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN pow(const FloatN x, const FloatN y) noexcept
{
  return Math::pow(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN pown(const FloatN x, const IntN y) noexcept
{
  return Math::pown(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN x) noexcept
{
  return Math::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN x) noexcept
{
  return Math::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN cbrt(const FloatN x) noexcept
{
  return Math::cbrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN ceil(const FloatN x) noexcept
{
  return Math::ceil(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN floor(const FloatN x) noexcept
{
  return Math::floor(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN trunc(const FloatN x) noexcept
{
  return Math::trunc(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN round(const FloatN x) noexcept
{
  return Math::round(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN rint(const FloatN x) noexcept
{
  return Math::rint(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] exp No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN frexp(const FloatN x, AddressSpacePointer exp) noexcept
{
  return Math::frexp(x, exp);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntegerN No description.
  \param [in] x No description.
  \param [in] k No description.
  \return No description
  */
template <typename FloatN, typename IntegerN> inline
FloatN ldexp(const FloatN x, const IntegerN k) noexcept
{
  return Math::ldexp(x, k);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN modf(const FloatN x, AddressSpacePointer iptr) noexcept
{
  return Math::modf(x, iptr);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam AddressSpacePointer No description.
  \param [in] x No description.
  \param [out] iptr No description.
  \return No description
  */
template <typename FloatN, typename AddressSpacePointer> inline
FloatN fract(const FloatN x, AddressSpacePointer iptr) noexcept
{
  return Math::fract(x, iptr);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
auto ilogb(const FloatN x) noexcept
{
  return Math::ilogb(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN logb(const FloatN x) noexcept
{
  return Math::logb(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN nextafter(const FloatN x, const FloatN y) noexcept
{
  return Math::nextafter(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN copysign(const FloatN x, const FloatN y) noexcept
{
  return Math::copysign(x, y);
}

namespace native {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN pow(const FloatN x, const FloatN y) noexcept
{
  return Math::Native::pow(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN pown(const FloatN x, const IntN y) noexcept
{
  return Math::Native::pown(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN x) noexcept
{
  return Math::Native::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN x) noexcept
{
  return Math::Native::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN cbrt(const FloatN x) noexcept
{
  return Math::Native::cbrt(x);
}

} /* namespace native */

namespace precision {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN> inline
FloatN pow(const FloatN x, const FloatN y) noexcept
{
  return Math::Precision::pow(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \tparam IntN No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <typename FloatN, typename IntN> inline
FloatN pown(const FloatN x, const IntN y) noexcept
{
  return Math::Precision::pown(x, y);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN x) noexcept
{
  return Math::Precision::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN x) noexcept
{
  return Math::Precision::rsqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN cbrt(const FloatN x) noexcept
{
  return Math::Precision::cbrt(x);
}

} /* namespace precision */

} /* namespace zivc */

#endif /* ZIVC_CL_MATH_INL_HPP */
