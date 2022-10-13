/*!
  \file context-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONTEXT_INL_HPP
#define ZIVC_CONTEXT_INL_HPP

#include "context.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <span>
#include <utility>
#include <vector>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "backend.hpp"
#include "device_info.hpp"
#include "zivc_config.hpp"
#include "auxiliary/error.hpp"
#include "auxiliary/id_data.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t Context::capacityForKernelsPerDevice() const noexcept
{
  return capacity_for_kernels_per_device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t Context::capacityForModulesPerDevice() const noexcept
{
  return capacity_for_modules_per_device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::span<const DeviceInfo* const> Context::deviceInfoList() const noexcept
{
  return *device_info_list_;
}

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
bool Context::hasBackend(const BackendType type) const noexcept
{
  [[maybe_unused]] const std::size_t index = static_cast<std::size_t>(type);
  ZIVC_ASSERT(index < kNumOfBackends, "The index is out of range.");
  const auto* backend_p = backend(type);
  const bool result = (backend_p != nullptr) && backend_p->isAvailable();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool Context::isDebugMode() const noexcept
{
  return static_cast<bool>(is_debug_mode_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
IdData Context::issueId() noexcept
{
  const int64b id = id_count_.fetch_add(1, std::memory_order::acq_rel);
  constexpr int64b threshold = 0;
  IdData id_data{(zisc::max)(id, threshold)};
  return id_data;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* Context::memoryResource() noexcept
{
  auto mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* Context::memoryResource() const noexcept
{
  auto mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
Backend* Context::backend(const BackendType type) noexcept
{
  const std::size_t index = static_cast<std::size_t>(type);
  ZIVC_ASSERT(index < kNumOfBackends, "The index is out of range.");
  SharedBackend& backend_p = backend_list_[index];
  return backend_p.get();
}

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
const Backend* Context::backend(const BackendType type) const noexcept
{
  const std::size_t index = static_cast<std::size_t>(type);
  ZIVC_ASSERT(index < kNumOfBackends, "The index is out of range.");
  const SharedBackend& backend_p = backend_list_[index];
  return backend_p.get();
}

/*!
  \details No detailed description

  \param [in] backend No description.
  */
inline
void Context::setBackend(SharedBackend&& backend) noexcept
{
  const std::size_t index = static_cast<std::size_t>(backend->type());
  ZIVC_ASSERT(index < kNumOfBackends, "The index is out of range.");
  backend_list_[index] = std::move(backend);
}

} // namespace zivc

#endif // ZIVC_CONTEXT_INL_HPP
