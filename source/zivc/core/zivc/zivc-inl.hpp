/*!
  \file zivc-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_INL_HPP
#define ZIVC_ZIVC_INL_HPP

#include "zivc.hpp"
// Standard C++ library
#include <concepts>
#include <cstddef>
#include <string_view>
#include <utility>
// Zivc
#include "buffer.hpp"
#include "device.hpp"
#include "kernel.hpp"
#include "kernel_set.hpp"
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#include "utility/buffer_init_params.hpp"
#include "utility/buffer_launch_options.hpp"
#include "utility/error.hpp"
#include "utility/kernel_arg_parser.hpp"
#include "utility/kernel_init_params.hpp"
#include "utility/launch_result.hpp"
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_kernel.hpp"
#endif // ZIVC_ENABLE_VULKAN_BACKEND
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam SrcType No description.
  \tparam DstType No description.
  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg SrcType, std::same_as<std::remove_const_t<SrcType>> DstType> inline
LaunchResult copy(const Buffer<SrcType>& source,
                  Buffer<DstType>* dest,
                  const BufferLaunchOptions<DstType>& launch_options)
{
  LaunchResult result;
  switch (dest->type()) {
   case BackendType::kCpu: {
    result = dest->template copyFromDerived<CpuBuffer>(source, launch_options);
    break;
   }
   case BackendType::kVulkan: {
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
    result = dest->template copyFromDerived<VulkanBuffer>(source, launch_options);
    break;
#else // ZIVC_ENABLE_VULKAN_BACKEND
    [[fallthrough]];
#endif // ZIVC_ENABLE_VULKAN_BACKEND
   }
   default: {
    ZIVC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] value No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg Type> inline
LaunchResult fill(typename Buffer<Type>::ConstReference value,
                  Buffer<Type>* buffer,
                  const BufferLaunchOptions<Type>& launch_options)
{
  LaunchResult result;
  switch (buffer->type()) {
   case BackendType::kCpu: {
    result = buffer->template fillDerived<CpuBuffer>(value, launch_options);
    break;
   }
   case BackendType::kVulkan: {
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
    result = buffer->template fillDerived<VulkanBuffer>(value, launch_options);
    break;
#else // ZIVC_ENABLE_VULKAN_BACKEND
    [[fallthrough]];
#endif // ZIVC_ENABLE_VULKAN_BACKEND
   }
   default: {
    ZIVC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in,out] device No description.
  \param [in] params No description.
  \return No description
  */
template <KernelArg Type> inline
SharedBuffer<Type> createBuffer(Device* device, const BufferInitParams& params)
{
  SharedBuffer<Type> buffer;
  switch (device->type()) {
   case BackendType::kCpu: {
    buffer = device->createDerivedBuffer<CpuBuffer, Type>(params);
    break;
   }
   case BackendType::kVulkan: {
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
    buffer = device->createDerivedBuffer<VulkanBuffer, Type>(params);
    break;
#else // ZIVC_ENABLE_VULKAN_BACKEND
    [[fallthrough]];
#endif // ZIVC_ENABLE_VULKAN_BACKEND
   }
   default: {
    ZIVC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return buffer;
}

/*!
  \details No detailed description

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  \param [in,out] device No description.
  \param [in] params No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
SharedKernel<kDim, KSet, Args...> createKernel(
    Device* device,
    const KernelInitParams<kDim, KSet, Args...>& params)
{
  SharedKernel<kDim, KSet, Args...> kernel;
  switch (device->type()) {
   case BackendType::kCpu: {
    kernel = device->createDerivedKernel<CpuKernel>(params);
    break;
   }
   case BackendType::kVulkan: {
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
    kernel = device->createDerivedKernel<VulkanKernel>(params);
    break;
#else // ZIVC_ENABLE_VULKAN_BACKEND
    [[fallthrough]];
#endif // ZIVC_ENABLE_VULKAN_BACKEND
   }
   default: {
    ZIVC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return kernel;
}

/*!
  \details No detailed description

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  \param [in] func No description.
  \param [in] kernel_name No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
KernelInitParams<kDim, KSet, Args...> createKernelInitParams(
    void (*func)(Args...),
    std::string_view kernel_name) noexcept
{
  KernelInitParams<kDim, KSet, Args...> params{func, kernel_name};
  return params;
}

#if defined(ZIVC_CREATE_KERNEL_INIT_PARAMS)
#undef ZIVC_CREATE_KERNEL_INIT_PARAMS
#endif // ZIVC_CREATE_KERNEL_INIT_PARAMS

/*!
  \def ZIVC_CREATE_KERNEL_INIT_PARAMS
  \brief No brief description

  No detailed description.

  \param [in] kernel_set_name No description.
  \param [in] kernel_name No description.
  \param [in] dimension No description.
  \return No description
  */
#define ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_set_name, kernel_name, dimension) \
    ::zivc::createKernelInitParams< \
        dimension, \
        ::zivc::kernel_set::KernelSet_ ## kernel_set_name >( \
            ::zivc::cl:: kernel_name, \
            #kernel_name )

} // namespace zivc

#endif // ZIVC_ZIVC_INL_HPP
