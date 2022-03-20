/*!
  \file vulkan_device_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_DEVICE_INFO_INL_HPP
#define ZIVC_VULKAN_DEVICE_INFO_INL_HPP

#include "vulkan_device_info.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <span>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
const VkPhysicalDevice& VulkanDeviceInfo::device() const noexcept
{
  return device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t VulkanDeviceInfo::deviceIndex() const noexcept
{
  return device_index_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::extensionPropertiesList() noexcept
    -> std::span<VkExtensionProperties>
{
  return extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::extensionPropertiesList() const noexcept
    -> std::span<const VkExtensionProperties>
{
  return extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::features() noexcept -> Features&
{
  return features_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::features() const noexcept -> const Features&
{
  return features_;
}

/*!
  \details No detailed description

  \tparam CppType No description.
  \tparam CType No description.
  \param [in] prop No description.
  \return No description
  */
template <typename CppType, typename CType> inline
CppType& VulkanDeviceInfo::initProp(CType& prop) noexcept
{
  static_assert(sizeof(CType) == sizeof(CppType),
                "The size of CType and CppType aren't same.");
  static_assert(std::is_constructible_v<CppType, CType>,
                "The CppType isn't constructible with CType.");
  auto p = ::new (std::addressof(prop)) CppType{};
  return *p;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::layerPropertiesList() noexcept
    -> std::span<VkLayerProperties>
{
  return layer_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::layerPropertiesList() const noexcept
    -> std::span<const VkLayerProperties>
{
  return layer_properties_list_;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam Types No description.
  \param [in,out] value1 No description.
  \param [in,out] value2 No description.
  \param [in,out] values No description.
  */
template <typename Type1, typename Type2, typename ...Types> inline
void VulkanDeviceInfo::link(Type1&& value1,
                            Type2&& value2,
                            Types&&... values) noexcept
{
  constexpr std::size_t num_of_rests = 1 + sizeof...(Types);
  value1.pNext = std::addressof(value2);
  if constexpr (1 < num_of_rests) {
    link(value2, values...);
  }
  else {
    value2.pNext = nullptr;
  }
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::memoryProperties() noexcept -> MemoryProperties&
{
  return memory_properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::memoryProperties() const noexcept -> const MemoryProperties&
{
  return memory_properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::properties() noexcept -> Properties&
{
  return properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::properties() const noexcept -> const Properties&
{
  return properties_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
inline
void VulkanDeviceInfo::setDeviceIndex(const std::size_t index) noexcept
{
  device_index_ = index;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::toolPropertiesList() noexcept
    -> std::span<VkPhysicalDeviceToolPropertiesEXT>
{
  return tool_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::toolPropertiesList() const noexcept
    -> std::span<const VkPhysicalDeviceToolPropertiesEXT>
{
  return tool_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::queueFamilyPropertiesList() noexcept
    -> std::span<QueueFamilyProperties>
{
  return queue_family_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::queueFamilyPropertiesList() const noexcept
    -> std::span<const QueueFamilyProperties>
{
  return queue_family_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::vendorId() const noexcept -> VendorId
{
  return vendor_id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t VulkanDeviceInfo::maxQueueFamilyCount() noexcept
{
  return 16;
}

} // namespace zivc

#endif // ZIVC_VULKAN_DEVICE_INFO_INL_HPP
