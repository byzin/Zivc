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
#include "cpu_features.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] mem_resource No description.
  */
CpuDeviceInfo::CpuDeviceInfo([[maybe_unused]] zisc::pmr::memory_resource* mem_resource) noexcept :
    DeviceInfo()
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
CpuDeviceInfo::CpuDeviceInfo(CpuDeviceInfo&& other) noexcept :
    DeviceInfo(std::move(other)),
    name_{std::move(other.name_)},
    vendor_name_{std::move(other.vendor_name_)},
    memory_stats_{std::move(other.memory_stats_)}
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
  DeviceInfo::operator=(std::move(other));
  name_ = std::move(other.name_);
  vendor_name_ = std::move(other.vendor_name_);
  memory_stats_ = std::move(other.memory_stats_);
  return *this;
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
std::size_t CpuDeviceInfo::availableMemory([[maybe_unused]] const std::size_t heap_index) const noexcept
{
  const std::size_t memory_size = memory_stats_.availablePhysicalMemory();
  return memory_size;
}

/*!
  \details No detailed description
  */
void CpuDeviceInfo::fetch() noexcept
{
  initCpuInfo();
  memory_stats_ = zisc::Memory::retrieveSystemStats();
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDeviceInfo::maxAllocationSize() const noexcept
{
  const std::size_t max_alloc = 16ull * 1024ull * 1024ull * 1024ull; // 16 GB
  return max_alloc;
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
std::size_t CpuDeviceInfo::numOfHeaps() const noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
std::size_t CpuDeviceInfo::totalMemory([[maybe_unused]] const std::size_t heap_index) const noexcept
{
  const std::size_t memory_size = memory_stats_.totalPhysicalMemory();
  return memory_size;
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
  auto set_name = [this](const char* n)
  {
    std::strcpy(name_.data(), n);
  };
  auto set_vendor_name = [this](const char* n)
  {
    std::strcpy(vendor_name_.data(), n);
  };

  using namespace cpu_features;
  // Initialize device info
#if defined(CPU_FEATURES_ARCH_X86)
  {
    char brand_string[49];
    FillX86BrandString(brand_string);
    set_name(brand_string);
  }
  {
    const X86Info info = GetX86Info();
    set_vendor_name(info.vendor);
  }
#elif defined(CPU_FEATURES_ARCH_ARM)
  static_assert(false, "Not implemented yet.");
  {
    const ArmInfo info = GetArmInfo();
  }
  set_name("ARM");
#elif defined(CPU_FEATURES_ARCH_AARCH64)
  static_assert(false, "Not implemented yet.");
  {
    const Aarch64Info info = GetAarch64Info();
  }
#elif defined(CPU_FEATURES_ARCH_MIPS)
  static_assert(false, "Not implemented yet.");
  {
    const MipsInfo info = GetMipsInfo();
  }
#elif defined(CPU_FEATURES_ARCH_PPC)
  static_assert(false, "Not implemented yet.");
  {
    const PPCInfo info = GetPPCInfo();
    const PPCPlatformStrings strings = GetPPCPlatformStrings();
  }
#endif
  if (std::string_view{name_.data()}.empty())
    set_name("N/A");
  if (std::string_view{vendor_name_.data()}.empty())
    set_vendor_name("N/A");
}

} // namespace zivc
