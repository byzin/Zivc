/*!
  \file cpu_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include "zisc/thread/thread_manager.hpp"
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
  [[nodiscard]]
  SharedDevice makeDevice(const DeviceInfo& device_info) override;

  //! Return the maximum task batch size per thread
  static constexpr uint32b maxTaskBatchSize() noexcept;

  //! Notify of device memory allocation
  void notifyOfDeviceMemoryAllocation(const std::size_t size) noexcept;

  //! Notify of device memory deallocation
  void notifyOfDeviceMemoryDeallocation(const std::size_t size) noexcept;

  //! Return the number of available devices
  std::size_t numOfDevices() const noexcept override;

  //! Return the task batch size per thread
  std::size_t taskBatchSize() const noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  zisc::ThreadManager& threadManager() noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  const zisc::ThreadManager& threadManager() const noexcept;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Update the device info list
  void updateDeviceInfoList() override;

 protected:
  //! Destroy the sub-platform
  void destroyData() noexcept override;

  //! Initialize the sub-platform
  void initData(PlatformOptions& options) override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  zisc::pmr::unique_ptr<CpuDeviceInfo> device_info_;
  zisc::pmr::unique_ptr<zisc::ThreadManager> thread_manager_;
  uint32b task_batch_size_ = 0;
  [[maybe_unused]] Padding<4> pad_;
};

} // namespace zivc

#include "cpu_sub_platform-inl.hpp"

#endif // ZIVC_CPU_SUB_PLATFORM_HPP
