/*!
  \file vulkan_dispatch_loader.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DISPATCH_LOADER_HPP
#define ZIVC_VULKAN_DISPATCH_LOADER_HPP

// Standard C++ library
#include <memory>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan.hpp"

namespace zivcvk {

// Forward declaration
class DispatchLoaderDynamic;
class DynamicLoader;

} // namespace zivcvk

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanDispatchLoader
{
 public:
  // Type aliases
  using LoaderImpl = zivcvk::DispatchLoaderDynamic;
  using ConstLoaderImpl = std::add_const_t<LoaderImpl>;
  using LoaderImplPtr = std::add_pointer_t<LoaderImpl>;
  using ConstLoaderImplPtr = std::add_pointer_t<ConstLoaderImpl>;


  //! Initialize the dispatch loader
  VulkanDispatchLoader(zisc::pmr::memory_resource* mem_resource,
                       std::string_view library_name);

  //! Initialize the dispatch loader
  VulkanDispatchLoader(zisc::pmr::memory_resource* mem_resource,
                       PFN_vkGetInstanceProcAddr get_proc_addr);

  //! Copy a data
  VulkanDispatchLoader(const VulkanDispatchLoader& other) noexcept;

  //! Move a data
  VulkanDispatchLoader(VulkanDispatchLoader&& other) noexcept;

  //! Finalize the dispatch loader
  ~VulkanDispatchLoader() noexcept;


  //! Copy a data
  VulkanDispatchLoader& operator=(const VulkanDispatchLoader& other) noexcept;

  //! Move a data
  VulkanDispatchLoader& operator=(VulkanDispatchLoader&& other) noexcept;

  //! Return the underlying getter of vulkan device proc addr
  PFN_vkGetDeviceProcAddr deviceProcAddr() const noexcept;

  //! Return the underlying getter of vulkan instance proc addr
  PFN_vkGetInstanceProcAddr instanceProcAddr() const noexcept;

  //! Check if the loader is available
  bool isAvailable() const noexcept;

  //! Check if the loader is dispatchable for the device
  bool isDispatchableForDevice() const noexcept;

  //! Check if the loader is dispatchable for the instance
  bool isDispatchableForInstance() const noexcept;

  //! Return the underlying vulkan dispatch loader
  LoaderImplPtr loaderImpl() noexcept;

  //! Return the underlying vulkan dispatch loader
  ConstLoaderImplPtr loaderImpl() const noexcept;

  //! The dispatcher obtains the function pointers from the given device
  void set(const VkDevice& device);

  //! The dispatcher obtains the function pointers from the given instance
  void set(const VkInstance& instance);

 private:
  //! Copy a data
  void copyFrom(const VulkanDispatchLoader& other) noexcept;

  //! Destroy all underlying data
  void destroy() noexcept;

  //! Initialize the dispatch loader with dynamic vulkan library
  void initialize(zisc::pmr::memory_resource* mem_resource,
                  std::string_view library_name);

  //! Initialize the dispatch loader
  void initialize(zisc::pmr::memory_resource* mem_resource,
                  PFN_vkGetInstanceProcAddr get_proc_addr);

  //! Move a data
  void moveFrom(VulkanDispatchLoader& other) noexcept;


  std::shared_ptr<zivcvk::DynamicLoader> dynamic_loader_ = nullptr;
  std::shared_ptr<LoaderImpl> loader_impl_ = nullptr;
};

//! Check if the given two loaders are same
bool operator==(const VulkanDispatchLoader& lhs,
                const VulkanDispatchLoader& rhs) noexcept;

//! Check if the given two loaders arenot same
bool operator!=(const VulkanDispatchLoader& lhs,
                const VulkanDispatchLoader& rhs) noexcept;

} // namespace zivc

#include "vulkan_dispatch_loader-inl.hpp"

#endif // ZIVC_VULKAN_DISPATCH_LOADER_HPP
