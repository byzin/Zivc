/*!
  \file cpu_features.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_features.hpp"
// Standard C++ library
#include <array>
#include <cstring>
#include <string_view>

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif // Z_GCC || Z_CLANG

// cpu_features
#include "cpu_features_macros.h"
#if defined(CPU_FEATURES_ARCH_X86)
#include "cpuinfo_x86.h"
#elif defined(CPU_FEATURES_ARCH_ARM)
#include "cpuinfo_arm.h"
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include "cpuinfo_aarch64.h"
#elif defined(CPU_FEATURES_ARCH_MIPS)
#include "cpuinfo_mips.h"
#elif defined(CPU_FEATURES_ARCH_PPC)
#include "cpuinfo_ppc.h"
#endif

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

// Zivc
#include "zivc/device_info.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [out] cpu_name No description.
  \param [out] vendor_name No description.
  */
void getCpuFeatures(char* cpu_name, char* vendor_name) noexcept
{
  IdData::NameT cpu_n;
  IdData::NameT vendor_n;
  auto set_cpu_name = [&cpu_n](const std::string_view name)
  {
    copyStr(name, cpu_n.data());
  };
  auto set_vendor_name = [&vendor_n](const std::string_view name)
  {
    copyStr(name, vendor_n.data());
  };

  // Init data
  set_cpu_name(DeviceInfo::invalidName());
  set_vendor_name(DeviceInfo::invalidName());

  // Initialize device info
  namespace cpu = cpu_features;
#if defined(CPU_FEATURES_ARCH_X86)
  {
    const cpu::X86Info info = cpu::GetX86Info();
    set_vendor_name(info.vendor);
    set_cpu_name(info.brand_string);
  }
#elif defined(CPU_FEATURES_ARCH_ARM)
  static_assert(false, "Not implemented yet.");
  {
    const cpu::ArmInfo info = cpu::GetArmInfo();
  }
#elif defined(CPU_FEATURES_ARCH_AARCH64)
  static_assert(false, "Not implemented yet.");
  {
    const cpu::Aarch64Info info = cpu::GetAarch64Info();
  }
#elif defined(CPU_FEATURES_ARCH_MIPS)
  static_assert(false, "Not implemented yet.");
  {
    const cpu::MipsInfo info = cpu::GetMipsInfo();
  }
#elif defined(CPU_FEATURES_ARCH_PPC)
  static_assert(false, "Not implemented yet.");
  {
    const cpu::PPCInfo info = cpu::GetPPCInfo();
    const cpu::PPCPlatformStrings strings = cpu::GetPPCPlatformStrings();
  }
#endif

  //
  copyStr(cpu_n.data(), cpu_name);
  copyStr(vendor_n.data(), vendor_name);
}

} // namespace zivc
