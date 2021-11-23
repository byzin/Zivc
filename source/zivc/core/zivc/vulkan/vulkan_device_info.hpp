/*!
  \file vulkan_device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_INFO_HPP
#define ZIVC_VULKAN_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"

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
    VkPhysicalDeviceExternalMemoryHostPropertiesEXT external_memory_host_;
    VkPhysicalDeviceFloatControlsProperties float_controls_;
    VkPhysicalDeviceFragmentDensityMapPropertiesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2PropertiesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShadingRatePropertiesKHR fragment_shading_rate_;
    VkPhysicalDeviceIDProperties id_;
    VkPhysicalDeviceInlineUniformBlockPropertiesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationPropertiesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance3Properties maintenance3_;
    VkPhysicalDeviceMaintenance4PropertiesKHR maintenance4_;
    VkPhysicalDeviceMultiDrawPropertiesEXT multi_draw_;
    VkPhysicalDeviceMultiviewProperties multiview_;
    VkPhysicalDevicePCIBusInfoPropertiesEXT pci_bus_info_;
    VkPhysicalDevicePerformanceQueryPropertiesKHR performance_query_;
    VkPhysicalDevicePointClippingProperties point_clipping_;
    VkPhysicalDevicePortabilitySubsetPropertiesKHR portability_subset_;
    VkPhysicalDeviceProtectedMemoryProperties protected_memory_;
    VkPhysicalDeviceProvokingVertexPropertiesEXT provoking_vertex_;
    VkPhysicalDevicePushDescriptorPropertiesKHR push_descriptor_;
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR ray_tracing_pipeline_;
    VkPhysicalDeviceRobustness2PropertiesEXT robustness2_;
    VkPhysicalDeviceSampleLocationsPropertiesEXT sample_locations_;
    VkPhysicalDeviceSamplerFilterMinmaxProperties sampler_filter_minmax_;
    VkPhysicalDeviceShaderIntegerDotProductPropertiesKHR shader_integer_dot_product_;
    VkPhysicalDeviceSubgroupProperties subgroup_;
    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT subgroup_size_control_;
    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTimelineSemaphoreProperties timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackPropertiesEXT transform_feedback_;
    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVulkan11Properties vulkan11_;
    VkPhysicalDeviceVulkan12Properties vulkan12_;
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
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT blend_operation_advanced_;
    VkPhysicalDeviceBorderColorSwizzleFeaturesEXT border_color_swizzle_;
    VkPhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_;
    VkPhysicalDeviceColorWriteEnableFeaturesEXT color_write_enable_;
    VkPhysicalDeviceConditionalRenderingFeaturesEXT conditional_rendering_;
    VkPhysicalDeviceCustomBorderColorFeaturesEXT custom_border_color_;
    VkPhysicalDeviceDepthClipEnableFeaturesEXT depth_clip_enabled_;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_;
    VkPhysicalDeviceDeviceMemoryReportFeaturesEXT device_memory_report_;
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_;
    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_;
    VkPhysicalDeviceExtendedDynamicState2FeaturesEXT extended_dynamic_state2_;
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2FeaturesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragment_shader_inter_lock_;
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragment_shading_rate_;
    VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT global_priority_query_;
    VkPhysicalDeviceHostQueryResetFeatures host_query_reset_;
    VkPhysicalDeviceImageRobustnessFeaturesEXT image_robustness_;
    VkPhysicalDeviceImagelessFramebufferFeatures imageless_framebuffer_;
    VkPhysicalDeviceIndexTypeUint8FeaturesEXT index_type_uint8_;
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationFeaturesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance4FeaturesKHR maintenance4_;
    VkPhysicalDeviceMemoryPriorityFeaturesEXT memory_priority_features_;
    VkPhysicalDeviceMultiDrawFeaturesEXT multi_draw_;
    VkPhysicalDeviceMultiviewFeatures multiview_;
    VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT pageable_device_local_memory_;
    VkPhysicalDevicePerformanceQueryFeaturesKHR performance_query_;
    VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT pipeline_creation_cache_control_;
    VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR pipeline_executable_properties_;
    VkPhysicalDevicePortabilitySubsetFeaturesKHR portability_subset_;
    VkPhysicalDevicePresentIdFeaturesKHR present_id_;
    VkPhysicalDevicePresentWaitFeaturesKHR present_wait_;
    VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT primitive_topology_list_restart_;
    VkPhysicalDevicePrivateDataFeaturesEXT private_data_features_;
    VkPhysicalDeviceProtectedMemoryFeatures protected_memory_;
    VkPhysicalDeviceProvokingVertexFeaturesEXT provoking_vertex_;
    VkPhysicalDeviceRayQueryFeaturesKHR ray_query_;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_features_;
    VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT rgba10x6_formats;
    VkPhysicalDeviceRobustness2FeaturesEXT robustness2_;
    VkPhysicalDeviceSamplerYcbcrConversionFeatures sampler_ycbcr_conversion_;
    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT scalar_block_layout_;
    VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures depth_stencil_layouts_;
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT shader_atomic_float2_;
    VkPhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    VkPhysicalDeviceShaderClockFeaturesKHR shader_clock_;
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT demote_to_helper_invocation_;
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_;
    VkPhysicalDeviceShaderFloat16Int8FeaturesKHR shader_float16_int8_;
    VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shader_image_atomic_int64_;
    VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR shader_integer_dot_product_;
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures shader_subgroup_extended_types_;
    VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR shader_subgroup_uniform_control_flow_;
    VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR shader_terminate_invocation_;
    VkPhysicalDeviceSubgroupSizeControlFeaturesEXT subgroup_size_control_;
    VkPhysicalDeviceSynchronization2FeaturesKHR synchronization2_;
    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT texture_compression_astchdr_;
    VkPhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackFeaturesEXT transform_feedback_;
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures uniform_buffer_standard_layout_;
    VkPhysicalDeviceVariablePointersFeatures variable_pointers_;
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT vertex_input_dynamic_state_;
    VkPhysicalDeviceVulkan11Features vulkan11_;
    VkPhysicalDeviceVulkan12Features vulkan12_;
    VkPhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_model_;
    VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR workgroup_memory_explicit_layout_;
    VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT ycbcr_2plane_444_formats_;
    VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ycbcr_image_arrays_;
    VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR zero_initialize_workgroup_memory_;
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

  //! Return the device index in the sub-platform
  std::size_t deviceIndex() const noexcept;

  //! Return extension properties list of the device
  zisc::pmr::vector<VkExtensionProperties>& extensionPropertiesList() noexcept;

  //! Return extension properties list of the device
  const zisc::pmr::vector<VkExtensionProperties>& extensionPropertiesList() const noexcept;

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
  zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() noexcept;

  //! Return layer properties list of the device
  const zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() const noexcept;

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

  //! Set the device index in the sub-platform
  void setDeviceIndex(const std::size_t index) noexcept;

  //! Return tool properties list of the device
  zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT>& toolPropertiesList() noexcept;

  //! Return tool properties list of the device
  const zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT>& toolPropertiesList() const noexcept;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Return queue family properties list of the device
  zisc::pmr::vector<QueueFamilyProperties>& queueFamilyPropertiesList() noexcept;

  //! Return queue family properties list of the device
  const zisc::pmr::vector<QueueFamilyProperties>& queueFamilyPropertiesList() const noexcept;

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
  // VkDisplayPlaneProperties2KHR;
  // VkDisplayProperties2KHR;
  // VkExternalBufferProperties;
  // VkExternalFenceProperties;
  // VkExternalImageFormatProperties;
  // VkExternalSemaphoreProperties;
  VkPhysicalDevice device_;
  std::size_t device_index_ = 0;
  IdData::NameType vendor_name_;
  VendorId vendor_id_;
  uint32b subgroup_size_ = 0;
  Properties properties_;
  Features features_;
  MemoryProperties memory_properties_;
  // getFormatProperties2
  // getFragmentShadingRatesKHR
  // getImageFormatProperties2
  // getMultisampleProperties
  // getQueueFamilyPerformanceQueryPassesKHR
  // getSparseImageFormatProperties2
  // getSurfaceCapabilities2KHR
  // getSurfaceFormats2
  // getSurfacePresentModesKHR
  // getSurfaceSupportKHR
};

} // namespace zivc

#include "vulkan_device_info-inl.hpp"

#endif // ZIVC_VULKAN_DEVICE_INFO_HPP
