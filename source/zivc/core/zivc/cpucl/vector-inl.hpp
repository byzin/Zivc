/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_VECTOR_INL_HPP
#define ZIVC_CPUCL_VECTOR_INL_HPP

#include "vector.hpp"
// Standard C++ library
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
  \details No detailed description
  */
template <Arithmetic T> inline
Vector<T, 2>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
Vector<T, 2>::Vector(ConstReference v) noexcept : Vector(v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 2>::Vector(ConstReference v0, ConstReference v1) noexcept :
    x{v0},
    y{v1}
{
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
Vector<T, 2>::operator bool() const noexcept
{
  const bool result = zisc::cast<bool>(x) && zisc::cast<bool>(y);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::end() noexcept -> iterator
{
  constexpr size_type align_s = alignment();
  Pointer p = zisc::assume_aligned<align_s>(data() + size());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::end() const noexcept -> const_iterator
{
  constexpr size_type align_s = alignment();
  ConstPointer p = zisc::assume_aligned<align_s>(data() + size());
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
auto Vector<T, 2>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 2>::get(const size_type index) noexcept -> Reference
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
auto Vector<T, 2>::get(const size_type index) const noexcept -> ConstReference
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
  */
template <Arithmetic T> inline
Vector<T, 3>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
Vector<T, 3>::Vector(ConstReference v) noexcept : Vector(v, v, v)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  \param [in] v2 No description.
  */
template <Arithmetic T> inline
Vector<T, 3>::Vector(ConstReference v0, ConstReference v1, ConstReference v2) noexcept :
    x{v0},
    y{v1},
    z{v2},
    pad_{zisc::cast<Type>(0)}
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 3>::Vector(const Vector2& v0, ConstReference v1) noexcept :
    Vector(v0.x, v0.y, v1)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 3>::Vector(ConstReference v0, const Vector2& v1) noexcept :
    Vector(v0, v1.x, v1.y)
{
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic Type> inline
auto Vector<Type, 3>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
Vector<T, 3>::operator bool() const noexcept
{
  const bool result = zisc::cast<bool>(x) && zisc::cast<bool>(y) &&
                      zisc::cast<bool>(z);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::end() noexcept -> iterator
{
  Pointer p = data() + size();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::end() const noexcept -> const_iterator
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
auto Vector<T, 3>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 3>::get(const size_type index) noexcept -> Reference
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
auto Vector<T, 3>::get(const size_type index) const noexcept -> ConstReference
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
  */
template <Arithmetic T> inline
Vector<T, 4>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  \details No detailed description

  \param [in] v No description.
  */
template <Arithmetic T> inline
Vector<T, 4>::Vector(ConstReference v) noexcept : Vector(v, v, v, v)
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
Vector<T, 4>::Vector(ConstReference v0, ConstReference v1, ConstReference v2, ConstReference v3) noexcept :
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
Vector<T, 4>::Vector(const Vector2& v0, ConstReference v1, ConstReference v2) noexcept :
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
Vector<T, 4>::Vector(ConstReference v0, const Vector2& v1, ConstReference v2) noexcept :
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
Vector<T, 4>::Vector(ConstReference v0, ConstReference v1, const Vector2& v2) noexcept :
    Vector(v0, v1, v2.x, v2.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 4>::Vector(const Vector2& v0, const Vector2& v1) noexcept :
    Vector(v0.x, v0.y, v1.x, v1.y)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 4>::Vector(const Vector3& v0, ConstReference v1) noexcept :
    Vector(v0.x, v0.y, v0.z, v1)
{
}

/*!
  \details No detailed description

  \param [in] v0 No description.
  \param [in] v1 No description.
  */
template <Arithmetic T> inline
Vector<T, 4>::Vector(ConstReference v0, const Vector3& v1) noexcept :
    Vector(v0, v1.x, v1.y, v1.z)
{
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::operator[](const size_type index) noexcept -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::operator[](const size_type index) const noexcept -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
Vector<T, 4>::operator bool() const noexcept
{
  const bool result = zisc::cast<bool>(x) && zisc::cast<bool>(y) &&
                      zisc::cast<bool>(z) && zisc::cast<bool>(z);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::begin() noexcept -> iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::begin() const noexcept -> const_iterator
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::end() noexcept -> iterator
{
  constexpr size_type align_s = alignment();
  Pointer p = zisc::assume_aligned<align_s>(data() + size());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::end() const noexcept -> const_iterator
{
  constexpr size_type align_s = alignment();
  ConstPointer p = zisc::assume_aligned<align_s>(data() + size());
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
auto Vector<T, 4>::data() noexcept -> Pointer
{
  constexpr size_type align_s = alignment();
  Pointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::data() const noexcept -> ConstPointer
{
  constexpr size_type align_s = alignment();
  ConstPointer p = zisc::assume_aligned<align_s>(&x);
  return p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <Arithmetic T> inline
auto Vector<T, 4>::get(const size_type index) noexcept -> Reference
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
auto Vector<T, 4>::get(const size_type index) const noexcept -> ConstReference
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
Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] += rhs[index];
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
Vector<Type1, kN>& operator+=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] += rhs;
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
Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] -= rhs[index];
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
Vector<Type1, kN>& operator-=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] -= rhs;
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
Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] *= rhs[index];
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
Vector<Type1, kN>& operator*=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] *= rhs;
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
Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] /= rhs[index];
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
Vector<Type1, kN>& operator/=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] /= rhs;
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
Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] %= rhs[index];
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
Vector<Type1, kN>& operator%=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] %= rhs;
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
Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] &= rhs[index];
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
Vector<Type1, kN>& operator&=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] &= rhs;
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
Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] |= rhs[index];
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
Vector<Type1, kN>& operator|=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] |= rhs;
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
Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] ^= rhs[index];
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
Vector<Type1, kN>& operator^=(Vector<Type1, kN>& lhs,
                              const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] ^= rhs;
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
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] <<= rhs[index];
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
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] <<= rhs;
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
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] >>= rhs[index];
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
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept
{
  for (size_t index = 0; index < kN; ++index)
    lhs[index] >>= rhs;
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
  constexpr auto one = zisc::cast<Type>(1);
  value += one;
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
  constexpr auto one = zisc::cast<Type>(1);
  value -= one;
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
  constexpr auto one = zisc::cast<Type>(1);
  const auto old = value;
  value += one;
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
  constexpr auto one = zisc::cast<Type>(1);
  const auto old = value;
  value -= one;
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
Vector<Type, kN> operator+(const Vector<Type, kN>& value) noexcept
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
Vector<Type, kN> operator-(const Vector<Type, kN>& value) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = -value[index];
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
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] + rhs[index];
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
Vector<Type2, kN> operator+(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs + rhs[index];
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
Vector<Type1, kN> operator+(const Vector<Type1, kN>& lhs,
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
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] - rhs[index];
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
Vector<Type2, kN> operator-(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs - rhs[index];
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
Vector<Type1, kN> operator-(const Vector<Type1, kN>& lhs,
                            const Type2& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] - rhs;
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
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] * rhs[index];
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
Vector<Type2, kN> operator*(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs * rhs[index];
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
Vector<Type1, kN> operator*(const Vector<Type1, kN>& lhs,
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
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] / rhs[index];
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
Vector<Type2, kN> operator/(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs / rhs[index];
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
Vector<Type1, kN> operator/(const Vector<Type1, kN>& lhs,
                            const Type2& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] / rhs;
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
Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] % rhs[index];
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
Vector<Type2, kN> operator%(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs % rhs[index];
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
Vector<Type1, kN> operator%(const Vector<Type1, kN>& lhs,
                            const Type2& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] % rhs;
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
Vector<Type, kN> operator~(const Vector<Type, kN>& value) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = ~value[index];
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
Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] & rhs[index];
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
Vector<Type2, kN> operator&(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs & rhs[index];
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
Vector<Type1, kN> operator&(const Vector<Type1, kN>& lhs,
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
Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] | rhs[index];
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
Vector<Type2, kN> operator|(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs | rhs[index];
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
Vector<Type1, kN> operator|(const Vector<Type1, kN>& lhs,
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
Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] ^ rhs[index];
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
Vector<Type2, kN> operator^(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type2, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs ^ rhs[index];
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
Vector<Type1, kN> operator^(const Vector<Type1, kN>& lhs,
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
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] << rhs[index]);
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
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] << rhs);
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
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] >> rhs[index]);
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
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept
{
  Vector<Type1, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] >> rhs);
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
Vector<bool, kN> operator!(const Vector<Type, kN>& value) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = !value[index];
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
Vector<bool, kN> operator&&(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] && rhs[index];
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
Vector<bool, kN> operator&&(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs && rhs[index];
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
Vector<bool, kN> operator&&(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator||(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] || rhs[index];
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
Vector<bool, kN> operator||(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs || rhs[index];
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
Vector<bool, kN> operator||(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator==(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::equal(lhs[index], rhs[index]);
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
Vector<bool, kN> operator==(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::equal(lhs, rhs[index]);
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
Vector<bool, kN> operator==(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator!=(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = !zisc::equal(lhs[index], rhs[index]);
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
Vector<bool, kN> operator!=(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = !zisc::equal(lhs, rhs[index]);
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
Vector<bool, kN> operator!=(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator<(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] < rhs[index];
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
Vector<bool, kN> operator<(const Type1& lhs,
                           const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs < rhs[index];
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
Vector<bool, kN> operator<(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator<=(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] <= rhs[index];
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
Vector<bool, kN> operator<=(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs <= rhs[index];
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
Vector<bool, kN> operator<=(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator>(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] > rhs[index];
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
Vector<bool, kN> operator>(const Type1& lhs,
                           const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs > rhs[index];
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
Vector<bool, kN> operator>(const Vector<Type1, kN>& lhs,
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
Vector<bool, kN> operator>=(const Vector<Type, kN>& lhs,
                            const Vector<Type, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] >= rhs[index];
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
Vector<bool, kN> operator>=(const Type1& lhs,
                            const Vector<Type2, kN>& rhs) noexcept
{
  Vector<bool, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = lhs >= rhs[index];
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
Vector<bool, kN> operator>=(const Vector<Type1, kN>& lhs,
                            const Type2& rhs) noexcept
{
  return (rhs <= lhs);
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload2(
    const size_t offset,
    const Pointer p) noexcept
{
  using T = std::remove_cvref_t<Pointer>;
  const auto result = Vec::vloadn<T, 2>(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto VectorData::vload2(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload2(offset, p.get());
  return result;
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload3(
    const size_t offset,
    const Pointer p) noexcept
{
  using T = std::remove_cvref_t<Pointer>;
  const auto result = Vec::vloadn<T, 3>(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto VectorData::vload3(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload3(offset, p.get());
  return result;
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto VectorData::vload4(
    const size_t offset,
    const Pointer p) noexcept
{
  using T = std::remove_cvref_t<Pointer>;
  const auto result = Vec::vloadn<T, 4>(offset, p);
  return result;
}
/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto VectorData::vload4(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload4(offset, p.get());
  return result;
}

/*!
  */
inline
float VectorData::vload_half(
    const size_t offset,
    const half* p) noexcept
{
  const half* address = p + offset;
  const float result = zisc::cast<float>(*address);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float VectorData::vload_half(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload_half(offset, p.get());
  return result;
}

/*!
  */
inline
float2 VectorData::vload_half2(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = Vec::vload_halfn<2>(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float2 VectorData::vload_half2(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload_half2(offset, p.get());
  return result;
}

/*!
  */
inline
float3 VectorData::vload_half3(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = Vec::vload_halfn<3>(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float3 VectorData::vload_half3(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload_half3(offset, p.get());
  return result;
}

/*!
  */
inline
float4 VectorData::vload_half4(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = Vec::vload_halfn<4>(offset, p);
  return result;
}
/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float4 VectorData::vload_half4(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = vload_half4(offset, p.get());
  return result;
}

/*!
  */
template <typename Type> inline
void VectorData::vstore2(
    const Vector<Type, 2>& data,
    const size_t offset,
    const std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Vec::vstoren<T, 2>(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void VectorData::vstore2(
    const Vector<Type, 2>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore2(data, offset, p.get());
}

/*!
  */
template <typename Type> inline
void VectorData::vstore3(
    const Vector<Type, 3>& data,
    const size_t offset,
    const std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Vec::vstoren<T, 3>(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void VectorData::vstore3(
    const Vector<Type, 3>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore3(data, offset, p.get());
}

/*!
  */
template <typename Type> inline
void VectorData::vstore4(
    const Vector<Type, 4>& data,
    const size_t offset,
    const std::add_pointer_t<Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  Vec::vstoren<T, 4>(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void VectorData::vstore4(
    const Vector<Type, 4>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore4(data, offset, p.get());
}

/*!
  */
inline
void VectorData::vstore_half(
    const float data,
    const size_t offset,
    half* p) noexcept
{
  half* address = p + offset;
  const half fdata = zisc::cast<half>(data);
  *address = fdata;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void VectorData::vstore_half(
    const float data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore_half(data, offset, p.get());
}

/*!
  */
inline
void VectorData::vstore_half2(
    const float2& data,
    const size_t offset,
    half* p) noexcept
{
  Vec::vstore_halfn(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void VectorData::vstore_half2(
    const float2& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore_half2(data, offset, p.get());
}

/*!
  */
inline
void VectorData::vstore_half3(
    const float3& data,
    const size_t offset,
    half* p) noexcept
{
  Vec::vstore_halfn(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void VectorData::vstore_half3(
    const float3& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore_half3(data, offset, p.get());
}

/*!
  */
inline
void VectorData::vstore_half4(
    const float4& data,
    const size_t offset,
    half* p) noexcept
{
  Vec::vstore_halfn(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void VectorData::vstore_half4(
    const float4& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  vstore_half4(data, offset, p.get());
}

template <typename Type, size_t kN> inline
Vector<Type, kN> VectorData::Vec::vloadn(
    const size_t offset,
    const Type* p) noexcept
{
  Vector<Type, kN> data;
  const Type* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    data[i] = address[i];
  return data;
}

template <size_t kN> inline
Vector<float, kN> VectorData::Vec::vload_halfn(
    const size_t offset,
    const half* p) noexcept
{
  Vector<float, kN> data;
  const half* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i) {
    const float v = zisc::cast<float>(address[i]);
    data[i] = v;
  }
  return data;
}

template <typename Type, size_t kN> inline
void VectorData::Vec::vstoren(
    const Vector<Type, kN>& data,
    const size_t offset,
    Type* p) noexcept
{
  Type* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i)
    address[i] = data[i];
}

template <size_t kN> inline
void VectorData::Vec::vstore_halfn(
    const Vector<float, kN>& data,
    const size_t offset,
    half* p) noexcept
{
  half* address = p + offset * kN;
  for (size_t i = 0; i < kN; ++i) {
    const half v = zisc::cast<half>(data[i]);
    address[i] = v;
  }
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto vload2(const size_t offset,
            const Pointer p) noexcept
{
  const auto result = VectorData::vload2(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto vload2(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload2<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto vload3(const size_t offset,
            const Pointer p) noexcept
{
  const auto result = VectorData::vload3(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto vload3(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload3<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
template <zisc::Pointer Pointer> inline
auto vload4(const size_t offset,
            const Pointer p) noexcept
{
  const auto result = VectorData::vload4(offset, p);
  return result;
}
/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto vload4(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload4<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
inline
float vload_half(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = VectorData::vload_half(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float vload_half(const size_t offset,
                 const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload_half<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
inline
float2 vload_half2(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = VectorData::vload_half2(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float2 vload_half2(const size_t offset,
                   const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload_half2<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
inline
float3 vload_half3(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = VectorData::vload_half3(offset, p);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float3 vload_half3(const size_t offset,
                   const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload_half3<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
inline
float4 vload_half4(
    const size_t offset,
    const half* p) noexcept
{
  const auto result = VectorData::vload_half4(offset, p);
  return result;
}
/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
float4 vload_half4(const size_t offset,
                   const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
{
  const auto result = VectorData::vload_half4<kAddressSpaceType, Type>(offset, p);
  return result;
}

/*!
  */
template <typename Type> inline
void vstore2(const Vector<Type, 2>& data,
            const size_t offset,
            const std::add_pointer_t<Type> p) noexcept
{
  VectorData::vstore2(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore2<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
template <typename Type> inline
void vstore3(const Vector<Type, 3>& data,
            const size_t offset,
            const std::add_pointer_t<Type> p) noexcept
{
  VectorData::vstore3(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore3<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
template <typename Type> inline
void vstore4(const Vector<Type, 4>& data,
            const size_t offset,
            const std::add_pointer_t<Type> p) noexcept
{
  VectorData::vstore4(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore4<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
inline
void vstore_half(
    const float data,
    const size_t offset,
    half* p) noexcept
{
  VectorData::vstore_half(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void vstore_half(
    const float data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore_half<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
inline
void vstore_half2(
    const float2& data,
    const size_t offset,
    half* p) noexcept
{
  VectorData::vstore_half2(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void vstore_half2(
    const float2& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore_half2<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
inline
void vstore_half3(
    const float3& data,
    const size_t offset,
    half* p) noexcept
{
  VectorData::vstore_half3(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void vstore_half3(
    const float3& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore_half3<kAddressSpaceType, Type>(data, offset, p);
}

/*!
  */
inline
void vstore_half4(
    const float4& data,
    const size_t offset,
    half* p) noexcept
{
  VectorData::vstore_half4(data, offset, p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, Half Type> inline
void vstore_half4(
    const float4& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  VectorData::vstore_half4<kAddressSpaceType, Type>(data, offset, p);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_VECTOR_INL_HPP
