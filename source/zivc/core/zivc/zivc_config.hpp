/*!
  \file zivc_config.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONFIG_HPP
#define ZIVC_CONFIG_HPP

// Standard C++ library
#include <concepts>
#include <limits>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zivc {

// General
// Integer types
using int8b = zisc::int8b;
using int16b = zisc::int16b;
using int32b = zisc::int32b;
using int64b = zisc::int64b;
// Unsigned integer types
using uint = zisc::uint;
using uint8b = zisc::uint8b;
using uint16b = zisc::uint16b;
using uint32b = zisc::uint32b;
using uint64b = zisc::uint64b;

//! Represent bits of a type
template <typename Type>
using BitT = std::conditional_t<sizeof(Type) == 1, uint8b,
             std::conditional_t<sizeof(Type) == 2, uint16b,
             std::conditional_t<sizeof(Type) == 4, uint32b,
             std::conditional_t<sizeof(Type) == 8, uint64b,
                                                   void>>>>;

//! Represent a padding for structure
template <std::size_t kSize>
using Padding = zisc::Padding<kSize>;

// Device

/*!
  \brief No brief description

  No detailed description.
  */
enum class BackendType : uint32b
{
  kCpu = 0,
  kVulkan
};

/*!
  \brief No brief description

  No detailed description.
  */
enum class VulkanDeviceCapability : uint32b
{
  kCompute = 0,
  kGui
};

// Buffer

/*!
  \brief No brief description

  No detailed description.
  */
enum class BufferUsage : uint32b
{
  kAuto = 0b1u << 0,
  kPreferDevice = 0b1u << 1,
  kPreferHost = 0b1u << 2,
};

/*!
  \brief No brief description

  No detailed description.
  */
enum class BufferFlag : uint32b
{
  kNone = 0b1u << 0,
  kSequentialWritable = 0b1u << 1, //!< Declares that mapped memory will only be written sequentially
  kRandomAccessible = 0b1u << 2, //!< Declares that mapped memory can be read, written, and accessed in random order
};

// Kernel

//! A kernel parameter is standard layout type and trivially copyable
template <typename Type>
concept KernelArg = std::is_trivial_v<Type> &&
                    std::is_standard_layout_v<Type> &&
                    !std::is_same_v<bool, std::remove_cvref_t<Type>>;

/*!
  \brief config values in zivc

  No detailed description.
  */
class Config
{
 public:
  //! Check if vulkan backend is enabled
  static constexpr bool isVulkanBackendEnabled() noexcept;

  //! Return the value of the major component of the Zivc version number
  static constexpr int versionMajor() noexcept;

  //! Return the value of the minor component of the Zivc version number
  static constexpr int versionMinor() noexcept;

  //! Return the value of the patch component of the Zivc version number
  static constexpr int versionPatch() noexcept;

  //! Return the version string of Zivc
  static constexpr std::string_view versionString() noexcept;
};

} // namespace zivc

#if __has_include("zivc/zivc_config-inl.hpp")
#include "zivc/zivc_config-inl.hpp" // It will be generted by cmake
#endif

#endif // ZIVC_CONFIG_HPP
