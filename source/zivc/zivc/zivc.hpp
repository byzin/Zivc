/*!
  \file zivc.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_HPP
#define ZIVC_ZIVC_HPP

// Standard C++ library
#include <cstddef>
#include <string_view>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "buffer.hpp"
#include "device.hpp"
#include "kernel.hpp"
#include "kernel_set.hpp"
#include "platform.hpp"
#include "platform_options.hpp"
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#include "utility/error.hpp"
#include "utility/kernel_init_params.hpp"
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_kernel.hpp"
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
#include "zivc/zivc_config.hpp"

namespace zivc {

//! Make a buffer
template <KernelArg Type>
[[nodiscard]]
SharedBuffer<Type> makeBuffer(Device* device, const BufferUsage flag);

//! Make a kernel
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
[[nodiscard]]
SharedKernel<kDim, KSet, Args...> makeKernel(
    Device* device,
    const KernelInitParams<kDim, KSet, Args...>& params);

//! Make a kernel parameters
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
KernelInitParams<kDim, KSet, Args...> makeKernelInitParams(
    const KernelSet<KSet>& kernel_set,
    void (*func)(Args...),
    std::string_view kernel_name) noexcept;

//! Make parameters for kernel initialization
#define ZIVC_MAKE_KERNEL_INIT_PARAMS(kernel_set_name, kernel_name, dimension)

} // namespace zivc

#include "zivc-inl.hpp"

#endif // ZIVC_ZIVC_HPP
