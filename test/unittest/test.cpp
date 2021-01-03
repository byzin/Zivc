/*!
  \file test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++library
#include <memory>
#include <string>
// Test
#include "cli.hpp"
#include "config.hpp"
#include "googletest.hpp"

namespace {

std::unique_ptr<CLI::App> makeCommandLineParser() noexcept
{
  auto parser = std::make_unique<CLI::App>("Zivc unit test.");
  parser->allow_extras();

  ztest::Config& config = ztest::Config::globalConfig();

  // Device option
  {
    const char* desc = "Specify the device which is used in the unit test.\n"
                       "possible values: 'cpu', 'vulkan', 'vulkan0' ... 'vulkan15'.";
    auto option = parser->add_option("--device", config.deviceName(), desc, true);
    auto validator = [](const std::string& device_name) noexcept
    {
      const auto id = ztest::Config::getDeviceId(device_name);
      std::string result;
      if (id == ztest::Config::invalidDeviceId())
        result = "Invalid value '" + device_name + "'.";
      return result;
    };
    option->check(validator);
  }

  return parser;
}

} // namespace

/*!
  \details No detailed description
  */
int main(int argc, char** argv)
{
  // Process command line arguments
  auto cli_parser = ::makeCommandLineParser();
  CLI11_PARSE(*cli_parser, argc, argv)

  // Run unit test
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
