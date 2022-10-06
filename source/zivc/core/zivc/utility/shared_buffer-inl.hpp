/*!
  \file shared_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHARED_BUFFER_INL_HPP
#define ZIVC_SHARED_BUFFER_INL_HPP

#include "shared_buffer.hpp"
// Standard C++ library
#include <compare>
#include <memory>
#include <utility>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <KernelArg Type> inline
SharedBuffer<Type>::SharedBuffer(const SharedPtr& ptr) noexcept : ptr_{ptr}
{
}

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <KernelArg Type> inline
SharedBuffer<Type>::SharedBuffer(SharedPtr&& ptr) noexcept : ptr_{std::move(ptr)}
{
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  */
template <KernelArg Type> template <typename T> inline
SharedBuffer<Type>::SharedBuffer(const std::shared_ptr<T>& ptr) noexcept : ptr_{ptr}
{
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  */
template <KernelArg Type> template <typename T> inline
SharedBuffer<Type>::SharedBuffer(std::shared_ptr<T>&& ptr) noexcept : ptr_{std::move(ptr)}
{
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
SharedBuffer<Type>::operator bool() const noexcept
{
  return static_cast<bool>(data());
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
auto SharedBuffer<Type>::operator*() const noexcept -> Reference
{
  return *data();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
auto SharedBuffer<Type>::operator->() const noexcept -> Pointer
{
  return get();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
auto SharedBuffer<Type>::data() noexcept -> SharedPtr&
{
  return ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
auto SharedBuffer<Type>::data() const noexcept -> const SharedPtr&
{
  return ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
auto SharedBuffer<Type>::get() const noexcept -> Pointer
{
  return data().get();
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] other No description.
  \return No description
  */
template <KernelArg Type> template <typename T> inline
bool SharedBuffer<Type>::ownerBefore(const SharedBuffer<T>& other) const noexcept
{
  return data().owner_before(other.data());
}

/*!
  \details No detailed description
  */
template <KernelArg Type> inline
void SharedBuffer<Type>::reset() noexcept
{
  return data().reset();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
long SharedBuffer<Type>::useCount() const noexcept
{
  return data().use_count();
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <KernelArg Type1, KernelArg Type2> inline
bool operator==(const SharedBuffer<Type1>& lhs,
                const SharedBuffer<Type2>& rhs) noexcept
{
  return lhs.data() == rhs.data();
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <KernelArg Type1, KernelArg Type2> inline
std::strong_ordering operator<=>(const SharedBuffer<Type1>& lhs,
                                 const SharedBuffer<Type2>& rhs) noexcept
{
  return lhs.data() <=> rhs.data();
}

} /* namespace zivc */

#endif /* ZIVC_SHARED_BUFFER_INL_HPP */
