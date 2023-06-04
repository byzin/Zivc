/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_UTILITY_HPP
#define ZIVC_CPUCL_UTILITY_HPP

// Standard C++ library
#include <array>
#include <concepts>
#include <cstddef>
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.
  */
class WorkItem
{
 public:
  //! Return the number of dimensions in use
  static uint32b getDimension() noexcept;

  //! Return the number of local work-items
  static constexpr size_t getEnqueuedLocalSize(const uint32b dimension) noexcept;

  //! Return the global work-item ID for dimension
  static size_t getGlobalId(const uint32b dimension) noexcept;

  //! Return the 3d global offset used in global id calculation
  static size_t getGlobalIdOffset(const uint32b dimension) noexcept;

  //! Return the work-items 1-dimensional global ID
  static size_t getGlobalLinearId() noexcept;

  //! Return the number of global work-items for dimension
  static size_t getGlobalSize(const uint32b dimension) noexcept;

  //! Return the unique local work-item ID
  static constexpr size_t getLocalId(const uint32b dimension) noexcept;

  //! Return the work-items 1-dimensional local ID
  static size_t getLocalLinearId() noexcept;

  //! Return the number of local work-items
  static constexpr size_t getLocalSize(const uint32b dimension) noexcept;

  //! Return the number of work-groups for dimension
  static size_t getNumOfGroups(const uint32b dimension) noexcept;

  //! Return the work-group ID for dimension
  static size_t getWorkGroupId(const uint32b dimension) noexcept;

  //! Set the number of dimensions in use
  static void setDimension(const uint32b dimension) noexcept;

  //! Set the 3d global offset used in global id calculation
  static void setGlobalIdOffset(const std::array<uint32b, 3>& offset) noexcept;

  //! Set the number of work-groups
  static void setNumOfGroups(const std::array<uint32b, 3>& size) noexcept;

  //! Set the work-group ID
  static void setWorkGroupId(const uint32b id) noexcept;

 private:
  static thread_local uint32b dimension_;
  static thread_local std::array<uint32b, 3> global_id_offset_;
  static thread_local std::array<uint32b, 3> num_of_work_groups_;
  static thread_local std::array<uint32b, 3> work_group_id_;
};

// OpenCL functions

//! Assert the message on standard error output and raise an error
template <typename... Types>
void assertIfFalse(const bool condition, const char* format, const Types&... args);

//! Write the result of the format to stdout
template <typename... Types>
int printf(const char* format, const Types&... args);

// Work-Item functions

//! Return the number of local work-items
constexpr size_t get_enqueued_local_size(const uint32b dimension) noexcept;

//! Return the global work-item ID for dimension
size_t get_global_id(const uint32b dimension) noexcept;

//! Return the work-items 1-dimensional global ID
size_t get_global_linear_id() noexcept;

//! Return the 3d global offset used in global ID calculation
size_t get_global_offset(const uint32b dimension) noexcept;

//! Return the number of global work-items for dimension
size_t get_global_size(const uint32b dimension) noexcept;

//! Return the work-group ID
size_t get_group_id(const uint32b dimension) noexcept;

//! Return the unique local work-item ID
constexpr size_t get_local_id(const uint32b dimension) noexcept;

//! Return the work-items 1-dimensional local ID
size_t get_local_linear_id() noexcept;

//! Return the number of local work-items
constexpr size_t get_local_size(const uint32b dimension) noexcept;

//! Return the number of work-groups that will execute a kernel
size_t get_num_groups(const uint32b dimension) noexcept;

//! Return the number of dimensions in use
uint32b get_work_dim() noexcept;

//

/*!
  \brief No brief description

  No detailed description.
  */
class Utility
{
 public:
  //! Compute the absolute value of a integer value (|x|)
  template <std::signed_integral Type>
  static std::make_unsigned_t<Type> abs(const Type& x) noexcept;

  //! Compute the absolute value of a integer value (|x|)
  template <std::unsigned_integral Type>
  static Type abs(const Type& x) noexcept;

  //! Compute the absolute value of a integer value (|x|)
  template <std::signed_integral Type, std::size_t kN>
  static Vector<std::make_unsigned_t<Type>, kN> abs(const Vector<Type, kN>& x) noexcept;

  //! Compute the absolute value of a integer value (|x|)
  template <std::unsigned_integral Type, std::size_t kN>
  static Vector<Type, kN> abs(const Vector<Type, kN>& x) noexcept;

  //! Compute the absolute value of a floating point value (|x|)
  template <std::floating_point Type>
  static Type fabs(const Type& x) noexcept;

  //! Compute the absolute value of a floating point value (|x|)
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> fabs(const Vector<Type, kN>& x) noexcept;

  //! Clamp a value between a pair of boundary values
  template <Arithmetic Type>
  static Type clamp(const Type& x, const Type& lo, const Type& hi) noexcept;

  //! Clamp a value between a pair of boundary values
  template <Arithmetic Type, std::size_t kN>
  static Vector<Type, kN> clamp(const Vector<Type, kN>& x,
                                const Vector<Type, kN>& lo,
                                const Vector<Type, kN>& hi) noexcept;

  //! Count the number of consecutive 0 bits, starting from the most significant bit
  template <std::integral Type>
  static Type clz(const Type& x) noexcept;

  //! Count the number of consecutive 0 bits, starting from the most significant bit
  template <std::integral Type, std::size_t kN>
  static Vector<Type, kN> clz(const Vector<Type, kN>& x) noexcept;

  //! Convert the radians to degrees
  template <std::floating_point Type>
  static Type degrees(const Type& radians) noexcept;

  //! Convert the radians to degrees
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> degrees(const Vector<Type, kN>& radians) noexcept;

  //! Check if the x is negative
  template <Arithmetic Type>
  static constexpr bool isNegative(const Type& x) noexcept;

  //! Return the greater of the given values
  template <Arithmetic Type>
  static Type max(const Type& x, const Type& y) noexcept;

  //! Return the greater of the given values
  template <Arithmetic Type, std::size_t kN>
  static Vector<Type, kN> max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

  //! Return the smaller of the given values
  template <Arithmetic Type>
  static Type min(const Type& x, const Type& y) noexcept;

  //! Return the smaller of the given values
  template <Arithmetic Type, std::size_t kN>
  static Vector<Type, kN> min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

  //! Count the number of 1 bits in an unsigned integer
  template <std::integral Type>
  static Type popcount(const Type& x) noexcept;

  //! Count the number of 1 bits in an unsigned integer
  template <std::integral Type, std::size_t kN>
  static Vector<Type, kN> popcount(const Vector<Type, kN>& x) noexcept;

  //! Convert the degrees to radians 
  template <std::floating_point Type>
  static Type radians(const Type& degrees) noexcept;

  //! Convert the degrees to radians 
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> radians(const Vector<Type, kN>& degrees) noexcept;

  //! Compute the result of bitwise left-rotation
  template <std::integral Type>
  static Type rotate(const Type& x, const Type& s) noexcept;

  //! Compute the result of bitwise left-rotation
  template <std::integral Type, std::size_t kN>
  static Vector<Type, kN> rotate(const Vector<Type, kN>& x,
                                 const Vector<Type, kN>& s) noexcept;

  //! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
  template <std::floating_point Type>
  static Type sign(const Type& x) noexcept;

  //! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> sign(const Vector<Type, kN>& x) noexcept;

  //! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
  template <std::floating_point Type>
  static Type smoothstep(const Type& edge0, const Type& edge1, const Type& x) noexcept;

  //! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> smoothstep(const Vector<Type, kN>& edge0,
                                     const Vector<Type, kN>& edge1,
                                     const Vector<Type, kN>& x) noexcept;

  //! Return 0.0 if x < edge, otherwise it returns 1.0
  template <std::floating_point Type>
  static Type step(const Type& edge, const Type& x) noexcept;

  //! Return 0.0 if x < edge, otherwise it returns 1.0
  template <std::floating_point Type, std::size_t kN>
  static Vector<Type, kN> step(const Vector<Type, kN>& edge,
                               const Vector<Type, kN>& x) noexcept;

  //! Compute '(hi << n) | lo'
  static int16b upsample(const int8b hi, const uint8b lo) noexcept;

  //! Compute '(hi << n) | lo'
  static uint16b upsample(const uint8b hi, const uint8b lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<int16b, kN> upsample(const Vector<int8b, kN>& hi,
                                     const Vector<uint8b, kN> lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<uint16b, kN> upsample(const Vector<uint8b, kN>& hi,
                                      const Vector<uint8b, kN> lo) noexcept;

  //! Compute '(hi << n) | lo'
  static int32b upsample(const int16b hi, const uint16b lo) noexcept;

  //! Compute '(hi << n) | lo'
  static uint32b upsample(const uint16b hi, const uint16b lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<int32b, kN> upsample(const Vector<int16b, kN>& hi,
                                     const Vector<uint16b, kN> lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<uint32b, kN> upsample(const Vector<uint16b, kN>& hi,
                                      const Vector<uint16b, kN> lo) noexcept;

  //! Compute '(hi << n) | lo'
  static int64b upsample(const int32b hi, const uint32b lo) noexcept;

  //! Compute '(hi << n) | lo'
  static uint64b upsample(const uint32b hi, const uint32b lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<int64b, kN> upsample(const Vector<int32b, kN>& hi,
                                     const Vector<uint32b, kN> lo) noexcept;

  //! Compute '(hi << n) | lo'
  template <std::size_t kN>
  static Vector<uint64b, kN> upsample(const Vector<uint32b, kN>& hi,
                                      const Vector<uint32b, kN> lo) noexcept;
};

//! Compute the absolute value of a integer value (|x|)
template <std::integral Type>
std::make_unsigned_t<Type> abs(const Type& x) noexcept;

//! Compute the absolute value of a integer value (|x|)
template <std::integral Type, std::size_t kN>
Vector<std::make_unsigned_t<Type>, kN> abs(const Vector<Type, kN>& x) noexcept;

//! Compute the absolute value of a floating point value (|x|)
template <std::floating_point Type>
Type fabs(const Type& x) noexcept;

//! Compute the absolute value of a floating point value (|x|)
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> fabs(const Vector<Type, kN>& x) noexcept;

//! Clamp a value between a pair of boundary values
template <Arithmetic Type>
Type clamp(const Type& x, const Type& lo, const Type& hi) noexcept;

//! Clamp a value between a pair of boundary values
template <Arithmetic Type, std::size_t kN>
Vector<Type, kN> clamp(const Vector<Type, kN>& x,
                       const Vector<Type, kN>& lo,
                       const Vector<Type, kN>& hi) noexcept;

//! Count the number of consecutive 0 bits, starting from the most significant bit
template <std::integral Type>
Type clz(const Type& x) noexcept;

//! Count the number of consecutive 0 bits, starting from the most significant bit
template <std::integral Type, std::size_t kN>
Vector<Type, kN> clz(const Vector<Type, kN>& x) noexcept;

//! Convert the radians to degrees
template <std::floating_point Type>
Type degrees(const Type& radians) noexcept;

//! Convert the radians to degrees
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> degrees(const Vector<Type, kN>& radians) noexcept;

//! Return the greater of the given values
template <Arithmetic Type>
Type max(const Type& x, const Type& y) noexcept;

//! Return the greater of the given values
template <Arithmetic Type, std::size_t kN>
Vector<Type, kN> max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

//! Return the smaller of the given values
template <Arithmetic Type>
Type min(const Type& x, const Type& y) noexcept;

//! Return the smaller of the given values
template <Arithmetic Type, std::size_t kN>
Vector<Type, kN> min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

//! Count the number of 1 bits in an unsigned integer
template <std::integral Type>
Type popcount(const Type& x) noexcept;

//! Count the number of 1 bits in an unsigned integer
template <std::integral Type, std::size_t kN>
Vector<Type, kN> popcount(const Vector<Type, kN>& x) noexcept;

//! Convert the degrees to radians 
template <std::floating_point Type>
Type radians(const Type& degrees) noexcept;

//! Convert the degrees to radians 
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> radians(const Vector<Type, kN>& degrees) noexcept;

//! Compute the result of bitwise left-rotation
template <std::integral Type>
Type rotate(const Type& x, const Type& s) noexcept;

//! Compute the result of bitwise left-rotation
template <std::integral Type, std::size_t kN>
Vector<Type, kN> rotate(const Vector<Type, kN>& x,
                        const Vector<Type, kN>& s) noexcept;

//! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
template <std::floating_point Type>
Type sign(const Type& x) noexcept;

//! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> sign(const Vector<Type, kN>& x) noexcept;

//! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
template <std::floating_point Type>
Type smoothstep(const Type& edge0, const Type& edge1, const Type& x) noexcept;

//! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> smoothstep(const Vector<Type, kN>& edge0,
                            const Vector<Type, kN>& edge1,
                            const Vector<Type, kN>& x) noexcept;

//! Return 0.0 if x < edge, otherwise it returns 1.0
template <std::floating_point Type>
Type step(const Type& edge, const Type& x) noexcept;

//! Return 0.0 if x < edge, otherwise it returns 1.0
template <std::floating_point Type, std::size_t kN>
Vector<Type, kN> step(const Vector<Type, kN>& edge,
                      const Vector<Type, kN>& x) noexcept;

//! Compute '(hi << n) | lo'
int16b upsample(const int8b hi, const uint8b lo) noexcept;

//! Compute '(hi << n) | lo'
uint16b upsample(const uint8b hi, const uint8b lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<int16b, kN> upsample(const Vector<int8b, kN>& hi,
                            const Vector<uint8b, kN> lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<uint16b, kN> upsample(const Vector<uint8b, kN>& hi,
                             const Vector<uint8b, kN> lo) noexcept;

//! Compute '(hi << n) | lo'
int32b upsample(const int16b hi, const uint16b lo) noexcept;

//! Compute '(hi << n) | lo'
uint32b upsample(const uint16b hi, const uint16b lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<int32b, kN> upsample(const Vector<int16b, kN>& hi,
                            const Vector<uint16b, kN> lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<uint32b, kN> upsample(const Vector<uint16b, kN>& hi,
                             const Vector<uint16b, kN> lo) noexcept;

//! Compute '(hi << n) | lo'
int64b upsample(const int32b hi, const uint32b lo) noexcept;

//! Compute '(hi << n) | lo'
uint64b upsample(const uint32b hi, const uint32b lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<int64b, kN> upsample(const Vector<int32b, kN>& hi,
                            const Vector<uint32b, kN> lo) noexcept;

//! Compute '(hi << n) | lo'
template <std::size_t kN>
Vector<uint64b, kN> upsample(const Vector<uint32b, kN>& hi,
                             const Vector<uint32b, kN> lo) noexcept;

} // namespace zivc::cl

#include "utility-inl.hpp"

#endif // ZIVC_CPUCL_UTILITY_HPP
