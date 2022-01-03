/*!
  \file cpu_device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_DEVICE_HPP
#define ZIVC_CPU_DEVICE_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/function_reference.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/thread/thread_manager.hpp"
// Zivc
#include "zivc/device.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class Fence;
class CpuDeviceInfo;
class CpuSubPlatform;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuDevice : public Device
{
 public:
  // Type aliases
  using Command = zisc::FunctionReference<void ()>;


  //! Initialize the cpu device
  CpuDevice(IdData&& id);

  //! Finalize the cpu instance
  ~CpuDevice() noexcept override;


  //! Return the underlying device info
  const CpuDeviceInfo& deviceInfoImpl() const noexcept;

  //! Return the memory usage by the given heap index
  zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) noexcept override;

  //! Return the memory usage by the given heap index
  const zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) const noexcept override;

  //! Notify of device memory allocation
  void notifyAllocation(const std::size_t size) noexcept;

  //! Notify of device memory deallocation
  void notifyDeallocation(const std::size_t size) noexcept;

  //! Return the number of available fences
  std::size_t numOfFences() const noexcept override;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the number of threads which are used for kernel execution
  std::size_t numOfThreads() const noexcept;

  //! Return the use of the given fence to the device
  void returnFence(Fence* fence) noexcept override;

  //! Set the number of fences
  void setFenceSize(const std::size_t s) override;

  //! Submit a kernel command
  void submit(const Command& command,
              const uint32b dimension,
              const std::array<uint32b, 3>& work_size,
              const std::array<uint32b, 3>& global_id_offset,
              std::atomic<uint32b>* id,
              Fence* fence);

  //! Take a use of a fence from the device
  void takeFence(Fence* fence) override;

  //! Return the task batch size per thread
  std::size_t taskBatchSize() const noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  zisc::ThreadManager& threadManager() noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  const zisc::ThreadManager& threadManager() const noexcept;

  //! Wait for a device to be idle
  void waitForCompletion() const override;

  //! Wait for a queue to be idle
  void waitForCompletion(const uint32b queue_index) const override;

  //! Wait for a fence to be signaled
  void waitForCompletion(const Fence& fence) const override;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

  //! Initialize the device
  void initData() override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  //! Execute a command on a number of the given batch size
  static void execBatchCommand(const Command& command,
                               const uint32b block_id,
                               const uint32b group_id_max,
                               const uint32b batch_size) noexcept;

  //! Issue new block ID
  static uint32b issue(std::atomic<uint32b>* counter) noexcept;

  //! Return the sub-platform
  CpuSubPlatform& parentImpl() noexcept;

  //! Return the sub-platform
  const CpuSubPlatform& parentImpl() const noexcept;


  zisc::Memory::Usage heap_usage_;
  zisc::pmr::unique_ptr<zisc::ThreadManager> thread_manager_;
};

} // namespace zivc

#include "cpu_device-inl.hpp"

#endif // ZIVC_CPU_DEVICE_HPP
