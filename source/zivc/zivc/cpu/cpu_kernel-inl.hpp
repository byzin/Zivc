/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_KERNEL_INL_HPP
#define ZIVC_CPU_KERNEL_INL_HPP

#include "cpu_kernel.hpp"
// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/kernel_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_parser.hpp"
#include "zivc/utility/kernel_init_params.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
CpuKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
~CpuKernel() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
kernel() const noexcept -> Function
{
  return kernel_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
LaunchResult CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
run(Args... args, const LaunchOptions& launch_options)
{
  CpuDevice& device = parentImpl();
  // Command recording
  auto c = [func = kernel(), &args..., &launch_options]() noexcept
  {
    using LauncherType = Launcher<FuncArgs...>;
    LauncherType::exec(func, launch_options, args...);
  };
  using CommandT = decltype(c);
  static_assert(sizeof(CommandStorage) == sizeof(CommandT));
  static_assert(std::alignment_of_v<CommandStorage> == std::alignment_of_v<CommandT>);
  auto command_mem = zisc::cast<void*>(commandStorage());
  CommandT* command = ::new (command_mem) CommandT{c};
  // id counter
  auto atomic_mem = zisc::cast<void*>(atomicStorage());
  std::atomic<uint32b>* id = ::new (atomic_mem) std::atomic<uint32b>{0};

  LaunchResult result{};
  // Command submission
  {
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isExternalSyncMode() ? &device : nullptr);
    constexpr uint32b dim = BaseKernel::dimension();
    const auto work_size = BaseKernel::expandWorkSize(launch_options.workSize(), 1);
    const auto global_offset =
        BaseKernel::expandWorkSize(launch_options.globalIdOffset(), 0);
    device.submit(*command, dim, work_size, global_offset, id, std::addressof(fence));
  }
  result.setAsync(true);
  return result;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
destroyData() noexcept
{
  kernel_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
initData(const Params& params)
{
  kernel_ = params.func();
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateDebugInfoImpl()
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
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <typename ...UnprocessedArgs> template <typename Type, typename ...Types>
inline
void
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
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
      using ValueP = typename BufferT::Pointer;
      auto data = zisc::cast<ValueP>(value.rawBufferData());
      ArgT cl_arg{data};
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
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
atomicStorage() noexcept -> AtomicStorage*
{
  return std::addressof(atomic_storage_);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
commandStorage() noexcept -> CommandStorage*
{
  return std::addressof(command_storage_);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
CpuDevice&
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::cast<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const CpuDevice&
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() const noexcept
{
  const auto p = BaseKernel::getParent();
  return *zisc::cast<const CpuDevice*>(p);
}

} // namespace zivc

#endif // ZIVC_CPU_KERNEL_INL_HPP
