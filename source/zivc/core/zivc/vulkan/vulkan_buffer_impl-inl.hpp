/*!
  \file vulkan_buffer_impl-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_IMPL_INL_HPP
#define ZIVC_VULKAN_BUFFER_IMPL_INL_HPP

#include "vulkan_buffer_impl.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Zivc
#include "utility/vulkan.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_cl.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/buffer_launch_options.hpp"
#include "zivc/auxiliary/kernel_common.hpp"
#include "zivc/auxiliary/error.hpp"
#include "zivc/auxiliary/launch_result.hpp"
#include "zivc/auxiliary/launch_options.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] command_buffer No description.
  \return No description
  */
template <KernelArg Type> inline
std::shared_ptr<KernelCommon> VulkanBufferImpl::createFillKernel(
    const VkCommandBuffer& command_buffer)
{
  constexpr std::size_t data_size = sizeof(Type);
  constexpr FillUnitSize unit_size = getFillUnitSize(data_size);
  std::shared_ptr<KernelCommon> kernel;
  switch (unit_size) {
   case FillUnitSize::k8:
    kernel = createFillU8Kernel(command_buffer);
    break;
   case FillUnitSize::k16:
    kernel = createFillU16Kernel(command_buffer);
    break;
   case FillUnitSize::k32:
    kernel = createFillU32Kernel(command_buffer);
    break;
   case FillUnitSize::k64:
    kernel = createFillU64Kernel(command_buffer);
    break;
   case FillUnitSize::k128:
    kernel = createFillU128Kernel(command_buffer);
    break;
   default:
    ZIVC_ASSERT(false, "Unsupported fill size is specified: ", data_size);
    break;
  }
  return kernel;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] fill_kernel No description.
  \param [in] data_buffer No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg Type> inline
LaunchResult VulkanBufferImpl::fill(KernelCommon* fill_kernel,
                                    Buffer<uint8b>* data_buffer,
                                    Buffer<Type>* buffer,
                                    const BufferLaunchOptions<Type>& launch_options) const
{
  const std::size_t offset = launch_options.destOffset();
  const std::size_t size = launch_options.size();

  LaunchResult result{};
  constexpr std::size_t data_size = sizeof(Type);
  constexpr FillUnitSize unit_size = getFillUnitSize(data_size);
  switch (unit_size) {
   case FillUnitSize::k8: {
    auto buff = buffer->template reinterp<uint8b>();
    result = fillU8(fill_kernel, data_buffer, &buff, launch_options, offset, size);
    break;
   }
   case FillUnitSize::k16: {
    auto data_buff = data_buffer->reinterp<uint16b>();
    auto buff = buffer->template reinterp<uint16b>();
    result = fillU16(fill_kernel, &data_buff, &buff, launch_options, offset, size);
    break;
   }
   case FillUnitSize::k32: {
    auto data_buff = data_buffer->reinterp<uint32b>();
    auto buff = buffer->template reinterp<uint32b>();
    result = fillU32(fill_kernel, &data_buff, &buff, launch_options, offset, size);
    break;
   }
   case FillUnitSize::k64: {
    auto data_buff = data_buffer->reinterp<cl_uint2>();
    auto buff = buffer->template reinterp<cl_uint2>();
    result = fillU64(fill_kernel, &data_buff, &buff, launch_options, offset, size);
    break;
   }
   case FillUnitSize::k128: {
    auto data_buff = data_buffer->reinterp<cl_uint4>();
    auto buff = buffer->template reinterp<cl_uint4>();
    result = fillU128(fill_kernel, &data_buff, &buff, launch_options, offset, size);
    break;
   }
   default:
    ZIVC_ASSERT(false, "Unsupported fill size is specified: ", data_size);
    break;
  }
  return result;
}

/*!
  \details No detailed description

  \param [in] size No description.
  \return No description
  */
inline
constexpr auto VulkanBufferImpl::getFillUnitSize(const std::size_t size) noexcept
    -> FillUnitSize
{
  const FillUnitSize unit_size = 
      (size % static_cast<std::size_t>(FillUnitSize::k128) == 0) ? FillUnitSize::k128 :
      (size % static_cast<std::size_t>(FillUnitSize::k64) == 0)  ? FillUnitSize::k64 :
      (size % static_cast<std::size_t>(FillUnitSize::k32) == 0)  ? FillUnitSize::k32 :
      (size % static_cast<std::size_t>(FillUnitSize::k16) == 0)  ? FillUnitSize::k16
                                                                 : FillUnitSize::k8;
  return unit_size;
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_IMPL_INL_HPP
