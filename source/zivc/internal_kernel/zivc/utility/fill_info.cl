/*!
  \file fill_info.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_INFO_CL
#define ZIVC_FILL_INFO_CL

// Zivc
#include "zivc/cl/types.cl"

namespace zivc {

// Forward declaration
class FillInfo;

//! Check if the two fill data are equal
bool operator==(const FillInfo& lhs, const FillInfo& rhs) noexcept;

//! Check if the two fill data are not equal
bool operator!=(const FillInfo& lhs, const FillInfo& rhs) noexcept;

/*!
  \brief No brief description

  No detailed description.
  */
class FillInfo
{
 public:
  //! Return the size of the buffer to be filled
  size_t bufferSize() const noexcept;

  //! Return the capacity of the data
  static constexpr size_t dataCapacityInBytes() noexcept;

  //! Return the data size
  size_t dataSize() const noexcept;

  //! Return the underlying header
  uint4 header() const noexcept;

  //! Return the id offset
  size_t idOffset() const noexcept;

  //! Set the size of the buffer to be filled
  void setBufferSize(const size_t size) noexcept;

  //! Set the data size
  void setDataSize(const size_t size) noexcept;

  //! Set the id offset
  void setIdOffset(const size_t offset) noexcept;

 private:
  using uint32b = zivc::uint32b;


  static constexpr size_t kInfoSize = 16;
  static constexpr size_t kDataCapacityInBytes = 256 - kInfoSize;


  uint32b buffer_size_;
  uint32b id_offset_;
  uint32b data_size_;
  uint32b pad_;
};

 /*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator==(const FillInfo& lhs, const FillInfo& rhs) noexcept
{
  const bool result = (lhs.bufferSize() == rhs.bufferSize()) &&
                      (lhs.idOffset() == rhs.idOffset()) &&
                      (lhs.dataSize() == rhs.dataSize());
  return result;
}

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator!=(const FillInfo& lhs, const FillInfo& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

} // namespace zivc

#include "fill_info-inl.cl"

#endif // ZIVC_FILL_INFO_CL
