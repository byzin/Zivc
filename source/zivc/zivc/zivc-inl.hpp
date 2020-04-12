/*!
  \file zivc-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_INL_HPP
#define ZIVC_ZIVC_INL_HPP

#include "zivc.hpp"
// Standard C++ library
#include <cstddef>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "buffer.hpp"
#include "device.hpp"
#include "kernel.hpp"
#include "kernel_set.hpp"
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#include "utility/kernel_parameters.hpp"
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_kernel.hpp"
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
#include "utility/kernel_arg_parser.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in,out] device No description.
  \param [in] flag No description.
  \return No description
  */
template <typename Type> inline
SharedBuffer<Type> makeBuffer(Device* device, const BufferUsage flag)
{
  SharedBuffer<Type> buffer;
  switch (device->type()) {
   case SubPlatformType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    buffer = d->makeBuffer<Type>(flag);
    break;
   }
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
   case SubPlatformType::kVulkan: {
    auto d = zisc::cast<VulkanDevice*>(device);
    buffer = d->makeBuffer<Type>(flag);
    break;
   }
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return buffer;
}

/*!
  \details No detailed description

  \tparam kDimension No description.
  \tparam SetType No description.
  \tparam ArgTypes No description.
  \param [in] parameters No description.
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...ArgTypes>
SharedKernel<kDimension, SetType, ArgTypes...> makeKernel(
    Device* device,
    const KernelParameters<SetType, ArgTypes...>& parameters)
{
  SharedKernel<kDimension, SetType, ArgTypes...> kernel;
  switch (device->type()) {
   case SubPlatformType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    kernel = d->makeKernel<kDimension>(parameters);
    break;
   }
#if defined(ZIVC_ENABLE_VULKAN_SUB_PLATFORM)
   case SubPlatformType::kVulkan: {
    auto d = zisc::cast<VulkanDevice*>(device);
    kernel = d->makeKernel<kDimension>(parameters);
    break;
   }
#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return kernel;
}

///*!
//  */
//template <DescriptorType kDescriptor1, DescriptorType kDescriptor2, typename Type>
//inline
//void copy(const Buffer<kDescriptor1, Type>& src,
//          Buffer<kDescriptor2, Type>* dst,
//          const std::size_t count,
//          const std::size_t src_offset,
//          const std::size_t dst_offset,
//          const uint32b queue_index) noexcept
//{
//  ZISC_ASSERT(src.SubPlatformType() == dst->SubPlatformType(),
//              "The device types of src and dst aren't same.");
//  switch (src.SubPlatformType()) {
//   case SubPlatformType::kCpu: {
//    using SrcBuffer = CpuBuffer<kDescriptor1, Type>;
//    using DstBuffer = CpuBuffer<kDescriptor2, Type>;
//    const auto src_buffer = zisc::cast<const SrcBuffer*>(std::addressof(src));
//    auto dst_buffer = zisc::cast<DstBuffer*>(dst);
//    src_buffer->copyTo(dst_buffer, count, src_offset, dst_offset, queue_index);
//    break;
//   }
//#ifdef ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//   case SubPlatformType::kVulkan: {
//    using SrcBuffer = VulkanBuffer<kDescriptor1, Type>;
//    using DstBuffer = VulkanBuffer<kDescriptor2, Type>;
//    const auto src_buffer = zisc::cast<const SrcBuffer*>(std::addressof(src));
//    auto dst_buffer = zisc::cast<DstBuffer*>(dst);
//    src_buffer->copyTo(dst_buffer, count, src_offset, dst_offset, queue_index);
//    break;
//   }
//#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//   default: {
//    ZISC_ASSERT(false, "Error: Unsupported buffer type is specified.");
//    break;
//   }
//  }
//}

//namespace cppinner {
//
//template <std::size_t kDimension, typename Function, typename ...BufferArgs>
//struct KernelMaker;
//
//template <std::size_t kDimension, typename Function, typename ...BufferArgs>
//struct KernelMaker<kDimension, Function, Kernel<kDimension, BufferArgs...>>
//{
//  static auto makeKernel(CpuDevice* d, Function func) noexcept
//  {
//    auto kernel = d->makeKernel<kDimension, Function, BufferArgs...>(func);
//    return kernel;
//  }
//
//#ifdef ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//  static auto makeKernel(VulkanDevice* d,
//                  const uint32b module_index,
//                  const std::string_view kernel_name) noexcept
//  {
//    auto kernel = d->makeKernel<kDimension, Function, BufferArgs...>(module_index,
//                                                                     kernel_name);
//    return kernel;
//  }
//#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//};
//
//} // namespace cppinner
//
///*!
//  */
//template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
//inline
//UniqueKernelWithoutLocal<kDimension, ArgumentTypes...> makeKernel(
//    Device* device,
//    const KernelSet<SetType> /* kernel_set */,
//    void (*cpu_kernel)(ArgumentTypes...),
//    const std::string_view vulkan_kernel) noexcept
//{
//  static_assert(!KernelArgParser<kDimension, ArgumentTypes...>::hasConstLocal(),
//                "The kernel has any arguments that is const local qualified.");
//  using UniqueKernel = UniqueKernelWithoutLocal<kDimension, ArgumentTypes...>;
//  using Function = void (*)(ArgumentTypes...);
//  using KernelMaker = cppinner::KernelMaker<kDimension,
//                                            Function,
//                                            typename UniqueKernel::value_type>;
//
//  UniqueKernel kernel;
//  switch (device->SubPlatformType()) {
//   case SubPlatformType::kCpu: {
//    auto d = zisc::cast<CpuDevice*>(device);
//    kernel = KernelMaker::makeKernel(d, cpu_kernel);
//    break;
//   }
//#ifdef ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//   case SubPlatformType::kVulkan: {
//    auto d = zisc::cast<VulkanDevice*>(device);
//    constexpr uint32b module_index = SetType::getId();
//    if (!d->hasShaderModule(module_index)) {
//      const auto spirv_code = SetType::getKernelSpirvCode(d->workResource());
//      d->setShaderModule(spirv_code, module_index);
//    }
//    kernel = KernelMaker::makeKernel(d, module_index, vulkan_kernel);
//    break;
//   }
//#endif // ZIVC_ENABLE_VULKAN_SUB_PLATFORM
//   default: {
//    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
//    break;
//   }
//  }
//  return kernel;
//}
//
//#undef ZIVC_MAKE_KERNEL_ARGS
//#define ZIVC_MAKE_KERNEL_ARGS(kernel_set, kernel) \
//  zivc:: kernel_set :: __KernelSet{}, \
//  &zivc::cl:: kernel_set :: kernel , \
//  #kernel

/*!
  \details No detailed description

  \param [in] kernel_set No description.
  \param [in] func No description.
  \param [in] kernel_name No description.
  \return No description
  */
template <typename SetType, typename ...ArgTypes> inline
KernelParameters<SetType, ArgTypes...> makeKernelParameters(
    const KernelSet<SetType>& /* kernel_set */,
    void (*func)(ArgTypes...),
    std::string_view kernel_name) noexcept
{
  KernelParameters<SetType, ArgTypes...> parameters{func, kernel_name};
  return parameters;
}

#if defined(ZIVC_MAKE_KERNEL_PARAMETERS)
#undef ZIVC_MAKE_KERNEL_PARAMETERS
#endif // ZIVC_MAKE_KERNEL_PARAMETERS

/*!
  \def ZIVC_MAKE_KERNEL_PARAMETERS
  \brief No brief description

  No detailed description.

  \param [in] kernel_set_name No description.
  \param [in] kernel_name No description.
  \return No description
  */
#define ZIVC_MAKE_KERNEL_PARAMETERS(kernel_set_name, kernel_name) \
    ::zivc::makeKernelParameters( \
        ::zivc::kernel_set::KernelSet_ ## kernel_set_name {}, \
        ::zivc::cl:: kernel_set_name :: kernel_name, \
        #kernel_name )

} // namespace zivc

#endif // ZIVC_ZIVC_INL_HPP
