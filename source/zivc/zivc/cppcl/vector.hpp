/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_VECTOR_HPP
#define ZIVC_CL_VECTOR_HPP

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

namespace zivc {

namespace cl {

/*!
  */
template <typename Type, size_t kN> struct Vector;

/*!
  */
template <typename Type>
struct alignas(2 * sizeof(Type)) Vector<Type, 2>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize all elements of a vector by v
  Vector(const Type v) noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1) noexcept;


  //! Return a reference to the element
  Type& operator[](const size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const size_t index) const noexcept;


  //! Return the size of the vector
  static constexpr size_t size() noexcept;


  Type x,
       y;
};

/*!
  */
template <typename Type>
struct alignas(4 * sizeof(Type)) Vector<Type, 3>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize all elements of a vector by v
  Vector(const Type v) noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1, const Type v2) noexcept;


  //! Return a reference to the element
  Type& operator[](const size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const size_t index) const noexcept;


  //! Return the size of the vector
  static constexpr size_t size() noexcept;


  Type x,
       y,
       z,
       __padding_;
};

/*!
  */
template <typename Type>
struct alignas(4 * sizeof(Type)) Vector<Type, 4>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize all elements of a vector by v
  Vector(const Type v) noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1, const Type v2, const Type v3) noexcept;


  //! Return a reference to the element
  Type& operator[](const size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const size_t index) const noexcept;


  //! Return the size of the vector
  static constexpr size_t size() noexcept;


  Type x,
       y,
       z,
       w;
};

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;
//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept;

//!
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept;

//!
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept;

//!
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept;

//!
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept;

//! Pre-increment a vector
template <typename Type, size_t kN>
Vector<Type, kN>& operator++(Vector<Type, kN>& value) noexcept;

//! Pre-decrement a vector
template <typename Type, size_t kN>
Vector<Type, kN>& operator--(Vector<Type, kN>& value) noexcept;

//! Post-increment a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator++(Vector<Type, kN>& value, int) noexcept;

//! Post-decrement a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator--(Vector<Type, kN>& value, int) noexcept;

//!
template <typename Type, size_t kN>
Vector<Type, kN> operator-(const Vector<Type, kN>& value) noexcept;

//! Compute an addition of two vectors
template <typename Type, size_t kN>
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute an addition of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator+(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute an addition of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a subtract of two vectors
template <typename Type, size_t kN>
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a subtract of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator-(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a subtract of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a multiplication of two vectors
template <typename Type, size_t kN>
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a multiplication of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator*(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a multiplication of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a division of two vectors
template <typename Type, size_t kN>
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a division of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator/(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a division of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a reminder of two vectors
template <typename Type, size_t kN>
Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a reminder of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator%(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a reminder of a scalar and a vector
template <typename Type, size_t kN>
Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a bitwise NOT
template <typename Type, size_t kN>
Vector<Type, kN> operator~(const Vector<Type, kN>& value) noexcept;

//! Compute a bitwise AND
template <typename Type, size_t kN>
Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise AND
template <typename Type, size_t kN>
Vector<Type, kN> operator&(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise AND
template <typename Type, size_t kN>
Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a bitwise OR
template <typename Type, size_t kN>
Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise OR
template <typename Type, size_t kN>
Vector<Type, kN> operator|(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise OR
template <typename Type, size_t kN>
Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a bitwise XOR
template <typename Type, size_t kN>
Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise XOR
template <typename Type, size_t kN>
Vector<Type, kN> operator^(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a bitwise XOR
template <typename Type, size_t kN>
Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept;

//! Compute a left shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept;

//! Compute a left shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator<<(const Type1& lhs,
                             const Vector<Type2, kN>& rhs) noexcept;

//! Compute a left shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept;

//! Compute a right shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept;

//! Compute a right shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator>>(const Type1& lhs,
                             const Vector<Type2, kN>& rhs) noexcept;

//! Compute a right shift
template <typename Type1, typename Type2, size_t kN>
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Check if two values are equal in value
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//!
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//! Return the result of boolean negation operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator!(
    const Vector<Type, kN>& value) noexcept;

//! Return the result of boolean AND operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Return the result of boolean AND operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Return the result of boolean AND operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

//! Return the result of boolean OR operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Return the result of boolean OR operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept;

//! Return the result of boolean OR operations
template <typename Type, size_t kN>
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept;

// Type aliases
// Vector2
using char2 = Vector<int8b, 2>;
using uchar2 = Vector<uint8b, 2>;
using short2 = Vector<int16b, 2>;
using ushort2 = Vector<uint16b, 2>;
using int2 = Vector<int32b, 2>;
using uint2 = Vector<uint32b, 2>;
using long2 = Vector<int64b, 2>;
using ulong2 = Vector<uint64b, 2>;
using half2 = Vector<half, 2>;
using float2 = Vector<float, 2>;
using double2 = Vector<double, 2>;
// Vector3
using char3 = Vector<int8b, 3>;
using uchar3 = Vector<uint8b, 3>;
using short3 = Vector<int16b, 3>;
using ushort3 = Vector<uint16b, 3>;
using int3 = Vector<int32b, 3>;
using uint3 = Vector<uint32b, 3>;
using long3 = Vector<int64b, 3>;
using ulong3 = Vector<uint64b, 3>;
using half3 = Vector<half, 3>;
using float3 = Vector<float, 3>;
using double3 = Vector<double, 3>;
// Vector4
using char4 = Vector<int8b, 4>;
using uchar4 = Vector<uint8b, 4>;
using short4 = Vector<int16b, 4>;
using ushort4 = Vector<uint16b, 4>;
using int4 = Vector<int32b, 4>;
using uint4 = Vector<uint32b, 4>;
using long4 = Vector<int64b, 4>;
using ulong4 = Vector<uint64b, 4>;
using half4 = Vector<half, 4>;
using float4 = Vector<float, 4>;
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
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
  static float vload_half(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  static float2 vload_half2(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
  static float2 vload_half2(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  static float3 vload_half3(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
  static float3 vload_half3(
      const size_t offset,
      const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  static float4 vload_half4(
      const size_t offset,
      const half* p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
  template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
float vload_half(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
float2 vload_half2(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
float2 vload_half2(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
float3 vload_half3(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
float3 vload_half3(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
float4 vload_half4(
    const size_t offset,
    const half* p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
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
template <AddressSpaceType kAddressSpaceType, inner::Half Type>
void vstore_half4(
    const float4& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept;

} // namespace cl

} // namespace zivc

#include "vector-inl.hpp"

#endif // ZIVC_CL_VECTOR_HPP
