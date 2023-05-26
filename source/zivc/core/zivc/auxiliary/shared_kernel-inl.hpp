/*!
  \file shared_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHARED_KERNEL_INL_HPP
#define ZIVC_SHARED_KERNEL_INL_HPP

#include "shared_kernel.hpp"
// Standard C++ library
#include <compare>
#include <cstddef>
#include <memory>
#include <utility>
// Zivc
#include "kernel_common.hpp"
#include "../kernel_set.hpp"
#include "../zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
template <typename T> inline
SharedKernel<kDim, KSet, Args...>::SharedKernel(const std::shared_ptr<T>& ptr) noexcept :
    ptr_{static_pointer_cast<KernelT>(ptr)}
{
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
template <typename T> inline
SharedKernel<kDim, KSet, Args...>::SharedKernel(std::shared_ptr<T>&& ptr) noexcept :
    ptr_{static_pointer_cast<KernelT>(std::move(ptr))}
{
  //! \todo Resolve the move ownership. The pointer_cast just copy, not move
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
template <typename T> inline
auto SharedKernel<kDim, KSet, Args...>::operator=(const std::shared_ptr<T>& ptr) noexcept
    -> SharedKernel&
{
  ptr_ = static_pointer_cast<KernelT>(ptr);
  return *this;
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] ptr No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
template <typename T> inline
auto SharedKernel<kDim, KSet, Args...>::operator=(std::shared_ptr<T>&& ptr) noexcept
    -> SharedKernel&
{
  //! \todo Resolve the move ownership. The pointer_cast just copy, not move
  ptr_ = static_pointer_cast<KernelT>(std::move(ptr));
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
SharedKernel<kDim, KSet, Args...>::operator bool() const noexcept
{
  return static_cast<bool>(data());
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
SharedKernel<kDim, KSet, Args...>::operator SharedKernelCommon() const noexcept
{
  return static_cast<SharedKernelCommon>(data());
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto SharedKernel<kDim, KSet, Args...>::operator*() const noexcept -> Reference
{
  return *data();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto SharedKernel<kDim, KSet, Args...>::operator->() const noexcept -> Pointer
{
  return get();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto SharedKernel<kDim, KSet, Args...>::data() noexcept -> SharedPtr&
{
  return ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto SharedKernel<kDim, KSet, Args...>::data() const noexcept -> const SharedPtr&
{
  return ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto SharedKernel<kDim, KSet, Args...>::get() const noexcept -> Pointer
{
  return data().get();
}

/*!
  \details No detailed description

  \tparam KSet2 No description.
  \tparam Args2 No description.
  \param [in] other No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
template <DerivedKSet KSet2, typename ...Args2> inline
bool SharedKernel<kDim, KSet, Args...>::ownerBefore(const SharedKernel<kDim, KSet2, Args2...>& other) const noexcept
{
  return data().owner_before(other.data());
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
void SharedKernel<kDim, KSet, Args...>::reset() noexcept
{
  return data().reset();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
long SharedKernel<kDim, KSet, Args...>::useCount() const noexcept
{
  return data().use_count();
}

/*!
  \details No detailed description

  \tparam kDim No description.
  \tparam KSet1 No description.
  \tparam Args1 No description.
  \tparam KSet2 No description.
  \tparam Args2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet1, typename ...Args1, DerivedKSet KSet2, typename ...Args2> inline
bool operator==(const SharedKernel<kDim, KSet1, Args1...>& lhs,
                const SharedKernel<kDim, KSet2, Args2...>& rhs) noexcept
{
  return lhs.data() == rhs.data();
}

/*!
  \details No detailed description

  \tparam kDim No description.
  \tparam KSet1 No description.
  \tparam Args1 No description.
  \tparam KSet2 No description.
  \tparam Args2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet1, typename ...Args1, DerivedKSet KSet2, typename ...Args2> inline
std::strong_ordering operator<=>(const SharedKernel<kDim, KSet1, Args1...>& lhs,
                                 const SharedKernel<kDim, KSet2, Args2...>& rhs) noexcept
{
  return lhs.data() <=> rhs.data();
}

} /* namespace zivc */

#endif /* ZIVC_SHARED_KERNEL_INL_HPP */
