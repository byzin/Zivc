/*!
  \file vulkan_device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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
#include <span>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "../zivc_config.hpp"
#include "../auxiliary/error.hpp"
#include "zivc/vulkan/vulkan_dispatch_loader.hpp"
#include "zivc/vulkan/vulkan_hpp.hpp"

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
        vendor_id_{VendorId::kUnknown}
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
    vendor_name_{other.vendor_name_},
    vendor_id_{other.vendor_id_},
    subgroup_size_{other.subgroup_size_},
    properties_{other.properties_},
    features_{other.features_},
    memory_properties_{other.memory_properties_}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDeviceInfo& VulkanDeviceInfo::operator=(VulkanDeviceInfo&& other) noexcept
{
  extension_properties_list_ = std::move(other.extension_properties_list_);
  layer_properties_list_ = std::move(other.layer_properties_list_);
  queue_family_properties_list_ = std::move(other.queue_family_properties_list_);
  tool_properties_list_ = std::move(other.tool_properties_list_);
  device_ = other.device_;
  device_index_ = other.device_index_;
  vendor_name_ = other.vendor_name_;
  vendor_id_ = other.vendor_id_;
  subgroup_size_ = other.subgroup_size_;
  properties_ = other.properties_;
  features_ = other.features_;
  memory_properties_ = other.memory_properties_;
  DeviceInfo::operator=(std::move(other));
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

  \param [in] name No description.
  \return No description
  */
bool VulkanDeviceInfo::isExtensionSupported(const std::string_view name) const noexcept
{
  const auto comp = [](const VkExtensionProperties& lhs,
                       const std::string_view rhs) noexcept
  {
    return compareProperties(lhs.extensionName, rhs);
  };
  const std::span prop_list = extensionPropertiesList();
  const auto ite = std::lower_bound(prop_list.begin(), prop_list.end(), name, comp);
  const bool result = (ite != prop_list.end()) && (ite->extensionName == name);
  return result;
}

/*!
  \details No detailed description

  \param [in] name No description.
  \return No description
  */
bool VulkanDeviceInfo::isLayerSupported(const std::string_view name) const noexcept
{
  const auto comp = [](const VkLayerProperties& lhs,
                       const std::string_view rhs) noexcept
  {
    return compareProperties(lhs.layerName, rhs);
  };
  const std::span prop_list = layerPropertiesList();
  const auto ite = std::lower_bound(prop_list.begin(), prop_list.end(), name, comp);
  const bool result = (ite != prop_list.end()) && (ite->layerName == name);
  return result;
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
  const std::size_t cap = props.properties1_.limits.maxPerStageDescriptorStorageBuffers;
  constexpr std::size_t num_of_reserved = 3;
  const std::size_t n = (num_of_reserved < cap) ? cap - num_of_reserved : 0;
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
  const std::string_view n{props.properties1_.deviceName};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
PhysicalDeviceType VulkanDeviceInfo::physicalDeviceType() const noexcept
{
  const Properties& props = properties();
  const auto type = static_cast<PhysicalDeviceType>(static_cast<uint32b>(props.properties1_.deviceType));
  return type;
}

/*!
  \details No detailed description

  \return No description
  */
BackendType VulkanDeviceInfo::type() const noexcept
{
  return BackendType::kVulkan;
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

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
bool VulkanDeviceInfo::compareProperties(const std::string_view lhs,
                                         const std::string_view rhs) noexcept
{
  const bool result = lhs < rhs;
  return result;
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchExtensionProperties(const VulkanDispatchLoader& dispatcher)
{
  using Props = vk::ExtensionProperties;
  static_assert(sizeof(VkExtensionProperties) == sizeof(Props));

  const vk::PhysicalDevice d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher.loader();

  zisc::pmr::vector<VkExtensionProperties>& prop_list = extension_properties_list_;
  uint32b size = 0;
  {
    const vk::Result result = d.enumerateDeviceExtensionProperties(nullptr, &size,
                                                                   nullptr, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device extension props failed.";
      vk::throwResultException(result, message);
    }
    prop_list.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto* data = zisc::reinterp<Props*>(prop_list.data());
    const vk::Result result = d.enumerateDeviceExtensionProperties(nullptr, &size,
                                                                   data, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device extension props failed.";
      vk::throwResultException(result, message);
    }
  }
  {
    const auto comp = [](const VkExtensionProperties& lhs,
                         const VkExtensionProperties& rhs) noexcept
    {
      return compareProperties(lhs.extensionName, rhs.extensionName);
    };
    std::sort(prop_list.begin(), prop_list.end(), comp);
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchFeatures(const VulkanDispatchLoader& dispatcher)
{
  const vk::PhysicalDevice d{device()};

  vk::PhysicalDeviceFeatures2 p;
  Features& props = features_;
  link(p,
       initProp<vk::PhysicalDevice16BitStorageFeatures>(
          props.b16bit_storage_),
       initProp<vk::PhysicalDevice4444FormatsFeaturesEXT>(
          props.argb_4444_formats_),
       initProp<vk::PhysicalDevice8BitStorageFeatures>(
          props.b8bit_storage_),
       initProp<vk::PhysicalDeviceASTCDecodeFeaturesEXT>(
          props.astc_decode_),
       initProp<vk::PhysicalDeviceAccelerationStructureFeaturesKHR>(
          props.acceleration_structure_),
       initProp<vk::PhysicalDeviceAddressBindingReportFeaturesEXT>(
          props.address_binding_report_),
       initProp<vk::PhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT>(
          props.attachment_feedback_loop_layout_),
       initProp<vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>(
          props.blend_operation_advanced_),
       initProp<vk::PhysicalDeviceBorderColorSwizzleFeaturesEXT>(
          props.border_color_swizzle_),
       initProp<vk::PhysicalDeviceBufferDeviceAddressFeatures>(
          props.buffer_device_address_),
       initProp<vk::PhysicalDeviceColorWriteEnableFeaturesEXT>(
          props.color_write_enable_),
       initProp<vk::PhysicalDeviceConditionalRenderingFeaturesEXT>(
          props.conditional_rendering_),
       initProp<vk::PhysicalDeviceCustomBorderColorFeaturesEXT>(
          props.custom_border_color_),
       initProp<vk::PhysicalDeviceDepthClampZeroOneFeaturesEXT>(
          props.depth_clamp_zero_one_),
       initProp<vk::PhysicalDeviceDepthClipControlFeaturesEXT>(
          props.depth_clip_control_),
       initProp<vk::PhysicalDeviceDepthClipEnableFeaturesEXT>(
          props.depth_clip_enabled_),
       initProp<vk::PhysicalDeviceDescriptorBufferFeaturesEXT>(
          props.descriptor_buffer_),
       initProp<vk::PhysicalDeviceDescriptorIndexingFeatures>(
          props.descriptor_indexing_),
       initProp<vk::PhysicalDeviceDeviceMemoryReportFeaturesEXT>(
          props.device_memory_report_),
       initProp<vk::PhysicalDeviceDynamicRenderingFeatures>(
          props.dynamic_rendering_),
       initProp<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>(
          props.extended_dynamic_state_),
       initProp<vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT>(
          props.extended_dynamic_state2_),
       initProp<vk::PhysicalDeviceExtendedDynamicState3FeaturesEXT>(
          props.extended_dynamic_state3_),
       initProp<vk::PhysicalDeviceFaultFeaturesEXT>(
          props.fault_feature_),
       initProp<vk::PhysicalDeviceFragmentDensityMapFeaturesEXT>(
          props.fragment_density_map_),
       initProp<vk::PhysicalDeviceFragmentDensityMap2FeaturesEXT>(
          props.fragment_density_map2_),
       initProp<vk::PhysicalDeviceFragmentShaderBarycentricFeaturesKHR>(
          props.fragment_shader_barycentric_),
       initProp<vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>(
          props.fragment_shader_inter_lock_),
       initProp<vk::PhysicalDeviceFragmentShadingRateFeaturesKHR>(
          props.fragment_shading_rate_),
       initProp<vk::PhysicalDeviceGlobalPriorityQueryFeaturesKHR>(
          props.global_priority_query_),
       initProp<vk::PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT>(
          props.graphics_pipeline_library_),
       initProp<vk::PhysicalDeviceHostQueryResetFeatures>(
          props.host_query_reset_),
       initProp<vk::PhysicalDeviceImage2DViewOf3DFeaturesEXT>(
          props.image_2d_view_of_3d_),
       initProp<vk::PhysicalDeviceImageCompressionControlFeaturesEXT>(
          props.image_compression_control_),
       initProp<vk::PhysicalDeviceImageCompressionControlSwapchainFeaturesEXT>(
          props.image_compression_control_swapchain_),
       initProp<vk::PhysicalDeviceImageRobustnessFeatures>(
          props.image_robustness_),
       initProp<vk::PhysicalDeviceImageSlicedViewOf3DFeaturesEXT>(
          props.image_sliced_view_of_3d_),
       initProp<vk::PhysicalDeviceImageViewMinLodFeaturesEXT>(
          props.image_view_min_lod_),
       initProp<vk::PhysicalDeviceImagelessFramebufferFeatures>(
          props.imageless_framebuffer_),
       initProp<vk::PhysicalDeviceIndexTypeUint8FeaturesEXT>(
          props.index_type_uint8_),
       initProp<vk::PhysicalDeviceInlineUniformBlockFeatures>(
          props.inline_uniform_block_),
       initProp<vk::PhysicalDeviceLegacyDitheringFeaturesEXT>(
          props.legacy_dithering_),
       initProp<vk::PhysicalDeviceLineRasterizationFeaturesEXT>(
          props.line_rasterization_),
       initProp<vk::PhysicalDeviceMaintenance4Features>(
          props.maintenance4_),
       initProp<vk::PhysicalDeviceMemoryPriorityFeaturesEXT>(
          props.memory_priority_),
       initProp<vk::PhysicalDeviceMeshShaderFeaturesEXT>(
          props.mesh_shader_),
       initProp<vk::PhysicalDeviceMultiDrawFeaturesEXT>(
          props.multi_draw_),
       initProp<vk::PhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT>(
          props.multisample_render_to_single_sampled_),
       initProp<vk::PhysicalDeviceMultiviewFeatures>(
          props.multiview_),
       initProp<vk::PhysicalDeviceMutableDescriptorTypeFeaturesEXT>(
          props.mutabledescriptor_type_),
       initProp<vk::PhysicalDeviceNonSeamlessCubeMapFeaturesEXT>(
          props.non_seamless_cube_map_),
       initProp<vk::PhysicalDeviceOpacityMicromapFeaturesEXT>(
          props.opacity_micromap_),
       initProp<vk::PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT>(
          props.pageable_device_local_memory_),
       initProp<vk::PhysicalDevicePerformanceQueryFeaturesKHR>(
          props.performance_query_),
       initProp<vk::PhysicalDevicePipelineCreationCacheControlFeatures>(
          props.pipeline_creation_cache_control_),
       initProp<vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR>(
          props.pipeline_executable_properties_),
       initProp<vk::PhysicalDevicePortabilitySubsetFeaturesKHR>(
          props.portability_subset_),
       initProp<vk::PhysicalDevicePipelinePropertiesFeaturesEXT>(
          props.pipeline_properties_),
       initProp<vk::PhysicalDevicePipelineProtectedAccessFeaturesEXT>(
          props.pipeline_protected_access_),
       initProp<vk::PhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT>(
          props.pipeline_library_group_handles_),
       initProp<vk::PhysicalDevicePipelineRobustnessFeaturesEXT>(
          props.pipeline_robustness_),
       initProp<vk::PhysicalDevicePresentIdFeaturesKHR>(
          props.present_id_),
       initProp<vk::PhysicalDevicePresentWaitFeaturesKHR>(
          props.present_wait_),
       initProp<vk::PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT>(
          props.primitive_topology_list_restart_),
       initProp<vk::PhysicalDevicePrimitivesGeneratedQueryFeaturesEXT>(
          props.primitives_generated_query_),
       initProp<vk::PhysicalDevicePrivateDataFeatures>(
          props.private_data_),
       initProp<vk::PhysicalDeviceProtectedMemoryFeatures>(
          props.protected_memory_),
       initProp<vk::PhysicalDeviceProvokingVertexFeaturesEXT>(
          props.provoking_vertex_),
       initProp<vk::PhysicalDeviceRGBA10X6FormatsFeaturesEXT>(
          props.rgba10x6_formats_),
       initProp<vk::PhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT>(
          props.rasterization_order_attachment_access_),
       initProp<vk::PhysicalDeviceRayQueryFeaturesKHR>(
          props.ray_query_),
       initProp<vk::PhysicalDeviceRayTracingMaintenance1FeaturesKHR>(
          props.ray_tracing_maintenance1_),
       initProp<vk::PhysicalDeviceRayTracingPipelineFeaturesKHR>(
          props.ray_tracing_pipeline_),
       initProp<vk::PhysicalDeviceRobustness2FeaturesEXT>(
          props.robustness2_),
       initProp<vk::PhysicalDeviceSamplerYcbcrConversionFeatures>(
          props.sampler_ycbcr_conversion_),
       initProp<vk::PhysicalDeviceScalarBlockLayoutFeatures>(
          props.scalar_block_layout_),
       initProp<vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>(
          props.depth_stencil_layouts_),
       initProp<vk::PhysicalDeviceShaderAtomicFloatFeaturesEXT>(
          props.shader_atomic_float_),
       initProp<vk::PhysicalDeviceShaderAtomicFloat2FeaturesEXT>(
          props.shader_atomic_float2_),
       initProp<vk::PhysicalDeviceShaderAtomicInt64Features>(
          props.shader_atomic_int64_),
       initProp<vk::PhysicalDeviceShaderClockFeaturesKHR>(
          props.shader_clock_),
       initProp<vk::PhysicalDeviceShaderDemoteToHelperInvocationFeatures>(
          props.demote_to_helper_invocation_),
       initProp<vk::PhysicalDeviceShaderDrawParametersFeatures>(
          props.shader_draw_parameters_),
       initProp<vk::PhysicalDeviceShaderFloat16Int8Features>(
          props.shader_float16_int8_),
       initProp<vk::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT>(
          props.shader_image_atomic_int64_),
       initProp<vk::PhysicalDeviceShaderIntegerDotProductFeatures>(
          props.shader_integer_dot_product_),
       initProp<vk::PhysicalDeviceShaderModuleIdentifierFeaturesEXT>(
          props.shader_module_identifier_),
       //initProp<vk::PhysicalDeviceShaderObjectFeaturesEXT>(
       //   props.shader_object_),
       initProp<vk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>(
          props.shader_subgroup_extended_types_),
       initProp<vk::PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR>(
          props.shader_subgroup_uniform_control_flow_),
       initProp<vk::PhysicalDeviceShaderTerminateInvocationFeatures>(
          props.shader_terminate_invocation_),
       //initProp<vk::PhysicalDeviceShaderTileImageFeaturesEXT>(
       //   props.shader_tile_image_),
       initProp<vk::PhysicalDeviceSubgroupSizeControlFeatures>(
          props.subgroup_size_control_),
       initProp<vk::PhysicalDeviceSubpassMergeFeedbackFeaturesEXT>(
          props.subpass_merge_feedback_),
       initProp<vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT>(
          props.swapchain_maintenance1_),
       initProp<vk::PhysicalDeviceSynchronization2Features>(
          props.synchronization2_),
       initProp<vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>(
          props.texel_buffer_alignment_),
       initProp<vk::PhysicalDeviceTextureCompressionASTCHDRFeatures>(
          props.texture_compression_astchdr_),
       initProp<vk::PhysicalDeviceTimelineSemaphoreFeatures>(
          props.timeline_semaphore_),
       initProp<vk::PhysicalDeviceTransformFeedbackFeaturesEXT>(
          props.transform_feedback_),
       initProp<vk::PhysicalDeviceUniformBufferStandardLayoutFeatures>(
          props.uniform_buffer_standard_layout_),
       initProp<vk::PhysicalDeviceVariablePointerFeatures>(
          props.variable_pointers_),
       initProp<vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT>(
          props.vertex_attribute_divisor_),
       initProp<vk::PhysicalDeviceVertexInputDynamicStateFeaturesEXT>(
          props.vertex_input_dynamic_state_),
       initProp<vk::PhysicalDeviceVulkan11Features>(
          props.vulkan11_),
       initProp<vk::PhysicalDeviceVulkan12Features>(
          props.vulkan12_),
       initProp<vk::PhysicalDeviceVulkan13Features>(
          props.vulkan13_),
       initProp<vk::PhysicalDeviceVulkanMemoryModelFeatures>(
          props.vulkan_memory_model_),
       initProp<vk::PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR>(
          props.workgroup_memory_explicit_layout_),
       initProp<vk::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT>(
          props.ycbcr_2plane_444_formats_),
       initProp<vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>(
          props.ycbcr_image_arrays_),
       initProp<vk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures>(
          props.zero_initialize_workgroup_memory_)
       );
  d.getFeatures2(&p, dispatcher.loader());
  props.features1_ = zisc::cast<VkPhysicalDeviceFeatures>(p.features);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchLayerProperties(const VulkanDispatchLoader& dispatcher)
{
  using Props = vk::LayerProperties;
  static_assert(sizeof(VkLayerProperties) == sizeof(Props));

  const vk::PhysicalDevice d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher.loader();

  zisc::pmr::vector<VkLayerProperties>& prop_list = layer_properties_list_;
  uint32b size = 0;
  {
    const vk::Result result = d.enumerateDeviceLayerProperties(&size, nullptr, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device layer props failed.";
      vk::throwResultException(result, message);
    }
    prop_list.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto* data = zisc::reinterp<Props*>(prop_list.data());
    const vk::Result result = d.enumerateDeviceLayerProperties(&size, data, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device layer props failed.";
      vk::throwResultException(result, message);
    }
  }
  {
    const auto comp = [](const VkLayerProperties& lhs,
                         const VkLayerProperties& rhs) noexcept
    {
      return compareProperties(lhs.layerName, rhs.layerName);
    };
    std::sort(prop_list.begin(), prop_list.end(), comp);
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchMemoryProperties(const VulkanDispatchLoader& dispatcher)
{
  const vk::PhysicalDevice d{device()};

  vk::PhysicalDeviceMemoryProperties2 p;
  MemoryProperties& props = memory_properties_;
  link(p,
       initProp<vk::PhysicalDeviceMemoryBudgetPropertiesEXT>(
          props.budget_)
       );
  d.getMemoryProperties2(&p, dispatcher.loader());
  props.properties1_ =
      zisc::cast<VkPhysicalDeviceMemoryProperties>(p.memoryProperties);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchProperties(const VulkanDispatchLoader& dispatcher)
{
  const vk::PhysicalDevice d{device()};

  vk::PhysicalDeviceProperties2 p;
  Properties& props = properties_;
  link(p,
       initProp<vk::PhysicalDeviceAccelerationStructurePropertiesKHR>(
          props.acceleration_structure_),
       initProp<vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>(
          props.blend_operation_advanced_),
       initProp<vk::PhysicalDeviceConservativeRasterizationPropertiesEXT>(
          props.conservative_rasterization_),
       initProp<vk::PhysicalDeviceCustomBorderColorPropertiesEXT>(
          props.custom_border_color_),
       initProp<vk::PhysicalDeviceDepthStencilResolveProperties>(
          props.depth_stencil_resolve_),
       initProp<vk::PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT>(
          props.descriptor_buffer_density_map_),
       initProp<vk::PhysicalDeviceDescriptorBufferPropertiesEXT>(
          props.descriptor_buffer_),
       initProp<vk::PhysicalDeviceDescriptorIndexingProperties>(
          props.descriptor_indexing_),
       initProp<vk::PhysicalDeviceDiscardRectanglePropertiesEXT>(
          props.discard_rectangle_),
       initProp<vk::PhysicalDeviceDriverProperties>(
          props.driver_),
       initProp<vk::PhysicalDeviceDrmPropertiesEXT>(
          props.drm_),
       initProp<vk::PhysicalDeviceExtendedDynamicState3PropertiesEXT>(
          props.extended_dynamic_state3_),
       initProp<vk::PhysicalDeviceExternalMemoryHostPropertiesEXT>(
          props.external_memory_host_),
       initProp<vk::PhysicalDeviceFloatControlsProperties>(
          props.float_controls_),
       initProp<vk::PhysicalDeviceFragmentDensityMapPropertiesEXT>(
          props.fragment_density_map_),
       initProp<vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT>(
          props.fragment_density_map2_),
       initProp<vk::PhysicalDeviceFragmentShaderBarycentricPropertiesKHR>(
          props.fragment_shader_barycentric_),
       initProp<vk::PhysicalDeviceFragmentShadingRatePropertiesKHR>(
          props.fragment_shading_rate_),
       initProp<vk::PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT>(
          props.graphics_pipeline_library_),
       initProp<vk::PhysicalDeviceIDProperties>(
          props.id_),
       initProp<vk::PhysicalDeviceInlineUniformBlockProperties>(
          props.inline_uniform_block_),
       initProp<vk::PhysicalDeviceLineRasterizationPropertiesEXT>(
          props.line_rasterization_),
       initProp<vk::PhysicalDeviceMaintenance3Properties>(
          props.maintenance3_),
       initProp<vk::PhysicalDeviceMaintenance4Properties>(
          props.maintenance4_),
       initProp<vk::PhysicalDeviceMeshShaderPropertiesEXT>(
          props.mesh_shader_),
       initProp<vk::PhysicalDeviceMultiDrawPropertiesEXT>(
          props.multi_draw_),
       initProp<vk::PhysicalDeviceMultiviewProperties>(
          props.multiview_),
       initProp<vk::PhysicalDevicePCIBusInfoPropertiesEXT>(
          props.pci_bus_info_),
       initProp<vk::PhysicalDeviceOpacityMicromapPropertiesEXT>(
          props.opacity_micromap_),
       initProp<vk::PhysicalDevicePerformanceQueryPropertiesKHR>(
          props.performance_query_),
       initProp<vk::PhysicalDevicePipelineRobustnessPropertiesEXT>(
          props.pipeline_robustness_),
       initProp<vk::PhysicalDevicePointClippingProperties>(
          props.point_clipping_),
       initProp<vk::PhysicalDevicePortabilitySubsetPropertiesKHR>(
          props.portability_subset_),
       initProp<vk::PhysicalDeviceProtectedMemoryProperties>(
          props.protected_memory_),
       initProp<vk::PhysicalDeviceProvokingVertexPropertiesEXT>(
          props.provoking_vertex_),
       initProp<vk::PhysicalDevicePushDescriptorPropertiesKHR>(
          props.push_descriptor_),
       initProp<vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>(
          props.ray_tracing_pipeline_),
       initProp<vk::PhysicalDeviceRobustness2PropertiesEXT>(
          props.robustness2_),
       initProp<vk::PhysicalDeviceSampleLocationsPropertiesEXT>(
          props.sample_locations_),
       initProp<vk::PhysicalDeviceSamplerFilterMinmaxProperties>(
          props.sampler_filter_minmax_),
       initProp<vk::PhysicalDeviceShaderIntegerDotProductProperties>(
          props.shader_integer_dot_product_),
       initProp<vk::PhysicalDeviceShaderModuleIdentifierPropertiesEXT>(
          props.shader_module_identifier_),
//       initProp<vk::PhysicalDeviceShaderObjectPropertiesEXT>(
//          props.shader_object_),
//       initProp<vk::PhysicalDeviceShaderTileImagePropertiesEXT>(
//          props.shader_tile_image_),
       initProp<vk::PhysicalDeviceSubgroupProperties>(
          props.subgroup_),
       initProp<vk::PhysicalDeviceSubgroupSizeControlProperties>(
          props.subgroup_size_control_),
       initProp<vk::PhysicalDeviceTexelBufferAlignmentProperties>(
          props.texel_buffer_alignment_),
       initProp<vk::PhysicalDeviceTimelineSemaphoreProperties>(
          props.timeline_semaphore_),
       initProp<vk::PhysicalDeviceTransformFeedbackPropertiesEXT>(
          props.transform_feedback_),
       initProp<vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>(
          props.vertex_attribute_divisor_),
       initProp<vk::PhysicalDeviceVulkan11Properties>(
          props.vulkan11_),
       initProp<vk::PhysicalDeviceVulkan12Properties>(
          props.vulkan12_),
       initProp<vk::PhysicalDeviceVulkan13Properties>(
          props.vulkan13_)
       );
  d.getProperties2(&p, dispatcher.loader());
  props.properties1_ = zisc::cast<VkPhysicalDeviceProperties>(p.properties);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchToolProperties(const VulkanDispatchLoader& dispatcher)
{
  using Props = vk::PhysicalDeviceToolPropertiesEXT;
  static_assert(sizeof(VkPhysicalDeviceToolPropertiesEXT) == sizeof(Props));

  const vk::PhysicalDevice d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher.loader();

  uint32b size = 0;
  {
    const vk::Result result = d.getToolPropertiesEXT(&size, nullptr, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device tool props failed.";
      vk::throwResultException(result, message);
    }
    tool_properties_list_.resize(zisc::cast<std::size_t>(size));
  }
  {
    auto* data = zisc::reinterp<Props*>(tool_properties_list_.data());
    const vk::Result result = d.getToolPropertiesEXT(&size, data, loader);
    if (result != vk::Result::eSuccess) {
      const char* message = "Fetching device tool props failed.";
      vk::throwResultException(result, message);
    }
  }
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchQueueFamilyProperties(const VulkanDispatchLoader& dispatcher)
{
  using Props = vk::QueueFamilyProperties2;
  static_assert(sizeof(VkQueueFamilyProperties2) == sizeof(Props));

  const vk::PhysicalDevice d{device()};
  VulkanDispatchLoader::ConstLoaderReference loader = dispatcher.loader();

  constexpr std::size_t max_size = maxQueueFamilyCount();
  uint32b size = 0;
  {
    d.getQueueFamilyProperties(&size, nullptr, loader);
    ZIVC_ASSERT(size <= max_size, "The number of queue families exceeded the limit.");
    queue_family_properties_list_.resize(zisc::cast<std::size_t>(size));
  }

  // Initialize property chain
  std::array<vk::QueueFamilyProperties2, max_size> p_list;
  for (std::size_t i = 0; i < size; ++i) {
    vk::QueueFamilyProperties2& p = p_list[i];
    QueueFamilyProperties& props = queue_family_properties_list_[i];
    link(p,
         initProp<vk::QueueFamilyGlobalPriorityPropertiesEXT>(
            props.global_priority_),
         initProp<vk::QueueFamilyQueryResultStatusPropertiesKHR>(
            props.query_result_status_),
         initProp<vk::QueueFamilyVideoPropertiesKHR>(
            props.video_)
        );
  }

  d.getQueueFamilyProperties2(&size, p_list.data(), loader);
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
  const VkPhysicalDeviceMemoryProperties& props = memoryProperties().properties1_;
  DeviceInfo::setHeapInfoListSize(props.memoryHeapCount);
  const std::span heap_info_list = DeviceInfo::heapInfoList();
  for (std::size_t i = 0; i < heap_info_list.size(); ++i) {
    MemoryHeapInfo& info = heap_info_list[i];
    const vk::MemoryHeap heap{props.memoryHeaps[i]};
    {
      constexpr vk::MemoryHeapFlagBits dflag = vk::MemoryHeapFlagBits::eDeviceLocal;
      const bool is_device_local = (heap.flags & dflag) == dflag;
      info.setDeviceLocal(is_device_local);
    }
    info.setTotalSize(heap.size);
    {
      const VkPhysicalDeviceMemoryBudgetPropertiesEXT& budget = memoryProperties().budget_;
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
    constexpr std::size_t default_subgroup_size = 32;
    switch (props.properties1_.vendorID) {
     default: {
      subgroup_size = default_subgroup_size;
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
  const auto set_vendor_name = [this](std::string_view n) noexcept
  {
    copyStr(n, vendor_name_.data());
  };

  switch (properties().properties1_.vendorID) {
   case static_cast<uint32b>(VendorId::kAmd): {
    vendor_id_ = VendorId::kAmd;
    set_vendor_name("AMD");
    break;
   }
   case static_cast<uint32b>(VendorId::kImgTec): {
    vendor_id_ = VendorId::kImgTec;
    set_vendor_name("ImgTec");
    break;
   }
   case static_cast<uint32b>(VendorId::kNvidia): {
    vendor_id_ = VendorId::kNvidia;
    set_vendor_name("NVIDIA");
    break;
   }
   case static_cast<uint32b>(VendorId::kArm): {
    vendor_id_ = VendorId::kArm;
    set_vendor_name("ARM");
    break;
   }
   case static_cast<uint32b>(VendorId::kQualcomm): {
    vendor_id_ = VendorId::kQualcomm;
    set_vendor_name("Qualcomm");
    break;
   }
   case static_cast<uint32b>(VendorId::kIntel): {
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
