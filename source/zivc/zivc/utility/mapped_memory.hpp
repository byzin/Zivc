/*!
  \file mapped_memory.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_MAPPED_MEMORY_HPP
#define ZIVC_MAPPED_MEMORY_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class BufferCommon;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <zisc::TriviallyCopyable T>
class MappedMemory : private zisc::NonCopyable<MappedMemory<T>>
{
 public:
  using Type = std::remove_volatile_t<T>;
  using ConstType = std::add_const_t<Type>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using Iterator = Pointer;
  using ConstIterator = ConstPointer;
  // For STL compatibility
  using value_type = Type;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;
  using iterator = Iterator;
  using const_iterator = ConstIterator;


  //! Create an empty memory
  MappedMemory() noexcept;

  //! Create a mapped memory
  MappedMemory(const BufferCommon* buffer);

  //! Move data
  MappedMemory(MappedMemory&& other) noexcept;

  //! Destruct the managed memory
  ~MappedMemory() noexcept;


  // For STL compatibility
  //! Return an iterator to the first element of the container
  Iterator begin() noexcept;

  //! Return an iterator to the first element of the container
  ConstIterator begin() const noexcept;

  //! Return an iterator to the first element of the container
  ConstIterator cbegin() const noexcept;

  //! Return an iterator following the last element of the container
  Iterator end() noexcept;

  //! Return an iterator following the last element of the container
  ConstIterator end() const noexcept;

  //! Return an iterator following the last element of the container
  ConstIterator cend() const noexcept;


  //! Move data
  MappedMemory& operator=(MappedMemory&& other) noexcept;

  //! Check whether this owns a memory
  explicit operator bool() const noexcept;

  //! Return the reference of the element by index
  Reference operator[](const std::size_t index) noexcept;

  //! Return the reference of the element by index
  ConstReference operator[](const std::size_t index) const noexcept;


  //! Return the pointer to the managed memory
  Pointer data() noexcept;

  //! Return the pointer to the managed memory
  ConstPointer data() const noexcept;

  //! Return the reference of the element by index
  Reference get(const std::size_t index) noexcept;

  //! Return the reference of the element by index
  ConstReference get(const std::size_t index) const noexcept;

  //! Check whether this owns a memory
  bool hasMemory() const noexcept;

  //! Set a value to the managed memory at index
  void set(const std::size_t index, ConstReference value) noexcept;

  //! Return the size of the memory array
  std::size_t size() const noexcept;

  //! Unmap the managed memory 
  void unmap() noexcept;

 private:
  //! Return the internal buffer
  const BufferCommon* internalBuffer() const noexcept;


  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");


  Pointer data_ = nullptr;
  const BufferCommon* buffer_ = nullptr;
};

} // namespace zivc

#include "mapped_memory-inl.hpp"

#endif // ZIVC_MAPPED_MEMORY_HPP
