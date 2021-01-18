/*!
  \file buffer_launch_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_LAUNCH_OPTIONS_INL_HPP
#define ZIVC_BUFFER_LAUNCH_OPTIONS_INL_HPP

#include "buffer_launch_options.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
BufferLaunchOptions<T>::BufferLaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <zisc::TriviallyCopyable T> inline
BufferLaunchOptions<T>::BufferLaunchOptions(const std::size_t s) noexcept :
    size_{s}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] s No description.
  \param [in] queue_index No description.
  */
template <zisc::TriviallyCopyable T> inline
BufferLaunchOptions<T>::BufferLaunchOptions(const std::size_t s,
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
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::destOffset() const noexcept
{
  return dest_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::destOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * destOffset();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool BufferLaunchOptions<T>::isExternalSyncMode() const noexcept
{
  return is_external_sync_mode_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::string_view BufferLaunchOptions<T>::label() const noexcept
{
  std::string_view l{label_.data()};
  return l;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const std::array<float, 4>& BufferLaunchOptions<T>::labelColor() const noexcept
{
  return label_color_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
uint32b BufferLaunchOptions<T>::queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setDestOffset(const std::size_t offset) noexcept
{
  dest_offset_ = offset;
}

/*!
  \details No detailed description

  \param [in] is_active No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setExternalSyncMode(const bool is_active) noexcept
{
  is_external_sync_mode_ = is_active ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \param [in] launch_label No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setLabel(const std::string_view launch_label) noexcept
{
  std::strncpy(label_.data(), launch_label.data(), launch_label.size() + 1);
}

/*!
  \details No detailed description

  \param [in] label_color No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setLabelColor(const std::array<float, 4>& label_color) noexcept
{
  label_color_ = label_color;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setQueueIndex(const uint32b queue_index) noexcept
{
  queue_index_ = queue_index;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setSize(const std::size_t s) noexcept
{
  size_ = s;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::setSourceOffset(const std::size_t offset) noexcept
{
  source_offset_ = offset;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void BufferLaunchOptions<T>::initialize() noexcept
{
  setLabel("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::size() const noexcept
{
  return size_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::sizeInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * size();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::sourceOffset() const noexcept
{
  return source_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferLaunchOptions<T>::sourceOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * sourceOffset();
  return s;
}

} // namespace zivc

#endif // ZIVC_BUFFER_LAUNCH_OPTIONS_INL_HPP
