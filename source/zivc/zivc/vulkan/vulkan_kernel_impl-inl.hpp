/*!
  \file vulkan_kernel_impl-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_INPL_INL_HPP
#define ZIVC_VULKAN_KERNEL_INPL_INL_HPP

#include "vulkan_kernel_impl.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] command_buffer No description.
  \param [in] pipeline_layout No description.
  \param [in] offset No description.
  \param [in] data No description.
  */
template <typename Type> inline
void VulkanKernelImpl::pushConstantCmd(const VkCommandBuffer& command_buffer,
                                       const VkPipelineLayout& pipeline_layout,
                                       const std::size_t offset,
                                       const Type& data)
{
  constexpr std::size_t size = sizeof(data);
  pushConstantCmd(command_buffer,
                  pipeline_layout,
                  offset,
                  size,
                  std::addressof(data));
}

/*!
  \details No detailed description

  \tparam kN No description.
  \param [in] descriptor_set No description.
  \param [in] buffer_list No description.
  \param [in] desc_type_list No description.
  */
template <std::size_t kN> inline
void VulkanKernelImpl::updateDescriptorSet(
    const VkDescriptorSet& descriptor_set,
    const std::array<VkBuffer, kN>& buffer_list,
    const std::array<VkDescriptorType, kN>& desc_type_list)
{
  std::array<VkDescriptorBufferInfo, kN> desc_info_list{};
  std::array<VkWriteDescriptorSet, kN> write_desc_list{};
  updateDescriptorSet(descriptor_set,
                      kN,
                      buffer_list.data(),
                      desc_type_list.data(),
                      desc_info_list.data(),
                      write_desc_list.data());
}

} // namespace zivc

#endif // ZIVC_VULKAN_KERNEL_INPL_INL_HPP
