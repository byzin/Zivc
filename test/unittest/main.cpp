/*!
  \file main.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++library
#include <cstdlib>
#include <memory>
#include <iostream>
#include <string>
// Zivc
#include "zivc/zivc.hpp"
// Test
#include "utility/cli.hpp"
#include "utility/config.hpp"
#include "utility/googletest.hpp"
#include "utility/test.hpp"

namespace {

std::unique_ptr<CLI::App> makeCommandLineParser(ztest::CliOption* options) noexcept
{
  auto parser = std::make_unique<CLI::App>("Zivc unit test.");
  parser->allow_extras();

  // Context option
  {
    const char* desc = "Use each test's zivc context instead of global context.";
    [[maybe_unused]] auto* option = parser->add_flag("--disable-global-context", options->disable_global_context_, desc);
  }
  // Device option
  {
    const char* desc = "Specify the device which is used in the unit test.\n"
                       "possible values: 'cpu', 'vulkan', 'vulkan0' ... 'vulkan15'.";
    auto* option = parser->add_option("--device", options->device_name_, desc);
    auto validator = [](const std::string& device_name) noexcept
    {
      const auto id = ztest::getDeviceId(device_name);
      std::string result;
      if (id == ztest::Config::invalidDeviceId())
        result = "Invalid value '" + device_name + "'.";
      return result;
    };
    option->check(validator);
  }
  // Debug option
  {
    const char* desc = "Disable debug mode.";
    [[maybe_unused]] auto* option = parser->add_flag("--nodebug", options->is_nodebug_, desc);
  }

  return parser;
}

void processCommandLineArgs(const ztest::CliOption& options) noexcept
{
  auto& config = ztest::Config::globalConfig();
  config.setDeviceId(ztest::getDeviceId(options.device_name_));
  config.enableDebugMode(!options.is_nodebug_);
  config.enableGlobalContext(!options.disable_global_context_);
}

bool checkIfDeviceIsAvailable()
{
  const zivc::SharedContext context = ztest::createContext();
  if (!context) {
    std::cerr << "[Error] Context isn't available." << std::endl;
    return false;
  }
  const ztest::Config& config = ztest::Config::globalConfig();
  const zivc::SharedDevice device = context->queryDevice(config.deviceId());
  if (!device) {
    std::cerr << "[Error] Device isn't available." << std::endl;
    return false;
  }
  return true;
}

} // namespace

/*!
  \details No detailed description
  */
int main(int argc, char** argv)
{
  // Process command line arguments
  {
    ztest::CliOption cli_options;
    auto cli_parser = ::makeCommandLineParser(std::addressof(cli_options));
    CLI11_PARSE(*cli_parser, argc, argv)
    ::processCommandLineArgs(cli_options);
  }

  // Verify device
  if (!::checkIfDeviceIsAvailable()) {
    return EXIT_FAILURE;
  }

  // Run unit test
  ::testing::InitGoogleTest(&argc, argv);
  const int result = RUN_ALL_TESTS();
  // Destroy resources
  {
    auto& config = ztest::Config::globalConfig();
    config.destroyContext();
  }
  return result;
}
