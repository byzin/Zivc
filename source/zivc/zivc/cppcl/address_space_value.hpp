/*!
  \file address_space_value.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ADDRESS_SPACE_VALUE_HPP
#define ZIVC_CL_ADDRESS_SPACE_VALUE_HPP

// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/non_copyable.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

// Concepts
//! Specity a type is implicitly convertible to another pointer of another type
template <typename From, typename To>
concept ConvertibleToPointer =
    zisc::ConvertibleTo<From,
                        std::add_pointer_t<std::remove_volatile_t<To>>>;
//! Specity pointer of a type is implicitly convertible to pointer of another type
template <typename From, typename To>
concept ConvertiblePointerToPointer =
    zisc::ConvertibleTo<std::add_pointer_t<std::remove_volatile_t<From>>,
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

// Forward declaration
template <AddressSpaceType, KernelParameter> class AddressSpacePointer;

/*!
  \brief No brief description

  No detailed description.

  \tparam kASpaceType No description.
  \tparam T No description.

  \note No notation.
  \attention No attention.
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
class AddressSpaceValue : private zisc::NonCopyable<AddressSpaceValue<kASpaceType, T>>
{
  //
  template <KernelParameter Type>
  using ASpaceValueT = AddressSpaceValue<kASpaceType, Type>;

 public:
  // Type aliases
  using Type = std::remove_volatile_t<T>;
  using PlainType = std::remove_const_t<Type>;
  using ConstType = std::add_const_t<PlainType>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using ASpacePointer = AddressSpacePointer<kASpaceType, T>;
  using ConstASpacePointer = AddressSpacePointer<kASpaceType, std::add_const_t<T>>;
  using ASpaceValueRef = std::add_lvalue_reference_t<AddressSpaceValue>;
  using ConstASpaceValue = std::add_const_t<AddressSpaceValue>;
  using ConstASpaceValueRef = std::add_lvalue_reference_t<ConstASpaceValue>;


  //! Initialize a data with nullptr
  AddressSpaceValue() noexcept;

  //! Copy a pointer
  template <ConvertiblePointerToPointer<T> ValueT>
  AddressSpaceValue(ASpaceValueT<ValueT>& other) noexcept;

  //! Move a pointer
  template <ConvertiblePointerToPointer<T> ValueT>
  AddressSpaceValue(ASpaceValueT<ValueT>&& other) noexcept;


  //! Dereference pointer to the managed object
  operator Reference() noexcept;

  //! Dereference pointer to the managed object
  operator ConstReference() const noexcept;

  //! Return the address space pointer to the managed object
  ASpacePointer operator&() noexcept;

  //! Return the address space pointer to the managed object
  ConstASpacePointer operator&() const noexcept;

  //! Replace the conents in the underlying pointer with the given value
  ASpaceValueRef operator=(ConstReference value) noexcept;


  //! Return the underlying pointer
  Pointer address() noexcept;

  //! Return the underlying pointer
  ConstPointer address() const noexcept;

  //! Return the pointer to the managed object
  ASpacePointer data() noexcept;

  //! Return the pointer to the managed object
  ConstASpacePointer data() const noexcept;

  //! Dereference pointer to the managed object
  Reference get() noexcept;

  //! Dereference pointer to the managed object
  ConstReference get() const noexcept;

 private:
  friend AddressSpacePointer<kASpaceType, T>;


  //! Initialize a data with the given pointer
  template <ConvertibleToPointer<T> PointerT>
  AddressSpaceValue(PointerT p) noexcept;


  Pointer data_ = nullptr;
};

} // namespace cl

} // namespace zivc

#include "address_space_value-inl.hpp"

#endif // ZIVC_CL_ADDRESS_SPACE_VALUE_HPP
