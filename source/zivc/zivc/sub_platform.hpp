/*!
  \file sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SUB_PLATFORM_HPP
#define ZIVC_SUB_PLATFORM_HPP

// Standard C++ library
#include <atomic>
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "device.hpp"
#include "device_info.hpp"
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
class PlatformOptions;
class Platform;

/*!
  \brief No brief description

  No detailed description.
  */
class SubPlatform : public ZivcObject
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<SubPlatform>;
  using WeakPtr = std::weak_ptr<SubPlatform>;


  //! Create an empty sub-platform
  SubPlatform(Platform* platform) noexcept;

  //! Finalize the platform
  ~SubPlatform() noexcept override;


  //! Destroy the sub-platform
  void destroy() noexcept;

  //! Add the underlying device info into the given list
  virtual void getDeviceInfoList(
      zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept = 0;

  //! Initialize the sub-platform
  void initialize(WeakPtr&& own, PlatformOptions& platform_options);

  //! Check if the sub-platform is available
  virtual bool isAvailable() const noexcept = 0;

  //! Check if the sub-platform is in debug mode
  bool isDebugMode() const noexcept override;

  //! Issue an ID of an object
  IdData issueId() noexcept override;

  //! Make a unique device
  virtual SharedDevice makeDevice(const DeviceInfo& device_info) = 0;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept override;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept override;

  //! Return the number of available devices 
  virtual std::size_t numOfDevices() const noexcept = 0;

  //! Update the device info list
  virtual void updateDeviceInfoList() noexcept = 0;

 protected:
  //! Destroy the sub-platform
  virtual void destroyData() noexcept = 0;

  //! Initialize the sub-platform
  virtual void initData(PlatformOptions& platform_options) = 0;

 private:
  Platform* platform_ = nullptr;
};

// Type aliases
using SharedSubPlatform = SubPlatform::SharedPtr;
using WeakSubPlatform = SubPlatform::WeakPtr;

} // namespace zivc

#endif // ZIVC_SUB_PLATFORM_HPP
