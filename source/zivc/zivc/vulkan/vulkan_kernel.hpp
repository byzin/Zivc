/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_HPP
#define ZIVC_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.h>
// Zivc
#include "zivc/kernel.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_parameters.hpp"

namespace zivc {

// Forward declaration
template <typename Type> class Buffer;
class VulkanDevice;
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class VulkanKernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
  \tparam SetType No description.
  \tparam FuncArgTypes No description.
  \tparam ArgTypes No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
class VulkanKernel<kDimension,
                   KernelParameters<SetType, FuncArgTypes...>,
                   ArgTypes...>
    : public Kernel<kDimension,
                    KernelParameters<SetType, FuncArgTypes...>,
                    ArgTypes...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<kDimension,
                            KernelParameters<SetType, FuncArgTypes...>,
                            ArgTypes...>;
  using Parameters = typename BaseKernel::Parameters;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  VulkanKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~VulkanKernel() noexcept override;


  //! Execute a kernel
  void run(ArgTypes... args, const LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Parameters& params) override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;

//  //! Bind buffers
//  void bindBuffers(std::add_lvalue_reference_t<BufferArgs>... args) noexcept;
//
//  //! Dispatch
//  void dispatch(const std::array<uint32b, kDimension> works) noexcept;
//
//  //! Get the VkBuffer of the given buffer
//  template <typename Type>
//  vk::Buffer& getVkBuffer(Type&& buffer) const noexcept;
//
//  //! Initialize a command buffer
//  void initCommandBuffer() noexcept;
//
//  //! Initialize a compute pipeline
//  void initComputePipeline(const uint32b module_index,
//                           const std::string_view kernel_name) noexcept;
//
//  //! Initialize a descriptor pool
//  void initDescriptorPool() noexcept;
//
//  //! Initialize a descriptor set
//  void initDescriptorSet() noexcept;
//
//  //! Initialize a descriptor set layout
//  void initDescriptorSetLayout() noexcept;
//
//  //! Initialize a kernel
//  void initialize(const uint32b module_index,
//                  const std::string_view kernel_name) noexcept;
//
//  //! Initialize a pipeline layout
//  void initPipelineLayout() noexcept;
//
//  //! Check if the current buffers are same as previous buffers
//  bool isSameArgs(std::add_lvalue_reference_t<BufferArgs>... args) const noexcept;


//  std::array<vk::Buffer, kNumOfBuffers> buffer_list_;
  VkDescriptorSetLayout desc_set_layout_ = VK_NULL_HANDLE;
  VkDescriptorPool desc_pool_ = VK_NULL_HANDLE;
  VkDescriptorSet desc_set_ = VK_NULL_HANDLE;
  VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
  VkPipeline pipeline_ = VK_NULL_HANDLE;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
};

} // namespace zivc

#include "vulkan_kernel-inl.hpp"

#endif // ZIVC_VULKAN_KERNEL_HPP
