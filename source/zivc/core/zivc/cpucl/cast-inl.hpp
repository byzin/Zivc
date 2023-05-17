/*!
  \file cast-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
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

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type, Scalar T> inline
Vector<Type, 2> Cast::convert(const Vector<T, 2>& from) noexcept
{
  if constexpr (std::is_same_v<Type, T>)
    return from;
  else
    return Vector<Type, 2>{static_cast<Type>(from.x), static_cast<Type>(from.y)};
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type, Scalar T> inline
Vector<Type, 3> Cast::convert(const Vector<T, 3>& from) noexcept
{
  if constexpr (std::is_same_v<Type, T>)
    return from;
  else
    return Vector<Type, 3>{static_cast<Type>(from.x), static_cast<Type>(from.y),
                           static_cast<Type>(from.z)};
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type, Scalar T> inline
Vector<Type, 4> Cast::convert(const Vector<T, 4>& from) noexcept
{
  if constexpr (std::is_same_v<Type, T>)
    return from;
  else
    return Vector<Type, 4>{static_cast<Type>(from.x), static_cast<Type>(from.y),
                           static_cast<Type>(from.z), static_cast<Type>(from.w)};
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type, Scalar T> inline
Vector<Type, 8> Cast::convert(const Vector<T, 8>& from) noexcept
{
  if constexpr (std::is_same_v<Type, T>)
    return from;
  else
    return Vector<Type, 8>{static_cast<Type>(from.s0), static_cast<Type>(from.s1),
                           static_cast<Type>(from.s2), static_cast<Type>(from.s3),
                           static_cast<Type>(from.s4), static_cast<Type>(from.s5),
                           static_cast<Type>(from.s6), static_cast<Type>(from.s7)};
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type, Scalar T> inline
Vector<Type, 16> Cast::convert(const Vector<T, 16>& from) noexcept
{
  if constexpr (std::is_same_v<Type, T>)
    return from;
  else
    return Vector<Type, 16>{static_cast<Type>(from.s0), static_cast<Type>(from.s1),
                            static_cast<Type>(from.s2), static_cast<Type>(from.s3),
                            static_cast<Type>(from.s4), static_cast<Type>(from.s5),
                            static_cast<Type>(from.s6), static_cast<Type>(from.s7),
                            static_cast<Type>(from.s8), static_cast<Type>(from.s9),
                            static_cast<Type>(from.sa), static_cast<Type>(from.sb),
                            static_cast<Type>(from.sc), static_cast<Type>(from.sd),
                            static_cast<Type>(from.se), static_cast<Type>(from.sf)};
}

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
  return Cast::convert<int8b, Type>(from);
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
  return Cast::convert<int8b, Type>(from);
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
  return Cast::convert<int8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
char8 convert_char8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<int8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
char16 convert_char16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<int8b, Type>(from);
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
  return Cast::convert<uint8b, Type>(from);
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
  return Cast::convert<uint8b, Type>(from);
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
  return Cast::convert<uint8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uchar8 convert_uchar8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<uint8b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uchar16 convert_uchar16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<uint8b, Type>(from);
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
  return Cast::convert<int16b, Type>(from);
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
  return Cast::convert<int16b, Type>(from);
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
  return Cast::convert<int16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
short8 convert_short8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<int16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
short16 convert_short16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<int16b, Type>(from);
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
  return Cast::convert<uint16b, Type>(from);
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
  return Cast::convert<uint16b, Type>(from);
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
  return Cast::convert<uint16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ushort8 convert_ushort8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<uint16b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ushort16 convert_ushort16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<uint16b, Type>(from);
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
  return Cast::convert<int32b, Type>(from);
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
  return Cast::convert<int32b, Type>(from);
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
  return Cast::convert<int32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int8 convert_int8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<int32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
int16 convert_int16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<int32b, Type>(from);
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
  return Cast::convert<uint32b, Type>(from);
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
  return Cast::convert<uint32b, Type>(from);
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
  return Cast::convert<uint32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint8 convert_uint8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<uint32b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
uint16 convert_uint16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<uint32b, Type>(from);
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
  return Cast::convert<int64b, Type>(from);
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
  return Cast::convert<int64b, Type>(from);
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
  return Cast::convert<int64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
long8 convert_long8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<int64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
long16 convert_long16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<int64b, Type>(from);
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
  return Cast::convert<uint64b, Type>(from);
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
  return Cast::convert<uint64b, Type>(from);
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
  return Cast::convert<uint64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ulong8 convert_ulong8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<uint64b, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
ulong16 convert_ulong16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<uint64b, Type>(from);
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
  return Cast::convert<float, Type>(from);
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
  return Cast::convert<float, Type>(from);
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
  return Cast::convert<float, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
float8 convert_float8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<float, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
float16 convert_float16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<float, Type>(from);
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
  return Cast::convert<double, Type>(from);
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
  return Cast::convert<double, Type>(from);
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
  return Cast::convert<double, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
double8 convert_double8(const Vector<Type, 8>& from) noexcept
{
  return Cast::convert<double, Type>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <Scalar Type> inline
double16 convert_double16(const Vector<Type, 16>& from) noexcept
{
  return Cast::convert<double, Type>(from);
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
char8 as_char8(const Type& from) noexcept requires SameSizeAs<char8, Type>
{
  return zisc::bit_cast<char8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
char16 as_char16(const Type& from) noexcept requires SameSizeAs<char16, Type>
{
  return zisc::bit_cast<char16>(from);
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
uchar8 as_uchar8(const Type& from) noexcept requires SameSizeAs<uchar8, Type>
{
  return zisc::bit_cast<uchar8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uchar16 as_uchar16(const Type& from) noexcept requires SameSizeAs<uchar16, Type>
{
  return zisc::bit_cast<uchar16>(from);
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
short8 as_short8(const Type& from) noexcept requires SameSizeAs<short8, Type>
{
  return zisc::bit_cast<short8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
short16 as_short16(const Type& from) noexcept requires SameSizeAs<short16, Type>
{
  return zisc::bit_cast<short16>(from);
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
ushort8 as_ushort8(const Type& from) noexcept requires SameSizeAs<ushort8, Type>
{
  return zisc::bit_cast<ushort8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ushort16 as_ushort16(const Type& from) noexcept requires SameSizeAs<ushort16, Type>
{
  return zisc::bit_cast<ushort16>(from);
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
int8 as_int8(const Type& from) noexcept requires SameSizeAs<int8, Type>
{
  return zisc::bit_cast<int8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
int16 as_int16(const Type& from) noexcept requires SameSizeAs<int16, Type>
{
  return zisc::bit_cast<int16>(from);
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
uint8 as_uint8(const Type& from) noexcept requires SameSizeAs<uint8, Type>
{
  return zisc::bit_cast<uint8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
uint16 as_uint16(const Type& from) noexcept requires SameSizeAs<uint16, Type>
{
  return zisc::bit_cast<uint16>(from);
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
long8 as_long8(const Type& from) noexcept requires SameSizeAs<long8, Type>
{
  return zisc::bit_cast<long8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
long16 as_long16(const Type& from) noexcept requires SameSizeAs<long16, Type>
{
  return zisc::bit_cast<long16>(from);
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
ulong8 as_ulong8(const Type& from) noexcept requires SameSizeAs<ulong8, Type>
{
  return zisc::bit_cast<ulong8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
ulong16 as_ulong16(const Type& from) noexcept requires SameSizeAs<ulong16, Type>
{
  return zisc::bit_cast<ulong16>(from);
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
float8 as_float8(const Type& from) noexcept requires SameSizeAs<float8, Type>
{
  return zisc::bit_cast<float8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
float16 as_float16(const Type& from) noexcept requires SameSizeAs<float16, Type>
{
  return zisc::bit_cast<float16>(from);
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

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double8 as_double8(const Type& from) noexcept requires SameSizeAs<double8, Type>
{
  return zisc::bit_cast<double8>(from);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] from No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
double16 as_double16(const Type& from) noexcept requires SameSizeAs<double16, Type>
{
  return zisc::bit_cast<double16>(from);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_CAST_INL_HPP
