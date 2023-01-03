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
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/ieee_754_binary.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

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
    data_{{v0, v1}}
{
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
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
constexpr auto Vector<T, 2>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::end() noexcept -> iterator
{
  Pointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::end() const noexcept -> const_iterator
{
  ConstPointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::alignment() noexcept -> size_type
{
  constexpr size_type align_s = std::alignment_of_v<Vector>;
  constexpr size_type s = sizeof(Vector);
  static_assert(s == size() * sizeof(Type), "The size of Vector isn't correct.");
  static_assert(align_s == s, "The alignment of Vector isn't correct.");
  return align_s;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::get(const size_type index) noexcept -> Reference
{
  Pointer p = data();
  return p[index];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::get(const size_type index) const noexcept -> ConstReference
{
  ConstPointer p = data();
  return p[index];
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 2>::size() noexcept -> size_type
{
  return 2;
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
    data_{{v0, v1, v2, static_cast<Type>(0)}}
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

  \param [in] index No description.
  \return No description
  */
template <Arithmetic Type> inline
constexpr auto Vector<Type, 3>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
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
constexpr auto Vector<T, 3>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::end() noexcept -> iterator
{
  Pointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::end() const noexcept -> const_iterator
{
  ConstPointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::alignment() noexcept -> size_type
{
  constexpr size_type align_s = std::alignment_of_v<Vector>;
  constexpr size_type s = sizeof(Vector);
  static_assert(s == 4 * sizeof(Type), "The size of Vector isn't correct.");
  static_assert(align_s == s, "The alignment of Vector isn't correct.");
  return align_s;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::get(const size_type index) noexcept -> Reference
{
  Pointer p = data();
  return p[index];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 3>::get(const size_type index) const noexcept -> ConstReference
{
  ConstPointer p = data();
  return p[index];
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
    data_{{v0, v1, v2, v3}}
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

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr Vector<T, 4>::operator bool() const noexcept
{
  const bool result = static_cast<bool>(x) && static_cast<bool>(y) &&
                      static_cast<bool>(z) && static_cast<bool>(z);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::end() noexcept -> iterator
{
  Pointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::end() const noexcept -> const_iterator
{
  ConstPointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::alignment() noexcept -> size_type
{
  constexpr size_type align_s = std::alignment_of_v<Vector>;
  constexpr size_type s = sizeof(Vector);
  static_assert(s == size() * sizeof(Type), "The size of Vector isn't correct.");
  static_assert(align_s == s, "The alignment of Vector isn't correct.");
  return align_s;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = std::is_constant_evaluated()
      ? data_.data()
      : zisc::assume_aligned<align_s>(data_.data());
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::get(const size_type index) noexcept -> Reference
{
  Pointer p = data();
  return p[index];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::get(const size_type index) const noexcept -> ConstReference
{
  ConstPointer p = data();
  return p[index];
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
constexpr auto Vector<T, 4>::size() noexcept -> size_type
{
  return 4;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] += r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] += rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] -= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] -= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] *= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] *= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] /= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] /= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] %= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] %= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] &= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] &= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] |= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] |= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] ^= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] ^= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] <<= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] <<= rhs;
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] >>= r[index];
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
  typename std::remove_reference_t<decltype(lhs)>::Pointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    l[index] >>= rhs;
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(value)>::ConstPointer v = value.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = -v[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] + r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs + r[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] - r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs - r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] - rhs;
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] * r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs * r[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] / r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs / r[index];
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
  Vector<Type1, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] / rhs;
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] % r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs % r[index];
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
  Vector<Type1, kN> result{static_cast<Type1>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] % rhs;
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(value)>::ConstPointer v = value.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = ~v[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] & r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs & r[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] | r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs | r[index];
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
  Vector<Type, kN> result{static_cast<Type>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] ^ r[index];
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
  Vector<Type2, kN> result{static_cast<Type2>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs ^ r[index];
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
  Vector<Type1, kN> result{static_cast<Type1>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = static_cast<Type1>(l[index] << r[index]);
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
  Vector<Type1, kN> result{static_cast<Type1>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = static_cast<Type1>(l[index] << rhs);
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
  Vector<Type1, kN> result{static_cast<Type1>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = static_cast<Type1>(l[index] >> r[index]);
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
  Vector<Type1, kN> result{static_cast<Type1>(0)};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = static_cast<Type1>(l[index] >> rhs);
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
constexpr Vector<bool, kN> operator!(const Vector<Type, kN>& value) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(value)>::ConstPointer v = value.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = !v[index];
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
constexpr Vector<bool, kN> operator&&(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] && r[index];
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
constexpr Vector<bool, kN> operator&&(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs && r[index];
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
constexpr Vector<bool, kN> operator&&(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator||(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] || r[index];
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
constexpr Vector<bool, kN> operator||(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs || r[index];
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
constexpr Vector<bool, kN> operator||(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator==(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = zisc::equal(l[index], r[index]);
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
constexpr Vector<bool, kN> operator==(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = zisc::equal(lhs, r[index]);
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
constexpr Vector<bool, kN> operator==(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator!=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = !zisc::equal(l[index], r[index]);
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
constexpr Vector<bool, kN> operator!=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = !zisc::equal(lhs, r[index]);
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
constexpr Vector<bool, kN> operator!=(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator<(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] < r[index];
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
constexpr Vector<bool, kN> operator<(const Type1& lhs,
                                     const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs < r[index];
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
constexpr Vector<bool, kN> operator<(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator<=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] <= r[index];
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
constexpr Vector<bool, kN> operator<=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs <= r[index];
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
constexpr Vector<bool, kN> operator<=(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator>(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] > r[index];
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
constexpr Vector<bool, kN> operator>(const Type1& lhs,
                                     const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs > r[index];
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
constexpr Vector<bool, kN> operator>(const Vector<Type1, kN>& lhs,
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
constexpr Vector<bool, kN> operator>=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(lhs)>::ConstPointer l = lhs.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = l[index] >= r[index];
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
constexpr Vector<bool, kN> operator>=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result{false};
  typename std::remove_reference_t<decltype(result)>::Pointer res = result.data();
  typename std::remove_reference_t<decltype(rhs)>::ConstPointer r = rhs.data();
  for (size_t index = 0; index < kN; ++index)
    res[index] = lhs >= r[index];
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
constexpr Vector<bool, kN> operator>=(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept
{
  return (rhs <= lhs);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload2(const size_t offset,
                        const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  const Vector<T, 2> result = Impl::vloadn<T, 2>(offset, p);
  return result;
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
auto VectorData::vload2(const size_t offset,
                        const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  const auto result = vload2(offset, p.get());
  return result;
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
auto VectorData::vload2(const size_t offset,
                        const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  const auto result = vload2(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload3(const size_t offset,
                        const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  const Vector<T, 3> result = Impl::vloadn<T, 3>(offset, p);
  return result;
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
auto VectorData::vload3(const size_t offset,
                        const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  const auto result = vload3(offset, p.get());
  return result;
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
auto VectorData::vload3(const size_t offset,
                        const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  const auto result = vload3(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload4(const size_t offset,
                        const Pointer p) noexcept
{
  using T = std::remove_cvref_t<std::remove_pointer_t<Pointer>>;
  const Vector<T, 4> result = Impl::vloadn<T, 4>(offset, p);
  return result;
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
auto VectorData::vload4(const size_t offset,
                        const AddressSpacePointer<kASpaceType, Type>& p) noexcept
{
  const auto result = vload4(offset, p.get());
  return result;
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
auto VectorData::vload4(const size_t offset,
                        const AddressSpacePointer<kASpaceType, const Type>& p) noexcept
{
  const auto result = vload4(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float VectorData::vload_half(const size_t offset,
                             const half* p) noexcept
{
  const half* address = p + offset;
  const float result = static_cast<float>(*address);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float VectorData::vload_half(const size_t offset,
                             const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  const float result = vload_half(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float VectorData::vload_half(const size_t offset,
                             const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  const float result = vload_half(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float2 VectorData::vload_half2(const size_t offset,
                               const half* p) noexcept
{
  const float2 result = Impl::vload_halfn<2>(offset, p);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 VectorData::vload_half2(const size_t offset,
                               const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  const float2 result = vload_half2(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float2 VectorData::vload_half2(const size_t offset,
                               const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  const float2 result = vload_half2(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
inline
float3 VectorData::vload_half3(const size_t offset,
                               const half* p) noexcept
{
  const float3 result = Impl::vload_halfn<3>(offset, p);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 VectorData::vload_half3(const size_t offset,
                               const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  const float3 result = vload_half3(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float3 VectorData::vload_half3(const size_t offset,
                               const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  const float3 result = vload_half3(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  \param [in] p No description.
  \param [out] z No description.
  \return No description
  */
inline
float4 VectorData::vload_half4(const size_t offset,
                               const half* p) noexcept
{
  const float4 result = Impl::vload_halfn<4>(offset, p);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 VectorData::vload_half4(const size_t offset,
                               const AddressSpacePointer<kASpaceType, half>& p) noexcept
{
  const float4 result = vload_half4(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType> inline
float4 VectorData::vload_half4(const size_t offset,
                               const AddressSpacePointer<kASpaceType, const half>& p) noexcept
{
  const float4 result = vload_half4(offset, p.get());
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::vstore2(const Vector<Type, 2>& data,
                         const size_t offset,
                         std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::vstoren<T, 2>(data, offset, p);
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
void VectorData::vstore2(const Vector<Type, 2>& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  vstore2(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::vstore3(const Vector<Type, 3>& data,
                         const size_t offset,
                         std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::vstoren<T, 3>(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType, typename Type> inline
void VectorData::vstore3(const Vector<Type, 3>& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  vstore3(data, offset, p.get());
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename Type> inline
void VectorData::vstore4(const Vector<Type, 4>& data,
                         const size_t offset,
                         std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Impl::vstoren<T, 4>(data, offset, p);
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
void VectorData::vstore4(const Vector<Type, 4>& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  vstore4(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::vstore_half(const float data,
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
void VectorData::vstore_half(const float data,
                             const size_t offset,
                             AddressSpacePointer<kASpaceType, half> p) noexcept
{
  vstore_half(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::vstore_half2(const float2& data,
                              const size_t offset,
                              half* p) noexcept
{
  Impl::vstore_halfn(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::vstore_half2(const float2& data,
                              const size_t offset,
                              AddressSpacePointer<kASpaceType, half> p) noexcept
{
  vstore_half2(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::vstore_half3(const float3& data,
                              const size_t offset,
                              half* p) noexcept
{
  Impl::vstore_halfn(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::vstore_half3(const float3& data,
                              const size_t offset,
                              AddressSpacePointer<kASpaceType, half> p) noexcept
{
  vstore_half3(data, offset, p.get());
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
inline
void VectorData::vstore_half4(const float4& data,
                              const size_t offset,
                              half* p) noexcept
{
  Impl::vstore_halfn(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <AddressSpaceType kASpaceType> inline
void VectorData::vstore_half4(const float4& data,
                              const size_t offset,
                              AddressSpacePointer<kASpaceType, half> p) noexcept
{
  vstore_half4(data, offset, p.get());
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
Vector<Type, kN> VectorData::Impl::vloadn(const size_t offset,
                                          const Type* p) noexcept
{
  Vector<Type, kN> data{};
  typename decltype(data)::Pointer d = data.data();
  const Type* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    d[i] = address[i];
  return data;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <size_t kN> inline
Vector<float, kN> VectorData::Impl::vload_halfn(const size_t offset,
                                                const half* p) noexcept
{
  Vector<float, kN> data{};
  typename decltype(data)::Pointer d = data.data();
  const half* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    d[i] = static_cast<float>(address[i]);
  return data;
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
void VectorData::Impl::vstoren(
    const Vector<Type, kN>& data,
    const size_t offset,
    Type* p) noexcept
{
  typename std::remove_reference_t<decltype(data)>::ConstPointer d = data.data();
  Type* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    address[i] = d[i];
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <size_t kN> inline
void VectorData::Impl::vstore_halfn(
    const Vector<float, kN>& data,
    const size_t offset,
    half* p) noexcept
{
  typename std::remove_reference_t<decltype(data)>::ConstPointer d = data.data();
  half* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    address[i] = static_cast<half>(d[i]);
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
  const auto result = VectorData::vload2(offset, p);
  return result;
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
  const auto result = VectorData::vload2<kASpaceType, Type>(offset, p);
  return result;
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
  const auto result = VectorData::vload2<kASpaceType, const Type>(offset, p);
  return result;
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
  const auto result = VectorData::vload3(offset, p);
  return result;
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
  const auto result = VectorData::vload3<kASpaceType, Type>(offset, p);
  return result;
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
  const auto result = VectorData::vload3<kASpaceType, const Type>(offset, p);
  return result;
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
  const auto result = VectorData::vload4(offset, p);
  return result;
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
  const auto result = VectorData::vload4<kASpaceType, Type>(offset, p);
  return result;
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
  const auto result = VectorData::vload4<kASpaceType, const Type>(offset, p);
  return result;
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
  const float result = VectorData::vload_half(offset, p);
  return result;
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
  const float result = VectorData::vload_half(offset, p);
  return result;
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
  const float result = VectorData::vload_half(offset, p);
  return result;
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
  const float2 result = VectorData::vload_half2(offset, p);
  return result;
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
  const float2 result = VectorData::vload_half2(offset, p);
  return result;
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
  const float2 result = VectorData::vload_half2(offset, p);
  return result;
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
  const float3 result = VectorData::vload_half3(offset, p);
  return result;
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
  const float3 result = VectorData::vload_half3(offset, p);
  return result;
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
  const float3 result = VectorData::vload_half3(offset, p);
  return result;
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
  const float4 result = VectorData::vload_half4(offset, p);
  return result;
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
  const float4 result = VectorData::vload_half4(offset, p);
  return result;
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
  const float4 result = VectorData::vload_half4(offset, p);
  return result;
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
  VectorData::vstore2(data, offset, p);
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
  VectorData::vstore2(data, offset, p);
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
  VectorData::vstore3(data, offset, p);
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
  VectorData::vstore3(data, offset, p);
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
  VectorData::vstore4(data, offset, p);
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
  VectorData::vstore4(data, offset, p);
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
  VectorData::vstore_half(data, offset, p);
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
  VectorData::vstore_half(data, offset, p);
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
  VectorData::vstore_half2(data, offset, p);
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
  VectorData::vstore_half2(data, offset, p);
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
  VectorData::vstore_half3(data, offset, p);
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
  VectorData::vstore_half3(data, offset, p);
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
  VectorData::vstore_half4(data, offset, p);
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
  VectorData::vstore_half4(data, offset, p);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_VECTOR_INL_HPP
