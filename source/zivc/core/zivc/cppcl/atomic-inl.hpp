/*!
  \file atomic-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ATOMIC_INL_HPP
#define ZIVC_CL_ATOMIC_INL_HPP

#include "atomic.hpp"
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/thread/atomic.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_add(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::add(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_sub(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::sub(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
Type atomic_xchg(AddressSpacePointer<kAddressSpaceType, Type> p,
                 const Type value) noexcept
{
  const auto old = zisc::Atomic::exchange(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_inc(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept
{
  const auto old = zisc::Atomic::increment(p.get());
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_dec(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept
{
  const auto old = zisc::Atomic::decrement(p.get());
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_cmpxchg(AddressSpacePointer<kAddressSpaceType, Integer> p,
                       const Integer comp,
                       const Integer value) noexcept
{
  const auto old = zisc::Atomic::compareAndExchange(p.get(), comp, value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_min(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::min(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_max(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::max(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_and(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::bitAnd(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_or(AddressSpacePointer<kAddressSpaceType, Integer> p,
                  const Integer value) noexcept
{
  const auto old = zisc::Atomic::bitOr(p.get(), value);
  return old;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_xor(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  const auto old = zisc::Atomic::bitXor(p.get(), value);
  return old;
}

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_ATOMIC_INL_HPP
