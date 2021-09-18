/*!
  \file fill_data.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_DATA_CL
#define ZIVC_FILL_DATA_CL

// Zivc
#include "zivc/cl/types.cl"

namespace zivc {

// Forward declaration
template <typename Type> class FillData;

//! Check if the two fill data are equal
template <typename Type>
bool operator==(const FillData<Type>& lhs, const FillData<Type>& rhs) noexcept;

//! Check if the two fill data are not equal
template <typename Type>
bool operator!=(const FillData<Type>& lhs, const FillData<Type>& rhs) noexcept;


/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class FillData
{
 public:
  //! Return the value of the data at the index
  Type operator[](const size_t index) const noexcept;


  //! Return the length of data array
  static constexpr size_t length() noexcept;

  //! Check if the data and the given data are qeual in value
  bool isEqual(const FillData& other) const noexcept;

  //! Set the given value to the data at the index
  void set(const Type value, const size_t index) noexcept;

  //! Return the value of the data at the index
  Type get(const size_t index) const noexcept;


 private:
  //! Represent the source data for fill
  template <size_t kN>
  struct DataImpl
  {
    //! Check if the data and the given data are equal in value
    bool isEqual(const DataImpl& other) const noexcept;

    //! Set the given value to the data at the index
    void set(const Type value, const size_t index) noexcept;

    //! Return the value of the data at the index
    Type get(const size_t index) const noexcept;


    DataImpl<kN - 1> precedence_;
    Type data_;
    static_assert(alignof(DataImpl<kN - 1>) == alignof(Type));
  };

  //! Represent the source data for fill
  template <>
  struct DataImpl<0>
  {
    //! Check if the data and the given data are equal in value
    bool isEqual(const DataImpl& other) const noexcept
    {
      return data_ == other.data_;
    }

    //! Set the given value to the data at the index
    void set(const Type value, const size_t index) noexcept
    {
      data_ = value;
    }

    //! Return the value of the data at the index
    Type get(const size_t index) const noexcept
    {
      return data_;
    }


    Type data_;
  };


  static constexpr size_t kSize = 1 * 16;
  static constexpr size_t kLength = kSize / sizeof(Type);


  DataImpl<kLength> data_;
  static_assert(sizeof(data_) == kSize);
};

// Type aliases
using FillDataU8 = FillData<uint8b>;
using FillDataU16 = FillData<uint16b>;

} // namespace zivc

#include "fill_data-inl.cl"

#endif // ZIVC_FILL_DATA_CL
