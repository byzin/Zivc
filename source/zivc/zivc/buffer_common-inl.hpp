/*!
  \file buffer_common-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_COMMON_INL_HPP
#define ZIVC_BUFFER_COMMON_INL_HPP

#include "buffer_common.hpp"
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/mapped_memory.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam T No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferCommon::getCapacity() const noexcept
{
  const std::size_t c = calcSize<T>(capacityInBytes());
  return c;
}

/*!
  \details No detailed description

  \tparam T No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferCommon::getSize() const noexcept
{
  const std::size_t s = calcSize<T>(sizeInBytes());
  return s;
}

/*!
  \details No detailed description

  \tparam T No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
MappedMemory<T> BufferCommon::makeMappedMemory() const
{
  const BufferCommon* p = isHostVisible() ? this : nullptr;
  MappedMemory<T> memory{p};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t BufferCommon::typeSize() const noexcept
{
  return zisc::cast<std::size_t>(type_size_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
BufferUsage BufferCommon::usage() const noexcept
{
  return buffer_usage_;
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] s No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t BufferCommon::calcSize(const std::size_t s) const noexcept
{
  const std::size_t n = s / sizeof(T);
  return n;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
inline
void BufferCommon::setTypeSize(const std::size_t s) noexcept
{
  type_size_ = zisc::cast<uint32b>(s);
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
void BufferCommon::setUsage(const BufferUsage flag) noexcept
{
  buffer_usage_ = flag;
}

} // namespace zivc

#endif // ZIVC_BUFFER_COMMON_INL_HPP
