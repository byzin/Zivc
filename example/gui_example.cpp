/*!
  \file gui_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
// Zisc
#include "zisc/memory/simple_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// GUI
#include "gui_application.hpp"
#include "gui_application_options.hpp"

namespace {

double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1024.0 * 1024.0);
  return mb;
}

void printWindowSurfaceType(const zivc::Platform& platform)
{
  const auto* vulkan_platform = zisc::reinterp<const zivc::VulkanSubPlatform*>(
    platform.subPlatform(zivc::SubPlatformType::kVulkan));
  using WSType = zivc::VulkanSubPlatform::WindowSurfaceType;
  const char* ws_name = nullptr;
  switch (vulkan_platform->windowSurfaceType()) {
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

int execGuiApp(zivc::PlatformOptions& platform_options,
               example::GuiApplicationOptions& gui_options)
{
  zivc::SharedPlatform platform;
  // Create a zivc platform
  try {
    platform = zivc::makePlatform(platform_options);
  }
  catch (const std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }
  if (!platform->hasSubPlatform(zivc::SubPlatformType::kVulkan)) {
    std::cerr << "Vulkan sub-platform not found." << std::endl;
    return EXIT_FAILURE;
  }

  printWindowSurfaceType(*platform);

  example::SharedGuiApp app;
  int result = EXIT_FAILURE;
  // Create an app
  try {
    app = example::makeGuiApp(*platform, gui_options);
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

} // namespace

/*!
  \details No detailed description

  \param [in] argc No description.
  \param [in] argv No description.
  \return No description
  */
int main(int argc, char** argv)
{
  zisc::SimpleMemoryResource mem_resource;

  // Zivc options
  zivc::PlatformOptions platform_options{&mem_resource};
  platform_options.setPlatformName("GuiExample");
  platform_options.setPlatformVersionMajor(zivc::Config::versionMajor());
  platform_options.setPlatformVersionMinor(zivc::Config::versionMinor());
  platform_options.setPlatformVersionPatch(zivc::Config::versionPatch());
  platform_options.enableVulkanSubPlatform(true);
  platform_options.enableVulkanWSIExtension(true);
  platform_options.enableDebugMode(true);
  // Gui options
  example::GuiApplicationOptions gui_options{&mem_resource};
  gui_options.setWindowTitle("Zivc gui example");

  const int result = ::execGuiApp(platform_options, gui_options);

  const std::string indent1 = "    ";
  std::cout << std::endl;
  std::cout << indent1 << "Host memory usage     : "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Host peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return result;
}
