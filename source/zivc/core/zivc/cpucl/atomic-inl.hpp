/*!
  \file atomic-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_ATOMIC_INL_HPP
#define ZIVC_CPUCL_ATOMIC_INL_HPP

#include "atomic.hpp"
// Standard C++ library
#include <atomic>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
#include "zisc/concurrency/atomic.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

static_assert(sizeof(std::memory_order::relaxed) == 4);
static_assert(sizeof(std::memory_order::acquire) == 4);
static_assert(sizeof(std::memory_order::release) == 4);
static_assert(sizeof(std::memory_order::acq_rel) == 4);
static_assert(sizeof(std::memory_order::seq_cst) == 4);

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [object] object No description.
  \param [in] desired No description.
  */
template <zisc::Pointer Pointer> inline
void Atomic::store(Pointer object,
                   const std::remove_pointer_t<Pointer> desired) noexcept
{
  store_explicit(object, desired, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
void Atomic::store(AddressSpacePointer<kASpaceType, Type> object,
                   const Type desired) noexcept
{
  store(object.get(), desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <zisc::Pointer Pointer> inline
void Atomic::store_explicit(Pointer object,
                            const std::remove_pointer_t<Pointer> desired,
                            const memory_order order) noexcept
{
  zisc::atomic_store(object, desired, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
void Atomic::store_explicit(AddressSpacePointer<kASpaceType, Type> object,
                            const Type desired,
                            const memory_order order) noexcept
{
  store_explicit(object.get(), desired, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] object No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::load(const Pointer object) noexcept
{
  return load_explicit(object, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] object No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::load(const AddressSpacePointer<kASpaceType, Type>& object) noexcept
{
  return load(object.get());
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] object No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::load_explicit(const Pointer object,
                                                     const memory_order order) noexcept
{
  return zisc::atomic_load(object, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] object No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::load_explicit(const AddressSpacePointer<kASpaceType, Type>& object,
                           const memory_order order) noexcept
{
  return load_explicit(object.get(), order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::exchange(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired) noexcept
{
  return exchange_explicit(object, desired, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::exchange(AddressSpacePointer<kASpaceType, Type> object,
                      const Type desired) noexcept
{
  return exchange(object.get(), desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::exchange_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order order) noexcept
{
  return zisc::atomic_exchange(object, desired, order);
}

/*!
  \details No detailed description

  \tparam t No description.
  \param [in] x No description.
  \param [in,out] y No description.
  \param [out] z No description.
  \return No description
  \exception std::exception No description.

  \note No notation.
  \attention No attention.
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::exchange_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type desired,
                               const memory_order order) noexcept
{
  return exchange_explicit(object.get(), desired, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
bool Atomic::compare_exchange_strong(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired) noexcept
{
  return compare_exchange_strong_explicit(object, expected, desired, memory_order_seq_cst, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer, AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong(Pointer object,
                                     AddressSpacePointer<kASpaceType, Type> expected,
                                     const Type desired) noexcept
{
  return compare_exchange_strong(object, expected.get(), desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Pointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Pointer Pointer, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong(AddressSpacePointer<kASpaceType, Type> object,
                                     Pointer expected,
                                     const Type desired) noexcept
{
  return compare_exchange_strong(object.get(), expected, desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType1 No description.
  \tparam kASpaceType2 No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType1, AddressSpaceType kASpaceType2, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong(AddressSpacePointer<kASpaceType1, Type> object,
                                     AddressSpacePointer<kASpaceType2, Type> expected,
                                     const Type desired) noexcept
{
  return compare_exchange_strong(object.get(), expected.get(), desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
bool Atomic::compare_exchange_strong_explicit(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  using T = std::add_const_t<std::remove_pointer_t<Pointer>>;
  T old = zisc::atomic_compare_exchange(object, *expected, desired, success, failure);
  return !zisc::updateIfTrue(*expected != old, *expected, old);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <zisc::Pointer Pointer, AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong_explicit(
    Pointer object,
    AddressSpacePointer<kASpaceType, Type> expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return compare_exchange_strong_explicit(object, expected.get(), desired, success, failure);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Pointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Pointer Pointer, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong_explicit(
    AddressSpacePointer<kASpaceType, Type> object,
    Pointer expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return compare_exchange_strong_explicit(object.get(), expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam kASpaceType1 No description.
  \tparam kASpaceType2 No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <AddressSpaceType kASpaceType1, AddressSpaceType kASpaceType2, zisc::Arithmetic Type> inline
bool Atomic::compare_exchange_strong_explicit(
    AddressSpacePointer<kASpaceType1, Type> object,
    AddressSpacePointer<kASpaceType2, Type> expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return compare_exchange_strong_explicit(object.get(), expected.get(), desired, success, failure);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_add(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_add_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_add(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_add(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_add_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_add(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_add_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept
{
  return fetch_add_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_sub(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_sub_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_sub(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_sub(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_sub_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_sub(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_sub_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept
{
  return fetch_sub_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_and(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_and_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_and(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_and(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_and_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_and(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_and_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept
{
  return fetch_and_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_or(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_or_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_or(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return fetch_or(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_or_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_or(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_or_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return fetch_or_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_xor(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_xor_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_xor(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_xor(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_xor_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_xor(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description

  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_xor_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept
{
  return fetch_xor_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_min(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_min_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_min(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_min(object.get(), operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_min_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_min(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_min_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept
{
  return fetch_min_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_max(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return fetch_max_explicit(object, operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_max(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept
{
  return fetch_max_explicit(object.get(), operand, memory_order_seq_cst);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> Atomic::fetch_max_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return zisc::atomic_fetch_max(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type Atomic::fetch_max_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return fetch_max_explicit(object.get(), operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [out] object No description.
  \param [in] desired No description.
  */
template <zisc::Pointer Pointer> inline
void atomic_store(Pointer object,
                  const std::remove_pointer_t<Pointer> desired) noexcept
{
  Atomic::store(object, desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
void atomic_store(AddressSpacePointer<kASpaceType, Type> object,
                  const Type desired) noexcept
{
  Atomic::store(object, desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <zisc::Pointer Pointer> inline
void atomic_store_explicit(Pointer object,
                           const std::remove_pointer_t<Pointer> desired,
                           const memory_order order) noexcept
{
  Atomic::store_explicit(object, desired, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
void atomic_store_explicit(AddressSpacePointer<kASpaceType, Type> object,
                           const Type desired,
                           const memory_order order) noexcept
{
  Atomic::store_explicit(object, desired, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] object No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_load(const Pointer object) noexcept
{
  return Atomic::load(object);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] object No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_load(const AddressSpacePointer<kASpaceType, Type>& object) noexcept
{
  return Atomic::load(object);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in] object No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_load_explicit(const Pointer object,
                                                    const memory_order order) noexcept
{
  return Atomic::load_explicit(object, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] object No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_load_explicit(const AddressSpacePointer<kASpaceType, Type>& object,
                          const memory_order order) noexcept
{
  return Atomic::load_explicit(object, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_exchange(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired) noexcept
{
  return Atomic::exchange(object, desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_exchange(AddressSpacePointer<kASpaceType, Type> object,
                     const Type desired) noexcept
{
  return Atomic::exchange(object, desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_exchange_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order order) noexcept
{
  return Atomic::exchange_explicit(object, desired, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] desired No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_exchange_explicit(AddressSpacePointer<kASpaceType, Type> object,
                              const Type desired,
                              const memory_order order) noexcept
{
  return Atomic::exchange_explicit(object, desired, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
bool atomic_compare_exchange_strong(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired) noexcept
{
  return Atomic::compare_exchange_strong(object, expected, desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <zisc::Pointer Pointer, AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong(Pointer object,
                                    AddressSpacePointer<kASpaceType, Type> expected,
                                    const Type desired) noexcept
{
  return Atomic::compare_exchange_strong(object, expected, desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Pointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Pointer Pointer, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong(AddressSpacePointer<kASpaceType, Type> object,
                                    Pointer expected,
                                    const Type desired) noexcept
{
  return Atomic::compare_exchange_strong(object, expected, desired);
}

/*!
  \details No detailed description

  \tparam kASpaceType1 No description.
  \tparam kASpaceType2 No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \return No description
  */
template <AddressSpaceType kASpaceType1, AddressSpaceType kASpaceType2, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong(AddressSpacePointer<kASpaceType1, Type> object,
                                    AddressSpacePointer<kASpaceType2, Type> expected,
                                    const Type desired) noexcept
{
  return Atomic::compare_exchange_strong(object, expected, desired);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
bool atomic_compare_exchange_strong_explicit(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return Atomic::compare_exchange_strong_explicit(object, expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <zisc::Pointer Pointer, AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong_explicit(
    Pointer object,
    AddressSpacePointer<kASpaceType, Type> expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return Atomic::compare_exchange_strong_explicit(object, expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Pointer No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Pointer Pointer, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong_explicit(
    AddressSpacePointer<kASpaceType, Type> object,
    Pointer expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return Atomic::compare_exchange_strong_explicit(object, expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam kASpaceType1 No description.
  \tparam kASpaceType2 No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in,out] expected No description.
  \param [in] desired No description.
  \param [in] success No description.
  \param [in] failure No description.
  \return No description
  */
template <AddressSpaceType kASpaceType1, AddressSpaceType kASpaceType2, zisc::Arithmetic Type> inline
bool atomic_compare_exchange_strong_explicit(
    AddressSpacePointer<kASpaceType1, Type> object,
    AddressSpacePointer<kASpaceType2, Type> expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept
{
  return Atomic::compare_exchange_strong_explicit(object, expected, desired, success, failure);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_add(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_add(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_add(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_add(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_add_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_add_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_add_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_add_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_sub(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_sub(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_sub(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_sub(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_sub_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_sub_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_sub_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_sub_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_and(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_and(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_and(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_and(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_and_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_and_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_and_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_and_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_or(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_or(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_or(AddressSpacePointer<kASpaceType, Type> object,
                     const Type operand) noexcept
{
  return Atomic::fetch_or(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_or_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_or_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_or_explicit(AddressSpacePointer<kASpaceType, Type> object,
                              const Type operand,
                              const memory_order order) noexcept
{
  return Atomic::fetch_or_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_xor(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_xor(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_xor(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_xor(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_xor_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_xor_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_xor_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_xor_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_min(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_min(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_min(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_min(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_min_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_min_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_min_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_min_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_max(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept
{
  return Atomic::fetch_max(object, operand);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_max(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept
{
  return Atomic::fetch_max(object, operand);
}

/*!
  \details No detailed description

  \tparam Pointer No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <zisc::Pointer Pointer> inline
std::remove_pointer_t<Pointer> atomic_fetch_max_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept
{
  return Atomic::fetch_max_explicit(object, operand, order);
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in,out] object No description.
  \param [in] operand No description.
  \param [in] order No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type> inline
Type atomic_fetch_max_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept
{
  return Atomic::fetch_max_explicit(object, operand, order);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_ATOMIC_INL_HPP
