/*!
  \file device_example.cpp
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
#include "zisc/utility.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"

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

/*!
  \details No detailed description

  \param [in] context No description.
  \return No description
  */
int doBufferExample(zivc::Context& context)
{
  const std::string indent1 = "    ";
  const std::string indent2 = indent1 + indent1;

  // CPU backend
  const bool has_cpu_backend = context.hasBackend(zivc::BackendType::kCpu);
  std::cout << indent1 << "CPU backend: " << has_cpu_backend << std::endl;

  // Vulkan backend
  const bool has_vulkan_backend = context.hasBackend(zivc::BackendType::kVulkan);
  std::cout << indent1 << "Vulkan backend: " << has_vulkan_backend << std::endl;

  // Print device info
  std::cout << std::endl;
  const auto& device_info_list = context.deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    if (i == 1 || i == 2)
      continue;
    std::cout << std::endl;
    const auto* info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getBackendTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;

    // Query a zivc device by the device index.
    zivc::SharedDevice device;
    try {
      device = context.queryDevice(i);
    }
    catch (const std::runtime_error& error) {
      std::cerr << error.what() << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << indent2 << "Num of queues       : "
              << device->numOfQueues() << std::endl;

    // Print buffer info
    using zivc::int32b;
    std::cout << std::endl;
    const std::string indent3 = indent2 + indent1;
    auto print_buffer_info = [&indent3](const zivc::Buffer<int32b>& buffer)
    {
      using Type = zivc::Buffer<int32b>::Type; // The alias of the element type
      const std::size_t s = sizeof(Type) * buffer.size();
      std::cout << indent3 << "n = " << buffer.size()
                << " (" << ::toMegaBytes(s) << " MB)." << std::endl;
      std::cout << indent3
                // Device local memory is most efficient for device access
                << "DeviceLocal : " << buffer.isDeviceLocal() << ", "
                // Allocated memory can be mapped for host access and writable
                << "HostVisible : " << buffer.isHostVisible() << ", "
                // Host coherent and cached memory is random accessible for host access
                << "HostCoherent: " << buffer.isHostCoherent() << ", "
                << "HostCached  : " << buffer.isHostCached() << std::endl;
    };

    // Create buffers
    std::cout << indent2
              << "Create a buffer (preference device memory): "
              << std::endl;
    auto buffer1 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
    buffer1->setSize(1);
    print_buffer_info(*buffer1);

    std::cout << indent2
              << "Create a buffer (preference device memory, host writable): "
              << std::endl;
    auto buffer2 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice,
                                                 zivc::BufferFlag::kSequentialWritable});
    buffer2->setSize(1);
    print_buffer_info(*buffer2);

    std::cout << indent2
              << "Create a buffer (preference device memory, host random accessible: "
              << std::endl;
    auto buffer3 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice,
                                                 zivc::BufferFlag::kRandomAccessible});
    buffer3->setSize(1);
    print_buffer_info(*buffer3);

    std::cout << indent2
              << "Create a buffer (preference host memory): "
              << std::endl;
    auto buffer4 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost});
    buffer4->setSize(1);
    print_buffer_info(*buffer4);

    std::cout << indent2
              << "Create a buffer (preference host memory, host writable): "
              << std::endl;
    auto buffer5 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kSequentialWritable});
    buffer5->setSize(1);
    print_buffer_info(*buffer5);

    std::cout << indent2
              << "Create a buffer (preference host memory, host writable): "
              << std::endl;
    auto buffer6 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                 zivc::BufferFlag::kRandomAccessible});
    buffer6->setSize(1);
    print_buffer_info(*buffer6);

    //! \todo Buffer examples
    constexpr std::size_t size = 128 * 1024 * 1024;
    constexpr std::size_t n = size / sizeof(int32b);

    // Print device memory usage
    std::cout << std::endl;
    const auto& heap_info_list = info->heapInfoList();
    for (std::size_t index = 0; index < heap_info_list.size(); ++index) {
      const auto& usage = device->memoryUsage(index);
      std::cout << indent2 << "[Heap" << index << "]      Memory usage: "
                << ::toMegaBytes(usage.total()) << " MB." << std::endl;
      std::cout << indent2 << "[Heap" << index << "] Peak Memory usage: "
                << ::toMegaBytes(usage.peak()) << " MB." << std::endl;
    }
  }
  return EXIT_SUCCESS;
}

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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  zisc::SimpleMemoryResource mem_resource;

  // Do buffer operations
  int exec_result = EXIT_FAILURE;
  {
    // Set context options
    zivc::ContextOptions context_options{&mem_resource};
    context_options.setContextName("BufferExample");
    context_options.setContextVersionMajor(zivc::Config::versionMajor());
    context_options.setContextVersionMinor(zivc::Config::versionMinor());
    context_options.setContextVersionPatch(zivc::Config::versionPatch());
    context_options.enableVulkanBackend(true);
    context_options.enableDebugMode(true);

    // Create zivc context
    zivc::SharedContext context;
    try {
      context = zivc::createContext(context_options);
    }
    catch (const std::runtime_error& error) {
      std::cerr << error.what() << std::endl;
      return EXIT_FAILURE;
    }

    exec_result = ::doBufferExample(*context);
  }

  ::printMemoryUsage(mem_resource);

  return exec_result;
}
