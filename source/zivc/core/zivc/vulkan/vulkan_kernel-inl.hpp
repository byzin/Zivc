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
#include <cstdio>
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
#include "utility/queue_debug_label_region.hpp"
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/device_info.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/error.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/kernel_arg_cache.hpp"
#include "zivc/utility/kernel_init_params.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
VulkanKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
const VkCommandBuffer& VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
LaunchResult VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
run(Args... args, const LaunchOptions& launch_options)
{
  VulkanDevice& device = parentImpl();

  updateDescriptorSet(args...);
  // Prepare command buffer
  prepareCommandBuffer();
  VkCommandBuffer command = commandBuffer();
  // Command recording
  {
    const bool have_new_pod = updatePodCacheIfNeeded(args...);
    const auto work_size = calcDispatchWorkSize(launch_options.workSize());

    const auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Update global and region offsets
      updateModuleScopePushConstantsCmd(work_size, launch_options);
      // Update POD buffer
      if (have_new_pod)
        updatePodBufferCmd();
      // Dispatch the kernel
      dispatchCmd(work_size);
    }
  }
  LaunchResult result{};
  // Command submission
  {
    const VkQueue q = device.getQueue(launch_options.queueIndex());
    result.fence().setDevice(launch_options.isExternalSyncMode() ? &device : nullptr);
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setCommandBufferRef(const VkCommandBuffer* command_ref) noexcept
{
  command_buffer_ref_ = command_ref;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr bool VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
constexpr bool VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
constexpr bool VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
dispatchCmd(const std::array<uint32b, 3>& work_size)
{
  VulkanKernelImpl impl{std::addressof(parentImpl())};
  VkCommandBuffer command = commandBuffer();
  impl.dispatchCmd(command,
                   kernel_data_,
                   desc_set_,
                   work_size);
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initData(const Params& params)
{
  static_assert(hasGlobalArg(), "The kernel doesn't have global argument.");
  validateData();
  VulkanDevice& device = parentImpl();

  // Command buffer reference
  const VkCommandBuffer* command_buffer_ref =
      zisc::cast<const VkCommandBuffer*>(params.vulkanCommandBufferPtr());
  setCommandBufferRef(command_buffer_ref);
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
  \tparam PodTypes No description.
  \param [in] cache No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename ...PodTypes> inline
auto VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
makePodCacheType(const KernelArgCache<PodTypes...>& cache) noexcept
{
  using ArgParser = typename BaseKernel::ArgParser;
  if constexpr (kIndex < ArgParser::kNumOfPodArgs) {
    constexpr auto pod_arg_info = ArgParser::getPodArgInfoList();
    constexpr std::size_t pod_index = pod_arg_info[kIndex].index();
    using ArgTuple = std::tuple<FuncArgs...>;
    using PodType = std::tuple_element_t<pod_index, ArgTuple>;
    auto precedence = concatArgCache<PodType>(cache);
    return makePodCacheType<kIndex + 1>(precedence);
  }
  else {
    return cache;
  }
}


/*!
  \details No detailed description

  \param [in] work_size No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::array<uint32b, 3> VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
calcDispatchWorkSize(const std::array<uint32b, kDim>& work_size) const noexcept
{
  const VulkanDevice& device = parentImpl();
  const auto& group_size = device.workGroupSizeDim(BaseKernel::dimension());
  std::array<uint32b, 3> dispatch_size{{1, 1, 1}};
  for (std::size_t i = 0; i < BaseKernel::dimension(); ++i) {
    const uint32b s = (work_size[i] + group_size[i] - 1) / group_size[i];
    dispatch_size[i] = s;
  }
  return dispatch_size;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] buffer No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <KernelArg Type>
inline
const VkBuffer& VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
getBufferHandle(const Buffer<Type>& buffer) noexcept
{
  ZISC_ASSERT(buffer.type() == SubPlatformType::kVulkan, "The buffer isn't vulkan.");
  using BufferT = VulkanBuffer<Type>;
  using BufferData = typename BufferT::BufferData;
  auto data = zisc::cast<const BufferData*>(buffer.rawBufferData());
  return data->buffer_;
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initBufferList(VkBuffer* buffer_list, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cvref_t<Type>;
  using ArgTypeInfo = KernelArgTypeInfo<T>;
  if constexpr (!ArgTypeInfo::kIsPod)
    buffer_list[kIndex] = getBufferHandle(value);
  if constexpr (0 < sizeof...(Types)) {
    constexpr std::size_t next_index = !ArgTypeInfo::kIsPod ? kIndex + 1 : kIndex;
    initBufferList<next_index>(buffer_list, std::forward<Types>(rest)...);
  }
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initPodBuffer()
{
  if constexpr (hasPodArg()) {
    auto& device = parentImpl();
    using BuffType = VulkanBuffer<PodCacheT>;
    constexpr auto desc_uniform = BuffType::DescriptorType::kUniform;
    {
      pod_buffer_ = device.template makeBuffer<PodCacheT>(BufferUsage::kDeviceOnly);
      zisc::cast<BuffType*>(pod_buffer_.get())->setDescriptorType(desc_uniform);
      pod_buffer_->setSize(1);
    }
    {
      pod_cache_ = device.template makeBuffer<PodCacheT>(BufferUsage::kHostOnly);
      zisc::cast<BuffType*>(pod_cache_.get())->setDescriptorType(desc_uniform);
      pod_cache_->setSize(1);
    }

#if defined(ZIVC_PRINT_CACHE_TREE)
    // Print POD cache tree
    KernelArgCache<void>::printValue<PodCacheT>(0,
                                                "ArgCache",
                                                std::addressof(std::cout));
    std::cout << std::endl;
    PodCacheT::printTree(1, std::addressof(std::cout));
#endif // ZIVC_PRINT_CACHE_TREE
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [out] cache No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type, typename ...Types> inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initPodCache(PodCacheT* cache, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cvref_t<Type>;
  using ArgTypeInfo = KernelArgTypeInfo<T>;
  if constexpr (ArgTypeInfo::kIsPod)
    cache->template set<kIndex>(std::forward<Type>(value));
  constexpr bool has_rest = 0 < sizeof...(Types);
  if constexpr (has_rest) {
    constexpr std::size_t next_index = ArgTypeInfo::kIsPod ? kIndex + 1 : kIndex;
    initPodCache<next_index>(cache, std::forward<Types>(rest)...);
  }
}

/*!
  \details No detailed description

  \param [in] args No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
makePodCache(Args... args) noexcept -> PodCacheT
{
  PodCacheT cache{};
  if constexpr (hasPodArg())
    initPodCache<0>(std::addressof(cache), args...);
  return cache;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
numOfAllBuffers() noexcept
{
  std::size_t n = BaseKernel::ArgParser::kNumOfBufferArgs;
  n += hasPodArg() ? 1 : 0;
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
VulkanDevice& VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::cast<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const VulkanDevice& VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() const noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::cast<const VulkanDevice*>(p);
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateDescriptorSet(Args... args)
{
  constexpr std::size_t n = numOfAllBuffers();
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
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateModuleScopePushConstantsCmd(const std::array<uint32b, 3>& work_size,
                                  const LaunchOptions& launch_options)
{
  std::array<uint32b, 23> constans;
  constans.fill(0);

  const VulkanDevice& device = parentImpl();
  const auto& group_size = device.workGroupSizeDim(BaseKernel::dimension());
  // Global offset
  {
    const auto global_offset =
        BaseKernel::expandWorkSize(launch_options.globalIdOffset(), 0);
    constexpr std::size_t offset = 0;
    for (std::size_t i = 0; i < global_offset.size(); ++i)
      constans[offset + i] = global_offset[i];
  }
  // Enqueued local size
  {
    constexpr std::size_t offset = 4;
    for (std::size_t i = 0; i < group_size.size(); ++i)
      constans[offset + i] = group_size[i];
  }
  // Global size
  {
    constexpr std::size_t offset = 8;
    for (std::size_t i = 0; i < work_size.size(); ++i)
      constans[offset + i] = work_size[i] * group_size[i];
  }
  // Num of workgroups
  {
    constexpr std::size_t offset = 16;
    for (std::size_t i = 0; i < work_size.size(); ++i)
      constans[offset + i] = work_size[i];
  }

  VulkanKernelImpl impl{std::addressof(parentImpl())};
  impl.pushConstantCmd(commandBuffer(), kernel_data_, 0, constans);
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updatePodBufferCmd()
{
  auto pod_cache = getBufferHandle(*pod_cache_);
  auto pod_buff = getBufferHandle(*pod_buffer_);
  {
    const VkBufferCopy copy_region{0, 0, sizeof(PodCacheT)};
    VulkanBufferImpl impl{std::addressof(parentImpl())};
    impl.copyCmd(commandBuffer(), pod_cache, pod_buff, copy_region);
  }
  {
    VulkanKernelImpl impl{std::addressof(parentImpl())};
    impl.addPodBarrierCmd(commandBuffer(), pod_buff);
  }
}

/*!
  \details No detailed description

  \param [in] args No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
bool VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updatePodCacheIfNeeded(Args... args) const noexcept
{
  bool have_new_pod = false;
  if constexpr (hasPodArg()) {
    const PodCacheT data = makePodCache(args...);
    ZISC_ASSERT(pod_cache_->isHostVisible(), "The cache isn't host visible.");
    auto cache = pod_cache_->mapMemory();
    have_new_pod = cache[0] != data;
    if (have_new_pod)
      cache[0] = data;
  }
  return have_new_pod;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void VulkanKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
validateData()
{
  const VulkanDevice& device = parentImpl();
  const DeviceInfo& info = device.deviceInfo();
  if (info.maxNumOfBuffersPerKernel() < BaseKernel::numOfBuffers()) {
    char message[256] = "";
    std::sprintf(message,
        "The number of buffer arguments in the kernel exceeded the limit. limit=%d, buffers=%d",
        zisc::cast<int>(info.maxNumOfBuffersPerKernel()),
        zisc::cast<int>(BaseKernel::numOfBuffers()));
    throw SystemError{ErrorCode::kNumOfParametersLimitExceeded, message};
  }
}

} // namespace zivc

#endif // ZIVC_VULKAN_KERNEL_INL_HPP
