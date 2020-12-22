/*!
  \file device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_HPP
#define ZIVC_DEVICE_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer.hpp"
#include "zivc_config.hpp"
#include "utility/kernel_arg_parser.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class Fence;
class SubPlatform;
template <typename SetType, typename ...ArgTypes> class KernelParameters;

/*!
  \brief No brief description

  No detailed description.
  */
class Device : public ZivcObject
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Device>;
  using WeakPtr = std::weak_ptr<Device>;


  //! Initialize the device
  Device(IdData&& id) noexcept;

  //! Finalize the device
  virtual ~Device() noexcept;


  //! Destroy the data
  void destroy() noexcept;

  //! Return the underlying device info
  const DeviceInfo& deviceInfo() const noexcept;

  //! Initialize the device
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const DeviceInfo& device_info);

  //! Make a buffer
  template <typename T>
  SharedBuffer<T> makeBuffer(const BufferUsage flag);

  //! Make a kernel
  template <std::size_t kDimension, typename SetType, typename ...ArgTypes>
  SharedKernel<kDimension, SetType, ArgTypes...> makeKernel(
      const KernelParameters<SetType, ArgTypes...>& parameters);

  //! Return the number of available fences
  virtual std::size_t numOfFences() const noexcept = 0;

  //! Return the number of underlying command queues
  virtual std::size_t numOfQueues() const noexcept = 0;

  //! Return the peak memory usage of the heap of the given number
  virtual std::size_t peakMemoryUsage(const std::size_t number) const noexcept = 0;

  //! Return the ownership of the fence to the device
  virtual void returnFence(Fence* fence) noexcept = 0;

  //! Set the number of fences
  virtual void setNumOfFences(const std::size_t s) = 0;

  //! Take a fence data from the device
  virtual void takeFence(Fence* fence) = 0;

  //! Return the current memory usage of the heap of the given number
  virtual std::size_t totalMemoryUsage(const std::size_t number) const noexcept = 0;

  //! Wait for a device to be idle
  virtual void waitForCompletion() const noexcept = 0;

  //! Wait for a queue to be idle
  virtual void waitForCompletion(const uint32b queue_index) const noexcept = 0;

  //! Wait for a fence to be signaled
  virtual void waitForCompletion(const Fence& fence) const noexcept = 0;

 protected:
  //! Destroy the data
  virtual void destroyData() noexcept = 0;

  //! Initialize the device
  virtual void initData() = 0;

 private:
  const DeviceInfo* device_info_ = nullptr;
};

// Type aliases
using SharedDevice = Device::SharedPtr;
using WeakDevice = Device::WeakPtr;

} // namespace zivc

#include "device-inl.hpp"

#endif // ZIVC_DEVICE_HPP
