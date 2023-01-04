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

//! Write the message on standard output
template <typename ...Types>
int32b print(ConstConstantPtr<char> format, const Types&... args) noexcept;

//! Assert the message on standard error output and raise an error
template <typename ...Types>
void assertIfFalse(const int32b condition,
                   ConstConstantPtr<char> format,
                   const Types&... args) noexcept;

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

//! Make a value
template <typename Type, typename ...ArgTypes>
constexpr Type make(const ArgTypes&... args) noexcept;

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

// Conversion functions

//! Convert type from T to Type
template <typename Type, typename T>
Type cast(T&& value) noexcept;

//! Convert an pointer type to another pointer type
template <typename Type, typename T>
Type castPointer(T&& ptr) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
Type reinterp(T object) noexcept;

//

//! Check if the given two values are same
template <typename Type1, typename Type2>
constexpr bool equal(const Type1& lhs, const Type2& rhs) noexcept;

//!
template <typename Type>
Type&& forward(RemoveReferenceType<Type>& t) noexcept;

template <typename Type>
Type&& forward(RemoveReferenceType<Type>&& t) noexcept;

} // namespace zivc

#include "utility-inl.hpp"

#endif /* ZIVC_CL_UTILITY_HPP */
