/*!
  \file device_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
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

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] indent No description.
  \param [in] buffer No description.
  */
template <typename Type>
void printBufferInfo(const std::string_view indent, const zivc::Buffer<Type>& buffer)
{
  const std::size_t s = buffer.sizeInBytes();
  std::cout << indent << "n = " << buffer.size()
            << " (" << ::toMegaBytes(s) << " MB)." << std::endl;
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
  const std::span device_info_list = context.deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    std::cout << std::endl;
    const zivc::DeviceInfo* info = device_info_list[i];
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
    using zivc::int64b;
    std::cout << std::endl;
    const std::string indent3 = indent2 + indent1;
    const auto print_memory_type = [&indent3](const zivc::Buffer<int64b>& buffer)
    {
      std::cout << indent3
                << "Heap index  : " << buffer.heapIndex() << std::endl;
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
    using Usage = zivc::BufferUsage;
    using Flag = zivc::BufferFlag;
    std::cout << indent2
              << "Create a buffer (preference device memory): "
              << std::endl;
    const zivc::SharedBuffer buffer1 = device->createBuffer<int64b>({Usage::kPreferDevice});
    print_memory_type(*buffer1);

    std::cout << indent2
              << "Create a buffer (preference device memory, host writable): "
              << std::endl;
    const zivc::SharedBuffer buffer2 = device->createBuffer<int64b>({Usage::kPreferDevice,
                                                                     Flag::kSequentialWritable});
    print_memory_type(*buffer2);

    std::cout << indent2
              << "Create a buffer (preference device memory, host random accessible: "
              << std::endl;
    const zivc::SharedBuffer buffer3 = device->createBuffer<int64b>({Usage::kPreferDevice,
                                                                     Flag::kRandomAccessible});
    print_memory_type(*buffer3);

    std::cout << indent2
              << "Create a buffer (preference host memory): "
              << std::endl;
    const zivc::SharedBuffer buffer4 = device->createBuffer<int64b>({Usage::kPreferHost});
    print_memory_type(*buffer4);

    std::cout << indent2
              << "Create a buffer (preference host memory, host writable): "
              << std::endl;
    const zivc::SharedBuffer buffer5 = device->createBuffer<int64b>({Usage::kPreferHost,
                                                                     Flag::kSequentialWritable});
    print_memory_type(*buffer5);

    std::cout << indent2
              << "Create a buffer (preference host memory, host writable): "
              << std::endl;
    const zivc::SharedBuffer buffer6 = device->createBuffer<int64b>({Usage::kPreferHost,
                                                                     Flag::kRandomAccessible});
    print_memory_type(*buffer6);

    // Allocate buffer memory
    {
      constexpr std::size_t size = 128 * 1024 * 1024;
      constexpr std::size_t n = size / sizeof(int64b);
      std::cout << indent2 << "Allocate 'sizeof(int) * n(" << n << "') bytes memory."
                << std::endl;
      buffer1->setSize(n);
      printBufferInfo(indent3, *buffer1);
    }

    // Fill buffer
    {
      constexpr int64b v = 99;
      std::cout << indent2 << "Fill integer buffer with '" << v << "'." << std::endl;
      // First create a launch options
      zivc::BufferLaunchOptions options = buffer1->createOptions();
      options.requestFence(true); // Request a fence because the fill can be async
      // Launch the fill command
      zivc::LaunchResult result = zivc::fill(v, buffer1.get(), options);
      // Wait this thread until the fill command done
      device->waitForCompletion(result.fence());
    }

    // Clear buffer memory
    {
      std::cout << indent2 << "Clear buffer memory." << std::endl;
      buffer1->clear();
      printBufferInfo(indent3, *buffer1);
    }

    // Mapping buffer memory
    constexpr std::size_t n_int64 = 10;
    buffer1->setSize(n_int64);
    buffer5->setSize(buffer1->size());
    buffer6->setSize(buffer1->size());
    {
      std::cout << indent2 << "Map buffer memory into host address space." << std::endl;
      printBufferInfo(indent3, *buffer5);
      // Map the buffer memory to host address space and write values into the mapped pointer
      // The buffer must be host visible
      zivc::MappedMemory mem = buffer5->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    // Copy buffer
    {
      std::cout << indent3 << "Copy the memory to the device local buffer." << std::endl;
      // Host to device copy
      // First create a launch options
      zivc::BufferLaunchOptions options = buffer5->createOptions();
      options.requestFence(true);
      // Launch the copy command
      zivc::LaunchResult result = zivc::copy(*buffer5, buffer1.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      // Device to host copy
      zivc::BufferLaunchOptions options = buffer1->createOptions();
      options.requestFence(true); // Request a fence because the copy can be async
      // Launch the copy command
      zivc::LaunchResult result = zivc::copy(*buffer1, buffer6.get(), options);
      // Wait this thread until the copy command done
      device->waitForCompletion(result.fence());
    }
    // Print mapped buffer memory
    {
      // Map the buffer memory to host address space and print the values
      // The buffer must be host visible and host cached
      zivc::MappedMemory mem = buffer6->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        std::cout << indent3 << "buffer6[" << i << "] = " << mem[i] << std::endl;
    }

    // Reinterpret cast of buffer
    {
      std::cout << indent2 << "Reinterpret cast from 'zivc::Buffer<int32b>' to 'zivc::Buffer<float>'." << std::endl;
      zivc::ReinterpBuffer fp4 = buffer5->reinterp<float>();
      printBufferInfo(indent3, fp4);
      // buffer5->sizeInBytes() == sizeof(float) * fp4->size()
      [[maybe_unused]] const std::size_t n_float = fp4.size(); // 20
      // Map the buffer memory
      {
        zivc::MappedMemory mem = fp4.mapMemory();
        std::iota(mem.begin(), mem.end(), 0.5f);
      }
      // Host to device copy
      zivc::ReinterpBuffer fp1 = buffer1->reinterp<float>();
      {
        zivc::BufferLaunchOptions options1 = fp4.createOptions();
        options1.requestFence(true);
        const zivc::LaunchResult result = zivc::copy(fp4, &fp1, options1);
        device->waitForCompletion(result.fence());
      }
      // Device to host copy
      zivc::ReinterpBuffer fp6 = buffer6->reinterp<float>();
      {
        zivc::BufferLaunchOptions options2 = fp1.createOptions();
        options2.requestFence(true);
        const zivc::LaunchResult result = zivc::copy(fp1, &fp6, options2);
        device->waitForCompletion(result.fence());
      }
      // Print mapped buffer memory
      {
        const zivc::MappedMemory mem = fp6.mapMemory();
        for (std::size_t i = 0; i < mem.size(); ++i)
          std::cout << indent3 << "fp6[" << i << "] = " << mem[i] << std::endl;
      }
    }

    // Print device memory usage
    std::cout << std::endl;
    const std::span heap_info_list = info->heapInfoList();
    for (std::size_t index = 0; index < heap_info_list.size(); ++index) {
      const zisc::Memory::Usage& usage = device->memoryUsage(index);
      std::cout << indent2 << "[Heap" << index << "]      Memory usage: "
                << ::toMegaBytes(usage.total()) << " MB." << std::endl;
      std::cout << indent2 << "[Heap" << index << "] Peak Memory usage: "
                << ::toMegaBytes(usage.peak()) << " MB." << std::endl;
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

    // Create a zivc context
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
