/*!
  \file vulkan_memory_allocator.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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

// VMA
#include "vk_mem_alloc.h"

#if defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_CLANG
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

#endif // ZIVC_VULKAN_MEMORY_ALLOCATOR_HPP
