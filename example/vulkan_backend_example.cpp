/*!
  \file vulkan_backend_example.cpp
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
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
#include "zivc/vulkan/vulkan_device_info.hpp"
#include "zivc/vulkan/vulkan_backend.hpp"
#include "zivc/vulkan/utility/vulkan_hpp.hpp"
#include "zivc/vulkan/utility/vulkan_dispatch_loader.hpp"
#endif // ZIVC_ENABLE_VULKAN_BACKEND

namespace {

/*!
  \details No detailed description

  \param [in] bytes No description.
  \return No description
  */
[[maybe_unused]] double toKiloBytes(const std::size_t bytes) noexcept
{
  const double kb = zisc::cast<double>(bytes) / 1024.0;
  return kb;
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
int printVulkanBackendInfo(const zivc::Context& context)
{
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
  // Get the vulkan backend
  const auto* backend_p =
    static_cast<const zivc::VulkanBackend*>(context.backend(zivc::BackendType::kVulkan));

  const std::string indent1 = "    ";
  const std::string indent2 = indent1 + indent1;
  const std::string indent3 = indent2 + indent1;
  const std::string indent4 = indent3 + indent1;
  const std::string indent5 = indent4 + indent1;

  const auto to_version_str = [](const zivc::uint32b version) noexcept
  {
    return std::to_string(zivc::vkGetVersionMajor(version)) + "." +
        std::to_string(zivc::vkGetVersionMinor(version)) + "." +
        std::to_string(zivc::vkGetVersionPatch(version));
  };

  // Print Vulkan instance extensions
  {
    std::cout << indent1 << "Vulkan extensions" << std::endl;
    const std::span properties = backend_p->extensionPropertiesList();
    for (const VkExtensionProperties& ext : properties) {
      std::cout << indent2 << ext.extensionName << ": "
                << to_version_str(ext.specVersion) << std::endl;
    }
  }

  // Print Vulkan instance layeres
  std::cout << std::endl;
  {
    std::cout << indent1 << "Vulkan layeres" << std::endl;
    const std::span properties = backend_p->layerPropertiesList();
    for (const VkLayerProperties& layer : properties) {
      std::cout << indent2 << layer.layerName << ": "
                << to_version_str(layer.specVersion)
                << " (" << to_version_str(layer.implementationVersion)
                << "): " << layer.description 
                << std::endl;
    }
  }

  // Print Vulkan device info
  std::cout << std::endl;
  const std::span info_list = backend_p->deviceInfoList();
  for (std::size_t i = 0; i < info_list.size(); ++i) {
    const zivc::VulkanDeviceInfo& info = info_list[i];
    std::cout << std::endl;
    {
      const VkPhysicalDeviceProperties& props = info.properties().properties1_;
      std::cout << indent1 << "## VulkanDevice[" << i << "]: "
                << props.deviceName << std::endl;
      std::cout << indent2 << "API version: "
                << to_version_str(props.apiVersion) << std::endl;
      std::cout << indent2 << "Driver version: "
                << to_version_str(props.driverVersion) << std::endl;
    }
    std::cout << std::endl;
    {
      std::cout << indent2 << "## Limits: " << std::endl;
      const VkPhysicalDeviceLimits& limits = info.properties().properties1_.limits;
      std::cout << indent3 << "Max uniform buffer range: "
                << ::toMegaBytes(limits.maxUniformBufferRange)
                << " MB" << std::endl;
      std::cout << indent3 << "Max storage buffer range: "
                << ::toMegaBytes(limits.maxStorageBufferRange)
                << " MB" << std::endl;
      std::cout << indent3 << "Max push constant size: "
                << ::toKiloBytes(limits.maxPushConstantsSize)
                << " KB" << std::endl;
      std::cout << indent3 << "Max memory allocation count: "
                << limits.maxMemoryAllocationCount << std::endl;
      std::cout << indent3 << "Max bound descriptor sets: "
                << limits.maxBoundDescriptorSets << std::endl;
      std::cout << indent3 << "Max per stage descriptor uniform buffers: "
                << limits.maxPerStageDescriptorUniformBuffers << std::endl;
      std::cout << indent3 << "Max per stage descriptor storage buffers: "
                << limits.maxPerStageDescriptorStorageBuffers << std::endl;
      std::cout << indent3 << "Max descriptor set uniform buffers: "
                << limits.maxDescriptorSetUniformBuffers << std::endl;
      std::cout << indent3 << "Max descriptor set storage buffers: "
                << limits.maxDescriptorSetStorageBuffers << std::endl;
      std::cout << indent3 << "Max compute shared memory size: "
                << ::toKiloBytes(limits.maxComputeSharedMemorySize)
                << " KB" << std::endl;
      std::cout << indent3 << "Max compute work group count: ["
                << limits.maxComputeWorkGroupCount[0] << ", "
                << limits.maxComputeWorkGroupCount[1] << ", "
                << limits.maxComputeWorkGroupCount[2] << "]" << std::endl;
      std::cout << indent3 << "Max compute work group invocations: "
                << limits.maxComputeWorkGroupInvocations << std::endl;
      std::cout << indent3 << "Max compute work group size: ["
                << limits.maxComputeWorkGroupSize[0] << ", "
                << limits.maxComputeWorkGroupSize[1] << ", "
                << limits.maxComputeWorkGroupSize[2] << "]" << std::endl;
      std::cout << indent3 << "Min memory map alignment: "
                << limits.minMemoryMapAlignment << " bytes" << std::endl;
      std::cout << indent3 << "Min uniform buffer offset alignment: "
                << limits.minUniformBufferOffsetAlignment << std::endl;
      std::cout << indent3 << "Min storage buffer offset alignment: "
                << limits.minStorageBufferOffsetAlignment << std::endl;
      {
        const VkPhysicalDevicePushDescriptorPropertiesKHR& descriptor_props = info.properties().push_descriptor_;
        std::cout << indent3 << "Max number of push descriptors: "
                  << descriptor_props.maxPushDescriptors << std::endl;
      }
    }
    std::cout << std::endl;
    // Extension properties
    {
      std::cout << indent2 << "Extensions" << std::endl;
      std::vector<VkExtensionProperties> props;
      {
        const std::span props2 = info.extensionPropertiesList();
        props.resize(props2.size());
        std::copy(props2.begin(), props2.end(), props.begin());
      }
      using Type = decltype(props)::value_type;
      const auto cmp = [](const Type& lhs, const Type& rhs) noexcept
      {
        return std::string_view{lhs.extensionName} <
               std::string_view{rhs.extensionName};
      };
      std::sort(props.begin(), props.end(), cmp);
      for (const VkExtensionProperties& ext : props) {
        std::cout << indent3 << ext.extensionName << ": "
                  << to_version_str(ext.specVersion) << std::endl;
      }
    }
    // Layer
    std::cout << std::endl;
    {
      std::cout << indent2 << "Layeres" << std::endl;
      const std::span props = info.layerPropertiesList();
      for (const VkLayerProperties& layer : props) {
        std::cout << indent3 << layer.layerName << ": "
                  << to_version_str(layer.specVersion)
                  << " (" << to_version_str(layer.implementationVersion)
                  << "): " << layer.description 
                  << std::endl;
      }
    }
    // Queue family
    std::cout << std::endl;
    {
      std::cout << indent2 << "Queue families" << std::endl;
      const std::span props = info.queueFamilyPropertiesList();
      for (std::size_t index = 0; index < props.size(); ++index) {
        const auto& prop = props[index];
        std::cout << indent3 << "QueueFamily[" << index << "]: "
                  << "counts: " << prop.properties1_.queueCount
                  << ", " << std::endl
                  << indent4 << "capabilities: "
                  << zivc::vk::to_string(zisc::cast<zivc::vk::QueueFlags>(prop.properties1_.queueFlags))
                  << std::endl;
      }
    }
    // Memory
    std::cout << std::endl;
    {
      std::cout << indent2 << "MemoryHeap" << std::endl;
      const auto& props = info.memoryProperties();
      const VkPhysicalDeviceMemoryProperties& mem = props.properties1_;
      for (std::size_t index = 0; index < mem.memoryHeapCount; ++index) {
        const VkMemoryHeap& heap = mem.memoryHeaps[index];
        std::cout << indent3 << "Heap[" << index << "] attribute: "
                  << zivc::vk::to_string(zisc::cast<zivc::vk::MemoryHeapFlags>(heap.flags))
                  << std::endl;
        std::cout << indent4 << "total  : "
                  << ::toMegaBytes(heap.size) << " MB"
                  << std::endl
                  << indent4 << "budget : "
                  << ::toMegaBytes(props.budget_.heapBudget[index]) << " MB"
                  << std::endl
                  << indent4 << "used   : "
                  << ::toMegaBytes(props.budget_.heapUsage[index]) << " MB"
                  << std::endl;
      }
    }
    std::cout << std::endl;
    {
      std::cout << indent2 << "Memory" << std::endl;
      const auto& props = info.memoryProperties();
      const VkPhysicalDeviceMemoryProperties& mem = props.properties1_;
      for (std::size_t index = 0; index < mem.memoryTypeCount; ++index) {
        const VkMemoryType& memory = mem.memoryTypes[index];
        std::cout << indent3 << "Memory[" << index << "] -> Heap["
                  << memory.heapIndex << "], properties: "
                  << zivc::vk::to_string(zisc::cast<zivc::vk::MemoryPropertyFlags>(memory.propertyFlags))
                  << std::endl;
      }
    }
    std::cout << std::endl;
    {
      std::cout << indent2 << "ExternalMemory" << std::endl;
      const auto& props = info.properties();
      std::cout << indent3 << "Min imported pointer alignment: "
                << props.external_memory_host_.minImportedHostPointerAlignment
                << " bytes" << std::endl;
    }
    {
      std::cout << indent2 << "Float" << std::endl;
      const VkPhysicalDeviceFloatControlsProperties& props = info.properties().float_controls_;
      std::cout << indent3 << "float16: " << std::endl;
      std::cout << indent4 << "+-0,inf,nan: "
                << props.shaderSignedZeroInfNanPreserveFloat16 << std::endl
                << indent4 << "denorm: "
                << props.shaderDenormPreserveFloat16 << std::endl
                << indent4 << "denorm to zero: "
                << props.shaderDenormFlushToZeroFloat16 << std::endl
                << indent4 << "round-to-nearest-even: "
                << props.shaderRoundingModeRTEFloat16 << std::endl
                << indent4 << "round-towards-zero: "
                << props.shaderRoundingModeRTZFloat16 << std::endl;
      std::cout << indent3 << "float32: " << std::endl;
      std::cout << indent4 << "+-0,inf,nan: "
                << props.shaderSignedZeroInfNanPreserveFloat32 << std::endl
                << indent4 << "denorm: "
                << props.shaderDenormPreserveFloat32 << std::endl
                << indent4 << "denorm to zero: "
                << props.shaderDenormFlushToZeroFloat32 << std::endl
                << indent4 << "round-to-nearest-even: "
                << props.shaderRoundingModeRTEFloat32 << std::endl
                << indent4 << "round-towards-zero: "
                << props.shaderRoundingModeRTZFloat32 << std::endl;
      std::cout << indent3 << "float64: " << std::endl;
      std::cout << indent4 << "+-0,inf,nan: "
                << props.shaderSignedZeroInfNanPreserveFloat64 << std::endl
                << indent4 << "denorm: "
                << props.shaderDenormPreserveFloat64 << std::endl
                << indent4 << "denorm to zero: "
                << props.shaderDenormFlushToZeroFloat64 << std::endl
                << indent4 << "round-to-nearest-even: "
                << props.shaderRoundingModeRTEFloat64 << std::endl
                << indent4 << "round-towards-zero: "
                << props.shaderRoundingModeRTZFloat64 << std::endl;
      const auto& features = info.features();
      const VkPhysicalDeviceShaderAtomicFloatFeaturesEXT& atomic_float = features.shader_atomic_float_;
      std::cout << indent3 << "atomic32 storage: "
                << atomic_float.shaderBufferFloat32AtomicAdd << std::endl
                << indent3 << "atomic64 storage: "
                << atomic_float.shaderBufferFloat64AtomicAdd << std::endl
                << indent3 << "atomic32 shared: "
                << atomic_float.shaderSharedFloat32AtomicAdd << std::endl
                << indent3 << "atomic64 shared: "
                << atomic_float.shaderSharedFloat64AtomicAdd << std::endl;
    }
    // Ray tracing features
    {
      const auto& props = info.properties();
      const auto& features = info.features();
      std::cout << indent2 << "Ray Tracing" << std::endl;

      const VkPhysicalDeviceAccelerationStructureFeaturesKHR& acc_structure = features.acceleration_structure_;
      std::cout << indent3 << "acceleration structure: "
                << acc_structure.accelerationStructure << std::endl;
      if (acc_structure.accelerationStructure != 0) {
        std::cout << indent4 << "capture reply: "
                  << acc_structure.accelerationStructureCaptureReplay << std::endl
                  << indent4 << "indirect build: "
                  << acc_structure.accelerationStructureIndirectBuild << std::endl
                  << indent4 << "host commands: "
                  << acc_structure.accelerationStructureHostCommands << std::endl;
      }
      if (acc_structure.accelerationStructure != 0) {
        const VkPhysicalDeviceAccelerationStructurePropertiesKHR acc_structure = props.acceleration_structure_;
        std::cout << indent4 << "Max geometry count: "
                  << acc_structure.maxGeometryCount << std::endl
                  << indent4 << "Max instance count: "
                  << acc_structure.maxInstanceCount << std::endl
                  << indent4 << "Max primitive count: "
                  << acc_structure.maxPrimitiveCount << std::endl
                  << indent4 << "Min scratch offset alignment: "
                  << acc_structure.minAccelerationStructureScratchOffsetAlignment << std::endl;
      }

      const VkPhysicalDeviceRayQueryFeaturesKHR& ray_query = features.ray_query_;
      std::cout << indent3 << "ray query: "
                << ray_query.rayQuery << std::endl;

      const VkPhysicalDeviceRayTracingPipelineFeaturesKHR& ray_pipeline = features.ray_tracing_pipeline_;
      std::cout << indent3 << "ray tracing pipeline: "
                << ray_pipeline.rayTracingPipeline << std::endl;
      if (ray_pipeline.rayTracingPipeline != 0) {
        std::cout << indent4 << "indirect trace ray: "
                  << ray_pipeline.rayTracingPipelineTraceRaysIndirect << std::endl;
        std::cout << indent4 << "primitive culling: "
                  << ray_pipeline.rayTraversalPrimitiveCulling << std::endl;
      }
      if (ray_pipeline.rayTracingPipeline != 0) {
        const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& ray_pipeline = props.ray_tracing_pipeline_;
        std::cout << indent4 << "Shader group handle size: "
                  << ray_pipeline.shaderGroupHandleSize << std::endl
                  << indent4 << "Max ray recursion depth: "
                  << ray_pipeline.maxRayRecursionDepth << std::endl
                  << indent4 << "Shader group base alignment: "
                  << ray_pipeline.shaderGroupBaseAlignment << std::endl
                  << indent4 << "Shader group handle alignment: "
                  << ray_pipeline.shaderGroupHandleAlignment << std::endl
                  << indent4 << "Max ray hit attribute size: "
                  << ray_pipeline.maxRayHitAttributeSize << std::endl;
      }

      const VkPhysicalDeviceOpacityMicromapFeaturesEXT& opacity_micromap = features.opacity_micromap_;
      std::cout << indent3 << "opacity micromap: "
                << opacity_micromap.micromap << std::endl;
      if (opacity_micromap.micromap != 0) {
        std::cout << opacity_micromap.micromap << std::endl
                  << indent4 << "micromap capture reply: "
                  << opacity_micromap.micromapCaptureReplay << std::endl
                  << indent4 << "micromap host commands: "
                  << opacity_micromap.micromapHostCommands << std::endl;
      }
      if (opacity_micromap.micromap != 0) {
        const VkPhysicalDeviceOpacityMicromapPropertiesEXT& opacity_micromap = props.opacity_micromap_;
        std::cout << indent4 << "max opacity2state subdivision level: "
                  << opacity_micromap.maxOpacity2StateSubdivisionLevel << std::endl
                  << indent4 << "max opacity4state subdivision level: "
                  << opacity_micromap.maxOpacity4StateSubdivisionLevel << std::endl;
      }
    }
  }
#endif // ZIVC_ENABLE_VULKAN_BACKEND
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
  std::cout << indent1 << "Memory usage: "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  // Any custom std::pmr::memory_resource can be specified as zivc memory allocator
  zisc::AllocFreeResource mem_resource;

  // Create a zivc context and print vulkan backend information
  int exec_result = EXIT_FAILURE;
  {
    // Set context options
    zivc::ContextOptions context_options{&mem_resource};
    context_options.setContextName("VulkanBackendExample");
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

    // Check if the context has vulkan backend
    const bool has_backend = context->hasBackend(zivc::BackendType::kVulkan);
    if (!has_backend) {
      std::cerr << "The context doesn't have vulkan backend." << std::endl;
      return EXIT_FAILURE;
    }

    exec_result = ::printVulkanBackendInfo(*context);
  }

  ::printMemoryUsage(mem_resource);

  return exec_result;
}
