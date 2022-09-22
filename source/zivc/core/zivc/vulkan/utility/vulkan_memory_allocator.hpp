/*!
  \file vulkan_memory_allocator.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_MEMORY_ALLOCATOR_HPP
#define ZIVC_VULKAN_MEMORY_ALLOCATOR_HPP

// Zivc
#include "vulkan.hpp"

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#endif // Z_GCC || Z_CLANG
#if defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weverything"
#endif // Z_CLANG
#if defined(Z_VISUAL_STUDIO)
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4189)
#pragma warning(disable : 4324)
#endif // Z_VISUAL_STUDIO

// VMA
#include "vma/vk_mem_alloc.h"
static_assert(VMA_MEMORY_BUDGET, "VK_EXT_memory_budget isn't supported in the vulkan.");

#if defined(Z_VISUAL_STUDIO)
#pragma warning(pop)
#endif // Z_VISUAL_STUDIO
#if defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_CLANG
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

#endif // ZIVC_VULKAN_MEMORY_ALLOCATOR_HPP
