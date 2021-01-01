/*!
  \file vulkan_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_KERNEL_INL_HPP
#define ZIVC_VULKAN_KERNEL_INL_HPP

#include "vulkan_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_buffer.hpp"
#include "vulkan_buffer_impl.hpp"
#include "vulkan_device.hpp"
#include "vulkan_kernel_impl.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/kernel_params.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
VulkanKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
~VulkanKernel() noexcept
{
  BaseKernel::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VkCommandBuffer&
VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
commandBuffer() noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const VkCommandBuffer& VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
commandBuffer() const noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
LaunchResult VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
run(Args... args, LaunchOptions& launch_options)
{
  VulkanDevice& device = parentImpl();
  // DescriptorSet
  updateDescriptorSet(args...);
  // POD
  const bool need_to_update_pod = checkIfPodUpdateIsNeeded(args...);
  const auto work_size = BaseKernel::expandWorkSize(launch_options.workSize());
  VkCommandBuffer command = commandBuffer();
  // Command recording
  {
    const auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Update global and region offsets
      updateGlobalAndRegionOffsets(launch_options);
      if (need_to_update_pod)
        updatePodBuffer();
      dispatchCmd(work_size);
    }
  }
  LaunchResult result{};
  // Command submission
  {
    const VkQueue q = device.getQueue(launch_options.queueIndex());
    if (launch_options.isExternalSyncMode())
      result.fence().setDevice(std::addressof(device));
    auto debug_region = device.makeQueueDebugLabel(q, launch_options);
    device.submit(command, q, result.fence());
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
hasGlobalArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfGlobalArgs;
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
hasLocalArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfLocalArgs;
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
hasPodArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfPodArgs;
  return result;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
destroyData() noexcept
{
  command_buffer_ = VK_NULL_HANDLE;
  if (pipeline_ != VK_NULL_HANDLE) {
    pod_cache_.reset();
    pod_buffer_.reset();
    VulkanKernelImpl impl{std::addressof(parentImpl())};
    impl.destroyPipeline(std::addressof(pipeline_layout_),
                         std::addressof(pipeline_));
    impl.destroyDescriptorSet(std::addressof(desc_set_layout_),
                              std::addressof(desc_pool_));
  }
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
dispatchCmd(const std::array<uint32b, 3>& work_size)
{
  VulkanKernelImpl impl{std::addressof(parentImpl())};
  impl.dispatchCmd(command_buffer_, desc_set_, pipeline_layout_, pipeline_, kDim, work_size);
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initData(const Params& params)
{
  static_assert(hasGlobalArg(), "The kernel doesn't have global argument.");
  VulkanDevice& device = parentImpl();
  device.addShaderModule(KSet{});
  VulkanKernelImpl impl{std::addressof(device)};
  impl.initDescriptorSet(BaseKernel::ArgParser::kNumOfBufferArgs,
                         zisc::cast<std::size_t>(hasPodArg() ? 1 : 0),
                         std::addressof(desc_set_layout_),
                         std::addressof(desc_pool_),
                         std::addressof(desc_set_));
  const auto& module_data = device.getShaderModule(KSet::id());
  impl.initPipeline(BaseKernel::dimension(),
                    BaseKernel::ArgParser::kNumOfLocalArgs,
                    desc_set_layout_,
                    module_data.module_,
                    params.kernelName(),
                    std::addressof(pipeline_layout_),
                    std::addressof(pipeline_));
  command_buffer_ = device.makeCommandBuffer();
  initPodBuffer();
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateDebugInfoImpl() noexcept
{
  VulkanDevice& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  const std::string_view kernel_name = id_data.name();

  auto set_debug_info = [this, &device, &id_data, &kernel_name]
  (const VkObjectType type, const auto& obj, const std::string_view suffix) noexcept
  {
    if (obj != VK_NULL_HANDLE) {
      IdData::NameType obj_name{""};
      std::strncpy(obj_name.data(), kernel_name.data(), kernel_name.size() + 1);
      std::strncat(obj_name.data(), suffix.data(), suffix.size());
      device.setDebugInfo(type, obj, obj_name.data(), this);
    }
  };

  set_debug_info(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT,
                 desc_set_layout_,
                 "_descsetlayout");
  set_debug_info(VK_OBJECT_TYPE_DESCRIPTOR_POOL,
                 desc_pool_,
                 "_descpool");
  set_debug_info(VK_OBJECT_TYPE_DESCRIPTOR_SET,
                 desc_set_,
                 "_descset");
  set_debug_info(VK_OBJECT_TYPE_PIPELINE_LAYOUT,
                 pipeline_layout_,
                 "_pipelinelayout");
  set_debug_info(VK_OBJECT_TYPE_PIPELINE,
                 pipeline_,
                 "_pipeline");
  set_debug_info(VK_OBJECT_TYPE_COMMAND_BUFFER,
                 command_buffer_,
                 "_commandbuffer");
  if (pod_buffer_) {
    IdData::NameType obj_name{""};
    const std::string_view suffix{"_podbuffer"};
    std::strncpy(obj_name.data(), kernel_name.data(), kernel_name.size() + 1);
    std::strncat(obj_name.data(), suffix.data(), suffix.size());
    pod_buffer_->setName(obj_name.data());
  }
  if (pod_cache_) {
    IdData::NameType obj_name{""};
    const std::string_view suffix{"_podcache"};
    std::strncpy(obj_name.data(), kernel_name.data(), kernel_name.size() + 1);
    std::strncat(obj_name.data(), suffix.data(), suffix.size());
    pod_cache_->setName(obj_name.data());
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex>
inline
auto VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
makePodTupleType() noexcept
{
  constexpr auto pod_arg_info = BaseKernel::ArgParser::getPodArgInfoList();
  if constexpr (kIndex < pod_arg_info.size()) {
    constexpr std::size_t pod_index = pod_arg_info[kIndex].index();
    using ArgTuple = std::tuple<FuncArgs...>;
    using PodType = std::tuple_element_t<pod_index, ArgTuple>;
    std::tuple<PodType> left{};
    auto right = makePodTupleType<kIndex + 1>();
    auto pod_params = std::tuple_cat(left, right);
    return pod_params;
  }
  else {
    std::tuple<> pod_params{};
    return pod_params;
  }
}

/*!
  \details No detailed description

  \param [in] args No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
bool VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
checkIfPodUpdateIsNeeded(Args... args) const noexcept
{
  bool need_to_update_pod = false;
  if constexpr (hasPodArg()) {
    const PodTuple pod = makePodTuple(args...);
    ZISC_ASSERT(pod_cache_->isHostVisible(), "The cache isn't host visible.");
    auto cache = pod_cache_->mapMemory();
    need_to_update_pod = cache[0] != pod;
    if (need_to_update_pod)
      cache[0] = pod;
  }
  return need_to_update_pod;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] buffer No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <typename Type>
inline
const VkBuffer& VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
getBufferHandle(const Buffer<Type>& buffer) noexcept
{
  using BufferT = VulkanBuffer<Type>;
  auto& handle = zisc::cast<const BufferT*>(std::addressof(buffer))->buffer();
  return handle;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [out] buffer_list No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type, typename ...Types>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initBufferList(VkBuffer* buffer_list, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cvref_t<Type>;
  using ASpaceInfo = AddressSpaceInfo<T>;
  if constexpr (!ASpaceInfo::kIsPod)
    buffer_list[kIndex] = getBufferHandle(value);
  if constexpr (0 < sizeof...(Types)) {
    constexpr std::size_t next_index = !ASpaceInfo::kIsPod ? kIndex + 1 : kIndex;
    initBufferList<next_index>(buffer_list, std::forward<Types>(rest)...);
  }
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initPodBuffer() noexcept
{
  if constexpr (hasPodArg()) {
    auto& device = parentImpl();
    using BuffType = VulkanBuffer<PodTuple>;
    constexpr auto desc_uniform = BuffType::DescriptorType::kUniform;
    {
      pod_buffer_ = device.template makeBuffer<PodTuple>(BufferUsage::kDeviceOnly);
      zisc::cast<BuffType*>(pod_buffer_.get())->setDescriptorType(desc_uniform);
      pod_buffer_->setSize(1);
    }
    {
      pod_cache_ = device.template makeBuffer<PodTuple>(BufferUsage::kHostOnly);
      zisc::cast<BuffType*>(pod_cache_.get())->setDescriptorType(desc_uniform);
      pod_cache_->setSize(1);
    }
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [out] pod_params No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type, typename ...Types>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initPodTuple(PodTuple* pod_params, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cvref_t<Type>;
  using ASpaceInfo = AddressSpaceInfo<T>;
  if constexpr (ASpaceInfo::kIsPod)
    std::get<kIndex>(*pod_params) = std::forward<Type>(value);
  if constexpr (0 < sizeof...(Types)) {
    constexpr std::size_t next_index = ASpaceInfo::kIsPod ? kIndex + 1 : kIndex;
    initPodTuple<next_index>(pod_params, std::forward<Types>(rest)...);
  }
}

/*!
  \details No detailed description

  \param [in] args No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
makePodTuple(Args... args) noexcept -> PodTuple
{
  PodTuple pod_params{};
  if constexpr (0 < sizeof...(Args))
    initPodTuple<0>(std::addressof(pod_params), args...);
  return pod_params;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
numOfBuffers() noexcept
{
  std::size_t n = BaseKernel::ArgParser::kNumOfGlobalArgs;
  n += hasPodArg() ? 1 : 0;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanDevice& VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::reinterp<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const VulkanDevice& VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() const noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::reinterp<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \param [in] args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateDescriptorSet(Args... args)
{
  constexpr std::size_t n = numOfBuffers();
  std::array<VkBuffer, n> buffer_list{};
  std::array<VkDescriptorType, n> desc_type_list{};
  initBufferList<0>(buffer_list.data(), args...);
  desc_type_list.fill(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
  if constexpr (hasPodArg()) {
    buffer_list[n - 1] = getBufferHandle(*pod_buffer_);
    desc_type_list[n - 1] = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  }
  VulkanKernelImpl impl{std::addressof(parentImpl())};
  impl.updateDescriptorSet(desc_set_, buffer_list, desc_type_list);
}

/*!
  \details No detailed description

  \param [in] options No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateGlobalAndRegionOffsets(const LaunchOptions& launch_options)
{
  std::array<uint32b, 7> data;
  data.fill(0);
  VulkanKernelImpl impl{std::addressof(parentImpl())};
  impl.pushConstantCmd(commandBuffer(), pipeline_layout_, 0, data);
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updatePodBuffer()
{
  using BufferT = VulkanBuffer<PodTuple>;
  const VkBufferCopy copy_region{0, 0, sizeof(PodTuple)};
  auto pod_cache = zisc::cast<const BufferT*>(pod_cache_.get())->buffer();
  auto pod_buff = zisc::cast<const BufferT*>(pod_buffer_.get())->buffer();
  {
    VulkanBufferImpl impl{std::addressof(parentImpl())};
    impl.copyCmd(commandBuffer(), pod_cache, pod_buff, copy_region);
  }
  {
    VulkanKernelImpl impl{std::addressof(parentImpl())};
    impl.addPodBarrierCmd(commandBuffer(), pod_buff);
  }
}

} // namespace zivc

#endif // ZIVC_VULKAN_KERNEL_INL_HPP
