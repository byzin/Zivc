/*!
  \file atomic-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ATOMIC_INL_HPP
#define ZIVC_CL_ATOMIC_INL_HPP

#include "atomic.hpp"
// Zivc
#include "types.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename AddressSpacePointer>
class Atomic::AtomicObject
{
  using T = RemoveCvrefAddressT<RemovePointerT<AddressSpacePointer>>;
  static_assert(sizeof(atomic_int) == 4);
  static_assert(sizeof(atomic_long) == 8);
  static_assert(sizeof(atomic_uint) == 4);
  static_assert(sizeof(atomic_ulong) == 8);
  static_assert(sizeof(atomic_float) == 4);
  static_assert(sizeof(atomic_double) == 8);
  using AtomicT = ConditionalT<kIsSignedInteger<T>,
                      ConditionalT<sizeof(T) == 4, atomic_int,
                      ConditionalT<sizeof(T) == 8, atomic_long,
                                                   void>>,
                  ConditionalT<kIsUnsignedInteger<T>,
                      ConditionalT<sizeof(T) == 4, atomic_uint,
                      ConditionalT<sizeof(T) == 8, atomic_ulong,
                                                   void>>,
                  ConditionalT<kIsFloat<T>,
                      ConditionalT<sizeof(T) == 4, atomic_float,
                      ConditionalT<sizeof(T) == 8, atomic_double,
                                                   void>>,
                  void>>>;
  static_assert(!kIsSame<void, AtomicT>, "The pointer is unsupported atomic type.");
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
#if !defined(ZIVC_CL_CPU)
  static_assert((ASpaceInfo::isGlobal() != 0) || (ASpaceInfo::isLocal() != 0),
                "The address space is unsupported.");
#endif // ZIVC_CL_CPU
  using TypeIn = typename ASpaceInfo::template AddressSpacePointerT<T>;
  using ConstTIn = typename ASpaceInfo::template ConstAddressSpacePointerT<T>;

 public:
  using Type = typename ASpaceInfo::template AddressSpacePointerT<AtomicT>;
  using ConstT = typename ASpaceInfo::template ConstAddressSpacePointerT<AtomicT>;
  using MemoryOrderT = ZIVC_CL_GLOBAL_NAMESPACE::memory_order;


  //! Convert to the built-in memory_order type
  static MemoryOrderT convert(const MemoryOrder order) noexcept
  {
    return static_cast<MemoryOrderT>(order);
  }
};

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <typename AddressSpacePointer, typename Type> inline
void Atomic::store(AddressSpacePointer object,
                   const Type desired,
                   const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  ZIVC_CL_GLOBAL_NAMESPACE::atomic_store_explicit(
      castPointer<typename Object::Type>(object),
      desired,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] object No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto Atomic::load(AddressSpacePointer object,
                  const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_load_explicit(
      castPointer<typename Object::Type>(object),
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in] desired No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::exchange(AddressSpacePointer object,
                      const Type desired,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_exchange_explicit(
      castPointer<typename Object::Type>(object),
      desired,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
bool Atomic::compareExchange(AddressSpacePointer object,
                             Type* expected,
                             const Type desired,
                             const MemoryOrder success,
                             const MemoryOrder failure) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_compare_exchange_strong_explicit(
      castPointer<typename Object::Type>(object),
      expected,
      desired,
      Object::convert(success),
      Object::convert(failure));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in,out] object No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto Atomic::fetchInc(AddressSpacePointer object,
                      const MemoryOrder order) noexcept
{
  using T = RemoveCvrefAddressT<decltype(object[0])>;
  return fetchAdd(object, T{1}, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in,out] object No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto Atomic::fetchDec(AddressSpacePointer object,
                      const MemoryOrder order) noexcept
{
  using T = RemoveCvrefAddressT<decltype(object[0])>;
  return fetchSub(object, T{1}, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchAdd(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_add_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchSub(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_sub_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchAnd(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_and_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchOr(AddressSpacePointer object,
                     const Type operand,
                     const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_or_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchXor(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_xor_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchMin(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_min_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type Atomic::fetchMax(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  using Object = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_max_explicit(
      castPointer<typename Object::Type>(object),
      operand,
      Object::convert(order));
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Function No description.
  \tparam Types No description.
  \param [in,out] p No description.
  \param [in] expression No description.
  \param [in] arguments No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Function, typename ...Types> inline
auto Atomic::perform(AddressSpacePointer p,
                     Function expression,
                     Types&&... arguments,
                     const MemoryOrder success,
                     const MemoryOrder failure) noexcept
{
  using ReturnT = RemoveCvrefT<decltype(expression(*p, arguments...))>;
  static_assert(isInteger32<ReturnT>(),
                "The return type of the expression isn't 32bit integer type.");
  // Perform an expression atomically
  auto old = *p;
  auto cmp = old;
  do {
    cmp = old;
    const ReturnT value = expression(cmp, forward<Types>(arguments)...);
    old = compareExchange(p, cmp, value, success, failure);
  } while (old != cmp);
  return old;
}


/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <typename AddressSpacePointer, typename Type> inline
void atomic_store(AddressSpacePointer object,
                  const Type desired,
                  const MemoryOrder order) noexcept
{
  Atomic::store(object, desired, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] object No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto atomic_load(AddressSpacePointer object,
                 const MemoryOrder order) noexcept
{
  return Atomic::load(object, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in] desired No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_exchange(AddressSpacePointer object,
                     const Type desired,
                     const MemoryOrder order) noexcept
{
  return Atomic::exchange(object, desired, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
bool atomic_compare_exchange(AddressSpacePointer object,
                             Type* expected,
                             const Type desired,
                             const MemoryOrder success,
                             const MemoryOrder failure) noexcept
{
  return Atomic::compareExchange(object, expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in,out] object No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto atomic_fetch_inc(AddressSpacePointer object,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchInc(object, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in,out] object No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto atomic_fetch_dec(AddressSpacePointer object,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchDec(object, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_add(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchAdd(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_sub(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchSub(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_and(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchAnd(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_or(AddressSpacePointer object,
                     const Type operand,
                     const MemoryOrder order) noexcept
{
  return Atomic::fetchOr(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_xor(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchXor(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_min(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchMin(object, operand, order);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
Type atomic_fetch_max(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order) noexcept
{
  return Atomic::fetchMax(object, operand, order);
}

} // namespace zivc

#endif /* ZIVC_CL_ATOMIC_INL_HPP */
