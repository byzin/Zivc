/*!
  \file array.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_ARRAY_HPP
#define ZIVC_CL_ARRAY_HPP

// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \brief Represent an array
  */
template <typename T, size_t kN>
class Array
{
 public:
  // Type aliases
  using Type = RemoveCvrefType<T>;
  using Reference = Type&;
  using ConstReference = const Type&;
  using Pointer = Type*;
  using ConstPointer = const Type*;
  using Iterator = Type*;
  using ConstIterator = const Type*;


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


  //! Return the reference by index
  Reference operator[](const size_t index) noexcept;

  //! Return the reference by index
  ConstReference operator[](const size_t index) const noexcept;


  //! Return the pointer to the array
  Pointer data() noexcept;

  //! Return the pointer to the array
  ConstPointer data() const noexcept;

  //! Fill the container with specified value
  void fill(ConstReference value) noexcept;

  //! Return the reference by index
  Reference get(const size_t index) noexcept;

  //! Return the reference by index
  ConstReference get(const size_t index) const noexcept;

  //! Return the largest element
  size_t getMaxIndex() const noexcept;

  //! Return the smallest element
  size_t getMinIndex() const noexcept;

  //! Check whether the array has the specified value
  bool hasValue(ConstReference value) const noexcept;

  //! Set value
  void set(const size_t index, ConstReference value) noexcept;

  //! Return the number of elements
  static constexpr size_t size() noexcept;

 private:
  Type data_[kN];
};

} // namespace zivc

#include "array-inl.hpp"

#endif // ZIVC_CL_ARRAY_HPP
