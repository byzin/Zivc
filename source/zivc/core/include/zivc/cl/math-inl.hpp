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
//  template <typename FloatN>
//  struct V2
//  {
//    FloatN x;
//    FloatN y;
//  };

  template <typename FloatN>
  static FloatN invert(const FloatN x) noexcept
  {
    const FloatN y =  1.0f / x;
    return y;
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

//  template <typename FloatN>
//  static FloatN upper(const FloatN x) noexcept
//  {
//    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
//    using UIntN = UInteger32VecT<VecInfo::size()>;
//
//    const FloatN y = castBit<FloatN>(castBit<UIntN>(x) & 0xfffff000u);
//    return y;
//  }

//  template <typename FloatN>
//  static V2<FloatN> addV2(const FloatN x, const V2<FloatN> y) noexcept
//  {
//    const FloatN rx = x + y.x;
//    const FloatN v = rx - x;
//    const FloatN ry = (x - (rx - v)) + (y.x - v) + y.y;
//
//    return {rx, ry};
//  }
//
//  template <typename FloatN>
//  static V2<FloatN> mulV2(const FloatN x, const FloatN y) noexcept
//  {
//    const FloatN xh = upper(x);
//    const FloatN xl = x - xh;
//    const FloatN yh = upper(y);
//    const FloatN yl = y - yh;
//
//    const FloatN rx = x * y;
//    const FloatN ry = xh * yh - rx + xl * yh + xh * yl + xl * yl;
//
//    return {rx, ry};
//  }
//
//  template <typename FloatN>
//  static V2<FloatN> mulV2(const V2<FloatN> x, const V2<FloatN> y) noexcept
//  {
//    const FloatN xh = upper(x.x);
//    const FloatN xl = x.x - xh;
//    const FloatN yh = upper(y.x);
//    const FloatN yl = y.x - yh;
//
//    const FloatN rx = x.x * y.x;
//    const FloatN ry = xh * yh - rx + xl * yh + xh * yl + xl * yl + x.x * y.y + x.y * y.x;
//
//    return {rx, ry};
//  }
//
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
  static FloatN mulsign(const FloatN x, const FloatN y) noexcept
  {
    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using UIntN = UInteger32VecT<VecInfo::size()>;

    constexpr UIntN k = 0b01u << 31u;
    const FloatN z = castBit<FloatN>(castBit<UIntN>(x) ^ (castBit<UIntN>(y) & k));
    return z;
  }

  template <typename FloatN, typename IntN>
  static FloatN pow2(const IntN x) noexcept
  {
    const FloatN y = castBit<FloatN>((x + 0x7f) << 23);
    return y;
  }

  template <typename FloatN, typename IntN>
  static FloatN ldexp2(const FloatN x, const IntN e) noexcept
  {
    const FloatN y = x * pow2<FloatN>(e >> 1) * pow2<FloatN>(e - (e >> 1));
    return y;
  }

  template <typename IntN, typename FloatN>
  static IntN ilogb(const FloatN x) noexcept
  {
    const CompResult<FloatN> m = x < 5.421010862427522e-20f;
    const FloatN d = select(x, x * 1.8446744073709552e19f, m);
    IntN q = (castBit<IntN>(d) >> 23) & 0xff;
    q = select(q - 0x7f, q - (64 + 0x7f), m);
    return q;
  }
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

  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept
  {
    const FloatN nan = LimitT::quietNan();
    FloatN d = select(x, nan, x < 0.0f);
    FloatN q = 1.0f;
    {
      const FloatN new_d = 1.8889465931478580e+22f * d;
      constexpr FloatN new_q = 7.2759576141834260e-12f;
      const CompResult<FloatN> comp = d < 5.2939559203393770e-23f;
      d = select(d, new_d, comp);
      q = select(q, new_q, comp);
    }
    {
      const FloatN new_d = 5.4210108624275220e-20f * d;
      constexpr FloatN new_q = 4294967296.0f;
      const CompResult<FloatN> comp = 1.8446744073709552e+19f < d;
      d = select(d, new_d, comp);
      q = select(q, new_q, comp);
    }

    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
    using IntN = Integer32VecT<VecInfo::size()>;

    // http://en.wikipedia.org/wiki/Fast_inverse_square_root
    FloatN y = castBit<FloatN>(0x5f375a86 - (castBit<IntN>(d + 1.0e-45f) >> 1));

    for (size_t i = 0; i < 4; ++i)
      y = y * (1.5f - 0.5f * d * y * y);
    const FloatN inf = LimitT::infinity();
    y = select(y * d * q, inf, isinf(d));
    return y;
  }

  //! Compute square root
//  template <typename FloatN>
//  static FloatN sqrt(const FloatN x) noexcept
//  {
//    using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
//    using IntN = Integer32VecT<VecInfo::size()>;
//
//    const FloatN nan = LimitT::quietNan();
//    FloatN d = select(x, nan, x < 0.0f);
//    FloatN q = 0.5f;
//    {
//      const FloatN new_d = 1.8889465931478580e+22f * d;
//      constexpr FloatN new_q = 7.2759576141834260e-12f * 0.5f;
//      const CompResult<FloatN> comp = d < 5.2939559203393770e-23f;
//      d = select(d, new_d, comp);
//      q = select(q, new_q, comp);
//    }
//    {
//      const FloatN new_d = 5.4210108624275220e-20f * d;
//      constexpr FloatN new_q = 4294967296.0f * 0.5f;
//      const CompResult<FloatN> comp = 1.8446744073709552e+19f < d;
//      d = select(d, new_d, comp);
//      q = select(q, new_q, comp);
//    }
//
//    // http://en.wikipedia.org/wiki/Fast_inverse_square_root
//    FloatN y = castBit<FloatN>(0x5f375a86 - (castBit<IntN>(d + 1.0e-45f) >> 1));
//
//    y = y * (1.5f - 0.5f * d * y * y);
//    y = y * (1.5f - 0.5f * d * y * y);
//    y = y * (1.5f - 0.5f * d * y * y) * d;
//
//    const UtilT::V2<FloatN> d2 = UtilT::mulV2<FloatN>(UtilT::addV2(d, UtilT::mulV2(y, y)),
//                                                      UtilT::frecV2(y));
//    y = (d2.x + d2.y) * q;
//
//    const FloatN inf = LimitT::infinity();
//    y = select(y, inf, isinf(d));
//    y = select(y, d, d == 0.0f);
//
//    return y;
//  }

  //! Compute cube root
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

namespace native {

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
