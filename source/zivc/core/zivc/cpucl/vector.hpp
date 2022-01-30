/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_VECTOR_HPP
#define ZIVC_CPUCL_VECTOR_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam kN No description.
  */
template <Arithmetic Type, size_t kN> struct Vector;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(2 * sizeof(T)) Vector<T, 2>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstType = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;
  using iterator = Pointer;
  using const_iterator = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1) noexcept;


  //! Return the reference to the element by the index
  constexpr Reference operator[](const size_type index) noexcept;

  //! Return the reference to the element by the index
  constexpr ConstReference operator[](const size_type index) const noexcept;

  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  // Iterators

  //! Return an iterator to the beginning
  constexpr iterator begin() noexcept;

  //! Return an iterator to the beginning
  constexpr const_iterator begin() const noexcept;

  //! Return an iterator to the endif
  constexpr iterator end() noexcept;

  //! Return an iterator to the endif
  constexpr const_iterator end() const noexcept;


  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the direct access to the underlying array
  constexpr Pointer data() noexcept;

  //! Return the direct access to the underlying array
  constexpr ConstPointer data() const noexcept;

  //! Return the reference to the element by the index
  constexpr Reference get(const size_type index) noexcept; 

  //! Return the reference to the element by the index
  constexpr ConstReference get(const size_type index) const noexcept; 

  //! Return the size of the vector
  static constexpr size_type size() noexcept;


  Type x;
  Type y;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(4 * sizeof(T)) Vector<T, 3>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstType = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;
  using Vector2 = Vector<T, 2>;


  // STL conpatible member types
  using value_type = Type;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;
  using iterator = Pointer;
  using const_iterator = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept;

  //! Fill all elements with the given value v
  constexpr Vector(ConstReference v) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, ConstReference v1, ConstReference v2) noexcept;

  //! Initialize a vector
  constexpr Vector(const Vector2& v0, ConstReference v1) noexcept;

  //! Initialize a vector
  constexpr Vector(ConstReference v0, const Vector2& v1) noexcept;


  //! Return the reference to the element by the index
  constexpr Reference operator[](const size_type index) noexcept;

  //! Return the reference to the element by the index
  constexpr ConstReference operator[](const size_type index) const noexcept;

  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  // Iterators

  //! Return an iterator to the beginning
  constexpr iterator begin() noexcept;

  //! Return an iterator to the beginning
  constexpr const_iterator begin() const noexcept;

  //! Return an iterator to the endif
  constexpr iterator end() noexcept;

  //! Return an iterator to the endif
  constexpr const_iterator end() const noexcept;


  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the direct access to the underlying array
  constexpr Pointer data() noexcept;

  //! Return the direct access to the underlying array
  constexpr ConstPointer data() const noexcept;

  //! Return the reference to the element by the index
  constexpr Reference get(const size_type index) noexcept; 

  //! Return the reference to the element by the index
  constexpr ConstReference get(const size_type index) const noexcept; 

  //! Return the size of the vector
  static constexpr size_type size() noexcept;


  Type x;
  Type y;
  Type z;
  [[maybe_unused]] Type pad_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <Arithmetic T>
struct alignas(4 * sizeof(T)) Vector<T, 4>
{
  //
  using Type = std::remove_cvref_t<T>;
  using ConstType = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;
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
  using iterator = Pointer;
  using const_iterator = ConstPointer;


  //! Initialize a vector
  constexpr Vector() noexcept;

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


  //! Return the reference to the element by the index
  constexpr Reference operator[](const size_type index) noexcept;

  //! Return the reference to the element by the index
  constexpr ConstReference operator[](const size_type index) const noexcept;

  //! Check if all elements are true
  constexpr operator bool() const noexcept;


  // Iterators

  //! Return an iterator to the beginning
  constexpr iterator begin() noexcept;

  //! Return an iterator to the beginning
  constexpr const_iterator begin() const noexcept;

  //! Return an iterator to the endif
  constexpr iterator end() noexcept;

  //! Return an iterator to the endif
  constexpr const_iterator end() const noexcept;


  //! Return the alignment of the vector
  static constexpr size_type alignment() noexcept;

  //! Return the direct access to the underlying array
  constexpr Pointer data() noexcept;

  //! Return the direct access to the underlying array
  constexpr ConstPointer data() const noexcept;

  //! Return the reference to the element by the index
  constexpr Reference get(const size_type index) noexcept; 

  //! Return the reference to the element by the index
  constexpr ConstReference get(const size_type index) const noexcept; 

  //! Return the size of the vector
  static constexpr size_type size() noexcept;


  Type x;
  Type y;
  Type z;
  Type w;
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
constexpr Vector<Type, kN>& operator++(Vector<Type, kN>& value) noexcept;

//! Pre-decrement a vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN>& operator--(Vector<Type, kN>& value) noexcept;

//! Post-increment a vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator++(Vector<Type, kN>& value, int) noexcept;

//! Post-decrement a vector
template <Arithmetic Type, size_t kN>
constexpr Vector<Type, kN> operator--(Vector<Type, kN>& value, int) noexcept;

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
constexpr Vector<bool, kN> operator!(const Vector<Type, kN>& value) noexcept;

//! Logical AND
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator&&(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//! Logical AND
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator&&(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Logical AND
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator&&(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Logical OR
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator||(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//! Logical OR
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator||(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Logical OR
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator||(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

// Comparison

//! Check if two values are equal in value
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator==(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator==(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator==(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator!=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator!=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator!=(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator<(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator<(const Type1& lhs,
                                     const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator<(const Vector<Type1, kN>& lhs,
                                     const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator<=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator<=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator<=(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator>(const Vector<Type, kN>& lhs,
                                     const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator>(const Type1& lhs,
                                     const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator>(const Vector<Type1, kN>& lhs,
                                     const Type2& rhs) noexcept;

//!
template <Arithmetic Type, size_t kN>
constexpr Vector<bool, kN> operator>=(const Vector<Type, kN>& lhs,
                                      const Vector<Type, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator>=(const Type1& lhs,
                                      const Vector<Type2, kN>& rhs) noexcept;

//!
template <Arithmetic Type1, Arithmetic Type2, size_t kN>
constexpr Vector<bool, kN> operator>=(const Vector<Type1, kN>& lhs,
                                      const Type2& rhs) noexcept;

// Type aliases
using char2 = Vector<int8b, 2>;
using char3 = Vector<int8b, 3>;
using char4 = Vector<int8b, 4>;

using uchar2 = Vector<uint8b, 2>;
using uchar3 = Vector<uint8b, 3>;
using uchar4 = Vector<uint8b, 4>;

using short2 = Vector<int16b, 2>;
using short3 = Vector<int16b, 3>;
using short4 = Vector<int16b, 4>;

using ushort2 = Vector<uint16b, 2>;
using ushort3 = Vector<uint16b, 3>;
using ushort4 = Vector<uint16b, 4>;

using int2 = Vector<int32b, 2>;
using int3 = Vector<int32b, 3>;
using int4 = Vector<int32b, 4>;

using uint2 = Vector<uint32b, 2>;
using uint3 = Vector<uint32b, 3>;
using uint4 = Vector<uint32b, 4>;

using long2 = Vector<int64b, 2>;
using long3 = Vector<int64b, 3>;
using long4 = Vector<int64b, 4>;

using ulong2 = Vector<uint64b, 2>;
using ulong3 = Vector<uint64b, 3>;
using ulong4 = Vector<uint64b, 4>;

using half2 = Vector<half, 2>;
using half3 = Vector<half, 3>;
using half4 = Vector<half, 4>;

using float2 = Vector<float, 2>;
using float3 = Vector<float, 3>;
using float4 = Vector<float, 4>;

using double2 = Vector<double, 2>;
using double3 = Vector<double, 3>;
using double4 = Vector<double, 4>;


// Load and store functions

/*!
  */
class VectorData
{
 public:
  //! Read a data from address (p + offset * 2)
  template <zisc::Pointer Pointer>
  static auto vload2(
      const size_t offset,
      const Pointer p) noexcept;

  //! Read a data from address (p + offset * 2)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static auto vload2(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <zisc::Pointer Pointer>
  static auto vload3(
      const size_t offset,
      const Pointer p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static auto vload3(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <zisc::Pointer Pointer>
  static auto vload4(
      const size_t offset,
      const Pointer p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static auto vload4(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  static float vload_half(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static float vload_half(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  static float2 vload_half2(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static float2 vload_half2(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  static float3 vload_half3(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static float3 vload_half3(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  static float4 vload_half4(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static float4 vload_half4(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Write to the address (p + offset * 2)
  template <typename Type>
  static void vstore2(
      const Vector<Type, 2>& data,
      const size_t offset,
      const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 2)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static void vstore2(
      const Vector<Type, 2>& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 3)
  template <typename Type>
  static void vstore3(
      const Vector<Type, 3>& data,
      const size_t offset,
      const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 3)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static void vstore3(
      const Vector<Type, 3>& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! Write to the address (p + offset * 4)
  template <typename Type>
  static void vstore4(
      const Vector<Type, 4>& data,
      const size_t offset,
      const std::add_pointer_t<Type> p) noexcept;

  //! Write to the address (p + offset * 4)
  template <AddressSpaceType kAddressSpaceType, typename Type>
  static void vstore4(
      const Vector<Type, 4>& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset)
  static void vstore_half(
      const float data,
      const size_t offset,
      half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset)
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static void vstore_half(
      const float data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 2)
  static void vstore_half2(
      const float2& data,
      const size_t offset,
      half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 2)
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static void vstore_half2(
      const float2& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 3)
  static void vstore_half3(
      const float3& data,
      const size_t offset,
      half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 3)
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static void vstore_half3(
      const float3& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 4)
  static void vstore_half4(
      const float4& data,
      const size_t offset,
      half* p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 4)
  template <AddressSpaceType kAddressSpaceType, Half Type>
  static void vstore_half4(
      const float4& data,
      const size_t offset,
      AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

 private:
  /*!
    */
  class Vec
  {
   public:
    template <typename Type, size_t kN>
    static Vector<Type, kN> vloadn(const size_t offset,
                                   const Type* p) noexcept;

    template <size_t kN>
    static Vector<float, kN> vload_halfn(const size_t offset,
                                         const half* p) noexcept;

    template <typename Type, size_t kN>
    static void vstoren(const Vector<Type, kN>& data,
                        const size_t offset,
                        Type* p) noexcept;

    template <size_t kN>
    static void vstore_halfn(const Vector<float, kN>& data,
                             const size_t offset,
                             half* p) noexcept;
  };
};

//! Read a data from address (p + offset * 2)
template <zisc::Pointer Pointer>
auto vload2(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 2)
template <AddressSpaceType kAddressSpaceType, typename Type>
auto vload2(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 3)
template <zisc::Pointer Pointer>
auto vload3(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 3)
template <AddressSpaceType kAddressSpaceType, typename Type>
auto vload3(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read a data from address (p + offset * 4)
template <zisc::Pointer Pointer>
auto vload4(const size_t offset,
            const Pointer p) noexcept;

//! Read a data from address (p + offset * 4)
template <AddressSpaceType kAddressSpaceType, typename Type>
auto vload4(const size_t offset,
            const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
float vload_half(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, Half Type>
float vload_half(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
float2 vload_half2(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, Half Type>
float2 vload_half2(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
float3 vload_half3(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, Half Type>
float3 vload_half3(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
float4 vload_half4(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, Half Type>
float4 vload_half4(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Write to the address (p + offset * 2)
template <typename Type>
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             const std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 2)
template <AddressSpaceType kAddressSpaceType, typename Type>
void vstore2(const Vector<Type, 2>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 3)
template <typename Type>
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             const std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 3)
template <AddressSpaceType kAddressSpaceType, typename Type>
void vstore3(const Vector<Type, 3>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! Write to the address (p + offset * 4)
template <typename Type>
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             const std::add_pointer_t<Type> p) noexcept;

//! Write to the address (p + offset * 4)
template <AddressSpaceType kAddressSpaceType, typename Type>
void vstore4(const Vector<Type, 4>& data,
             const size_t offset,
             AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset)
void vstore_half(
    const float data,
    const size_t offset,
    half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset)
template <AddressSpaceType kAddressSpaceType, Half Type>
void vstore_half(
    const float data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 2)
void vstore_half2(
    const float2& data,
    const size_t offset,
    half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 2)
template <AddressSpaceType kAddressSpaceType, Half Type>
void vstore_half2(
    const float2& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 3)
void vstore_half3(
    const float3& data,
    const size_t offset,
    half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 3)
template <AddressSpaceType kAddressSpaceType, Half Type>
void vstore_half3(
    const float3& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 4)
void vstore_half4(
    const float4& data,
    const size_t offset,
    half* p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 4)
template <AddressSpaceType kAddressSpaceType, Half Type>
void vstore_half4(
    const float4& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

} // namespace zivc::cl

#include "vector-inl.hpp"

#endif // ZIVC_CPUCL_VECTOR_HPP
