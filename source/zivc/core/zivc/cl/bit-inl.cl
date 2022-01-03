/*!
  \file bit-inl.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BIT_INL_CL
#define ZIVC_BIT_INL_CL

#include "bit.cl"

namespace zivc {

#define ZIVC_CL_DEFINE_CAST_BIT_IMPL(type) \
  template <> \
  struct Bit::CastImpl< type > \
  { \
    template <typename From> \
    static type cast(const From from) noexcept \
    { \
      return as_ ## type (from); \
    } \
  }

ZIVC_CL_DEFINE_CAST_BIT_IMPL(uchar4);
ZIVC_CL_DEFINE_CAST_BIT_IMPL(uint);

template <typename To, typename From> inline
To Bit::castBit(const From& from) noexcept
{
  static_assert(sizeof(From) == sizeof(To),
                "The 'From' and 'To' have different number of bytes.");
  const To to = CastImpl<To>::cast(from);
  return to;
}

//! Reinterpret the object representation of one type as that of another
template <typename To, typename From> inline
To castBit(const From& from) noexcept
{
  const To to = Bit::castBit<To>(from);
  return to;
}

} // namespace zivc

#endif // ZIVC_BIT_INL_CL
