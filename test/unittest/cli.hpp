/*!
  \file cli.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_CLI_HPP
#define ZIVC_TEST_CLI_HPP

// Standard C++ library
#include <string>

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weverything"
#endif // Z_GCC || Z_CLANG

// CLI11
#include "CLI/CLI.hpp"

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

// Zivc
#include "zivc/zivc_config.hpp"


namespace ztest {

/*!
  \brief No brief description

  No detailed description.
  */
struct CliOption
{
  std::string device_name_ = "cpu";
};

//! Get the device ID
zivc::uint32b getDeviceId(std::string name) noexcept;

} // namespace ztest

#endif // ZIVC_TEST_CLI_HPP
