/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_VECTOR_INL_HPP
#define ZIVC_CPUCL_VECTOR_INL_HPP

#include "vector.hpp"
// Standard C++ library
#include <array>
#include <concepts>
#include <cstddef>
#include <functional>
#include <span>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/ieee_754_binary.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "types.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \def ZIVC_CPUCL_VECTOR_TYPE_CHECK
  \brief No brief description

  No detailed description.

  \param [in] type No description.
  */
#define ZIVC_CPUCL_VECTOR_TYPE_CHECK(type) \
    static_assert(std::is_trivially_copyable_v< type >, \
                  "The vector type isn't trivially copyable."); \
    static_assert(std::is_trivial_v< type >, \
                  "The vector type isn't trivial."); \
    static_assert(std::equality_comparable< type >, \
                  "The vector type isn't equality comparable.")

ZIVC_CPUCL_VECTOR_TYPE_CHECK(char2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(char3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(char4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uchar2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uchar3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uchar4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(short2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(short3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(short4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ushort2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ushort3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ushort4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(int2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(int3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(int4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uint2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uint3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(uint4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(long2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(long3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(long4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ulong2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ulong3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(ulong4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(half2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(half3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(half4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(float2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(float3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(float4);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(double2);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(double3);
ZIVC_CPUCL_VECTOR_TYPE_CHECK(double4);


/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <Arithmetic Type>
struct CompResultType<Type>
{
  using IntegerT = int32b;
  using ResultT = IntegerT;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam kN No description.
  */
template <Arithmetic Type, std::size_t kN>
struct CompResultType<Vector<Type, kN>>
{
  using IntegerT = std::conditional_t<sizeof(Type) == 1, int8b,
                   std::conditional_t<sizeof(Type) == 2, int16b,
                   std::conditional_t<sizeof(Type) == 4, int32b,
                                                         int64b>>>;
  using ResultT = Vector<IntegerT, kN>;
};

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 2>::Vector(ConstReference v) noexcept : Vector(v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 2>::Vector(ConstReference v0, ConstReference v1) noexcept :
    x{v0},
    y{v1}
{
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 2>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(x) && static_cast<bool>(y);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::alignedThis() noexcept -> Vector*
{
  Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::alignedThis() const noexcept -> const Vector*
{
  const Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::alignment() noexcept -> size_type
{
  constexpr size_type a = std::alignment_of_v<Vector>;
  static_assert(a == sizeof(Vector), "The alignment isn't correct.");
  return a;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::size() noexcept -> size_type
{
  constexpr std::size_t s = 2;
  static_assert(s * sizeof(Type) == sizeof(Vector), "The size isn't correct.");
  return s;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, zisc::InvocableR<R, T> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x);
    rptr->y = func(xptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector<T1, 2>& y) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector<T1, 2>* yptr = y.alignedThis();
    rptr->x = func(xptr->x, yptr->x);
    rptr->y = func(xptr->y, yptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           Vector<T1, 2>* y) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    Vector<T1, 2>* yptr = y->alignedThis();
    rptr->x = func(xptr->x, &yptr->x);
    rptr->y = func(xptr->y, &yptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const T1& x,
                                           const Vector& y) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* yptr = y.alignedThis();
    rptr->x = func(x, yptr->x);
    rptr->y = func(x, yptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y);
    rptr->y = func(xptr->y, y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           const Vector<T1, 2>& z) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    const Vector<T1, 2>* zptr = z.alignedThis();
    rptr->x = func(xptr->x, yptr->x, zptr->x);
    rptr->y = func(xptr->y, yptr->y, zptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in,out] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           Vector<T1, 2>* z) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    Vector<T1, 2>* zptr = z->alignedThis();
    rptr->x = func(xptr->x, yptr->x, &zptr->x);
    rptr->y = func(xptr->y, yptr->y, &zptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const T1& x,
                                           const T1& y,
                                           const Vector& z) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* zptr = z.alignedThis();
    rptr->x = func(x, y, zptr->x);
    rptr->y = func(x, y, zptr->y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func> inline
constexpr Vector<R, 2> Vector<T, 2>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y,
                                           const T1& z) noexcept
{
  using ReturnVecT = Vector<R, 2>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y, z);
    rptr->y = func(xptr->y, y, z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] data No description.
  \return No description
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr auto Vector<T, 2>::load(const std::span<T1> data) noexcept -> Vector
{
  Vector result{};
  {
    Vector* rptr = result.alignedThis();
    rptr->x = static_cast<Type>(data[0]);
    rptr->y = static_cast<Type>(data[1]);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] v No description.
  \param [out] data No description.
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr void Vector<T, 2>::store(const Vector& v, std::span<T1> data) noexcept
{
  const Vector* vptr = v.alignedThis();
  data[0] = static_cast<T1>(vptr->x);
  data[1] = static_cast<T1>(vptr->y);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::sum() const noexcept -> Type
{
  const Vector* ptr = alignedThis();
  const Type s = ptr->x + ptr->y;
  return s;
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 3>::Vector(ConstReference v) noexcept : Vector(v, v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 3>::Vector(ConstReference v0, ConstReference v1, ConstReference v2) noexcept :
    x{v0},
    y{v1},
    z{v2},
    pad_{static_cast<Type>(0.0)}
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 3>::Vector(const Vector2& v0, ConstReference v1) noexcept :
    Vector(v0.x, v0.y, v1)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 3>::Vector(ConstReference v0, const Vector2& v1) noexcept :
    Vector(v0, v1.x, v1.y)
{
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 3>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(x) && static_cast<bool>(y) &&
                      static_cast<bool>(z);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::alignedThis() noexcept -> Vector*
{
  Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::alignedThis() const noexcept -> const Vector*
{
  const Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::alignment() noexcept -> size_type
{
  constexpr size_type a = std::alignment_of_v<Vector>;
  static_assert(a == sizeof(Vector), "The alignment isn't correct.");
  return a;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::size() noexcept -> size_type
{
  return 3;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, zisc::InvocableR<R, T> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x);
    rptr->y = func(xptr->y);
    rptr->z = func(xptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector<T1, 3>& y) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector<T1, 3>* yptr = y.alignedThis();
    rptr->x = func(xptr->x, yptr->x);
    rptr->y = func(xptr->y, yptr->y);
    rptr->z = func(xptr->z, yptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           Vector<T1, 3>* y) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    Vector<T1, 3>* yptr = y->alignedThis();
    rptr->x = func(xptr->x, &yptr->x);
    rptr->y = func(xptr->y, &yptr->y);
    rptr->z = func(xptr->z, &yptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const T1& x,
                                           const Vector& y) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* yptr = y.alignedThis();
    rptr->x = func(x, yptr->x);
    rptr->y = func(x, yptr->y);
    rptr->z = func(x, yptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y);
    rptr->y = func(xptr->y, y);
    rptr->z = func(xptr->z, y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           const Vector<T1, 3>& z) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    const Vector<T1, 3>* zptr = z.alignedThis();
    rptr->x = func(xptr->x, yptr->x, zptr->x);
    rptr->y = func(xptr->y, yptr->y, zptr->y);
    rptr->z = func(xptr->z, yptr->z, zptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in,out] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           Vector<T1, 3>* z) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    Vector<T1, 3>* zptr = z->alignedThis();
    rptr->x = func(xptr->x, yptr->x, &zptr->x);
    rptr->y = func(xptr->y, yptr->y, &zptr->y);
    rptr->z = func(xptr->z, yptr->z, &zptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const T1& x,
                                           const T1& y,
                                           const Vector& z) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* zptr = z.alignedThis();
    rptr->x = func(x, y, zptr->x);
    rptr->y = func(x, y, zptr->y);
    rptr->z = func(x, y, zptr->z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func> inline
constexpr Vector<R, 3> Vector<T, 3>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y,
                                           const T1& z) noexcept
{
  using ReturnVecT = Vector<R, 3>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y, z);
    rptr->y = func(xptr->y, y, z);
    rptr->z = func(xptr->z, y, z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] data No description.
  \return No description
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr auto Vector<T, 3>::load(const std::span<T1> data) noexcept -> Vector
{
  Vector result{};
  {
    Vector* rptr = result.alignedThis();
    rptr->x = static_cast<Type>(data[0]);
    rptr->y = static_cast<Type>(data[1]);
    rptr->z = static_cast<Type>(data[2]);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] v No description.
  \param [out] data No description.
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr void Vector<T, 3>::store(const Vector& v, std::span<T1> data) noexcept
{
  const Vector* vptr = v.alignedThis();
  data[0] = static_cast<T1>(vptr->x);
  data[1] = static_cast<T1>(vptr->y);
  data[2] = static_cast<T1>(vptr->z);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::sum() const noexcept -> Type
{
  const Vector* ptr = alignedThis();
  const Type s = ptr->x + ptr->y + ptr->z;
  return s;
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(ConstReference v) noexcept : Vector(v, v, v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(ConstReference v0, ConstReference v1, ConstReference v2, ConstReference v3) noexcept :
    x{v0},
    y{v1},
    z{v2},
    w{v3}
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(const Vector2& v0, ConstReference v1, ConstReference v2) noexcept :
    Vector(v0.x, v0.y, v1, v2)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(ConstReference v0, const Vector2& v1, ConstReference v2) noexcept :
    Vector(v0, v1.x, v1.y, v2)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(ConstReference v0, ConstReference v1, const Vector2& v2) noexcept :
    Vector(v0, v1, v2.x, v2.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(const Vector2& v0, const Vector2& v1) noexcept :
    Vector(v0.x, v0.y, v1.x, v1.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(const Vector3& v0, ConstReference v1) noexcept :
    Vector(v0.x, v0.y, v0.z, v1)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::Vector(ConstReference v0, const Vector3& v1) noexcept :
    Vector(v0, v1.x, v1.y, v1.z)
{
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(x) && static_cast<bool>(y) &&
                      static_cast<bool>(z) && static_cast<bool>(w);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::alignedThis() noexcept -> Vector*
{
  Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::alignedThis() const noexcept -> const Vector*
{
  const Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::alignment() noexcept -> size_type
{
  constexpr size_type a = std::alignment_of_v<Vector>;
  static_assert(a == sizeof(Vector), "The alignment isn't correct.");
  return a;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::size() noexcept -> size_type
{
  constexpr std::size_t s = 4;
  static_assert(s * sizeof(Type) == sizeof(Vector), "The size isn't correct.");
  return s;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, zisc::InvocableR<R, T> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x);
    rptr->y = func(xptr->y);
    rptr->z = func(xptr->z);
    rptr->w = func(xptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector<T1, 4>& y) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector<T1, 4>* yptr = y.alignedThis();
    rptr->x = func(xptr->x, yptr->x);
    rptr->y = func(xptr->y, yptr->y);
    rptr->z = func(xptr->z, yptr->z);
    rptr->w = func(xptr->w, yptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           Vector<T1, 4>* y) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    Vector<T1, 4>* yptr = y->alignedThis();
    rptr->x = func(xptr->x, &yptr->x);
    rptr->y = func(xptr->y, &yptr->y);
    rptr->z = func(xptr->z, &yptr->z);
    rptr->w = func(xptr->w, &yptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const T1& x,
                                           const Vector& y) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* yptr = y.alignedThis();
    rptr->x = func(x, yptr->x);
    rptr->y = func(x, yptr->y);
    rptr->z = func(x, yptr->z);
    rptr->w = func(x, yptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y);
    rptr->y = func(xptr->y, y);
    rptr->z = func(xptr->z, y);
    rptr->w = func(xptr->w, y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           const Vector<T1, 4>& z) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    const Vector<T1, 4>* zptr = z.alignedThis();
    rptr->x = func(xptr->x, yptr->x, zptr->x);
    rptr->y = func(xptr->y, yptr->y, zptr->y);
    rptr->z = func(xptr->z, yptr->z, zptr->z);
    rptr->w = func(xptr->w, yptr->w, zptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in,out] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           Vector<T1, 4>* z) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    Vector<T1, 4>* zptr = z->alignedThis();
    rptr->x = func(xptr->x, yptr->x, &zptr->x);
    rptr->y = func(xptr->y, yptr->y, &zptr->y);
    rptr->z = func(xptr->z, yptr->z, &zptr->z);
    rptr->w = func(xptr->w, yptr->w, &zptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const T1& x,
                                           const T1& y,
                                           const Vector& z) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* zptr = z.alignedThis();
    rptr->x = func(x, y, zptr->x);
    rptr->y = func(x, y, zptr->y);
    rptr->z = func(x, y, zptr->z);
    rptr->w = func(x, y, zptr->w);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func> inline
constexpr Vector<R, 4> Vector<T, 4>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y,
                                           const T1& z) noexcept
{
  using ReturnVecT = Vector<R, 4>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->x = func(xptr->x, y, z);
    rptr->y = func(xptr->y, y, z);
    rptr->z = func(xptr->z, y, z);
    rptr->w = func(xptr->w, y, z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] data No description.
  \return No description
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr auto Vector<T, 4>::load(const std::span<T1> data) noexcept -> Vector
{
  Vector result{};
  {
    Vector* rptr = result.alignedThis();
    rptr->x = static_cast<Type>(data[0]);
    rptr->y = static_cast<Type>(data[1]);
    rptr->z = static_cast<Type>(data[2]);
    rptr->w = static_cast<Type>(data[3]);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] v No description.
  \param [out] data No description.
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr void Vector<T, 4>::store(const Vector& v, std::span<T1> data) noexcept
{
  const Vector* vptr = v.alignedThis();
  data[0] = static_cast<T1>(vptr->x);
  data[1] = static_cast<T1>(vptr->y);
  data[2] = static_cast<T1>(vptr->z);
  data[3] = static_cast<T1>(vptr->w);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::sum() const noexcept -> Type
{
  const Vector* ptr = alignedThis();
  const Type s = ptr->x + ptr->y + ptr->z + ptr->w;
  return s;
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 8>::Vector(ConstReference v) noexcept :
    Vector(v, v, v, v, v, v, v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  \param [in] v4 No description.
  \param [in] v5 No description.
  \param [in] v6 No description.
  \param [in] v7 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 8>::Vector(ConstReference v0, ConstReference v1,
                               ConstReference v2, ConstReference v3,
                               ConstReference v4, ConstReference v5,
                               ConstReference v6, ConstReference v7) noexcept :
    s0{v0},
    s1{v1},
    s2{v2},
    s3{v3},
    s4{v4},
    s5{v5},
    s6{v6},
    s7{v7}
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 8>::Vector(const Vector2& v0, const Vector2& v1,
                               const Vector2& v2, const Vector2& v3) noexcept :
    Vector(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 8>::Vector(const Vector4& v0, const Vector4& v1) noexcept :
    Vector(v0.x, v0.y, v0.w, v0.z, v1.x, v1.y, v1.z, v1.w)
{
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 8>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(s0) && static_cast<bool>(s1) &&
                      static_cast<bool>(s2) && static_cast<bool>(s3) &&
                      static_cast<bool>(s4) && static_cast<bool>(s5) &&
                      static_cast<bool>(s6) && static_cast<bool>(s7);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 8>::alignedThis() noexcept -> Vector*
{
  Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 8>::alignedThis() const noexcept -> const Vector*
{
  const Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 8>::alignment() noexcept -> size_type
{
  constexpr size_type a = std::alignment_of_v<Vector>;
  static_assert(a == sizeof(Vector), "The alignment isn't correct.");
  return a;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 8>::size() noexcept -> size_type
{
  constexpr std::size_t s = 8;
  static_assert(s * sizeof(Type) == sizeof(Vector), "The size isn't correct.");
  return s;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, zisc::InvocableR<R, T> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0);
    rptr->s1 = func(xptr->s1);
    rptr->s2 = func(xptr->s2);
    rptr->s3 = func(xptr->s3);
    rptr->s4 = func(xptr->s4);
    rptr->s5 = func(xptr->s5);
    rptr->s6 = func(xptr->s6);
    rptr->s7 = func(xptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector<T1, 8>& y) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector<T1, 8>* yptr = y.alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           Vector<T1, 8>* y) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    Vector<T1, 8>* yptr = y->alignedThis();
    rptr->s0 = func(xptr->s0, &yptr->s0);
    rptr->s1 = func(xptr->s1, &yptr->s1);
    rptr->s2 = func(xptr->s2, &yptr->s2);
    rptr->s3 = func(xptr->s3, &yptr->s3);
    rptr->s4 = func(xptr->s4, &yptr->s4);
    rptr->s5 = func(xptr->s5, &yptr->s5);
    rptr->s6 = func(xptr->s6, &yptr->s6);
    rptr->s7 = func(xptr->s7, &yptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const T1& x,
                                           const Vector& y) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* yptr = y.alignedThis();
    rptr->s0 = func(x, yptr->s0);
    rptr->s1 = func(x, yptr->s1);
    rptr->s2 = func(x, yptr->s2);
    rptr->s3 = func(x, yptr->s3);
    rptr->s4 = func(x, yptr->s4);
    rptr->s5 = func(x, yptr->s5);
    rptr->s6 = func(x, yptr->s6);
    rptr->s7 = func(x, yptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0, y);
    rptr->s1 = func(xptr->s1, y);
    rptr->s2 = func(xptr->s2, y);
    rptr->s3 = func(xptr->s3, y);
    rptr->s4 = func(xptr->s4, y);
    rptr->s5 = func(xptr->s5, y);
    rptr->s6 = func(xptr->s6, y);
    rptr->s7 = func(xptr->s7, y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           const Vector<T1, 8>& z) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    const Vector<T1, 8>* zptr = z.alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0, zptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1, zptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2, zptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3, zptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4, zptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5, zptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6, zptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7, zptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in,out] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           const Vector& y,
                                           Vector<T1, 8>* z) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    Vector<T1, 8>* zptr = z->alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0, &zptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1, &zptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2, &zptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3, &zptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4, &zptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5, &zptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6, &zptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7, &zptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const T1& x,
                                           const T1& y,
                                           const Vector& z) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* zptr = z.alignedThis();
    rptr->s0 = func(x, y, zptr->s0);
    rptr->s1 = func(x, y, zptr->s1);
    rptr->s2 = func(x, y, zptr->s2);
    rptr->s3 = func(x, y, zptr->s3);
    rptr->s4 = func(x, y, zptr->s4);
    rptr->s5 = func(x, y, zptr->s5);
    rptr->s6 = func(x, y, zptr->s6);
    rptr->s7 = func(x, y, zptr->s7);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func> inline
constexpr Vector<R, 8> Vector<T, 8>::apply(const Func& func,
                                           const Vector& x,
                                           const T1& y,
                                           const T1& z) noexcept
{
  using ReturnVecT = Vector<R, 8>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0, y, z);
    rptr->s1 = func(xptr->s1, y, z);
    rptr->s2 = func(xptr->s2, y, z);
    rptr->s3 = func(xptr->s3, y, z);
    rptr->s4 = func(xptr->s4, y, z);
    rptr->s5 = func(xptr->s5, y, z);
    rptr->s6 = func(xptr->s6, y, z);
    rptr->s7 = func(xptr->s7, y, z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] data No description.
  \return No description
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr auto Vector<T, 8>::load(const std::span<T1> data) noexcept -> Vector
{
  Vector result{};
  {
    Vector* rptr = result.alignedThis();
    rptr->s0 = static_cast<Type>(data[0]);
    rptr->s1 = static_cast<Type>(data[1]);
    rptr->s2 = static_cast<Type>(data[2]);
    rptr->s3 = static_cast<Type>(data[3]);
    rptr->s4 = static_cast<Type>(data[4]);
    rptr->s5 = static_cast<Type>(data[5]);
    rptr->s6 = static_cast<Type>(data[6]);
    rptr->s7 = static_cast<Type>(data[7]);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] v No description.
  \param [out] data No description.
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr void Vector<T, 8>::store(const Vector& v, std::span<T1> data) noexcept
{
  const Vector* vptr = v.alignedThis();
  data[0] = static_cast<T1>(vptr->s0);
  data[1] = static_cast<T1>(vptr->s1);
  data[2] = static_cast<T1>(vptr->s2);
  data[3] = static_cast<T1>(vptr->s3);
  data[4] = static_cast<T1>(vptr->s4);
  data[5] = static_cast<T1>(vptr->s5);
  data[6] = static_cast<T1>(vptr->s6);
  data[7] = static_cast<T1>(vptr->s7);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 8>::sum() const noexcept -> Type
{
  const Vector* ptr = alignedThis();
  const Type s = ptr->s0 + ptr->s1 + ptr->s2 + ptr->s3 +
                 ptr->s4 + ptr->s5 + ptr->s6 + ptr->s7;
  return s;
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::Vector(ConstReference v) noexcept :
    Vector(v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  \param [in] v4 No description.
  \param [in] v5 No description.
  \param [in] v6 No description.
  \param [in] v7 No description.
  \param [in] v8 No description.
  \param [in] v9 No description.
  \param [in] va No description.
  \param [in] vb No description.
  \param [in] vc No description.
  \param [in] vd No description.
  \param [in] ve No description.
  \param [in] vf No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::Vector(ConstReference v0, ConstReference v1,
                                ConstReference v2, ConstReference v3,
                                ConstReference v4, ConstReference v5,
                                ConstReference v6, ConstReference v7,
                                ConstReference v8, ConstReference v9,
                                ConstReference va, ConstReference vb,
                                ConstReference vc, ConstReference vd,
                                ConstReference ve, ConstReference vf) noexcept :
    s0{v0},
    s1{v1},
    s2{v2},
    s3{v3},
    s4{v4},
    s5{v5},
    s6{v6},
    s7{v7},
    s8{v8},
    s9{v9},
    sa{va},
    sb{vb},
    sc{vc},
    sd{vd},
    se{ve},
    sf{vf}
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  \param [in] v4 No description.
  \param [in] v5 No description.
  \param [in] v6 No description.
  \param [in] v7 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::Vector(const Vector2& v0, const Vector2& v1,
                                const Vector2& v2, const Vector2& v3,
                                const Vector2& v4, const Vector2& v5,
                                const Vector2& v6, const Vector2& v7) noexcept :
    Vector(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y,
           v4.x, v4.y, v5.x, v5.y, v6.x, v6.y, v7.x, v7.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  \param [in] v3 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::Vector(const Vector4& v0, const Vector4& v1,
                                const Vector4& v2, const Vector4& v3) noexcept :
    Vector(v0.x, v0.y, v0.z, v0.w, v1.x, v1.y, v1.z, v1.w,
           v2.x, v2.y, v2.z, v2.w, v3.x, v3.y, v3.z, v3.w)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::Vector(const Vector8& v0, const Vector8& v1) noexcept :
    Vector(v0.s0, v0.s1, v0.s2, v0.s3, v0.s4, v0.s5, v0.s6, v0.s7,
           v1.s0, v1.s1, v1.s2, v1.s3, v1.s4, v1.s5, v1.s6, v1.s7)
{
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 16>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(s0) && static_cast<bool>(s1) &&
                      static_cast<bool>(s2) && static_cast<bool>(s3) &&
                      static_cast<bool>(s4) && static_cast<bool>(s5) &&
                      static_cast<bool>(s6) && static_cast<bool>(s7) &&
                      static_cast<bool>(s8) && static_cast<bool>(s9) &&
                      static_cast<bool>(sa) && static_cast<bool>(sb) &&
                      static_cast<bool>(sc) && static_cast<bool>(sd) &&
                      static_cast<bool>(se) && static_cast<bool>(sf);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 16>::alignedThis() noexcept -> Vector*
{
  Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 16>::alignedThis() const noexcept -> const Vector*
{
  const Vector* p = std::is_constant_evaluated()
      ? this
      : zisc::assume_aligned<alignment()>(this);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 16>::alignment() noexcept -> size_type
{
  constexpr size_type a = std::alignment_of_v<Vector>;
  static_assert(a == sizeof(Vector), "The alignment isn't correct.");
  return a;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 16>::size() noexcept -> size_type
{
  constexpr std::size_t s = 16;
  static_assert(s * sizeof(Type) == sizeof(Vector), "The size isn't correct.");
  return s;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, zisc::InvocableR<R, T> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0);
    rptr->s1 = func(xptr->s1);
    rptr->s2 = func(xptr->s2);
    rptr->s3 = func(xptr->s3);
    rptr->s4 = func(xptr->s4);
    rptr->s5 = func(xptr->s5);
    rptr->s6 = func(xptr->s6);
    rptr->s7 = func(xptr->s7);
    rptr->s8 = func(xptr->s8);
    rptr->s9 = func(xptr->s9);
    rptr->sa = func(xptr->sa);
    rptr->sb = func(xptr->sb);
    rptr->sc = func(xptr->sc);
    rptr->sd = func(xptr->sd);
    rptr->se = func(xptr->se);
    rptr->sf = func(xptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             const Vector<T1, 16>& y) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector<T1, 16>* yptr = y.alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7);
    rptr->s8 = func(xptr->s8, yptr->s8);
    rptr->s9 = func(xptr->s9, yptr->s9);
    rptr->sa = func(xptr->sa, yptr->sa);
    rptr->sb = func(xptr->sb, yptr->sb);
    rptr->sc = func(xptr->sc, yptr->sc);
    rptr->sd = func(xptr->sd, yptr->sd);
    rptr->se = func(xptr->se, yptr->se);
    rptr->sf = func(xptr->sf, yptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             Vector<T1, 16>* y) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    Vector<T1, 16>* yptr = y->alignedThis();
    rptr->s0 = func(xptr->s0, &yptr->s0);
    rptr->s1 = func(xptr->s1, &yptr->s1);
    rptr->s2 = func(xptr->s2, &yptr->s2);
    rptr->s3 = func(xptr->s3, &yptr->s3);
    rptr->s4 = func(xptr->s4, &yptr->s4);
    rptr->s5 = func(xptr->s5, &yptr->s5);
    rptr->s6 = func(xptr->s6, &yptr->s6);
    rptr->s7 = func(xptr->s7, &yptr->s7);
    rptr->s8 = func(xptr->s8, &yptr->s8);
    rptr->s9 = func(xptr->s9, &yptr->s9);
    rptr->sa = func(xptr->sa, &yptr->sa);
    rptr->sb = func(xptr->sb, &yptr->sb);
    rptr->sc = func(xptr->sc, &yptr->sc);
    rptr->sd = func(xptr->sd, &yptr->sd);
    rptr->se = func(xptr->se, &yptr->se);
    rptr->sf = func(xptr->sf, &yptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                           const T1& x,
                                           const Vector& y) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* yptr = y.alignedThis();
    rptr->s0 = func(x, yptr->s0);
    rptr->s1 = func(x, yptr->s1);
    rptr->s2 = func(x, yptr->s2);
    rptr->s3 = func(x, yptr->s3);
    rptr->s4 = func(x, yptr->s4);
    rptr->s5 = func(x, yptr->s5);
    rptr->s6 = func(x, yptr->s6);
    rptr->s7 = func(x, yptr->s7);
    rptr->s8 = func(x, yptr->s8);
    rptr->s9 = func(x, yptr->s9);
    rptr->sa = func(x, yptr->sa);
    rptr->sb = func(x, yptr->sb);
    rptr->sc = func(x, yptr->sc);
    rptr->sd = func(x, yptr->sd);
    rptr->se = func(x, yptr->se);
    rptr->sf = func(x, yptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             const T1& y) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0, y);
    rptr->s1 = func(xptr->s1, y);
    rptr->s2 = func(xptr->s2, y);
    rptr->s3 = func(xptr->s3, y);
    rptr->s4 = func(xptr->s4, y);
    rptr->s5 = func(xptr->s5, y);
    rptr->s6 = func(xptr->s6, y);
    rptr->s7 = func(xptr->s7, y);
    rptr->s8 = func(xptr->s8, y);
    rptr->s9 = func(xptr->s9, y);
    rptr->sa = func(xptr->sa, y);
    rptr->sb = func(xptr->sb, y);
    rptr->sc = func(xptr->sc, y);
    rptr->sd = func(xptr->sd, y);
    rptr->se = func(xptr->se, y);
    rptr->sf = func(xptr->sf, y);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             const Vector& y,
                                             const Vector<T1, 16>& z) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    const Vector<T1, 16>* zptr = z.alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0, zptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1, zptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2, zptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3, zptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4, zptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5, zptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6, zptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7, zptr->s7);
    rptr->s8 = func(xptr->s8, yptr->s8, zptr->s8);
    rptr->s9 = func(xptr->s9, yptr->s9, zptr->s9);
    rptr->sa = func(xptr->sa, yptr->sa, zptr->sa);
    rptr->sb = func(xptr->sb, yptr->sb, zptr->sb);
    rptr->sc = func(xptr->sc, yptr->sc, zptr->sc);
    rptr->sd = func(xptr->sd, yptr->sd, zptr->sd);
    rptr->se = func(xptr->se, yptr->se, zptr->se);
    rptr->sf = func(xptr->sf, yptr->sf, zptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in,out] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             const Vector& y,
                                             Vector<T1, 16>* z) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    const Vector* yptr = y.alignedThis();
    Vector<T1, 16>* zptr = z->alignedThis();
    rptr->s0 = func(xptr->s0, yptr->s0, &zptr->s0);
    rptr->s1 = func(xptr->s1, yptr->s1, &zptr->s1);
    rptr->s2 = func(xptr->s2, yptr->s2, &zptr->s2);
    rptr->s3 = func(xptr->s3, yptr->s3, &zptr->s3);
    rptr->s4 = func(xptr->s4, yptr->s4, &zptr->s4);
    rptr->s5 = func(xptr->s5, yptr->s5, &zptr->s5);
    rptr->s6 = func(xptr->s6, yptr->s6, &zptr->s6);
    rptr->s7 = func(xptr->s7, yptr->s7, &zptr->s7);
    rptr->s8 = func(xptr->s8, yptr->s8, &zptr->s8);
    rptr->s9 = func(xptr->s9, yptr->s9, &zptr->s9);
    rptr->sa = func(xptr->sa, yptr->sa, &zptr->sa);
    rptr->sb = func(xptr->sb, yptr->sb, &zptr->sb);
    rptr->sc = func(xptr->sc, yptr->sc, &zptr->sc);
    rptr->sd = func(xptr->sd, yptr->sd, &zptr->sd);
    rptr->se = func(xptr->se, yptr->se, &zptr->se);
    rptr->sf = func(xptr->sf, yptr->sf, &zptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const T1& x,
                                             const T1& y,
                                             const Vector& z) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* zptr = z.alignedThis();
    rptr->s0 = func(x, y, zptr->s0);
    rptr->s1 = func(x, y, zptr->s1);
    rptr->s2 = func(x, y, zptr->s2);
    rptr->s3 = func(x, y, zptr->s3);
    rptr->s4 = func(x, y, zptr->s4);
    rptr->s5 = func(x, y, zptr->s5);
    rptr->s6 = func(x, y, zptr->s6);
    rptr->s7 = func(x, y, zptr->s7);
    rptr->s8 = func(x, y, zptr->s8);
    rptr->s9 = func(x, y, zptr->s9);
    rptr->sa = func(x, y, zptr->sa);
    rptr->sb = func(x, y, zptr->sb);
    rptr->sc = func(x, y, zptr->sc);
    rptr->sd = func(x, y, zptr->sd);
    rptr->se = func(x, y, zptr->se);
    rptr->sf = func(x, y, zptr->sf);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Func No description.
  \param [in] func No description.
  \param [in] x No description.
  \param [in] y No description.
  \param [in] z No description.
  \return No description
  */
template <Arithmetic T>
template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func> inline
constexpr Vector<R, 16> Vector<T, 16>::apply(const Func& func,
                                             const Vector& x,
                                             const T1& y,
                                             const T1& z) noexcept
{
  using ReturnVecT = Vector<R, 16>;
  ReturnVecT result{};
  {
    ReturnVecT* rptr = result.alignedThis();
    const Vector* xptr = x.alignedThis();
    rptr->s0 = func(xptr->s0, y, z);
    rptr->s1 = func(xptr->s1, y, z);
    rptr->s2 = func(xptr->s2, y, z);
    rptr->s3 = func(xptr->s3, y, z);
    rptr->s4 = func(xptr->s4, y, z);
    rptr->s5 = func(xptr->s5, y, z);
    rptr->s6 = func(xptr->s6, y, z);
    rptr->s7 = func(xptr->s7, y, z);
    rptr->s8 = func(xptr->s8, y, z);
    rptr->s9 = func(xptr->s9, y, z);
    rptr->sa = func(xptr->sa, y, z);
    rptr->sb = func(xptr->sb, y, z);
    rptr->sc = func(xptr->sc, y, z);
    rptr->sd = func(xptr->sd, y, z);
    rptr->se = func(xptr->se, y, z);
    rptr->sf = func(xptr->sf, y, z);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] data No description.
  \return No description
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr auto Vector<T, 16>::load(const std::span<T1> data) noexcept -> Vector
{
  Vector result{};
  {
    Vector* rptr = result.alignedThis();
    rptr->s0 = static_cast<Type>(data[0]);
    rptr->s1 = static_cast<Type>(data[1]);
    rptr->s2 = static_cast<Type>(data[2]);
    rptr->s3 = static_cast<Type>(data[3]);
    rptr->s4 = static_cast<Type>(data[4]);
    rptr->s5 = static_cast<Type>(data[5]);
    rptr->s6 = static_cast<Type>(data[6]);
    rptr->s7 = static_cast<Type>(data[7]);
    rptr->s8 = static_cast<Type>(data[8]);
    rptr->s9 = static_cast<Type>(data[9]);
    rptr->sa = static_cast<Type>(data[10]);
    rptr->sb = static_cast<Type>(data[11]);
    rptr->sc = static_cast<Type>(data[12]);
    rptr->sd = static_cast<Type>(data[13]);
    rptr->se = static_cast<Type>(data[14]);
    rptr->sf = static_cast<Type>(data[15]);
  }
  return result;
}

/*!
  \details No detailed description

  \tparam T1 No description.
  \param [in] v No description.
  \param [out] data No description.
  */
template <Arithmetic T> template <Arithmetic T1> inline
constexpr void Vector<T, 16>::store(const Vector& v, std::span<T1> data) noexcept
{
  const Vector* vptr = v.alignedThis();
  data[0] = static_cast<T1>(vptr->s0);
  data[1] = static_cast<T1>(vptr->s1);
  data[2] = static_cast<T1>(vptr->s2);
  data[3] = static_cast<T1>(vptr->s3);
  data[4] = static_cast<T1>(vptr->s4);
  data[5] = static_cast<T1>(vptr->s5);
  data[6] = static_cast<T1>(vptr->s6);
  data[7] = static_cast<T1>(vptr->s7);
  data[8] = static_cast<T1>(vptr->s8);
  data[9] = static_cast<T1>(vptr->s9);
  data[10] = static_cast<T1>(vptr->sa);
  data[11] = static_cast<T1>(vptr->sb);
  data[12] = static_cast<T1>(vptr->sc);
  data[13] = static_cast<T1>(vptr->sd);
  data[14] = static_cast<T1>(vptr->se);
  data[15] = static_cast<T1>(vptr->sf);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 16>::sum() const noexcept -> Type
{
  const Vector* ptr = alignedThis();
  const Type s = ptr->s0 + ptr->s1 + ptr->s2 + ptr->s3 +
                 ptr->s4 + ptr->s5 + ptr->s6 + ptr->s7 +
                 ptr->s8 + ptr->s9 + ptr->sa + ptr->sb +
                 ptr->sc + ptr->sd + ptr->se + ptr->sf;
  return s;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator+=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs - rhs;
  return lhs;

}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator-=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs - rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs * rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator*=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs * rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs / rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator/=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs / rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs % rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator%=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs % rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs & rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator&=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs & rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs | rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator|=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs | rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs ^ rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator^=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept
{
  lhs = lhs ^ rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                                         const Vector<Type2, kN>& rhs) noexcept
{
  lhs = lhs << rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                                         const Type2& rhs) noexcept
{
  lhs = lhs << rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                                         const Vector<Type2, kN>& rhs) noexcept
{
  lhs = lhs >> rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in,out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                                         const Type2& rhs) noexcept
{
  lhs = lhs >> rhs;
  return lhs;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
Vector<Type, kN>& operator++(Vector<Type, kN>& value) noexcept
{
  value += static_cast<Type>(1);
  return value;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
Vector<Type, kN>& operator--(Vector<Type, kN>& value) noexcept
{
  value -= static_cast<Type>(1);
  return value;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
Vector<Type, kN> operator++(Vector<Type, kN>& value, int) noexcept
{
  const Vector<Type, kN> old = value;
  value += static_cast<Type>(1);
  return old;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in,out] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
Vector<Type, kN> operator--(Vector<Type, kN>& value, int) noexcept
{
  const Vector<Type, kN> old = value;
  value -= static_cast<Type>(1);
  return old;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator+(const Vector<Type, kN>& value) noexcept
{
  return value;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator-(const Vector<Type, kN>& value) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type>(std::negate<Type>{},
                                                       value);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::plus<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator+(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::plus<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator+(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs + lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::minus<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator-(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::minus<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator-(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const VectorT result = VectorT::template apply<Type1, Type2>(std::minus<Type1>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::multiplies<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator*(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::multiplies<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator*(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs * lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::divides<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator/(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::divides<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator/(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const VectorT result = VectorT::template apply<Type1, Type2>(std::divides<Type1>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::modulus<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator%(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::modulus<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator%(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const VectorT result = VectorT::template apply<Type1, Type2>(std::modulus<Type1>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN> operator~(const Vector<Type, kN>& value) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type>(std::bit_not<Type>{},
                                                       value);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::bit_and<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator&(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::bit_and<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator&(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs & lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::bit_or<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator|(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::bit_or<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator|(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs | lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type, size_t kN> inline
constexpr Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  const VectorT result = VectorT::template apply<Type, Type>(std::bit_xor<Type>{},
                                                             lhs,
                                                             rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type2, kN> operator^(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  const VectorT result = VectorT::template apply<Type2, Type1>(std::bit_xor<Type2>{},
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator^(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs ^ lhs);
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                                       const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const auto left_shift = [](const Type1& x, const Type2& y) noexcept -> Type1
  {
    const Type1 result = x << y;
    return result;
  };
  const VectorT result = VectorT::template apply<Type1, Type2>(left_shift,
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                                       const Type2& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const auto left_shift = [](const Type1& x, const Type2& y) noexcept -> Type1
  {
    const Type1 result = x << y;
    return result;
  };
  const VectorT result = VectorT::template apply<Type1, Type2>(left_shift,
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                                       const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const auto right_shift = [](const Type1& x, const Type2& y) noexcept -> Type1
  {
    const Type1 result = x >> y;
    return result;
  };
  const VectorT result = VectorT::template apply<Type1, Type2>(right_shift,
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN> inline
constexpr Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                                       const Type2& rhs) noexcept
{
  using VectorT = Vector<Type1, kN>;
  const auto right_shift = [](const Type1& x, const Type2& y) noexcept -> Type1
  {
    const Type1 result = x >> y;
    return result;
  };
  const VectorT result = VectorT::template apply<Type1, Type2>(right_shift,
                                                               lhs,
                                                               rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] value No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator!(const Vector<Type, kN>& value) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto logical_not = [](const Type& x) noexcept -> IntegerT
  {
    const int32b result = std::logical_not<Type>{}(x) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT>(logical_not,
                                                           value);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator&&(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto logical_and = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::logical_and<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(logical_and,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator&&(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto logical_and = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::logical_and<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(logical_and,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator&&(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs && lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator||(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto logical_or = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::logical_or<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(logical_or,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator||(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto logical_or = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::logical_or<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(logical_or,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator||(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs || lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator==(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto equal_to = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::equal_to<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(equal_to,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator==(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto equal_to = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::equal_to<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(equal_to,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator==(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs == lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator!=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto not_equal_to = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::not_equal_to<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(not_equal_to,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator!=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto not_equal_to = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::not_equal_to<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(not_equal_to,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator!=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs != lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator<(const Vector<Type, kN>& lhs,
                                            const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto less = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::less<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(less,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator<(const Type1& lhs,
                                             const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto less = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::less<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(less,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator<(const Vector<Type1, kN>& lhs,
                                             const Type2& rhs) noexcept
{
  return (rhs > lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator<=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto less_equal = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::less_equal<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(less_equal,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator<=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto less_equal = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::less_equal<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(less_equal,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator<=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs >= lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator>(const Vector<Type, kN>& lhs,
                                            const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto greater = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::greater<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(greater,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator>(const Type1& lhs,
                                             const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto greater = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::greater<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(greater,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator>(const Vector<Type1, kN>& lhs,
                                             const Type2& rhs) noexcept
{
  return (rhs < lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type, size_t kN> inline
constexpr CompResultVec<Type, kN> operator>=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept
{
  using VectorT = Vector<Type, kN>;
  using ResultT = CompResultVec<Type, kN>;
  using IntegerT = typename ResultT::Type;
  const auto greater_equal = [](const Type& x, const Type& y) noexcept -> IntegerT
  {
    const int32b result = std::greater_equal<Type>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type>(greater_equal,
                                                                 lhs,
                                                                 rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type2, kN> operator>=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept
{
  using VectorT = Vector<Type2, kN>;
  using ResultT = CompResultVec<Type2, kN>;
  using IntegerT = typename ResultT::Type;
  const auto greater_equal = [](const Type1& x, const Type2& y) noexcept -> IntegerT
  {
    const int32b result = std::greater_equal<Type2>{}(x, y) ? kVTrue : kVFalse;
    return static_cast<IntegerT>(result);
  };
  const ResultT result = VectorT::template apply<IntegerT, Type1>(greater_equal,
                                                                  lhs,
                                                                  rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam kN No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <Arithmetic Type1, Arithmetic Type2, size_t kN> inline
constexpr CompResultVec<Type1, kN> operator>=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept
{
  return (rhs <= lhs);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename Type, size_t kN> inline
Vector<Type, kN> VectorData::Impl::loadN(const size_t offset,
                                         const Type* p) noexcept
{
  using VectorT = Vector<Type, kN>;
  const Type* address = p + offset * kN;
  const VectorT result = VectorT::template load<const Type>({address, kN});
  return result;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <size_t kN> inline
Vector<float, kN> VectorData::Impl::loadHalfN(const size_t offset,
                                              const half* p) noexcept
{
  using VectorT = Vector<float, kN>;
  const half* address = p + offset * kN;
  const VectorT result = VectorT::template load<const half>({address, kN});
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam kN No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type, size_t kN> inline
void VectorData::Impl::storeN(const Vector<Type, kN>& data,
                              const size_t offset,
                              Type* p) noexcept
{
  using VectorT = Vector<Type, kN>;
  Type* address = p + offset * kN;
  VectorT::template store<Type>(data, {address, kN});
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <size_t kN> inline
void VectorData::Impl::storeHalfN(const Vector<float, kN>& data,
                                  const size_t offset,
                                  half* p) noexcept
{
  using VectorT = Vector<float, kN>;
  half* address = p + offset * kN;
  VectorT::template store<half>(data, {address, kN});
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::load2(const size_t offset,
                       const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  return Impl::loadN<T, 2>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load2(const size_t offset,
                       const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return load2(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load2(const size_t offset,
                       const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return load2(offset, p.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::load3(const size_t offset,
                       const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  return Impl::loadN<T, 3>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load3(const size_t offset,
                       const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return load3(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load3(const size_t offset,
                       const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return load3(offset, p.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::load4(const size_t offset,
                       const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  return Impl::loadN<T, 4>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load4(const size_t offset,
                       const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return load4(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load4(const size_t offset,
                       const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return load4(offset, p.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::load8(const size_t offset,
                       const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  return Impl::loadN<T, 8>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load8(const size_t offset,
                       const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return load8(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load8(const size_t offset,
                       const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return load8(offset, p.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::load16(const size_t offset,
                        const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  return Impl::loadN<T, 16>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load16(const size_t offset,
                        const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return load16(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto VectorData::load16(const size_t offset,
                        const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return load16(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float VectorData::loadHalf(const size_t offset,
                           const half* p) noexcept
{
  const half* address = p + offset;
  return static_cast<float>(*address);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float VectorData::loadHalf(const size_t offset,
                           const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float VectorData::loadHalf(const size_t offset,
                           const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float2 VectorData::loadHalf2(const size_t offset,
                             const half* p) noexcept
{
  return Impl::loadHalfN<2>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 VectorData::loadHalf2(const size_t offset,
                             const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf2(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 VectorData::loadHalf2(const size_t offset,
                             const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf2(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float3 VectorData::loadHalf3(const size_t offset,
                             const half* p) noexcept
{
  return Impl::loadHalfN<3>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 VectorData::loadHalf3(const size_t offset,
                             const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf3(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 VectorData::loadHalf3(const size_t offset,
                             const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf3(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \param [out] z No description.
  \return No description
  */
inline
float4 VectorData::loadHalf4(const size_t offset,
                             const half* p) noexcept
{
  return Impl::loadHalfN<4>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 VectorData::loadHalf4(const size_t offset,
                             const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf4(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 VectorData::loadHalf4(const size_t offset,
                             const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf4(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \param [out] z No description.
  \return No description
  */
inline
float8 VectorData::loadHalf8(const size_t offset,
                             const half* p) noexcept
{
  return Impl::loadHalfN<8>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float8 VectorData::loadHalf8(const size_t offset,
                             const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf8(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float8 VectorData::loadHalf8(const size_t offset,
                             const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf8(offset, p.get());
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \param [out] z No description.
  \return No description
  */
inline
float16 VectorData::loadHalf16(const size_t offset,
                               const half* p) noexcept
{
  return Impl::loadHalfN<16>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float16 VectorData::loadHalf16(const size_t offset,
                               const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return loadHalf16(offset, p.get());
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float16 VectorData::loadHalf16(const size_t offset,
                               const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return loadHalf16(offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::store2(const Vector<Type, 2>& data,
                        const size_t offset,
                        std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::storeN<T, 2>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::store2(const Vector<Type, 2>& data,
                        const size_t offset,
                        AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  store2(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::store3(const Vector<Type, 3>& data,
                        const size_t offset,
                        std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::storeN<T, 3>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::store3(const Vector<Type, 3>& data,
                        const size_t offset,
                        AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  store3(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::store4(const Vector<Type, 4>& data,
                        const size_t offset,
                        std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::storeN<T, 4>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::store4(const Vector<Type, 4>& data,
                        const size_t offset,
                        AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  store4(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::store8(const Vector<Type, 8>& data,
                        const size_t offset,
                        std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::storeN<T, 8>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::store8(const Vector<Type, 8>& data,
                        const size_t offset,
                        AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  store8(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::store16(const Vector<Type, 16>& data,
                         const size_t offset,
                         std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::storeN<T, 16>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::store16(const Vector<Type, 16>& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  store16(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf(const float data,
                           const size_t offset,
                           half* p) noexcept
{
  half* address = p + offset;
  const half fdata = static_cast<half>(data);
  *address = fdata;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf(const float data,
                           const size_t offset,
                           AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf2(const float2& data,
                            const size_t offset,
                            half* p) noexcept
{
  Impl::storeHalfN(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf2(const float2& data,
                            const size_t offset,
                            AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf2(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf3(const float3& data,
                            const size_t offset,
                            half* p) noexcept
{
  Impl::storeHalfN(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf3(const float3& data,
                            const size_t offset,
                            AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf3(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf4(const float4& data,
                            const size_t offset,
                            half* p) noexcept
{
  Impl::storeHalfN(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf4(const float4& data,
                            const size_t offset,
                            AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf4(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf8(const float8& data,
                            const size_t offset,
                            half* p) noexcept
{
  Impl::storeHalfN(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf8(const float8& data,
                            const size_t offset,
                            AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf8(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::storeHalf16(const float16& data,
                             const size_t offset,
                             half* p) noexcept
{
  Impl::storeHalfN(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::storeHalf16(const float16& data,
                             const size_t offset,
                             AddressSpacePointer<kASpaceType, half> p) noexcept
{
  storeHalf16(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto vload2(const size_t offset,
            const Pointer p) noexcept
{
  return VectorData::load2(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload2(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return VectorData::load2<kASpaceType, Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload2(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return VectorData::load2<kASpaceType, const Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto vload3(const size_t offset,
            const Pointer p) noexcept
{
  return VectorData::load3(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload3(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return VectorData::load3<kASpaceType, Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload3(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return VectorData::load3<kASpaceType, const Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto vload4(const size_t offset,
            const Pointer p) noexcept
{
  return VectorData::load4(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload4(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return VectorData::load4<kASpaceType, Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload4(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return VectorData::load4<kASpaceType, const Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto vload8(const size_t offset,
            const Pointer p) noexcept
{
  return VectorData::load8(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload8(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return VectorData::load8<kASpaceType, Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload8(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return VectorData::load8<kASpaceType, const Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto vload16(const size_t offset,
             const Pointer p) noexcept
{
  return VectorData::load16(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload16(const size_t offset,
             const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  return VectorData::load16<kASpaceType, Type>(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, typename Type> inline
auto vload16(const size_t offset,
             const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  return VectorData::load16<kASpaceType, const Type>(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float vload_half(const size_t offset,
                 const half* p) noexcept
{
  return VectorData::loadHalf(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float vload_half(const size_t offset,
                 const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float vload_half(const size_t offset,
                 const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float2 vload_half2(const size_t offset,
                   const half* p) noexcept
{
  return VectorData::loadHalf2(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 vload_half2(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf2(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 vload_half2(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf2(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float3 vload_half3(const size_t offset,
                   const half* p) noexcept
{
  return VectorData::loadHalf3(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 vload_half3(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf3(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 vload_half3(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf3(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] half No description.
  \return No description
  */
inline
float4 vload_half4(const size_t offset,
                   const half* p) noexcept
{
  return VectorData::loadHalf4(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 vload_half4(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf4(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 vload_half4(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf4(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] half No description.
  \return No description
  */
inline
float8 vload_half8(const size_t offset,
                   const half* p) noexcept
{
  return VectorData::loadHalf8(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float8 vload_half8(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf8(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float8 vload_half8(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf8(offset, p);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] half No description.
  \return No description
  */
inline
float16 vload_half16(const size_t offset,
                     const half* p) noexcept
{
  return VectorData::loadHalf16(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float16 vload_half16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  return VectorData::loadHalf16(offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float16 vload_half16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  return VectorData::loadHalf16(offset, p);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept
{
  VectorData::store2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  VectorData::store2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept
{
  VectorData::store3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  VectorData::store3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept
{
  VectorData::store4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  VectorData::store4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void vstore8(const Vector<Type, 8>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept
{
  VectorData::store8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void vstore8(const Vector<Type, 8>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  VectorData::store8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void vstore16(const Vector<Type, 16>& data,
              const size_t offset,
              std::add_pointer_t<Type> p) noexcept
{
  VectorData::store16(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void vstore16(const Vector<Type, 16>& data,
              const size_t offset,
              AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  VectorData::store16(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half(const float data,
                 const size_t offset,
                 half* p) noexcept
{
  VectorData::storeHalf(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half(const float data,
                 const size_t offset,
                 AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half2(const float2& data,
                  const size_t offset,
                  half* p) noexcept
{
  VectorData::storeHalf2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half2(const float2& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf2(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half3(const float3& data,
                  const size_t offset,
                  half* p) noexcept
{
  VectorData::storeHalf3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half3(const float3& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf3(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half4(const float4& data,
                  const size_t offset,
                  half* p) noexcept
{
  VectorData::storeHalf4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half4(const float4& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf4(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half8(const float8& data,
                  const size_t offset,
                  half* p) noexcept
{
  VectorData::storeHalf8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half8(const float8& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf8(data, offset, p);
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void vstore_half16(const float16& data,
                   const size_t offset,
                   half* p) noexcept
{
  VectorData::storeHalf16(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void vstore_half16(const float16& data,
                   const size_t offset,
                   AddressSpacePointer<kASpaceType, half> p) noexcept
{
  VectorData::storeHalf16(data, offset, p);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_VECTOR_INL_HPP
