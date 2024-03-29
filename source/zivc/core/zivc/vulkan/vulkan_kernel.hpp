/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_HPP
#define ZIVC_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/kernel_arg_cache.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class VulkanKernel;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanKernelHelper
{
 public:
  //!
  template <typename VKernel, typename Type, typename ...Types>
  static LaunchResult run(VKernel* kernel,
                          const Type& launch_options,
                          Types&& ...args);

  //!
  template <typename VKernel, typename Type>
  static void updateModuleScopePushConstantsCmd(VKernel* kernel,
                                                const std::array<uint32b, 3>& work_size,
                                                const Type& launch_options);
};

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam FuncArgs No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
class VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...> :
    public Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>;
  using Params = typename BaseKernel::Params;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  VulkanKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~VulkanKernel() noexcept override;


  //! Return the command buffer
  const VkCommandBuffer& commandBuffer() const noexcept;

  //! Check if the kernel has global arg
  static constexpr bool hasGlobalArg() noexcept;

  //! Check if the kernel has local arg
  static constexpr bool hasLocalArg() noexcept;

  //! Check if the kernel has pod arg
  static constexpr bool hasPodArg() noexcept;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult run(Args... args, const LaunchOptions& launch_options) override
  {
    //! \note Separate declaration and definition cause a build error on visual studio
    return VulkanKernelHelper::run(this, launch_options, std::forward<Args>(args)...);
  }

  //! Set a command buffer reference
  void setCommandBufferRef(const VkCommandBuffer* command_ref) noexcept;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Record dispatching the kernel to the parent device
  void dispatchCmd(const std::array<uint32b, 3>& work_size);

  //! Initialize the kernel
  void initData(const Params& params) override;

  //! Update the debug info
  void updateDebugInfoImpl() override;

 private:
  friend VulkanKernelHelper;


  //! Make a struct of POD cache
  template <std::size_t kIndex, typename ...PodTypes>
  static auto makePodCacheType(const KernelArgCache<PodTypes...>& cache) noexcept;


  using PodCacheT = decltype(makePodCacheType<0>(KernelArgCache<void>{}));
  static_assert(std::is_trivially_copyable_v<PodCacheT>,
                "The POD values aren't trivially copyable.");
  static_assert(zisc::EqualityComparable<PodCacheT>,
                "The POD values aren't equality comparable.");


  //! Calculate the dispatch work size
  std::array<uint32b, 3> calcDispatchWorkSize(const std::array<uint32b, kDim>& work_size) const noexcept;

  //! Get the underlying VkBuffer from the given buffer
  template <KernelArg Type>
  static const VkBuffer& getBufferHandle(const Buffer<Type>& buffer) noexcept;

  //! Initialize the buffer list
  template <std::size_t kIndex, typename Type, typename ...Types>
  static void initBufferList(VkBuffer* buffer_list,
                             Type&& value,
                             Types&&... rest) noexcept;

  //! Initialize the POD buffer
  void initPodBuffer();

  //! Initialize a POD cache from the given arguments
  template <std::size_t kIndex, typename Type, typename ...Types>
  static void initPodCache(PodCacheT* cache, Type&& value, Types&&... rest) noexcept;

  //! Make a POD data from the given POD parameters
  static PodCacheT makePodCache(Args... args) noexcept;

  //! Return the number of buffers which is needed for the kernel (including pod)
  static constexpr std::size_t numOfAllBuffers() noexcept;

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;

  //! Prepare command buffer
  void prepareCommandBuffer();

  //! Update debug info of the underlying command buffer
  void updateCommandBufferDebugInfo();

  //! Update the underlying descriptor set with the given arguments
  void updateDescriptorSet(Args... args);

  //! Update module scope push constans
  void updateModuleScopePushConstantsCmd(const std::array<uint32b, 3>& work_size,
                                         const LaunchOptions& launch_options)
  {
    //! \note Separate declaration and definition cause a build error on visual studio
    VulkanKernelHelper::updateModuleScopePushConstantsCmd(this,
                                                          work_size,
                                                          launch_options);
  }

  //! Update pod cache with the given args if needed
  bool updatePodCacheIfNeeded(Args... args) const noexcept;

  //! Update POD buffer
  void updatePodBufferCmd();

  //! Validate kernel data
  void validateData();


  const void* kernel_data_ = nullptr;
  VkDescriptorPool desc_pool_ = ZIVC_VK_NULL_HANDLE;
  VkDescriptorSet desc_set_ = ZIVC_VK_NULL_HANDLE;
  const VkCommandBuffer* command_buffer_ref_ = nullptr;
  VkCommandBuffer command_buffer_ = ZIVC_VK_NULL_HANDLE;
  SharedBuffer<PodCacheT> pod_buffer_;
  SharedBuffer<PodCacheT> pod_cache_;
};

} // namespace zivc

#include "vulkan_kernel-inl.hpp"

#endif // ZIVC_VULKAN_KERNEL_HPP
