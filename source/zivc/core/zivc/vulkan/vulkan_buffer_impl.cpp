/*!
  \file vulkan_buffer_impl.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_buffer_impl.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "internal/vulkan_memory_allocator.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "zivc/kernel.hpp"
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/kernel_set/kernel_set-zivc_internal_kernel.hpp"
#include "zivc/auxiliary/buffer_common.hpp"
#include "zivc/auxiliary/error.hpp"
#include "zivc/auxiliary/kernel_common.hpp"
#include "zivc/auxiliary/kernel_init_params.hpp"
#include "zivc/auxiliary/launch_result.hpp"
#include "zivc/auxiliary/launch_options.hpp"

namespace {

/*!
  \details No detailed description

  \tparam Params No description.
  \param [in] device No description.
  \param [in] command_buffer No description.
  \param [in] params No description.
  \return No description
  */
template <typename Params>
[[nodiscard]]
auto createFillKernelImpl(zivc::VulkanDevice* device,
                          const VkCommandBuffer& command_buffer,
                          Params& params)
{
  params.setVulkanCommandBufferPtr(std::addressof(command_buffer));
  const zivc::SharedKernel kernel = device->createKernel(params);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto createFillU8KernelImpl(zivc::VulkanDevice* device,
                            const VkCommandBuffer& command_buffer)
{
  zivc::KernelInitParams p = ZIVC_CREATE_KERNEL_INIT_PARAMS(zivc_internal_kernel,
                                                            Zivc_fillU8Kernel,
                                                            1);
  const zivc::SharedKernel kernel = createFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto createFillU16KernelImpl(zivc::VulkanDevice* device,
                             const VkCommandBuffer& command_buffer)
{
  zivc::KernelInitParams p = ZIVC_CREATE_KERNEL_INIT_PARAMS(zivc_internal_kernel,
                                                            Zivc_fillU16Kernel,
                                                            1);
  const zivc::SharedKernel kernel = createFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto createFillU32KernelImpl(zivc::VulkanDevice* device,
                             const VkCommandBuffer& command_buffer)
{
  zivc::KernelInitParams p = ZIVC_CREATE_KERNEL_INIT_PARAMS(zivc_internal_kernel,
                                                            Zivc_fillU32Kernel,
                                                            1);
  const zivc::SharedKernel kernel = createFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto createFillU64KernelImpl(zivc::VulkanDevice* device,
                             const VkCommandBuffer& command_buffer)
{
  zivc::KernelInitParams p = ZIVC_CREATE_KERNEL_INIT_PARAMS(zivc_internal_kernel,
                                                            Zivc_fillU64Kernel,
                                                            1);
  const zivc::SharedKernel kernel = createFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto createFillU128KernelImpl(zivc::VulkanDevice* device,
                              const VkCommandBuffer& command_buffer)
{
  zivc::KernelInitParams p = ZIVC_CREATE_KERNEL_INIT_PARAMS(zivc_internal_kernel,
                                                            Zivc_fillU128Kernel,
                                                            1);
  const zivc::SharedKernel kernel = createFillKernelImpl(device, command_buffer, p);
  return kernel;
}

} // namespace

namespace zivc {

/*!
  \details No detailed description

  \param [in] device No description.
  */
VulkanBufferImpl::VulkanBufferImpl(VulkanDevice* device) noexcept : device_{device}
{
}

/*!
  \details No detailed description
  */
VulkanBufferImpl::~VulkanBufferImpl() noexcept
{
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] object No description.
  \param [in] desc_type No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::allocateMemory(const std::size_t size,
                                      const BufferCommon& object,
                                      const VkBufferUsageFlagBits desc_type,
                                      void* user_data,
                                      VkBuffer* buffer,
                                      VmaAllocation* vm_allocation,
                                      VmaAllocationInfo* alloc_info) const
{
  uint32b index_list_size = 0;
  const std::array family_index_list = device().getQueueFamilyIndexList(&index_list_size);
  VkBufferCreateInfo binfo = createBufferCreateInfo(size, desc_type);
  binfo.queueFamilyIndexCount = index_list_size;
  binfo.pQueueFamilyIndices = family_index_list.data();

  const VmaAllocationCreateInfo alloc_create_info = createAllocCreateInfo(object.usage(),
                                                                          object.flag(),
                                                                          user_data);
  const VkResult result = vmaCreateBuffer(device().memoryAllocator(),
                                          std::addressof(binfo),
                                          std::addressof(alloc_create_info),
                                          buffer,
                                          vm_allocation,
                                          alloc_info);
  if (result != VK_SUCCESS) [[unlikely]] {
    std::string message = "Buffer memroy allocation failed. size: " +
                          std::to_string(size) + " bytes.";
    message = createErrorMessage(object, message);
    throw SystemError{ErrorCode::kBufferMemoryAllocationFailed, message};
  }
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] source_buffer No description.
  \param [in] dest_buffer No description.
  \param [in] region No description.
  */
void VulkanBufferImpl::copyCmd(const VkCommandBuffer& command_buffer,
                               const VkBuffer& source_buffer,
                               const VkBuffer& dest_buffer,
                               const VkBufferCopy& region) const
{
  const vk::CommandBuffer command{command_buffer};
  ZIVC_ASSERT(command, "The given command buffer is null.");
  const vk::Buffer source{source_buffer};
  ZIVC_ASSERT(source, "The given source buffer is null.");
  const vk::Buffer dest{dest_buffer};
  ZIVC_ASSERT(dest, "The given dest buffer is null.");
  const vk::BufferCopy copy_region{region};
  command.copyBuffer(source, dest, copy_region, device().dispatcher().loader());
}

/*!
  \details No detailed description

  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [in,out] alloc_info No description.
  */
void VulkanBufferImpl::deallocateMemory(
    VkBuffer* buffer,
    VmaAllocation* vm_allocation,
    VmaAllocationInfo* alloc_info) const noexcept
{
  if (vk::Buffer{*buffer} && (vm_allocation != nullptr)) {
    vmaDestroyBuffer(device().memoryAllocator(), *buffer, *vm_allocation);
  }
  alloc_info->deviceMemory = ZIVC_VK_NULL_HANDLE;
  alloc_info->offset = 0;
  alloc_info->size = 0;
  alloc_info->pMappedData = nullptr;
  alloc_info->pUserData = nullptr;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam KType No description.
  \tparam KTypes No description.
  \param [in] kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
template <typename Type, typename KType, typename ...KTypes>
LaunchResult VulkanBufferImpl::fillImpl(Kernel<KType, KTypes...>* kernel,
                                        Buffer<Type>* data_buffer,
                                        Buffer<Type>* buffer,
                                        const LaunchOptions& launch_options,
                                        const std::size_t offset,
                                        const std::size_t size) const
{
  using FillInfoT = zivc::cl::zivc::FillInfo;

  constexpr std::size_t type_size = sizeof(Type);
  const std::size_t adjustment = (type_size <= 2) ? data_buffer->size() : 1;

  KernelLaunchOptions kernel_launch_options = kernel->createOptions();
  const std::size_t work_size = (size + FillInfoT::batchSize() - 1) /
                                FillInfoT::batchSize();
  kernel_launch_options.setWorkSize({{zisc::cast<uint32b>(work_size * adjustment)}});
  kernel_launch_options.setQueueIndex(launch_options.queueIndex());
  kernel_launch_options.requestFence(launch_options.isFenceRequested());
  kernel_launch_options.setLabel(launch_options.label());
  kernel_launch_options.setLabelColor(launch_options.labelColor());

  FillInfoT info{};
  info.setElementOffset(offset * adjustment);
  info.setElementSize(size * adjustment);
  info.setDataSize(data_buffer->size());

  LaunchResult result = kernel->run(*data_buffer, *buffer, info, kernel_launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
LaunchResult VulkanBufferImpl::fillU8(KernelCommon* fill_kernel,
                                      Buffer<uint8b>* data_buffer,
                                      Buffer<uint8b>* buffer,
                                      const LaunchOptions& launch_options,
                                      const std::size_t offset,
                                      const std::size_t size) const
{
  using KernelT = decltype(::createFillU8KernelImpl(nullptr, nullptr))::ElementT;
  auto* kernel = static_cast<KernelT*>(fill_kernel);
  LaunchResult result = fillImpl<uint8b>(kernel, data_buffer, buffer,
                                         launch_options, offset, size);
  return result;
}

/*!
  \details No detailed description

  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
LaunchResult VulkanBufferImpl::fillU16(KernelCommon* fill_kernel,
                                       Buffer<uint16b>* data_buffer,
                                       Buffer<uint16b>* buffer,
                                       const LaunchOptions& launch_options,
                                       const std::size_t offset,
                                       const std::size_t size) const
{
  using KernelT = decltype(::createFillU16KernelImpl(nullptr, nullptr))::ElementT;
  auto* kernel = static_cast<KernelT*>(fill_kernel);
  LaunchResult result = fillImpl<uint16b>(kernel, data_buffer, buffer,
                                          launch_options, offset, size);
  return result;
}

/*!
  \details No detailed description

  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
LaunchResult VulkanBufferImpl::fillU32(KernelCommon* fill_kernel,
                                       Buffer<uint32b>* data_buffer,
                                       Buffer<uint32b>* buffer,
                                       const LaunchOptions& launch_options,
                                       const std::size_t offset,
                                       const std::size_t size) const
{
  using KernelT = decltype(::createFillU32KernelImpl(nullptr, nullptr))::ElementT;
  auto* kernel = static_cast<KernelT*>(fill_kernel);
  LaunchResult result = fillImpl<uint32b>(kernel, data_buffer, buffer,
                                          launch_options, offset, size);
  return result;
}

/*!
  \details No detailed description

  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
LaunchResult VulkanBufferImpl::fillU64(KernelCommon* fill_kernel,
                                       Buffer<cl_uint2>* data_buffer,
                                       Buffer<cl_uint2>* buffer,
                                       const LaunchOptions& launch_options,
                                       const std::size_t offset,
                                       const std::size_t size) const
{
  using KernelT = decltype(::createFillU64KernelImpl(nullptr, nullptr))::ElementT;
  auto* kernel = static_cast<KernelT*>(fill_kernel);
  LaunchResult result = fillImpl<cl_uint2>(kernel, data_buffer, buffer,
                                           launch_options, offset, size);
  return result;
}

/*!
  \details No detailed description

  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
LaunchResult VulkanBufferImpl::fillU128(KernelCommon* fill_kernel,
                                       Buffer<cl_uint4>* data_buffer,
                                       Buffer<cl_uint4>* buffer,
                                       const LaunchOptions& launch_options,
                                       const std::size_t offset,
                                       const std::size_t size) const
{
  using KernelT = decltype(::createFillU128KernelImpl(nullptr, nullptr))::ElementT;
  auto* kernel = static_cast<KernelT*>(fill_kernel);
  LaunchResult result = fillImpl<cl_uint4>(kernel, data_buffer, buffer,
                                           launch_options, offset, size);
  return result;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] buffer No description.
  \param [in] dest_offset No description.
  \param [in] size No description.
  \param [in] data No description.
  */
void VulkanBufferImpl::fillFastCmd(const VkCommandBuffer& command_buffer,
                                   const VkBuffer& buffer,
                                   const std::size_t dest_offset,
                                   const std::size_t size,
                                   const uint32b data) const noexcept
{
  const vk::CommandBuffer command{command_buffer};
  ZIVC_ASSERT(command, "The given command buffer is null.");
  const vk::Buffer buf{buffer};
  ZIVC_ASSERT(buf, "The given buffer is null.");
  command.fillBuffer(buf, dest_offset, size, data, device().dispatcher().loader());
}

/*!
  \details No detailed description

  \param [in] object No description.
  \param [in] desc_type No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::initAllocationInfo(const BufferCommon& object,
                                          const VkBufferUsageFlagBits desc_type,
                                          VmaAllocationInfo* alloc_info) const
{
  // Clear 
  {
    VkBuffer buffer = ZIVC_VK_NULL_HANDLE;
    deallocateMemory(std::addressof(buffer), nullptr, alloc_info);
  }

  VkBufferCreateInfo binfo = createBufferCreateInfo(1, desc_type);
  uint32b index_list_size = 0;
  const std::array family_index_list = device().getQueueFamilyIndexList(&index_list_size);
  binfo.queueFamilyIndexCount = index_list_size;
  binfo.pQueueFamilyIndices = family_index_list.data();

  const VmaAllocationCreateInfo alloc_create_info = createAllocCreateInfo(object.usage(),
                                                                          object.flag(),
                                                                          nullptr);
  const VkResult result = vmaFindMemoryTypeIndexForBufferInfo(
      device().memoryAllocator(),
      std::addressof(binfo),
      std::addressof(alloc_create_info),
      std::addressof(alloc_info->memoryType));
  if (result != VK_SUCCESS) {
    const std::string message = createErrorMessage(
        object,
        "Device memory allocation failed.");
    throwResultException(result, message.data());
  }
}

/*!
  \details No detailed description

  \param [in] result No description.
  \param [in] message No description.
  */
void VulkanBufferImpl::throwResultException(const VkResult result, const char* message)
{
  const auto r = static_cast<vk::Result>(result);
  vk::throwResultException(r, message);
}

/*!
  \details No detailed description

  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \return No description

  */
VmaAllocationCreateInfo VulkanBufferImpl::createAllocCreateInfo(
    const BufferUsage buffer_usage,
    const BufferFlag buffer_flag,
    void* user_data) noexcept
{
  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT | toVmaFlag(buffer_flag);
  alloc_create_info.usage = toVmaUsage(buffer_usage);
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = ZIVC_VK_NULL_HANDLE;
  alloc_create_info.pUserData = user_data;
  alloc_create_info.priority = 0.0f;

  return alloc_create_info;
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] desc_type No description.
  \return No description
  */
VkBufferCreateInfo VulkanBufferImpl::createBufferCreateInfo(
    const std::size_t size,
    const VkBufferUsageFlagBits desc_type) noexcept
{
  // Buffer create info
  vk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  const auto descriptor_type = static_cast<vk::BufferUsageFlagBits>(desc_type);
  buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferSrc |
                             vk::BufferUsageFlagBits::eTransferDst |
                             descriptor_type;
  buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
  buffer_create_info.queueFamilyIndexCount = 0;
  buffer_create_info.pQueueFamilyIndices = nullptr;

  return zisc::cast<VkBufferCreateInfo>(buffer_create_info);
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
SharedKernelCommon VulkanBufferImpl::createFillU8Kernel(
    const VkCommandBuffer& command_buffer)
{
  zivc::SharedKernel kernel = ::createFillU8KernelImpl(std::addressof(device()),
                                                       command_buffer);
  return std::move(kernel.data());
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
SharedKernelCommon VulkanBufferImpl::createFillU16Kernel(
    const VkCommandBuffer& command_buffer)
{
  zivc::SharedKernel kernel = ::createFillU16KernelImpl(std::addressof(device()),
                                                        command_buffer);
  return std::move(kernel.data());
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
SharedKernelCommon VulkanBufferImpl::createFillU32Kernel(
    const VkCommandBuffer& command_buffer)
{
  zivc::SharedKernel kernel = ::createFillU32KernelImpl(std::addressof(device()),
                                                        command_buffer);
  return std::move(kernel.data());
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
SharedKernelCommon VulkanBufferImpl::createFillU64Kernel(
    const VkCommandBuffer& command_buffer)
{
  zivc::SharedKernel kernel = ::createFillU64KernelImpl(std::addressof(device()),
                                                        command_buffer);
  return std::move(kernel.data());
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
SharedKernelCommon VulkanBufferImpl::createFillU128Kernel(
    const VkCommandBuffer& command_buffer)
{
  zivc::SharedKernel kernel = ::createFillU128KernelImpl(std::addressof(device()),
                                                         command_buffer);
  return std::move(kernel.data());
}

/*!
  \details No detailed description

  \return No description
  */
inline
VulkanDevice& VulkanBufferImpl::device() noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDevice& VulkanBufferImpl::device() const noexcept
{
  return *device_;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  \return No description
  */
inline
constexpr VmaAllocationCreateFlags VulkanBufferImpl::toVmaFlag(const BufferFlag flag) noexcept
{
  VmaAllocationCreateFlags buf_flag = VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT;
  switch (flag) {
   case BufferFlag::kSequentialWritable: {
    buf_flag |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    break;
   }
   case BufferFlag::kRandomAccessible: {
    buf_flag |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    break;
   }
   default:
    break;
  }
  return buf_flag;
}

/*!
  \details No detailed description

  \param [in] usage No description.
  \return No description
  */
inline
constexpr VmaMemoryUsage VulkanBufferImpl::toVmaUsage(const BufferUsage usage) noexcept
{
  VmaMemoryUsage mem_usage = VMA_MEMORY_USAGE_UNKNOWN;
  switch (usage) {
   case BufferUsage::kAuto: {
    mem_usage = VMA_MEMORY_USAGE_AUTO;
    break;
   }
   case BufferUsage::kPreferDevice: {
    mem_usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    break;
   }
   case BufferUsage::kPreferHost: {
    mem_usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    break;
   }
   default:
    break;
  }
  return mem_usage;
}

} // namespace zivc
