/*!
  \file context.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONTEXT_HPP
#define ZIVC_CONTEXT_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
#include <span>
#include <vector>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "backend.hpp"
#include "device.hpp"
#include "zivc_config.hpp"
#include "auxiliary/zivc_object.hpp"

namespace zisc {

// Forward declaration
class ThreadManager;

} /* namespace zisc */

namespace zivc {

// Forward declaration
class DeviceInfo;
class ContextOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class Context : private zisc::NonCopyable<Context>
{
 public:
  //! Create an empty context
  Context() noexcept;

  //! Move a data
  Context(Context&& other) noexcept;

  //! Finalize the context
  ~Context() noexcept;


  //! Move a data
  Context& operator=(Context&& other) noexcept;


  //! Return the maximum possible number of kernels that can be created
  std::size_t capacityForKernelsPerDevice() const noexcept;

  //! Return the maximum possible number of modules that can be created
  std::size_t capacityForModulesPerDevice() const noexcept;

  //! Destroy the context
  void destroy() noexcept;

  //! Return the device info list
  std::span<const DeviceInfo* const> deviceInfoList() const noexcept;

  //! Check if the context has the backend of the given type
  bool hasBackend(const BackendType type) const noexcept;

  //! Initialize the context
  void initialize(ContextOptions& options);

  //! Check if the context is in debug mode
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

  //! Return the backend of the given type
  Backend* backend(const BackendType type) noexcept;

  //! Return the backend of the given type
  const Backend* backend(const BackendType type) const noexcept;

  //! Return the underlying thread manager which is used for cpu kernel exection
  zisc::ThreadManager& threadManager() noexcept;

  //! Return the underlying thread manager which is used for cpu kernel exection
  const zisc::ThreadManager& threadManager() const noexcept;

 private:
  //! Create a backend
  template <typename BackendType>
  void initBackend(ContextOptions& options);

  //! Create a unique device
  [[nodiscard]]
  SharedDevice createDevice(const std::size_t device_index);

  //! Move memory resource data
  void moveMemoryResource(Context& other) noexcept;

  //! Set debug mode
  void setDebugMode(const bool is_debug_mode) noexcept;

  //! Set memory resource for Zivc
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Set a backend of the given type
  void setBackend(SharedBackend&& backend) noexcept;

  //! Update the underlying device info
  void updateDeviceInfo();


  static constexpr std::size_t kNumOfBackends = 2;


  std::unique_ptr<zisc::pmr::memory_resource> default_mem_resource_;
  zisc::pmr::memory_resource* custom_mem_resource_ = nullptr;
  std::array<SharedBackend, kNumOfBackends> backend_list_;
  ZivcObject::UniqueVectorT<WeakDevice> device_list_;
  ZivcObject::UniqueVectorT<const DeviceInfo*> device_info_list_;
  std::atomic<int64b> id_count_ = 0;
  uint32b capacity_for_kernels_per_device_ = 0;
  uint32b capacity_for_modules_per_device_ = 0;
  zisc::Boolean is_debug_mode_;
  [[maybe_unused]] Padding<7> pad_;
};

// Type aliases
using SharedContext = std::shared_ptr<Context>;

//! Create a context
[[nodiscard]]
SharedContext createContext(ContextOptions& options);

} // namespace zivc

#include "context-inl.hpp"

#endif // ZIVC_CONTEXT_HPP
