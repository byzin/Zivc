/*!
  \file cli.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_EXAMPLE_CLI_HPP
#define ZIVC_EXAMPLE_CLI_HPP

// Standard C++ library
#include <array>
#include <limits>
#include <string>

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#endif // Z_GCC || Z_CLANG
#if defined(Z_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#endif // Z_GCC
#if defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weverything"
#endif // Z_CLANG

// Workaround errors with X11
#undef Success

// CLI11
#include "CLI/CLI.hpp"

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

// Zivc
#include "zivc/zivc_config.hpp"


namespace example {

/*!
  \brief No brief description

  No detailed description.
  */
struct CliOption
{
  static constexpr zivc::uint32b invalidDeviceId() noexcept
  {
    return (std::numeric_limits<zivc::uint32b>::max)();
  }

  std::string device_name_ = "cpu";
  bool is_nodebug_ = false;
  [[maybe_unused]] zivc::Padding<7> padding_;
};

//! Get the device ID
zivc::uint32b getDeviceId(std::string name) noexcept;

} // namespace example

#endif // ZIVC_EXAMPLE_CLI_HPP
