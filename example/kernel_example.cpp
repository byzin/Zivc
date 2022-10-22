/*!
  \file kernel_example.cpp
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
#include "zivc/zivc_config.hpp"
#include "zivc/kernel_set/kernel_set-example.hpp"

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
int doKernelExample(zivc::Context& context)
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
    const zivc::DeviceInfo* const info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getBackendTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;

    // Query a zivc device by the device index
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

    std::cout << std::endl;
    const std::string indent3 = indent2 + indent1;
    using zivc::int32b;

    // Create buffers
    constexpr std::size_t n = 10;
    // Input1
    const zivc::SharedBuffer buffer1 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
    buffer1->setSize(n);
    // Input2
    const zivc::SharedBuffer buffer2 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
    buffer2->setSize(n);
    // Output
    const zivc::SharedBuffer buffer3 = device->createBuffer<int32b>({zivc::BufferUsage::kPreferDevice});
    buffer3->setSize(n);
    // Staging buffer
    const zivc::SharedBuffer staging = device->createBuffer<int32b>({zivc::BufferUsage::kPreferHost,
                                                               zivc::BufferFlag::kRandomAccessible});
    staging->setSize(n);

    // Initialize the input buffers
    {
      zivc::MappedMemory mem = staging->mapMemory();
      std::iota(mem.begin(), mem.end(), 0);
    }
    {
      zivc::BufferLaunchOptions options = staging->createOptions();
      options.requestFence(true);
      zivc::LaunchResult result = zivc::copy(*staging, buffer1.get(), options);
      device->waitForCompletion(result.fence());
    }
    {
      zivc::BufferLaunchOptions options = buffer2->createOptions();
      options.requestFence(true);
      zivc::LaunchResult result = zivc::fill(10, buffer2.get(), options);
      device->waitForCompletion(result.fence());
    }

    auto print_kernel_info = [&indent3](const zivc::KernelCommon& kernel) noexcept
    {
      std::cout << indent3
                << "Workgroup size dimension: " << kernel.dimensionSize() << std::endl
                << "Number of arguments     : " << kernel.argSize() << std::endl
                << "Number of buffer args   : " << kernel.bufferSize() << std::endl
                << "Number of pod args      : " << kernel.podSize() << std::endl;
    };

    // Create a kernel
    std::cout << indent2 << "Launch kernel1" << std::endl;
    const zivc::KernelInitParams kernel_params1 = ZIVC_CREATE_KERNEL_INIT_PARAMS(example,
                                                                                 kernel1,
                                                                                 1);
    const zivc::SharedKernel kernel1 = device->createKernel(kernel_params1);
    print_kernel_info(*kernel1);

    // Launch the kernel
    {
      // First create options for launching the kernel
      zivc::KernelLaunchOptions options = kernel1->createOptions();
      options.setWorkSize({{n}});
      options.requestFence(true);
      // Launch the kernel
      const zivc::uint32b resolution = n;
      zivc::LaunchResult result = kernel1->run(*buffer1, *buffer2, *buffer3, resolution, options);
      // Wait this thread until the kernel done
      device->waitForCompletion(result.fence());
    }
    // Print the output
    {
      zivc::BufferLaunchOptions options = buffer3->createOptions();
      options.requestFence(true);
      zivc::LaunchResult result = zivc::copy(*buffer3, staging.get(), options);
      device->waitForCompletion(result.fence());

      const zivc::MappedMemory mem = staging->mapMemory();
      for (std::size_t i = 0; i < mem.size(); ++i)
        std::cout << indent3 << "output[" << i << "] = " << mem[i] << std::endl;
    }

    // Create another kernel
    std::cout << indent2 << "Launch kernel2" << std::endl;
    const zivc::KernelInitParams kernel_params2 = ZIVC_CREATE_KERNEL_INIT_PARAMS(example,
                                                                                 kernel2,
                                                                                 1);
    const zivc::SharedKernel kernel2 = device->createKernel(kernel_params2);
    print_kernel_info(*kernel2);

    // Reinterpreted buffers also can be used as kernel argument
    {
      // Initialize buffers
      zivc::ReinterpBuffer staging_fp = staging->reinterp<float>();
      {
        zivc::MappedMemory mem = staging_fp.mapMemory();
        std::iota(mem.begin(), mem.end(), 0.25f);
      }
      zivc::ReinterpBuffer fp1 = buffer1->reinterp<float>();
      zivc::ReinterpBuffer fp3 = buffer3->reinterp<float>();
      {
        zivc::BufferLaunchOptions options = staging_fp.createOptions();
        options.requestFence(true);
        zivc::LaunchResult result = zivc::copy(staging_fp, &fp1, options);
        device->waitForCompletion(result.fence());
      }
      {
        // Create options
        zivc::KernelLaunchOptions options = kernel2->createOptions();
        options.setWorkSize({{n}});
        options.requestFence(true);
        // Launch the kernel
        const zivc::uint32b resolution = n;
        zivc::LaunchResult result = kernel2->run(fp1, fp3, resolution, options);
        // Wait this thread until the kernel done
        device->waitForCompletion(result.fence());
      }
      {
        zivc::BufferLaunchOptions options = fp3.createOptions();
        options.requestFence(true);
        zivc::LaunchResult result = zivc::copy(fp3, &staging_fp, options);
        device->waitForCompletion(result.fence());

        const zivc::MappedMemory mem = staging_fp.mapMemory();
        for (std::size_t i = 0; i < mem.size(); ++i)
          std::cout << indent3 << "output[" << i << "] = " << mem[i] << std::endl;
      }
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

  // Exec kernel
  int exec_result = EXIT_FAILURE;
  {
    // Context options
    zivc::ContextOptions context_options{&mem_resource};
    context_options.setContextName("KernelExample");
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

    exec_result = ::doKernelExample(*context);
  }

  ::printMemoryUsage(mem_resource);

  return exec_result;
}
