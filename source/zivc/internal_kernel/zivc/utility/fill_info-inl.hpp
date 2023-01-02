/*!
  \file fill_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_FILL_INFO_INL_HPP
#define ZIVC_CL_FILL_INFO_INL_HPP

#include "fill_info.hpp"
// Zivc
#include "zivc/cl/types.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr size_t FillInfo::batchSize() noexcept
{
  const size_t batch_size = 16;
  return batch_size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillInfo::elementOffset() const noexcept
{
  return static_cast<size_t>(element_offset_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillInfo::elementSize() const noexcept
{
  return static_cast<size_t>(element_size_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr size_t FillInfo::dataCapacityInBytes() noexcept
{
  static_assert(sizeof(FillInfo) == kInfoSize, "The size of FillInfo isn't correct.");
  return kDataCapacityInBytes;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillInfo::dataSize() const noexcept
{
  return static_cast<size_t>(data_size_);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
inline
void FillInfo::setElementOffset(const size_t offset) noexcept
{
  element_offset_ = static_cast<uint32b>(offset);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void FillInfo::setElementSize(const size_t size) noexcept
{
  element_size_ = static_cast<uint32b>(size);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void FillInfo::setDataSize(const size_t size) noexcept
{
  data_size_ = static_cast<uint32b>(size);
}

} // namespace zivc

#endif // ZIVC_CL_FILL_INFO_INL_HPP
