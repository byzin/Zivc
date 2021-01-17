/*!
  \file mapped_memory-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_MAPPED_MEMORY_INL_HPP
#define ZIVC_MAPPED_MEMORY_INL_HPP

#include "mapped_memory.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T>::MappedMemory() noexcept
{
}

/*!
  \details No detailed description

  \param [in] buffer No description.
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T>::MappedMemory(ConstBufferP buffer) :
    data_{buffer ? buffer->mappedMemory() : nullptr},
    buffer_{buffer}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T>::MappedMemory(MappedMemory&& other) noexcept
{
  zisc::swap(data_, other.data_);
  zisc::swap(buffer_, other.buffer_);
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T>::~MappedMemory() noexcept
{
  unmap();
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::begin() noexcept -> Iterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::begin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::cbegin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::end() noexcept -> Iterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::end() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::cend() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::operator=(MappedMemory&& other) noexcept -> MappedMemory&
{
  zisc::swap(data_, other.data_);
  zisc::swap(buffer_, other.buffer_);
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T>::operator bool() const noexcept
{
  const bool result = hasMemory();
  return result;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::operator[](const std::size_t index) noexcept
    -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::operator[](const std::size_t index) const noexcept
    -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::data() noexcept -> Pointer
{
  return data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::data() const noexcept -> ConstPointer
{
  return data_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::get(const std::size_t index) noexcept -> Reference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  Reference value = d[index];
  return value;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto MappedMemory<T>::get(const std::size_t index) const noexcept -> ConstReference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  ConstReference value = d[index];
  return value;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool MappedMemory<T>::hasMemory() const noexcept
{
  const bool result = data() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \param [in] value No description.
  */
template <zisc::TriviallyCopyable T> inline
void MappedMemory<T>::set(const std::size_t index, ConstReference value) noexcept
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  d[index] = value;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t MappedMemory<T>::size() const noexcept
{
  const std::size_t s = hasMemory() ? buffer_->size() : 0;
  return s;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void MappedMemory<T>::unmap() noexcept
{
  if (hasMemory())
    buffer_->unmapMemory();
  data_ = nullptr;
  buffer_ = nullptr;
}

} // namespace zivc

#endif // ZIVC_MAPPED_MEMORY_INL_HPP
