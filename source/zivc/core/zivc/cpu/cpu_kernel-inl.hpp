/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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
#include <utility>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "../kernel.hpp"
#include "../kernel_set.hpp"
#include "../zivc_config.hpp"
#include "../cpucl/address_space_pointer.hpp"
#include "../auxiliary/fence.hpp"
#include "../auxiliary/id_data.hpp"
#include "../auxiliary/kernel_launch_options.hpp"
#include "../auxiliary/kernel_init_params.hpp"
#include "../auxiliary/launch_result.hpp"
#include "../internal/kernel_arg_cache.hpp"
#include "../internal/kernel_arg_info.hpp"
#include "../internal/kernel_arg_type_info.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
CpuKernel(IdData&& id) noexcept : BaseKernelT(std::move(id))
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
kernel() const noexcept -> FunctionT
{
  return kernel_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
LaunchResult CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
run(Args... args, const LaunchOptionsT& launch_options)
{
  updateArgCache(args...);
  Command* command = createCommand(); // Command recording
  std::atomic<uint32b>* id = createIdCounter();

  LaunchResult result{};
  CpuDevice& device = parentImpl();
  // Command submission
  {
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isFenceRequested() ? &device : nullptr);
    using BaseT = BaseKernelT;
    constexpr uint32b dim = BaseT::dimension();
    const std::array work_size = BaseT::expandWorkSize(launch_options.workSize(), 1);
    const std::array id_offset = BaseT::expandWorkSize(launch_options.globalIdOffset(), 0);
    device.submit(*command, dim, work_size, id_offset, id, std::addressof(fence));
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
initData(const ParamsT& params)
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

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::CacheTypeCreator::
createArg() noexcept
{
  using EmptyT = internal::KernelArgCache<void>;
  internal::KernelArgCache cache = (EmptyT{} + ... + internal::KernelArgCache<Args>{});
  static_assert(decltype(cache)::isValid(), "The kernel doesn't have parameter.");
  return cache;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::CacheTypeCreator::
createLocal() noexcept
{
  using ArgParserT = typename BaseKernelT::ArgParserT;
  constexpr std::size_t n = ArgParserT::kNumOfLocalArgs;
  if constexpr (n == 0) {
    return internal::KernelArgCache<void>{};
  }
  else {
    internal::KernelArgCache cache = createLocalImpl(std::make_index_sequence<n>());
    using CacheT = decltype(cache);
    static_assert(std::is_trivial_v<CacheT>, "The locals aren't trivial.");
    static_assert(std::equality_comparable<CacheT>, "The locals aren't equal comparable.");
    return cache;
  }
}

/*!
  \details No detailed description

  \tparam kIndices No description.
  \param [in] indices No description.
  \return No description

  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t ...kIndices> inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::CacheTypeCreator::
createLocalImpl([[maybe_unused]] std::index_sequence<kIndices...> indices) noexcept
{
  using EmptyT = internal::KernelArgCache<void>;
  internal::KernelArgCache cache = (EmptyT{} + ... + getLocal<kIndices>());
  return cache;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex> inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::CacheTypeCreator::
getLocal() noexcept
{
  using ArgParserT = typename BaseKernelT::ArgParserT;
  constexpr internal::KernelArgInfo info = ArgParserT::getLocalArgInfoList()[kIndex];
  using ArgumentT = typename BaseKernelT::template ArgT<info.index()>;
  using ArgTypeInfoT = internal::KernelArgTypeInfo<ArgumentT>;
  static_assert(ArgTypeInfoT::kIsLocal, "The Type isn't local addressed.");
  // Create local variable type
  constexpr std::size_t n = CpuDeviceInfo::maxWorkGroupSize();
  using LocalT = std::array<typename ArgTypeInfoT::ElementT, n>;
  return internal::KernelArgCache<LocalT>{};
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::Command::
operator()() noexcept
{
  using ArgParserT = typename BaseKernelT::ArgParserT;
  kernel_->runImpl(std::make_index_sequence<ArgParserT::kNumOfArgs>{});
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
atomicStorage() noexcept -> AtomicStorageT*
{
  return std::addressof(atomic_storage_);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
commandStorage() noexcept -> CommandStorageT*
{
  return std::addressof(command_storage_);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
createCommand() noexcept -> Command*
{
  // Command recording
  // To guarantee that the command is alive while kernel execution,
  // create the command using the member storage
  static_assert(sizeof(CommandStorageT) == sizeof(Command));
  static_assert(std::alignment_of_v<CommandStorageT> == std::alignment_of_v<Command>);
  auto* memory = static_cast<void*>(commandStorage());
  auto* command = ::new (memory) Command{{this}};

  return command;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::atomic<uint32b>* CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
createIdCounter() noexcept
{
  // To guarantee that the counter is alive while kernel execution,
  // create the counter using the member storage
  auto* memory = zisc::cast<void*>(atomicStorage());
  auto* id = ::new (memory) std::atomic<uint32b>{0};
  return id;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] local_cache No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex> inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
getArg(LocalCacheT& local_cache) noexcept -> ArgT<kIndex>
{
  using ArgumentT = ArgT<kIndex>;
  using ArgParserT = typename BaseKernelT::ArgParserT;
  constexpr internal::KernelArgInfo info = ArgParserT::getArgInfoList()[kIndex];
  constexpr std::size_t offset = info.localOffset();
  if constexpr (info.isLocal()) {
    constexpr std::size_t cache_index = offset;
    auto* data = local_cache.template get<cache_index>().data();
    ArgumentT cl_arg{data};
    return cl_arg;
  }
  else if constexpr (info.isPod()) {
    constexpr std::size_t cache_index = kIndex - offset;
    ArgumentT cl_arg = arg_cache_.template get<cache_index>();
    return cl_arg;
  }
  else { // global
    constexpr std::size_t cache_index = kIndex - offset;
    BufferCommon* cache = arg_cache_.template get<cache_index>();
    auto* data = static_cast<typename ArgumentT::Pointer>(cache->rawBufferData());
    ArgumentT cl_arg{data};
    return cl_arg;
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
CpuDevice& CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() noexcept
{
  ZivcObject* p = BaseKernelT::getParent();
  return *static_cast<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const CpuDevice& CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
parentImpl() const noexcept
{
  const ZivcObject* p = BaseKernelT::getParent();
  return *static_cast<const CpuDevice*>(p);
}

/*!
  \details No detailed description

  \tparam kIndices No description.
  \param [in] indices No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t... kIndices> inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
runImpl([[maybe_unused]] std::index_sequence<kIndices...> indices) noexcept
{
  FunctionT func = kernel();
  LocalCacheT local_cache{};
  std::invoke(func, getArg<kIndices>(local_cache)...);
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \param [in] value No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type> inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setArgCache(Buffer<Type>& value) noexcept
{
  arg_cache_.template set<kIndex>(std::addressof(value));
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \param [in] value No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, typename Type> inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
setArgCache(const Type value) noexcept
{
  arg_cache_.template set<kIndex>(value);
}

/*!
  \details No detailed description

  \param [in] args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateArgCache(Args... args) noexcept
{
  constexpr std::size_t n = sizeof...(Args);
  updateArgCacheImpl(args..., std::make_index_sequence<n>());
}

/*!
  \details No detailed description

  \param [in] args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t... kIndices> inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateArgCacheImpl(Args... args,
                   [[maybe_unused]] std::index_sequence<kIndices...> indices) noexcept
{
  (..., setArgCache<kIndices>(args));
}

} // namespace zivc

#endif // ZIVC_CPU_KERNEL_INL_HPP
