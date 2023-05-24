/*!
  \file cast.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_CAST_HPP
#define ZIVC_CPUCL_CAST_HPP

// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.
  */
class Cast
{
 public:
  //! Convert to another vector type
  template <Scalar Type, Scalar T>
  static Vector<Type, 2> convert(const Vector<T, 2>& from) noexcept;

  //! Convert to another vector type
  template <Scalar Type, Scalar T>
  static Vector<Type, 3> convert(const Vector<T, 3>& from) noexcept;

  //! Convert to another vector type
  template <Scalar Type, Scalar T>
  static Vector<Type, 4> convert(const Vector<T, 4>& from) noexcept;

  //! Convert to another vector type
  template <Scalar Type, Scalar T>
  static Vector<Type, 8> convert(const Vector<T, 8>& from) noexcept;

  //! Convert to another vector type
  template <Scalar Type, Scalar T>
  static Vector<Type, 16> convert(const Vector<T, 16>& from) noexcept;
};

//! Convert to a char from
template <Scalar Type>
int8b convert_char(const Type& from) noexcept;

//! Convert to a char2 from
template <Scalar Type>
char2 convert_char2(const Vector<Type, 2>& from) noexcept;

//! Convert to a char3 from
template <Scalar Type>
char3 convert_char3(const Vector<Type, 3>& from) noexcept;

//! Convert to a char4 from
template <Scalar Type>
char4 convert_char4(const Vector<Type, 4>& from) noexcept;

//! Convert to a char8 from
template <Scalar Type>
char8 convert_char8(const Vector<Type, 8>& from) noexcept;

//! Convert to a char16 from
template <Scalar Type>
char16 convert_char16(const Vector<Type, 16>& from) noexcept;

//! Convert to a uchar from
template <Scalar Type>
uint8b convert_uchar(const Type& from) noexcept;

//! Convert to a uchar2 from
template <Scalar Type>
uchar2 convert_uchar2(const Vector<Type, 2>& from) noexcept;

//! Convert to a uchar3 from
template <Scalar Type>
uchar3 convert_uchar3(const Vector<Type, 3>& from) noexcept;

//! Convert to a uchar4 from
template <Scalar Type>
uchar4 convert_uchar4(const Vector<Type, 4>& from) noexcept;

//! Convert to a uchar8 from
template <Scalar Type>
uchar8 convert_uchar8(const Vector<Type, 8>& from) noexcept;

//! Convert to a uchar16 from
template <Scalar Type>
uchar16 convert_uchar16(const Vector<Type, 16>& from) noexcept;

//! Convert to a short from
template <Scalar Type>
int16b convert_short(const Type& from) noexcept;

//! Convert to a short2 from
template <Scalar Type>
short2 convert_short2(const Vector<Type, 2>& from) noexcept;

//! Convert to a short3 from
template <Scalar Type>
short3 convert_short3(const Vector<Type, 3>& from) noexcept;

//! Convert to a short4 from
template <Scalar Type>
short4 convert_short4(const Vector<Type, 4>& from) noexcept;

//! Convert to a short8 from
template <Scalar Type>
short8 convert_short8(const Vector<Type, 8>& from) noexcept;

//! Convert to a short16 from
template <Scalar Type>
short16 convert_short16(const Vector<Type, 16>& from) noexcept;

//! Convert to a ushort from
template <Scalar Type>
uint16b convert_ushort(const Type& from) noexcept;

//! Convert to a ushort2 from
template <Scalar Type>
ushort2 convert_ushort2(const Vector<Type, 2>& from) noexcept;

//! Convert to a ushort3 from
template <Scalar Type>
ushort3 convert_ushort3(const Vector<Type, 3>& from) noexcept;

//! Convert to a ushort4 from
template <Scalar Type>
ushort4 convert_ushort4(const Vector<Type, 4>& from) noexcept;

//! Convert to a ushort8 from
template <Scalar Type>
ushort8 convert_ushort8(const Vector<Type, 8>& from) noexcept;

//! Convert to a ushort16 from
template <Scalar Type>
ushort16 convert_ushort16(const Vector<Type, 16>& from) noexcept;

//! Convert to a int from
template <Scalar Type>
int32b convert_int(const Type& from) noexcept;

//! Convert to a int2 from
template <Scalar Type>
int2 convert_int2(const Vector<Type, 2>& from) noexcept;

//! Convert to a int3 from
template <Scalar Type>
int3 convert_int3(const Vector<Type, 3>& from) noexcept;

//! Convert to a int4 from
template <Scalar Type>
int4 convert_int4(const Vector<Type, 4>& from) noexcept;

//! Convert to a int8 from
template <Scalar Type>
int8 convert_int8(const Vector<Type, 8>& from) noexcept;

//! Convert to a int16 from
template <Scalar Type>
int16 convert_int16(const Vector<Type, 16>& from) noexcept;

//! Convert to a uint from
template <Scalar Type>
uint32b convert_uint(const Type& from) noexcept;

//! Convert to a uint2 from
template <Scalar Type>
uint2 convert_uint2(const Vector<Type, 2>& from) noexcept;

//! Convert to a uint3 from
template <Scalar Type>
uint3 convert_uint3(const Vector<Type, 3>& from) noexcept;

//! Convert to a uint4 from
template <Scalar Type>
uint4 convert_uint4(const Vector<Type, 4>& from) noexcept;

//! Convert to a uint8 from
template <Scalar Type>
uint8 convert_uint8(const Vector<Type, 8>& from) noexcept;

//! Convert to a uint16 from
template <Scalar Type>
uint16 convert_uint16(const Vector<Type, 16>& from) noexcept;

//! Convert to a long from
template <Scalar Type>
int64b convert_long(const Type& from) noexcept;

//! Convert to a long2 from
template <Scalar Type>
long2 convert_long2(const Vector<Type, 2>& from) noexcept;

//! Convert to a long3 from
template <Scalar Type>
long3 convert_long3(const Vector<Type, 3>& from) noexcept;

//! Convert to a long4 from
template <Scalar Type>
long4 convert_long4(const Vector<Type, 4>& from) noexcept;

//! Convert to a long8 from
template <Scalar Type>
long8 convert_long8(const Vector<Type, 8>& from) noexcept;

//! Convert to a long16 from
template <Scalar Type>
long16 convert_long16(const Vector<Type, 16>& from) noexcept;

//! Convert to a ulong from
template <Scalar Type>
uint64b convert_ulong(const Type& from) noexcept;

//! Convert to a ulong2 from
template <Scalar Type>
ulong2 convert_ulong2(const Vector<Type, 2>& from) noexcept;

//! Convert to a ulong3 from
template <Scalar Type>
ulong3 convert_ulong3(const Vector<Type, 3>& from) noexcept;

//! Convert to a ulong4 from
template <Scalar Type>
ulong4 convert_ulong4(const Vector<Type, 4>& from) noexcept;

//! Convert to a ulong8 from
template <Scalar Type>
ulong8 convert_ulong8(const Vector<Type, 8>& from) noexcept;

//! Convert to a ulong16 from
template <Scalar Type>
ulong16 convert_ulong16(const Vector<Type, 16>& from) noexcept;

//! Convert to a float from
template <Scalar Type>
float convert_float(const Type& from) noexcept;

//! Convert to a float2 from
template <Scalar Type>
float2 convert_float2(const Vector<Type, 2>& from) noexcept;

//! Convert to a float3 from
template <Scalar Type>
float3 convert_float3(const Vector<Type, 3>& from) noexcept;

//! Convert to a float4 from
template <Scalar Type>
float4 convert_float4(const Vector<Type, 4>& from) noexcept;

//! Convert to a float8 from
template <Scalar Type>
float8 convert_float8(const Vector<Type, 8>& from) noexcept;

//! Convert to a float16 from
template <Scalar Type>
float16 convert_float16(const Vector<Type, 16>& from) noexcept;

//! Convert to a double from
template <Scalar Type>
double convert_double(const Type& from) noexcept;

//! Convert to a double2 from
template <Scalar Type>
double2 convert_double2(const Vector<Type, 2>& from) noexcept;

//! Convert to a double3 from
template <Scalar Type>
double3 convert_double3(const Vector<Type, 3>& from) noexcept;

//! Convert to a double4 from
template <Scalar Type>
double4 convert_double4(const Vector<Type, 4>& from) noexcept;

//! Convert to a double8 from
template <Scalar Type>
double8 convert_double8(const Vector<Type, 8>& from) noexcept;

//! Convert to a double16 from
template <Scalar Type>
double16 convert_double16(const Vector<Type, 16>& from) noexcept;

//! A data is reinterpreted as a char data
template <zisc::TriviallyCopyable Type>
int8b as_char(const Type& from) noexcept requires SameSizeAs<int8b, Type>;

//! A data is reinterpreted as a char2 data
template <zisc::TriviallyCopyable Type>
char2 as_char2(const Type& from) noexcept requires SameSizeAs<char2, Type>;

//! A data is reinterpreted as a char3 data
template <zisc::TriviallyCopyable Type>
char3 as_char3(const Type& from) noexcept requires SameSizeAs<char3, Type>;

//! A data is reinterpreted as a char4 data
template <zisc::TriviallyCopyable Type>
char4 as_char4(const Type& from) noexcept requires SameSizeAs<char4, Type>;

//! A data is reinterpreted as a char8 data
template <zisc::TriviallyCopyable Type>
char8 as_char8(const Type& from) noexcept requires SameSizeAs<char8, Type>;

//! A data is reinterpreted as a char16 data
template <zisc::TriviallyCopyable Type>
char16 as_char16(const Type& from) noexcept requires SameSizeAs<char16, Type>;

//! A data is reinterpreted as a uchar data
template <zisc::TriviallyCopyable Type>
uint8b as_uchar(const Type& from) noexcept requires SameSizeAs<uint8b, Type>;

//! A data is reinterpreted as a uchar2 data
template <zisc::TriviallyCopyable Type>
uchar2 as_uchar2(const Type& from) noexcept requires SameSizeAs<uchar2, Type>;

//! A data is reinterpreted as a uchar3 data
template <zisc::TriviallyCopyable Type>
uchar3 as_uchar3(const Type& from) noexcept requires SameSizeAs<uchar3, Type>;

//! A data is reinterpreted as a uchar4 data
template <zisc::TriviallyCopyable Type>
uchar4 as_uchar4(const Type& from) noexcept requires SameSizeAs<uchar4, Type>;

//! A data is reinterpreted as a uchar8 data
template <zisc::TriviallyCopyable Type>
uchar8 as_uchar8(const Type& from) noexcept requires SameSizeAs<uchar8, Type>;

//! A data is reinterpreted as a uchar16 data
template <zisc::TriviallyCopyable Type>
uchar16 as_uchar16(const Type& from) noexcept requires SameSizeAs<uchar16, Type>;

//! A data is reinterpreted as a short data
template <zisc::TriviallyCopyable Type>
int16b as_short(const Type& from) noexcept requires SameSizeAs<int16b, Type>;

//! A data is reinterpreted as a short2 data
template <zisc::TriviallyCopyable Type>
short2 as_short2(const Type& from) noexcept requires SameSizeAs<short2, Type>;

//! A data is reinterpreted as a short3 data
template <zisc::TriviallyCopyable Type>
short3 as_short3(const Type& from) noexcept requires SameSizeAs<short3, Type>;

//! A data is reinterpreted as a short4 data
template <zisc::TriviallyCopyable Type>
short4 as_short4(const Type& from) noexcept requires SameSizeAs<short4, Type>;

//! A data is reinterpreted as a short8 data
template <zisc::TriviallyCopyable Type>
short8 as_short8(const Type& from) noexcept requires SameSizeAs<short8, Type>;

//! A data is reinterpreted as a short16 data
template <zisc::TriviallyCopyable Type>
short16 as_short16(const Type& from) noexcept requires SameSizeAs<short16, Type>;

//! A data is reinterpreted as a ushort data
template <zisc::TriviallyCopyable Type>
uint16b as_ushort(const Type& from) noexcept requires SameSizeAs<uint16b, Type>;

//! A data is reinterpreted as a ushort2 data
template <zisc::TriviallyCopyable Type>
ushort2 as_ushort2(const Type& from) noexcept requires SameSizeAs<ushort2, Type>;

//! A data is reinterpreted as a ushort3 data
template <zisc::TriviallyCopyable Type>
ushort3 as_ushort3(const Type& from) noexcept requires SameSizeAs<ushort3, Type>;

//! A data is reinterpreted as a ushort4 data
template <zisc::TriviallyCopyable Type>
ushort4 as_ushort4(const Type& from) noexcept requires SameSizeAs<ushort4, Type>;

//! A data is reinterpreted as a ushort8 data
template <zisc::TriviallyCopyable Type>
ushort8 as_ushort8(const Type& from) noexcept requires SameSizeAs<ushort8, Type>;

//! A data is reinterpreted as a ushort16 data
template <zisc::TriviallyCopyable Type>
ushort16 as_ushort16(const Type& from) noexcept requires SameSizeAs<ushort16, Type>;

//! A data is reinterpreted as a int data
template <zisc::TriviallyCopyable Type>
int32b as_int(const Type& from) noexcept requires SameSizeAs<int32b, Type>;

//! A data is reinterpreted as a int2 data
template <zisc::TriviallyCopyable Type>
int2 as_int2(const Type& from) noexcept requires SameSizeAs<int2, Type>;

//! A data is reinterpreted as a int3 data
template <zisc::TriviallyCopyable Type>
int3 as_int3(const Type& from) noexcept requires SameSizeAs<int3, Type>;

//! A data is reinterpreted as a int4 data
template <zisc::TriviallyCopyable Type>
int4 as_int4(const Type& from) noexcept requires SameSizeAs<int4, Type>;

//! A data is reinterpreted as a int8 data
template <zisc::TriviallyCopyable Type>
int8 as_int8(const Type& from) noexcept requires SameSizeAs<int8, Type>;

//! A data is reinterpreted as a int16 data
template <zisc::TriviallyCopyable Type>
int16 as_int16(const Type& from) noexcept requires SameSizeAs<int16, Type>;

//! A data is reinterpreted as a uint data
template <zisc::TriviallyCopyable Type>
uint32b as_uint(const Type& from) noexcept requires SameSizeAs<uint32b, Type>;

//! A data is reinterpreted as a uint2 data
template <zisc::TriviallyCopyable Type>
uint2 as_uint2(const Type& from) noexcept requires SameSizeAs<uint2, Type>;

//! A data is reinterpreted as a uint3 data
template <zisc::TriviallyCopyable Type>
uint3 as_uint3(const Type& from) noexcept requires SameSizeAs<uint3, Type>;

//! A data is reinterpreted as a uint4 data
template <zisc::TriviallyCopyable Type>
uint4 as_uint4(const Type& from) noexcept requires SameSizeAs<uint4, Type>;

//! A data is reinterpreted as a uint8 data
template <zisc::TriviallyCopyable Type>
uint8 as_uint8(const Type& from) noexcept requires SameSizeAs<uint8, Type>;

//! A data is reinterpreted as a uint16 data
template <zisc::TriviallyCopyable Type>
uint16 as_uint16(const Type& from) noexcept requires SameSizeAs<uint16, Type>;

//! A data is reinterpreted as a long data
template <zisc::TriviallyCopyable Type>
int64b as_long(const Type& from) noexcept requires SameSizeAs<int64b, Type>;

//! A data is reinterpreted as a long2 data
template <zisc::TriviallyCopyable Type>
long2 as_long2(const Type& from) noexcept requires SameSizeAs<long2, Type>;

//! A data is reinterpreted as a long3 data
template <zisc::TriviallyCopyable Type>
long3 as_long3(const Type& from) noexcept requires SameSizeAs<long3, Type>;

//! A data is reinterpreted as a long4 data
template <zisc::TriviallyCopyable Type>
long4 as_long4(const Type& from) noexcept requires SameSizeAs<long4, Type>;

//! A data is reinterpreted as a long8 data
template <zisc::TriviallyCopyable Type>
long8 as_long8(const Type& from) noexcept requires SameSizeAs<long8, Type>;

//! A data is reinterpreted as a long16 data
template <zisc::TriviallyCopyable Type>
long16 as_long16(const Type& from) noexcept requires SameSizeAs<long16, Type>;

//! A data is reinterpreted as a ulong data
template <zisc::TriviallyCopyable Type>
uint64b as_ulong(const Type& from) noexcept requires SameSizeAs<uint64b, Type>;

//! A data is reinterpreted as a ulong2 data
template <zisc::TriviallyCopyable Type>
ulong2 as_ulong2(const Type& from) noexcept requires SameSizeAs<ulong2, Type>;

//! A data is reinterpreted as a ulong3 data
template <zisc::TriviallyCopyable Type>
ulong3 as_ulong3(const Type& from) noexcept requires SameSizeAs<ulong3, Type>;

//! A data is reinterpreted as a ulong4 data
template <zisc::TriviallyCopyable Type>
ulong4 as_ulong4(const Type& from) noexcept requires SameSizeAs<ulong4, Type>;

//! A data is reinterpreted as a ulong8 data
template <zisc::TriviallyCopyable Type>
ulong8 as_ulong8(const Type& from) noexcept requires SameSizeAs<ulong8, Type>;

//! A data is reinterpreted as a ulong16 data
template <zisc::TriviallyCopyable Type>
ulong16 as_ulong16(const Type& from) noexcept requires SameSizeAs<ulong16, Type>;

//! A data is reinterpreted as a float data
template <zisc::TriviallyCopyable Type>
float as_float(const Type& from) noexcept requires SameSizeAs<float, Type>;

//! A data is reinterpreted as a float2 data
template <zisc::TriviallyCopyable Type>
float2 as_float2(const Type& from) noexcept requires SameSizeAs<float2, Type>;

//! A data is reinterpreted as a float3 data
template <zisc::TriviallyCopyable Type>
float3 as_float3(const Type& from) noexcept requires SameSizeAs<float3, Type>;

//! A data is reinterpreted as a float4 data
template <zisc::TriviallyCopyable Type>
float4 as_float4(const Type& from) noexcept requires SameSizeAs<float4, Type>;

//! A data is reinterpreted as a float8 data
template <zisc::TriviallyCopyable Type>
float8 as_float8(const Type& from) noexcept requires SameSizeAs<float8, Type>;

//! A data is reinterpreted as a float16 data
template <zisc::TriviallyCopyable Type>
float16 as_float16(const Type& from) noexcept requires SameSizeAs<float16, Type>;

//! A data is reinterpreted as a double data
template <zisc::TriviallyCopyable Type>
double as_double(const Type& from) noexcept requires SameSizeAs<double, Type>;

//! A data is reinterpreted as a double2 data
template <zisc::TriviallyCopyable Type>
double2 as_double2(const Type& from) noexcept requires SameSizeAs<double2, Type>;

//! A data is reinterpreted as a double3 data
template <zisc::TriviallyCopyable Type>
double3 as_double3(const Type& from) noexcept requires SameSizeAs<double3, Type>;

//! A data is reinterpreted as a double4 data
template <zisc::TriviallyCopyable Type>
double4 as_double4(const Type& from) noexcept requires SameSizeAs<double4, Type>;

//! A data is reinterpreted as a double8 data
template <zisc::TriviallyCopyable Type>
double8 as_double8(const Type& from) noexcept requires SameSizeAs<double8, Type>;

//! A data is reinterpreted as a double16 data
template <zisc::TriviallyCopyable Type>
double16 as_double16(const Type& from) noexcept requires SameSizeAs<double16, Type>;

} // namespace zivc::cl

#include "cast-inl.hpp"

#endif // ZIVC_CPUCL_CAST_HPP
