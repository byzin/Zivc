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
#include "zisc/error.hpp"
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
#include "utility/reinterp_buffer.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <KernelArg T> inline
Buffer<T>::Buffer(IdData&& id) noexcept : BufferCommon(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t Buffer<T>::capacity() const noexcept
{
  const std::size_t c = getCapacity<Type>();
  return c;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
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
template <KernelArg T> inline
LaunchResult Buffer<T>::copyFrom(const Buffer& source,
                                 const LaunchOptions& launch_options)
{
  auto result = zivc::copy(source, this, launch_options);
  return result;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
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
template <KernelArg T> inline
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
template <KernelArg T> inline
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
template <KernelArg T> inline
auto Buffer<T>::mapMemory() -> MappedMemory<Type>
{
  return makeMappedMemory<Type>();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto Buffer<T>::mapMemory() const -> MappedMemory<ConstType>
{
  return makeMappedMemory<ConstType>();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto Buffer<T>::makeOptions() const noexcept -> LaunchOptions
{
  LaunchOptions options{size()};
  return options;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> template <KernelArg NewType> inline
auto Buffer<T>::reinterp() noexcept -> ReinterpBufferT<NewType>
{
  auto id_copy = id().copy();
  auto own = zisc::cast<BufferCommon*>(getOwn());
  ReinterpBufferT<NewType> new_buffer{own, std::move(id_copy)};
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> template <KernelArg NewType> inline
auto Buffer<T>::reinterp() const noexcept -> ConstReinterpBufferT<NewType>
{
  auto id_copy = id().copy();
  auto own = zisc::cast<const BufferCommon*>(getOwn());
  ConstReinterpBufferT<NewType> new_buffer{own, std::move(id_copy)};
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t Buffer<T>::size() const noexcept
{
  const std::size_t s = getSize<Type>();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T>
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::copyFromDerived(const Buffer<T>& source,
                                        const LaunchOptions& launch_options)
{
  //! \todo Remove me
  ZISC_ASSERT(type() == source.type(), "Type mismatch found.");
  //! \todo Remove me
  ZISC_ASSERT(getParent() == source.getParent(), "Type mismatch found.");
  auto result = Derived<T>::copyFromImpl(source, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T>
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::fillDerived(ConstReference value,
                                    const LaunchOptions& launch_options)
{
  auto result = Derived<T>::fillImpl(value, this, launch_options);
  return result;
}

} // namespace zivc

#endif // ZIVC_BUFFER_INL_HPP
