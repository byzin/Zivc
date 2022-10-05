/*!
  \file context_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  Example usage of zivc::Context.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"

namespace {

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
std::string getBackendTypeString(const zivc::BackendType type)
{
  std::string type_string;
  switch (type) {
   case zivc::BackendType::kCpu:
    type_string = "CPU";
    break;
   case zivc::BackendType::kVulkan:
    type_string = "Vulkan";
    break;
   default:
    break;
  }
  return type_string;
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

int printContextInfo(const zivc::Context& context)
{
  const std::string indent1 = "    ";
  const std::string indent2 = indent1 + indent1;

  // CPU backend
  bool has_backend = context.hasBackend(zivc::BackendType::kCpu);
  std::cout << indent1 << "CPU backend: " << has_backend << std::endl;

  // Vulkan backend
  has_backend = context.hasBackend(zivc::BackendType::kVulkan);
  std::cout << indent1 << "Vulkan backend: " << has_backend << std::endl;

  // Print device info
  std::cout << std::endl;
  const std::span device_info_list = context.deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    std::cout << std::endl;
    const zivc::DeviceInfo* const info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getBackendTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;
    const std::array workgroup_counts = info->maxWorkGroupCount();
    std::cout << indent2 << "Max work group count: " << "("
              << workgroup_counts[0] << ", "
              << workgroup_counts[1] << ", "
              << workgroup_counts[2] << ")" << std::endl;
    std::cout << indent2 << "Work group size     : "
              << info->workGroupSize() << std::endl;
    std::cout << indent2 << "Max allocation size : "
              << ::toMegaBytes(info->maxAllocationSize()) << " MB." << std::endl;
    const std::string indent3 = indent2 + indent1;
    const std::span heap_info_list = info->heapInfoList();
    for (std::size_t index = 0; index < heap_info_list.size(); ++index) {
      std::cout << indent2 << "MemoryHeap[" << index << "]" << std::endl;
      const zivc::MemoryHeapInfo& heap_info = heap_info_list[index];
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

/*!
  \details No detailed description

  \param [in] mem_resource No description.
  */
void printMemoryUsage(const zisc::AllocFreeResource& mem_resource) noexcept
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  // Any custom std::pmr::memory_resource can be speicified as zivc memory allcator
  zisc::AllocFreeResource mem_resource;

  // Create a zivc context and print backend information
  int exec_result = EXIT_FAILURE;
  {
    // Set context options
    zivc::ContextOptions context_options{&mem_resource};
    context_options.setContextName("ContextExample");
    context_options.setContextVersionMajor(zivc::Config::versionMajor());
    context_options.setContextVersionMinor(zivc::Config::versionMinor());
    context_options.setContextVersionPatch(zivc::Config::versionPatch());
    context_options.enableVulkanBackend(true);
    context_options.enableDebugMode(true);

    // Create a zivc context
    zivc::SharedContext context;
    try {
       context = zivc::createContext(context_options);
    }
    catch (const std::runtime_error& error) {
      std::cerr << error.what() << std::endl;
      return EXIT_FAILURE;
    }

    exec_result = ::printContextInfo(*context);
  }

  ::printMemoryUsage(mem_resource);

  return exec_result;
}
