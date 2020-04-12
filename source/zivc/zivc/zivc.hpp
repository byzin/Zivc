/*!
  \file zivc.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_HPP
#define ZIVC_ZIVC_HPP

// Standard C++ library
#include <cstddef>
#include <string_view>
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
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_kernel.hpp"
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
#include "utility/kernel_arg_parser.hpp"
#include "zivc/zivc_config.hpp"
//#include "zivc/cppcl/types.hpp"
//#include "zivc/cppcl/vector.hpp"

namespace zivc {

// Forward declaration
template <typename SetType> class KernelSet;

//! Make a buffer
template <typename Type>
SharedBuffer<Type> makeBuffer(Device* device, const BufferUsage flag);

//! Make a kernel
template <std::size_t kDimension, typename SetType, typename ...ArgTypes>
SharedKernel<kDimension, SetType, ArgTypes...> makeKernel(
    Device* device,
    const KernelParameters<SetType, ArgTypes...>& parameters);

////! Copy a src buffer to a dst buffer
//template <DescriptorType kDescriptor1, DescriptorType kDescriptor2, typename Type>
//void copy(const Buffer<kDescriptor1, Type>& src,
//          Buffer<kDescriptor2, Type>* dst,
//          const std::size_t count,
//          const std::size_t src_offset,
//          const std::size_t dst_offset,
//          const uint32b queue_index) noexcept;

//template <std::size_t kDimension, typename ...ArgumentTypes>
//using UniqueKernelWithoutLocal = zisc::UniqueMemoryPointer<
//    typename KernelArgParser<kDimension, ArgumentTypes...>::KernelWithoutLocal>;
//
////! Make a kernel
//template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
//UniqueKernelWithoutLocal<kDimension, ArgumentTypes...>
//makeKernel(Device* device,
//           const KernelSet<SetType> kernel_set,
//           void (*cpu_kernel)(ArgumentTypes...),
//           const std::string_view vulkan_kernel) noexcept;

//! Create arguments of makeKernel
//#define ZIVC_MAKE_KERNEL_ARGS(kernel_set, kernel)

//! Make a kernel parameters
template <typename SetType, typename ...ArgTypes>
KernelParameters<SetType, ArgTypes...> makeKernelParameters(
    const KernelSet<SetType>& kernel_set,
    void (*func)(ArgTypes...),
    std::string_view kernel_name) noexcept;

//! Make a kernel parameters
#define ZIVC_MAKE_KERNEL_PARAMETERS(kernel_set_name, kernel_name)

} // namespace zivc

#include "zivc-inl.hpp"

#endif // ZIVC_ZIVC_HPP
