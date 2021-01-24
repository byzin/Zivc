/*!
  \file buffer_common.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_COMMON_HPP
#define ZIVC_BUFFER_COMMON_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

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


  //! Return the capacity of the buffer in bytes
  virtual std::size_t capacityInBytes() const noexcept = 0;

  //! Return the capacity of the buffer
  template <zisc::TriviallyCopyable T>
  std::size_t getCapacity() const noexcept;

  //! Return the number of elements of the buffer
  template <zisc::TriviallyCopyable T>
  std::size_t getSize() const noexcept;

  //! Return the index of used heap
  virtual std::size_t heapIndex() const noexcept = 0;

  //! Check if the buffer is the most efficient for the device access
  virtual bool isDeviceLocal() const noexcept = 0;

  //! Check if the buffer is cached on the host
  virtual bool isHostCached() const noexcept = 0;

  //! Check if the buffer doesn't need to be unmapped
  virtual bool isHostCoherent() const noexcept = 0;

  //! Check if the buffer can be mapped for the host access
  virtual bool isHostVisible() const noexcept = 0;

  //! Map a buffer memory to a host
  [[nodiscard]]
  virtual void* mapMemory() const = 0;

  //! Change the number of elements
  virtual void setSize(const std::size_t s) = 0;

  //! Return the size of the buffer in bytes
  virtual std::size_t sizeInBytes() const noexcept = 0;

  //! Return the underlying type size in bytes
  std::size_t typeSize() const noexcept;

  //! Unmap a buffer memory
  virtual void unmapMemory() const noexcept = 0;

  //! Return the buffer usage flag
  BufferUsage usage() const noexcept;

 protected:
  //! Calculate the number of elements from the given size
  template <zisc::TriviallyCopyable T>
  std::size_t calcSize(const std::size_t s) const noexcept;

  //! Set internal type size
  void setTypeSize(const std::size_t s) noexcept;

  //! Set buffer usage flag
  void setUsage(const BufferUsage flag) noexcept;


  BufferUsage buffer_usage_;
  uint32b type_size_ = 0;
};

} // namespace zivc

#include "buffer_common-inl.hpp"

#endif // ZIVC_BUFFER_COMMON_HPP
