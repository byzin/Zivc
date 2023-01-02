/*!
  \file shared_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHARED_BUFFER_HPP
#define ZIVC_SHARED_BUFFER_HPP

// Standard C++ library
#include <compare>
#include <memory>
#include <type_traits>
// Zivc
#include "buffer_common.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

/*!
  \brief No brief description

  No detailed description.

  \tparam KernelArg No description.
  */
template <KernelArg Type>
class SharedBuffer
{
  using BufferT = Buffer<Type>;

 public:
  // Type aliases
  using ElementT = std::remove_extent_t<BufferT>;
  using ConstElementT = std::add_const_t<ElementT>;
  using Pointer = std::add_pointer_t<ElementT>;
  using ConstPointer = std::add_const_t<std::add_pointer_t<ConstElementT>>;
  using Reference = std::add_lvalue_reference_t<ElementT>;
  using ConstReference = std::add_lvalue_reference_t<ConstElementT>;
  using SharedPtr = std::shared_ptr<BufferT>;
  using WeakPtr = std::weak_ptr<BufferT>;

  // STL compatible
  using element_type = ElementT;
  using weak_type = WeakPtr;


  //! Create a null ptr
  SharedBuffer() noexcept = default;

  //! Copy a shared ptr
  template <typename T>
  SharedBuffer(const std::shared_ptr<T>& ptr) noexcept;

  //! Move a shared ptr
  template <typename T>
  SharedBuffer(std::shared_ptr<T>&& ptr) noexcept;

  //! Destroy the underlying ptr
  ~SharedBuffer() noexcept = default;


  //! Assign the shared pointer
  template <typename T>
  SharedBuffer& operator=(const std::shared_ptr<T>& ptr) noexcept;

  //! Assign the shared pointer
  template <typename T>
  SharedBuffer& operator=(std::shared_ptr<T>&& ptr) noexcept;

  //! Check if the stored pointer is not null
  explicit operator bool() const noexcept;

  //! Apply cast to the stored pointer
  operator SharedBufferCommon() const noexcept;

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

  //! Provide owner-base ordering of SharedBuffer
  template <KernelArg Type2>
  bool ownerBefore(const SharedBuffer<Type2>& other) const noexcept;

  //! Replace the managed object
  void reset() noexcept;

  //! Return the number of SharedBuffer objects referring to the same managed object
  long useCount() const noexcept;

 private:
  SharedPtr ptr_;
};

// Type aliases
template <KernelArg Type>
using WeakBuffer = typename SharedBuffer<Type>::WeakPtr;

//!
template <KernelArg Type1, KernelArg Type2>
bool operator==(const SharedBuffer<Type1>& lhs,
                const SharedBuffer<Type2>& rhs) noexcept;

//!
template <KernelArg Type1, KernelArg Type2>
std::strong_ordering operator<=>(const SharedBuffer<Type1>& lhs,
                                 const SharedBuffer<Type2>& rhs) noexcept;

} /* namespace zivc */

#include "shared_buffer-inl.hpp"

#endif /* ZIVC_SHARED_BUFFER_HPP */
