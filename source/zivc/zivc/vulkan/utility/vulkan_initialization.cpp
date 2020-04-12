/*!
  \file vulkan_initialization.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// VMA configurations
#if defined(ZIVC_ENABLE_VMA_IMPLEMENTATION)
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_IMPLEMENTATION 1
// VMA Debug
#if defined(Z_DEBUG_MODE)
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1
#define VMA_DEBUG_MARGIN 32
#define VMA_DEBUG_DETECT_CORRUPTION 1
#else
static_assert(false);
#endif // Z_DEBUG_MODE
#endif // ZIVC_ENABLE_VMA_IMPLEMENTATION

// Standard C++ library
#include <memory>
// VMA
#include "vk_mem_alloc.h"
// Zivc
#include "zivc/zivc_config.hpp"

namespace {

void** getDataPtr(VmaAllocator alloc) noexcept
{
  void** ptr = nullptr;
#if defined(ZIVC_ENABLE_VMA_IMPLEMENTATION)
  auto& props = alloc->m_MemProps;
  auto& unused_heap = props.memoryHeaps[VK_MAX_MEMORY_HEAPS - 1];
  ptr = reinterpret_cast<void**>(std::addressof(unused_heap));
#endif // ZIVC_ENABLE_VMA_IMPLEMENTATION
  return ptr;
}

}

namespace zivc {

//! Return the user data of the given allocator
void* getUserData(VmaAllocator alloc) noexcept;

//! Set a user data to the given allocator
void setUserData(VmaAllocator alloc, void* data) noexcept;

void* getUserData(VmaAllocator alloc) noexcept
{
  void** ptr = ::getDataPtr(alloc);
  return *ptr;
}

void setUserData(VmaAllocator alloc, void* data) noexcept
{
  void** ptr = ::getDataPtr(alloc);
  *ptr = data;
}

} // namespace zivc
