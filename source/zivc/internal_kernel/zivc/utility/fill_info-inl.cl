/*!
  \file fill_info-inl.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_INFO_INL_CL
#define ZIVC_FILL_INFO_INL_CL

#include "fill_info.cl"
// Zivc
#include "zivc/cl/types.cl"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillInfo::bufferSize() const noexcept
{
  return static_cast<size_t>(buffer_size_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr size_t FillInfo::dataCapacityInBytes() noexcept
{
  static_assert(sizeof(FillInfo) == kInfoSize, "The size of FillInfo is wrong.");
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

  \return No description
  */
inline
size_t FillInfo::idOffset() const noexcept
{
  return static_cast<size_t>(id_offset_);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void FillInfo::setBufferSize(const size_t size) noexcept
{
  buffer_size_ = static_cast<uint32b>(size);
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

/*!
  \details No detailed description

  \param [in] offset No description.
  */
inline
void FillInfo::setIdOffset(const size_t offset) noexcept
{
  id_offset_ = static_cast<uint32b>(offset);
}

} // namespace zivc

#endif // ZIVC_FILL_INFO_INL_CL
