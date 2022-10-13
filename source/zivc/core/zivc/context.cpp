/*!
  \file context.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "context.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/concurrency/thread_manager.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "backend.hpp"
#include "context_options.hpp"
#include "device.hpp"
#include "zivc_config.hpp"
#include "cpu/cpu_backend.hpp"
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
#include "vulkan/vulkan_backend.hpp"
#endif // ZIVC_ENABLE_VULKAN_BACKEND
#include "auxiliary/error.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
Context::Context() noexcept
{
  setDebugMode(false);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Context::Context(Context&& other) noexcept :
    default_mem_resource_{std::move(other.default_mem_resource_)},
    custom_mem_resource_{other.custom_mem_resource_},
    device_list_{std::move(other.device_list_)},
    device_info_list_{std::move(other.device_info_list_)},
    id_count_{other.id_count_.load(std::memory_order::acquire)},
    is_debug_mode_{other.is_debug_mode_}
{
  std::move(other.backend_list_.begin(),
            other.backend_list_.end(),
            backend_list_.begin());
}

/*!
  \details No detailed description
  */
Context::~Context() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
Context& Context::operator=(Context&& other) noexcept
{
  default_mem_resource_ = std::move(other.default_mem_resource_);
  custom_mem_resource_ = other.custom_mem_resource_;
  device_list_ = std::move(other.device_list_);
  device_info_list_ = std::move(other.device_info_list_);
  id_count_ = other.id_count_.load(std::memory_order::acquire);
  is_debug_mode_ = other.is_debug_mode_;
  std::move(other.backend_list_.begin(),
            other.backend_list_.end(),
            backend_list_.begin());
  return *this;
}

/*!
  \details No detailed description
  */
void Context::destroy() noexcept
{
  device_info_list_.reset();
  device_list_.reset();
  for (SharedBackend& backend_p : backend_list_)
    backend_p.reset();
  default_mem_resource_.reset();
  custom_mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] options No description.
  */
void Context::initialize(ContextOptions& options)
{
  // Clear the previous context data first 
  destroy();

  capacity_for_kernels_per_device_ = options.capacityForKernelsPerDevice();
  capacity_for_modules_per_device_ = options.capacityForModulesPerDevice();

  setMemoryResource(options.memoryResource());
  setDebugMode(options.debugModeEnabled());
  id_count_.store(0, std::memory_order::release);

  // Initialize backends
  initBackend<CpuBackend>(options);
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
  if (options.vulkanBackendEnabled())
    initBackend<VulkanBackend>(options);
#endif // ZIVC_ENABLE_VULKAN_BACKEND

  // Device list
  using DeviceList = decltype(device_list_)::element_type;
  device_list_ = zisc::pmr::allocateUnique<DeviceList>(
    memoryResource(),
    DeviceList{DeviceList::allocator_type{memoryResource()}});

  // Get device info list
  using DeviceInfoList = decltype(device_info_list_)::element_type;
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      memoryResource(),
      DeviceInfoList{DeviceInfoList::allocator_type{memoryResource()}});
  updateDeviceInfo();
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Context::queryDevice(const std::size_t device_index)
{
  if (device_list_->size() <= device_index) {
    const char* message = "The device index is out of range.";
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }
  SharedDevice device;
  // Check if the device is already created
  if (const WeakDevice d = (*device_list_)[device_index]; !d.expired()) {
    device = d.lock();
  }
  // If not, create a new device
  if (!device) {
    device = createDevice(device_index);
    (*device_list_)[device_index] = device;
  }
  return device;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::ThreadManager& Context::threadManager() noexcept
{
  Backend* backend_p = backend(BackendType::kCpu);
  auto* cpu_backend = static_cast<CpuBackend*>(backend_p);
  return cpu_backend->threadManager();
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::ThreadManager& Context::threadManager() const noexcept
{
  const Backend* backend_p = backend(BackendType::kCpu);
  const auto* cpu_backend = static_cast<const CpuBackend*>(backend_p);
  return cpu_backend->threadManager();
}

/*!
  \details No detailed description

  \tparam BackendType No description.
  \param [in,out] options No description.
  */
template <typename BackendType>
void Context::initBackend(ContextOptions& options)
{
  const zisc::pmr::polymorphic_allocator<BackendType> alloc{memoryResource()};
  SharedBackend backend_p = std::allocate_shared<BackendType>(alloc, this);
  WeakBackend own{backend_p};
  backend_p->initialize(std::move(own), options);
  setBackend(std::move(backend_p));
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Context::createDevice(const std::size_t device_index)
{
  const std::span info_list = deviceInfoList();
  if (info_list.size() <= device_index) {
    const char* message = "The device index is out of range.";
    throw SystemError{ErrorCode::kInitializationFailed, message};
  }
  const DeviceInfo* info = info_list[device_index];
  Backend* backend_p = backend(info->type());
  ZIVC_ASSERT(backend_p->isAvailable(), "The backend isn't available.");
  SharedDevice device = backend_p->createDevice(*info);
  return device;
}

/*!
  \details No detailed description

  \param [in] is_debug_mode No description.
  */
void Context::setDebugMode(const bool is_debug_mode) noexcept
{
  is_debug_mode_ = is_debug_mode;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
void Context::setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept
{
  custom_mem_resource_ = mem_resource;
  if (mem_resource != nullptr)
    default_mem_resource_.reset();
  else
    default_mem_resource_ = std::make_unique<zisc::AllocFreeResource>();
}

/*!
  \details No detailed description
  */
void Context::updateDeviceInfo()
{
  device_info_list_->clear();
  std::size_t num_of_devices = 0;
  // 
  for (const SharedBackend& backend_p : backend_list_) {
    if (backend_p && backend_p->isAvailable()) {
      backend_p->updateDeviceInfo();
      num_of_devices += backend_p->numOfDevices();
    }
  }
  //
  device_info_list_->reserve(num_of_devices);
  for (const SharedBackend& backend_p : backend_list_) {
    if (backend_p && backend_p->isAvailable()) {
      for (std::size_t i = 0; i < backend_p->numOfDevices(); ++i) {
        const DeviceInfo& info = backend_p->deviceInfo(i);
        device_info_list_->emplace_back(std::addressof(info));
      }
    }
  }
  device_list_->resize(num_of_devices);
}

/*!
  \details No detailed description

  \param [in] options No description.
  \return No description
  */
SharedContext createContext(ContextOptions& options)
{
  SharedContext context;

  // Create a context
  zisc::pmr::memory_resource* mem_resource = options.memoryResource();
  if (mem_resource != nullptr) {
    const zisc::pmr::polymorphic_allocator<Context> alloc{mem_resource};
    context = std::allocate_shared<Context>(alloc);
  }
  else {
    context = std::make_shared<Context>();
  }

  // Initialize the context
  try {
    context->initialize(options);
  }
  catch (const std::runtime_error& error) {
    context->destroy();
    context.reset();
    throw error;
  }

  return context;
}

} // namespace zivc
