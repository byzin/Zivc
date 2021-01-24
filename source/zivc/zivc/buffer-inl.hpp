/*!
  \file buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_INL_HPP
#define ZIVC_BUFFER_INL_HPP

#include "buffer.hpp"
// Standard C++ library
#include <cstring>
#include <memory>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
#include "zisc/zisc_config.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer_common.hpp"
#include "zivc_config.hpp"
#include "utility/buffer_launch_options.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/mapped_memory.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
Buffer<T>::Buffer(IdData&& id) noexcept : BufferCommon(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t Buffer<T>::capacity() const noexcept
{
  const std::size_t c = getCapacity<Type>();
  return c;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void Buffer<T>::clear() noexcept
{
  destroyData();
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult Buffer<T>::copyFrom(const Buffer& source,
                                 const LaunchOptions& launch_options)
{
  auto result = zivc::copy(source, this, launch_options);
  return result;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void Buffer<T>::destroy() noexcept
{
  clear();
  destroyObject();
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult Buffer<T>::fill(ConstReference value,
                             const LaunchOptions& launch_options)
{
  auto result = zivc::fill(value, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] buffer_usage No description.
  */
template <zisc::TriviallyCopyable T> inline
void Buffer<T>::initialize(ZivcObject::SharedPtr&& parent,
                           WeakPtr&& own,
                           const BufferUsage buffer_usage)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  setUsage(buffer_usage);
  setTypeSize(sizeof(Type));
  initData();

  ZivcObject::setNameIfEmpty("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::makeMappedMemory() -> MappedMemory<Type>
{
  const BufferCommon* p = isHostVisible() ? this : nullptr;
  MappedMemory<Type> memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::makeMappedMemory() const -> MappedMemory<ConstType>
{
  const BufferCommon* p = isHostVisible() ? this : nullptr;
  MappedMemory<ConstType> memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::makeOptions() const noexcept -> LaunchOptions
{
  LaunchOptions options{size()};
  return options;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable NewType> inline
Buffer<NewType>* Buffer<T>::reinterp() noexcept
{
  auto new_buffer = zisc::reinterp<Buffer<NewType>*>(this);
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable NewType> inline
const Buffer<NewType>* Buffer<T>::reinterp() const noexcept
{
  auto new_buffer = zisc::reinterp<const Buffer<NewType>*>(this);
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t Buffer<T>::size() const noexcept
{
  const std::size_t s = getSize<Type>();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T>
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::copyFromDerived(const Buffer<T>& source,
                                        const LaunchOptions& launch_options)
{
  using DerivedBufferP = std::add_pointer_t<Derived<T>>;
  auto dest = zisc::cast<DerivedBufferP>(this);
  auto result = dest->copyFromImpl(source, launch_options);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T>
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::fillDerived(ConstReference value,
                                    const LaunchOptions& launch_options)
{
  using DerivedBufferP = std::add_pointer_t<Derived<T>>;
  auto dest = zisc::cast<DerivedBufferP>(this);
  auto result = dest->fillImpl(value, launch_options);
  return result;
}

} // namespace zivc

#endif // ZIVC_BUFFER_INL_HPP
