/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_INL_HPP
#define ZIVC_VULKAN_BUFFER_INL_HPP

#include "vulkan_buffer.hpp"
// Standard C++ library
#include <algorithm>
#include <memory>
#include <utility>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "vulkan_device_info.hpp"
#include "utility/cmd_debug_label_region.hpp"
#include "utility/cmd_record_region.hpp"
#include "utility/queue_debug_label_region.hpp"
#include "zivc/buffer.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/mapped_memory.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <typename T> inline
VulkanBuffer<T>::VulkanBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
VulkanBuffer<T>::~VulkanBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VmaAllocation& VulkanBuffer<T>::allocation() noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VmaAllocation& VulkanBuffer<T>::allocation() const noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return vm_alloc_info_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VkBuffer& VulkanBuffer<T>::buffer() noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VkBuffer& VulkanBuffer<T>::buffer() const noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto VulkanBuffer<T>::descriptorType() const noexcept -> DescriptorType
{
  return desc_type_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
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
template <typename T> inline
bool VulkanBuffer<T>::isDeviceLocal() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool VulkanBuffer<T>::isHostCached() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool VulkanBuffer<T>::isHostCoherent() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool VulkanBuffer<T>::isHostVisible() const noexcept
{
  const bool result = hasMemoryProperty(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  return result;
}

/*!
  \details No detailed description

  \param [in] type No description.
  */
template <typename T> inline
void VulkanBuffer<T>::setDescriptorType(const DescriptorType type) noexcept
{
  desc_type_ = type;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
void VulkanBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_size = size();
  if (s != prev_size) {
    Buffer<T>::clear();
    if (0 < s) {
      const std::size_t mem_size = sizeof(Type) * s;
      auto& device = parentImpl();
      device.allocateMemory(mem_size,
                            Buffer<T>::usage(),
                            descriptorTypeVk(),
                            std::addressof(Buffer<T>::id()),
                            std::addressof(buffer()),
                            std::addressof(allocation()),
                            std::addressof(vm_alloc_info_));
      ZivcObject::updateDebugInfo();
    }
    initCommandBuffer();
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t VulkanBuffer<T>::size() const noexcept
{
  const auto& info = allocationInfo();
  const std::size_t s = info.size / sizeof(Type);
  return s;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult VulkanBuffer<T>::copyFromImpl(
    const Buffer<T>& source,
    const BufferLaunchOptions<T>& launch_options)
{
  LaunchResult result{};
  if (isDeviceLocal() || source.isDeviceLocal())
    result = copyOnDevice(source, launch_options);
  else
    result = copyOnHost(source, launch_options);
  return result;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::destroyData() noexcept
{
  if (buffer_ != VK_NULL_HANDLE) {
    auto& device = parentImpl();
    device.deallocateMemory(std::addressof(buffer()),
                            std::addressof(allocation()),
                            std::addressof(vm_alloc_info_));
    initData();
  }
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult VulkanBuffer<T>::fillImpl(ConstReference value,
                                       const BufferLaunchOptions<T>& launch_options)
{
  LaunchResult result{};
  if (isDeviceLocal()) {
    const std::size_t offsetBytes = launch_options.destOffsetInBytes();
    const std::size_t sizeBytes = launch_options.sizeInBytes();
    if ((4 % sizeof(T) == 0) && (offsetBytes % 4 == 0) && (sizeBytes % 4 == 0))
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
  */
template <typename T> inline
void VulkanBuffer<T>::initData()
{
  setDescriptorType(DescriptorType::kStorage);
  buffer_ = VK_NULL_HANDLE;
  vm_allocation_ = VK_NULL_HANDLE;
  vm_alloc_info_.memoryType = 0;
  vm_alloc_info_.deviceMemory = VK_NULL_HANDLE;
  vm_alloc_info_.offset = 0;
  vm_alloc_info_.size = 0;
  vm_alloc_info_.pMappedData = nullptr;
  vm_alloc_info_.pUserData = nullptr;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto VulkanBuffer<T>::mappedMemory() const -> Pointer
{
  Pointer p = nullptr;
  const auto& device = parentImpl();
  const auto result = vmaMapMemory(device.memoryAllocator(), allocation(), &p);
  if (result != VK_SUCCESS) {
    //! \todo Raise an exception
    printf("[Warning] Memory mapping failed.\n");
  }
  return p;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::unmapMemory() const noexcept
{
  const auto& device = parentImpl();
  vmaUnmapMemory(device.memoryAllocator(), allocation());
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::updateDebugInfoImpl() noexcept
{
  auto& device = parentImpl();
  const IdData& id_data = ZivcObject::id();
  {
    const VkBuffer& handle = buffer();
    if (handle != VK_NULL_HANDLE) {
      device.setDebugInfo(VK_OBJECT_TYPE_BUFFER,
                          *zisc::treatAs<const uint64b*>(std::addressof(handle)),
                          id_data.name(),
                          this);
    }
  }
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult VulkanBuffer<T>::copyOnDevice(
    const Buffer<T>& source,
    const BufferLaunchOptions<T>& launch_options)
{
  VulkanDevice& device = parentImpl();
  {
    CmdDebugLabelRegion debug_region{Buffer<T>::isDebugMode() ? command_buffer_
                                                              : VK_NULL_HANDLE,
                                     device.dispatcher(),
                                     launch_options.label(),
                                     launch_options.labelColor()};
    {
      CmdRecordRegion record_region{command_buffer_,
                                    device.dispatcher(),
                                    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
      auto src = zisc::cast<const VulkanBuffer*>(std::addressof(source));
      const VkBufferCopy copy_region{launch_options.sourceOffsetInBytes(),
                                     launch_options.destOffsetInBytes(),
                                     launch_options.sizeInBytes()};
      device.copyBufferCmd(command_buffer_, src->buffer(), buffer(), copy_region);
    }
  }
  LaunchResult result{};
  {
    VkQueue q = device.getQueue(launch_options.queueIndex());
    if (launch_options.isExternalSyncMode())
      device.takeFence(std::addressof(result.fence()));
    QueueDebugLabelRegion debug_region{Buffer<T>::isDebugMode() ? q
                                                                : VK_NULL_HANDLE,
                                       device.dispatcher(),
                                       launch_options.label(),
                                       launch_options.labelColor()};
    device.submitKernelQueue(command_buffer_, q, result.fence());
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
template <typename T> inline
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
template <typename T> inline
LaunchResult VulkanBuffer<T>::fillFastOnDevice(
    ConstReference value,
    const BufferLaunchOptions<T>& launch_options)
{
  // Create a data for fill
  uint32b data = 0;
  constexpr std::size_t n = sizeof(data) / sizeof(T);
  std::fill_n(zisc::treatAs<Pointer>(std::addressof(data)), n, value);

  VulkanDevice& device = parentImpl();
  {
    CmdDebugLabelRegion debug_region{Buffer<T>::isDebugMode() ? command_buffer_
                                                              : VK_NULL_HANDLE,
                                     device.dispatcher(),
                                     launch_options.label(),
                                     launch_options.labelColor()};
    {
      CmdRecordRegion record_region{command_buffer_,
                                    device.dispatcher(),
                                    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
      device.fillBufferFastCmd(command_buffer_,
                               buffer(),
                               launch_options.destOffsetInBytes(),
                               launch_options.sizeInBytes(),
                               data);
    }
  }
  LaunchResult result{};
  {
    VkQueue q = device.getQueue(launch_options.queueIndex());
    if (launch_options.isExternalSyncMode())
      device.takeFence(std::addressof(result.fence()));
    QueueDebugLabelRegion debug_region{Buffer<T>::isDebugMode() ? q
                                                                : VK_NULL_HANDLE,
                                       device.dispatcher(),
                                       launch_options.label(),
                                       launch_options.labelColor()};
    device.submitKernelQueue(command_buffer_, q, result.fence());
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
template <typename T> inline
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
template <typename T> inline
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
template <typename T> inline
bool VulkanBuffer<T>::hasMemoryProperty(const VkMemoryPropertyFlagBits flag) const noexcept
{
  const auto& device = parentImpl();
  const auto& device_info = device.deviceInfoData();
  const auto& info = allocationInfo();

  const auto& mem_props = device_info.memoryProperties().properties1_;
  const auto& mem_type = mem_props.memoryTypes[info.memoryType];
  const bool has_property = (mem_type.propertyFlags & flag) == flag;
  return has_property;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::initCommandBuffer()
{
  if ((command_buffer_ == VK_NULL_HANDLE) && isDeviceLocal()) {
    auto& device = parentImpl();
    device.initKernelCommandBuffer(std::addressof(command_buffer_));
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VulkanDevice& VulkanBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *zisc::treatAs<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VulkanDevice& VulkanBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::treatAs<const VulkanDevice*>(p);
}

// Device

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> VulkanDevice::makeBuffer(const BufferUsage flag)
{
  using BufferType = VulkanBuffer<T>;
  zisc::pmr::polymorphic_allocator<BufferType> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferType>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakBuffer<T> own{buffer};
  buffer->initialize(std::move(parent), std::move(own), flag);

  return buffer;
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_INL_HPP

///*!
//  \file vulkan_buffer-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZIVC_VULKAN_BUFFER_INL_HPP
//#define ZIVC_VULKAN_BUFFER_INL_HPP
//
//#include "vulkan_buffer.hpp"
//// Standard C++ library
//#include <cstddef>
//// Vulkan
//#include <vulkan/vulkan.hpp>
//#include "vk_mem_alloc.h"
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/utility.hpp"
//// Zivc
//#include "vulkan_device.hpp"
//#include "zivc/buffer.hpp"
//#include "zivc/zivc_config.hpp"
//
//namespace zivc {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::VulkanBuffer(const VulkanDevice* device,
//                                           const BufferUsage usage_flag) noexcept :
//    Buffer<kDescriptor, T>(usage_flag),
//    device_{device}
//{
//  initialize();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::VulkanBuffer(const VulkanDevice* device,
//                                           const BufferUsage usage_flag,
//                                           const std::size_t size) noexcept :
//    VulkanBuffer(device, usage_flag)
//{
//  setSize(size);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::~VulkanBuffer() noexcept
//{
//  destroy();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VmaAllocationInfo& VulkanBuffer<kDescriptor, T>::allocationInfo() noexcept
//{
//  return alloc_info_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const VmaAllocationInfo& VulkanBuffer<kDescriptor, T>::allocationInfo() const noexcept
//{
//  return alloc_info_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//vk::Buffer& VulkanBuffer<kDescriptor, T>::buffer() noexcept
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const vk::Buffer& VulkanBuffer<kDescriptor, T>::buffer() const noexcept
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> template <DescriptorType kDstDescriptor>
//inline
//void VulkanBuffer<kDescriptor, T>::copyTo(VulkanBuffer<kDstDescriptor, T>* dst,
//                                          const std::size_t count,
//                                          const std::size_t src_offset,
//                                          const std::size_t dst_offset,
//                                          const uint32b queue_index) const noexcept
//{
//  const std::size_t s = sizeof(Type) * count;
//  ZISC_ASSERT(s <= dst->memoryUsage(), "The size of dst buffer memory is less.");
//  const std::size_t src_offset_size = sizeof(Type) * src_offset;
//  const std::size_t dst_offset_size = sizeof(Type) * dst_offset;
//  const vk::BufferCopy copy_info{src_offset_size, dst_offset_size, s};
//
//  vk::CommandBufferBeginInfo begin_info{};
//  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
//  copy_command_.begin(begin_info);
//
//  copy_command_.copyBuffer(buffer(), dst->buffer(), 1, &copy_info);
//
//  copy_command_.end();
//  device_->submit(QueueType::kTransfer, queue_index, copy_command_);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::destroy() noexcept
//{
//  if (buffer_) {
//    auto d = const_cast<VulkanDevice*>(device_);
//    d->deallocate(this);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//SubPlatformType VulkanBuffer<kDescriptor, T>::SubPlatformType() const noexcept
//{
//  return SubPlatformType::kVulkan;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isDeviceMemory() const noexcept
//{
//  const auto& info = device_->physicalDeviceInfo();
//  const auto& memory_property = info.memoryProperties().properties1_;
//  const uint32b index = allocationInfo().memoryType;
//  const auto flag = memory_property.memoryTypes[index].propertyFlags;
//  const bool result = (flag & vk::MemoryPropertyFlagBits::eDeviceLocal) ==
//                      vk::MemoryPropertyFlagBits::eDeviceLocal;
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isHostMemory() const noexcept
//{
//  const bool result = !isDeviceMemory();
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isHostVisible() const noexcept
//{
//  const auto& info = device_->physicalDeviceInfo();
//  const auto& memory_property = info.memoryProperties().properties1_;
//  const uint32b index = allocationInfo().memoryType;
//  const auto flag = memory_property.memoryTypes[index].propertyFlags;
//  const bool result = (flag & vk::MemoryPropertyFlagBits::eHostVisible) ==
//                      vk::MemoryPropertyFlagBits::eHostVisible;
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VmaAllocation& VulkanBuffer<kDescriptor, T>::memory() noexcept
//{
//  return memory_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const VmaAllocation& VulkanBuffer<kDescriptor, T>::memory() const noexcept
//{
//  return memory_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t VulkanBuffer<kDescriptor, T>::memoryUsage() const noexcept
//{
//  const auto memory_usage = zisc::cast<std::size_t>(alloc_info_.size);
//  return memory_usage;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::read(Pointer data,
//                                        const std::size_t count,
//                                        const std::size_t offset,
//                                        const uint32b queue_index) const noexcept
//{
//  if (isHostVisible()) {
//    auto src = this->mapMemory();
//    const std::size_t s = sizeof(Type) * count;
//    std::memcpy(data, src.data() + offset, s);
//  }
//  else {
//    VulkanBuffer<kDescriptor, Type> dst{device_, BufferUsage::kHostOnly};
//    dst.setSize(count);
//    copyTo(&dst, count, offset, 0, queue_index);
//    device_->waitForCompletion(QueueType::kTransfer, queue_index);
//    dst.read(data, count, 0, queue_index);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::setSize(const std::size_t size) noexcept
//{
//  destroy();
//  size_ = size;
//  auto d = const_cast<VulkanDevice*>(device_);
//  d->allocate(size, this);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t VulkanBuffer<kDescriptor, T>::size() const noexcept
//{
//  return size_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::write(ConstPointer data,
//                                         const std::size_t count,
//                                         const std::size_t offset,
//                                         const uint32b queue_index) noexcept
//{
//  if (isHostVisible()) {
//    auto dst = this->mapMemory();
//    const std::size_t s = sizeof(Type) * count;
//    std::memcpy(dst.data() + offset, data, s);
//  }
//  else {
//    VulkanBuffer<kDescriptor, Type> src{device_, BufferUsage::kHostOnly};
//    src.setSize(count);
//    src.write(data, count, 0, queue_index);
//    src.copyTo(this, count, 0, offset, queue_index);
//    device_->waitForCompletion(QueueType::kTransfer, queue_index);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::initialize() noexcept
//{
//  ZISC_ASSERT(device_ != nullptr, "The device is null.");
//  alloc_info_.size = 0;
//  // Initialize a copy command
//  const vk::CommandBufferAllocateInfo alloc_info{
//      device_->commandPool(QueueType::kTransfer),
//      vk::CommandBufferLevel::ePrimary,
//      1};
//  auto [r, copy_commands] = device_->device().allocateCommandBuffers(alloc_info);
//  ZISC_ASSERT(r == vk::Result::eSuccess, "Command buffer allocation failed.");
//  copy_command_ = copy_commands[0];
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto VulkanBuffer<kDescriptor, T>::mappedMemory() const noexcept -> Pointer
//{
//  void* d = nullptr;
//  const auto result = vmaMapMemory(device_->memoryAllocator(), memory_, &d);
//  ZISC_ASSERT(result == VK_SUCCESS, "Buffer memory map failed.");
//  (void)result;
//  return zisc::cast<Pointer>(d);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::unmapMemory() const noexcept
//{
//  vmaUnmapMemory(device_->memoryAllocator(), memory_);
//}
//
//} // namespace zivc
//
//#endif // ZIVC_VULKAN_BUFFER_INL_HPP
