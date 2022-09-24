/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/cpucl/address_space_pointer.hpp"
#include "zivc/utility/fence.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_arg_type_info.hpp"
#include "zivc/utility/kernel_launch_options.hpp"
#include "zivc/utility/kernel_init_params.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/type_pack.hpp"

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
  updateArgCache<0>(args...);
  // Command recording
  auto c = [this]() noexcept
  {
    runImpl<0, 0>();
//    constexpr std::size_t n = BaseKernelT::numOfArgs();
//    runImpl(std::make_index_sequence<n>{});
  };
  using CommandT = decltype(c);
  static_assert(sizeof(CommandStorage) == sizeof(CommandT));
  static_assert(std::alignment_of_v<CommandStorage> == std::alignment_of_v<CommandT>);
  auto* command_mem = zisc::cast<void*>(commandStorage());
  CommandT* command = ::new (command_mem) CommandT{c};
  // id counter
  auto atomic_mem = zisc::cast<void*>(atomicStorage());
  std::atomic<uint32b>* id = ::new (atomic_mem) std::atomic<uint32b>{0};

  LaunchResult result{};
  CpuDevice& device = parentImpl();
  // Command submission
  {
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isFenceRequested() ? &device : nullptr);
    constexpr uint32b dim = BaseKernelT::dimension();
    const auto work_size = BaseKernelT::expandWorkSize(launch_options.workSize(), 1);
    const auto id_offset = BaseKernelT::expandWorkSize(launch_options.globalIdOffset(), 0);
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

  \tparam Type No description.
  \tparam Types No description.
  \tparam ArgTypes No description.
  \param [in] cache No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <typename Type, typename ...Types, typename ...ArgTypes> inline
auto CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
makeArgCacheType(const KernelArgCache<ArgTypes...>& cache) noexcept
{
  constexpr std::size_t num_of_args = sizeof...(Types) + 1;
  auto precedence = concatArgCache<Type>(cache);
  if constexpr (1 < num_of_args) {
    return makeArgCacheType<Types...>(precedence);
  }
  else {
    return precedence;
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
  auto p = BaseKernelT::getParent();
  return *static_cast<CpuDevice*>(p);
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
  const auto p = BaseKernelT::getParent();
  return *zisc::cast<const CpuDevice*>(p);
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Types No description.
  \param [in] cl_args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, std::size_t kCacheIndex, typename ...Types> inline
void
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
runImpl(Types&&... cl_args) noexcept
{
  using ArgParserT = typename BaseKernelT::ArgParserT;
  if constexpr (kIndex < ArgParserT::kNumOfArgs) {
    using CacheT = typename ArgCache::template CacheT<kIndex>;
    using ArgT = std::remove_cv_t<std::remove_pointer_t<CacheT>>;
    using ArgTInfo = KernelArgTypeInfo<ArgT>;
    if constexpr (ArgTInfo::kIsLocal) { // Process a local argument
      using ElementT = typename ArgTInfo::ElementT;
      ElementT storage{};
      auto data = std::addressof(storage);
      cl::AddressSpacePointer<cl::AddressSpaceType::kLocal, ElementT> cl_arg{data};
      runImpl<kIndex + 1, kCacheIndex>(std::forward<Types>(cl_args)..., cl_arg);
    }
    else if constexpr (ArgTInfo::kIsPod) { // Process a pod argument
      auto cl_arg = arg_cache_.template get<kCacheIndex>();
      runImpl<kIndex + 1, kCacheIndex + 1>(std::forward<Types>(cl_args)..., cl_arg);
    }
    else { // Process a global argument
      using ElementT = typename ArgTInfo::ElementT;
      using Pointer = typename ArgT::Pointer;
      BufferCommon* cache = arg_cache_.template get<kCacheIndex>();
      auto data = static_cast<Pointer>(cache->rawBufferData());
      cl::AddressSpacePointer<cl::AddressSpaceType::kGlobal, ElementT> cl_arg{data};
      runImpl<kIndex + 1, kCacheIndex + 1>(std::forward<Types>(cl_args)..., cl_arg);
    }
  }
  else { // Launch the kernel
    FunctionT func = kernel();
    std::invoke(func, std::forward<Types>(cl_args)...);
  }
}

template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <typename I, I... indices> inline
void CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
runImpl([[maybe_unused]] const std::integer_sequence<I, indices...> index_seq) noexcept
{
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [in] value No description.
  \param [in] values No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, KernelArg Type, typename ...Types> inline
void
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateArgCache(const Type value, Types&&... values) noexcept
{
  constexpr std::size_t num_of_rest = sizeof...(Types);
  arg_cache_.template set<kIndex>(value);
  if constexpr (0 < num_of_rest)
    updateArgCache<kIndex + 1>(std::forward<Types>(values)...);
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam Type No description.
  \tparam Types No description.
  \param [in] value No description.
  \param [in] values No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
template <std::size_t kIndex, KernelArg Type, typename ...Types> inline
void
CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>::
updateArgCache(Buffer<Type>& value, Types&&... values) noexcept
{
  constexpr std::size_t num_of_rest = sizeof...(Types);
  arg_cache_.template set<kIndex>(std::addressof(value));
  if constexpr (0 < num_of_rest)
    updateArgCache<kIndex + 1>(std::forward<Types>(values)...);
}

} // namespace zivc

#endif // ZIVC_CPU_KERNEL_INL_HPP
