/*!
  \file fill_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_KERNEL_CL
#define ZIVC_FILL_KERNEL_CL

#include "zivc/cl/bit.cl"
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"

namespace zivc {

// Forward declaration
class FillData;

//! Check if the two fill data are equal
bool operator==(const FillData& lhs, const FillData& rhs) noexcept;

//! Check if the two fill data are not equal
bool operator!=(const FillData& lhs, const FillData& rhs) noexcept;


/*!
  \brief No brief description

  No detailed description.
  */
class FillData
{
 public:
  // Header

  //! Return the size of the stored value
  size_t dataSize() const noexcept;

  //! Return the underlying header
  uint4 header() const noexcept;

  //! Return the offset
  size_t offset() const noexcept;

  //! Set the offset
  void setOffset(const size_t o) noexcept;

  //! Set the size of work items
  void setSize(const size_t s) noexcept;

  //! Return the size of work items
  size_t size() const noexcept;

  // Storage

  //! Return the capacity of the underlying storage
  static constexpr size_t capacity() noexcept;

  //! Set the given value into the data
  void setValue(zivc::ConstPrivatePtr<zivc::uint8b> data,
                const size_t data_size) noexcept;

  //! Return the storage at the given index
  uint4 storage(const size_t index) const noexcept;

  //! Return the size of the underlying storage array
  static constexpr size_t storageSize() noexcept;

  //! Return the value
  uint8b value(const size_t index) const noexcept;

 private:
  static_assert(sizeof(uint8b) == 1);
  static_assert(sizeof(uchar4) == 4);


  //!
  uint8b getC(const size_t index) const noexcept;

  //!
  uchar4 getC4(const size_t index) const noexcept;

  //!
  void setC(const uint8b value, const size_t index) noexcept;

  //!
  void setC4(const uchar4 value, const size_t index) noexcept;


  static constexpr uint kSize = 256;
  static constexpr uint kHeaderSize = sizeof(uint4);
  static_assert(kHeaderSize < kSize);
  static constexpr uint kStorageSize = kSize - kHeaderSize;
  static_assert((kStorageSize % sizeof(uint4)) == 0);
  static constexpr uint kStorageArrayLength = kStorageSize / sizeof(uint4);


  uint4 header_;
  uint4 storage_[kStorageArrayLength];
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator==(const FillData& lhs, const FillData& rhs) noexcept
{
  bool result = false;
  // Header
  {
    const int4 r = lhs.header() == rhs.header();
    result = r.x && r.y && r.z && r.w;
  }
  // Storage
  for (size_t i = 0; result && (i < FillData::storageSize()); ++i) {
    const int4 r = lhs.storage(i) == rhs.storage(i);
    result = r.x && r.y && r.z && r.w;
  }
  return result;
}

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
inline
bool operator!=(const FillData& lhs, const FillData& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillData::dataSize() const noexcept
{
  return static_cast<size_t>(header_.z);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint4 FillData::header() const noexcept
{
  return header_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillData::offset() const noexcept
{
  return static_cast<size_t>(header_.x);
}

/*!
  \details No detailed description

  \param [in] o No description.
  */
inline
void FillData::setOffset(const size_t o) noexcept
{
  header_.x = static_cast<zivc::uint32b>(o);
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
inline
void FillData::setSize(const size_t s) noexcept
{
  header_.y = static_cast<zivc::uint32b>(s);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t FillData::size() const noexcept
{
  return static_cast<size_t>(header_.y);
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr size_t FillData::capacity() noexcept
{
  return static_cast<size_t>(kStorageSize);
}

/*!
  \details No detailed description

  \param [in] data No description.
  */
inline
void FillData::setValue(const zivc::ConstPrivatePtr<uint8b> data,
                        const size_t data_size) noexcept
{
  static_assert(sizeof(FillData) == FillData::kSize);
  header_.z = static_cast<uint32b>(data_size);
  for (size_t index = 0; index < data_size; ++index)
    setC(data[index], index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
uint4 FillData::storage(const size_t index) const noexcept
{
  return storage_[index];
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr size_t FillData::storageSize() noexcept
{
  return static_cast<size_t>(kStorageArrayLength);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
uint8b FillData::value(const size_t index) const noexcept
{
  const uint8b v = getC(index);
  return v;
}

inline
uint8b FillData::getC(const size_t index) const noexcept
{
  constexpr size_t c4_size = sizeof(uchar4) / sizeof(uint8b);
  const size_t c4_index = index / c4_size;
  const uchar4 data = getC4(c4_index);
  const uint8b value = (index & 0b10)
      ? (index & 0b01) ? data.w : data.z
      : (index & 0b01) ? data.y : data.x;
  return value;
}

inline
uchar4 FillData::getC4(const size_t index) const noexcept
{
  constexpr size_t storage_size = sizeof(uint4) / sizeof(uchar4);
  const size_t storage_index = index / storage_size;
  const uint4 data = storage_[storage_index];
  const uint32b v = (index & 0b10)
      ? (index & 0b01) ? data.w : data.z
      : (index & 0b01) ? data.y : data.x;
  const uchar4 value = zivc::castBit<uchar4>(v);
  return value;
}

inline
void FillData::setC(const uint8b value, const size_t index) noexcept
{
  constexpr size_t c4_size = sizeof(uchar4) / sizeof(uint8b);
  const size_t c4_index = index / c4_size;
  uchar4 data = getC4(c4_index);
  {
    const size_t element_index = index % c4_size;
    if (element_index & 0b10) {
      if (element_index & 0b01)
        data.w = value;
      else
        data.z = value;
    }
    else {
      if (element_index & 0b01)
        data.y = value;
      else
        data.x = value;
    }
  }
  setC4(data, c4_index);
}

inline
void FillData::setC4(const uchar4 value, const size_t index) noexcept
{
  constexpr size_t storage_size = sizeof(uint4) / sizeof(uchar4);
  const size_t storage_index = index / storage_size;
  uint4 data = storage_[storage_index];
  {
    const uint32b v = zivc::castBit<uint32b>(value);
    const size_t element_index = index % storage_size;
    if (element_index & 0b10) {
      if (element_index & 0b01)
        data.w = v;
      else
        data.z = v;
    }
    else {
      if (element_index & 0b01)
        data.y = v;
      else
        data.x = v;
    }
  }
  storage_[storage_index] = data;
}

} // namespace zivc

/*!
  \details No detailed description

  \param [out] buffer No description.
  \param [in] data No description.
  */
__kernel void Zivc_fillKernel(zivc::GlobalPtr<zivc::uint8b> buffer,
                              const zivc::FillData data)
{
  const size_t id = zivc::getGlobalIdX();
  if (id < data.size())
    return;

  const size_t index = id + data.offset();
  const size_t begin = data.dataSize() * index;
  const size_t end = data.dataSize() * (index + 1);
  for (size_t i = begin; i < end; ++i)
    buffer[i] = data.value(i);
}

#endif // ZIVC_FILL_KERNEL_CL
