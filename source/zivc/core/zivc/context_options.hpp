/*!
  \file context_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CONTEXT_OPTIONS_HPP
#define ZIVC_CONTEXT_OPTIONS_HPP

// Standard C++ library
#include <string_view>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "auxiliary/id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class ContextOptions : private zisc::NonCopyable<ContextOptions>
{
 public:
  //! Create a context options
  ContextOptions() noexcept;

  //! Create a context options with the given memory resource
  ContextOptions(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move option data
  ContextOptions(ContextOptions&& other) noexcept;


  //! Move option data
  ContextOptions& operator=(ContextOptions&& other) noexcept;


  //! Return the maximum possible number of shader kernels that can be created
  uint32b capacityForKernelsPerDevice() const noexcept;

  //! Return the maximum possible number of shader modules that can be created
  uint32b capacityForModulesPerDevice() const noexcept;

  //! Return the context name
  std::string_view contextName() const noexcept;

  //! Return the value of the major component of the context version number
  uint32b contextVersionMajor() const noexcept;

  //! Return the value of the minor component of the context version number
  uint32b contextVersionMinor() const noexcept;

  //! Return the value of the patch component of the context version number
  uint32b contextVersionPatch() const noexcept;

  //! Return the number of thread for kernel execution
  uint32b cpuNumOfThreads() const noexcept;

  //! Return the task batch size per thread
  uint32b cpuTaskBatchSize() const noexcept;

  //! Check whether the debug mode is enabled
  bool debugModeEnabled() const noexcept;

  //! Enable the debug mode
  void enableDebugMode(const bool debug_mode_enabled) noexcept;

  //! Enable the vulkan backend
  void enableVulkanBackend(const bool backend_enabled) noexcept;

  //! Enable WSI (Window System Integration) extension on Vulkan
  void enableVulkanWSIExtension(const bool extension_enabled) noexcept;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Set the maximum possible number of shader kernels that can be created
  void setCapacityForKernelsPerDevice(const uint32b cap) noexcept;

  //! Set the maximum possible number of shader modules that can be created
  void setCapacityForModulesPerDevice(const uint32b cap) noexcept;

  //! Set the context name
  void setContextName(std::string_view name) noexcept;

  //! Set the value of the major component of the context version number
  void setContextVersionMajor(const uint32b major) noexcept;

  //! Set the value of the minor component of the context version number
  void setContextVersionMinor(const uint32b minor) noexcept;

  //! Set the value of the patch component of the context version number
  void setContextVersionPatch(const uint32b patch) noexcept;

  //! Set the number of threads for kernel execution
  void setCpuNumOfThreads(const uint32b num_of_threads) noexcept;

  //! Set the task batch size per thread
  void setCpuTaskBatchSize(const uint32b task_batch_size) noexcept;

  //! Set memory resource for Zivc
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Set a ptr of a VkInstance object which is used instead of internal instance
  void setVulkanInstancePtr(void* instance_ptr) noexcept;

  //! Set the name of vulkan library
  void setVulkanLibraryName(std::string_view name) noexcept;

  //! Set a ptr of a PFN_vkGetInstanceProcAddr which is used instead of internal function
  void setVulkanGetProcAddrPtr(void* get_proc_addr_ptr) noexcept;

  //! Return a ptr of a VkInstance object
  void* vulkanInstancePtr() noexcept;

  //! Return the name of vulkan library
  std::string_view vulkanLibraryName() const noexcept;

  //! Return a ptr of a PFN_vkGetInstanceProcAddr
  void* vulkanGetProcAddrPtr() noexcept;

  //! Check whether the vulkan backend is enabled
  bool vulkanBackendEnabled() const noexcept;

  //! Check whether WSI (Window System Integration) extension is enabled
  bool vulkanWSIExtensionEnabled() const noexcept;

 private:
  //! Initialize options
  void initialize() noexcept;


  IdData::NameT context_name_;
  IdData::NameT vulkan_library_name_;
  zisc::pmr::memory_resource* mem_resource_;
  uint32b context_version_major_;
  uint32b context_version_minor_;
  uint32b context_version_patch_;
  uint32b capacity_for_kernels_per_device;
  uint32b capacity_for_modules_per_device;
  uint32b cpu_num_of_threads_;
  uint32b cpu_task_batch_size_;
  zisc::Boolean debug_mode_enabled_; //!< Enable debugging in Zivc
  zisc::Boolean vulkan_backend_enabled_;
  zisc::Boolean vulkan_wsi_extension_enabled_;
  [[maybe_unused]] Padding<1> pad_;
  void* vulkan_instance_ptr_ = nullptr;
  void* vulkan_get_proc_addr_ptr_ = nullptr;
};

} // namespace zivc

#include "context_options-inl.hpp"

#endif // ZIVC_CONTEXT_OPTIONS_HPP
