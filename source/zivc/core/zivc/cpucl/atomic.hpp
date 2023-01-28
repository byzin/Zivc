/*!
  \file atomic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_ATOMIC_HPP
#define ZIVC_CPUCL_ATOMIC_HPP

// Standard C++ library
#include <atomic>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

// atomic objects
using atomic_int = int32b;
using atomic_uint = uint32b;
using atomic_long = int64b;
using atomic_ulong = uint64b;
using atomic_float = float;
using atomic_double = double; 
using atomic_intptr_t = intptr_t;
using atomic_uintptr_t = uintptr_t;
using atomic_size_t = size_t;
using atomic_ptrdiff_t = ptrdiff_t;

// memory order
using std::memory_order;
inline constexpr memory_order memory_order_relaxed = std::memory_order::relaxed;
inline constexpr memory_order memory_order_acquire = std::memory_order::acquire;
inline constexpr memory_order memory_order_release = std::memory_order::release;
inline constexpr memory_order memory_order_acq_rel = std::memory_order::acq_rel;
inline constexpr memory_order memory_order_seq_cst = std::memory_order::seq_cst;

/*!
  \brief No brief description

  No detailed description.
  */
class Atomic
{
 public:
  //! Aomically replace the value pointed by object with the value of desired
  template <zisc::Pointer Pointer>
  static void store(Pointer object,
                    const std::remove_pointer_t<Pointer> desired) noexcept;

  //! Aomically replace the value pointed by object with the value of desired
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static void store(AddressSpacePointer<kASpaceType, Type> object,
                    const Type desired) noexcept;

  //! Aomically replace the value pointed by object with the value of desired
  template <zisc::Pointer Pointer>
  static void store_explicit(Pointer object,
                             const std::remove_pointer_t<Pointer> desired,
                             const memory_order order) noexcept;

  //! Aomically replace the value pointed by object with the value of desired
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static void store_explicit(AddressSpacePointer<kASpaceType, Type> object,
                             const Type desired,
                             const memory_order order) noexcept;

  //! Atomically returns the value pointed to by object.
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> load(const Pointer object) noexcept;

  //! Atomically returns the value pointed to by object.
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type load(const AddressSpacePointer<kASpaceType, Type>& object) noexcept;

  //! Atomically returns the value pointed to by object.
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> load_explicit(const Pointer object,
                                                      const memory_order order) noexcept;

  //! Atomically returns the value pointed to by object.
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type load_explicit(const AddressSpacePointer<kASpaceType, Type>& object,
                            const memory_order order) noexcept;

  //! Atomically replace the value pointed to by object with desired.
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> exchange(
      Pointer object,
      const std::remove_pointer_t<Pointer> desired) noexcept;

  //! Atomically replace the value pointed to by object with desired.
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type exchange(AddressSpacePointer<kASpaceType, Type> object,
                       const Type desired) noexcept;

  //! Atomically replace the value pointed to by object with desired.
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> exchange_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> desired,
      const memory_order order) noexcept;

  //! Atomically replace the value pointed to by object with desired.
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type exchange_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type desired,
                                const memory_order order) noexcept;

  //! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
  template <zisc::Pointer Pointer>
  static bool compare_exchange_strong(
      Pointer object,
      Pointer expected,
      const std::remove_pointer_t<Pointer> desired) noexcept;

  //! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static bool compare_exchange_strong(AddressSpacePointer<kASpaceType, Type> object,
                                      Type* expected,
                                      const Type desired) noexcept;

  //! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
  template <zisc::Pointer Pointer>
  static bool compare_exchange_strong_explicit(
      Pointer object,
      Pointer expected,
      const std::remove_pointer_t<Pointer> desired,
      const memory_order success,
      const memory_order failure) noexcept;

  //! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static bool compare_exchange_strong_explicit(
      AddressSpacePointer<kASpaceType, Type> object,
      Type* expected,
      const Type desired,
      const memory_order success,
      const memory_order failure) noexcept;

  //! Compute (old + operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_add(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Compute (old + operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_add(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Compute (old + operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_add_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Compute (old + operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_add_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;

  //! Compute (old - operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_sub(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Compute (old - operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_sub(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Compute (old - operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_sub_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Compute (old - operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_sub_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;

  //! Compute (old & operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_and(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Compute (old & operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_and(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Compute (old & operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_and_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Compute (old & operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_and_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;

  //! Compute (old | operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_or(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Compute (old | operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_or(AddressSpacePointer<kASpaceType, Type> object,
                       const Type operand) noexcept;

  //! Compute (old | operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_or_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Compute (old | operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_or_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                const Type operand,
                                const memory_order order) noexcept;

  //! Compute (old ^ operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_xor(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Compute (old ^ operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_xor(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Compute (old ^ operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_xor_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Compute (old ^ operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_xor_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;

  //! Perform min(old, operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_min(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Perform min(old, operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_min(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Perform min(old, operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_min_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Perform min(old, operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_min_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;

  //! Perform max(old, operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_max(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand) noexcept;

  //! Perform max(old, operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_max(AddressSpacePointer<kASpaceType, Type> object,
                        const Type operand) noexcept;

  //! Perform max(old, operand) and store result at location pointed by object, return old
  template <zisc::Pointer Pointer>
  static std::remove_pointer_t<Pointer> fetch_max_explicit(
      Pointer object,
      const std::remove_pointer_t<Pointer> operand,
      const memory_order order) noexcept;

  //! Perform max(old, operand) and store result at location pointed by object, return old
  template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
  static Type fetch_max_explicit(AddressSpacePointer<kASpaceType, Type> object,
                                 const Type operand,
                                 const memory_order order) noexcept;
};

//! Aomically replace the value pointed by object with the value of desired
template <zisc::Pointer Pointer>
void atomic_store(Pointer object,
                  const std::remove_pointer_t<Pointer> desired) noexcept;

//! Aomically replace the value pointed by object with the value of desired
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
void atomic_store(AddressSpacePointer<kASpaceType, Type> object,
                  const Type desired) noexcept;

//! Aomically replace the value pointed by object with the value of desired
template <zisc::Pointer Pointer>
void atomic_store_explicit(Pointer object,
                           const std::remove_pointer_t<Pointer> desired,
                           const memory_order order) noexcept;

//! Aomically replace the value pointed by object with the value of desired
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
void atomic_store_explicit(AddressSpacePointer<kASpaceType, Type> object,
                           const Type desired,
                           const memory_order order) noexcept;

//! Atomically returns the value pointed to by object.
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_load(const Pointer object) noexcept;

//! Atomically returns the value pointed to by object.
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_load(const AddressSpacePointer<kASpaceType, Type>& object) noexcept;

//! Atomically returns the value pointed to by object.
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_load_explicit(const Pointer object,
                                                    const memory_order order) noexcept;

//! Atomically returns the value pointed to by object.
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_load_explicit(const AddressSpacePointer<kASpaceType, Type>& object,
                          const memory_order order) noexcept;

//! Atomically replace the value pointed to by object with desired.
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_exchange(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired) noexcept;

//! Atomically replace the value pointed to by object with desired.
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_exchange(AddressSpacePointer<kASpaceType, Type> object,
                     const Type desired) noexcept;

//! Atomically replace the value pointed to by object with desired.
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_exchange_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order order) noexcept;

//! Atomically replace the value pointed to by object with desired.
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_exchange_explicit(AddressSpacePointer<kASpaceType, Type> object,
                              const Type desired,
                              const memory_order order) noexcept;

//! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
template <zisc::Pointer Pointer>
bool atomic_compare_exchange_strong(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired) noexcept;

//! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
bool atomic_compare_exchange_strong(AddressSpacePointer<kASpaceType, Type> object,
                                    Type* expected,
                                    const Type desired) noexcept;

//! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
template <zisc::Pointer Pointer>
bool atomic_compare_exchange_strong_explicit(
    Pointer object,
    Pointer expected,
    const std::remove_pointer_t<Pointer> desired,
    const memory_order success,
    const memory_order failure) noexcept;

//! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
bool atomic_compare_exchange_strong_explicit(
    AddressSpacePointer<kASpaceType, Type> object,
    Type* expected,
    const Type desired,
    const memory_order success,
    const memory_order failure) noexcept;

//! Compute (old + operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_add(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Compute (old + operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_add(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Compute (old + operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_add_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Compute (old + operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_add_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

//! Compute (old - operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_sub(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Compute (old - operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_sub(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Compute (old - operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_sub_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Compute (old - operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_sub_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

//! Compute (old & operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_and(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Compute (old & operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_and(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Compute (old & operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_and_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Compute (old & operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_and_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

//! Compute (old | operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_or(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Compute (old | operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_or(AddressSpacePointer<kASpaceType, Type> object,
                     const Type operand) noexcept;

//! Compute (old | operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_or_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Compute (old | operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_or_explicit(AddressSpacePointer<kASpaceType, Type> object,
                              const Type operand,
                              const memory_order order) noexcept;

//! Compute (old ^ operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_xor(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Compute (old ^ operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_xor(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Compute (old ^ operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_xor_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Compute (old ^ operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_xor_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

//! Perform min(old, operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_min(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Perform min(old, operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_min(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Perform min(old, operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_min_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Perform min(old, operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_min_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

//! Perform max(old, operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_max(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand) noexcept;

//! Perform max(old, operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_max(AddressSpacePointer<kASpaceType, Type> object,
                      const Type operand) noexcept;

//! Perform max(old, operand) and store result at location pointed by object, return old
template <zisc::Pointer Pointer>
std::remove_pointer_t<Pointer> atomic_fetch_max_explicit(
    Pointer object,
    const std::remove_pointer_t<Pointer> operand,
    const memory_order order) noexcept;

//! Perform max(old, operand) and store result at location pointed by object, return old
template <AddressSpaceType kASpaceType, zisc::Arithmetic Type>
Type atomic_fetch_max_explicit(AddressSpacePointer<kASpaceType, Type> object,
                               const Type operand,
                               const memory_order order) noexcept;

} // namespace zivc::cl

#include "atomic-inl.hpp"

#endif // ZIVC_CPUCL_ATOMIC_HPP
