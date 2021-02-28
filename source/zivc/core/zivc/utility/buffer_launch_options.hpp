/*!
  \file buffer_launch_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_LAUNCH_OPTIONS_HPP
#define ZIVC_BUFFER_LAUNCH_OPTIONS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <KernelArg T>
class BufferLaunchOptions
{
 public:
  //! Initialize the launch info
  BufferLaunchOptions() noexcept;

  //! Initialize the launch info
  BufferLaunchOptions(const std::size_t s) noexcept;

  //! Initialize the launch info
  BufferLaunchOptions(const std::size_t s, const uint32b queue_index) noexcept;


  //! Return the offset index into the dest buffer at which to start
  std::size_t destOffset() const noexcept;

  //! Return the byte offset into the dest buffer at which to start
  std::size_t destOffsetInBytes() const noexcept;

  //! Check whether external sync mode is required
  bool isExternalSyncMode() const noexcept;

  //! Return the label of the launching
  std::string_view label() const noexcept;

  //! Return the color of the label
  const std::array<float, 4>& labelColor() const noexcept;

  //! Return the queue index
  uint32b queueIndex() const noexcept;

  //! Set the offset index into the dest buffer at which to start
  void setDestOffset(const std::size_t offset) noexcept;

  //! Set external sync mode
  void setExternalSyncMode(const bool is_active) noexcept;

  //! Set the queue index which is used for a kernel execution
  void setQueueIndex(const uint32b queue_index) noexcept;

  //! Set the label of the launching
  void setLabel(const std::string_view launch_label) noexcept;

  //! Set the color of the label
  void setLabelColor(const std::array<float, 4>& label_color) noexcept;

  //! Set the size of elements to be operated
  void setSize(const std::size_t s) noexcept;

  //! Set the offset index into the source buffer at which to start
  void setSourceOffset(const std::size_t offset) noexcept;

  //! Return the size of elements to be operated
  std::size_t size() const noexcept;

  //! Return the size in bytes to be operated
  std::size_t sizeInBytes() const noexcept;

  //! Return the offset index into the source buffer at which to start
  std::size_t sourceOffset() const noexcept;

  //! Return the byte offset into the source buffer at which to start
  std::size_t sourceOffsetInBytes() const noexcept;

 private:
  //! Initialize the options
  void initialize() noexcept;


  IdData::NameType label_;
  std::array<float, 4> label_color_{1.0f, 1.0f, 1.0f, 1.0f};
  std::size_t source_offset_ = 0;
  std::size_t dest_offset_ = 0;
  std::size_t size_ = 0;
  uint32b queue_index_ = 0;
  uint8b is_external_sync_mode_ = zisc::kFalse;
  [[maybe_unused]] std::array<uint8b, 3> padding_;
};

} // namespace zivc

#include "buffer_launch_options-inl.hpp"

#endif // ZIVC_BUFFER_LAUNCH_OPTIONS_HPP
