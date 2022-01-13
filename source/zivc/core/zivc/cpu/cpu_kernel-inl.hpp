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

  \param [in] kernel No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename CKernel, typename Type> inline
LaunchResult CpuKernelHelper::run(CKernel* kernel, const Type& launch_options)
{
  using KernelT = std::remove_cvref_t<CKernel>;
  // Command recording
  auto c = [kernel]() noexcept
  {
    kernel->template runImpl<0, 0>();
  };
  using CommandT = decltype(c);
  using CommandStorage = typename KernelT::CommandStorage;
  static_assert(sizeof(CommandStorage) == sizeof(CommandT));
  static_assert(std::alignment_of_v<CommandStorage> == std::alignment_of_v<CommandT>);
  auto command_mem = zisc::cast<void*>(kernel->commandStorage());
  CommandT* command = ::new (command_mem) CommandT{c};
  // id counter
  auto atomic_mem = zisc::cast<void*>(kernel->atomicStorage());
  std::atomic<uint32b>* id = ::new (atomic_mem) std::atomic<uint32b>{0};

  LaunchResult result{};
  CpuDevice& device = kernel->parentImpl();
  // Command submission
  {
    Fence& fence = result.fence();
    fence.setDevice(launch_options.isFenceRequested() ? &device : nullptr);
    constexpr uint32b dim = KernelT::dimension();
    const auto work_size = KernelT::expandWorkSize(launch_options.workSize(), 1);
    const auto id_offset = KernelT::expandWorkSize(launch_options.globalIdOffset(), 0);
    device.submit(*command, dim, work_size, id_offset, id, std::addressof(fence));
  }
  result.setAsync(true);
  return result;
}

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
  using ArgParserT = typename BaseKernel::ArgParser;
  if constexpr (kIndex < ArgParserT::kNumOfArgs) {
    using CacheType = typename ArgCache::template CacheType<kIndex>;
    using ArgType = std::remove_cv_t<std::remove_pointer_t<CacheType>>;
    using ArgTypeInfo = KernelArgTypeInfo<ArgType>;
    if constexpr (ArgTypeInfo::kIsLocal) { // Process a local argument
      using ElementType = typename ArgTypeInfo::ElementType;
      ElementType storage{};
      auto data = std::addressof(storage);
      cl::AddressSpacePointer<cl::AddressSpaceType::kLocal, ElementType> cl_arg{data};
      runImpl<kIndex + 1, kCacheIndex>(std::forward<Types>(cl_args)..., cl_arg);
    }
    else if constexpr (ArgTypeInfo::kIsPod) { // Process a pod argument
      auto cl_arg = arg_cache_.template get<kCacheIndex>();
      runImpl<kIndex + 1, kCacheIndex + 1>(std::forward<Types>(cl_args)..., cl_arg);
    }
    else { // Process a global argument
      using ElementType = typename ArgTypeInfo::ElementType;
      using PointerT = typename ArgType::Pointer;
      BufferCommon* cache = arg_cache_.template get<kCacheIndex>();
      auto data = zisc::cast<PointerT>(cache->rawBufferData());
      cl::AddressSpacePointer<cl::AddressSpaceType::kGlobal, ElementType> cl_arg{data};
      runImpl<kIndex + 1, kCacheIndex + 1>(std::forward<Types>(cl_args)..., cl_arg);
    }
  }
  else { // Launch the kernel
    Function func = kernel();
    std::invoke(func, std::forward<Types>(cl_args)...);
  }
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
