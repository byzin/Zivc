/*!
  \file zivc.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_HPP
#define ZIVC_ZIVC_HPP

// Standard C++ library
#include <cstddef>
#include <string_view>
// Zivc
#include "../../core/zivc/buffer.hpp"
#include "../../core/zivc/context.hpp"
#include "../../core/zivc/context_options.hpp"
#include "../../core/zivc/device.hpp"
#include "../../core/zivc/kernel.hpp"
#include "../../core/zivc/kernel_set.hpp"
#include "../../core/zivc/cpu/cpu_buffer.hpp"
#include "../../core/zivc/cpu/cpu_device.hpp"
#include "../../core/zivc/cpu/cpu_kernel.hpp"
#include "../../core/zivc/auxiliary/buffer_init_params.hpp"
#include "../../core/zivc/auxiliary/error.hpp"
#include "../../core/zivc/auxiliary/kernel_init_params.hpp"
#if defined(ZIVC_ENABLE_VULKAN_BACKEND)
#include "../../core/zivc/vulkan/vulkan_buffer.hpp"
#include "../../core/zivc/vulkan/vulkan_device.hpp"
#include "../../core/zivc/vulkan/vulkan_kernel.hpp"
#endif // ZIVC_ENABLE_VULKAN_BACKEND
#include "../../core/zivc/zivc_cl.hpp"
#include "../../core/zivc/zivc_config.hpp"

namespace zivc {

//! Create a buffer
template <KernelArg Type>
[[nodiscard]]
SharedBuffer<Type> createBuffer(
    Device* device,
    const BufferInitParams& params);

//! Create a kernel
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
[[nodiscard]]
SharedKernel<kDim, KSet, Args...> createKernel(
    Device* device,
    const KernelInitParams<kDim, KSet, Args...>& params);

//! Create a kernel parameters
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
KernelInitParams<kDim, KSet, Args...> createKernelInitParams(
    void (*func)(Args...),
    std::string_view kernel_name) noexcept;

//! Create parameters for kernel initialization
#define ZIVC_CREATE_KERNEL_INIT_PARAMS(kernel_set_name, kernel_name, dimension)

} // namespace zivc

#include "../../core/zivc/zivc-inl.hpp"

#endif // ZIVC_ZIVC_HPP
