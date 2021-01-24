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
#include "zisc/concepts.hpp"
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
#include "utility/pod_data.hpp"
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
  VkCommandBuffer& command = command_buffer_ref_
      ? *command_buffer_ref_
      : command_buffer_;
  return command;
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
  const VkCommandBuffer& command = command_buffer_ref_
      ? *command_buffer_ref_
      : command_buffer_;
  return command;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
LaunchResult VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
run(Args... args, const LaunchOptions& launch_options)
{
  VulkanDevice& device = parentImpl();
  // DescriptorSet
  updateDescriptorSet(args...);
  // POD buffer
  const bool need_to_update_pod = checkIfPodUpdateIsNeeded(args...);
  const auto work_size = BaseKernel::expandWorkSize(launch_options.workSize());
  // Prepare command buffer
  prepareCommandBuffer();
  VkCommandBuffer command = commandBuffer();
  // Command recording
  {
    const auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Update global and region offsets
      updateGlobalAndRegionOffsetsCmd(launch_options);
      // Update POD buffer
      if (need_to_update_pod)
        updatePodBufferCmd();
      // Dispatch the kernel
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
  \param [in] command_ref No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setCommandBufferRef(VkCommandBuffer* command_ref) noexcept
{
  command_buffer_ref_ = command_ref;
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
  command_buffer_ = ZIVC_VK_NULL_HANDLE;
  pod_cache_.reset();
  pod_buffer_.reset();
  if (desc_pool_ != ZIVC_VK_NULL_HANDLE) {
    VulkanKernelImpl impl{std::addressof(parentImpl())};
    impl.destroyDescriptorSet(std::addressof(desc_pool_));
  }
  kernel_data_ = nullptr;
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
  VkCommandBuffer command = commandBuffer();
  impl.dispatchCmd(command,
                   kernel_data_,
                   desc_set_,
                   BaseKernel::dimension(),
                   work_size);
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

  // Add a shader module
  const auto& module_data = device.addShaderModule(KSet{});
  // Add a kernel data
  const std::size_t num_of_storage_buffers = BaseKernel::ArgParser::kNumOfBufferArgs;
  const std::size_t num_of_uniform_buffers = hasPodArg() ? 1 : 0;
  const std::size_t num_of_local_args = BaseKernel::ArgParser::kNumOfLocalArgs;
  const auto& kernel_data = device.addShaderKernel(module_data,
                                                   params.kernelName(),
                                                   BaseKernel::dimension(),
                                                   num_of_storage_buffers,
                                                   num_of_uniform_buffers,
                                                   num_of_local_args);
  kernel_data_ = std::addressof(kernel_data);
  VulkanKernelImpl impl{std::addressof(device)};
  impl.initDescriptorSet(num_of_storage_buffers,
                         num_of_uniform_buffers,
                         kernel_data_,
                         std::addressof(desc_pool_),
                         std::addressof(desc_set_));
  initPodBuffer();
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateDebugInfoImpl()
{
  VulkanDevice& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  const std::string_view kernel_name = id_data.name();

  auto set_debug_info = [this, &device, &kernel_name]
  (const VkObjectType type, const auto& obj, const std::string_view suffix)
  {
    if (obj != ZIVC_VK_NULL_HANDLE) {
      IdData::NameType obj_name{""};
      std::strncpy(obj_name.data(), kernel_name.data(), kernel_name.size() + 1);
      std::strncat(obj_name.data(), suffix.data(), suffix.size());
      device.setDebugInfo(type, obj, obj_name.data(), this);
    }
  };

  set_debug_info(VK_OBJECT_TYPE_DESCRIPTOR_POOL,
                 desc_pool_,
                 "_descpool");
  set_debug_info(VK_OBJECT_TYPE_DESCRIPTOR_SET,
                 desc_set_,
                 "_descset");
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
  updateCommandBufferDebugInfo();
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
makePodDataType() noexcept
{
  constexpr auto pod_arg_info = BaseKernel::ArgParser::getPodArgInfoList();
  if constexpr (kIndex < pod_arg_info.size()) {
    constexpr std::size_t pod_index = pod_arg_info[kIndex].index();
    using ArgTuple = std::tuple<FuncArgs...>;
    using PodType = std::tuple_element_t<pod_index, ArgTuple>;
    auto prev_data = makePodDataType<kIndex + 1>();
    auto data = concatPodData<PodType>(prev_data);
    return data;
  }
  else {
    PodData<void> data{};
    return data;
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
    const PodDataT data = makePodData(args...);
    ZISC_ASSERT(pod_cache_->isHostVisible(), "The cache isn't host visible.");
    auto cache = pod_cache_->makeMappedMemory();
    need_to_update_pod = cache[0] == data;
    if (need_to_update_pod)
      cache[0] = data;
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
template <zisc::TriviallyCopyable Type>
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
initPodBuffer()
{
  if constexpr (hasPodArg()) {
    auto& device = parentImpl();
    using BuffType = VulkanBuffer<PodDataT>;
    constexpr auto desc_uniform = BuffType::DescriptorType::kUniform;
    {
      pod_buffer_ = device.template makeBuffer<PodDataT>(BufferUsage::kDeviceOnly);
      zisc::cast<BuffType*>(pod_buffer_.get())->setDescriptorType(desc_uniform);
      pod_buffer_->setSize(1);
    }
    {
      pod_cache_ = device.template makeBuffer<PodDataT>(BufferUsage::kHostOnly);
      zisc::cast<BuffType*>(pod_cache_.get())->setDescriptorType(desc_uniform);
      pod_cache_->setSize(1);
    }
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
makePodData(Args... args) noexcept -> PodDataT
{
  PodDataT data{};
  if constexpr (0 < sizeof...(Args))
    makePodDataImpl<0>(std::addressof(data), args...);
  return data;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [out] data No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type, typename ...Types>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
makePodDataImpl(PodDataT* data, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cvref_t<Type>;
  using ASpaceInfo = AddressSpaceInfo<T>;
  if constexpr (ASpaceInfo::kIsPod)
    data->template set<kIndex>(value);
  if constexpr (0 < sizeof...(Types)) {
    constexpr std::size_t next_index = ASpaceInfo::kIsPod ? kIndex + 1 : kIndex;
    makePodDataImpl<next_index>(data, std::forward<Types>(rest)...);
  }
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
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
prepareCommandBuffer()
{
  VkCommandBuffer command = commandBuffer();
  if (command == ZIVC_VK_NULL_HANDLE) {
    VulkanDevice& device = parentImpl();
    command_buffer_ = device.makeCommandBuffer();
    updateCommandBufferDebugInfo();
  }
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateCommandBufferDebugInfo()
{
  VulkanDevice& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  const std::string_view kernel_name = id_data.name();

  auto set_debug_info = [this, &device, &kernel_name]
  (const VkObjectType type, const auto& obj, const std::string_view suffix)
  {
    if (obj != ZIVC_VK_NULL_HANDLE) {
      IdData::NameType obj_name{""};
      std::strncpy(obj_name.data(), kernel_name.data(), kernel_name.size() + 1);
      std::strncat(obj_name.data(), suffix.data(), suffix.size());
      device.setDebugInfo(type, obj, obj_name.data(), this);
    }
  };

  set_debug_info(VK_OBJECT_TYPE_COMMAND_BUFFER,
                 command_buffer_,
                 "_commandbuffer");
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

  \param [in] launch_options No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updateGlobalAndRegionOffsetsCmd(const LaunchOptions& launch_options)
{
  static_cast<void>(launch_options);
  std::array<uint32b, 7> data;
  data.fill(0);
  VulkanKernelImpl impl{std::addressof(parentImpl())};
  impl.pushConstantCmd(commandBuffer(), kernel_data_, 0, data);
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
updatePodBufferCmd()
{
  using BufferT = VulkanBuffer<PodDataT>;
  const VkBufferCopy copy_region{0, 0, sizeof(PodDataT)};
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
