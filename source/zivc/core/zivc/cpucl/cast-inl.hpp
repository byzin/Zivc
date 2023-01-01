/*!
  \file cast-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_CAST_INL_HPP
#define ZIVC_CPUCL_CAST_INL_HPP

#include "cast.hpp"
// Standard C++ library
#include <array>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

namespace inner {

/*!
  \details No detailed description

  \tparam T No description.
  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar T, Scalar Type> inline
Vector<T, 2> convertToVector(const Vector<Type, 2>& from) noexcept
{
  if constexpr (std::is_same_v<T, Type>)
    return from;
  else
    return Vector<T, 2>{static_cast<T>(from.x), static_cast<T>(from.y)};
}

/*!
  \details No detailed description

  \tparam T No description.
  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar T, Scalar Type> inline
Vector<T, 3> convertToVector(const Vector<Type, 3>& from) noexcept
{
  if constexpr (std::is_same_v<T, Type>)
    return from;
  else
    return Vector<T, 3>{static_cast<T>(from.x), static_cast<T>(from.y),
                        static_cast<T>(from.z)};
}

/*!
  \details No detailed description

  \tparam T No description.
  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar T, Scalar Type> inline
Vector<T, 4> convertToVector(const Vector<Type, 4>& from) noexcept
{
  if constexpr (std::is_same_v<T, Type>)
    return from;
  else
    return Vector<T, 4>{static_cast<T>(from.x), static_cast<T>(from.y),
                        static_cast<T>(from.z), static_cast<T>(from.w)};
}

} // namespace inner

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int8b convert_char(const Type& from) noexcept
{
  return static_cast<int8b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
char2 convert_char2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<int8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
char3 convert_char3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<int8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
char4 convert_char4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<int8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint8b convert_uchar(const Type& from) noexcept
{
  return static_cast<uint8b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uchar2 convert_uchar2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<uint8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uchar3 convert_uchar3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<uint8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uchar4 convert_uchar4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<uint8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int16b convert_short(const Type& from) noexcept
{
  return static_cast<int16b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
short2 convert_short2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<int16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
short3 convert_short3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<int16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
short4 convert_short4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<int16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint16b convert_ushort(const Type& from) noexcept
{
  return static_cast<uint16b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ushort2 convert_ushort2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<uint16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ushort3 convert_ushort3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<uint16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ushort4 convert_ushort4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<uint16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int32b convert_int(const Type& from) noexcept
{
  return static_cast<int32b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int2 convert_int2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<int32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int3 convert_int3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<int32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int4 convert_int4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<int32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint32b convert_uint(const Type& from) noexcept
{
  return static_cast<uint32b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint2 convert_uint2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<uint32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint3 convert_uint3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<uint32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint4 convert_uint4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<uint32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int64b convert_long(const Type& from) noexcept
{
  return static_cast<int64b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
long2 convert_long2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<int64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
long3 convert_long3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<int64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
long4 convert_long4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<int64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint64b convert_ulong(const Type& from) noexcept
{
  return static_cast<uint64b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ulong2 convert_ulong2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<uint64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ulong3 convert_ulong3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<uint64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ulong4 convert_ulong4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<uint64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \return No description
  */
template <Scalar Type> inline
float convert_float(const Type& from) noexcept
{
  return static_cast<float>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
float2 convert_float2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<float, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
float3 convert_float3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<float, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
float4 convert_float4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<float, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from description.
  \return No description
  */
template <Scalar Type> inline
double convert_double(const Type& from) noexcept
{
  return static_cast<double>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
double2 convert_double2(const Vector<Type, 2>& from) noexcept
{
  return inner::convertToVector<double, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
double3 convert_double3(const Vector<Type, 3>& from) noexcept
{
  return inner::convertToVector<double, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
double4 convert_double4(const Vector<Type, 4>& from) noexcept
{
  return inner::convertToVector<double, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int8b as_char(const Type& from) noexcept requires SameSizeAs<int8b, Type>
{
  return zisc::bit_cast<int8b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
char2 as_char2(const Type& from) noexcept requires SameSizeAs<char2, Type>
{
  return zisc::bit_cast<char2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
char3 as_char3(const Type& from) noexcept requires SameSizeAs<char3, Type>
{
  return zisc::bit_cast<char3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
char4 as_char4(const Type& from) noexcept requires SameSizeAs<char4, Type>
{
  return zisc::bit_cast<char4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint8b as_uchar(const Type& from) noexcept requires SameSizeAs<uint8b, Type>
{
  return zisc::bit_cast<uint8b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uchar2 as_uchar2(const Type& from) noexcept requires SameSizeAs<uchar2, Type>
{
  return zisc::bit_cast<uchar2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uchar3 as_uchar3(const Type& from) noexcept requires SameSizeAs<uchar3, Type>
{
  return zisc::bit_cast<uchar3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uchar4 as_uchar4(const Type& from) noexcept requires SameSizeAs<uchar4, Type>
{
  return zisc::bit_cast<uchar4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int16b as_short(const Type& from) noexcept requires SameSizeAs<int16b, Type>
{
  return zisc::bit_cast<int16b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
short2 as_short2(const Type& from) noexcept requires SameSizeAs<short2, Type>
{
  return zisc::bit_cast<short2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
short3 as_short3(const Type& from) noexcept requires SameSizeAs<short3, Type>
{
  return zisc::bit_cast<short3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
short4 as_short4(const Type& from) noexcept requires SameSizeAs<short4, Type>
{
  return zisc::bit_cast<short4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint16b as_ushort(const Type& from) noexcept requires SameSizeAs<uint16b, Type>
{
  return zisc::bit_cast<uint16b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ushort2 as_ushort2(const Type& from) noexcept requires SameSizeAs<ushort2, Type>
{
  return zisc::bit_cast<ushort2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ushort3 as_ushort3(const Type& from) noexcept requires SameSizeAs<ushort3, Type>
{
  return zisc::bit_cast<ushort3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ushort4 as_ushort4(const Type& from) noexcept requires SameSizeAs<ushort4, Type>
{
  return zisc::bit_cast<ushort4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int32b as_int(const Type& from) noexcept requires SameSizeAs<int32b, Type>
{
  return zisc::bit_cast<int32b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int2 as_int2(const Type& from) noexcept requires SameSizeAs<int2, Type>
{
  return zisc::bit_cast<int2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int3 as_int3(const Type& from) noexcept requires SameSizeAs<int3, Type>
{
  return zisc::bit_cast<int3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int4 as_int4(const Type& from) noexcept requires SameSizeAs<int4, Type>
{
  return zisc::bit_cast<int4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint32b as_uint(const Type& from) noexcept requires SameSizeAs<uint32b, Type>
{
  return zisc::bit_cast<uint32b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint2 as_uint2(const Type& from) noexcept requires SameSizeAs<uint2, Type>
{
  return zisc::bit_cast<uint2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint3 as_uint3(const Type& from) noexcept requires SameSizeAs<uint3, Type>
{
  return zisc::bit_cast<uint3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint4 as_uint4(const Type& from) noexcept requires SameSizeAs<uint4, Type>
{
  return zisc::bit_cast<uint4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int64b as_long(const Type& from) noexcept requires SameSizeAs<int64b, Type>
{
  return zisc::bit_cast<int64b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
long2 as_long2(const Type& from) noexcept requires SameSizeAs<long2, Type>
{
  return zisc::bit_cast<long2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
long3 as_long3(const Type& from) noexcept requires SameSizeAs<long3, Type>
{
  return zisc::bit_cast<long3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
long4 as_long4(const Type& from) noexcept requires SameSizeAs<long4, Type>
{
  return zisc::bit_cast<long4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint64b as_ulong(const Type& from) noexcept requires SameSizeAs<uint64b, Type>
{
  return zisc::bit_cast<uint64b>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ulong2 as_ulong2(const Type& from) noexcept requires SameSizeAs<ulong2, Type>
{
  return zisc::bit_cast<ulong2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ulong3 as_ulong3(const Type& from) noexcept requires SameSizeAs<ulong3, Type>
{
  return zisc::bit_cast<ulong3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ulong4 as_ulong4(const Type& from) noexcept requires SameSizeAs<ulong4, Type>
{
  return zisc::bit_cast<ulong4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
float as_float(const Type& from) noexcept requires SameSizeAs<float, Type>
{
  return zisc::bit_cast<float>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
float2 as_float2(const Type& from) noexcept requires SameSizeAs<float2, Type>
{
  return zisc::bit_cast<float2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
float3 as_float3(const Type& from) noexcept requires SameSizeAs<float3, Type>
{
  return zisc::bit_cast<float3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
float4 as_float4(const Type& from) noexcept requires SameSizeAs<float4, Type>
{
  return zisc::bit_cast<float4>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double as_double(const Type& from) noexcept requires SameSizeAs<double, Type>
{
  return zisc::bit_cast<double>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double2 as_double2(const Type& from) noexcept requires SameSizeAs<double2, Type>
{
  return zisc::bit_cast<double2>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double3 as_double3(const Type& from) noexcept requires SameSizeAs<double3, Type>
{
  return zisc::bit_cast<double3>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double4 as_double4(const Type& from) noexcept requires SameSizeAs<double4, Type>
{
  return zisc::bit_cast<double4>(from);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_CAST_INL_HPP
