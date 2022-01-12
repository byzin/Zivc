/*!
  \file googletest.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_GOOGLE_TEST_HPP
#define ZIVC_TEST_GOOGLE_TEST_HPP

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
#pragma warning(disable : 4018)
#endif // Z_VISUAL_STUDIO

// Workaround errors with X11
#undef Bool
#undef None

// GoogleTest
#include "gtest/gtest.h"

#if defined(Z_VISUAL_STUDIO)
#pragma warning(pop)
#endif // Z_VISUAL_STUDIO
#if defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_CLANG
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

#endif // ZIVC_TEST_GOOGLE_TEST_HPP
