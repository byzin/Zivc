/*!
  \file gui_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
// Zisc
#include "zisc/memory/simple_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// GUI
#include "cli.hpp"
#include "gui_application.hpp"
#include "gui_application_options.hpp"

namespace {

/*!
  \details No detailed description

  \param [in] options No description.
  \return No description
  */
std::unique_ptr<CLI::App> makeCommandLineParser(example::CliOption* options) noexcept
{
  auto parser = std::make_unique<CLI::App>("GUI example.");
  parser->allow_extras();

  // Device option
  {
    const char* desc = "Specify the device which is used in the unit test.\n"
                       "possible values: 'cpu', 'vulkan', 'vulkan0' ... 'vulkan15'.";
    auto* option = parser->add_option("--device", options->device_name_, desc);
    auto validator = [](const std::string& device_name) noexcept
    {
      const auto id = example::getDeviceId(device_name);
      std::string result;
      if (id == example::CliOption::invalidDeviceId())
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

/*!
  \details No detailed description

  \param [in] cli_options No description.
  \param [in,out] context_options No description.
  */
void processCommandLineArgs(const example::CliOption& cli_options,
                            zivc::ContextOptions* context_options) noexcept
{
  context_options->enableDebugMode(!cli_options.is_nodebug_);
}

/*!
  \details No detailed description

  \param [in] bytes No description.
  \return No description
  */
double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1024.0 * 1024.0);
  return mb;
}

/*!
  \details No detailed description

  \param [in] context No description.
  \return No description
  */
void printWindowSurfaceType(const zivc::Context& context)
{
  const auto* vulkan_backend = zisc::reinterp<const zivc::VulkanBackend*>(
    context.backend(zivc::BackendType::kVulkan));
  using WSType = zivc::VulkanBackend::WindowSurfaceType;
  const char* ws_name = nullptr;
  switch (vulkan_backend->windowSurfaceType()) {
   case WSType::kWin32: {
    ws_name = "Win32";
    break;
   }
   case WSType::kMetal: {
    ws_name = "Metal";
    break;
   }
   case WSType::kMacOS: {
    ws_name = "macOS";
    break;
   }
   case WSType::kXcb: {
    ws_name = "Xcb";
    break;
   }
   case WSType::kXlib: {
    ws_name = "X11";
    break;
   }
   case WSType::kWayland: {
    ws_name = "Wayland";
    break;
   }
   case WSType::kNone:
   default: {
    break;
   }
  }
  std::cout << "## Vulkan window surface type: " << ws_name << std::endl;
}

/*!
  \details No detailed description

  \param [in] context_options No description.
  \param [in] gui_options No description.
  \return No description
  */
int execGuiApp(zivc::Context* context,
               example::GuiApplicationOptions& gui_options)
{
  printWindowSurfaceType(*context);

  example::SharedGuiApp app;
  int result = EXIT_FAILURE;
  // Create an app
  try {
    app = example::createGuiApp(*context, gui_options);
  }
  catch (const std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
  }

  // Execute the app
  if (app) {
    result = app->run();
    app->destroy();
  }

  return result;
}

/*!
  \details No detailed description

  \param [in] mem_resource No description.
  */
void printMemoryUsage(const zisc::SimpleMemoryResource& mem_resource) noexcept
{
  const std::string indent1 = "    ";
  std::cout << std::endl;
  std::cout << indent1 << "Host memory usage     : "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Host peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;
}

} // namespace

/*!
  \details No detailed description

  \param [in] argc No description.
  \param [in] argv No description.
  \return No description
  */
int main(int argc, char** argv)
{
  // Any custom std::pmr::memory_resource can be speicified as zivc memory allcator
  zisc::SimpleMemoryResource mem_resource;

  // Context options
  zivc::ContextOptions context_options{&mem_resource};

  // Process command line arguments
  {
    example::CliOption cli_options;
    auto cli_parser = ::makeCommandLineParser(std::addressof(cli_options));
    CLI11_PARSE(*cli_parser, argc, argv)
    ::processCommandLineArgs(cli_options, &context_options);
  }

  // Zivc options
  context_options.setContextName("GuiExample");
  context_options.setContextVersionMajor(zivc::Config::versionMajor());
  context_options.setContextVersionMinor(zivc::Config::versionMinor());
  context_options.setContextVersionPatch(zivc::Config::versionPatch());
  context_options.enableVulkanBackend(true);
  context_options.enableVulkanWSIExtension(true);

  zivc::SharedContext context;
  // Create a zivc context
  try {
    context = zivc::createContext(context_options);
  }
  catch (const std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }
  if (!context->hasBackend(zivc::BackendType::kVulkan)) {
    std::cerr << "Vulkan backend not found." << std::endl;
    return EXIT_FAILURE;
  }

  // Gui options
  example::GuiApplicationOptions gui_options{&mem_resource};
  gui_options.setWindowTitle("Zivc gui example");

  const int exec_result = ::execGuiApp(context.get(), gui_options);

  ::printMemoryUsage(mem_resource);

  return exec_result;
}
