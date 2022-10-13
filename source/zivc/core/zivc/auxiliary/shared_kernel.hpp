/*!
  \file shared_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHARED_KERNEL_HPP
#define ZIVC_SHARED_KERNEL_HPP

// Standard C++ library
#include <compare>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "kernel_common.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/internal/kernel_arg_parser.hpp"

namespace zivc {

// Forward declaration
template <typename, typename...> class Kernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
class SharedKernel
{
  using KernelT = typename internal::KernelArgParser<Args...>::
                      template KernelT<Kernel, kDim, KSet>;

 public:
  // Type aliases
  using ElementT = std::remove_extent_t<KernelT>;
  using ConstElementT = std::add_const_t<ElementT>;
  using Pointer = std::add_pointer_t<ElementT>;
  using ConstPointer = std::add_const_t<std::add_pointer_t<ConstElementT>>;
  using Reference = std::add_lvalue_reference_t<ElementT>;
  using ConstReference = std::add_lvalue_reference_t<ConstElementT>;
  using SharedPtr = std::shared_ptr<KernelT>;
  using WeakPtr = std::weak_ptr<KernelT>;

  // STL compatible
  using element_type = ElementT;
  using weak_type = WeakPtr;


  //! Create a null ptr
  SharedKernel() noexcept = default;

  //! Copy a shared ptr
  template <typename T>
  SharedKernel(const std::shared_ptr<T>& ptr) noexcept;

  //! Move a shared ptr
  template <typename T>
  SharedKernel(std::shared_ptr<T>&& ptr) noexcept;

  //! Destroy the underlying ptr
  ~SharedKernel() noexcept = default;


  //! Assign the shared pointer
  template <typename T>
  SharedKernel& operator=(const std::shared_ptr<T>& ptr) noexcept;

  //! Assign the shared pointer
  template <typename T>
  SharedKernel& operator=(std::shared_ptr<T>&& ptr) noexcept;

  //! Check if the stored pointer is not null
  explicit operator bool() const noexcept;

  //! Apply cast to the stored pointer
  operator SharedKernelCommon() const noexcept;

  //! Dereference the stored pointer
  Reference operator*() const noexcept;

  //! Dereference the stored pointer
  Pointer operator->() const noexcept;


  //! Return the underlying shared pointer
  SharedPtr& data() noexcept;

  //! Return the underlying shared pointer
  const SharedPtr& data() const noexcept;

  //! Return the stored pointer
  Pointer get() const noexcept;

  //! Provide owner-base ordering of SharedKernel
  template <DerivedKSet KSet2, typename ...Args2>
  bool ownerBefore(const SharedKernel<kDim, KSet2, Args2...>& other) const noexcept;

  //! Replace the managed object
  void reset() noexcept;

  //! Return the number of SharedKernel objects referring to the same managed object
  long useCount() const noexcept;

 private:
  SharedPtr ptr_;
};

// Type aliases
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
using WeakKernel = typename SharedKernel<kDim, KSet, Args...>::WeakPtr;

//!
template <std::size_t kDim, DerivedKSet KSet1, typename ...Args1, DerivedKSet KSet2, typename ...Args2>
bool operator==(const SharedKernel<kDim, KSet1, Args1...>& lhs,
                const SharedKernel<kDim, KSet2, Args2...>& rhs) noexcept;

//!
template <std::size_t kDim, DerivedKSet KSet1, typename ...Args1, DerivedKSet KSet2, typename ...Args2>
std::strong_ordering operator<=>(const SharedKernel<kDim, KSet1, Args1...>& lhs,
                                 const SharedKernel<kDim, KSet2, Args2...>& rhs) noexcept;

} /* namespace zivc */

#include "shared_kernel-inl.hpp"

#endif /* ZIVC_SHARED_KERNEL_HPP */
