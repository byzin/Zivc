/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_KERNEL_INL_HPP
#define ZIVC_CPU_KERNEL_INL_HPP

#include "cpu_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zivc/kernel.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/kernel_parameters.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
CpuKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
~CpuKernel() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
auto
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
kernel() const noexcept -> Function
{
  return kernel_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
LaunchResult
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
run(ArgTypes... args, const LaunchOptions& launch_options)
{
  CpuDevice& device = parentImpl();
  // Command recording
  auto command = [func = kernel(), &args..., &launch_options]() noexcept
  {
    using LauncherType = Launcher<FuncArgTypes...>;
    LauncherType::exec(func, launch_options, args...);
  };
  LaunchResult result{};
  // Command submission
  {
    if (launch_options.isExternalSyncMode())
      device.takeFence(std::addressof(result.fence()));
    const auto work_size = BaseKernel::expandWorkSize(launch_options.workSize());
    device.submit(command, work_size, std::addressof(result.fence()));
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
destroyData() noexcept
{
  kernel_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
initData(const Parameters& params)
{
  kernel_ = params.func();
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
updateDebugInfoImpl() noexcept
{
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Types No description.
  \param [in] func No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
template <typename ...UnprocessedArgs> template <typename Type, typename ...Types>
inline
void
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
Launcher<UnprocessedArgs...>::exec(Function func,
                                   const LaunchOptions& launch_options,
                                   Type&& value,
                                   Types&&... rest) noexcept
{
  using ArgTuple = std::tuple<UnprocessedArgs...>;
  constexpr std::size_t is_ready_to_run = std::tuple_size_v<ArgTuple> == 0;
  if constexpr (is_ready_to_run) { // Invoke the underlying kernel
    std::invoke(func, std::forward<Type>(value), std::forward<Types>(rest)...);
  }
  else { // Process arguments
    using ArgT = std::remove_volatile_t<typename std::tuple_element_t<0, ArgTuple>>;
    using ArgInfo = KernelArgInfo<ArgT>;
    using NextLauncher = typename LauncherHelper<UnprocessedArgs...>::NextLauncher;
    if constexpr (ArgInfo::kIsLocal) { // Process a local argument
      typename ArgInfo::ElementType storage{};
      ArgT cl_arg{std::addressof(storage)};
      NextLauncher::exec(func,
                         launch_options,
                         std::forward<Type>(value),
                         std::forward<Types>(rest)...,
                         cl_arg);
    }
    else if constexpr (ArgInfo::kIsPod) {
      NextLauncher::exec(func,
                         launch_options,
                         std::forward<Types>(rest)...,
                         std::forward<Type>(value));
    }
    else { // Process a global argument
      using BufferT = std::remove_reference_t<Type>;
      using CpuBufferT = CpuBuffer<typename BufferT::Type>;
      using CpuBufferPtr = std::add_pointer_t<CpuBufferT>;
      auto buffer = zisc::cast<CpuBufferPtr>(std::addressof(value));
      ArgT cl_arg{buffer->data()};
      NextLauncher::exec(func,
                         launch_options,
                         std::forward<Types>(rest)...,
                         cl_arg);
    }
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuDevice&
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::treatAs<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
inline
const CpuDevice&
CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>::
parentImpl() const noexcept
{
  const auto p = BaseKernel::getParent();
  return *zisc::treatAs<const CpuDevice*>(p);
}

/*!
  \details No detailed description

  \tparam kDimension No description.
  \tparam SetType No description.
  \tparam ArgTypes No description.
  \param [in] parameters No description.
  \return No description
  */
template <std::size_t kDimension, typename SetType, typename ...ArgTypes> inline
SharedKernel<kDimension, SetType, ArgTypes...> CpuDevice::makeKernel(
    const KernelParameters<SetType, ArgTypes...>& parameters) noexcept
{
  using SharedKernelType = SharedKernel<kDimension, SetType, ArgTypes...>;
  using KernelType = typename KernelArgParser<ArgTypes...>::
                         template CpuKernelType<kDimension, SetType>;
  zisc::pmr::polymorphic_allocator<KernelType> alloc{memoryResource()};
  SharedKernelType kernel = std::allocate_shared<KernelType>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakKernel<kDimension, SetType, ArgTypes...> own{kernel};
  kernel->initialize(std::move(parent), std::move(own), parameters);

  return kernel;
}

} // namespace zivc

#endif // ZIVC_CPU_KERNEL_INL_HPP
