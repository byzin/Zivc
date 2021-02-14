/*!
  \file address_space_pointer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ADDRESS_SPACE_POINTER_INL_HPP
#define ZIVC_CL_ADDRESS_SPACE_POINTER_INL_HPP

#include "address_space_pointer.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

/*!
  \details No detailed description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
AddressSpacePointer<kASpaceType, T>::AddressSpacePointer() noexcept
{
}

/*!
  \details No detailed description

  \tparam PointerT No description.
  \param [in,out] p No description.
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertibleToPointer<T> PointerT> inline
AddressSpacePointer<kASpaceType, T>::AddressSpacePointer(PointerT p) noexcept :
    data_{zisc::cast<Pointer>(p)}
{
}

/*!
  \details No detailed description

  \param [in,out] p No description.
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertiblePointerToPointer<T> ValueT> inline
AddressSpacePointer<kASpaceType, T>::AddressSpacePointer(ASpacePointerT<ValueT>& p) noexcept :
    data_{p.get()}
{
}

/*!
  \details No detailed description

  \param [in,out] p No description.
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertiblePointerToPointer<T> ValueT> inline
AddressSpacePointer<kASpaceType, T>::AddressSpacePointer(ASpacePointerT<ValueT>&& p) noexcept :
    data_{p.get()}
{
}

/*!
  \details No detailed description

  \tparam PointerT No description.
  \param [in,out] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertibleToPointer<T> PointerT> inline
auto AddressSpacePointer<kASpaceType, T>::operator=(PointerT p) noexcept
    -> ASpacePointerRef
{
  data_ = zisc::cast<Pointer>(p);
  return *this;
}

/*!
  \details No detailed description

  \param [in,out] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertiblePointerToPointer<T> ValueT> inline
auto AddressSpacePointer<kASpaceType, T>::operator=(ASpacePointerT<ValueT>& p) noexcept -> ASpacePointerRef
{
  data_ = p.get();
  return *this;
}

/*!
  \details No detailed description

  \param [in,out] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <ConvertiblePointerToPointer<T> ValueT> inline
auto AddressSpacePointer<kASpaceType, T>::operator=(ASpacePointerT<ValueT>&& p) noexcept -> ASpacePointerRef
{
  data_ = p.get();
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
AddressSpacePointer<kASpaceType, T>::operator bool() const noexcept
{
  const bool result = get() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <zisc::Integer Integer> inline
auto AddressSpacePointer<kASpaceType, T>::operator[](const Integer index) noexcept -> Reference
{
  auto p = get() + zisc::cast<ptrdiff_t>(index);
  return *p;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <zisc::Integer Integer> inline
auto AddressSpacePointer<kASpaceType, T>::operator[](const Integer index) const noexcept -> ConstReference
{
  auto p = get() + zisc::cast<ptrdiff_t>(index);
  return *p;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator*() noexcept -> Reference
{
  auto p = get();
  return *p;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator*() const noexcept -> ConstReference 
{
  auto p = get();
  return *p;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator->() noexcept -> Pointer
{
  auto data = get();
  return data;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator->() const noexcept -> ConstPointer
{
  auto data = get();
  return data;
}

/*!
  \details No detailed description

  \param [in] n No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <zisc::Integer Integer> inline
auto AddressSpacePointer<kASpaceType, T>::operator+=(const Integer n) noexcept
    -> ASpacePointerRef
{
  auto data = get() + zisc::cast<ptrdiff_t>(n);
  data_ = data;
  return *this;
}

/*!
  \details No detailed description

  \param [in] n No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <zisc::Integer Integer> inline
auto AddressSpacePointer<kASpaceType, T>::operator-=(const Integer n) noexcept
    -> ASpacePointerRef
{
  auto data = get() - zisc::cast<ptrdiff_t>(n);
  data_ = data;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator+() noexcept -> ASpacePointerRef
{
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator++() noexcept
    -> ASpacePointerRef
{
  auto data = get() + 1;
  data_ = data;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator--() noexcept
    -> ASpacePointerRef
{
  auto data = get() - 1;
  data_ = data;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator++(int) noexcept
    -> AddressSpacePointer
{
  auto data = get();
  data_ = data + 1;
  return AddressSpacePointer{data};
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::operator--(int) noexcept
    -> AddressSpacePointer
{
  auto data = get();
  data_ = data - 1;
  return AddressSpacePointer{data};
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::get() noexcept -> Pointer 
{
  return data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpacePointer<kASpaceType, T>::get() const noexcept -> ConstPointer 
{
  return data_;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] p No description.
  \param [in] n No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type, zisc::Integer Integer>
inline
AddressSpacePointer<kASpaceType, Type> operator+(
    AddressSpacePointer<kASpaceType, Type> p,
    const Integer n) noexcept
{
  AddressSpacePointer<kASpaceType, Type> result = p;
  result += n;
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] n No description.
  \param [in] p No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type, zisc::Integer Integer>
inline
AddressSpacePointer<kASpaceType, Type> operator+(
    const Integer n,
    AddressSpacePointer<kASpaceType, Type> p) noexcept
{
  auto result = p + n;
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] p No description.
  \param [in] n No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type, zisc::Integer Integer>
inline
AddressSpacePointer<kASpaceType, Type> operator-(
    AddressSpacePointer<kASpaceType, Type> p,
    const Integer n) noexcept
{
  AddressSpacePointer<kASpaceType, Type> result = p;
  result -= n;
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
ptrdiff_t operator-(const AddressSpacePointer<kASpaceType, Type>& lhs,
                    const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const ptrdiff_t diff = zisc::cast<ptrdiff_t>(lhs.get() - rhs.get());
  return diff;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator==(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() == rhs.get();
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator==(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const std::nullptr_t rhs) noexcept
{
  const bool result = lhs.get() == rhs;
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator==(const std::nullptr_t lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = lhs == rhs.get();
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator!=(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator!=(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const std::nullptr_t rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator!=(const std::nullptr_t lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator<(const AddressSpacePointer<kASpaceType, Type>& lhs,
               const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() < rhs.get();
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator<=(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() <= rhs.get();
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator>(const AddressSpacePointer<kASpaceType, Type>& lhs,
               const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = rhs < lhs;
  return result;
}

/*!
  \details No detailed description

  \tparam kASpaceType No description.
  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter Type> inline
bool operator>=(const AddressSpacePointer<kASpaceType, Type>& lhs,
                const AddressSpacePointer<kASpaceType, Type>& rhs) noexcept
{
  const bool result = rhs <= lhs;
  return result;
}

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_ADDRESS_SPACE_POINTER_INL_HPP
