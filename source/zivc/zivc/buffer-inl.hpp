/*!
  \file buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
#include "zisc/utility.hpp"
#include "zisc/zisc_config.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/mapped_memory.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <typename T> inline
Buffer<T>::LaunchOptions::LaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
Buffer<T>::LaunchOptions::LaunchOptions(const std::size_t s) noexcept :
    size_{s}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] s No description.
  \param [in] queue_index No description.
  */
template <typename T> inline
Buffer<T>::LaunchOptions::LaunchOptions(const std::size_t s,
                                        const uint32b queue_index) noexcept :
    size_{s},
    queue_index_{queue_index}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::destOffset() const noexcept
{
  return dest_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::destOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(Type) * destOffset();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool Buffer<T>::LaunchOptions::isExternalSyncMode() const noexcept
{
  return is_external_sync_mode_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::string_view Buffer<T>::LaunchOptions::label() const noexcept
{
  std::string_view l{label_.data()};
  return l;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const std::array<float, 4>& Buffer<T>::LaunchOptions::labelColor() const noexcept
{
  return label_color_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
uint32b Buffer<T>::LaunchOptions::queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setDestOffset(const std::size_t offset) noexcept
{
  dest_offset_ = offset;
}

/*!
  \details No detailed description

  \param [in] is_active No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setExternalSyncMode(const bool is_active) noexcept
{
  is_external_sync_mode_ = is_active ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \param [in] launch_label No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setLabel(const std::string_view launch_label) noexcept
{
  std::strncpy(label_.data(), launch_label.data(), launch_label.size() + 1);
}

/*!
  \details No detailed description

  \param [in] label_color No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setLabelColor(const std::array<float, 4>& label_color) noexcept
{
  label_color_ = label_color;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setQueueIndex(const uint32b queue_index) noexcept
{
  queue_index_ = queue_index;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setSize(const std::size_t s) noexcept
{
  size_ = s;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::setSourceOffset(const std::size_t offset) noexcept
{
  source_offset_ = offset;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void Buffer<T>::LaunchOptions::initialize() noexcept
{
  setLabel("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::size() const noexcept
{
  return size_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::sizeInBytes() const noexcept
{
  const std::size_t s = sizeof(Type) * size();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::sourceOffset() const noexcept
{
  return source_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t Buffer<T>::LaunchOptions::sourceOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(Type) * sourceOffset();
  return s;
}

/*!
  \details No detailed description
  */
template <typename T> inline
Buffer<T>::Buffer(IdData&& id) noexcept : ZivcObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
void Buffer<T>::destroy() noexcept
{
  clear();
  destroyObject();
}

/*!
  \details No detailed description
  */
template <typename T> inline
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
template <typename T> inline
LaunchResult Buffer<T>::copyFrom(const Buffer& source,
                                 const LaunchOptions& launch_options)
{
  const LaunchOptions o = processOptions(launch_options);
  auto result = copyFromImpl(source, o);
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult Buffer<T>::fill(ConstReference value,
                             const LaunchOptions& launch_options)
{
  const LaunchOptions o = processOptions(launch_options);
  auto result = fillImpl(value, o);
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] buffer_usage No description.
  */
template <typename T> inline
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
template <typename T> inline
auto Buffer<T>::makeOptions() const noexcept -> LaunchOptions
{
  return LaunchOptions{};
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto Buffer<T>::mapMemory() -> MappedMemory<Type>
{
  using MappedMem = MappedMemory<Type>;
  typename MappedMem::ConstBufferP p = isHostVisible() ? this : nullptr;
  MappedMem memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto Buffer<T>::mapMemory() const noexcept -> MappedMemory<ConstType>
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
template <typename T> template <typename DstType> inline
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
template <typename T> template <typename DstType> inline
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
template <typename T> inline
BufferUsage Buffer<T>::usage() const noexcept
{
  return buffer_usage_;
}

/*!
  \details No detailed description

  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
auto Buffer<T>::processOptions(LaunchOptions launch_options) const noexcept -> LaunchOptions
{
  if (launch_options.size() == kBufferWholeSize)
    launch_options.setSize(size());
  return launch_options;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename Type> inline
LaunchResult copy(const Buffer<Type>& source,
                  Buffer<Type>* dest,
                  const BufferLaunchOptions<Type>& launch_options)
{
  auto result = dest->copyFrom(source, launch_options);
  return result;
}

} // namespace zivc

#endif // ZIVC_BUFFER_INL_HPP
