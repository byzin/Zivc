/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_INL_HPP
#define ZIVC_VULKAN_BUFFER_INL_HPP

#include "vulkan_buffer.hpp"
// Standard C++ library
#include <algorithm>
#include <bit>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/bit.hpp"
#include "zisc/boolean.hpp"
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
#include "zivc/backend.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/buffer_init_params.hpp"
#include "zivc/utility/buffer_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/error.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <KernelArg T> inline
VulkanBuffer<T>::VulkanBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
VulkanBuffer<T>::~VulkanBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
VmaAllocation& VulkanBuffer<T>::allocation() noexcept
{
  return rawBuffer().vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const VmaAllocation& VulkanBuffer<T>::allocation() const noexcept
{
  return rawBuffer().vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return rawBuffer().vm_alloc_info_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
VkBuffer& VulkanBuffer<T>::buffer() noexcept
{
  return rawBuffer().buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const VkBuffer& VulkanBuffer<T>::buffer() const noexcept
{
  return rawBuffer().buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
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
template <KernelArg T> inline
VkCommandBuffer& VulkanBuffer<T>::commandBuffer() noexcept
{
  return rawBuffer().command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const VkCommandBuffer& VulkanBuffer<T>::commandBuffer() const noexcept
{
  return rawBuffer().command_buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto VulkanBuffer<T>::descriptorType() const noexcept -> DescriptorT
{
  return rawBuffer().desc_type_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
VkBufferUsageFlagBits VulkanBuffer<T>::descriptorTypeVk() const noexcept
{
  VkBufferUsageFlagBits flag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  switch (descriptorType()) {
   case DescriptorT::kUniform:
    flag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    break;
   case DescriptorT::kStorage:
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
template <KernelArg T> inline
std::size_t VulkanBuffer<T>::heapIndex() const noexcept
{
  const auto& mem_type = memoryType();
  return mem_type.heapIndex;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::isDeviceLocal() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::isHostCached() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::isHostCoherent() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::isHostVisible() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
void* VulkanBuffer<T>::mapMemoryData() const
{
  void* p = nullptr;
  const auto& device = parentImpl();
  const VkResult result = vmaMapMemory(device.memoryAllocator(), allocation(), &p);
  if (result != VK_SUCCESS) [[unlikely]] {
    const std::string message = createErrorMessage(
        *this,
        "Memory mapping failed.");
    VulkanBufferImpl::throwResultException(result, message.data());
  }
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
void* VulkanBuffer<T>::rawBufferData()
{
  return std::addressof(rawBuffer());
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const void* VulkanBuffer<T>::rawBufferData() const noexcept
{
  return std::addressof(rawBuffer());
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <KernelArg T> inline
void VulkanBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_cap = Buffer<T>::capacity();
  if (prev_cap < s) {
    Buffer<T>::clear();
    const std::size_t mem_size = sizeof(Type) * s;
    const VulkanBufferImpl impl{std::addressof(parentImpl())};
    impl.allocateMemory(mem_size,
                        *this,
                        descriptorTypeVk(),
                        std::addressof(Buffer<T>::id()),
                        std::addressof(buffer()),
                        std::addressof(allocation()),
                        std::addressof(rawBuffer().vm_alloc_info_));
    if (!isInternal()) {
      initCommandBuffer();
      initFillKernel();
    }
    ZivcObject::updateDebugInfo();
  }
  size_ = s;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t VulkanBuffer<T>::sizeInBytes() const noexcept
{
  const std::size_t s = sizeof(Type) * size_;
  return s;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void VulkanBuffer<T>::unmapMemoryData() const noexcept
{
  const auto& device = parentImpl();
  vmaUnmapMemory(device.memoryAllocator(), allocation());
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void VulkanBuffer<T>::destroyData() noexcept
{
  if (rawBuffer().fill_data_)
    rawBuffer().fill_data_.reset();
  if (rawBuffer().fill_kernel_)
    rawBuffer().fill_kernel_.reset();
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

  \param [in] params No description.
  */
template <KernelArg T> inline
void VulkanBuffer<T>::initData(const BufferInitParams& params)
{
  VulkanDevice& device = parentImpl();
  {
    rawBuffer().desc_type_ = params.descriptorType();
    is_internal_ = params.internalBufferFlag();
  }
  {
    const VulkanBufferImpl impl{std::addressof(device)};
    impl.initAllocationInfo(*this,
                            descriptorTypeVk(),
                            std::addressof(rawBuffer().vm_alloc_info_));
  }
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void VulkanBuffer<T>::updateDebugInfoImpl() noexcept
{
  auto& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  const std::string_view buffer_name = id_data.name();
  if (buffer() != ZIVC_VK_NULL_HANDLE) {
    device.setDebugInfo(VK_OBJECT_TYPE_BUFFER, buffer(), buffer_name, this);
  }
  if (rawBuffer().command_buffer_ != ZIVC_VK_NULL_HANDLE) {
    IdData::NameT obj_name{""};
    const std::string_view suffix{"_commandbuffer"};
    copyStr(buffer_name, obj_name.data());
    concatStr(suffix, obj_name.data());
    const std::string name = obj_name.data();
    device.setDebugInfo(VK_OBJECT_TYPE_COMMAND_BUFFER, rawBuffer().command_buffer_, name, this);
  }
  if (rawBuffer().fill_kernel_) {
    IdData::NameT obj_name{""};
    const std::string_view suffix{"_fillkernel"};
    copyStr(buffer_name, obj_name.data());
    concatStr(suffix, obj_name.data());
    rawBuffer().fill_kernel_->setName(obj_name.data());
  }
  if (rawBuffer().fill_data_) {
    IdData::NameT obj_name{""};
    const std::string_view suffix{"_filldata"};
    copyStr(buffer_name, obj_name.data());
    concatStr(suffix, obj_name.data());
    rawBuffer().fill_data_->setName(obj_name.data());
  }
}

/*!
  \details According to the vkCmdFillBuffer specification: https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkCmdFillBuffer.html
  size and offset must be multiple of 4

  \param [in] size_bytes No description.
  \param [in] offset_bytes No description.
  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::canFillFast(const std::size_t offset_bytes,
                                  const std::size_t size_bytes) noexcept
{
  constexpr bool is_t_mul_of_4 = (sizeof(T) <= 4) && std::has_single_bit(sizeof(T));
  const bool result = is_t_mul_of_4 && (offset_bytes % 4 == 0) && (size_bytes % 4 == 0);
  return result;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::copyFromImpl(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  LaunchResult result{};
  const bool has_device_local = source.isDeviceLocal() || dest->isDeviceLocal();
  const bool is_host_copyable = source.isHostReadable() && dest->isHostWritable();
  if (has_device_local || !is_host_copyable)
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
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::copyOnDevice(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  VulkanDevice& device = *static_cast<VulkanDevice*>(dest->getParent());

  const auto& src_data = *static_cast<const BufferData*>(source.rawBufferData());
  auto& dst_data = *static_cast<BufferData*>(dest->rawBufferData());

  VkCommandBuffer command = dest->isDeviceLocal() ? dst_data.command_buffer_
                                                  : src_data.command_buffer_;
  {
    constexpr auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    // Recording scope
    auto record_region = device.makeCmdRecord(command, flags);
    {
      // Start labeling for debug until the end of the scope
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
    constexpr VulkanDeviceCapability cap = VulkanDeviceCapability::kCompute;
    VkQueue q = device.getQueue(cap, launch_options.queueIndex());
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isFenceRequested() ? &device : nullptr);
    // Start labeling for debug until the end of the scope
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
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::copyOnHost(
    const BufferCommon& source,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options) noexcept
{
  {
    using DataT = std::remove_cvref_t<D>;
    using DataPtr = std::add_pointer_t<DataT>;
    using ConstDataPtr = std::add_pointer_t<std::add_const_t<DataT>>;
    const auto* src_ptr = static_cast<ConstDataPtr>(source.mapMemoryData());
    src_ptr = src_ptr + launch_options.sourceOffset();
    auto* dst_ptr = static_cast<DataPtr>(dest->mapMemoryData());
    dst_ptr = dst_ptr + launch_options.destOffset();
    std::copy_n(src_ptr, launch_options.size(), dst_ptr);
  }
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \return No description
  */
template <KernelArg T> inline
uint32b VulkanBuffer<T>::createDataForFillFast(ConstReference value) noexcept
{
  using ValueT = std::conditional_t<sizeof(T) == 1, uint8b,
                 std::conditional_t<sizeof(T) == 2, uint16b,
                 std::conditional_t<sizeof(T) == 4, uint32b,
                                                    void*>>>;
  uint32b data = 0;
  if constexpr (zisc::UnsignedInteger<ValueT>) {
    data = static_cast<uint32b>(zisc::bit_cast<ValueT>(value));
    if constexpr (sizeof(ValueT) == 1)
      data = static_cast<uint32b>(data << 8) | data;
    if constexpr (sizeof(ValueT) <= 2)
      data = static_cast<uint32b>(data << 16) | data;
  }
  return data;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::fillFastOnDevice(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  VulkanDevice& device = *static_cast<VulkanDevice*>(dest->getParent());

  // Create a data for fill
  const uint32b data = createDataForFillFast(value);
  auto& dst_data = *static_cast<BufferData*>(dest->rawBufferData());
  VkCommandBuffer command = dst_data.command_buffer_;
  // Record commands
  {
    constexpr auto flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    // Recording scope
    auto record_region = device.makeCmdRecord(command, flags);
    {
      // Start labeling for debug until the end of the scope
      auto debug_region = device.makeCmdDebugLabel(command, launch_options);
      // Record buffer filling operation
      const VulkanBufferImpl impl{std::addressof(device)};
      const std::size_t offset_bytes = launch_options.destOffsetInBytes();
      const std::size_t size_bytes = launch_options.sizeInBytes();
      impl.fillFastCmd(command, dst_data.buffer_, offset_bytes, size_bytes, data);
    }
  }
  LaunchResult result{};
  // Submit the recorded commands
  {
    constexpr VulkanDeviceCapability cap = VulkanDeviceCapability::kCompute;
    VkQueue q = device.getQueue(cap, launch_options.queueIndex());
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isFenceRequested() ? &device : nullptr);
    // Start labeling for debug until the end of the scope
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
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::fillImpl(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  LaunchResult result{};
  if (dest->isDeviceLocal() || !dest->isHostWritable()) {
    if (canFillFast(launch_options.destOffsetInBytes(), launch_options.sizeInBytes()))
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
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::fillOnDevice(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options)
{
  BufferData* dest_data = static_cast<BufferData*>(dest->rawBufferData());
  Buffer<uint8b>* fill_data = dest_data->fill_data_.get();
  Buffer<D>* dest_buffer = static_cast<Buffer<D>*>(dest);
  // Set the value into the fill data
  {
    constexpr std::size_t s = sizeof(typename Buffer<D>::Type);
    fill_data->setSize(s);
    auto mem = fill_data->mapMemory();
    std::memcpy(mem.data(), std::addressof(value), mem.size());
  }
  //
  VulkanDevice& device = *static_cast<VulkanDevice*>(dest->getParent());
  const VulkanBufferImpl impl{std::addressof(device)};
  LaunchResult result = impl.fill(dest_data->fill_kernel_.get(),
                                  fill_data,
                                  dest_buffer,
                                  launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> template <KernelArg D> inline
LaunchResult VulkanBuffer<T>::fillOnHost(
    typename Buffer<D>::ConstReference value,
    BufferCommon* dest,
    const BufferLaunchOptions<D>& launch_options) noexcept
{
  {
    using DataT = std::remove_cvref_t<D>;
    using DataPtr = std::add_pointer_t<DataT>;
    auto* dst_ptr = static_cast<DataPtr>(dest->mapMemoryData());
    dst_ptr = dst_ptr + launch_options.destOffset();
    std::fill_n(dst_ptr, launch_options.size(), value);
  }
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept
{
  const auto& mem_type = memoryType();
  const bool has_property = (mem_type.propertyFlags & flag) == flag;
  return has_property;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void VulkanBuffer<T>::initCommandBuffer()
{
  if (commandBuffer() == ZIVC_VK_NULL_HANDLE) {
    auto& device = parentImpl();
    rawBuffer().command_buffer_ = device.createCommandBuffer();
  }
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void VulkanBuffer<T>::initFillKernel()
{
  VulkanDevice& device = parentImpl();
  if (!rawBuffer().fill_kernel_ && isDeviceLocal()) {
    ZIVC_ASSERT(commandBuffer() != ZIVC_VK_NULL_HANDLE, "Command buffer is null.");
    VulkanBufferImpl impl{std::addressof(device)};
    rawBuffer().fill_kernel_ = impl.createFillKernel<T>(commandBuffer());
  }
  if (!rawBuffer().fill_data_ && isDeviceLocal()) {
    BufferInitParams params{BufferUsage::kPreferHost,
                            BufferFlag::kSequentialWritable};
    params.setInternalBufferFlag(true);
    rawBuffer().fill_data_ = device.createBuffer<uint8b>(params);
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool VulkanBuffer<T>::isInternal() const noexcept
{
  return is_internal_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
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

  \return No description
  */
template <KernelArg T> inline
VulkanDevice& VulkanBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *static_cast<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const VulkanDevice& VulkanBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *static_cast<const VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto VulkanBuffer<T>::rawBuffer() noexcept -> BufferData&
{
  return buffer_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto VulkanBuffer<T>::rawBuffer() const noexcept -> const BufferData&
{
  return buffer_data_;
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_INL_HPP
