/*!
  \file address_space_value-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ADDRESS_SPACE_VALUE_INL_HPP
#define ZIVC_CL_ADDRESS_SPACE_VALUE_INL_HPP

#include "address_space_value.hpp"
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "address_space_pointer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

/*!
  \details No detailed description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
AddressSpaceValue<kASpaceType, T>::AddressSpaceValue() noexcept
{
}

/*!
  \details No detailed description

  \tparam PointerT No description.
  \param [in,out] p No description.
  */
template <AddressSpaceType kASpaceType, KernelParameter T>
template <zisc::ConvertibleTo<std::add_pointer_t<std::remove_volatile_t<T>>> PointerT>
inline
AddressSpaceValue<kASpaceType, T>::AddressSpaceValue(PointerT p) noexcept :
    data_{zisc::cast<Pointer>(p)}
{
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
AddressSpaceValue<kASpaceType, T>::operator Reference() noexcept
{
  return get();
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
AddressSpaceValue<kASpaceType, T>::operator ConstReference() const noexcept
{
  return get();
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::operator&() noexcept -> ASpacePointer
{
  return data();
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::operator&() const noexcept -> ConstASpacePointer
{
  return data();
}

/*!
  \details No detailed description

  \param [in] value No description.
  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::operator=(ConstReference value) noexcept
    -> ASpaceValueRef 
{
  auto& ref = get();
  ref = value;
  return *this;
}
/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::data() noexcept -> ASpacePointer
{
  ASpacePointer p{data_};
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::data() const noexcept -> ConstASpacePointer
{
  ConstASpacePointer p{data_};
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::get() noexcept -> Reference
{
  return *data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <AddressSpaceType kASpaceType, KernelParameter T> inline
auto AddressSpaceValue<kASpaceType, T>::get() const noexcept -> ConstReference
{
  return *data_;
}

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_ADDRESS_SPACE_VALUE_INL_HPP