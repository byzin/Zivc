/*!
  \file platform_example.cpp
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
#include <memory>
#include <stdexcept>
#include <string>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"

namespace {

std::string getSubPlatformTypeString(const zivc::SubPlatformType type)
{
  std::string type_string;
  switch (type) {
   case zivc::SubPlatformType::kCpu:
    type_string = "CPU";
    break;
   case zivc::SubPlatformType::kVulkan:
    type_string = "Vulkan";
    break;
  }
  return type_string;
}

double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1024.0 * 1024.0);
  return mb;
}

int showPlatformInfo(zivc::PlatformOptions& platform_options)
{
  // Make a platform
  std::cout << "Create a platform." << std::endl;
  zivc::SharedPlatform platform;
  try {
     platform = zivc::makePlatform(platform_options);
  }
  catch (const std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  const std::string indent1 = "    ";
  const std::string indent2 = indent1 + indent1;
  // CPU sub-platform
  bool has_subplatform = platform->hasSubPlatform(zivc::SubPlatformType::kCpu);
  std::cout << indent1 << "CPU sub-platform: " << has_subplatform << std::endl;

  // Vulkan sub-platform
  has_subplatform = platform->hasSubPlatform(zivc::SubPlatformType::kVulkan);
  std::cout << indent1 << "Vulkan sub-platform: " << has_subplatform << std::endl;

  // Show device info
  std::cout << std::endl;
  const auto& device_info_list = platform->deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    std::cout << std::endl;
    const auto* const info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getSubPlatformTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;
    const auto workgroup_counts = info->maxWorkGroupCount();
    std::cout << indent2 << "Max work group count: " << "("
              << workgroup_counts[0] << ", "
              << workgroup_counts[1] << ", "
              << workgroup_counts[2] << ")" << std::endl;
    std::cout << indent2 << "Work group size     : "
              << info->workGroupSize() << std::endl;
    std::cout << indent2 << "Max allocation size : "
              << ::toMegaBytes(info->maxAllocationSize()) << " MB." << std::endl;
    const std::string indent3 = indent2 + indent1;
    const auto& heap_info_list = info->heapInfoList();
    for (std::size_t index = 0; index < heap_info_list.size(); ++index) {
      std::cout << indent2 << "MemoryHeap[" << index << "]" << std::endl;
      const auto& heap_info = heap_info_list[index];
      std::cout << indent3 << "Device local    : "
                << heap_info.isDeviceLocal() << std::endl;
      std::cout << indent3 << "Total memory    : "
                << ::toMegaBytes(heap_info.totalSize()) << " MB." << std::endl;
      std::cout << indent3 << "Available memory: "
                << ::toMegaBytes(heap_info.availableSize()) << " MB." << std::endl;
    }
  }
  return EXIT_SUCCESS;
}

} // namespace

int main(int /* argc */, char** /* argv */)
{
  zisc::SimpleMemoryResource mem_resource;

  // Set platform options
  zivc::PlatformOptions platform_options{&mem_resource};
  platform_options.setPlatformName("PlatformExample");
  platform_options.setPlatformVersionMajor(zivc::Config::versionMajor());
  platform_options.setPlatformVersionMinor(zivc::Config::versionMinor());
  platform_options.setPlatformVersionPatch(zivc::Config::versionPatch());
  platform_options.enableVulkanSubPlatform(true);
  platform_options.enableDebugMode(true);

  const int result = ::showPlatformInfo(platform_options);

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
