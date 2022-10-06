/*!
  \file buffer_common.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_COMMON_HPP
#define ZIVC_BUFFER_COMMON_HPP

// Standard C++ library
#include <concepts>
#include <cstddef>
#include <type_traits>
// Zivc
#include "id_data.hpp"
#include "zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class BufferCommon;

// Concepts
template <typename Derived>
concept DerivedBuffer = std::derived_from<BufferCommon, std::remove_cv_t<Derived>>;

/*!
  \brief No brief description

  No detailed description.
  */
class BufferCommon : public ZivcObject
{
 public:
  //! Initialize the buffer
  BufferCommon(IdData&& id) noexcept;

  //! Finalize the buffer
  ~BufferCommon() noexcept override;


  //! Return the capacity of the buffer
  std::size_t capacity() const noexcept;

  //! Return the capacity of the buffer in bytes
  virtual std::size_t capacityInBytes() const noexcept = 0;

  //! Clear the contents of the buffer
  virtual void clear() noexcept = 0;

  //! Destroy the buffer
  virtual void destroy() noexcept = 0;

  //! Return the buffer flag
  BufferFlag flag() const noexcept;

  //! Return the index of used heap
  virtual std::size_t heapIndex() const noexcept = 0;

  //! Check if the buffer is the most efficient for the device access
  virtual bool isDeviceLocal() const noexcept = 0;

  //! Check if the buffer is cached (readable) on the host
  virtual bool isHostCached() const noexcept = 0;

  //! Check if the buffer doesn't need to be unmapped
  virtual bool isHostCoherent() const noexcept = 0;

  //! Check if the buffer is host readable
  bool isHostReadable() const noexcept;

  //! Check if the buffer can be mapped (writable) for the host access
  virtual bool isHostVisible() const noexcept = 0;

  //! Check if the buffer is host writable
  bool isHostWritable() const noexcept;

  //! Map a buffer memory to a host
  [[nodiscard]]
  virtual void* mapMemoryData() const = 0;

  //! Return the underlying buffer data
  virtual void* rawBufferData() = 0;

  //! Return the underlying buffer data
  virtual const void* rawBufferData() const noexcept = 0;

  //! Change the number of elements
  virtual void setSize(const std::size_t s) = 0;

  //! Return the number of elements of the buffer
  std::size_t size() const noexcept;

  //! Return the size of the buffer in bytes
  virtual std::size_t sizeInBytes() const noexcept = 0;

  //! Return the size of the element type in bytes
  virtual std::size_t typeSizeInBytes() const noexcept = 0;

  //! Unmap a buffer memory
  virtual void unmapMemoryData() const noexcept = 0;

  //! Return the buffer usage flag
  BufferUsage usage() const noexcept;

 protected:
 //! Calculate the number of elements from the given size in bytes
  std::size_t calcNumOfElements(const std::size_t size) const noexcept;

  //! Set buffer flag
  void setFlag(const BufferFlag flag) noexcept;

  //! Set buffer usage flag
  void setUsage(const BufferUsage flag) noexcept;


  BufferUsage buffer_usage_ = BufferUsage::kAuto;
  BufferFlag buffer_flag_ = BufferFlag::kNone;
};

} // namespace zivc

#include "buffer_common-inl.hpp"

#endif // ZIVC_BUFFER_COMMON_HPP
