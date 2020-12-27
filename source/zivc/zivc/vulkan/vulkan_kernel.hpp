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
#include <tuple>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.h>
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;
template <std::size_t, DerivedKSet, typename...> class KernelParams;
template <typename, typename...> class VulkanKernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam FuncArgs No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
class VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...> :
    public Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>;
  using Params = typename BaseKernel::Params;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  VulkanKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~VulkanKernel() noexcept override;


  //! Return the command buffer
  VkCommandBuffer& commandBuffer() noexcept;

  //! Return the command buffer
  const VkCommandBuffer& commandBuffer() const noexcept;

  //! Check if the kernel has global arg
  static constexpr bool hasGlobalArg() noexcept;

  //! Check if the kernel has local arg
  static constexpr bool hasLocalArg() noexcept;

  //! Check if the kernel has pod arg
  static constexpr bool hasPodArg() noexcept;

  //! Execute a kernel
  LaunchResult run(Args... args, LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Record dispatching the kernel to the parent device
  void dispatchCmd(const std::array<uint32b, 3>& work_size);

  //! Initialize the kernel
  void initData(const Params& params) override;

  //! Update the debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  //! Make a tuple of POD parameters
  template <std::size_t kIndex = 0>
  static auto makePodTupleType() noexcept;


  using PodTuple = decltype(makePodTupleType());


  //! Get the underlying VkBuffer from the given buffer
  template <typename Type>
  static const VkBuffer& getBufferHandle(const Buffer<Type>& buffer) noexcept;

  //! Initialize the buffer list
  template <std::size_t kIndex, typename Type, typename ...Types>
  static void initBufferList(VkBuffer* buffer_list, Type&& value, Types&&... rest) noexcept;

  //! Initialize the POD buffer
  void initPodBuffer() noexcept;

  //! Initialize the given POD tuple
  template <std::size_t kIndex, typename Type, typename ...Types>
  static void initPodTuple(PodTuple* pod_params, Type&& value, Types&&... rest) noexcept;

  //! Initialize the given POD tuple
  static PodTuple makePodTuple(Args... args) noexcept;

  //! Return the number of buffers which is needed for the kernel (including pod)
  static constexpr std::size_t numOfBuffers() noexcept;

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;

  //! Update the underlying descriptor set with the given arguments
  void updateDescriptorSet(Args... args);

//  //! Bind buffers
//  void bindBuffers(std::add_lvalue_reference_t<BufferArgs>... args) noexcept;
//
//  //! Dispatch
//  void dispatch(const std::array<uint32b, kDim> works) noexcept;
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
  SharedBuffer<PodTuple> pod_buffer_;
  SharedBuffer<PodTuple> pod_cache_;
};

} // namespace zivc

#include "vulkan_kernel-inl.hpp"

#endif // ZIVC_VULKAN_KERNEL_HPP
