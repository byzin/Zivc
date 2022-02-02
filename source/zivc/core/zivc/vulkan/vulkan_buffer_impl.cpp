/*!
  \file vulkan_buffer_impl.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_buffer_impl.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "vulkan_device.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "utility/vulkan_hpp.hpp"
#include "utility/vulkan_memory_allocator.hpp"
#include "zivc/zivc.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/kernel_set/kernel_set-zivc_internal_kernel.hpp"
#include "zivc/utility/error.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/launch_options.hpp"

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
auto makeFillKernelImpl(zivc::VulkanDevice* device,
                        const VkCommandBuffer& command_buffer,
                        Params& params)
{
  params.setVulkanCommandBufferPtr(std::addressof(command_buffer));
  auto kernel = device->makeKernel(params);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto makeFillU8KernelImpl(zivc::VulkanDevice* device,
                          const VkCommandBuffer& command_buffer)
{
  auto p = ZIVC_MAKE_KERNEL_INIT_PARAMS(zivc_internal_kernel, Zivc_fillU8Kernel, 1);
  auto kernel = makeFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto makeFillU16KernelImpl(zivc::VulkanDevice* device,
                           const VkCommandBuffer& command_buffer)
{
  auto p = ZIVC_MAKE_KERNEL_INIT_PARAMS(zivc_internal_kernel, Zivc_fillU16Kernel, 1);
  auto kernel = makeFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto makeFillU32KernelImpl(zivc::VulkanDevice* device,
                           const VkCommandBuffer& command_buffer)
{
  auto p = ZIVC_MAKE_KERNEL_INIT_PARAMS(zivc_internal_kernel, Zivc_fillU32Kernel, 1);
  auto kernel = makeFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto makeFillU64KernelImpl(zivc::VulkanDevice* device,
                           const VkCommandBuffer& command_buffer)
{
  auto p = ZIVC_MAKE_KERNEL_INIT_PARAMS(zivc_internal_kernel, Zivc_fillU64Kernel, 1);
  auto kernel = makeFillKernelImpl(device, command_buffer, p);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] command_buffer No description.
  \return No description
  */
[[nodiscard]]
auto makeFillU128KernelImpl(zivc::VulkanDevice* device,
                            const VkCommandBuffer& command_buffer)
{
  auto p = ZIVC_MAKE_KERNEL_INIT_PARAMS(zivc_internal_kernel, Zivc_fillU128Kernel, 1);
  auto kernel = makeFillKernelImpl(device, command_buffer, p);
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
  \param [in] buffer_usage No description.
  \param [in] desc_type No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::allocateMemory(const std::size_t size,
                                      const BufferUsage buffer_usage,
                                      const VkBufferUsageFlagBits desc_type,
                                      void* user_data,
                                      VkBuffer* buffer,
                                      VmaAllocation* vm_allocation,
                                      VmaAllocationInfo* alloc_info) const
{
  uint32b index_list_size = 0;
  const auto family_index_list = device().getQueueFamilyIndexList(&index_list_size);
  VkBufferCreateInfo binfo = makeBufferCreateInfo(size, desc_type);
  binfo.queueFamilyIndexCount = index_list_size;
  binfo.pQueueFamilyIndices = family_index_list.data();

  const auto alloc_create_info = makeAllocCreateInfo(buffer_usage, user_data);
  const auto result = vmaCreateBuffer(device().memoryAllocator(),
                                      std::addressof(binfo),
                                      std::addressof(alloc_create_info),
                                      buffer,
                                      vm_allocation,
                                      alloc_info);
  if (result != VK_SUCCESS) {
    const char* message = "Device memory allocation failed.";
    throwResultException(result, message);
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
  const zivcvk::CommandBuffer command{command_buffer};
  ZIVC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer source{source_buffer};
  ZIVC_ASSERT(source, "The given source buffer is null.");
  const zivcvk::Buffer dest{dest_buffer};
  ZIVC_ASSERT(dest, "The given dest buffer is null.");
  const zivcvk::BufferCopy copy_region{region};
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
  if (zivcvk::Buffer{*buffer}) {
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
  \tparam KernelType No description.
  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \param [in] offset No description.
  \param [in] size No description.
  \return No description
  */
template <typename Type, typename KernelType>
LaunchResult VulkanBufferImpl::fillImpl(KernelCommon* fill_kernel,
                                        Buffer<Type>* data_buffer,
                                        Buffer<Type>* buffer,
                                        const LaunchOptions& launch_options,
                                        const std::size_t offset,
                                        const std::size_t size) const
{
  using FillKernelP = std::add_pointer_t<KernelType>;
  using FillInfoT = zivc::cl::zivc::FillInfo;
  auto* kernel = zisc::cast<FillKernelP>(fill_kernel);

  auto kernel_launch_options = kernel->makeOptions();
  const std::size_t work_size = (size + FillInfoT::batchSize() - 1) /
                                FillInfoT::batchSize();

  constexpr std::size_t type_size = sizeof(Type);
  const std::size_t adjustment = (type_size <= 2) ? data_buffer->size() : 1;

  kernel_launch_options.setWorkSize({zisc::cast<uint32b>(work_size * adjustment)});
  kernel_launch_options.setQueueIndex(launch_options.queueIndex());
  kernel_launch_options.requestFence(launch_options.isFenceRequested());
  kernel_launch_options.setLabel(launch_options.label());
  kernel_launch_options.setLabelColor(launch_options.labelColor());

  FillInfoT info{};
  info.setElementOffset(offset * adjustment);
  info.setElementSize(size * adjustment);
  info.setDataSize(data_buffer->size());

  auto result = kernel->run(*data_buffer, *buffer, info, kernel_launch_options);
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
  using FillKernelT =
      std::remove_cvref_t<decltype(*::makeFillU8KernelImpl(nullptr, nullptr))>;
  auto result = fillImpl<uint8b, FillKernelT>(fill_kernel, data_buffer, buffer,
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
  using FillKernelT =
      std::remove_cvref_t<decltype(*::makeFillU16KernelImpl(nullptr, nullptr))>;
  auto result = fillImpl<uint16b, FillKernelT>(fill_kernel, data_buffer, buffer,
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
  using FillKernelT =
      std::remove_cvref_t<decltype(*::makeFillU32KernelImpl(nullptr, nullptr))>;
  auto result = fillImpl<uint32b, FillKernelT>(fill_kernel, data_buffer, buffer,
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
  using FillKernelT =
      std::remove_cvref_t<decltype(*::makeFillU64KernelImpl(nullptr, nullptr))>;
  auto result = fillImpl<cl_uint2, FillKernelT>(fill_kernel, data_buffer, buffer,
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
  using FillKernelT =
      std::remove_cvref_t<decltype(*::makeFillU128KernelImpl(nullptr, nullptr))>;
  auto result = fillImpl<cl_uint4, FillKernelT>(fill_kernel, data_buffer, buffer,
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
  const zivcvk::CommandBuffer command{command_buffer};
  ZIVC_ASSERT(command, "The given command buffer is null.");
  const zivcvk::Buffer buf{buffer};
  ZIVC_ASSERT(buf, "The given buffer is null.");
  command.fillBuffer(buf, dest_offset, size, data, device().dispatcher().loader());
}

/*!
  \details No detailed description

  \param [in] buffer_usage No description.
  \param [in] desc_type No description.
  \param [out] alloc_info No description.
  */
void VulkanBufferImpl::initAllocationInfo(const BufferUsage buffer_usage,
                                          const VkBufferUsageFlagBits desc_type,
                                          VmaAllocationInfo* alloc_info) const
{
  // Clear 
  {
    VkBuffer buffer = ZIVC_VK_NULL_HANDLE;
    deallocateMemory(std::addressof(buffer), nullptr, alloc_info);
  }

  VkBufferCreateInfo binfo = makeBufferCreateInfo(1, desc_type);
  uint32b index_list_size = 0;
  const auto family_index_list = device().getQueueFamilyIndexList(&index_list_size);
  binfo.queueFamilyIndexCount = index_list_size;
  binfo.pQueueFamilyIndices = family_index_list.data();

  const auto alloc_create_info = makeAllocCreateInfo(buffer_usage, nullptr);
  const auto result = vmaFindMemoryTypeIndexForBufferInfo(
      device().memoryAllocator(),
      std::addressof(binfo),
      std::addressof(alloc_create_info),
      std::addressof(alloc_info->memoryType));
  if (result != VK_SUCCESS) {
    const char* message = "Device memory allocation failed.";
    throwResultException(result, message);
  }
}

/*!
  \details No detailed description

  \param [in] result No description.
  \param [in] message No description.
  */
void VulkanBufferImpl::throwResultException(const VkResult result, const char* message)
{
  const auto r = zisc::cast<zivcvk::Result>(result);
  zivcvk::throwResultException(r, message);
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

  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \return No description

  */
VmaAllocationCreateInfo VulkanBufferImpl::makeAllocCreateInfo(
    const BufferUsage buffer_usage,
    void* user_data) noexcept
{
  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = VMA_ALLOCATION_CREATE_STRATEGY_MIN_FRAGMENTATION_BIT;
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
VkBufferCreateInfo VulkanBufferImpl::makeBufferCreateInfo(
    const std::size_t size,
    const VkBufferUsageFlagBits desc_type) noexcept
{
  // Buffer create info
  zivcvk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  const auto descriptor_type = zisc::cast<zivcvk::BufferUsageFlagBits>(desc_type);
  buffer_create_info.usage = zivcvk::BufferUsageFlagBits::eTransferSrc |
                             zivcvk::BufferUsageFlagBits::eTransferDst |
                             descriptor_type;
  buffer_create_info.sharingMode = zivcvk::SharingMode::eExclusive;
  buffer_create_info.queueFamilyIndexCount = 0;
  buffer_create_info.pQueueFamilyIndices = nullptr;

  return zisc::cast<VkBufferCreateInfo>(buffer_create_info);
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
std::shared_ptr<KernelCommon> VulkanBufferImpl::makeFillU8Kernel(
    const VkCommandBuffer& command_buffer)
{
  auto kernel = ::makeFillU8KernelImpl(std::addressof(device()), command_buffer);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
std::shared_ptr<KernelCommon> VulkanBufferImpl::makeFillU16Kernel(
    const VkCommandBuffer& command_buffer)
{
  auto kernel = ::makeFillU16KernelImpl(std::addressof(device()), command_buffer);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
std::shared_ptr<KernelCommon> VulkanBufferImpl::makeFillU32Kernel(
    const VkCommandBuffer& command_buffer)
{
  auto kernel = ::makeFillU32KernelImpl(std::addressof(device()), command_buffer);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
std::shared_ptr<KernelCommon> VulkanBufferImpl::makeFillU64Kernel(
    const VkCommandBuffer& command_buffer)
{
  auto kernel = ::makeFillU64KernelImpl(std::addressof(device()), command_buffer);
  return kernel;
}

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \return No description
  */
std::shared_ptr<KernelCommon> VulkanBufferImpl::makeFillU128Kernel(
    const VkCommandBuffer& command_buffer)
{
  auto kernel = ::makeFillU128KernelImpl(std::addressof(device()), command_buffer);
  return kernel;
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
   case BufferUsage::kDeviceOnly: {
    mem_usage = VMA_MEMORY_USAGE_GPU_ONLY;
    break;
   }
   case BufferUsage::kHostOnly: {
    mem_usage = VMA_MEMORY_USAGE_CPU_ONLY;
    break;
   }
   case BufferUsage::kHostToDevice: {
    mem_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    break;
   }
   case BufferUsage::kDeviceToHost: {
    mem_usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
    break;
   }
   default:
    break;
  }
  return mem_usage;
}

} // namespace zivc
