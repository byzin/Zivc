/*!
  \file atomic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ATOMIC_HPP
#define ZIVC_CL_ATOMIC_HPP

#include "types.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
enum class MemoryOrder : int32b
{
  kRelaxed = static_cast<int32b>(ZIVC_CL_GLOBAL_NAMESPACE::memory_order_relaxed),
  kAcquire = static_cast<int32b>(ZIVC_CL_GLOBAL_NAMESPACE::memory_order_acquire),
  kRelease = static_cast<int32b>(ZIVC_CL_GLOBAL_NAMESPACE::memory_order_release),
  kAcqRel = static_cast<int32b>(ZIVC_CL_GLOBAL_NAMESPACE::memory_order_acq_rel),
  kSeqCst = static_cast<int32b>(ZIVC_CL_GLOBAL_NAMESPACE::memory_order_seq_cst)
};

inline constexpr MemoryOrder kMemoryOrderRelaxed = MemoryOrder::kRelaxed;
inline constexpr MemoryOrder kMemoryOrderAcquire = MemoryOrder::kAcquire;
inline constexpr MemoryOrder kMemoryOrderRelease = MemoryOrder::kRelease;
inline constexpr MemoryOrder kMemoryOrderAcqRel = MemoryOrder::kAcqRel;
inline constexpr MemoryOrder kMemoryOrderSeqCst = MemoryOrder::kSeqCst;

/*!
  \brief No brief description

  No detailed description.
  */
class Atomic
{
 public:
  //! Aomically replace the value pointed by object with the value of desired
  template <typename AddressSpacePointer, typename Type>
  static void store(AddressSpacePointer object,
                    const Type desired,
                    const MemoryOrder order = kMemoryOrderRelease) noexcept;

  //! Atomically returns the value pointed to by object.
  template <typename AddressSpacePointer>
  static auto load(AddressSpacePointer object,
                   const MemoryOrder order = kMemoryOrderAcquire) noexcept;

  //! Atomically replace the value pointed to by object with desired.
  template <typename AddressSpacePointer, typename Type>
  static Type exchange(AddressSpacePointer object,
                       const Type desired,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
  template <typename AddressSpacePointer1, typename AddressSpacePointer2, typename Type>
  static bool compareAndExchange(AddressSpacePointer1 object,
                                 AddressSpacePointer2 expected,
                                 const Type desired,
                                 const MemoryOrder success = kMemoryOrderAcqRel,
                                 const MemoryOrder failure = kMemoryOrderAcquire) noexcept;

  //! Compute (old + 1) and store result at location pointed by object, return old
  template <typename AddressSpacePointer>
  static auto fetchInc(AddressSpacePointer object,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old - 1) and store result at location pointed by object, return old
  template <typename AddressSpacePointer>
  static auto fetchDec(AddressSpacePointer object,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old + operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchAdd(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old - operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchSub(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old & operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchAnd(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old | operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchOr(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute (old ^ operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchXor(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute min(old, operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchMin(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Compute max(old, operand) and store result at location pointed by object, return old
  template <typename AddressSpacePointer, typename Type>
  static Type fetchMax(AddressSpacePointer object,
                       const Type operand,
                       const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

  //! Perform an expression atomically
  template <typename AddressSpacePointer, typename Function, typename ...Types>
  static auto perform(AddressSpacePointer p,
                      const MemoryOrder order,
                      Function expression,
                      Types... arguments) noexcept;

 private:
  //! Atomic object type that corresponds to the given type
  template <typename T>
  struct AtomicType;

  //! Get atomic type by the given type
  template <typename AddressSpacePointer>
  class AtomicObject;


  //!
  static constexpr MemoryOrder getLoadOrder(const MemoryOrder order) noexcept;
};

// CL style function aliases

//! Aomically replace the value pointed by object with the value of desired
template <typename AddressSpacePointer, typename Type>
void atomic_store(AddressSpacePointer object,
                  const Type desired,
                  const MemoryOrder order = kMemoryOrderRelease) noexcept;

//! Atomically returns the value pointed to by object.
template <typename AddressSpacePointer>
auto atomic_load(AddressSpacePointer object,
                 const MemoryOrder order = kMemoryOrderAcquire) noexcept;

//! Atomically replace the value pointed to by object with desired.
template <typename AddressSpacePointer, typename Type>
Type atomic_exchange(AddressSpacePointer object,
                     const Type desired,
                     const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Atomically, compares the value pointed to by object for equality with that in expected, and if true, replaces the value pointed to by object with desired
template <typename AddressSpacePointer1, typename AddressSpacePointer2, typename Type>
bool atomic_compare_exchange(AddressSpacePointer1 object,
                             AddressSpacePointer2 expected,
                             const Type desired,
                             const MemoryOrder success = kMemoryOrderAcqRel,
                             const MemoryOrder failure = kMemoryOrderAcquire) noexcept;

//! Compute (old + 1) and store result at location pointed by object, return old
template <typename AddressSpacePointer>
auto atomic_fetch_inc(AddressSpacePointer object,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old - 1) and store result at location pointed by object, return old
template <typename AddressSpacePointer>
auto atomic_fetch_dec(AddressSpacePointer object,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old + operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_add(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old - operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_sub(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old & operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_and(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old | operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_or(AddressSpacePointer object,
                     const Type operand,
                     const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute (old ^ operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_xor(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute min(old, operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_min(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

//! Compute max(old, operand) and store result at location pointed by object, return old
template <typename AddressSpacePointer, typename Type>
Type atomic_fetch_max(AddressSpacePointer object,
                      const Type operand,
                      const MemoryOrder order = kMemoryOrderAcqRel) noexcept;

} // namespace zivc

#include "atomic-inl.hpp"

#endif /* ZIVC_CL_ATOMIC_HPP */
