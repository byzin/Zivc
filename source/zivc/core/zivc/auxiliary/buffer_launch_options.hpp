/*!
  \file buffer_launch_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
// Zivc
#include "id_data.hpp"
#include "launch_options.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <KernelArg T>
class BufferLaunchOptions : public LaunchOptions
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

  //! Set the offset index into the dest buffer at which to start
  void setDestOffset(const std::size_t offset) noexcept;

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


  std::size_t source_offset_ = 0;
  std::size_t dest_offset_ = 0;
  std::size_t size_ = 0;
};

} // namespace zivc

#include "buffer_launch_options-inl.hpp"

#endif // ZIVC_BUFFER_LAUNCH_OPTIONS_HPP
