/*!
  \file cpu_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_SUB_PLATFORM_HPP
#define ZIVC_CPU_SUB_PLATFORM_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "zivc/device.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class Platform;
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuSubPlatform : public SubPlatform
{
 public:
  //! Create an empty cpu sub-platform
  CpuSubPlatform(Platform* platform) noexcept;

  //! Finalize the sub-platform
  ~CpuSubPlatform() noexcept override;


  //! Add the underlying device info into the given list
  void getDeviceInfoList(zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept override;

  //! Check if the sub-platform is available
  bool isAvailable() const noexcept override;

  //! Make a unique device
  SharedDevice makeDevice(const DeviceInfo& device_info) noexcept override;

  //! Return the maximum task batch size per thread
  static constexpr uint32b maxTaskBatchSize() noexcept;

  //! Return the number of available devices
  std::size_t numOfDevices() const noexcept override;

  //! Return the number of thread which is used for kernel execution
  std::size_t numOfThreads() const noexcept;

  //! Return the task batch size per thread
  std::size_t taskBatchSize() const noexcept;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Update the device info list
  void updateDeviceInfoList() noexcept override;

 protected:
  //! Destroy the sub-platform
  void destroyData() noexcept override;

  //! Initialize the sub-platform
  void initData(PlatformOptions& platform_options) override;

  //! Update debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  zisc::pmr::unique_ptr<CpuDeviceInfo> device_info_;
  uint32b num_of_threads_ = 0;
  uint32b task_batch_size_ = 0;
};

} // namespace zivc

#include "cpu_sub_platform-inl.hpp"

#endif // ZIVC_CPU_SUB_PLATFORM_HPP
