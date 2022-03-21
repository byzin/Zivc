/*!
  \file buffer_common-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_COMMON_INL_HPP
#define ZIVC_BUFFER_COMMON_INL_HPP

#include "buffer_common.hpp"
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "error.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t BufferCommon::capacity() const noexcept
{
  const std::size_t cap = calcNumOfElements(capacityInBytes());
  return cap;
}

/*!
  \details No detailed description

  \return No description
  */
inline
BufferFlag BufferCommon::flag() const noexcept
{
  return buffer_flag_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool BufferCommon::isHostReadable() const noexcept
{
  const bool result = isHostCoherent() && isHostCached();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool BufferCommon::isHostWritable() const noexcept
{
  const bool result = isHostVisible();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t BufferCommon::size() const noexcept
{
  const std::size_t s = calcNumOfElements(sizeInBytes());
  return s;
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

  \param [in] s No description.
  \return No description
  */
inline
std::size_t BufferCommon::calcNumOfElements(const std::size_t size) const noexcept
{
  [[maybe_unused]] const std::size_t remainder = size % typeSizeInBytes();
  ZIVC_ASSERT(remainder == 0, "The size cannot be divided by the type size.");
  const std::size_t n = size / typeSizeInBytes();
  return n;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
void BufferCommon::setFlag(const BufferFlag flag) noexcept
{
  buffer_flag_ = flag;
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
