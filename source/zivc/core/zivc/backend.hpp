/*!
  \file backend.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BACKEND_HPP
#define ZIVC_BACKEND_HPP

// Standard C++ library
#include <atomic>
#include <memory>
#include <span>
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
class ContextOptions;
class Context;

/*!
  \brief No brief description

  No detailed description.
  */
class Backend : public ZivcObject
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Backend>;
  using WeakPtr = std::weak_ptr<Backend>;


  //! Create an empty backend
  Backend(Context* context) noexcept;

  //! Finalize the backend
  ~Backend() noexcept override;


  //! Return the underlying device info
  virtual const DeviceInfo& deviceInfo(const std::size_t index) const noexcept = 0;

  //! Destroy the backend
  void destroy() noexcept;

  //! Initialize the backend
  void initialize(WeakPtr&& own, ContextOptions& options);

  //! Check if the backend is available
  virtual bool isAvailable() const noexcept = 0;

  //! Check if the backend is in debug mode
  bool isDebugMode() const noexcept override;

  //! Issue an ID of an object
  [[nodiscard]]
  IdData issueId() noexcept override;

  //! Create a unique device
  [[nodiscard]]
  virtual SharedDevice createDevice(const DeviceInfo& device_info) = 0;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() override;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept override;

  //! Return the number of available devices 
  virtual std::size_t numOfDevices() const noexcept = 0;

  //! Update the underlying device info
  virtual void updateDeviceInfo() = 0;

 protected:
  //! Destroy the backend
  virtual void destroyData() noexcept = 0;

  //! Initialize the backend
  virtual void initData(ContextOptions& options) = 0;

 private:
  Context* context_ = nullptr;
};

// Type aliases
using SharedBackend = Backend::SharedPtr;
using WeakBackend = Backend::WeakPtr;

} // namespace zivc

#endif // ZIVC_BACKEND_HPP
