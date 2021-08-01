/*!
  \file vulkan_device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device_info.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(zisc::pmr::memory_resource* mem_resource)
    noexcept :
        DeviceInfo(mem_resource),
        extension_properties_list_{
          decltype(extension_properties_list_)::allocator_type{mem_resource}},
        layer_properties_list_{
          decltype(layer_properties_list_)::allocator_type{mem_resource}},
        queue_family_properties_list_{
          decltype(queue_family_properties_list_)::allocator_type{mem_resource}},
        tool_properties_list_{
          decltype(tool_properties_list_)::allocator_type{mem_resource}},
        vendor_id_{VendorId::kUnknown},
        subgroup_size_{0}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept :
    DeviceInfo(std::move(other)),
    extension_properties_list_{std::move(other.extension_properties_list_)},
    layer_properties_list_{std::move(other.layer_properties_list_)},
    queue_family_properties_list_{std::move(other.queue_family_properties_list_)},
    tool_properties_list_{std::move(other.tool_properties_list_)},
    device_{other.device_},
    device_index_{other.device_index_},
    vendor_name_{std::move(other.vendor_name_)},
    vendor_id_{other.vendor_id_},
    subgroup_size_{other.subgroup_size_},
    properties_{std::move(other.properties_)},
    features_{std::move(other.features_)},
    memory_properties_{std::move(other.memory_properties_)}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDeviceInfo& VulkanDeviceInfo::operator=(VulkanDeviceInfo&& other) noexcept
{
  DeviceInfo::operator=(std::move(other));
  extension_properties_list_ = std::move(other.extension_properties_list_);
  layer_properties_list_ = std::move(other.layer_properties_list_);
  queue_family_properties_list_ = std::move(other.queue_family_properties_list_);
  tool_properties_list_ = std::move(other.tool_properties_list_);
  device_ = other.device_;
  device_index_ = other.device_index_;
  vendor_name_ = std::move(other.vendor_name_);
  vendor_id_ = other.vendor_id_;
  subgroup_size_ = other.subgroup_size_;
  properties_ = std::move(other.properties_);
  features_ = std::move(other.features_);
  memory_properties_ = std::move(other.memory_properties_);
  return *this;
}

/*!
  \details No detailed description
  */
VulkanDeviceInfo::~VulkanDeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in] vdevice No description.
  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetch(const VkPhysicalDevice& vdevice,
                             const VulkanDispatchLoader& dispatcher)
{
  device_ = vdevice;
  // Fetch properties from the given device
  fetchExtensionProperties(dispatcher);
  fetchLayerProperties(dispatcher);
  fetchQueueFamilyProperties(dispatcher);
  fetchToolProperties(dispatcher);
  fetchProperties(dispatcher);
  fetchFeatures(dispatcher);
  fetchMemoryProperties(dispatcher);
  // Set device info
  initHeapInfoList();
  initVendorInfo();
  initSubgroupSize();
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::maxAllocationSize() const noexcept
{
  const Properties& props = properties();
  const std::size_t max_alloc = props.maintenance3_.maxMemoryAllocationSize;
  return max_alloc;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::maxNumOfBuffersPerKernel() const noexcept
{
  const Properties& props = properties();
  const auto counts = props.properties1_.limits.maxPerStageDescriptorStorageBuffers;
  constexpr std::size_t num_of_reserved = 7;
  const std::size_t n = (num_of_reserved < counts) ? counts - num_of_reserved : 0;
  const std::size_t max_n = (std::min)(n, maxNumOfFunctionParameters());
  return max_n;
}

/*!
  \details No detailed description

  \return No description
  */
std::array<uint32b, 3> VulkanDeviceInfo::maxWorkGroupCount() const noexcept
{
  const Properties& props = properties();
  const uint32b* counts = props.properties1_.limits.maxComputeWorkGroupCount;
  const std::array<uint32b, 3> max_list{{counts[0], counts[1], counts[2]}};
  return max_list;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDeviceInfo::name() const noexcept
{
  const Properties& props = properties();
  std::string_view n{props.properties1_.deviceName};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType VulkanDeviceInfo::type() const noexcept
{
  return SubPlatformType::kVulkan;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDeviceInfo::vendorName() const noexcept
{
  const std::string_view n{vendor_name_.data()};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
uint32b VulkanDeviceInfo::workGroupSize() const noexcept
{
  return subgroup_size_;
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchExtensionProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zivcvk::ExtensionProperties;
  static_assert(sizeof(VkExtensionProperties) == sizeof(Props));

  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  uint32b size = 0;
  {
    auto result = d.enumerateDeviceExtensionProperties(nullptr, &size,
                                                       nullptr, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device extension props failed.";
      zivcvk::throwResultException(result, message);
    }
    extension_properties_list_.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto data = zisc::reinterp<Props*>(extension_properties_list_.data());
    auto result = d.enumerateDeviceExtensionProperties(nullptr, &size,
                                                       data, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device extension props failed.";
      zivcvk::throwResultException(result, message);
    }
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchFeatures(
    const VulkanDispatchLoader& dispatcher)
{
  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zivcvk::PhysicalDeviceFeatures2 p;
  auto& props = features_;
  link(p,
       initProp<zivcvk::PhysicalDevice16BitStorageFeatures>(
          props.b16bit_storage_),
       initProp<zivcvk::PhysicalDevice4444FormatsFeaturesEXT>(
          props.argb_4444_formats_),
       initProp<zivcvk::PhysicalDevice8BitStorageFeatures>(
          props.b8bit_storage_),
       initProp<zivcvk::PhysicalDeviceASTCDecodeFeaturesEXT>(
          props.astc_decode_),
       initProp<zivcvk::PhysicalDeviceAccelerationStructureFeaturesKHR>(
          props.acceleration_structure_),
       initProp<zivcvk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>(
          props.blend_operation_advanced_),
       initProp<zivcvk::PhysicalDeviceBufferDeviceAddressFeatures>(
          props.buffer_device_address_),
       initProp<zivcvk::PhysicalDeviceColorWriteEnableFeaturesEXT>(
          props.color_write_enable_),
       initProp<zivcvk::PhysicalDeviceConditionalRenderingFeaturesEXT>(
          props.conditional_rendering_),
       initProp<zivcvk::PhysicalDeviceCustomBorderColorFeaturesEXT>(
          props.custom_border_color_),
       initProp<zivcvk::PhysicalDeviceDepthClipEnableFeaturesEXT>(
          props.depth_clip_enabled_),
       initProp<zivcvk::PhysicalDeviceDescriptorIndexingFeatures>(
          props.descriptor_indexing_),
       initProp<zivcvk::PhysicalDeviceDeviceMemoryReportFeaturesEXT>(
          props.device_memory_report_),
       initProp<zivcvk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>(
          props.extended_dynamic_state_),
       initProp<zivcvk::PhysicalDeviceExtendedDynamicState2FeaturesEXT>(
          props.extended_dynamic_state2_),
       initProp<zivcvk::PhysicalDeviceFragmentDensityMapFeaturesEXT>(
          props.fragment_density_map_),
       initProp<zivcvk::PhysicalDeviceFragmentDensityMap2FeaturesEXT>(
          props.fragment_density_map2_),
       initProp<zivcvk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>(
          props.fragment_shader_inter_lock_),
       initProp<zivcvk::PhysicalDeviceFragmentShadingRateFeaturesKHR>(
          props.fragment_shading_rate_),
       initProp<zivcvk::PhysicalDeviceGlobalPriorityQueryFeaturesEXT>(
          props.global_priority_query_),
       initProp<zivcvk::PhysicalDeviceHostQueryResetFeatures>(
          props.host_query_reset_),
       initProp<zivcvk::PhysicalDeviceImageRobustnessFeaturesEXT>(
          props.image_robustness_),
       initProp<zivcvk::PhysicalDeviceImagelessFramebufferFeatures>(
          props.imageless_framebuffer_),
       initProp<zivcvk::PhysicalDeviceIndexTypeUint8FeaturesEXT>(
          props.index_type_uint8_),
       initProp<zivcvk::PhysicalDeviceInlineUniformBlockFeaturesEXT>(
          props.inline_uniform_block_),
       initProp<zivcvk::PhysicalDeviceLineRasterizationFeaturesEXT>(
          props.line_rasterization_),
       initProp<zivcvk::PhysicalDeviceMemoryPriorityFeaturesEXT>(
          props.memory_priority_features_),
       initProp<zivcvk::PhysicalDeviceMultiDrawFeaturesEXT>(
          props.multi_draw_),
       initProp<zivcvk::PhysicalDeviceMultiviewFeatures>(
          props.multiview_),
       initProp<zivcvk::PhysicalDevicePerformanceQueryFeaturesKHR>(
          props.performance_query_),
       initProp<zivcvk::PhysicalDevicePipelineCreationCacheControlFeaturesEXT>(
          props.pipeline_creation_cache_control_),
       initProp<zivcvk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR>(
          props.pipeline_executable_properties_),
       initProp<zivcvk::PhysicalDevicePortabilitySubsetFeaturesKHR>(
          props.portability_subset_),
       initProp<zivcvk::PhysicalDevicePrivateDataFeaturesEXT>(
          props.private_data_features_),
       initProp<zivcvk::PhysicalDeviceProtectedMemoryFeatures>(
          props.protected_memory_),
       initProp<zivcvk::PhysicalDeviceProvokingVertexFeaturesEXT>(
          props.provoking_vertex_),
       initProp<zivcvk::PhysicalDeviceRayQueryFeaturesKHR>(
          props.ray_query_),
       initProp<zivcvk::PhysicalDeviceRayTracingPipelineFeaturesKHR>(
          props.ray_tracing_pipeline_features_),
       initProp<zivcvk::PhysicalDeviceRobustness2FeaturesEXT>(
          props.robustness2_),
       initProp<zivcvk::PhysicalDeviceSamplerYcbcrConversionFeatures>(
          props.sampler_ycbcr_conversion_),
       initProp<zivcvk::PhysicalDeviceScalarBlockLayoutFeatures>(
          props.scalar_block_layout_),
       initProp<zivcvk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>(
          props.depth_stencil_layouts_),
       initProp<zivcvk::PhysicalDeviceShaderAtomicFloatFeaturesEXT>(
          props.shader_atomic_float_),
       initProp<zivcvk::PhysicalDeviceShaderAtomicInt64Features>(
          props.shader_atomic_int64_),
       initProp<zivcvk::PhysicalDeviceShaderClockFeaturesKHR>(
          props.shader_clock_),
       initProp<zivcvk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT>(
          props.demote_to_helper_invocation_),
       initProp<zivcvk::PhysicalDeviceShaderDrawParametersFeatures>(
          props.shader_draw_parameters_),
       initProp<zivcvk::PhysicalDeviceShaderFloat16Int8FeaturesKHR>(
          props.shader_float16_int8_),
       initProp<zivcvk::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT>(
          props.shader_image_atomic_int64_),
       initProp<zivcvk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>(
          props.shader_subgroup_extended_types_),
       initProp<zivcvk::PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR>(
          props.shader_subgroup_uniform_control_flow_),
       initProp<zivcvk::PhysicalDeviceShaderTerminateInvocationFeaturesKHR>(
          props.shader_terminate_invocation_),
       initProp<zivcvk::PhysicalDeviceSubgroupSizeControlFeaturesEXT>(
          props.subgroup_size_control_),
       initProp<zivcvk::PhysicalDeviceSynchronization2FeaturesKHR>(
          props.synchronization2_),
       initProp<zivcvk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>(
          props.texel_buffer_alignment_),
       initProp<zivcvk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT>(
          props.texture_compression_astchdr_),
       initProp<zivcvk::PhysicalDeviceTimelineSemaphoreFeatures>(
          props.timeline_semaphore_),
       initProp<zivcvk::PhysicalDeviceTransformFeedbackFeaturesEXT>(
          props.transform_feedback_),
       initProp<zivcvk::PhysicalDeviceUniformBufferStandardLayoutFeatures>(
          props.uniform_buffer_standard_layout_),
       initProp<zivcvk::PhysicalDeviceVariablePointerFeatures>(
          props.variable_pointers_),
       initProp<zivcvk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT>(
          props.vertex_attribute_divisor_),
       initProp<zivcvk::PhysicalDeviceVertexInputDynamicStateFeaturesEXT>(
          props.vertex_input_dynamic_state_),
       initProp<zivcvk::PhysicalDeviceVulkan11Features>(
          props.vulkan11_),
       initProp<zivcvk::PhysicalDeviceVulkan12Features>(
          props.vulkan12_),
       initProp<zivcvk::PhysicalDeviceVulkanMemoryModelFeatures>(
          props.vulkan_memory_model_),
       initProp<zivcvk::PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR>(
          props.workgroup_memory_explicit_layout_),
       initProp<zivcvk::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT>(
          props.ycbcr_2plane_444_formats_),
       initProp<zivcvk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>(
          props.ycbcr_image_arrays_),
       initProp<zivcvk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR>(
          props.zero_initialize_workgroup_memory_)
       );
  d.getFeatures2(&p, *loader);
  props.features1_ = zisc::cast<VkPhysicalDeviceFeatures>(p.features);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchLayerProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zivcvk::LayerProperties;
  static_assert(sizeof(VkLayerProperties) == sizeof(Props));

  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  uint32b size = 0;
  {
    auto result = d.enumerateDeviceLayerProperties(&size, nullptr, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device layer props failed.";
      zivcvk::throwResultException(result, message);
    }
    layer_properties_list_.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto data = zisc::reinterp<Props*>(layer_properties_list_.data());
    auto result = d.enumerateDeviceLayerProperties(&size, data, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device layer props failed.";
      zivcvk::throwResultException(result, message);
    }
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchMemoryProperties(
    const VulkanDispatchLoader& dispatcher)
{
  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zivcvk::PhysicalDeviceMemoryProperties2 p;
  auto& props = memory_properties_;
  link(p,
       initProp<zivcvk::PhysicalDeviceMemoryBudgetPropertiesEXT>(
          props.budget_)
       );
  d.getMemoryProperties2(&p, *loader);
  props.properties1_ =
      zisc::cast<VkPhysicalDeviceMemoryProperties>(p.memoryProperties);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchProperties(
    const VulkanDispatchLoader& dispatcher)
{
  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zivcvk::PhysicalDeviceProperties2 p;
  auto& props = properties_;
  link(p,
       initProp<zivcvk::PhysicalDeviceAccelerationStructurePropertiesKHR>(
          props.acceleration_structure_),
       initProp<zivcvk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>(
          props.blend_operation_advanced_),
       initProp<zivcvk::PhysicalDeviceConservativeRasterizationPropertiesEXT>(
          props.conservative_rasterization_),
       initProp<zivcvk::PhysicalDeviceCustomBorderColorPropertiesEXT>(
          props.custom_border_color_),
       initProp<zivcvk::PhysicalDeviceDepthStencilResolveProperties>(
          props.depth_stencil_resolve_),
       initProp<zivcvk::PhysicalDeviceDescriptorIndexingProperties>(
          props.descriptor_indexing_),
       initProp<zivcvk::PhysicalDeviceDiscardRectanglePropertiesEXT>(
          props.discard_rectangle_),
       initProp<zivcvk::PhysicalDeviceDriverProperties>(
          props.driver_),
       initProp<zivcvk::PhysicalDeviceDrmPropertiesEXT>(
          props.drm_),
       initProp<zivcvk::PhysicalDeviceExternalMemoryHostPropertiesEXT>(
          props.external_memory_host_),
       initProp<zivcvk::PhysicalDeviceFloatControlsProperties>(
          props.float_controls_),
       initProp<zivcvk::PhysicalDeviceFragmentDensityMapPropertiesEXT>(
          props.fragment_density_map_),
       initProp<zivcvk::PhysicalDeviceFragmentDensityMap2PropertiesEXT>(
          props.fragment_density_map2_),
       initProp<zivcvk::PhysicalDeviceFragmentShadingRatePropertiesKHR>(
          props.fragment_shading_rate_),
       initProp<zivcvk::PhysicalDeviceIDProperties>(
          props.id_),
       initProp<zivcvk::PhysicalDeviceInlineUniformBlockPropertiesEXT>(
          props.inline_uniform_block_),
       initProp<zivcvk::PhysicalDeviceLineRasterizationPropertiesEXT>(
          props.line_rasterization_),
       initProp<zivcvk::PhysicalDeviceMaintenance3Properties>(
          props.maintenance3_),
       initProp<zivcvk::PhysicalDeviceMultiDrawPropertiesEXT>(
          props.multi_draw_),
       initProp<zivcvk::PhysicalDeviceMultiviewProperties>(
          props.multiview_),
       initProp<zivcvk::PhysicalDevicePCIBusInfoPropertiesEXT>(
          props.pci_bus_info_),
       initProp<zivcvk::PhysicalDevicePerformanceQueryPropertiesKHR>(
          props.performance_query_),
       initProp<zivcvk::PhysicalDevicePointClippingProperties>(
          props.point_clipping_),
       initProp<zivcvk::PhysicalDevicePortabilitySubsetPropertiesKHR>(
          props.portability_subset_),
       initProp<zivcvk::PhysicalDeviceProtectedMemoryProperties>(
          props.protected_memory_),
       initProp<zivcvk::PhysicalDeviceProvokingVertexPropertiesEXT>(
          props.provoking_vertex_),
       initProp<zivcvk::PhysicalDevicePushDescriptorPropertiesKHR>(
          props.push_descriptor_),
       initProp<zivcvk::PhysicalDeviceRayTracingPipelinePropertiesKHR>(
          props.ray_tracing_pipeline_),
       initProp<zivcvk::PhysicalDeviceRobustness2PropertiesEXT>(
          props.robustness2_),
       initProp<zivcvk::PhysicalDeviceSampleLocationsPropertiesEXT>(
          props.sample_locations_),
       initProp<zivcvk::PhysicalDeviceSamplerFilterMinmaxProperties>(
          props.sampler_filter_minmax_),
       initProp<zivcvk::PhysicalDeviceSubgroupProperties>(
          props.subgroup_),
       initProp<zivcvk::PhysicalDeviceSubgroupSizeControlPropertiesEXT>(
          props.subgroup_size_control_),
       initProp<zivcvk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT>(
          props.texel_buffer_alignment_),
       initProp<zivcvk::PhysicalDeviceTimelineSemaphoreProperties>(
          props.timeline_semaphore_),
       initProp<zivcvk::PhysicalDeviceTransformFeedbackPropertiesEXT>(
          props.transform_feedback_),
       initProp<zivcvk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>(
          props.vertex_attribute_divisor_),
       initProp<zivcvk::PhysicalDeviceVulkan11Properties>(
          props.vulkan11_),
       initProp<zivcvk::PhysicalDeviceVulkan12Properties>(
          props.vulkan12_)
       );
  d.getProperties2(&p, *loader);
  props.properties1_ = zisc::cast<VkPhysicalDeviceProperties>(p.properties);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchToolProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zivcvk::PhysicalDeviceToolPropertiesEXT;
  static_assert(sizeof(VkPhysicalDeviceToolPropertiesEXT) == sizeof(Props));

  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  uint32b size = 0;
  {
    auto result = d.getToolPropertiesEXT(&size, nullptr, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device tool props failed.";
      zivcvk::throwResultException(result, message);
    }
    tool_properties_list_.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto data = zisc::reinterp<Props*>(tool_properties_list_.data());
    auto result = d.getToolPropertiesEXT(&size, data, *loader);
    if (result != zivcvk::Result::eSuccess) {
      const char* message = "Fetching device tool props failed.";
      zivcvk::throwResultException(result, message);
    }
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchQueueFamilyProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zivcvk::QueueFamilyProperties2;
  static_assert(sizeof(VkQueueFamilyProperties2) == sizeof(Props));

  const zivcvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  constexpr std::size_t max_size = maxQueueFamilyCount();
  uint32b size = 0;
  {
    d.getQueueFamilyProperties(&size, nullptr, *loader);
    ZISC_ASSERT(size <= max_size, "The number of queue families exceeded the limit.");
    queue_family_properties_list_.resize(zisc::cast<std::size_t>(size));
  }

  // Initialize property chain
  std::array<zivcvk::QueueFamilyProperties2, max_size> p_list;
  for (std::size_t i = 0; i < size; ++i) {
    auto& p = p_list[i];
    auto& props = queue_family_properties_list_[i];
    link(p,
         initProp<zivcvk::QueueFamilyGlobalPriorityPropertiesEXT>(
            props.global_priority_)
        );
  }

  d.getQueueFamilyProperties2(&size, p_list.data(), *loader);
  for (std::size_t i = 0; i < size; ++i) {
    queue_family_properties_list_[i].properties1_ =
        zisc::cast<VkQueueFamilyProperties>(p_list[i].queueFamilyProperties);
  }
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::initHeapInfoList() noexcept
{
  const auto& props = memoryProperties().properties1_;
  auto& heap_info_list = DeviceInfo::heapInfoList();
  heap_info_list.clear();
  heap_info_list.resize(props.memoryHeapCount);
  for (std::size_t i = 0; i < heap_info_list.size(); ++i) {
    MemoryHeapInfo& info = heap_info_list[i];
    const zivcvk::MemoryHeap heap{props.memoryHeaps[i]};
    {
      constexpr auto dflag = zivcvk::MemoryHeapFlagBits::eDeviceLocal;
      const bool is_device_local = (heap.flags & dflag) == dflag;
      info.setDeviceLocal(is_device_local);
    }
    info.setTotalSize(heap.size);
    {
      const auto& budget = memoryProperties().budget_;
      const std::size_t size = budget.heapBudget[i] - budget.heapUsage[i];
      info.setAvailableSize(size);
    }
  }
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::initSubgroupSize() noexcept
{
  const Properties& props = properties();
  uint32b subgroup_size = props.subgroup_.subgroupSize;
  if (subgroup_size == 0) {
    switch (props.properties1_.vendorID) {
     default: {
      subgroup_size = 32;
     }
    }
  }
  subgroup_size_ = subgroup_size;
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::initVendorInfo() noexcept
{
  auto set_vendor_name = [this](std::string_view n)
  {
    std::strncpy(vendor_name_.data(), n.data(), n.size() + 1);
  };

  switch (properties().properties1_.vendorID) {
   case zisc::cast<uint32b>(VendorId::kAmd): {
    vendor_id_ = VendorId::kAmd;
    set_vendor_name("AMD");
    break;
   }
   case zisc::cast<uint32b>(VendorId::kImgTec): {
    vendor_id_ = VendorId::kImgTec;
    set_vendor_name("ImgTec");
    break;
   }
   case zisc::cast<uint32b>(VendorId::kNvidia): {
    vendor_id_ = VendorId::kNvidia;
    set_vendor_name("NVIDIA");
    break;
   }
   case zisc::cast<uint32b>(VendorId::kArm): {
    vendor_id_ = VendorId::kArm;
    set_vendor_name("ARM");
    break;
   }
   case zisc::cast<uint32b>(VendorId::kQualcomm): {
    vendor_id_ = VendorId::kQualcomm;
    set_vendor_name("Qualcomm");
    break;
   }
   case zisc::cast<uint32b>(VendorId::kIntel): {
    vendor_id_ = VendorId::kIntel;
    set_vendor_name("INTEL");
    break;
   }
   default: {
    vendor_id_ = VendorId::kUnknown;
    set_vendor_name("Unknown");
    break;
   }
  }
}

} // namespace zivc
