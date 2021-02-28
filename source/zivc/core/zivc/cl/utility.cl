/*!
  \file utility.cl
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_UTILITY_CL
#define ZIVC_UTILITY_CL

// Zivc
#include "types.cl"
#include "type_traits.cl"

namespace zivc {

//! Write the message on standard output
template <typename ...Types>
int32b print(ConstConstantPtr<char> format, const Types... args) noexcept;

//! Assert the message on standard error output and raise an error
template <typename ...Types>
void assertIfFalse(const int32b condition,
                   ConstConstantPtr<char> format,
                   const Types... args) noexcept;

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
template <typename Type, typename ...ArgumentTypes>
Type make(const ArgumentTypes... args) noexcept;

//! Make a char2 value
char2 makeChar2(const int8b v) noexcept;

//! Make a char2 value
char2 makeChar2(const int8b x, const int8b y) noexcept;

//! Make a char3 value
char3 makeChar3(const int8b v) noexcept;

//! Make a char3 value
char3 makeChar3(const int8b x, const int8b y, const int8b z) noexcept;

//! Make a char4 value
char4 makeChar4(const int8b v) noexcept;

//! Make a char4 value
char4 makeChar4(const int8b x, const int8b y,
                const int8b z, const int8b w) noexcept;

//! Make a uchar2 value
uchar2 makeUChar2(const uint8b v) noexcept;

//! Make a uchar2 value
uchar2 makeUChar2(const uint8b x, const uint8b y) noexcept;

//! Make a uchar3 value
uchar3 makeUChar3(const uint8b v) noexcept;

//! Make a uchar3 value
uchar3 makeUChar3(const uint8b x, const uint8b y, const uint8b z) noexcept;

//! Make a uchar4 value
uchar4 makeUChar4(const uint8b v) noexcept;

//! Make a uchar4 value
uchar4 makeUChar4(const uint8b x, const uint8b y,
                  const uint8b z, const uint8b w) noexcept;

//! Make a short2 value
short2 makeShort2(const int16b v) noexcept;

//! Make a short2 value
short2 makeShort2(const int16b x, const int16b y) noexcept;

//! Make a short3 value
short3 makeShort3(const int16b v) noexcept;

//! Make a short3 value
short3 makeShort3(const int16b x, const int16b y, const int16b z) noexcept;

//! Make a short4 value
short4 makeShort4(const int16b v) noexcept;

//! Make a short4 value
short4 makeShort4(const int16b x, const int16b y,
                  const int16b z, const int16b w) noexcept;

//! Make a ushort2 value
ushort2 makeUShort2(const uint16b v) noexcept;

//! Make a ushort2 value
ushort2 makeUShort2(const uint16b x, const uint16b y) noexcept;

//! Make a ushort3 value
ushort3 makeUShort3(const uint16b v) noexcept;

//! Make a ushort3 value
ushort3 makeUShort3(const uint16b x, const uint16b y, const uint16b z) noexcept;

//! Make a ushort4 value
ushort4 makeUShort4(const uint16b v) noexcept;

//! Make a ushort4 value
ushort4 makeUShort4(const uint16b x, const uint16b y,
                  const uint16b z, const uint16b w) noexcept;

//! Make a int2 value
int2 makeInt2(const int32b v) noexcept;

//! Make a int2 value
int2 makeInt2(const int32b x, const int32b y) noexcept;

//! Make a int3 value
int3 makeInt3(const int32b v) noexcept;

//! Make a int3 value
int3 makeInt3(const int32b x, const int32b y, const int32b z) noexcept;

//! Make a int4 value
int4 makeInt4(const int32b v) noexcept;

//! Make a int4 value
int4 makeInt4(const int32b x, const int32b y,
              const int32b z, const int32b w) noexcept;

//! Make a uint2 value
uint2 makeUInt2(const uint32b v) noexcept;

//! Make a uint2 value
uint2 makeUInt2(const uint32b x, const uint32b y) noexcept;

//! Make a uint3 value
uint3 makeUInt3(const uint32b v) noexcept;

//! Make a uint3 value
uint3 makeUInt3(const uint32b x, const uint32b y, const uint32b z) noexcept;

//! Make a uint4 value
uint4 makeUInt4(const uint32b v) noexcept;

//! Make a uint4 value
uint4 makeUInt4(const uint32b x, const uint32b y,
                const uint32b z, const uint32b w) noexcept;

//! Make a long2 value
long2 makeLong2(const int64b v) noexcept;

//! Make a long2 value
long2 makeLong2(const int64b x, const int64b y) noexcept;

//! Make a long3 value
long3 makeLong3(const int64b v) noexcept;

//! Make a long3 value
long3 makeLong3(const int64b x, const int64b y, const int64b z) noexcept;

//! Make a long4 value
long4 makeLong4(const int64b v) noexcept;

//! Make a long4 value
long4 makeLong4(const int64b x, const int64b y,
                const int64b z, const int64b w) noexcept;

//! Make a ulong2 value
ulong2 makeULong2(const uint64b v) noexcept;

//! Make a ulong2 value
ulong2 makeULong2(const uint64b x, const uint64b y) noexcept;

//! Make a ulong3 value
ulong3 makeULong3(const uint64b v) noexcept;

//! Make a ulong3 value
ulong3 makeULong3(const uint64b x, const uint64b y, const uint64b z) noexcept;

//! Make a ulong4 value
ulong4 makeULong4(const uint64b v) noexcept;

//! Make a ulong4 value
ulong4 makeULong4(const uint64b x, const uint64b y,
                  const uint64b z, const uint64b w) noexcept;

//! Make a float2 value
float2 makeFloat2(const float v) noexcept;

//! Make a float2 value
float2 makeFloat2(const float x, const float y) noexcept;

//! Make a float3 value
float3 makeFloat3(const float v) noexcept;

//! Make a float3 value
float3 makeFloat3(const float x, const float y, const float z) noexcept;

//! Make a float4 value
float4 makeFloat4(const float v) noexcept;

//! Make a float4 value
float4 makeFloat4(const float x, const float y,
                  const float z, const float w) noexcept;

//! Make a double2 value
double2 makeDouble2(const double v) noexcept;

//! Make a double2 value
double2 makeDouble2(const double x, const double y) noexcept;

//! Make a double3 value
double3 makeDouble3(const double v) noexcept;

//! Make a double3 value
double3 makeDouble3(const double x, const double y, const double z) noexcept;

//! Make a double4 value
double4 makeDouble4(const double v) noexcept;

//! Make a double4 value
double4 makeDouble4(const double x, const double y,
                    const double z, const double w) noexcept;

//! Convert type from T to Type
template <typename Type, typename T>
Type cast(T value) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
Type treatAs(T object) noexcept;

//!
template <typename Type>
Type&& forward(RemoveReferenceType<Type>& t) noexcept;

template <typename Type>
Type&& forward(RemoveReferenceType<Type>&& t) noexcept;

} // namespace zivc

#include "utility-inl.cl"

#endif /* ZIVC_UTILITY_CL */
