/*!
  \file vulkan_initialization.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// VMA configurations
#if defined(ZIVC_ENABLE_VMA_IMPLEMENTATION)
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_IMPLEMENTATION 1
#define VMA_STATS_STRING_ENABLED 0
// VMA Debug
#if defined(Z_DEBUG_MODE)
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1
#define VMA_DEBUG_MARGIN 256
#define VMA_DEBUG_DETECT_CORRUPTION 1
#else // release
//! \todo Set optimization configurations
//static_assert(false, "Set optimization configurations.");
#endif // Z_DEBUG_MODE
#endif // ZIVC_ENABLE_VMA_IMPLEMENTATION

// Standard C++ library
#include <memory>
// Zivc
#include "vulkan_memory_allocator.hpp"
#include "../../zivc_config.hpp"
