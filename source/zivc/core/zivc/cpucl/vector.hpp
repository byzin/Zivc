/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_VECTOR_HPP
#define ZIVC_CPUCL_VECTOR_HPP

// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <span>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam kN No description.
  */
template <Arithmetic Type, size_t kN> struct Vector;

#if defined(Z_MSVC)
constexpr size_t kVectorAlignmentMax = 64; //!< \attention MSVC doesn't support more than 64 byte alignment
#else // Z_MSVC
constexpr size_t kVectorAlignmentMax = (std::numeric_limits<size_t>::max)();
#endif // Z_MSVC

//! Represent a vector alignment
template <Arithmetic Type, size_t kN>
constexpr size_t kVectorAlignment = (std::min)(kN * sizeof(Type), kVectorAlignmentMax);

// Comparison type

//! Integer type that is used as a comparison result of the given type
template <typename Type>
struct CompResultType;

//! Integer type that is used as a comparison result of the given type
template <typename Type>
using CompResult = typename CompResultType<std::remove_cvref_t<Type>>::ResultT;

//! Integer type that is used as a comparison result of the given type
template <Arithmetic Type, std::size_t kN>
using CompResultVec = CompResult<Vector<Type, kN>>;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(kVectorAlignment<T, 2>) Vector<T, 2>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstT = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstT>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstT>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept = default;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1) noexcept;


  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr Vector* alignedThis() noexcept;

  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr const Vector* alignedThis() const noexcept;

  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the size of the vector
  static constexpr size_type size() noexcept;

  //

  //! Apply the given function to all elements
  template <Arithmetic R, zisc::InvocableR<R, T> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      const Vector<T1, 2>& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      Vector<T1, 2>* y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const T1& x,
                                      const Vector& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      const Vector<T1, 2>& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      Vector<T1, 2>* z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const T1& x,
                                      const T1& y,
                                      const Vector& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func>
  static constexpr Vector<R, 2> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y,
                                      const T1& z) noexcept;

  //! Read a data
  template <Arithmetic T1>
  static constexpr Vector load(const std::span<T1> data) noexcept;

  //! Write to the data
  template <Arithmetic T1>
  static constexpr void store(const Vector& v, std::span<T1> data) noexcept;

  //! Sum up the elements
  constexpr Type sum() const noexcept;


  Type x,
       y;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(kVectorAlignment<T, 4>) Vector<T, 3>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstT = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstT>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstT>;
  using Vector2 = Vector<T, 2>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept = default;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1, ConstReference v2) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, ConstReference v1) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, const Vector2& v1) noexcept;


  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr Vector* alignedThis() noexcept;

  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr const Vector* alignedThis() const noexcept;

  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the size of the vector
  static constexpr size_type size() noexcept;

  //

  //! Apply the given function to all elements
  template <Arithmetic R, zisc::InvocableR<R, T> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      const Vector<T1, 3>& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      Vector<T1, 3>* y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const T1& x,
                                      const Vector& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      const Vector<T1, 3>& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      Vector<T1, 3>* z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const T1& x,
                                      const T1& y,
                                      const Vector& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func>
  static constexpr Vector<R, 3> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y,
                                      const T1& z) noexcept;

  //! Read a data
  template <Arithmetic T1>
  static constexpr Vector load(const std::span<T1> data) noexcept;

  //! Write to the data
  template <Arithmetic T1>
  static constexpr void store(const Vector& v, std::span<T1> data) noexcept;

  //! Sum up the elements
  constexpr Type sum() const noexcept;


  Type x,
       y,
       z;
  [[maybe_unused]] Type pad_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(kVectorAlignment<T, 4>) Vector<T, 4>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstT = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstT>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstT>;
  using Vector2 = Vector<T, 2>;
  using Vector3 = Vector<T, 3>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept = default;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1,
                   ConstReference v2, ConstReference v3) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, ConstReference v1, ConstReference v2) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, const Vector2& v1, ConstReference v2) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1, const Vector2& v2) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, const Vector2& v1) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector3& v0, ConstReference v1) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, const Vector3& v1) noexcept;


  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr Vector* alignedThis() noexcept;

  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr const Vector* alignedThis() const noexcept;

  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the size of the vector
  static constexpr size_type size() noexcept;

  //

  //! Apply the given function to all elements
  template <Arithmetic R, zisc::InvocableR<R, T> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      const Vector<T1, 4>& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      Vector<T1, 4>* y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const T1& x,
                                      const Vector& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      const Vector<T1, 4>& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      Vector<T1, 4>* z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const T1& x,
                                      const T1& y,
                                      const Vector& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func>
  static constexpr Vector<R, 4> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y,
                                      const T1& z) noexcept;

  //! Read a data
  template <Arithmetic T1>
  static constexpr Vector load(const std::span<T1> data) noexcept;

  //! Write to the data
  template <Arithmetic T1>
  static constexpr void store(const Vector& v, std::span<T1> data) noexcept;

  //! Sum up the elements
  constexpr Type sum() const noexcept;


  Type x,
       y,
       z,
       w;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(kVectorAlignment<T, 8>) Vector<T, 8>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstT = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstT>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstT>;
  using Vector2 = Vector<T, 2>;
  using Vector3 = Vector<T, 3>;
  using Vector4 = Vector<T, 4>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept = default;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1,
                   ConstReference v2, ConstReference v3,
                   ConstReference v4, ConstReference v5,
                   ConstReference v6, ConstReference v7) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, const Vector2& v1,
                   const Vector2& v2, const Vector2& v3) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector4& v0, const Vector4& v1) noexcept;


  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr Vector* alignedThis() noexcept;

  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr const Vector* alignedThis() const noexcept;

  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the size of the vector
  static constexpr size_type size() noexcept;

  //

  //! Apply the given function to all elements
  template <Arithmetic R, zisc::InvocableR<R, T> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      const Vector<T1, 8>& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      Vector<T1, 8>* y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const T1& x,
                                      const Vector& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      const Vector<T1, 8>& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      const Vector& y,
                                      Vector<T1, 8>* z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const T1& x,
                                      const T1& y,
                                      const Vector& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func>
  static constexpr Vector<R, 8> apply(const Func& func,
                                      const Vector& x,
                                      const T1& y,
                                      const T1& z) noexcept;

  //! Read a data
  template <Arithmetic T1>
  static constexpr Vector load(const std::span<T1> data) noexcept;

  //! Write to the data
  template <Arithmetic T1>
  static constexpr void store(const Vector& v, std::span<T1> data) noexcept;

  //! Sum up the elements
  constexpr Type sum() const noexcept;


  Type s0,
       s1,
       s2,
       s3,
       s4,
       s5,
       s6,
       s7;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(kVectorAlignment<T, 16>) Vector<T, 16>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstT = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstT>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstT>;
  using Vector2 = Vector<T, 2>;
  using Vector3 = Vector<T, 3>;
  using Vector4 = Vector<T, 4>;
  using Vector8 = Vector<T, 8>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept = default;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1,
                   ConstReference v2, ConstReference v3,
                   ConstReference v4, ConstReference v5,
                   ConstReference v6, ConstReference v7,
                   ConstReference v8, ConstReference v9,
                   ConstReference va, ConstReference vb,
                   ConstReference vc, ConstReference vd,
                   ConstReference ve, ConstReference vf) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, const Vector2& v1,
                   const Vector2& v2, const Vector2& v3,
                   const Vector2& v4, const Vector2& v5,
                   const Vector2& v6, const Vector2& v7) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector4& v0, const Vector4& v1,
                   const Vector4& v2, const Vector4& v3) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector8& v0, const Vector8& v1) noexcept;


  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr Vector* alignedThis() noexcept;

  //! Return this pointer that is informed as aligned to at least 2 * sizeof(T)
  constexpr const Vector* alignedThis() const noexcept;

  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the size of the vector
  static constexpr size_type size() noexcept;

  //

  //! Apply the given function to all elements
  template <Arithmetic R, zisc::InvocableR<R, T> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       const Vector<T1, 16>& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1*> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       Vector<T1, 16>* y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const T1& x,
                                       const Vector& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       const T1& y) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       const Vector& y,
                                       const Vector<T1, 16>& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T, T1*> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       const Vector& y,
                                       Vector<T1, 16>* z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T1, T1, T> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const T1& x,
                                       const T1& y,
                                       const Vector& z) noexcept;

  //! Apply the given function to all elements
  template <Arithmetic R, Arithmetic T1, zisc::InvocableR<R, T, T1, T1> Func>
  static constexpr Vector<R, 16> apply(const Func& func,
                                       const Vector& x,
                                       const T1& y,
                                       const T1& z) noexcept;

  //! Read a data
  template <Arithmetic T1>
  static constexpr Vector load(const std::span<T1> data) noexcept;

  //! Write to the data
  template <Arithmetic T1>
  static constexpr void store(const Vector& v, std::span<T1> data) noexcept;

  //! Sum up the elements
  constexpr Type sum() const noexcept;


  Type s0,
       s1,
       s2,
       s3,
       s4,
       s5,
       s6,
       s7,
       s8,
       s9,
       sa,
       sb,
       sc,
       sd,
       se,
       sf;
};

// Assignment

//! Apply addition assignment to each element of the given vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply addition assignment to each element of the given vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN>& operator+=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply subtraction assignment to each element of the given vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply subtraction assignment to each element of the given vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN>& operator-=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply multiplication assignment to each element of the given vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply multiplication assignment to each element of the given vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN>& operator*=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply division assignment to each element of the given vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply division assignment to each element of the given vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN>& operator/=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply modulo assignment to each element of the given vector
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply modulo assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator%=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply bitwise AND assignment to each element of the given vector
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply bitwise AND assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator&=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply bitwise OR assignment to each element of the given vector
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply bitwise OR assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator|=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply bitwise XOR assignment to each element of the given vector
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                                       const Vector<Type, kN>& rhs) noexcept;

//! Apply bitwise XOR assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator^=(Vector<Type1, kN>& lhs,
                                        const Type2& rhs) noexcept;

//! Apply bitwise left shift assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                                         const Vector<Type2, kN>& rhs) noexcept;

//! Apply bitwise left shift assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                                         const Type2& rhs) noexcept;

//! Apply bitwise right shift assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                                         const Vector<Type2, kN>& rhs) noexcept;

//! Apply bitwise right shift assignment to each element of the given vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                                         const Type2& rhs) noexcept;

// Increment/decrement

//! Pre-increment a vector
template <Arithmetic Type, size_t kN>
Vector<Type, kN>& operator++(Vector<Type, kN>& value) noexcept;

//! Pre-decrement a vector
template <Arithmetic Type, size_t kN>
Vector<Type, kN>& operator--(Vector<Type, kN>& value) noexcept;

//! Post-increment a vector
template <Arithmetic Type, size_t kN>
Vector<Type, kN> operator++(Vector<Type, kN>& value, int) noexcept;

//! Post-decrement a vector
template <Arithmetic Type, size_t kN>
Vector<Type, kN> operator--(Vector<Type, kN>& value, int) noexcept;

// Arithmetic

//! Unary plus
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator+(const Vector<Type, kN>& value) noexcept;

//! Unary minus
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator-(const Vector<Type, kN>& value) noexcept;

//! Compute an addition of two vectors
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute an addition of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type2, kN> operator+(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute an addition of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN> operator+(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a subtract of two vectors
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a subtract of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type2, kN> operator-(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a subtract of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN> operator-(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a multiplication of two vectors
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a multiplication of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type2, kN> operator*(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a multiplication of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN> operator*(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a division of two vectors
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a division of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type2, kN> operator/(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a division of a scalar and a vector
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<Type1, kN> operator/(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a reminder of two vectors
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a reminder of a scalar and a vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type2, kN> operator%(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a reminder of a scalar and a vector
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator%(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a bitwise NOT
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN> operator~(const Vector<Type, kN>& value) noexcept;

//! Compute a bitwise AND
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise AND
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type2, kN> operator&(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a bitwise AND
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator&(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a bitwise OR
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise OR
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type2, kN> operator|(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a bitwise OR
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator|(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a bitwise XOR
template <zisc::Integer Type, size_t kN>
constexpr Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise XOR
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type2, kN> operator^(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Compute a bitwise XOR
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator^(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Compute a left shift
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                                       const Vector<Type2, kN>& rhs) noexcept;

//! Compute a left shift
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                                       const Type2& rhs) noexcept;

//! Compute a right shift
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                                       const Vector<Type2, kN>& rhs) noexcept;

//! Compute a right shift
template <zisc::Integer Type1, zisc::Integer Type2, size_t kN>
constexpr Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                                       const Type2& rhs) noexcept;

// Logical

//! Logical NOT
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator!(const Vector<Type, kN>& value) noexcept;

//! Logical AND
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator&&(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//! Logical AND
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator&&(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//! Logical AND
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator&&(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

//! Logical OR
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator||(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//! Logical OR
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator||(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//! Logical OR
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator||(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

// Comparison

//! Check if two values are equal in value
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator==(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator==(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator==(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator!=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator!=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator!=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator<(const Vector<Type, kN>& lhs,
                                            const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator<(const Type1& lhs,
                                             const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator<(const Vector<Type1, kN>& lhs,
                                             const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator<=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator<=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator<=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator>(const Vector<Type, kN>& lhs,
                                            const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator>(const Type1& lhs,
                                             const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator>(const Vector<Type1, kN>& lhs,
                                             const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr CompResultVec<Type, kN> operator>=(const Vector<Type, kN>& lhs,
                                             const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type2, kN> operator>=(const Type1& lhs,
                                              const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr CompResultVec<Type1, kN> operator>=(const Vector<Type1, kN>& lhs,
                                              const Type2& rhs) noexcept;

// Type aliases
using char2 = Vector<int8b, 2>;
using char3 = Vector<int8b, 3>;
using char4 = Vector<int8b, 4>;
using char8 = Vector<int8b, 8>;
using char16 = Vector<int8b, 16>;

using uchar2 = Vector<uint8b, 2>;
using uchar3 = Vector<uint8b, 3>;
using uchar4 = Vector<uint8b, 4>;
using uchar8 = Vector<uint8b, 8>;
using uchar16 = Vector<uint8b, 16>;

using short2 = Vector<int16b, 2>;
using short3 = Vector<int16b, 3>;
using short4 = Vector<int16b, 4>;
using short8 = Vector<int16b, 8>;
using short16 = Vector<int16b, 16>;

using ushort2 = Vector<uint16b, 2>;
using ushort3 = Vector<uint16b, 3>;
using ushort4 = Vector<uint16b, 4>;
using ushort8 = Vector<uint16b, 8>;
using ushort16 = Vector<uint16b, 16>;

using int2 = Vector<int32b, 2>;
using int3 = Vector<int32b, 3>;
using int4 = Vector<int32b, 4>;
using int8 = Vector<int32b, 8>;
using int16 = Vector<int32b, 16>;

using uint2 = Vector<uint32b, 2>;
using uint3 = Vector<uint32b, 3>;
using uint4 = Vector<uint32b, 4>;
using uint8 = Vector<uint32b, 8>;
using uint16 = Vector<uint32b, 16>;

using long2 = Vector<int64b, 2>;
using long3 = Vector<int64b, 3>;
using long4 = Vector<int64b, 4>;
using long8 = Vector<int64b, 8>;
using long16 = Vector<int64b, 16>;

using ulong2 = Vector<uint64b, 2>;
using ulong3 = Vector<uint64b, 3>;
using ulong4 = Vector<uint64b, 4>;
using ulong8 = Vector<uint64b, 8>;
using ulong16 = Vector<uint64b, 16>;

using half2 = Vector<half, 2>;
using half3 = Vector<half, 3>;
using half4 = Vector<half, 4>;
using half8 = Vector<half, 8>;
using half16 = Vector<half, 16>;

using float2 = Vector<float, 2>;
using float3 = Vector<float, 3>;
using float4 = Vector<float, 4>;
using float8 = Vector<float, 8>;
using float16 = Vector<float, 16>;

using double2 = Vector<double, 2>;
using double3 = Vector<double, 3>;
using double4 = Vector<double, 4>;
using double8 = Vector<double, 8>;
using double16 = Vector<double, 16>;


// Load and store functions

/*!
  \brief No brief description

  No detailed description.
  */
class VectorData
{
 public:
  //! Read a data from address (p + offset * 2)
  template <zisc::Pointer Pointer>
  static auto load2(const size_t offset,
                    const Pointer p) noexcept;

  //! Read a data from address (p + offset * 2)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load2(const size_t offset,
                    const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 2)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load2(const size_t offset,
                    const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <zisc::Pointer Pointer>
  static auto load3(const size_t offset,
                    const Pointer p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load3(const size_t offset,
                    const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load3(const size_t offset,
                    const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <zisc::Pointer Pointer>
  static auto load4(const size_t offset,
                    const Pointer p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load4(const size_t offset,
                    const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load4(const size_t offset,
                    const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

  //! Read a data from address (p + offset * 8)
  template <zisc::Pointer Pointer>
  static auto load8(const size_t offset,
                    const Pointer p) noexcept;

  //! Read a data from address (p + offset * 8)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load8(const size_t offset,
                    const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 8)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load8(const size_t offset,
                    const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

  //! Read a data from address (p + offset * 16)
  template <zisc::Pointer Pointer>
  static auto load16(const size_t offset,
                     const Pointer p) noexcept;

  //! Read a data from address (p + offset * 16)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 16)
  template <AddressSpaceType kASpaceType, typename Type>
  static auto load16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  static float loadHalf(const size_t offset,
                        const half* p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float loadHalf(const size_t offset,
                        const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float loadHalf(const size_t offset,
                        const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  static float2 loadHalf2(const size_t offset,
                          const half* p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float2 loadHalf2(const size_t offset,
                          const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float2 loadHalf2(const size_t offset,
                          const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  static float3 loadHalf3(const size_t offset,
                          const half* p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float3 loadHalf3(const size_t offset,
                          const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float3 loadHalf3(const size_t offset,
                          const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  static float4 loadHalf4(const size_t offset,
                          const half* p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float4 loadHalf4(const size_t offset,
                          const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float4 loadHalf4(const size_t offset,
                          const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Read half data from address (p + offset * 8). The address must be 16bit aligned
  static float8 loadHalf8(const size_t offset,
                          const half* p) noexcept;

  //! Read half data from address (p + offset * 8). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float8 loadHalf8(const size_t offset,
                          const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset * 8). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float8 loadHalf8(const size_t offset,
                          const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Read half data from address (p + offset * 16). The address must be 16bit aligned
  static float16 loadHalf16(const size_t offset,
                            const half* p) noexcept;

  //! Read half data from address (p + offset * 16). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float16 loadHalf16(const size_t offset,
                            const AddressSpacePointer<kASpaceType, half>& p) noexcept;

  //! Read half data from address (p + offset * 16). The address must be 16bit aligned
  template <AddressSpaceType kASpaceType>
  static float16 loadHalf16(const size_t offset,
                            const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

  //! Write to the address (p + offset * 2)
  template <typename Type>
  static void store2(const Vector<Type, 2>& data,
                     const size_t offset,
                     std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 2)
  template <AddressSpaceType kASpaceType, typename Type>
  static void store2(const Vector<Type, 2>& data,
                     const size_t offset,
                     AddressSpacePointer<kASpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 3)
  template <typename Type>
  static void store3(const Vector<Type, 3>& data,
                     const size_t offset,
                     std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 3)
  template <AddressSpaceType kASpaceType, typename Type>
  static void store3(const Vector<Type, 3>& data,
                     const size_t offset,
                     AddressSpacePointer<kASpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 4)
  template <typename Type>
  static void store4(const Vector<Type, 4>& data,
                     const size_t offset,
                     const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 4)
  template <AddressSpaceType kASpaceType, typename Type>
  static void store4(const Vector<Type, 4>& data,
                     const size_t offset,
                     AddressSpacePointer<kASpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 8)
  template <typename Type>
  static void store8(const Vector<Type, 8>& data,
                     const size_t offset,
                     const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 8)
  template <AddressSpaceType kASpaceType, typename Type>
  static void store8(const Vector<Type, 8>& data,
                     const size_t offset,
                     AddressSpacePointer<kASpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 16)
  template <typename Type>
  static void store16(const Vector<Type, 16>& data,
                      const size_t offset,
                      const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 16)
  template <AddressSpaceType kASpaceType, typename Type>
  static void store16(const Vector<Type, 16>& data,
                      const size_t offset,
                      AddressSpacePointer<kASpaceType, Type> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset)
  static void storeHalf(const float data,
                        const size_t offset,
                        half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset)
  template <AddressSpaceType kASpaceType>
  static void storeHalf(const float data,
                        const size_t offset,
                        AddressSpacePointer<kASpaceType, half> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 2)
  static void storeHalf2(const float2& data,
                         const size_t offset,
                         half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 2)
  template <AddressSpaceType kASpaceType>
  static void storeHalf2(const float2& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, half> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 3)
  static void storeHalf3(const float3& data,
                         const size_t offset,
                         half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 3)
  template <AddressSpaceType kASpaceType>
  static void storeHalf3(const float3& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, half> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 4)
  static void storeHalf4(const float4& data,
                         const size_t offset,
                         half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 4)
  template <AddressSpaceType kASpaceType>
  static void storeHalf4(const float4& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, half> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 8)
  static void storeHalf8(const float8& data,
                         const size_t offset,
                         half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 8)
  template <AddressSpaceType kASpaceType>
  static void storeHalf8(const float8& data,
                         const size_t offset,
                         AddressSpacePointer<kASpaceType, half> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 16)
  static void storeHalf16(const float16& data,
                          const size_t offset,
                          half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 16)
  template <AddressSpaceType kASpaceType>
  static void storeHalf16(const float16& data,
                          const size_t offset,
                          AddressSpacePointer<kASpaceType, half> p) noexcept;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  struct Impl
  {
    //!
    template <typename Type, size_t kN>
    static Vector<Type, kN> loadN(const size_t offset,
                                  const Type* p) noexcept;

    //!
    template <size_t kN>
    static Vector<float, kN> loadHalfN(const size_t offset,
                                       const half* p) noexcept;

    //!
    template <typename Type, size_t kN>
    static void storeN(const Vector<Type, kN>& data,
                       const size_t offset,
                       Type* p) noexcept;

    //!
    template <size_t kN>
    static void storeHalfN(const Vector<float, kN>& data,
                           const size_t offset,
                           half* p) noexcept;
  };
};

//! Read a data from address (p + offset * 2)
template <zisc::Pointer Pointer>
auto vload2(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 2)
template <AddressSpaceType kASpaceType, typename Type>
auto vload2(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 2)
template <AddressSpaceType kASpaceType, typename Type>
auto vload2(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;


//! Read a data from address (p + offset * 3)
template <zisc::Pointer Pointer>
auto vload3(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 3)
template <AddressSpaceType kASpaceType, typename Type>
auto vload3(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 3)
template <AddressSpaceType kASpaceType, typename Type>
auto vload3(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

//! Read a data from address (p + offset * 4)
template <zisc::Pointer Pointer>
auto vload4(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 4)
template <AddressSpaceType kASpaceType, typename Type>
auto vload4(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 4)
template <AddressSpaceType kASpaceType, typename Type>
auto vload4(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

//! Read a data from address (p + offset * 8)
template <zisc::Pointer Pointer>
auto vload8(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 8)
template <AddressSpaceType kASpaceType, typename Type>
auto vload8(const size_t offset,
            const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 8)
template <AddressSpaceType kASpaceType, typename Type>
auto vload8(const size_t offset,
            const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

//! Read a data from address (p + offset * 16)
template <zisc::Pointer Pointer>
auto vload16(const size_t offset,
             const Pointer p) noexcept;

//! Read a data from address (p + offset * 16)
template <AddressSpaceType kASpaceType, typename Type>
auto vload16(const size_t offset,
             const AddressSpacePointer<kASpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 16)
template <AddressSpaceType kASpaceType, typename Type>
auto vload16(const size_t offset,
             const AddressSpacePointer<kASpaceType, const Type>& p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
float vload_half(const size_t offset,
                 const half* p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float vload_half(const size_t offset,
                 const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float vload_half(const size_t offset,
                 const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
float2 vload_half2(const size_t offset,
                   const half* p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float2 vload_half2(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float2 vload_half2(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
float3 vload_half3(const size_t offset,
                   const half* p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float3 vload_half3(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float3 vload_half3(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
float4 vload_half4(const size_t offset,
                   const half* p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float4 vload_half4(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float4 vload_half4(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Read half data from address (p + offset * 8). The address must be 16bit aligned
float8 vload_half8(const size_t offset,
                   const half* p) noexcept;

//! Read half data from address (p + offset * 8). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float8 vload_half8(const size_t offset,
                   const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset * 8). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float8 vload_half8(const size_t offset,
                   const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Read half data from address (p + offset * 16). The address must be 16bit aligned
float16 vload_half16(const size_t offset,
                     const half* p) noexcept;

//! Read half data from address (p + offset * 16). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float16 vload_half16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, half>& p) noexcept;

//! Read half data from address (p + offset * 16). The address must be 16bit aligned
template <AddressSpaceType kASpaceType>
float16 vload_half16(const size_t offset,
                     const AddressSpacePointer<kASpaceType, const half>& p) noexcept;

//! Write to the address (p + offset * 2)
template <typename Type>
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 2)
template <AddressSpaceType kASpaceType, typename Type>
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 3)
template <typename Type>
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 3)
template <AddressSpaceType kASpaceType, typename Type>
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 4)
template <typename Type>
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 4)
template <AddressSpaceType kASpaceType, typename Type>
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 8)
template <typename Type>
void vstore8(const Vector<Type, 8>& data,
             const size_t offset,
             std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 8)
template <AddressSpaceType kASpaceType, typename Type>
void vstore8(const Vector<Type, 8>& data,
             const size_t offset,
             AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 16)
template <typename Type>
void vstore16(const Vector<Type, 16>& data,
              const size_t offset,
              std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 16)
template <AddressSpaceType kASpaceType, typename Type>
void vstore16(const Vector<Type, 16>& data,
              const size_t offset,
              AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset)
void vstore_half(const float data,
                 const size_t offset,
                 half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset)
template <AddressSpaceType kASpaceType>
void vstore_half(const float data,
                 const size_t offset,
                 AddressSpacePointer<kASpaceType, half> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 2)
void vstore_half2(const float2& data,
                  const size_t offset,
                  half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 2)
template <AddressSpaceType kASpaceType>
void vstore_half2(const float2& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 3)
void vstore_half3(const float3& data,
                  const size_t offset,
                  half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 3)
template <AddressSpaceType kASpaceType>
void vstore_half3(const float3& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 4)
void vstore_half4(const float4& data,
                  const size_t offset,
                  half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 4)
template <AddressSpaceType kASpaceType>
void vstore_half4(const float4& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 8)
void vstore_half8(const float8& data,
                  const size_t offset,
                  half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 8)
template <AddressSpaceType kASpaceType>
void vstore_half8(const float8& data,
                  const size_t offset,
                  AddressSpacePointer<kASpaceType, half> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 16)
void vstore_half16(const float16& data,
                   const size_t offset,
                   half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 16)
template <AddressSpaceType kASpaceType>
void vstore_half16(const float16& data,
                   const size_t offset,
                   AddressSpacePointer<kASpaceType, half> p) noexcept;

} // namespace zivc::cl

#include "vector-inl.hpp"

#endif // ZIVC_CPUCL_VECTOR_HPP
