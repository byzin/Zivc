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
Buffer<T>::Buffer(IdData&& id) noexcept : ZivcObject(std::move(id))
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
  std::size_t c = capacityImpl();
  c = correctSize(c);
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
  auto result = zivc::fill(this, value, launch_options);
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
  buffer_usage_ = buffer_usage;
  initData();

  ZivcObject::setNameIfEmpty("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::makeOptions() const noexcept -> LaunchOptions
{
  return LaunchOptions{};
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::mapMemory() -> MappedMemory<Type>
{
  using MappedMem = MappedMemory<Type>;
  typename MappedMem::ConstBufferP p = isHostVisible()
      ? this
      : nullptr;
  MappedMem memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::mapMemory() const -> MappedMemory<ConstType>
{
  using MappedMem = MappedMemory<ConstType>;
  typename MappedMem::ConstBufferP p = isHostVisible()
      ? this->reinterp<ConstType>()
      : nullptr;
  MappedMem memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable DstType> inline
Buffer<DstType>* Buffer<T>::reinterp() noexcept
{
  using DstBuffer = Buffer<DstType>;
  DstBuffer* dst = zisc::reinterp<DstBuffer*>(this);
  return dst;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable DstType> inline
const Buffer<DstType>* Buffer<T>::reinterp() const noexcept
{
  using DstBuffer = Buffer<DstType>;
  const DstBuffer* dst = zisc::reinterp<const DstBuffer*>(this);
  return dst;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t Buffer<T>::size() const noexcept
{
  std::size_t s = sizeImpl();
  s = correctSize(s);
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
BufferUsage Buffer<T>::usage() const noexcept
{
  return buffer_usage_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t Buffer<T>::correctSize(const std::size_t s) const noexcept
{
  const std::size_t t_orig = zisc::cast<std::size_t>(type_size_);
  constexpr std::size_t t_new = sizeof(T);
  const std::size_t s_new = (t_orig * s) / t_new;
  return s_new;
}

/*!
  \details No detailed description

  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto Buffer<T>::processOptions(LaunchOptions launch_options) const noexcept
    -> LaunchOptions
{
  if (launch_options.size() == kBufferWholeSize) {
    const std::size_t s = size() - launch_options.destOffset();
    launch_options.setSize(s);
  }
  return launch_options;
}

/*!
  \details No detailed description

  \tparam Derived No description.
  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> 
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::copyFromDerived(const Buffer& source,
                                        const LaunchOptions& launch_options)
{
  auto dest = zisc::cast<Derived<T>*>(this);
  const LaunchOptions o = processOptions(launch_options);
  auto result = dest->copyFromImpl(source, o);
  return result;
}

/*!
  \details No detailed description

  \tparam Derived No description.
  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> 
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::fillDerived(ConstReference value,
                                    const LaunchOptions& launch_options)
{
  auto dest = zisc::cast<Derived<T>*>(this);
  const LaunchOptions o = processOptions(launch_options);
  auto result = dest->fillImpl(value, o);
  return result;
}

} // namespace zivc

#endif // ZIVC_BUFFER_INL_HPP
