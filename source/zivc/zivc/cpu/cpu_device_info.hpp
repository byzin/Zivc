/*!
  \file cpu_device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_DEVICE_INFO_HPP
#define ZIVC_CPU_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
// Zisc
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class CpuDeviceInfo : public DeviceInfo
{
 public:
  // Type aliases
  using MemoryStats = zisc::Memory::SystemMemoryStats;


  //! Initialize the device info
  CpuDeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  CpuDeviceInfo(CpuDeviceInfo&& other) noexcept;

  //! Finalize the device info
  ~CpuDeviceInfo() noexcept override;


  //! Move a data
  CpuDeviceInfo& operator=(CpuDeviceInfo&& other) noexcept;


  //! Return the amount of actual device memory currently available in bytes
  std::size_t availableMemory(const std::size_t heap_index) const noexcept override;

  //! Fetch device info from the host
  void fetch() noexcept;

  //! Return the maximum size of an allocation in bytes
  std::size_t maxAllocationSize() const noexcept override;

  //! Return the maximum work group count
  std::array<uint32b, 3> maxWorkGroupCount() const noexcept override;

  //! Return the memory stats of the device
  const MemoryStats& memoryStats() const noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return the number of heaps of the device local
  std::size_t numOfHeaps() const noexcept override;

  //! Return the amount of actual device memory in bytes
  std::size_t totalMemory(const std::size_t heap_index) const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Return the local work group size of the device
  uint32b workGroupSize() const noexcept override;

 private:
  //! Initialize CPU info
  void initCpuInfo() noexcept;


  IdData::NameType name_;
  IdData::NameType vendor_name_;
  MemoryStats memory_stats_;
};

} // namespace zivc

#endif // ZIVC_CPU_DEVICE_INFO_HPP
