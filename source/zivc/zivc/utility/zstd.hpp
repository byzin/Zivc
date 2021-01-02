/*!
  \file zstd.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZSTD_HPP
#define ZIVC_ZSTD_HPP

#if defined(ZIVC_BAKE_KERNELS)

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation-unknown-command"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // Z_GCC || Z_CLANG

// Zstandard
#define ZSTD_STATIC_LINKING_ONLY
#include <zstd.h>

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

#endif // ZIVC_BAKE_KERNELS

#endif // ZIVC_ZSTD_HPP
