/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_HPP
#define ZIVC_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <concepts>
#include <cstddef>
#include <span>
#include <type_traits>
#include <utility>
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/id_data.hpp"
#include "zivc/auxiliary/launch_result.hpp"
#include "zivc/internal/kernel_arg_cache.hpp"

namespace zivc {

// Forward declaration
class VulkanDevice;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
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
class VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...> :
    public Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>
{
 public:
  // Type aliases
  using BaseKernelT = Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>;
  using ParamsT = typename BaseKernelT::ParamsT;
  using LaunchOptionsT = typename BaseKernelT::LaunchOptionsT;


  //! Initialize the kernel
  VulkanKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~VulkanKernel() noexcept override;


  //! Return the command buffer
  const VkCommandBuffer& commandBuffer() const noexcept;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult run(Args... args, const LaunchOptionsT& launch_options) override;

  //! Set a command buffer reference
  void setCommandBufferRef(const VkCommandBuffer* command_ref) noexcept;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Record dispatching the kernel to the parent device
  void dispatchCmd(const std::span<const uint32b, 3> work_size);

  //! Initialize the kernel
  void initData(const ParamsT& params) override;

  //! Update the debug info
  void updateDebugInfoImpl() override;

 private:
  //!
  struct CacheTypeCreator
  {
    //! Create a struct of POD cache
    static auto createPod() noexcept;

    //! Create a struct of POD cache
    template <std::size_t... kIndices>
    static auto createPodImpl(std::index_sequence<kIndices...> indices) noexcept;

    //! Return the type value of the pod variable by the position
    template <std::size_t kIndex>
    static auto getPod() noexcept;

    // Type aliases
    using PodCacheT = decltype(createPod());
  };


  // Type aliases
  using PodCacheT = typename CacheTypeCreator::PodCacheT;
  template <std::size_t kIndex>
  using ArgT = typename BaseKernelT::template ArgT<kIndex>;


  //! Calculate the dispatch work size
  std::array<uint32b, 3> calcDispatchWorkSize(const std::span<const uint32b, kDim> work_size) const noexcept;

  //! Get the underlying VkBuffer from the given buffer
  template <KernelArg Type>
  static const VkBuffer& getBufferHandle(const Buffer<Type>& buffer) noexcept;

  //! Initialize the POD buffer
  void initPodBuffer();

  //! Return the number of descriptors which is needed for the kernel (including pod)
  static constexpr std::size_t numOfDescriptors() noexcept;

  //! Return the device
  VulkanDevice& parentImpl() noexcept;

  //! Return the device
  const VulkanDevice& parentImpl() const noexcept;

  //! Prepare command buffer
  void prepareCommandBuffer();

  //! Set
  template <std::size_t kIndex, typename Type>
  void setArgCache(const Buffer<Type>& value,
                   VkBuffer* buffer_list,
                   PodCacheT* pod_cache) noexcept;

  //! Set
  template <std::size_t kIndex, typename Type>
  void setArgCache(const Type value,
                   VkBuffer* buffer_list,
                   PodCacheT* pod_cache) noexcept;

  //! Update the underlying descriptor set and pod cache
  bool updateArgCache(Args... args);

  //! Update the underlying descriptor set and pod cache
  template <std::size_t ...kIndices>
  void updateArgCacheImpl(Args... args,
                          VkBuffer* buffer_list,
                          PodCacheT* pod_cache,
                          std::index_sequence<kIndices...> indices) noexcept;

  //! Update debug info of the underlying command buffer
  void updateCommandBufferDebugInfo();

  //! Update module scope push constans
  void updateModuleScopePushConstantsCmd(const std::span<const uint32b, 3>& work_size,
                                         const LaunchOptionsT& launch_options);

  //! Update POD buffer
  void updatePodBufferCmd();

  //! Update pod cache buffer with the given cache if needed
  bool updatePodCacheIfNeeded(const PodCacheT& new_cache) const noexcept;

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
