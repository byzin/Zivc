/*!
  \file memory_heap_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_MEMORY_HEAP_INFO_HPP
#define ZIVC_MEMORY_HEAP_INFO_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/memory/memory.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class MemoryHeapInfo
{
 public:
  // Type alias
  using UsageT = zisc::Memory::Usage;


  //! Return the available memory size in the heap
  std::size_t availableSize() const noexcept;

  //! Check if the heap is device local
  bool isDeviceLocal() const noexcept;

  //! Set the available memory size of the heap
  void setAvailableSize(const std::size_t size) noexcept;

  //! Set the flag of device local
  void setDeviceLocal(const bool flag) noexcept;

  //! Set the total memory size of the heap
  void setTotalSize(const std::size_t size) noexcept;

  //! Return the total memory size in the heap
  std::size_t totalSize() const noexcept;

  //! Return the currently used size for buffers in the heap
  UsageT& usedSizeForBuffer() noexcept;

  //! Return the currently used size for buffers in the heap
  const UsageT& usedSizeForBuffer() const noexcept;

 private:
  std::size_t total_size_ = 0;
  std::size_t available_size_ = 0;
  UsageT used_size_for_buffer_;
  zisc::Boolean is_device_local_;
  [[maybe_unused]] Padding<std::alignment_of_v<std::size_t> - 1> pad_;
};

} // namespace zivc

#include "memory_heap_info-inl.hpp"

#endif // ZIVC_MEMORY_HEAP_INFO_HPP
