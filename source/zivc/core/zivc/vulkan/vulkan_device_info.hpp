/*!
  \file vulkan_device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_INFO_HPP
#define ZIVC_VULKAN_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <span>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/id_data.hpp"

namespace zivc {

// Forward declaration
class VulkanDispatchLoader;

/*!
  \brief Properties and features of a vulkan device

  No detailed description.
  */
class VulkanDeviceInfo : public DeviceInfo
{
 public:
  //! The Vendor IDs of vulkan device
  enum class VendorId : uint32b
  {
    kUnknown = 0,
    kAmd = 0x1002, // AMD
    kImgTec = 0x1010, // ImgTec
    kNvidia = 0x10de, // NVIDIA
    kArm = 0x13b5, // ARM
    kQualcomm = 0x5143, // Qualcomm
    kIntel = 0x8086 // INTEL
  };

  //! Properties of a device
  struct Properties
  {
    VkPhysicalDeviceProperties properties1_;
    VkPhysicalDeviceAccelerationStructurePropertiesKHR acceleration_structure_;
    VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT blend_operation_advanced_;
    VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservative_rasterization_;
    VkPhysicalDeviceCustomBorderColorPropertiesEXT custom_border_color_;
    VkPhysicalDeviceDepthStencilResolveProperties depth_stencil_resolve_;
    VkPhysicalDeviceDescriptorIndexingProperties descriptor_indexing_;
    VkPhysicalDeviceDiscardRectanglePropertiesEXT discard_rectangle_;
    VkPhysicalDeviceDriverProperties driver_;
    VkPhysicalDeviceDrmPropertiesEXT drm_;
    VkPhysicalDeviceExtendedDynamicState3PropertiesEXT extended_dynamic_state3_;
    VkPhysicalDeviceExternalMemoryHostPropertiesEXT external_memory_host_;
    VkPhysicalDeviceFloatControlsProperties float_controls_;
    VkPhysicalDeviceFragmentDensityMapPropertiesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2PropertiesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR fragment_shader_barycentric_;
    VkPhysicalDeviceFragmentShadingRatePropertiesKHR fragment_shading_rate_;
    VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT graphics_pipeline_library_;
    VkPhysicalDeviceIDProperties id_;
    VkPhysicalDeviceInlineUniformBlockProperties inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationPropertiesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance3Properties maintenance3_;
    VkPhysicalDeviceMaintenance4Properties maintenance4_;
    VkPhysicalDeviceMeshShaderPropertiesEXT mesh_shader_;
    VkPhysicalDeviceMultiDrawPropertiesEXT multi_draw_;
    VkPhysicalDeviceMultiviewProperties multiview_;
    VkPhysicalDeviceOpacityMicromapPropertiesEXT opacity_micromap_;
    VkPhysicalDevicePCIBusInfoPropertiesEXT pci_bus_info_;
    VkPhysicalDevicePerformanceQueryPropertiesKHR performance_query_;
    VkPhysicalDevicePipelineRobustnessPropertiesEXT pipeline_robustness_;
    VkPhysicalDevicePointClippingProperties point_clipping_;
    VkPhysicalDevicePortabilitySubsetPropertiesKHR portability_subset_;
    VkPhysicalDeviceProtectedMemoryProperties protected_memory_;
    VkPhysicalDeviceProvokingVertexPropertiesEXT provoking_vertex_;
    VkPhysicalDevicePushDescriptorPropertiesKHR push_descriptor_;
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR ray_tracing_pipeline_;
    VkPhysicalDeviceRobustness2PropertiesEXT robustness2_;
    VkPhysicalDeviceSampleLocationsPropertiesEXT sample_locations_;
    VkPhysicalDeviceSamplerFilterMinmaxProperties sampler_filter_minmax_;
    VkPhysicalDeviceShaderIntegerDotProductProperties shader_integer_dot_product_;
    VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT shader_module_identifier_;
    VkPhysicalDeviceSubgroupProperties subgroup_;
    VkPhysicalDeviceSubgroupSizeControlProperties subgroup_size_control_;
    VkPhysicalDeviceTexelBufferAlignmentProperties texel_buffer_alignment_;
    VkPhysicalDeviceTimelineSemaphoreProperties timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackPropertiesEXT transform_feedback_;
    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVulkan11Properties vulkan11_;
    VkPhysicalDeviceVulkan12Properties vulkan12_;
    VkPhysicalDeviceVulkan13Properties vulkan13_;
  };

  //! Features of a device
  struct Features
  {
    VkPhysicalDeviceFeatures features1_;
    [[maybe_unused]] Padding<4> pad_;
    VkPhysicalDevice16BitStorageFeatures b16bit_storage_;
    VkPhysicalDevice4444FormatsFeaturesEXT argb_4444_formats_;
    VkPhysicalDevice8BitStorageFeatures b8bit_storage_;
    VkPhysicalDeviceASTCDecodeFeaturesEXT astc_decode_;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_;
    VkPhysicalDeviceAddressBindingReportFeaturesEXT address_binding_report_;
    VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT attachment_feedback_loop_layout_;
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT blend_operation_advanced_;
    VkPhysicalDeviceBorderColorSwizzleFeaturesEXT border_color_swizzle_;
    VkPhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_;
    VkPhysicalDeviceColorWriteEnableFeaturesEXT color_write_enable_;
    VkPhysicalDeviceConditionalRenderingFeaturesEXT conditional_rendering_;
    VkPhysicalDeviceCustomBorderColorFeaturesEXT custom_border_color_;
    VkPhysicalDeviceDepthClampZeroOneFeaturesEXT depth_clamp_zero_one_;
    VkPhysicalDeviceDepthClipControlFeaturesEXT depth_clip_control_;
    VkPhysicalDeviceDepthClipEnableFeaturesEXT depth_clip_enabled_;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_;
    VkPhysicalDeviceDeviceMemoryReportFeaturesEXT device_memory_report_;
    VkPhysicalDeviceDynamicRenderingFeatures dynamic_rendering_;
    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_;
    VkPhysicalDeviceExtendedDynamicState2FeaturesEXT extended_dynamic_state2_;
    VkPhysicalDeviceExtendedDynamicState3FeaturesEXT extended_dynamic_state3_;
    VkPhysicalDeviceFaultFeaturesEXT fault_feature_;
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2FeaturesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR fragment_shader_barycentric_;
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragment_shader_inter_lock_;
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragment_shading_rate_;
    VkPhysicalDeviceGlobalPriorityQueryFeaturesKHR global_priority_query_;
    VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT graphics_pipeline_library_;
    VkPhysicalDeviceHostQueryResetFeatures host_query_reset_;
    VkPhysicalDeviceImage2DViewOf3DFeaturesEXT image_2d_view_of_3d_;
    VkPhysicalDeviceImageCompressionControlFeaturesEXT image_compression_control_;
    VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT image_compression_control_swapchain_;
    VkPhysicalDeviceImageRobustnessFeatures image_robustness_;
    VkPhysicalDeviceImageViewMinLodFeaturesEXT image_view_min_lod_;
    VkPhysicalDeviceImagelessFramebufferFeatures imageless_framebuffer_;
    VkPhysicalDeviceIndexTypeUint8FeaturesEXT index_type_uint8_;
    VkPhysicalDeviceInlineUniformBlockFeatures inline_uniform_block_;
    VkPhysicalDeviceLegacyDitheringFeaturesEXT legacy_dithering_;
    VkPhysicalDeviceLineRasterizationFeaturesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance4Features maintenance4_;
    VkPhysicalDeviceMemoryPriorityFeaturesEXT memory_priority_;
    VkPhysicalDeviceMeshShaderFeaturesEXT mesh_shader_;
    VkPhysicalDeviceMultiDrawFeaturesEXT multi_draw_;
    VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT multisample_render_to_single_sampled_;
    VkPhysicalDeviceMultiviewFeatures multiview_;
    VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT mutabledescriptor_type_;
    VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT non_seamless_cube_map_;
    VkPhysicalDeviceOpacityMicromapFeaturesEXT opacity_micromap_;
    VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT pageable_device_local_memory_;
    VkPhysicalDevicePerformanceQueryFeaturesKHR performance_query_;
    VkPhysicalDevicePipelineCreationCacheControlFeatures pipeline_creation_cache_control_;
    VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR pipeline_executable_properties_;
    VkPhysicalDevicePipelinePropertiesFeaturesEXT pipeline_properties_;
    VkPhysicalDevicePipelineProtectedAccessFeaturesEXT pipeline_protected_access_;
    VkPhysicalDevicePipelineRobustnessFeaturesEXT pipeline_robustness_;
    VkPhysicalDevicePortabilitySubsetFeaturesKHR portability_subset_;
    VkPhysicalDevicePresentIdFeaturesKHR present_id_;
    VkPhysicalDevicePresentWaitFeaturesKHR present_wait_;
    VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT primitive_topology_list_restart_;
    VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT primitives_generated_query_;
    VkPhysicalDevicePrivateDataFeatures private_data_;
    VkPhysicalDeviceProtectedMemoryFeatures protected_memory_;
    VkPhysicalDeviceProvokingVertexFeaturesEXT provoking_vertex_;
    VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT rasterization_order_attachment_access_;
    VkPhysicalDeviceRayQueryFeaturesKHR ray_query_;
    VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR ray_tracing_maintenance1_;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_;
    VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT rgba10x6_formats;
    VkPhysicalDeviceRobustness2FeaturesEXT robustness2_;
    VkPhysicalDeviceSamplerYcbcrConversionFeatures sampler_ycbcr_conversion_;
    VkPhysicalDeviceScalarBlockLayoutFeatures scalar_block_layout_;
    VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures depth_stencil_layouts_;
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT shader_atomic_float2_;
    VkPhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    VkPhysicalDeviceShaderClockFeaturesKHR shader_clock_;
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures demote_to_helper_invocation_;
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_;
    VkPhysicalDeviceShaderFloat16Int8Features shader_float16_int8_;
    VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shader_image_atomic_int64_;
    VkPhysicalDeviceShaderIntegerDotProductFeatures shader_integer_dot_product_;
    VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT shader_module_identifier_;
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures shader_subgroup_extended_types_;
    VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR shader_subgroup_uniform_control_flow_;
    VkPhysicalDeviceShaderTerminateInvocationFeatures shader_terminate_invocation_;
    VkPhysicalDeviceSubgroupSizeControlFeatures subgroup_size_control_;
    VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT subpass_merge_feedback_;
    VkPhysicalDeviceSynchronization2Features synchronization2_;
    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTextureCompressionASTCHDRFeatures texture_compression_astchdr_;
    VkPhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackFeaturesEXT transform_feedback_;
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures uniform_buffer_standard_layout_;
    VkPhysicalDeviceVariablePointersFeatures variable_pointers_;
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT vertex_input_dynamic_state_;
    VkPhysicalDeviceVulkan11Features vulkan11_;
    VkPhysicalDeviceVulkan12Features vulkan12_;
    VkPhysicalDeviceVulkan13Features vulkan13_;
    VkPhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_model_;
    VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR workgroup_memory_explicit_layout_;
    VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT ycbcr_2plane_444_formats_;
    VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ycbcr_image_arrays_;
    VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures zero_initialize_workgroup_memory_;
  };

  //! Memory properties of a device
  struct MemoryProperties
  {
    VkPhysicalDeviceMemoryProperties properties1_;
    VkPhysicalDeviceMemoryBudgetPropertiesEXT budget_;
  };

  //! Queue family properties of a device
  struct QueueFamilyProperties
  {
    VkQueueFamilyProperties properties1_;
    VkQueueFamilyGlobalPriorityPropertiesEXT global_priority_;
    VkQueueFamilyQueryResultStatusPropertiesKHR query_result_status_;
    VkQueueFamilyVideoPropertiesKHR video_;
  };

  //! Format properties
  struct FormatProperties
  {
    VkFormatProperties properties1_;
    VkDrmFormatModifierPropertiesListEXT drm_format_modifier_;
    VkDrmFormatModifierPropertiesList2EXT drm_format_modifier2_;
    VkFormatProperties3 properties3_;
    VkSubpassResolvePerformanceQueryEXT subpass_resolve_performance_;
  };


  //! Initialize the device info
  VulkanDeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept;

  //! Finalize the device info
  ~VulkanDeviceInfo() noexcept override;


  //! Move a data
  VulkanDeviceInfo& operator=(VulkanDeviceInfo&& other) noexcept;


  //! Return the underlying vulkan physical device
  const VkPhysicalDevice& device() const noexcept;

  //! Return the device index in the backend
  std::size_t deviceIndex() const noexcept;

  //! Return extension properties list of the device
  std::span<VkExtensionProperties> extensionPropertiesList() noexcept;

  //! Return extension properties list of the device
  std::span<const VkExtensionProperties> extensionPropertiesList() const noexcept;

  //! Return features of the device
  Features& features() noexcept;

  //! Return features of the device
  const Features& features() const noexcept;

  //! Fetch device info from a physical device
  void fetch(const VkPhysicalDevice& vdevice,
             const VulkanDispatchLoader& dispatcher);

  //! Initialize a property with the given type
  template <typename CppType, typename CType>
  static CppType& initProp(CType& prop) noexcept;

  //! Check if the given extension is supported in the device
  bool isExtensionSupported(const std::string_view name) const noexcept;

  //! Check if the given layer is supported in the device
  bool isLayerSupported(const std::string_view name) const noexcept;

  //! Return layer properties list of the device
  std::span<VkLayerProperties> layerPropertiesList() noexcept;

  //! Return layer properties list of the device
  std::span<const VkLayerProperties> layerPropertiesList() const noexcept;

  template <typename Type1, typename Type2, typename ...Types>
  static void link(Type1&& value1, Type2&& value2, Types&&... values) noexcept;

  //! Return the possible maximum size of an allocation
  std::size_t maxAllocationSize() const noexcept override;

  //! Return the possible maximum number of buffer arguments per kernel
  std::size_t maxNumOfBuffersPerKernel() const noexcept override;

  //! Return the maximum work group count
  std::array<uint32b, 3> maxWorkGroupCount() const noexcept override;

  //! Return memory properties of the device
  MemoryProperties& memoryProperties() noexcept;

  //! Return memory properties of the device
  const MemoryProperties& memoryProperties() const noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return properties of the device
  Properties& properties() noexcept;

  //! Return properties of the device
  const Properties& properties() const noexcept;

  //! Set the device index in the backend
  void setDeviceIndex(const std::size_t index) noexcept;

  //! Return tool properties list of the device
  std::span<VkPhysicalDeviceToolPropertiesEXT> toolPropertiesList() noexcept;

  //! Return tool properties list of the device
  std::span<const VkPhysicalDeviceToolPropertiesEXT> toolPropertiesList() const noexcept;

  //! Return the backend type
  BackendType type() const noexcept override;

  //! Return queue family properties list of the device
  std::span<QueueFamilyProperties> queueFamilyPropertiesList() noexcept;

  //! Return queue family properties list of the device
  std::span<const QueueFamilyProperties> queueFamilyPropertiesList() const noexcept;

  //! Return the vendor ID
  VendorId vendorId() const noexcept;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Return the local work group size of the device
  uint32b workGroupSize() const noexcept override;

 private:
  //! Compare two properties by it's name
  static bool compareProperties(const std::string_view lhs,
                                const std::string_view rhs) noexcept;

  //! Fetch extension properties from a physical device
  void fetchExtensionProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch features from a physical device
  void fetchFeatures(const VulkanDispatchLoader& dispatcher);

  //! Fetch layer properties from a physical device
  void fetchLayerProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch memory properties from a physical device
  void fetchMemoryProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch properties from a physical device
  void fetchProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch tool properties from a physical device
  void fetchToolProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch queue family properties from a physical device
  void fetchQueueFamilyProperties(const VulkanDispatchLoader& dispatcher);

  //! Initialize the heap info list
  void initHeapInfoList() noexcept;

  //! Initialize the subgroup size of the device
  void initSubgroupSize() noexcept;

  //! Initialize the vendor info
  void initVendorInfo() noexcept;

  //! Return the maximum possible the number of queue families
  static constexpr std::size_t maxQueueFamilyCount() noexcept;


  zisc::pmr::vector<VkExtensionProperties> extension_properties_list_;
  zisc::pmr::vector<VkLayerProperties> layer_properties_list_;
  zisc::pmr::vector<QueueFamilyProperties> queue_family_properties_list_;
  zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT> tool_properties_list_;
  VkPhysicalDevice device_;
  std::size_t device_index_ = 0;
  IdData::NameT vendor_name_;
  VendorId vendor_id_;
  uint32b subgroup_size_ = 0;
  Properties properties_;
  Features features_;
  MemoryProperties memory_properties_;
  // FormatProperties
  // VkImageFormatProperties2 
  // VkSparseImageFormatProperties2
  // VkExternalBufferProperties
  // VkExternalSemaphoreProperties
  // VkQueryPoolPerformanceCreateInfoKHR
  // VkMultisamplePropertiesEXT
};

} // namespace zivc

#include "vulkan_device_info-inl.hpp"

#endif // ZIVC_VULKAN_DEVICE_INFO_HPP
