/*!
  \file platform_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_PLATFORM_OPTIONS_HPP
#define ZIVC_PLATFORM_OPTIONS_HPP

// Standard C++ library
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class PlatformOptions : private zisc::NonCopyable<PlatformOptions>
{
 public:
  //! Create a platform options
  PlatformOptions() noexcept;

  //! Create a platform options with the given memory resource
  PlatformOptions(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move option data
  PlatformOptions(PlatformOptions&& other) noexcept;


  //! Move option data
  PlatformOptions& operator=(PlatformOptions&& other) noexcept;


  //! Return the number of thread for kernel execution
  uint32b cpuNumOfThreads() const noexcept;

  //! Return the task batch size per thread
  uint32b cpuTaskBatchSize() const noexcept;

  //! Enable the debug mode
  void enableDebugMode(const bool debug_mode_enabled) noexcept;

  //! Enable the vulkan sub-platform
  void enableVulkanSubPlatform(const bool sub_platform_enabled) noexcept;

  //! Enable WSI (Window System Integration) extension on Vulkan
  void enableVulkanWSIExtension(const bool extension_enabled) noexcept;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the platform name
  std::string_view platformName() const noexcept;

  //! Return the value of the major component of the platform version number
  uint32b platformVersionMajor() const noexcept;

  //! Return the value of the minor component of the platform version number
  uint32b platformVersionMinor() const noexcept;

  //! Return the value of the patch component of the platform version number
  uint32b platformVersionPatch() const noexcept;

  //! Check whether the debug mode is enabled
  bool debugModeEnabled() const noexcept;

  //! Set the number of threads for kernel execution
  void setCpuNumOfThreads(const uint32b num_of_threads) noexcept;

  //! Set the task batch size per thread
  void setCpuTaskBatchSize(const uint32b task_batch_size) noexcept;

  //! Set memory resource for Zivc
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Set the platform name
  void setPlatformName(std::string_view name) noexcept;

  //! Set the value of the major component of the platform version number
  void setPlatformVersionMajor(const uint32b major) noexcept;

  //! Set the value of the minor component of the platform version number
  void setPlatformVersionMinor(const uint32b minor) noexcept;

  //! Set the value of the patch component of the platform version number
  void setPlatformVersionPatch(const uint32b patch) noexcept;

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

  //! Check whether the vulkan sub-platform is enabled
  bool vulkanSubPlatformEnabled() const noexcept;

  //! Check whether WSI (Window System Integration) extension is enabled
  bool vulkanWSIExtensionEnabled() const noexcept;

 private:
  //! Initialize options
  void initialize() noexcept;


  IdData::NameType platform_name_;
  IdData::NameType vulkan_library_name_;
  zisc::pmr::memory_resource* mem_resource_;
  uint32b platform_version_major_;
  uint32b platform_version_minor_;
  uint32b platform_version_patch_;
  int32b debug_mode_enabled_; //!< Enable debugging in Zivc
  uint32b cpu_num_of_threads_ = 0;
  uint32b cpu_task_batch_size_ = 32;
  int32b vulkan_sub_platform_enabled_;
  int32b vulkan_wsi_extension_enabled_;
  void* vulkan_instance_ptr_ = nullptr;
  void* vulkan_get_proc_addr_ptr_ = nullptr;
};

} // namespace zivc

#include "platform_options-inl.hpp"

#endif // ZIVC_PLATFORM_OPTIONS_HPP
