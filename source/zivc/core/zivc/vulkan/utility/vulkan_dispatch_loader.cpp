/*!
  \file vulkan_dispatch_loader.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_dispatch_loader.hpp"
// Standard C++ library
#include <cstddef>
#include <exception>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_hpp.hpp"
#include "zivc/utility/error.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(
    zisc::pmr::memory_resource* mem_resource,
    std::string_view library_name)
{
  initialize(mem_resource, library_name);
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \param [in] get_proc_addr No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(
    zisc::pmr::memory_resource* mem_resource,
    PFN_vkGetInstanceProcAddr get_proc_addr)
{
  initialize(mem_resource, get_proc_addr);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(const VulkanDispatchLoader& other) noexcept
{
  copyFrom(other);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(VulkanDispatchLoader&& other) noexcept
{
  moveFrom(other);
}

/*!
  \details No detailed description
  */
VulkanDispatchLoader::~VulkanDispatchLoader() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDispatchLoader& VulkanDispatchLoader::operator=(
    const VulkanDispatchLoader& other) noexcept
{
  if (*this != other)
    copyFrom(other);
  return *this;
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDispatchLoader& VulkanDispatchLoader::operator=(
    VulkanDispatchLoader&& other) noexcept
{
  if (*this != other)
    moveFrom(other);
  return *this;
}

/*!
  \details No detailed description

  \param [in] device No description.
  */
void VulkanDispatchLoader::set(const VkDevice& device)
{
  LoaderImplPtr impl = loaderImpl();
  if (impl != nullptr)
    impl->init(zivcvk::Device{device});
}

/*!
  \details No detailed description

  \param [in] instance No description.
  */
void VulkanDispatchLoader::set(const VkInstance& instance)
{
  LoaderImplPtr impl = loaderImpl();
  if (impl != nullptr)
    impl->init(zivcvk::Instance{instance});
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
void VulkanDispatchLoader::copyFrom(const VulkanDispatchLoader& other) noexcept
{
  if (*this != other) {
    destroy();
    dynamic_loader_ = other.dynamic_loader_;
    loader_impl_ = other.loader_impl_;
  }
}

/*!
  \details No detailed description
  */
void VulkanDispatchLoader::destroy() noexcept
{
  loader_impl_.reset();
  dynamic_loader_.reset();
}

/*!
  \details No detailed description
  */
void VulkanDispatchLoader::initialize(zisc::pmr::memory_resource* mem_resource,
                                      [[maybe_unused]] std::string_view library_name)
{
  PFN_vkGetInstanceProcAddr get_proc_addr = nullptr;
#if defined(ZIVC_DYNAMIC_VULKAN_LOADING)
  ZISC_ASSERT(mem_resource != nullptr, "The memory resource is null.");
  {
    using Loader = zivcvk::DynamicLoader;
    zisc::pmr::polymorphic_allocator<Loader> alloc{mem_resource};
    try {
      dynamic_loader_ = std::allocate_shared<Loader>(alloc, library_name.data());
    }
    catch (const std::runtime_error& error) {
      destroy();
      const char* message = error.what();
      throw SystemError{ErrorCode::kVulkanLibraryNotFound, message};
    }
  }
  ZISC_ASSERT(dynamic_loader_->success(), "Vulkan library not found.");
  const char* name = "vkGetInstanceProcAddr";
  get_proc_addr = dynamic_loader_->getProcAddress<PFN_vkGetInstanceProcAddr>(name);
#else // ZIVC_DYNAMIC_VULKAN_LOADING
  get_proc_addr = ::vkGetInstanceProcAddr;
#endif // ZIVC_DYNAMIC_VULKAN_LOADING
  initialize(mem_resource, get_proc_addr);
}

/*!
  \details No detailed description

  \param [in] get_proc_addr No description.
  */
void VulkanDispatchLoader::initialize(zisc::pmr::memory_resource* mem_resource,
                                      PFN_vkGetInstanceProcAddr get_proc_addr)
{
  ZISC_ASSERT(mem_resource != nullptr, "The memory resource is null.");
  ZISC_ASSERT(get_proc_addr != nullptr, "'get_proc_addr' is null.");
  {
    zisc::pmr::polymorphic_allocator<LoaderImpl> alloc{mem_resource};
    loader_impl_ = std::allocate_shared<LoaderImpl>(alloc, get_proc_addr);
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
void VulkanDispatchLoader::moveFrom(VulkanDispatchLoader& other) noexcept
{
  if (*this != other) {
    dynamic_loader_ = std::move(other.dynamic_loader_);
    loader_impl_ = std::move(other.loader_impl_);
  }
}

} // namespace zivc
