/*!
  \file address_space_pointer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_ADDRESS_SPACE_POINTER_HPP
#define ZIVC_CPUCL_ADDRESS_SPACE_POINTER_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

// Concepts
//! Specity a type is implicitly convertible to another pointer of another type
template <typename From, typename To>
concept ConvertibleToPointer =
    std::convertible_to<From,
                        std::add_pointer_t<std::remove_volatile_t<To>>>;
//! Specity pointer of a type is implicitly convertible to pointer of another type
template <typename From, typename To>
concept ConvertiblePointerToPointer =
    std::convertible_to<std::add_pointer_t<std::remove_volatile_t<From>>,
                        std::add_pointer_t<std::remove_volatile_t<To>>>;

/*!
  \brief Represent address space type

  No detailed description.
  */
enum class AddressSpaceType: uint32b
{
  kGlobal = 0,
  kLocal,
  kConstant,
  kPrivate
};

/*!
  \brief Represent a address space pointer

  No detailed description.

  \tparam kASpaceType No description.
  \tparam T No description.
  */
template <AddressSpaceType kASpaceType, KernelArg T>
class AddressSpacePointer
{
  //
  template <KernelArg Type>
  using ASpacePointerT = AddressSpacePointer<kASpaceType, Type>;

 public:
  // Type aliases
  using Type = std::remove_volatile_t<T>;
  using ConstType = std::add_const_t<Type>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference<ConstType>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using ASpacePointerRef = std::add_lvalue_reference_t<AddressSpacePointer>;


  //! Initialize a pointer by nullptr
  AddressSpacePointer() noexcept;

  //! Initialize a pointer with the given value
  template <ConvertibleToPointer<T> PointerT>
  AddressSpacePointer(PointerT p) noexcept;

  //! Initialize a pointer with the given value 
  template <ConvertiblePointerToPointer<T> ValueT>
  AddressSpacePointer(ASpacePointerT<ValueT>& p) noexcept;

  //! Move a pointer
  template <ConvertiblePointerToPointer<T> ValueT>
  AddressSpacePointer(ASpacePointerT<ValueT>&& p) noexcept;


  //! Assign a pointer
  template <ConvertibleToPointer<T> PointerT>
  ASpacePointerRef operator=(PointerT data) noexcept;

  //! Copy a pointer
  template <ConvertiblePointerToPointer<T> ValueT>
  ASpacePointerRef operator=(ASpacePointerT<ValueT>& p) noexcept;

  //! Move a pointer
  template <ConvertiblePointerToPointer<T> ValueT>
  ASpacePointerRef operator=(ASpacePointerT<ValueT>&& p) noexcept;


  //! Check whether this owns an object
  explicit operator bool() const noexcept;

  //! Dereference pointer to the managed object by the given index
  template <zisc::Integer Integer>
  Reference operator[](const Integer index) noexcept;

  //! Dereference pointer to the managed object by the given index
  template <zisc::Integer Integer>
  ConstReference operator[](const Integer index) const noexcept;

  //! Dereference pointer to the managed object
  Reference operator*() noexcept;

  //! Dereference pointer to the managed object
  ConstReference operator*() const noexcept;

  //! Dereference pointer to the managed object
  Pointer operator->() noexcept;

  //! Dereference pointer to the managed object
  ConstPointer operator->() const noexcept;

  //! Get a pointer to n-th element and assign it
  template <zisc::Integer Integer>
  ASpacePointerRef operator+=(const Integer n) noexcept;

  //! Get a pointer to -n-th element and assign it
  template <zisc::Integer Integer>
  ASpacePointerRef operator-=(const Integer n) noexcept;

  //! Return a pointer to the managed object
  ASpacePointerRef operator+() noexcept;

  //! Pre-increment a pointer address
  ASpacePointerRef operator++() noexcept;

  //! Pre-decrement a pointer address
  ASpacePointerRef operator--() noexcept;

  //! Post-increment a pointer address
  AddressSpacePointer operator++(int) noexcept;

  //! Post-decrement a pointer address
  AddressSpacePointer operator--(int) noexcept;


  //! Return the underlying address space type
  static constexpr AddressSpaceType type() noexcept;

  //! Return a pointer to the managed object
  Pointer get() noexcept;

  //! Return a pointer to the managed object
  ConstPointer get() const noexcept;

 private:
  Pointer data_ = nullptr;
};

//! Compute a pointer to n-th element
template <AddressSpaceType kASpaceType, typename Type, zisc::Integer Integer>
AddressSpacePointer<kASpaceType, Type> operator+(
    AddressSpacePointer<kASpaceType, Type> p,
    const Integer n) noexcept;

//! Compute a pointer to n-th element
template <AddressSpaceType kASpaceType, typename Type, zisc::Integer Integer>
AddressSpacePointer<kASpaceType, Type> operator+(
    const Integer n,
    AddressSpacePointer<kASpaceType, Type> p) noexcept;

//! Compute a pointer to -n-th element
template <AddressSpaceType kASpaceType, typename Type, zisc::Integer Integer>
AddressSpacePointer<kASpaceType, Type> operator-(
    AddressSpacePointer<kASpaceType, Type> p,
    const Integer n) noexcept;

//! Compute the distance between lhs and rhs
template <AddressSpaceType kASpaceType, typename Type>
ptrdiff_t operator-(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator==(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator==(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const std::nullptr_t rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator==(
    const std::nullptr_t lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator!=(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator!=(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const std::nullptr_t rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator!=(
    const std::nullptr_t lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is less than rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator<(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is less than or equal rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator<=(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is greater than rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator>(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

//! Return true if lhs is greater than or equal rhs, false otherwise
template <AddressSpaceType kASpaceType, typename Type>
bool operator>=(
    const AddressSpacePointer<kASpaceType, Type>& lhs,
    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept;

} // namespace cl

} // namespace zivc

#include "address_space_pointer-inl.hpp"

#endif // ZIVC_CPUCL_ADDRESS_SPACE_POINTER_HPP
