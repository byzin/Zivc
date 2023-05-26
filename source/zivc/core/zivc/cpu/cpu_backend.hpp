/*!
  \file cpu_backend.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_BACKEND_HPP
#define ZIVC_CPU_BACKEND_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <span>
#include <vector>
// Zisc
#include "zisc/concurrency/thread_manager.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "../backend.hpp"
#include "../device.hpp"
#include "../zivc_config.hpp"

namespace zivc {

// Forward declaration
class Context;
class ContextOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuBackend : public Backend
{
 public:
  //! Create an empty cpu backend
  CpuBackend(Context* context) noexcept;

  //! Finalize the backend
  ~CpuBackend() noexcept override;


  //! Create a unique device
  [[nodiscard]]
  SharedDevice createDevice(const DeviceInfo& device_info) override;

  //! Return the underlying device info
  const DeviceInfo& deviceInfo(const std::size_t index) const noexcept override;

  //! Check if the backend is available
  bool isAvailable() const noexcept override;

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

  //! Return the backend type
  BackendType type() const noexcept override;

  //! Update the underlying device info
  void updateDeviceInfo() override;

 protected:
  //! Destroy the backend
  void destroyData() noexcept override;

  //! Initialize the backend
  void initData(ContextOptions& options) override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  zisc::pmr::unique_ptr<CpuDeviceInfo> device_info_;
  zisc::pmr::unique_ptr<zisc::ThreadManager> thread_manager_;
  uint32b task_batch_size_ = 0;
  [[maybe_unused]] Padding<4> pad_;
};

} // namespace zivc

#include "cpu_backend-inl.hpp"

#endif // ZIVC_CPU_BACKEND_HPP
