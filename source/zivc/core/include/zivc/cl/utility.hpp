/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_UTILITY_HPP
#define ZIVC_CL_UTILITY_HPP

// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class Utility
{
 public:
  //

  //! Assert the message on standard error output and raise an error (only cpu)
  template <typename ...Types>
  static void assertIfFalse(const bool condition, ConstConstantPtr<char> format, const Types... args);

  //! Write the message on standard output (only cpu)
  template <typename ...Types>
  static int32b print(ConstConstantPtr<char> format, const Types... args);

  // Conversion functions

  //! Convert type from T to Type
  template <typename Type, typename T>
  static Type cast(T value) noexcept;

  //! Treat T* as Type*
  template <typename Type, typename T>
  static Type reinterp(T object) noexcept;

  //

  //! Compute the absolute value of a value (|x|)
  template <typename IntegerN>
  static auto abs(const IntegerN x) noexcept;

  //! Compute a value between a pair of boundary values
  template <typename ArithN>
  static ArithN clamp(const ArithN x, const ArithN lo, const ArithN hi) noexcept;

  //! Count the number of consecutive 0 bits, starting from the most significant bit
  template <typename IntegerN>
  static IntegerN clz(const IntegerN x) noexcept;

  //! Convert the radians to degrees
  template <typename FloatN>
  static FloatN degrees(const FloatN radians) noexcept;

  //! Return the greater of the given values
  template <typename ArithN>
  static ArithN max(const ArithN x, const ArithN y) noexcept;

  //! Return the smaller of the given values
  template <typename ArithN>
  static ArithN min(const ArithN x, const ArithN y) noexcept;

  //! Count the number of 1 bits in an integer
  template <typename IntegerN>
  static IntegerN popcount(const IntegerN x) noexcept;

  //! Convert the degrees to radians
  template <typename FloatN>
  static FloatN radians(const FloatN degrees) noexcept;

  //! Compute the result of bitwise left-rotation
  template <typename IntegerN>
  static IntegerN rotate(const IntegerN x, const IntegerN s) noexcept;

  //! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
  template <typename FloatN>
  static FloatN sign(const FloatN x) noexcept;

  //! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
  template <typename FloatN>
  static FloatN smoothstep(const FloatN edge0, const FloatN edge1, const FloatN x) noexcept;

  //! Return 0.0 if x < edge, otherwise it returns 1.0
  template <typename FloatN>
  static FloatN step(const FloatN edge, const FloatN x) noexcept;

  //! Compute '(hi << n) | lo'
  template <typename IntegerN, typename UIntegerN>
  static auto upsample(const IntegerN hi, const UIntegerN lo) noexcept;

  // Conditional function

  //! Update the value in the lhs with the rhs if the flag is true
  template <typename AddressSpacePointer, typename Type>
  static bool updateIfTrue(const bool flag, AddressSpacePointer lhs, Type rhs) noexcept;

 private:
  template <typename Type>
  struct CastImpl;
  template <typename Type>
  struct ReinterpImpl;
};

//

//! Assert the message on standard error output and raise an error (only cpu)
template <typename ...Types>
void assertIfFalse(const bool condition, ConstConstantPtr<char> format, const Types... args);

//! Write the message on standard output (only cpu)
template <typename ...Types>
int32b print(ConstConstantPtr<char> format, const Types... args);

// Conversion functions

//! Convert type from T to Type
template <typename Type, typename T>
Type cast(T value) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
Type reinterp(T object) noexcept;

//

//! Compute the absolute value of a value (|x|)
template <typename ArithN>
auto abs(const ArithN x) noexcept;

//! Compute a value between a pair of boundary values
template <typename ArithN>
ArithN clamp(const ArithN x, const ArithN lo, const ArithN hi) noexcept;

//! Count the number of consecutive 0 bits, starting from the most significant bit
template <typename IntegerN>
IntegerN clz(const IntegerN x) noexcept;

//! Convert the radians to degrees
template <typename FloatN>
FloatN degrees(const FloatN radians) noexcept;

//! Return the greater of the given values
template <typename ArithN>
ArithN max(const ArithN x, const ArithN y) noexcept;

//! Return the smaller of the given values
template <typename ArithN>
ArithN min(const ArithN x, const ArithN y) noexcept;

//! Count the number of 1 bits in an integer
template <typename IntegerN>
IntegerN popcount(const IntegerN x) noexcept;

//! Convert the degrees to radians
template <typename FloatN>
FloatN radians(const FloatN degrees) noexcept;

//! Compute the result of bitwise left-rotation
template <typename IntegerN>
IntegerN rotate(const IntegerN x, const IntegerN s) noexcept;

//! Return 1.0 if x > 0, or -1.0 if x < 0, otherwise x
template <typename FloatN>
FloatN sign(const FloatN x) noexcept;

//! Return 0.0 if x <= edge0 and 1.0 if x >= edge1 and otherwise performs Hermite interpolation
template <typename FloatN>
FloatN smoothstep(const FloatN edge0, const FloatN edge1, const FloatN x) noexcept;

//! Return 0.0 if x < edge, otherwise it returns 1.0
template <typename FloatN>
FloatN step(const FloatN edge, const FloatN x) noexcept;

//! Compute '(hi << n) | lo'
template <typename IntegerN, typename UIntegerN>
auto upsample(const IntegerN hi, const UIntegerN lo) noexcept;

// Conditional function

//! Update the value in the lhs with the rhs if the flag is true
template <typename AddressSpacePointer, typename Type>
bool updateIfTrue(const bool flag, AddressSpacePointer lhs, Type rhs) noexcept;

// Work-Item functions

//! Return the number of dimensions in use
uint32b getWorkDim() noexcept;

//! Return the number of global work-items for dimension by 'dimension'
size_t getGlobalSize(const uint32b dimension) noexcept;

//! Return the number of global work-items of x dimension
size_t getGlobalSizeX() noexcept;

//! Return the number of global work-items of y dimension
size_t getGlobalSizeY() noexcept;

//! Return the number of global work-items of z dimension
size_t getGlobalSizeZ() noexcept;

//! Return the global work-item ID for dimension by 'dimension'
size_t getGlobalId(const uint32b dimension) noexcept;

//! Return the global work-item ID of x dimension
size_t getGlobalIdX() noexcept;

//! Return the global work-item ID of y dimension
size_t getGlobalIdY() noexcept;

//! Return the global work-item ID of z dimension
size_t getGlobalIdZ() noexcept;

//! Return the number of local work-items for dimension by 'dimension'
size_t getLocalSize(const uint32b dimension) noexcept;

//! Return the number of local work-items of x dimension
size_t getLocalSizeX() noexcept;

//! Return the number of local work-items of y dimension
size_t getLocalSizeY() noexcept;

//! Return the number of local work-items of z dimension
size_t getLocalSizeZ() noexcept;

//! Return the number of local work-items for dimension by 'dimension'
size_t getEnqueuedLocalSize(const uint32b dimension) noexcept;

//! Return the number of local work-items of x dimension
size_t getEnqueuedLocalSizeX() noexcept;

//! Return the number of local work-items of y dimension
size_t getEnqueuedLocalSizeY() noexcept;

//! Return the number of local work-items of z dimension
size_t getEnqueuedLocalSizeZ() noexcept;

//! Return the local work-item ID for dimension by 'dimension'
size_t getLocalId(const uint32b dimension) noexcept;

//! Return the local work-item ID of x dimension
size_t getLocalIdX() noexcept;

//! Return the local work-item ID of y dimension
size_t getLocalIdY() noexcept;

//! Return the local work-item ID of z dimension
size_t getLocalIdZ() noexcept;

//! Return the number of work-groups for dimension by 'dimension'
size_t getNumGroups(const uint32b dimension) noexcept;

//! Return the number of work-groups of x dimension
size_t getNumGroupsX() noexcept;

//! Return the number of work-groups of y dimension
size_t getNumGroupsY() noexcept;

//! Return the number of work-groups of z dimension
size_t getNumGroupsZ() noexcept;

//! Return the work-group ID for dimension by 'dimension'
size_t getGroupId(const uint32b dimension) noexcept;

//! Return the work-group ID of x dimension
size_t getGroupIdX() noexcept;

//! Return the work-group ID of y dimension
size_t getGroupIdY() noexcept;

//! Return the work-group ID of z dimension
size_t getGroupIdZ() noexcept;

//! Return the global offset value used in global id calculation for dimension
size_t getGlobalOffset(const uint32b dimension) noexcept;

//! Return the global offset of x dimension value used in global id calculation
size_t getGlobalOffsetX() noexcept;

//! Return the global offset of y dimension value used in global id calculation
size_t getGlobalOffsetY() noexcept;

//! Return the global offset of z dimension value used in global id calculation
size_t getGlobalOffsetZ() noexcept;

//! Return the work-items 1-dimensional global ID
size_t getGlobalLinearId() noexcept;

//! Return the work-items 1-dimensional local ID
size_t getLocalLinearId() noexcept;

// Type utilities

//! Make a char2 value
constexpr char2 makeChar2(const int8b v) noexcept;

//! Make a char2 value
constexpr char2 makeChar2(const int8b v0, const int8b v1) noexcept;

//! Make a char3 value
constexpr char3 makeChar3(const int8b v) noexcept;

//! Make a char3 value
constexpr char3 makeChar3(const int8b v0, const int8b v1, const int8b v2) noexcept;

//! Make a char3 value
constexpr char3 makeChar3(const char2 v0, const int8b v1) noexcept;

//! Make a char3 value
constexpr char3 makeChar3(const int8b v0, const char2 v1) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const int8b v) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const int8b v0, const int8b v1,
                          const int8b v2, const int8b v3) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const char2 v0, const int8b v1, const int8b v2) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const int8b v0, const char2 v1, const int8b v2) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const int8b v0, const int8b v1, const char2 v2) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const char2 v0, const char2 v1) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const char3 v0, const int8b v1) noexcept;

//! Make a char4 value
constexpr char4 makeChar4(const int8b v0, const char3 v1) noexcept;

//! Make a char8 value
constexpr char8 makeChar8(const int8b v) noexcept;

//! Make a char8 value
constexpr char8 makeChar8(const int8b v0, const int8b v1,
                          const int8b v2, const int8b v3,
                          const int8b v4, const int8b v5,
                          const int8b v6, const int8b v7) noexcept;

//! Make a char8 value
constexpr char8 makeChar8(const char2 v0, const char2 v1,
                          const char2 v2, const char2 v3) noexcept;

//! Make a char8 value
constexpr char8 makeChar8(const char4 v0, const char4 v1) noexcept;

//! Make a char16 value
constexpr char16 makeChar16(const int8b v) noexcept;

//! Make a char16 value
constexpr char16 makeChar16(const int8b v0, const int8b v1,
                            const int8b v2, const int8b v3,
                            const int8b v4, const int8b v5,
                            const int8b v6, const int8b v7,
                            const int8b v8, const int8b v9,
                            const int8b v10, const int8b v11,
                            const int8b v12, const int8b v13,
                            const int8b v14, const int8b v15) noexcept;

//! Make a char16 value
constexpr char16 makeChar16(const char2 v0, const char2 v1,
                            const char2 v2, const char2 v3,
                            const char2 v4, const char2 v5,
                            const char2 v6, const char2 v7) noexcept;

//! Make a char16 value
constexpr char16 makeChar16(const char4 v0, const char4 v1,
                            const char4 v2, const char4 v3) noexcept;

//! Make a char16 value
constexpr char16 makeChar16(const char8 v0, const char8 v1) noexcept;

//! Make a uchar2 value
constexpr uchar2 makeUChar2(const uint8b v) noexcept;

//! Make a uchar2 value
constexpr uchar2 makeUChar2(const uint8b v0, const uint8b v1) noexcept;

//! Make a uchar3 value
constexpr uchar3 makeUChar3(const uint8b v) noexcept;

//! Make a uchar3 value
constexpr uchar3 makeUChar3(const uint8b v0, const uint8b v1, const uint8b v2) noexcept;

//! Make a uchar3 value
constexpr uchar3 makeUChar3(const uchar2 v0, const uint8b v1) noexcept;

//! Make a uchar3 value
constexpr uchar3 makeUChar3(const uint8b v0, const uchar2 v1) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uint8b v) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uint8b v0, const uint8b v1,
                            const uint8b v2, const uint8b v3) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uchar2 v0, const uint8b v1, const uint8b v2) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uint8b v0, const uchar2 v1, const uint8b v2) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uint8b v0, const uint8b v1, const uchar2 v2) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uchar2 v0, const uchar2 v1) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uchar3 v0, const uint8b v1) noexcept;

//! Make a uchar4 value
constexpr uchar4 makeUChar4(const uint8b v0, const uchar3 v1) noexcept;

//! Make a uuchar8 value
constexpr uchar8 makeUChar8(const uint8b v) noexcept;

//! Make a uchar8 value
constexpr uchar8 makeUChar8(const uint8b v0, const uint8b v1,
                            const uint8b v2, const uint8b v3,
                            const uint8b v4, const uint8b v5,
                            const uint8b v6, const uint8b v7) noexcept;

//! Make a uchar8 value
constexpr uchar8 makeUChar8(const uchar2 v0, const uchar2 v1, const uchar2 v2, const uchar2 v3) noexcept;

//! Make a uchar8 value
constexpr uchar8 makeUChar8(const uchar4 v0, const uchar4 v1) noexcept;

//! Make a uchar16 value
constexpr uchar16 makeUChar16(const uint8b v) noexcept;

//! Make a uchar16 value
constexpr uchar16 makeUChar16(const uint8b v0, const uint8b v1,
                              const uint8b v2, const uint8b v3,
                              const uint8b v4, const uint8b v5,
                              const uint8b v6, const uint8b v7,
                              const uint8b v8, const uint8b v9,
                              const uint8b v10, const uint8b v11,
                              const uint8b v12, const uint8b v13,
                              const uint8b v14, const uint8b v15) noexcept;

//! Make a uchar16 value
constexpr uchar16 makeUChar16(const uchar2 v0, const uchar2 v1,
                              const uchar2 v2, const uchar2 v3,
                              const uchar2 v4, const uchar2 v5,
                              const uchar2 v6, const uchar2 v7) noexcept;

//! Make a uchar16 value
constexpr uchar16 makeUChar16(const uchar4 v0, const uchar4 v1,
                              const uchar4 v2, const uchar4 v3) noexcept;

//! Make a uchar16 value
constexpr uchar16 makeUChar16(const uchar8 v0, const uchar8 v1) noexcept;

//! Make a short2 value
constexpr short2 makeShort2(const int16b v) noexcept;

//! Make a short2 value
constexpr short2 makeShort2(const int16b v0, const int16b v1) noexcept;

//! Make a short3 value
constexpr short3 makeShort3(const int16b v) noexcept;

//! Make a short3 value
constexpr short3 makeShort3(const int16b v0, const int16b v1, const int16b v2) noexcept;

//! Make a short3 value
constexpr short3 makeShort3(const short2 v0, const int16b v1) noexcept;

//! Make a short3 value
constexpr short3 makeShort3(const int16b v0, const short2 v1) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const int16b v) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const int16b v0, const int16b v1,
                            const int16b v2, const int16b v3) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const short2 v0, const int16b v1, const int16b v2) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const int16b v0, const short2 v1, const int16b v2) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const int16b v0, const int16b v1, const short2 v2) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const short2 v0, const short2 v1) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const short3 v0, const int16b v1) noexcept;

//! Make a short4 value
constexpr short4 makeShort4(const int16b v0, const short3 v1) noexcept;

//! Make a short8 value
constexpr short8 makeShort8(const int16b v) noexcept;

//! Make a short8 value
constexpr short8 makeShort8(const int16b v0, const int16b v1,
                            const int16b v2, const int16b v3,
                            const int16b v4, const int16b v5,
                            const int16b v6, const int16b v7) noexcept;

//! Make a short8 value
constexpr short8 makeShort8(const short2 v0, const short2 v1,
                            const short2 v2, const short2 v3) noexcept;

//! Make a short8 value
constexpr short8 makeShort8(const short4 v0, const short4 v1) noexcept;

//! Make a short16 value
constexpr short16 makeShort16(const int16b v) noexcept;

//! Make a short16 value
constexpr short16 makeShort16(const int16b v0, const int16b v1,
                              const int16b v2, const int16b v3,
                              const int16b v4, const int16b v5,
                              const int16b v6, const int16b v7,
                              const int16b v8, const int16b v9,
                              const int16b v10, const int16b v11,
                              const int16b v12, const int16b v13,
                              const int16b v14, const int16b v15) noexcept;

//! Make a short16 value
constexpr short16 makeShort16(const short2 v0, const short2 v1,
                              const short2 v2, const short2 v3,
                              const short2 v4, const short2 v5,
                              const short2 v6, const short2 v7) noexcept;

//! Make a short16 value
constexpr short16 makeShort16(const short4 v0, const short4 v1,
                              const short4 v2, const short4 v3) noexcept;

//! Make a short16 value
constexpr short16 makeShort16(const short8 v0, const short8 v1) noexcept;

//! Make a ushort2 value
constexpr ushort2 makeUShort2(const uint16b v) noexcept;

//! Make a ushort2 value
constexpr ushort2 makeUShort2(const uint16b v0, const uint16b v1) noexcept;

//! Make a ushort3 value
constexpr ushort3 makeUShort3(const uint16b v) noexcept;

//! Make a ushort3 value
constexpr ushort3 makeUShort3(const uint16b v0, const uint16b v1, const uint16b v2) noexcept;

//! Make a ushort3 value
constexpr ushort3 makeUShort3(const ushort2 v0, const uint16b v1) noexcept;

//! Make a ushort3 value
constexpr ushort3 makeUShort3(const uint16b v0, const ushort2 v1) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const uint16b v) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const uint16b v0, const uint16b v1,
                              const uint16b v2, const uint16b v3) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const ushort2 v0, const uint16b v1, const uint16b v2) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const uint16b v0, const ushort2 v1, const uint16b v2) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const uint16b v0, const uint16b v1, const ushort2 v2) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const ushort2 v0, const ushort2 v1) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const ushort3 v0, const uint16b v1) noexcept;

//! Make a ushort4 value
constexpr ushort4 makeUShort4(const uint16b v0, const ushort3 v1) noexcept;

//! Make a ushort8 value
constexpr ushort8 makeUShort8(const uint16b v) noexcept;

//! Make a ushort8 value
constexpr ushort8 makeUShort8(const uint16b v0, const uint16b v1,
                              const uint16b v2, const uint16b v3,
                              const uint16b v4, const uint16b v5,
                              const uint16b v6, const uint16b v7) noexcept;

//! Make a ushort8 value
constexpr ushort8 makeUShort8(const ushort2 v0, const ushort2 v1,
                              const ushort2 v2, const ushort2 v3) noexcept;

//! Make a ushort8 value
constexpr ushort8 makeUShort8(const ushort4 v0, const ushort4 v1) noexcept;

//! Make a ushort16 value
constexpr ushort16 makeUShort16(const uint16b v) noexcept;

//! Make a ushort16 value
constexpr ushort16 makeUShort16(const uint16b v0, const uint16b v1,
                                const uint16b v2, const uint16b v3,
                                const uint16b v4, const uint16b v5,
                                const uint16b v6, const uint16b v7,
                                const uint16b v8, const uint16b v9,
                                const uint16b v10, const uint16b v11,
                                const uint16b v12, const uint16b v13,
                                const uint16b v14, const uint16b v15) noexcept;

//! Make a ushort16 value
constexpr ushort16 makeUShort16(const ushort2 v0, const ushort2 v1,
                                const ushort2 v2, const ushort2 v3,
                                const ushort2 v4, const ushort2 v5,
                                const ushort2 v6, const ushort2 v7) noexcept;

//! Make a ushort16 value
constexpr ushort16 makeUShort16(const ushort4 v0, const ushort4 v1,
                                const ushort4 v2, const ushort4 v3) noexcept;

//! Make a ushort16 value
constexpr ushort16 makeUShort16(const ushort8 v0, const ushort8 v1) noexcept;

//! Make a int2 value
constexpr int2 makeInt2(const int32b v) noexcept;

//! Make a int2 value
constexpr int2 makeInt2(const int32b v0, const int32b v1) noexcept;

//! Make a int3 value
constexpr int3 makeInt3(const int32b v) noexcept;

//! Make a int3 value
constexpr int3 makeInt3(const int32b v0, const int32b v1, const int32b v2) noexcept;

//! Make a int3 value
constexpr int3 makeInt3(const int2 v0, const int32b v1) noexcept;

//! Make a int3 value
constexpr int3 makeInt3(const int32b v0, const int2 v1) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int32b v) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int32b v0, const int32b v1,
                        const int32b v2, const int32b v3) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int2 v0, const int32b v1, const int32b v2) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int32b v0, const int2 v1, const int32b v2) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int32b v0, const int32b v1, const int2 v2) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int2 v0, const int2 v1) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int3 v0, const int32b v1) noexcept;

//! Make a int4 value
constexpr int4 makeInt4(const int32b v0, const int3 v1) noexcept;

//! Make a int8 value
constexpr int8 makeInt8(const int32b v) noexcept;

//! Make a int8 value
constexpr int8 makeInt8(const int32b v0, const int32b v1,
                        const int32b v2, const int32b v3,
                        const int32b v4, const int32b v5,
                        const int32b v6, const int32b v7) noexcept;

//! Make a int8 value
constexpr int8 makeInt8(const int2 v0, const int2 v1,
                        const int2 v2, const int2 v3) noexcept;

//! Make a int8 value
constexpr int8 makeInt8(const int4 v0, const int4 v1) noexcept;

//! Make a int16 value
constexpr int16 makeInt16(const int32b v) noexcept;

//! Make a int16 value
constexpr int16 makeInt16(const int32b v0, const int32b v1,
                          const int32b v2, const int32b v3,
                          const int32b v4, const int32b v5,
                          const int32b v6, const int32b v7,
                          const int32b v8, const int32b v9,
                          const int32b v10, const int32b v11,
                          const int32b v12, const int32b v13,
                          const int32b v14, const int32b v15) noexcept;

//! Make a int16 value
constexpr int16 makeInt16(const int2 v0, const int2 v1,
                          const int2 v2, const int2 v3,
                          const int2 v4, const int2 v5,
                          const int2 v6, const int2 v7) noexcept;

//! Make a int16 value
constexpr int16 makeInt16(const int4 v0, const int4 v1,
                          const int4 v2, const int4 v3) noexcept;

//! Make a int16 value
constexpr int16 makeInt16(const int8 v0, const int8 v1) noexcept;

//! Make a uint2 value
constexpr uint2 makeUInt2(const uint32b v) noexcept;

//! Make a uint2 value
constexpr uint2 makeUInt2(const uint32b v0, const uint32b v1) noexcept;

//! Make a uint3 value
constexpr uint3 makeUInt3(const uint32b v) noexcept;

//! Make a uint3 value
constexpr uint3 makeUInt3(const uint32b v0, const uint32b v1, const uint32b v2) noexcept;

//! Make a uint3 value
constexpr uint3 makeUInt3(const uint2 v0, const uint32b v1) noexcept;

//! Make a uint3 value
constexpr uint3 makeUInt3(const uint32b v0, const uint2 v1) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint32b v) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint32b v0, const uint32b v1,
                          const uint32b v2, const uint32b v3) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint2 v0, const uint32b v1, const uint32b v2) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint32b v0, const uint2 v1, const uint32b v2) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint32b v0, const uint32b v1, const uint2 v2) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint2 v0, const uint2 v1) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint3 v0, const uint32b v1) noexcept;

//! Make a uint4 value
constexpr uint4 makeUInt4(const uint32b v0, const uint3 v1) noexcept;

//! Make a uint8 value
constexpr uint8 makeUInt8(const uint32b v) noexcept;

//! Make a uint8 value
constexpr uint8 makeUInt8(const uint32b v0, const uint32b v1,
                          const uint32b v2, const uint32b v3,
                          const uint32b v4, const uint32b v5,
                          const uint32b v6, const uint32b v7) noexcept;

//! Make a uint8 value
constexpr uint8 makeUInt8(const uint2 v0, const uint2 v1,
                          const uint2 v2, const uint2 v3) noexcept;

//! Make a uint8 value
constexpr uint8 makeUInt8(const uint4 v0, const uint4 v1) noexcept;

//! Make a uint16 value
constexpr uint16 makeUInt16(const uint32b v) noexcept;

//! Make a uint16 value
constexpr uint16 makeUInt16(const uint32b v0, const uint32b v1,
                            const uint32b v2, const uint32b v3,
                            const uint32b v4, const uint32b v5,
                            const uint32b v6, const uint32b v7,
                            const uint32b v8, const uint32b v9,
                            const uint32b v10, const uint32b v11,
                            const uint32b v12, const uint32b v13,
                            const uint32b v14, const uint32b v15) noexcept;

//! Make a uint16 value
constexpr uint16 makeUInt16(const uint2 v0, const uint2 v1,
                            const uint2 v2, const uint2 v3,
                            const uint2 v4, const uint2 v5,
                            const uint2 v6, const uint2 v7) noexcept;

//! Make a uint16 value
constexpr uint16 makeUInt16(const uint4 v0, const uint4 v1,
                            const uint4 v2, const uint4 v3) noexcept;

//! Make a uint16 value
constexpr uint16 makeUInt16(const uint8 v0, const uint8 v1) noexcept;

//! Make a long2 value
constexpr long2 makeLong2(const int64b v) noexcept;

//! Make a long2 value
constexpr long2 makeLong2(const int64b v0, const int64b v1) noexcept;

//! Make a long3 value
constexpr long3 makeLong3(const int64b v) noexcept;

//! Make a long3 value
constexpr long3 makeLong3(const int64b v0, const int64b v1, const int64b v2) noexcept;

//! Make a long3 value
constexpr long3 makeLong3(const long2 v0, const int64b v1) noexcept;

//! Make a long3 value
constexpr long3 makeLong3(const int64b v0, const long2 v1) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const int64b v) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const int64b v0, const int64b v1,
                          const int64b v2, const int64b v3) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const long2 v0, const int64b v1, const int64b v2) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const int64b v0, const long2 v1, const int64b v2) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const int64b v0, const int64b v1, const long2 v2) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const long2 v0, const long2 v1) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const long3 v0, const int64b v1) noexcept;

//! Make a long4 value
constexpr long4 makeLong4(const int64b v0, const long3 v1) noexcept;

//! Make a long8 value
constexpr long8 makeLong8(const int64b v) noexcept;

//! Make a long8 value
constexpr long8 makeLong8(const int64b v0, const int64b v1,
                          const int64b v2, const int64b v3,
                          const int64b v4, const int64b v5,
                          const int64b v6, const int64b v7) noexcept;

//! Make a long8 value
constexpr long8 makeLong8(const long2 v0, const long2 v1,
                          const long2 v2, const long2 v3) noexcept;

//! Make a long8 value
constexpr long8 makeLong8(const long4 v0, const long4 v1) noexcept;

//! Make a long16 value
constexpr long16 makeLong16(const int64b v) noexcept;

//! Make a long16 value
constexpr long16 makeLong16(const int64b v0, const int64b v1,
                            const int64b v2, const int64b v3,
                            const int64b v4, const int64b v5,
                            const int64b v6, const int64b v7,
                            const int64b v8, const int64b v9,
                            const int64b v10, const int64b v11,
                            const int64b v12, const int64b v13,
                            const int64b v14, const int64b v15) noexcept;

//! Make a long16 value
constexpr long16 makeLong16(const long2 v0, const long2 v1,
                            const long2 v2, const long2 v3,
                            const long2 v4, const long2 v5,
                            const long2 v6, const long2 v7) noexcept;

//! Make a long16 value
constexpr long16 makeLong16(const long4 v0, const long4 v1,
                            const long4 v2, const long4 v3) noexcept;

//! Make a long16 value
constexpr long16 makeLong16(const long8 v0, const long8 v1) noexcept;

//! Make a ulong2 value
constexpr ulong2 makeULong2(const uint64b v) noexcept;

//! Make a ulong2 value
constexpr ulong2 makeULong2(const uint64b v0, const uint64b v1) noexcept;

//! Make a ulong3 value
constexpr ulong3 makeULong3(const uint64b v) noexcept;

//! Make a ulong3 value
constexpr ulong3 makeULong3(const uint64b v0, const uint64b v1, const uint64b v2) noexcept;

//! Make a ulong3 value
constexpr ulong3 makeULong3(const ulong2 v0, const uint64b v1) noexcept;

//! Make a ulong3 value
constexpr ulong3 makeULong3(const uint64b v0, const ulong2 v1) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const uint64b v) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const uint64b v0, const uint64b v1,
                            const uint64b v2, const uint64b v3) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const ulong2 v0, const uint64b v1, const uint64b v2) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const uint64b v0, const ulong2 v1, const uint64b v2) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const uint64b v0, const uint64b v1, const ulong2 v2) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const ulong2 v0, const ulong2 v1) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const ulong3 v0, const uint64b v1) noexcept;

//! Make a ulong4 value
constexpr ulong4 makeULong4(const uint64b v0, const ulong3 v1) noexcept;

//! Make a ulong8 value
constexpr ulong8 makeULong8(const uint64b v) noexcept;

//! Make a ulong8 value
constexpr ulong8 makeULong8(const uint64b v0, const uint64b v1,
                            const uint64b v2, const uint64b v3,
                            const uint64b v4, const uint64b v5,
                            const uint64b v6, const uint64b v7) noexcept;

//! Make a ulong8 value
constexpr ulong8 makeULong8(const ulong2 v0, const ulong2 v1,
                            const ulong2 v2, const ulong2 v3) noexcept;

//! Make a ulong8 value
constexpr ulong8 makeULong8(const ulong4 v0, const ulong4 v1) noexcept;

//! Make a ulong16 value
constexpr ulong16 makeULong16(const uint64b v) noexcept;

//! Make a ulong16 value
constexpr ulong16 makeULong16(const uint64b v0, const uint64b v1,
                              const uint64b v2, const uint64b v3,
                              const uint64b v4, const uint64b v5,
                              const uint64b v6, const uint64b v7,
                              const uint64b v8, const uint64b v9,
                              const uint64b v10, const uint64b v11,
                              const uint64b v12, const uint64b v13,
                              const uint64b v14, const uint64b v15) noexcept;

//! Make a ulong16 value
constexpr ulong16 makeULong16(const ulong2 v0, const ulong2 v1,
                              const ulong2 v2, const ulong2 v3,
                              const ulong2 v4, const ulong2 v5,
                              const ulong2 v6, const ulong2 v7) noexcept;

//! Make a ulong16 value
constexpr ulong16 makeULong16(const ulong4 v0, const ulong4 v1,
                              const ulong4 v2, const ulong4 v3) noexcept;

//! Make a ulong16 value
constexpr ulong16 makeULong16(const ulong8 v0, const ulong8 v1) noexcept;

//! Make a float2 value
constexpr float2 makeFloat2(const float v) noexcept;

//! Make a float2 value
constexpr float2 makeFloat2(const float v0, const float v1) noexcept;

//! Make a float3 value
constexpr float3 makeFloat3(const float v) noexcept;

//! Make a float3 value
constexpr float3 makeFloat3(const float v0, const float v1, const float v2) noexcept;

//! Make a float3 value
constexpr float3 makeFloat3(const float2 v0, const float v1) noexcept;

//! Make a float3 value
constexpr float3 makeFloat3(const float v0, const float2 v1) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float v) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float v0, const float v1,
                            const float v2, const float v3) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float2 v0, const float v1, const float v2) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float v0, const float2 v1, const float v2) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float v0, const float v1, const float2 v2) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float2 v0, const float2 v1) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float3 v0, const float v1) noexcept;

//! Make a float4 value
constexpr float4 makeFloat4(const float v0, const float3 v1) noexcept;

//! Make a float8 value
constexpr float8 makeFloat8(const float v) noexcept;

//! Make a float8 value
constexpr float8 makeFloat8(const float v0, const float v1,
                            const float v2, const float v3,
                            const float v4, const float v5,
                            const float v6, const float v7) noexcept;

//! Make a float8 value
constexpr float8 makeFloat8(const float2 v0, const float2 v1,
                            const float2 v2, const float2 v3) noexcept;

//! Make a float8 value
constexpr float8 makeFloat8(const float4 v0, const float4 v1) noexcept;

//! Make a float16 value
constexpr float16 makeFloat16(const float v) noexcept;

//! Make a float16 value
constexpr float16 makeFloat16(const float v0, const float v1,
                              const float v2, const float v3,
                              const float v4, const float v5,
                              const float v6, const float v7,
                              const float v8, const float v9,
                              const float v10, const float v11,
                              const float v12, const float v13,
                              const float v14, const float v15) noexcept;

//! Make a float16 value
constexpr float16 makeFloat16(const float2 v0, const float2 v1,
                              const float2 v2, const float2 v3,
                              const float2 v4, const float2 v5,
                              const float2 v6, const float2 v7) noexcept;

//! Make a float16 value
constexpr float16 makeFloat16(const float4 v0, const float4 v1,
                              const float4 v2, const float4 v3) noexcept;

//! Make a float16 value
constexpr float16 makeFloat16(const float8 v0, const float8 v1) noexcept;

//! Make a double2 value
constexpr double2 makeDouble2(const double v) noexcept;

//! Make a double2 value
constexpr double2 makeDouble2(const double v0, const double v1) noexcept;

//! Make a double3 value
constexpr double3 makeDouble3(const double v) noexcept;

//! Make a double3 value
constexpr double3 makeDouble3(const double v0, const double v1, const double v2) noexcept;

//! Make a double3 value
constexpr double3 makeDouble3(const double2 v0, const double v1) noexcept;

//! Make a double3 value
constexpr double3 makeDouble3(const double v0, const double2 v1) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double v) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double v0, const double v1,
                              const double v2, const double v3) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double2 v0, const double v1, const double v2) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double v0, const double2 v1, const double v2) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double v0, const double v1, const double2 v2) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double2 v0, const double2 v1) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double3 v0, const double v1) noexcept;

//! Make a double4 value
constexpr double4 makeDouble4(const double v0, const double3 v1) noexcept;

//! Make a double8 value
constexpr double8 makeDouble8(const double v) noexcept;

//! Make a double8 value
constexpr double8 makeDouble8(const double v0, const double v1,
                              const double v2, const double v3,
                              const double v4, const double v5,
                              const double v6, const double v7) noexcept;

//! Make a double8 value
constexpr double8 makeDouble8(const double2 v0, const double2 v1,
                              const double2 v2, const double2 v3) noexcept;

//! Make a double8 value
constexpr double8 makeDouble8(const double4 v0, const double4 v1) noexcept;

//! Make a double16 value
constexpr double16 makeDouble16(const double v) noexcept;

//! Make a double16 value
constexpr double16 makeDouble16(const double v0, const double v1,
                                const double v2, const double v3,
                                const double v4, const double v5,
                                const double v6, const double v7,
                                const double v8, const double v9,
                                const double v10, const double v11,
                                const double v12, const double v13,
                                const double v14, const double v15) noexcept;

//! Make a double16 value
constexpr double16 makeDouble16(const double2 v0, const double2 v1,
                                const double2 v2, const double2 v3,
                                const double2 v4, const double2 v5,
                                const double2 v6, const double2 v7) noexcept;

//! Make a double16 value
constexpr double16 makeDouble16(const double4 v0, const double4 v1,
                                const double4 v2, const double4 v3) noexcept;

//! Make a double16 value
constexpr double16 makeDouble16(const double8 v0, const double8 v1) noexcept;

} // namespace zivc

#include "utility-inl.hpp"

#endif /* ZIVC_CL_UTILITY_HPP */
