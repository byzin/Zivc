/*!
  \file buffer_launch_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <KernelArg T> inline
BufferLaunchOptions<T>::BufferLaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <KernelArg T> inline
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
template <KernelArg T> inline
BufferLaunchOptions<T>::BufferLaunchOptions(const std::size_t s,
                                            const uint32b queue_index) noexcept :
    LaunchOptions(queue_index),
    size_{s}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::destOffset() const noexcept
{
  return dest_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::destOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * destOffset();
  return s;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <KernelArg T> inline
void BufferLaunchOptions<T>::setDestOffset(const std::size_t offset) noexcept
{
  dest_offset_ = offset;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <KernelArg T> inline
void BufferLaunchOptions<T>::setSize(const std::size_t s) noexcept
{
  size_ = s;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
template <KernelArg T> inline
void BufferLaunchOptions<T>::setSourceOffset(const std::size_t offset) noexcept
{
  source_offset_ = offset;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void BufferLaunchOptions<T>::initialize() noexcept
{
  setLabel("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::size() const noexcept
{
  return size_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::sizeInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * size();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::sourceOffset() const noexcept
{
  return source_offset_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t BufferLaunchOptions<T>::sourceOffsetInBytes() const noexcept
{
  const std::size_t s = sizeof(T) * sourceOffset();
  return s;
}

} // namespace zivc

#endif // ZIVC_BUFFER_LAUNCH_OPTIONS_INL_HPP
