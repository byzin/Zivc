/*!
  \file zivc_config.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONFIG_HPP
#define ZIVC_CONFIG_HPP

// Standard C++ library
#include <limits>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/zisc_config.hpp"
#include "zisc/utility.hpp"

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

// Device

/*!
  \brief No brief description

  No detailed description.
  */
enum class SubPlatformType : uint32b
{
  kCpu = 0,
  kVulkan
};

// Buffer

constexpr std::size_t kBufferWholeSize = (std::numeric_limits<uint64b>::max)();

/*!
  \brief No brief description

  No detailed description.
  */
enum class BufferUsage : uint32b
{
  kDeviceOnly = 0b1u << 0,
  kHostOnly = 0b1u << 1,
  kHostToDevice = 0b1u << 2,
  kDeviceToHost = 0b1u << 3,
};

/*!
  \brief config values in zivc

  No detailed description.
  */
class Config
{
 public:
  //! Result type of vector comparison
  template <typename Type>
  using ComparisonResultType = std::conditional_t<sizeof(Type) == 1, int8b,
                               std::conditional_t<sizeof(Type) == 2, int16b,
                               std::conditional_t<sizeof(Type) == 8, int64b,
                                                                     int32b>>>;


  //! Check if vulkan sub-platform is enabled
  static constexpr bool isVulkanSubPlatformEnabled() noexcept;

  //! Represent 'false' value of scalar value comparison
  static constexpr int32b scalarResultFalse() noexcept;

  //! Represent 'true' value of scalar value comparison
  static constexpr int32b scalarResultTrue() noexcept;

  //! Represent 'false' value of scalar value comparison
  template <typename Type>
  static constexpr ComparisonResultType<Type> vecResultFalse() noexcept;

  //! Represent 'true' value of scalar value comparison
  template <typename Type>
  static constexpr ComparisonResultType<Type> vecResultTrue() noexcept;

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

#include "zivc/zivc_config-inl.hpp"

#endif // ZIVC_CONFIG_HPP
