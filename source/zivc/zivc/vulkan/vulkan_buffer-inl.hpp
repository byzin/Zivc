/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_INL_HPP
#define ZIVC_VULKAN_BUFFER_INL_HPP

#include "vulkan_buffer.hpp"
// Standard C++ library
#include <algorithm>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_buffer_impl.hpp"
#include "vulkan_device.hpp"
#include "vulkan_device_info.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "zivc/buffer.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/buffer_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/error.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/mapped_memory.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
VulkanBuffer<T>::VulkanBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
VulkanBuffer<T>::~VulkanBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VmaAllocation& VulkanBuffer<T>::allocation() noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VmaAllocation& VulkanBuffer<T>::allocation() const noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return vm_alloc_info_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VkBuffer& VulkanBuffer<T>::buffer() noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VkBuffer& VulkanBuffer<T>::buffer() const noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VkCommandBuffer& VulkanBuffer<T>::commandBuffer() noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VkCommandBuffer& VulkanBuffer<T>::commandBuffer() const noexcept
{
  return command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto VulkanBuffer<T>::descriptorType() const noexcept -> DescriptorType
{
  return desc_type_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VkBufferUsageFlagBits VulkanBuffer<T>::descriptorTypeVk() const noexcept
{
  VkBufferUsageFlagBits flag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  switch (descriptorType()) {
   case DescriptorType::kUniform:
    flag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    break;
   case DescriptorType::kStorage:
    flag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    break;
   default:
    break;
  }
  return flag;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t VulkanBuffer<T>::heapIndex() const noexcept
{
  const auto& mem_type = memoryType();
  return mem_type.heapIndex;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool VulkanBuffer<T>::isDeviceLocal() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool VulkanBuffer<T>::isHostCached() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool VulkanBuffer<T>::isHostCoherent() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool VulkanBuffer<T>::isHostVisible() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  return result;
}

/*!
  \details No detailed description

  \param [in] type No description.
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::setDescriptorType(const DescriptorType type) noexcept
{
  desc_type_ = type;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_cap = capacityImpl();
  if (prev_cap < s) {
    Buffer<T>::clear();
    const std::size_t mem_size = sizeof(Type) * s;
    const VulkanBufferImpl impl{std::addressof(parentImpl())};
    impl.allocateMemory(mem_size,
                        Buffer<T>::usage(),
                        descriptorTypeVk(),
                        std::addressof(Buffer<T>::id()),
                        std::addressof(buffer()),
                        std::addressof(allocation()),
                        std::addressof(vm_alloc_info_));
    ZivcObject::updateDebugInfo();
    initCommandBuffer();
  }
  size_ = s;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t VulkanBuffer<T>::capacityImpl() const noexcept
{
  const auto& info = allocationInfo();
  const std::size_t s = info.size / sizeof(Type);
  return s;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::destroyData() noexcept
{
  if (buffer() != ZIVC_VK_NULL_HANDLE) {
    const VulkanBufferImpl impl{std::addressof(parentImpl())};
    impl.deallocateMemory(std::addressof(buffer()),
                          std::addressof(allocation()),
                          std::addressof(vm_alloc_info_));
    buffer_ = ZIVC_VK_NULL_HANDLE;
    vm_allocation_ = ZIVC_VK_NULL_HANDLE;
    size_ = 0;
  }
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::initData()
{
  VulkanDevice& device = parentImpl();
  {
    const VulkanBufferImpl impl{std::addressof(device)};
    impl.initAllocationInfo(Buffer<T>::usage(),
                            descriptorTypeVk(),
                            std::addressof(vm_alloc_info_));
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto VulkanBuffer<T>::mappedMemory() const -> Pointer
{
  void* p = nullptr;
  const auto& device = parentImpl();
  const VkResult result = vmaMapMemory(device.memoryAllocator(), allocation(), &p);
  if (result != VK_SUCCESS) {
    const char* message = "Memory mapping failed.";
    VulkanBufferImpl::throwResultException(result, message);
  }
  return zisc::cast<Pointer>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t VulkanBuffer<T>::sizeImpl() const noexcept
{
  return size_;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::unmapMemory() const noexcept
{
  const auto& device = parentImpl();
  vmaUnmapMemory(device.memoryAllocator(), allocation());
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::updateDebugInfoImpl() noexcept
{
  auto& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  const std::string_view buffer_name = id_data.name();
  if (buffer() != ZIVC_VK_NULL_HANDLE) {
    device.setDebugInfo(VK_OBJECT_TYPE_BUFFER, buffer(), buffer_name, this);
  }
  if (command_buffer_ != ZIVC_VK_NULL_HANDLE) {
    IdData::NameType obj_name{""};
    const std::string_view suffix{"_commandbuffer"};
    std::strncpy(obj_name.data(), buffer_name.data(), buffer_name.size() + 1);
    std::strncat(obj_name.data(), suffix.data(), suffix.size());
    const std::string name = obj_name.data();
    device.setDebugInfo(VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_, name, this);
  }
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::copyFromImpl(
    const Buffer<T>& source,
    const BufferLaunchOptions<T>& launch_options)
{
  ZISC_ASSERT(source.type() == SubPlatformType::kVulkan, "'source' isn't vulkan type.");
  LaunchResult result{};
  if (isDeviceLocal() || source.isDeviceLocal())
    result = copyOnDevice(source, launch_options);
  else
    result = copyOnHost(source, launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::copyOnDevice(
    const Buffer<T>& source,
    const BufferLaunchOptions<T>& launch_options)
{
  ZISC_ASSERT(isDeviceLocal() || source.isDeviceLocal(), "No device local buffer.");
  VulkanDevice& device = parentImpl();
  auto src = zisc::cast<const VulkanBuffer*>(std::addressof(source));
  VkCommandBuffer command = isDeviceLocal() ? commandBuffer() : src->commandBuffer();
  {
    constexpr auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Record buffer copying operation
      const VkBufferCopy copy_region{launch_options.sourceOffsetInBytes(),
                                     launch_options.destOffsetInBytes(),
                                     launch_options.sizeInBytes()};
      const VulkanBufferImpl impl{std::addressof(device)};
      impl.copyCmd(command, src->buffer(), buffer(), copy_region);
    }
  }
  LaunchResult result{};
  // Submit the recorded commands
  {
    VkQueue q = device.getQueue(launch_options.queueIndex());
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isExternalSyncMode() ? &device : nullptr);
    auto debug_region = device.makeQueueDebugLabel(q, launch_options);
    device.submit(command, q, fence);
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::copyOnHost(
    const Buffer<T>& source,
    const BufferLaunchOptions<T>& launch_options) noexcept
{
  {
    auto source_mem = source.mapMemory();
    auto memory = Buffer<T>::mapMemory();
    ConstPointer src = source_mem.begin() + launch_options.sourceOffset();
    Pointer dest = memory.begin() + launch_options.destOffset();
    std::copy_n(src, launch_options.size(), dest);
  }
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::fillFastOnDevice(
    ConstReference value,
    const BufferLaunchOptions<T>& launch_options)
{
  // Create a data for fill
  const uint32b data = makeDataForFillFast(value);

  VulkanDevice& device = parentImpl();
  VkCommandBuffer command = commandBuffer();
  {
    constexpr auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Record buffer filling operation
      const VulkanBufferImpl impl{std::addressof(device)};
      const std::size_t offsetBytes = launch_options.destOffsetInBytes();
      const std::size_t sizeBytes = launch_options.sizeInBytes();
      impl.fillFastCmd(command, buffer(), offsetBytes, sizeBytes, data);
    }
  }
  LaunchResult result{};
  // Submit the recorded commands
  {
    VkQueue q = device.getQueue(launch_options.queueIndex());
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isExternalSyncMode() ? &device : nullptr);
    auto debug_region = device.makeQueueDebugLabel(q, launch_options);
    device.submit(command, q, fence);
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::fillImpl(ConstReference value,
                                       const BufferLaunchOptions<T>& launch_options)
{
  LaunchResult result{};
  if (isDeviceLocal()) {
    constexpr bool vsize_can_be_multiple_of_4 = (sizeof(T) <= 4) &&
                                                zisc::has_single_bit(sizeof(T));
    const std::size_t offsetBytes = launch_options.destOffsetInBytes();
    const std::size_t sizeBytes = launch_options.sizeInBytes();
    if (vsize_can_be_multiple_of_4 && (offsetBytes % 4 == 0) && (sizeBytes % 4 == 0))
      result = fillFastOnDevice(value, launch_options);
    else
      result = fillOnDevice(value, launch_options);
  }
  else {
    result = fillOnHost(value, launch_options);
  }
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::fillOnDevice(
    ConstReference value,
    const BufferLaunchOptions<T>& launch_options)
{
  static_cast<void>(value);
  static_cast<void>(launch_options);
  ZISC_ASSERT(false, "'fillDevice' isn't implemented yet.");
  //! \todo Make a fill array and copy to the device?
  LaunchResult result{};
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
LaunchResult VulkanBuffer<T>::fillOnHost(
    ConstReference value,
    const BufferLaunchOptions<T>& launch_options) noexcept
{
  {
    auto memory = Buffer<T>::mapMemory();
    Pointer dest = memory.begin() + launch_options.destOffset();
    std::fill_n(dest, launch_options.size(), value);
  }
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
bool VulkanBuffer<T>::hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept
{
  const auto& mem_type = memoryType();
  const bool has_property = (mem_type.propertyFlags & flag) == flag;
  return has_property;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::initCommandBuffer()
{
  if ((commandBuffer() == ZIVC_VK_NULL_HANDLE) && isDeviceLocal()) {
    auto& device = parentImpl();
    command_buffer_ = device.makeCommandBuffer();
    updateDebugInfoImpl();
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VkMemoryType& VulkanBuffer<T>::memoryType() const noexcept
{
  const auto& device = parentImpl();
  const auto& device_info = device.deviceInfoImpl();
  const auto& info = allocationInfo();

  const auto& mem_props = device_info.memoryProperties().properties1_;
  const auto& mem_type = mem_props.memoryTypes[info.memoryType];
  return mem_type;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
uint32b VulkanBuffer<T>::makeDataForFillFast(ConstReference value) noexcept
{
  uint32b data = 0;
  constexpr bool vsize_can_be_multiple_of_4 = (sizeof(T) <= sizeof(data)) &&
                                              zisc::has_single_bit(sizeof(T));
  if constexpr (vsize_can_be_multiple_of_4) {
    using ValueT = std::conditional_t<sizeof(T) == 1, uint8b,
                   std::conditional_t<sizeof(T) == 2, uint16b,
                                                      uint32b>>;
    const uint32b v = zisc::cast<uint32b>(zisc::bit_cast<ValueT>(value));
    constexpr std::size_t n = sizeof(data) / sizeof(ValueT);
    for (std::size_t i = 0; i < n; ++i) {
      constexpr std::size_t bits = 8 * sizeof(ValueT);
      const std::size_t shift = i * bits;
      const uint32b d = v << shift;
      data = data | d;
    }
  }
  return data;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VulkanDevice& VulkanBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *zisc::reinterp<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VulkanDevice& VulkanBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::reinterp<const VulkanDevice*>(p);
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_INL_HPP
