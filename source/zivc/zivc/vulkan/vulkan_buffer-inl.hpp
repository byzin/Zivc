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
#include "zisc/concepts.hpp"
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
  return rawBuffer().vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VmaAllocation& VulkanBuffer<T>::allocation() const noexcept
{
  return rawBuffer().vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return rawBuffer().vm_alloc_info_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VkBuffer& VulkanBuffer<T>::buffer() noexcept
{
  return rawBuffer().buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VkBuffer& VulkanBuffer<T>::buffer() const noexcept
{
  return rawBuffer().buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
std::size_t VulkanBuffer<T>::capacityInBytes() const noexcept
{
  const auto& info = allocationInfo();
  const std::size_t c = info.size;
  return c;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
VkCommandBuffer& VulkanBuffer<T>::commandBuffer() noexcept
{
  return rawBuffer().command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VkCommandBuffer& VulkanBuffer<T>::commandBuffer() const noexcept
{
  return rawBuffer().command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto VulkanBuffer<T>::descriptorType() const noexcept -> DescriptorType
{
  return rawBuffer().desc_type_;
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

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
void* VulkanBuffer<T>::mapMemoryData() const
{
  void* p = nullptr;
  const auto& device = parentImpl();
  const VkResult result = vmaMapMemory(device.memoryAllocator(), allocation(), &p);
  if (result != VK_SUCCESS) {
    const char* message = "Memory mapping failed.";
    VulkanBufferImpl::throwResultException(result, message);
  }
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto VulkanBuffer<T>::rawBuffer() noexcept -> BufferData&
{
  return buffer_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
auto VulkanBuffer<T>::rawBuffer() const noexcept -> const BufferData&
{
  return buffer_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
void* VulkanBuffer<T>::rawBufferData() noexcept
{
  return std::addressof(rawBuffer());
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const void* VulkanBuffer<T>::rawBufferData() const noexcept
{
  return std::addressof(rawBuffer());
}

/*!
  \details No detailed description

  \param [in] type No description.
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::setDescriptorType(const DescriptorType type) noexcept
{
  rawBuffer().desc_type_ = type;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_cap = Buffer<T>::capacity();
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
                        std::addressof(rawBuffer().vm_alloc_info_));
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
std::size_t VulkanBuffer<T>::sizeInBytes() const noexcept
{
  const std::size_t s = sizeof(Type) * size_;
  return s;
}

/*!
  \details No detailed description
  */
template <zisc::TriviallyCopyable T> inline
void VulkanBuffer<T>::unmapMemoryData() const noexcept
{
  const auto& device = parentImpl();
  vmaUnmapMemory(device.memoryAllocator(), allocation());
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
                          std::addressof(rawBuffer().vm_alloc_info_));
    rawBuffer().buffer_ = ZIVC_VK_NULL_HANDLE;
    rawBuffer().vm_allocation_ = ZIVC_VK_NULL_HANDLE;
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
                            std::addressof(rawBuffer().vm_alloc_info_));
  }
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
  if (rawBuffer().command_buffer_ != ZIVC_VK_NULL_HANDLE) {
    IdData::NameType obj_name{""};
    const std::string_view suffix{"_commandbuffer"};
    std::strncpy(obj_name.data(), buffer_name.data(), buffer_name.size() + 1);
    std::strncat(obj_name.data(), suffix.data(), suffix.size());
    const std::string name = obj_name.data();
    device.setDebugInfo(VK_OBJECT_TYPE_COMMAND_BUFFER, rawBuffer().command_buffer_, name, this);
  }
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::copyFromImpl(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  LaunchResult result{};
  if (source.isDeviceLocal() || dest->isDeviceLocal())
    result = copyOnDevice(source, dest, launch_options);
  else
    result = copyOnHost(source, dest, launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::copyOnDevice(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  VulkanDevice& device = *zisc::cast<VulkanDevice*>(dest->getParent());

  const auto& src_data = *zisc::cast<const BufferData*>(source.rawBufferData());
  auto& dst_data = *zisc::cast<BufferData*>(dest->rawBufferData());

  VkCommandBuffer command = dest->isDeviceLocal() ? dst_data.command_buffer_
                                                  : src_data.command_buffer_;
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
      impl.copyCmd(command, src_data.buffer_, dst_data.buffer_, copy_region);
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
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::copyOnHost(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options) noexcept
{
  {
    using ConstD = std::add_const_t<D>;
    auto src_data = source.makeMappedMemory<ConstD>();
    auto* src = src_data.begin() + launch_options.sourceOffset();
    auto dst_data = dest->makeMappedMemory<D>();
    auto* dst = dst_data.begin() + launch_options.destOffset();
    std::copy_n(src, launch_options.size(), dst);
  }
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::fillFastOnDevice(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  VulkanDevice& device = *zisc::cast<VulkanDevice*>(dest->getParent());

  // Create a data for fill
  const uint32b data = makeDataForFillFast(value);
  auto& dst_data = *zisc::cast<BufferData*>(dest->rawBufferData());
  VkCommandBuffer command = dst_data.command_buffer_;
  // Record commands
  {
    constexpr auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    auto record_region = device.makeCmdRecord(command, flags);
    {
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Record buffer filling operation
      const VulkanBufferImpl impl{std::addressof(device)};
      const std::size_t offsetBytes = launch_options.destOffsetInBytes();
      const std::size_t sizeBytes = launch_options.sizeInBytes();
      impl.fillFastCmd(command, dst_data.buffer_, offsetBytes, sizeBytes, data);
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
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::fillImpl(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  LaunchResult result{};
  if (dest->isDeviceLocal()) {
    constexpr bool vsize_is_factor_of_4 = (sizeof(T) <= 4) &&
                                          zisc::has_single_bit(sizeof(T));
    const std::size_t offsetBytes = launch_options.destOffsetInBytes();
    const std::size_t sizeBytes = launch_options.sizeInBytes();
    if (vsize_is_factor_of_4 && (offsetBytes % 4 == 0) && (sizeBytes % 4 == 0))
      result = fillFastOnDevice(value, dest, launch_options);
    else
      result = fillOnDevice(value, dest, launch_options);
  }
  else {
    result = fillOnHost(value, dest, launch_options);
  }
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::fillOnDevice(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  static_cast<void>(value);
  static_cast<void>(dest);
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
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> template <zisc::TriviallyCopyable D> inline
LaunchResult VulkanBuffer<T>::fillOnHost(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options) noexcept
{
  {
    auto dst_data = dest->makeMappedMemory<D>();
    auto dst = dst_data.begin() + launch_options.destOffset();
    std::fill_n(dst, launch_options.size(), value);
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
    rawBuffer().command_buffer_ = device.makeCommandBuffer();
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
  using ValueT = std::conditional_t<sizeof(T) == 1, uint8b,
                 std::conditional_t<sizeof(T) == 2, uint16b,
                 std::conditional_t<sizeof(T) == 4, uint32b,
                                                    void*>>>;
  uint32b data = 0;
  if constexpr (zisc::UnsignedInteger<ValueT>) {
    data = zisc::cast<uint32b>(zisc::bit_cast<ValueT>(value));
    if constexpr (sizeof(ValueT) == 1)
      data = zisc::cast<uint32b>(data << 8) | data;
    if constexpr (sizeof(ValueT) <= 2)
      data = zisc::cast<uint32b>(data << 16) | data;
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
  return *zisc::cast<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <zisc::TriviallyCopyable T> inline
const VulkanDevice& VulkanBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::cast<const VulkanDevice*>(p);
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_INL_HPP
