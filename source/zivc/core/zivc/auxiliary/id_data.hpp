/*!
  \file id_data.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ID_DATA_HPP
#define ZIVC_ID_DATA_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class IdData : private zisc::NonCopyable<IdData>
{
  static constexpr std::size_t kDataSize = 4;
  static constexpr std::size_t kMaxNameLength = 256;

 public:
  // Types
  using NameT = std::array<char, kMaxNameLength>;


  //! Create an ID data
  IdData(const int64b id) noexcept;

  //! Move a data
  IdData(IdData&& other) noexcept;


  //! Move a data
  IdData& operator=(IdData&& other) noexcept;


  //! Copy the id data
  IdData copy() const noexcept;

  //! Return the array size of the data
  static constexpr std::size_t dataSize() noexcept;

  //! Return the file name where this data is created
  std::string_view fileName() const noexcept;

  //! Return the data by the given index
  void* getData(const std::size_t index) noexcept;

  //! Return the data by the given index
  const void* getData(const std::size_t index) const noexcept;

  //! Chekc if this ID data has a data
  bool hasData(const std::size_t index) const noexcept;

  //! Check if this data has a file name and a line number where it is created
  bool hasFileInfo() const noexcept;

  //! Check if this data has a name
  bool hasName() const noexcept;

  //! Return the ID
  int64b id() const noexcept;

  //! Return the invalid ID number
  static constexpr int64b invalidId() noexcept;

  //! Return the line number where this data is created
  int64b lineNumber() const noexcept;

  //! Return the maximum length of name
  static constexpr std::size_t maxNameLength() noexcept;

  //! Return the name of this data
  std::string_view name() const noexcept;

  //! Set a data by the given index
  void setData(void* data, const std::size_t index) noexcept;

  //! Set a file name and a line number where this data is created
  void setFileInfo(std::string_view file_name,
                   const int64b line_number) noexcept;

  //! Set a name
  void setName(std::string_view data_name) noexcept;

 private:
  std::array<void*, kDataSize> data_list_;
  int64b id_;
  int64b line_number_;
  NameT name_;
  NameT file_name_;
};

//! Concatenate two strings
char* concatStr(const std::string_view source, char* dest) noexcept;

//! Copy one string to another
char* copyStr(const std::string_view source, char* dest) noexcept;

} // namespace zivc

#include "id_data-inl.hpp"

#endif // ZIVC_ID_DATA_HPP
