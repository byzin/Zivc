/*!
  \file kernel_example.cpp
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
#include <iostream>
#include <memory>
#include <string>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/kernel_set/kernel_set-example.hpp"

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

} // namespace

int main(int /* argc */, char** /* argv */)
{
  zisc::SimpleMemoryResource mem_resource;

  // Platform options
  zivc::PlatformOptions platform_options{&mem_resource};
  platform_options.setPlatformName("KernelExample");
  platform_options.setPlatformVersionMajor(zivc::Config::versionMajor());
  platform_options.setPlatformVersionMinor(zivc::Config::versionMinor());
  platform_options.setPlatformVersionPatch(zivc::Config::versionPatch());
  platform_options.enableVulkanSubPlatform(true);
  platform_options.enableDebugMode(true);

  // Platform
  std::cout << "Create a platform." << std::endl;
  auto platform = zivc::makePlatform(&mem_resource, platform_options);

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
  platform->updateDeviceInfoList();
  const auto& device_info_list = platform->deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    std::cout << std::endl;
    const auto info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getSubPlatformTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;
    std::cout << indent2 << "Num of heaps        : "
              << info->numOfHeaps() << std::endl;

    auto device = platform->makeDevice(i);
    std::cout << indent2 << "Num of queues       : "
              << device->numOfQueues() << std::endl;

    std::cout << std::endl;
    const std::string indent3 = indent2 + indent1;
    auto print_buffer_info = [&indent3](zivc::Buffer<int>* buffer)
    {
      using Type = zivc::Buffer<int>::Type;
      const std::size_t s = sizeof(Type) * buffer->size();
      std::cout << indent3 << "n = " << buffer->size()
                << " (" << ::toMegaBytes(s) << " MB)." << std::endl;
      std::cout << indent3
                << "DeviceLocal: " << buffer->isDeviceLocal() << ", "
                << "HostVisible: " << buffer->isHostVisible() << ", "
                << "HostCoherent: " << buffer->isHostCoherent() << ", "
                << "HostCached: " << buffer->isHostCached() << std::endl;
    };
    constexpr std::size_t size = 128 * 1024 * 1024;
    constexpr std::size_t n = size / sizeof(int);

    auto buffer1 = device->makeBuffer<int>(zivc::BufferUsage::kDeviceOnly);
    buffer1->setSize(n);
    std::cout << indent2 << "Create a device only buffer: " << std::endl;
    print_buffer_info(buffer1.get());

    auto buffer2 = device->makeBuffer<int>(zivc::BufferUsage::kHostOnly);
    buffer2->setSize(n);
    std::cout << indent2 << "Create a host only buffer: " << std::endl;
    print_buffer_info(buffer2.get());

    auto buffer3 = device->makeBuffer<int>(zivc::BufferUsage::kDeviceToHost);
    buffer3->setSize(n);
    std::cout << indent2 << "Create a device to host buffer: " << std::endl;
    print_buffer_info(buffer3.get());

    auto buffer4 = device->makeBuffer<int>(zivc::BufferUsage::kHostToDevice);
    buffer4->setSize(n);
    std::cout << indent2 << "Create a host to device buffer: " << std::endl;
    print_buffer_info(buffer4.get());

    std::cout << std::endl;
    for (std::size_t num = 0; num < info->numOfHeaps(); ++num) {
      std::cout << indent2 << "[Heap" << num << "]      Memory usage: "
                << ::toMegaBytes(device->totalMemoryUsage(num))
                << " MB." << std::endl;
      std::cout << indent2 << "[Heap" << num << "] Peak Memory usage: "
                << ::toMegaBytes(device->peakMemoryUsage(num))
                << " MB." << std::endl;
    }

    auto kernel_params = ZIVC_MAKE_KERNEL_PARAMS(example, testKernel, 1);
    auto kernel = device->makeKernel(kernel_params);

    auto launch_options = kernel->makeOptions();
    launch_options.setWorkSize({1920 * 1080});
    launch_options.setExternalSyncMode(true);
    auto result = kernel->run(*buffer1, *buffer3, n, launch_options);
    device->waitForCompletion(result.fence());
  }

  std::cout << std::endl;
  std::cout << indent1 << "Host memory usage     : "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Host peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return 0;
}
