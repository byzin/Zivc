/*!
  \file device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer.hpp"
#include "kernel_set.hpp"
#include "zivc_config.hpp"
#include "utility/kernel_arg_parser.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
class DeviceInfo;
class Fence;
class SubPlatform;
template <std::size_t, DerivedKSet, typename...> class KernelParams;
template <typename T>
SharedBuffer<T> makeBuffer(Device* device, const BufferUsage flag);
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
SharedKernel<kDim, KSet, Args...> makeKernel(
    Device* device,
    const KernelParams<kDim, KSet, Args...>& params);

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
  ~Device() noexcept override;


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
  [[nodiscard]]
  SharedBuffer<T> makeBuffer(const BufferUsage flag);

  //! Make a kernel from the given parameters
  template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
  [[nodiscard]]
  SharedKernel<kDim, KSet, Args...> makeKernel(
      const KernelParams<kDim, KSet, Args...>& params);

  //! Return the memory usage by the given heap index
  virtual zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) noexcept = 0;

  //! Return the memory usage by the given heap index
  virtual const zisc::Memory::Usage& memoryUsage(const std::size_t heap_index) const noexcept = 0;

  //! Return the number of available fences
  virtual std::size_t numOfFences() const noexcept = 0;

  //! Return the number of underlying command queues
  virtual std::size_t numOfQueues() const noexcept = 0;

  //! Return the use of the given fence to the device
  virtual void returnFence(Fence* fence) noexcept = 0;

  //! Set the number of fences
  virtual void setFenceSize(const std::size_t s) = 0;

  //! Take a use of a fence from the device
  virtual void takeFence(Fence* fence) = 0;

  //! Wait for a device to be idle
  virtual void waitForCompletion() const = 0;

  //! Wait for a queue to be idle
  virtual void waitForCompletion(const uint32b queue_index) const = 0;

  //! Wait for a fence to be signaled
  virtual void waitForCompletion(const Fence& fence) const = 0;

 protected:
  //! Destroy the data
  virtual void destroyData() noexcept = 0;

  //! Initialize the device
  virtual void initData() = 0;

 private:
  template <typename T>
  friend SharedBuffer<T> makeBuffer(Device*, const BufferUsage);
  template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
  friend SharedKernel<kDim, KSet, Args...> makeKernel(Device*, const KernelParams<kDim, KSet, Args...>&);


  //! Make a buffer
  template <template<typename> typename Derived, typename T>
  [[nodiscard]]
  SharedBuffer<T> makeDerivedBuffer(const BufferUsage flag);

  //! Make a kernel from the given parameters
  template <template<typename, typename...> typename Derived,
            std::size_t kDim, DerivedKSet KSet, typename ...Args>
  [[nodiscard]]
  SharedKernel<kDim, KSet, Args...> makeDerivedKernel(
      const KernelParams<kDim, KSet, Args...>& params);


  const DeviceInfo* device_info_ = nullptr;
};

// Type aliases
using SharedDevice = Device::SharedPtr;
using WeakDevice = Device::WeakPtr;

} // namespace zivc

#include "device-inl.hpp"

#endif // ZIVC_DEVICE_HPP
