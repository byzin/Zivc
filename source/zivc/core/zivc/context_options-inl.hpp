/*!
  \file context_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONTEXT_OPTIONS_INL_HPP
#define ZIVC_CONTEXT_OPTIONS_INL_HPP

#include "context_options.hpp"
// Standard C++ library
#include <cstring>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
ContextOptions::ContextOptions() noexcept :
    ContextOptions(nullptr)
{
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
inline
ContextOptions::ContextOptions(zisc::pmr::memory_resource* mem_resource) noexcept :
    context_name_{{"Context"}},
    vulkan_library_name_{{""}},
    mem_resource_{mem_resource},
    context_version_major_{0},
    context_version_minor_{0},
    context_version_patch_{0},
    cpu_num_of_threads_{0},
    cpu_task_batch_size_{32},
    vulkan_instance_ptr_{nullptr},
    vulkan_get_proc_addr_ptr_{nullptr}
{
  initialize();
  setMemoryResource(mem_resource);
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
inline
ContextOptions::ContextOptions(ContextOptions&& other) noexcept :
    context_name_{std::move(other.context_name_)},
    vulkan_library_name_{std::move(other.vulkan_library_name_)},
    mem_resource_{std::move(other.mem_resource_)},
    context_version_major_{other.context_version_major_},
    context_version_minor_{other.context_version_minor_},
    context_version_patch_{other.context_version_patch_},
    cpu_num_of_threads_{other.cpu_num_of_threads_},
    cpu_task_batch_size_{other.cpu_task_batch_size_},
    debug_mode_enabled_{other.debug_mode_enabled_},
    vulkan_backend_enabled_{other.vulkan_backend_enabled_},
    vulkan_wsi_extension_enabled_{other.vulkan_wsi_extension_enabled_},
    vulkan_instance_ptr_{other.vulkan_instance_ptr_},
    vulkan_get_proc_addr_ptr_{other.vulkan_get_proc_addr_ptr_}
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
inline
ContextOptions& ContextOptions::operator=(ContextOptions&& other) noexcept
{
  context_name_ = std::move(other.context_name_);
  vulkan_library_name_ = std::move(other.vulkan_library_name_);
  mem_resource_ = std::move(other.mem_resource_);
  context_version_major_ = other.context_version_major_;
  context_version_minor_ = other.context_version_minor_;
  context_version_patch_ = other.context_version_patch_;
  cpu_num_of_threads_ = other.cpu_num_of_threads_;
  cpu_task_batch_size_ = other.cpu_task_batch_size_;
  debug_mode_enabled_ = other.debug_mode_enabled_;
  vulkan_backend_enabled_ = other.vulkan_backend_enabled_;
  vulkan_wsi_extension_enabled_ = other.vulkan_wsi_extension_enabled_;
  vulkan_instance_ptr_ = other.vulkan_instance_ptr_;
  vulkan_get_proc_addr_ptr_ = other.vulkan_get_proc_addr_ptr_;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b ContextOptions::cpuNumOfThreads() const noexcept
{
  return cpu_num_of_threads_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b ContextOptions::cpuTaskBatchSize() const noexcept
{
  return cpu_task_batch_size_;
}

/*!
  \details No detailed description

  \param [in] debug_mode_enabled No description.
  */
inline
void ContextOptions::enableDebugMode(const bool debug_mode_enabled) noexcept
{
  debug_mode_enabled_ = debug_mode_enabled;
}

/*!
  \details No detailed description

  \param [in] backend_enabled No description.
  */
inline
void ContextOptions::enableVulkanBackend(const bool backend_enabled)
    noexcept
{
  vulkan_backend_enabled_ = backend_enabled;
}

/*!
  \details No detailed description

  \param [in] extension_enabled No description.
  */
inline
void ContextOptions::enableVulkanWSIExtension(const bool extension_enabled)
    noexcept
{
  vulkan_wsi_extension_enabled_ = extension_enabled;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* ContextOptions::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* ContextOptions::memoryResource() const noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view ContextOptions::contextName() const noexcept
{
  const std::string_view name{context_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b ContextOptions::contextVersionMajor() const noexcept
{
  return context_version_major_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b ContextOptions::contextVersionMinor() const noexcept
{
  return context_version_minor_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b ContextOptions::contextVersionPatch() const noexcept
{
  return context_version_patch_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ContextOptions::debugModeEnabled() const noexcept
{
  return static_cast<bool>(debug_mode_enabled_);
}

/*!
  \details No detailed description

  \param [in] num_of_threads No description.
  */
inline
void ContextOptions::setCpuNumOfThreads(const uint32b num_of_threads) noexcept
{
  cpu_num_of_threads_ = num_of_threads;
}

/*!
  \details No detailed description

  \param [in] task_batch_size No description.
  */
inline
void ContextOptions::setCpuTaskBatchSize(const uint32b task_batch_size) noexcept
{
  cpu_task_batch_size_ = task_batch_size;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
inline
void ContextOptions::setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept
{
  mem_resource_ = mem_resource;
}

/*!
  \details No detailed description

  \param [in] name No description.
  */
inline
void ContextOptions::setContextName(std::string_view name) noexcept
{
  copyStr(name, context_name_.data());
}

/*!
  \details No detailed description

  \param [in] major No description.
  */
inline
void ContextOptions::setContextVersionMajor(const uint32b major) noexcept
{
  context_version_major_ = major;
}

/*!
  \details No detailed description

  \param [in] minor No description.
  */
inline
void ContextOptions::setContextVersionMinor(const uint32b minor) noexcept
{
  context_version_minor_ = minor;
}

/*!
  \details No detailed description

  \param [in] patch No description.
  */
inline
void ContextOptions::setContextVersionPatch(const uint32b patch) noexcept
{
  context_version_patch_ = patch;
}

/*!
  \details No detailed description

  \param [in] instance_ptr No description.
  */
inline
void ContextOptions::setVulkanInstancePtr(void* instance_ptr) noexcept
{
  vulkan_instance_ptr_ = instance_ptr;
}

/*!
  \details No detailed description

  \param [in] name No description.
  */
inline
void ContextOptions::setVulkanLibraryName(std::string_view name) noexcept
{
  copyStr(name, vulkan_library_name_.data());
}

/*!
  \details No detailed description

  \param [in] get_proc_addr_ptr No description.
  */
inline
void ContextOptions::setVulkanGetProcAddrPtr(void* get_proc_addr_ptr) noexcept
{
  vulkan_get_proc_addr_ptr_ = get_proc_addr_ptr;
}

/*!
  \details No detailed description

  \return No description
  */
inline
void* ContextOptions::vulkanInstancePtr() noexcept
{
  return vulkan_instance_ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view ContextOptions::vulkanLibraryName() const noexcept
{
  const std::string_view name{vulkan_library_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
void* ContextOptions::vulkanGetProcAddrPtr() noexcept
{
  return vulkan_get_proc_addr_ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ContextOptions::vulkanBackendEnabled() const noexcept
{
  return static_cast<bool>(vulkan_backend_enabled_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ContextOptions::vulkanWSIExtensionEnabled() const noexcept
{
  return static_cast<bool>(vulkan_wsi_extension_enabled_);
}

/*!
  \details No detailed description
  */
inline
void ContextOptions::initialize() noexcept
{
#if defined(Z_DEBUG_MODE)
  enableDebugMode(true);
#else // Z_DEBUG_MODE
  enableDebugMode(false);
#endif // Z_DEBUG_MODE
  enableVulkanBackend(true);
  enableVulkanWSIExtension(false);
}

} // namespace zivc

#endif // ZIVC_CONTEXT_OPTIONS_INL_HPP
