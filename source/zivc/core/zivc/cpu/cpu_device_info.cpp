/*!
  \file cpu_device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_device_info.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/cpu_features.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] mem_resource No description.
  */
CpuDeviceInfo::CpuDeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept :
    DeviceInfo(mem_resource)
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
CpuDeviceInfo::CpuDeviceInfo(CpuDeviceInfo&& other) noexcept :
    DeviceInfo(std::move(other)),
    name_{other.name_},
    vendor_name_{other.vendor_name_},
    memory_stats_{other.memory_stats_}
{
}

/*!
  \details No detailed description
  */
CpuDeviceInfo::~CpuDeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
CpuDeviceInfo& CpuDeviceInfo::operator=(CpuDeviceInfo&& other) noexcept
{
  name_ = other.name_;
  vendor_name_ = other.vendor_name_;
  memory_stats_ = other.memory_stats_;
  DeviceInfo::operator=(std::move(other));
  return *this;
}

/*!
  \details No detailed description
  */
void CpuDeviceInfo::fetch() noexcept
{
  initCpuInfo();
  initHeapInfoList();
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDeviceInfo::maxAllocationSize() const noexcept
{
  const std::size_t max_alloc = 8ULL * 1024ULL * 1024ULL * 1024ULL; // 8 GB
  return max_alloc;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDeviceInfo::maxNumOfBuffersPerKernel() const noexcept
{
  return maxNumOfFunctionParameters();
}

/*!
  \details No detailed description

  \return No description
  */
std::array<uint32b, 3> CpuDeviceInfo::maxWorkGroupCount() const noexcept
{
  constexpr uint32b m = (std::numeric_limits<uint32b>::max)();
  constexpr std::array<uint32b, 3> max_list{{m, m, m}};
  return max_list;
}

/*!
  \details No detailed description

  \return No description
  */
auto CpuDeviceInfo::memoryStats() const noexcept -> const MemoryStats&
{
  return memory_stats_;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view CpuDeviceInfo::name() const noexcept
{
  const std::string_view n{name_.data()};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType CpuDeviceInfo::type() const noexcept
{
  return SubPlatformType::kCpu;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view CpuDeviceInfo::vendorName() const noexcept
{
  const std::string_view n{vendor_name_.data()};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
uint32b CpuDeviceInfo::workGroupSize() const noexcept
{
  return 1;
}

/*!
  \details No detailed description
  */
void CpuDeviceInfo::initCpuInfo() noexcept
{
  getCpuFeatures(name_.data(), vendor_name_.data());
}

/*!
  \details No detailed description
  */
void CpuDeviceInfo::initHeapInfoList() noexcept
{
  memory_stats_ = zisc::Memory::retrieveSystemStats();
  MemoryHeapInfo info;
  info.setDeviceLocal(true);
  {
    const std::size_t size = memory_stats_.totalPhysicalMemory();
    info.setTotalSize(size);
  }
  {
    const std::size_t size = memory_stats_.availablePhysicalMemory();
    info.setAvailableSize(size);
  }
  auto& heap_info_list = DeviceInfo::heapInfoList();
  heap_info_list.clear();
  heap_info_list.resize(1);
  heap_info_list[0] = info;
}

} // namespace zivc
