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
template <typename T>
struct Atomic::AtomicType
{
  static_assert(0 <= sizeof(T), "The input type isn't supported for atomic operation.");
  using Type = void;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<int32b>
{
  using Type = atomic_int;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<int64b>
{
  using Type = atomic_long;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<uint32b>
{
  using Type = atomic_uint;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<uint64b>
{
  using Type = atomic_ulong;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<float>
{
  using Type = atomic_float;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Atomic::AtomicType<double>
{
  using Type = atomic_double;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename AddressSpacePointer>
class Atomic::AtomicObject
{
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The input object isn't pointer.");
  using T = RemoveCvT<typename ASpaceInfo::DataT>;
  using AtomicT = typename AtomicType<T>::Type;
  static_assert(sizeof(T) == sizeof(AtomicT), "The atomic type is invalid.");

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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  ZIVC_CL_GLOBAL_NAMESPACE::atomic_store_explicit(
      reinterp<typename ObjectT::Type>(object),
      desired,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_load_explicit(
      reinterp<typename ObjectT::Type>(object),
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_exchange_explicit(
      reinterp<typename ObjectT::Type>(object),
      desired,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_compare_exchange_strong_explicit(
      reinterp<typename ObjectT::Type>(object),
      expected,
      desired,
      ObjectT::convert(success),
      ObjectT::convert(failure));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_add_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_sub_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_and_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_or_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_xor_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_min_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
  using ObjectT = AtomicObject<AddressSpacePointer>;
  return ZIVC_CL_GLOBAL_NAMESPACE::atomic_fetch_max_explicit(
      reinterp<typename ObjectT::Type>(object),
      operand,
      ObjectT::convert(order));
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
