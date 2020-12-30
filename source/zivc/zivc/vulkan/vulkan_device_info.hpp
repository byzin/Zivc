/*!
  \file vulkan_device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
    VkPhysicalDeviceCustomBorderColorPropertiesEXT custom_border_color_;
    VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservative_rasterization_;
    VkPhysicalDeviceDepthStencilResolveProperties depth_stencil_resolve_;
    VkPhysicalDeviceDescriptorIndexingProperties descriptor_indexing_;
    VkPhysicalDeviceDiscardRectanglePropertiesEXT discard_rectangle_;
    VkPhysicalDeviceDriverProperties driver_;
    VkPhysicalDeviceExternalMemoryHostPropertiesEXT external_memory_host_;
    VkPhysicalDeviceFloatControlsProperties float_controls_;
    VkPhysicalDeviceFragmentDensityMapPropertiesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2PropertiesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShadingRatePropertiesKHR fragment_shading_rate_;
    VkPhysicalDeviceIDProperties id_properties_;
    VkPhysicalDeviceInlineUniformBlockPropertiesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationPropertiesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance3Properties maintenance3_;
    VkPhysicalDeviceMultiviewProperties multiview_;
    VkPhysicalDevicePCIBusInfoPropertiesEXT pci_bus_info_;
    VkPhysicalDevicePerformanceQueryPropertiesKHR performance_query_;
    VkPhysicalDevicePointClippingProperties point_clipping_;
//    VkPhysicalDevicePortabilitySubsetPropertiesKHR portability_subset_;
    VkPhysicalDeviceProtectedMemoryProperties protected_memory_;
    VkPhysicalDevicePushDescriptorPropertiesKHR push_descriptor_;
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR ray_tracing_pipeline_;
    VkPhysicalDeviceRobustness2PropertiesEXT robustness2_;
    VkPhysicalDeviceSampleLocationsPropertiesEXT sample_locations_;
    VkPhysicalDeviceSamplerFilterMinmaxProperties sampler_filter_minmax_;
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
    [[maybe_unused]] uint32b padding_;
    VkPhysicalDevice16BitStorageFeatures b16bit_storage_;
    VkPhysicalDevice4444FormatsFeaturesEXT argb_4444_formats_;
    VkPhysicalDevice8BitStorageFeatures b8bit_storage_;
    VkPhysicalDeviceASTCDecodeFeaturesEXT astc_decode_;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_;
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT blend_operation_advanced_;
    VkPhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_;
    VkPhysicalDeviceConditionalRenderingFeaturesEXT conditional_rendering_;
    VkPhysicalDeviceCustomBorderColorFeaturesEXT custom_border_color_;
    VkPhysicalDeviceDepthClipEnableFeaturesEXT depth_clip_enabled_;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_;
    VkPhysicalDeviceDeviceMemoryReportFeaturesEXT device_memory_report_;
    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_;
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentDensityMap2FeaturesEXT fragment_density_map2_;
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragment_shader_inter_lock_;
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragment_shading_rate_;
    VkPhysicalDeviceHostQueryResetFeatures host_query_reset_;
    VkPhysicalDeviceImageRobustnessFeaturesEXT image_robustness_;
    VkPhysicalDeviceImagelessFramebufferFeatures imageless_framebuffer_;
    VkPhysicalDeviceIndexTypeUint8FeaturesEXT index_type_uint8_;
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationFeaturesEXT line_rasterization_;
    VkPhysicalDeviceMemoryPriorityFeaturesEXT memory_priority_features_;
    VkPhysicalDeviceMultiviewFeatures multiview_;
    VkPhysicalDevicePerformanceQueryFeaturesKHR performance_query_;
    VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT pipeline_creation_cache_control_;
    VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR pipeline_executable_properties_;
//    VkPhysicalDevicePortabilitySubsetFeaturesKHR portability_subset_;
    VkPhysicalDevicePrivateDataFeaturesEXT private_data_features_;
    VkPhysicalDeviceProtectedMemoryFeatures protected_memory_;
    VkPhysicalDeviceRayQueryFeaturesKHR ray_query_;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_features_;
    VkPhysicalDeviceRobustness2FeaturesEXT robustness2_;
    VkPhysicalDeviceSamplerYcbcrConversionFeatures sampler_ycbcr_conversion_;
    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT scalar_block_layout_;
    VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures depth_stencil_layouts_;
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float_;
    VkPhysicalDeviceShaderAtomicInt64Features shader_atomic_int64_;
    VkPhysicalDeviceShaderClockFeaturesKHR shader_clock_;
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT demote_to_helper_invocation_;
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_;
    VkPhysicalDeviceShaderFloat16Int8FeaturesKHR shader_float16_int8_;
    VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shader_image_atomic_int64_;
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures shader_subgroup_extended_types_;
    VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR shader_terminate_invocation_;
    VkPhysicalDeviceSubgroupSizeControlFeaturesEXT subgroup_size_control_;
    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT texture_compression_astchdr_;
    VkPhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackFeaturesEXT transform_feedback_;
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures uniform_buffer_standard_layout_;
    VkPhysicalDeviceVariablePointersFeatures variable_pointers_;
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVulkan11Features vulkan11_;
    VkPhysicalDeviceVulkan12Features vulkan12_;
    VkPhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_model_;
    VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ycbcr_image_arrays_;
  };

  //! Memory properties of a device
  struct MemoryProperties
  {
    VkPhysicalDeviceMemoryProperties properties1_;
    VkPhysicalDeviceMemoryBudgetPropertiesEXT budget_;
  };

  //! Initialize the device info
  VulkanDeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept;

  //! Finalize the device info
  ~VulkanDeviceInfo() noexcept override;


  //! Move a data
  VulkanDeviceInfo& operator=(VulkanDeviceInfo&& other) noexcept;


  //! Return the amount of actual device memory currently available in bytes
  std::size_t availableMemory(const std::size_t heap_index) const noexcept override;

  //! Return the underlying vulkan physical device
  const VkPhysicalDevice& device() const noexcept;

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

  //! Return the index of the vulkan heap that is device local of the number-th
  std::size_t getDeviceHeapIndex(const std::size_t number) const noexcept;

  //! Return the number of the vulkan heap by the given index
  std::size_t getDeviceHeapNumber(const std::size_t index) const noexcept;

  //! Initialize a property with the given type
  template <typename CppType, typename CType>
  static CppType& initProp(CType& prop) noexcept;

  //! Return layer properties list of the device
  zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() noexcept;

  //! Return layer properties list of the device
  const zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() const noexcept;

  template <typename Type1, typename Type2, typename ...Types>
  static void link(Type1&& value1, Type2&& value2, Types&&... values) noexcept;

  //! Return the maximum size of an allocation
  std::size_t maxAllocationSize() const noexcept override;

  //! Return the maximum work group count
  std::array<uint32b, 3> maxWorkGroupCount() const noexcept override;

  //! Return memory properties of the device
  MemoryProperties& memoryProperties() noexcept;

  //! Return memory properties of the device
  const MemoryProperties& memoryProperties() const noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return the number of heaps of the device local
  std::size_t numOfHeaps() const noexcept override;

  //! Return properties of the device
  Properties& properties() noexcept;

  //! Return properties of the device
  const Properties& properties() const noexcept;

  //! Return tool properties list of the device
  zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT>& toolPropertiesList() noexcept;

  //! Return tool properties list of the device
  const zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT>& toolPropertiesList() const noexcept;

  //! Return the amount of actual device memory in bytes
  std::size_t totalMemory(const std::size_t heap_index) const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Return queue family properties list of the device
  zisc::pmr::vector<VkQueueFamilyProperties>& queueFamilyPropertiesList() noexcept;

  //! Return queue family properties list of the device
  const zisc::pmr::vector<VkQueueFamilyProperties>& queueFamilyPropertiesList() const noexcept;

  //! Return the vendor ID
  VendorId vendorId() const noexcept;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Return the local work group size of the device
  uint32b workGroupSize() const noexcept override;

 private:
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

  //! Find the indices of device load heaps
  void findDeviceLocalHeaps() noexcept;

  //! Initialize the subgroup size of the device
  void initSubgroupSize() noexcept;

  //! Initialize the vendor info
  void initVendorInfo() noexcept;


  zisc::pmr::vector<VkExtensionProperties> extension_properties_list_;
  zisc::pmr::vector<VkLayerProperties> layer_properties_list_;
  zisc::pmr::vector<VkQueueFamilyProperties> queue_family_properties_list_;
  zisc::pmr::vector<VkPhysicalDeviceToolPropertiesEXT> tool_properties_list_;
  // VkDisplayPlaneProperties2KHR;
  // VkDisplayProperties2KHR;
  // VkExternalBufferProperties;
  // VkExternalFenceProperties;
  // VkExternalImageFormatProperties;
  // VkExternalSemaphoreProperties;
  zisc::pmr::vector<std::size_t> device_local_index_list_;
  VkPhysicalDevice device_;
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
