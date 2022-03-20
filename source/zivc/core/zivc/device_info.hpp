/*!
  \file device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INFO_HPP
#define ZIVC_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/memory_heap_info.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class DeviceInfo : private zisc::NonCopyable<DeviceInfo>
{
 public:
  //! Initialize the device info
  DeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  DeviceInfo(DeviceInfo&& other) noexcept;

  //! Finalize the device info
  virtual ~DeviceInfo() noexcept;


  //! Move a data
  DeviceInfo& operator=(DeviceInfo&& other) noexcept;


  //! Return the heap info by the given index
  MemoryHeapInfo& heapInfo(const std::size_t heap_index) noexcept;

  //! Return the heap info by the given index
  const MemoryHeapInfo& heapInfo(const std::size_t heap_index) const noexcept;

  //! Return the heap info list
  std::span<MemoryHeapInfo> heapInfoList() noexcept;

  //! Return the heap info list
  std::span<const MemoryHeapInfo> heapInfoList() const noexcept;

  //! Return invalid name string
  static std::string_view invalidName() noexcept;

  //! Return the possible maximum size of an allocation in bytes
  virtual std::size_t maxAllocationSize() const noexcept = 0;

  //! Return the possible maximum number of buffer arguments per kernel
  virtual std::size_t maxNumOfBuffersPerKernel() const noexcept = 0;

  //! Return the maximum work group count
  virtual std::array<uint32b, 3> maxWorkGroupCount() const noexcept = 0;

  //! Return the device name
  virtual std::string_view name() const noexcept = 0;

  //! Return the backend type
  virtual BackendType type() const noexcept = 0;

  //! Return the vendor name
  virtual std::string_view vendorName() const noexcept = 0;

  //! Return the local work group size of the device
  virtual uint32b workGroupSize() const noexcept = 0;

 protected:
  //! Return the maximum number of parameters for a function
  static constexpr std::size_t maxNumOfFunctionParameters() noexcept;

  //! Set the size of heap info list
  void setHeapInfoListSize(const std::size_t size) noexcept;

 private:
  static constexpr char kInvalidName[] = "N/A";


  zisc::pmr::vector<MemoryHeapInfo> heap_info_list_;
};

} // namespace zivc
#include "device_info-inl.hpp"

#endif // ZIVC_DEVICE_INFO_HPP
