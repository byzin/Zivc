/*!
  \file platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_PLATFORM_HPP
#define ZIVC_PLATFORM_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "device.hpp"
#include "sub_platform.hpp"
#include "zivc_config.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class Platform : private zisc::NonCopyable<Platform>
{
 public:
  //! Create an empty platform
  Platform() noexcept;

  //! Move a data
  Platform(Platform&& other) noexcept;

  //! Finalize the platform
  ~Platform() noexcept;


  //! Move a data
  Platform& operator=(Platform&& other) noexcept;


  //! Destroy the platform
  void destroy() noexcept;

  //! Return the device info list
  const zisc::pmr::vector<const DeviceInfo*>& deviceInfoList() const noexcept;

  //! Check if the platform has the sub-platform of the given type
  bool hasSubPlatform(const SubPlatformType type) const noexcept;

  //! Initialize the platform
  void initialize(PlatformOptions& options);

  //! Check if the platform is in debug mode
  bool isDebugMode() const noexcept;

  //! Issue an ID of an object
  [[nodiscard]]
  IdData issueId() noexcept;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Quaery a device
  [[nodiscard]]
  SharedDevice queryDevice(const std::size_t device_index);

  //! Return the sub-platform of the given type
  SubPlatform* subPlatform(const SubPlatformType type) noexcept;

  //! Return the sub-platform of the given type
  const SubPlatform* subPlatform(const SubPlatformType type) const noexcept;

 private:
  //! Create a sub-platform
  template <typename SubPlatformType>
  void initSubPlatform(PlatformOptions& options);

  //! Make a unique device
  [[nodiscard]]
  SharedDevice makeDevice(const std::size_t device_index);

  //! Move memory resource data
  void moveMemoryResource(Platform& other) noexcept;

  //! Set debug mode
  void setDebugMode(const bool is_debug_mode) noexcept;

  //! Set memory resource for Zivc
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Set a sub-platform of the given type
  void setSubPlatform(SharedSubPlatform&& sub_platform) noexcept;

  //! Update the device info list
  void updateDeviceInfoList();


  static constexpr std::size_t kNumOfSubPlatforms = 2;


  std::unique_ptr<zisc::pmr::memory_resource> default_mem_resource_;
  zisc::pmr::memory_resource* custom_mem_resource_ = nullptr;
  std::array<SharedSubPlatform, kNumOfSubPlatforms> sub_platform_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<WeakDevice>> device_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<const DeviceInfo*>> device_info_list_;
  std::atomic<int64b> id_count_ = 0;
  int32b is_debug_mode_;
  [[maybe_unused]] Padding<4> pad_;
};

// Type aliases
using SharedPlatform = std::shared_ptr<Platform>;

//! Make a unique platform
[[nodiscard]]
SharedPlatform makePlatform(PlatformOptions& options);

} // namespace zivc

#include "platform-inl.hpp"

#endif // ZIVC_PLATFORM_HPP
