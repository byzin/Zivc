/*!
  \file bit-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_BIT_INL_HPP
#define ZIVC_CL_BIT_INL_HPP

#include "bit.hpp"
#include "types.hpp"

namespace zivc {

#define ZIVC_CL_DEFINE_CAST_BIT_IMPL(type) \
  template <> \
  struct Bit::CastImpl< type > \
  { \
    template <typename From> \
    static constexpr type cast(const From from) noexcept \
    { \
      return as_ ## type (from); \
    } \
  }

ZIVC_CL_DEFINE_CAST_BIT_IMPL(char);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(char2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(char3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(char4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(char8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(char16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(short16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ushort16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(int16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(long16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(ulong16);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half2);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half3);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half4);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half8);
//ZIVC_CL_DEFINE_CAST_BIT_IMPL(half16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(float16);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double2);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double3);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double8);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(double16);

/*!
  \details No detailed description

  \tparam To No description.
  \tparam From No description.
  \param [in] from No description.
  \return No description
  */
template <typename To, typename From> inline
constexpr To Bit::castBit(const From& from) noexcept
{
  static_assert(sizeof(From) == sizeof(To),
                "The 'From' and 'To' have different number of bytes.");
  const To to = CastImpl<To>::cast(from);
  return to;
}

/*!
  \details No detailed description

  \tparam To No description.
  \tparam From No description.
  \param [in] from No description.
  \return No description
  */
template <typename To, typename From> inline
constexpr To castBit(const From& from) noexcept
{
  const To to = Bit::castBit<To>(from);
  return to;
}

} // namespace zivc

#endif // ZIVC_CL_BIT_INL_HPP
