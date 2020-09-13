/*!
  \file vulkan_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
// Vulkan
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "zivc/buffer.hpp"
#include "zivc/kernel.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/kernel_parameters.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
VulkanKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
~VulkanKernel() noexcept
{
  BaseKernel::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
VkCommandBuffer&
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
commandBuffer() noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
const VkCommandBuffer&
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
commandBuffer() const noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
LaunchResult
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
run(ArgTypes... args, const LaunchOptions& launch_options)
{
  VulkanDevice& device = parentImpl();
  // DescriptorSet
  updateDescriptorSet(args...);
  // POD
  bool need_to_update_pod = false;
  if constexpr (hasPodArg()) {
    const PodTuple pod_params = makePodTuple(args...);
    auto cache = pod_cache_->mapMemory();
//    need_to_update_pod = cache[0] != pod_params;
    need_to_update_pod = true;
    ZISC_ASSERT(pod_cache_->isHostVisible(), "The cache isn't host visible.");
    if (need_to_update_pod)
      cache[0] = pod_params;
  }
  const auto work_size = BaseKernel::expandWorkSize(launch_options.workSize());
  VkCommandBuffer command = commandBuffer();
  // Command recording
  {
    CmdDebugLabelRegion debug_region{BaseKernel::isDebugMode() ? command
                                                               : VK_NULL_HANDLE,
                                     device.dispatcher(),
                                     launch_options.label(),
                                     launch_options.labelColor()};
    {
      CmdRecordRegion record_region{command,
                                    device.dispatcher(),
                                    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
      if (need_to_update_pod) {
        using BufType = VulkanBuffer<PodTuple>;
        const VkBufferCopy copy_region{0, 0, sizeof(PodTuple)};
        auto pod_cache = zisc::cast<const BufType*>(pod_cache_.get())->buffer();
        auto pod_buf = zisc::cast<const BufType*>(pod_buffer_.get())->buffer();
        device.copyBufferCmd(command, pod_cache, pod_buf, copy_region);
      }
      dispatchCmd(work_size);
    }
  }
  LaunchResult result{};
  // Command submission
  {
    const VkQueue q = device.getQueue(launch_options.queueIndex());
    if (launch_options.isExternalSyncMode())
      result.fence().setDevice(std::addressof(device));
    QueueDebugLabelRegion debug_region{BaseKernel::isDebugMode() ? q
                                                                 : VK_NULL_HANDLE,
                                       device.dispatcher(),
                                       launch_options.label(),
                                       launch_options.labelColor()};
    device.submit(command, q, result.fence());
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr bool
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
hasGlobalArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfGlobalArgs;
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr bool
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
hasLocalArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfLocalArgs;
  return result;
}

/*!
  \details No detailed description
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr bool
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
hasPodArg() noexcept
{
  const bool result = 0 < BaseKernel::ArgParser::kNumOfPodArgs;
  return result;
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
destroyData() noexcept
{
  command_buffer_ = VK_NULL_HANDLE;
  if (pipeline_ != VK_NULL_HANDLE) {
    VulkanDevice& device = parentImpl();
    pod_cache_.reset();
    pod_buffer_.reset();
    device.destroyKernelPipeline(std::addressof(pipeline_layout_),
                                 std::addressof(pipeline_));
    device.destroyKernelDescriptorSet(std::addressof(desc_set_layout_),
                                      std::addressof(desc_pool_));
  }
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
dispatchCmd(const std::array<uint32b, 3>& work_size)
{
  VulkanDevice& device = parentImpl();
  device.dispatchKernelCmd(command_buffer_, desc_set_, pipeline_layout_, pipeline_,
    kDimension, work_size);
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
initData(const Parameters& params)
{
  static_assert(hasGlobalArg(), "The kernel doesn't have global argument.");
  VulkanDevice& device = parentImpl();
  device.addShaderModule(SetType{});
  device.initKernelDescriptorSet(BaseKernel::ArgParser::kNumOfBufferArgs,
                                 zisc::cast<std::size_t>(hasPodArg() ? 1 : 0),
                                 std::addressof(desc_set_layout_),
                                 std::addressof(desc_pool_),
                                 std::addressof(desc_set_));
  const auto& module_data = device.getShaderModule(SetType::id());
  device.initKernelPipeline(BaseKernel::dimension(),
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
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
      auto handle = zisc::treatAs<const uint64b*>(std::addressof(obj));
      device.setDebugInfo(type, *handle, obj_name.data(), this);
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
template <std::size_t kIndex>
inline
auto
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
makePodTupleType() noexcept
{
  constexpr auto pod_arg_info = BaseKernel::ArgParser::getPodArgInfoList();
  if constexpr (kIndex < pod_arg_info.size()) {
    constexpr std::size_t pod_index = pod_arg_info[kIndex].index();
    using ArgTuple = std::tuple<FuncArgTypes...>;
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

  \tparam Type No description.
  \param [in] buffer No description.
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
template <typename Type>
inline
const VkBuffer&
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
getBufferHandle(const Buffer<Type>& buffer) noexcept
{
  using BuffType = VulkanBuffer<Type>;
  auto& handle = zisc::cast<const BuffType*>(std::addressof(buffer))->buffer();
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
template <std::size_t kIndex, typename Type, typename ...Types>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
initBufferList(VkBuffer* buffer_list, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
template <std::size_t kIndex, typename Type, typename ...Types>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
initPodTuple(PodTuple* pod_params, Type&& value, Types&&... rest) noexcept
{
  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
auto
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
makePodTuple(ArgTypes... args) noexcept -> PodTuple
{
  PodTuple pod_params{};
  if constexpr (0 < sizeof...(ArgTypes))
    initPodTuple<0>(std::addressof(pod_params), args...);
  return pod_params;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr std::size_t
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
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
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
VulkanDevice&
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::treatAs<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
const VulkanDevice&
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
parentImpl() const noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::treatAs<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \param [in] args No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
VulkanKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
updateDescriptorSet(ArgTypes... args)
{
  auto& device = parentImpl();

  constexpr std::size_t n = numOfBuffers();
  std::array<VkBuffer, n> buffer_list{};
  std::array<VkDescriptorType, n> desc_type_list{};
  initBufferList<0>(buffer_list.data(), args...);
  desc_type_list.fill(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
  if constexpr (hasPodArg()) {
    using BufType = VulkanBuffer<PodTuple>;
    buffer_list[n - 1] = zisc::cast<const BufType*>(pod_buffer_.get())->buffer();
    desc_type_list[n - 1] = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  }
  device.updateDescriptorSet(desc_set_, buffer_list, desc_type_list);
}

///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::bindBuffers(
//    std::add_lvalue_reference_t<BufferArgs>... args) noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//
//  if ((0 == kNumOfBuffers) || isSameArgs(args...))
//    return;
//
//  std::array<vk::Buffer, kNumOfBuffers> buffer_list{getVkBuffer(args)...};
//  std::array<vk::DescriptorBufferInfo, kNumOfBuffers> descriptor_info_list;
//  std::array<vk::WriteDescriptorSet, kNumOfBuffers> descriptor_set_list;
//
//  constexpr auto buffer_info_list = ParseResult::getGlobalArgInfoList();
//  for (std::size_t index = 0; index < kNumOfBuffers; ++index) {
//    auto& descriptor_info = descriptor_info_list[index];
//    descriptor_info.buffer = buffer_list[index];
//    descriptor_info.offset = 0;
//    descriptor_info.range = VK_WHOLE_SIZE;
//
//    auto& buffer_info = buffer_info_list[index];
//    auto& descriptor_set = descriptor_set_list[index];
//    const bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
//        (buffer_info.isConstant() || buffer_info.isPod());
//    descriptor_set.dstSet = descriptor_set_;
//    descriptor_set.dstBinding = zisc::cast<uint32b>(buffer_info.index());
//    descriptor_set.dstArrayElement = 0;
//    descriptor_set.descriptorCount = 1;
//    descriptor_set.descriptorType = is_uniform_buffer
//        ? vk::DescriptorType::eUniformBuffer
//        : vk::DescriptorType::eStorageBuffer;
//    descriptor_set.pImageInfo = nullptr;
//    descriptor_set.pBufferInfo = &descriptor_info;
//    descriptor_set.pTexelBufferView = nullptr;
//  }
//
//  const auto& device = device_->device();
//  device.updateDescriptorSets(zisc::cast<uint32b>(kNumOfBuffers),
//                              descriptor_set_list.data(),
//                              0,
//                              nullptr);
//  buffer_list_ = std::move(buffer_list);
//}
//


///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::VulkanKernel(
//    VulkanDevice* device,
//    const uint32b module_index,
//    const std::string_view kernel_name) noexcept :
//        device_{device}
//{
//  ZISC_ASSERT(device_ != nullptr, "The device is null.");
//  initialize(module_index, kernel_name);
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::~VulkanKernel()
//    noexcept
//{
//  destroy();
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::destroy()
//    noexcept
//{
//  const auto& device = device_->device();
//  if (compute_pipeline_) {
//    device.destroyPipeline(compute_pipeline_, nullptr);
//    compute_pipeline_ = nullptr;
//  }
//  if (pipeline_layout_) {
//    device.destroyPipelineLayout(pipeline_layout_, nullptr);
//    pipeline_layout_ = nullptr;
//  }
//  if (descriptor_pool_) {
//    device.destroyDescriptorPool(descriptor_pool_, nullptr);
//    descriptor_pool_ = nullptr;
//  }
//  if (descriptor_set_layout_) {
//    device.destroyDescriptorSetLayout(descriptor_set_layout_, nullptr);
//    descriptor_set_layout_ = nullptr;
//  }
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
//    noexcept -> VulkanDevice*
//{
//  return device_;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
//    const noexcept -> const VulkanDevice*
//{
//  return device_;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::SubPlatformType()
//    const noexcept -> SubPlatformType
//{
//  return SubPlatformType::kVulkan;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::run(
//    std::add_lvalue_reference_t<BufferArgs>... args,
//    const std::array<uint32b, kDimension> works,
//    const uint32b queue_index) noexcept
//{
//  if (!isSameArgs(args...))
//    bindBuffers(args...);
//  dispatch(works);
//  device()->submit(QueueType::kCompute, queue_index, command_buffer_);
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::bindBuffers(
//    std::add_lvalue_reference_t<BufferArgs>... args) noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//
//  if ((0 == kNumOfBuffers) || isSameArgs(args...))
//    return;
//
//  std::array<vk::Buffer, kNumOfBuffers> buffer_list{getVkBuffer(args)...};
//  std::array<vk::DescriptorBufferInfo, kNumOfBuffers> descriptor_info_list;
//  std::array<vk::WriteDescriptorSet, kNumOfBuffers> descriptor_set_list;
//
//  constexpr auto buffer_info_list = ParseResult::getGlobalArgInfoList();
//  for (std::size_t index = 0; index < kNumOfBuffers; ++index) {
//    auto& descriptor_info = descriptor_info_list[index];
//    descriptor_info.buffer = buffer_list[index];
//    descriptor_info.offset = 0;
//    descriptor_info.range = VK_WHOLE_SIZE;
//
//    auto& buffer_info = buffer_info_list[index];
//    auto& descriptor_set = descriptor_set_list[index];
//    const bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
//        (buffer_info.isConstant() || buffer_info.isPod());
//    descriptor_set.dstSet = descriptor_set_;
//    descriptor_set.dstBinding = zisc::cast<uint32b>(buffer_info.index());
//    descriptor_set.dstArrayElement = 0;
//    descriptor_set.descriptorCount = 1;
//    descriptor_set.descriptorType = is_uniform_buffer
//        ? vk::DescriptorType::eUniformBuffer
//        : vk::DescriptorType::eStorageBuffer;
//    descriptor_set.pImageInfo = nullptr;
//    descriptor_set.pBufferInfo = &descriptor_info;
//    descriptor_set.pTexelBufferView = nullptr;
//  }
//
//  const auto& device = device_->device();
//  device.updateDescriptorSets(zisc::cast<uint32b>(kNumOfBuffers),
//                              descriptor_set_list.data(),
//                              0,
//                              nullptr);
//  buffer_list_ = std::move(buffer_list);
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::dispatch(
//    std::array<uint32b, kDimension> works) noexcept
//{
//  const auto group_size = device_->calcWorkGroupSize(works);
//  vk::CommandBufferBeginInfo begin_info{};
//  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
//  command_buffer_.begin(begin_info);
//
//  command_buffer_.bindPipeline(vk::PipelineBindPoint::eCompute, compute_pipeline_);
//  command_buffer_.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
//                                     pipeline_layout_,
//                                     0,
//                                     1,
//                                     &descriptor_set_,
//                                     0,
//                                     nullptr);
//  command_buffer_.dispatch(group_size[0], group_size[1], group_size[2]);
//
//  command_buffer_.end();
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//template <typename Type> inline
//auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::getVkBuffer(
//    Type&& buffer) const noexcept -> vk::Buffer&
//{
//  ZISC_ASSERT(buffer.SubPlatformType() == SubPlatformType::kVulkan,
//              "The device type of the buffer isn't vulkan.");
//
//  using ElementType = typename std::remove_reference_t<Type>::Type;
//  constexpr DescriptorType descriptor =
//      std::remove_reference_t<Type>::descriptorType();
//  using VulkanBufferT = VulkanBuffer<descriptor, ElementType>;
//  using VulkanBufferPtr = std::add_pointer_t<VulkanBufferT>;
//
//  auto b = zisc::cast<VulkanBufferPtr>(std::addressof(buffer));
//  return b->buffer();
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initCommandBuffer()
//    noexcept
//{
//  const vk::CommandBufferAllocateInfo alloc_info{
//      device_->commandPool(QueueType::kCompute),
//      vk::CommandBufferLevel::ePrimary,
//      1};
//  const auto& device = device_->device();
//  auto [result, command_buffers] = device.allocateCommandBuffers(alloc_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Command buffer allocation failed.");
//  command_buffer_ = command_buffers[0];
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initComputePipeline(
//    const uint32b module_index,
//    const std::string_view kernel_name) noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//
//  // Set constant IDs
//  constexpr std::size_t num_of_entries = 3u + ParseResult::kNumOfLocalArgs;
//  std::array<uint32b, num_of_entries> constant_data;
//  {
//    const auto& local_work_size = device_->localWorkSize<kDimension>();
//    for (std::size_t i = 0; i < local_work_size.size(); ++i)
//      constant_data[i] = local_work_size[i];
//    for (std::size_t i = local_work_size.size(); i < num_of_entries; ++i)
//      constant_data[i] = device_->subgroupSize();
//  }
//  std::array<vk::SpecializationMapEntry, num_of_entries> entries;
//  for (std::size_t i = 0; i < entries.size(); ++i) {
//    entries[i].constantID = zisc::cast<uint32b>(i);
//    entries[i].offset = zisc::cast<uint32b>(i * sizeof(uint32b));
//    entries[i].size = sizeof(uint32b);
//  }
//  const vk::SpecializationInfo info{zisc::cast<uint32b>(num_of_entries),  
//                                    entries.data(),
//                                    num_of_entries * sizeof(uint32b),
//                                    constant_data.data()};
//  // Shader stage create info
//  const auto& shader_module = device_->getShaderModule(module_index);
//  const vk::PipelineShaderStageCreateInfo shader_stage_create_info{
//      vk::PipelineShaderStageCreateFlags{},
//      vk::ShaderStageFlagBits::eCompute,
//      shader_module,
//      kernel_name.data(),
//      &info};
//  // Pipeline create info
//  const vk::ComputePipelineCreateInfo create_info{
//      vk::PipelineCreateFlags{},
//      shader_stage_create_info,
//      pipeline_layout_};
//
//  const auto& device = device_->device();
//  auto [result, pipelines] = device.createComputePipelines(vk::PipelineCache{},
//                                                           create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Compute pipeline creation failed.");
//  compute_pipeline_ = pipelines[0];
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorPool()
//    noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//  static_assert(0 < ParseResult::kNumOfStorageBuffer,
//                "The kernel doesn't have any storage buffer argument.");
//
//  constexpr bool has_uniform_buffer = Config::isUniformBufferEnabled() &&
//                                      (0 < ParseResult::kNumOfUniformBuffer);
//  constexpr std::size_t num_of_info = has_uniform_buffer ? 2 : 1;
//  std::array<vk::DescriptorPoolSize, num_of_info> pool_size_list;
//  if constexpr (has_uniform_buffer) {
//    pool_size_list[0].type = vk::DescriptorType::eUniformBuffer;
//    pool_size_list[0].descriptorCount =
//        zisc::cast<uint32b>(ParseResult::kNumOfUniformBuffer);
//    pool_size_list[1].type = vk::DescriptorType::eStorageBuffer;
//    pool_size_list[1].descriptorCount =
//        zisc::cast<uint32b>(ParseResult::kNumOfStorageBuffer);
//    static_assert(kNumOfBuffers ==
//        (ParseResult::kNumOfStorageBuffer + ParseResult::kNumOfUniformBuffer),
//        "The number of arguments is wrong.");
//  }
//  else {
//    pool_size_list[0].type = vk::DescriptorType::eStorageBuffer;
//    pool_size_list[0].descriptorCount = zisc::cast<uint32b>(kNumOfBuffers);
//  }
//  const vk::DescriptorPoolCreateInfo create_info{vk::DescriptorPoolCreateFlags{},
//                                                 1,
//                                                 zisc::cast<uint32b>(num_of_info),
//                                                 pool_size_list.data()};
//  const auto& device = device_->device();
//  auto [result, descriptor_pool] = device.createDescriptorPool(create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Descriptor pool creation failed.");
//  descriptor_pool_ = descriptor_pool;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorSet()
//    noexcept
//{
//  const vk::DescriptorSetAllocateInfo alloc_info{descriptor_pool_,
//                                                 1,
//                                                 &descriptor_set_layout_};
//  const auto& device = device_->device();
//  auto [result, descriptor_sets] = device.allocateDescriptorSets(alloc_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Descriptor set allocation failed.");
//  descriptor_set_ = descriptor_sets[0];
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorSetLayout()
//    noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//  constexpr auto buffer_info_list = ParseResult::getGlobalArgInfoList();
//
//  std::array<vk::DescriptorSetLayoutBinding, kNumOfBuffers> layout_bindings;
//  for (std::size_t index = 0; index < kNumOfBuffers; ++index) {
//    auto& buffer_info = buffer_info_list[index];
//    const bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
//        (buffer_info.isConstant() || buffer_info.isPod());
//
//    layout_bindings[index] = vk::DescriptorSetLayoutBinding{
//      zisc::cast<uint32b>(index),
//        is_uniform_buffer ? vk::DescriptorType::eUniformBuffer
//                          : vk::DescriptorType::eStorageBuffer,
//        1,
//        vk::ShaderStageFlagBits::eCompute};
//  }
//  const vk::DescriptorSetLayoutCreateInfo create_info{
//      vk::DescriptorSetLayoutCreateFlags{},
//      zisc::cast<uint32b>(layout_bindings.size()),
//      layout_bindings.data()};
//  const auto& device = device_->device();
//  auto [result, descriptor_set_layout] = device.createDescriptorSetLayout(create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess,
//              "Descriptor set layout creation failed.");
//  descriptor_set_layout_ = descriptor_set_layout;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initialize(
//    const uint32b module_index, const std::string_view kernel_name) noexcept
//{
//  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
//  static_assert(kNumOfBuffers == ParseResult::kNumOfGlobalArgs,
//                "The number of buffers is wrong.");
//
//  initDescriptorSetLayout();
//  initDescriptorPool();
//  initDescriptorSet();
//  initPipelineLayout();
//  initComputePipeline(module_index, kernel_name);
//  initCommandBuffer();
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initPipelineLayout()
//    noexcept
//{
//  const vk::PipelineLayoutCreateInfo create_info{
//      vk::PipelineLayoutCreateFlags{},
//      1,
//      &descriptor_set_layout_};
//  const auto& device = device_->device();
//  auto [result, pipeline_layout] = device.createPipelineLayout(create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Pipeline layout creation failed.");
//  pipeline_layout_ = pipeline_layout;
//}
//
///*!
//  */
//template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
//inline
//bool VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::isSameArgs(
//    std::add_lvalue_reference_t<BufferArgs>... args) const noexcept
//{
//  std::array<vk::Buffer, kNumOfBuffers> buffer_list{{getVkBuffer(args)...}};
//  bool result = true;
//  for (std::size_t i = 0; (i < buffer_list.size()) && result; ++i)
//    result = buffer_list_[i] == buffer_list[i];
//  return result;
//}

/*!
  \details No detailed description

  \tparam kDimension No description.
  \tparam SetType No description.
  \tparam ArgTypes No description.
  \param [in] parameters No description.
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...ArgTypes> inline
SharedKernel<kDimension, SetType, ArgTypes...> VulkanDevice::makeKernel(
    const KernelParameters<SetType, ArgTypes...>& parameters)
{
  using SharedKernelType = SharedKernel<kDimension, SetType, ArgTypes...>;
  using ArgParser = KernelArgParser<ArgTypes...>;
  using KernelType = typename ArgParser:: template VulkanKernelType<kDimension, SetType>;
  zisc::pmr::polymorphic_allocator<KernelType> alloc{memoryResource()};
  SharedKernelType kernel = std::allocate_shared<KernelType>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakKernel<kDimension, SetType, ArgTypes...> own{kernel};
  kernel->initialize(std::move(parent), std::move(own), parameters);

  return kernel;
}

} // namespace zivc

#endif // ZIVC_VULKAN_KERNEL_INL_HPP
