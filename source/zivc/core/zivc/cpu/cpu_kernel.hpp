/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_KERNEL_HPP
#define ZIVC_CPU_KERNEL_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/kernel_arg_cache.hpp"
#include "zivc/utility/kernel_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;
class CpuDevice;
template <std::size_t, DerivedKSet, typename...> class KernelInitParams;
template <typename, typename...> class CpuKernel;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuKernelHelper
{
 public:
  template <typename CKernel, typename Type>
  static LaunchResult run(CKernel* kernel, const Type& launch_options);
};

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam FuncArgs No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
class CpuKernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...> :
    public Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>;
  using Params = typename BaseKernel::Params;
  using Function = typename Params::Function;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  CpuKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~CpuKernel() noexcept override;


  //! Return the underlying kernel function
  Function kernel() const noexcept;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult run(Args... args, const LaunchOptions& launch_options) override
  {
    //! \note Separate declaration and definition cause a build error on visual studio
    updateArgCache<0>(args...);
    return CpuKernelHelper::run(this, launch_options);
  }

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Params& params) override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  friend CpuKernelHelper;


  //! Make a struct of arg cache
  template <typename Type, typename ...Types, typename ...ArgTypes>
  static auto makeArgCacheType(const KernelArgCache<ArgTypes...>& cache) noexcept;


  // Storage types
  using ArgCache = decltype(makeArgCacheType<Args...>(KernelArgCache<void>{}));
  using CommandStorage = std::aligned_storage_t<
      sizeof(void*),
      std::alignment_of_v<void*>>;
  using AtomicStorage = std::aligned_storage_t<
      sizeof(std::atomic<uint32b>),
      std::alignment_of_v<std::atomic<uint32b>>>;


  //! Return the memory for command
  AtomicStorage* atomicStorage() noexcept;

  //! Return the memory for command
  CommandStorage* commandStorage() noexcept;

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;

  //! Run the underlying kernel
  template <std::size_t kIndex, std::size_t kCacheIndex, typename ...Types>
  void runImpl(Types&&... cl_args) noexcept;

  //! Update arg cache
  template <std::size_t kIndex, KernelArg Type, typename ...Types>
  void updateArgCache(const Type value, Types&&... values) noexcept;

  //! Update arg cache
  template <std::size_t kIndex, KernelArg Type, typename ...Types>
  void updateArgCache(Buffer<Type>& value, Types&&... values) noexcept;


  Function kernel_ = nullptr;
  ArgCache arg_cache_;
  CommandStorage command_storage_;
  AtomicStorage atomic_storage_;
  [[maybe_unused]] uint32b padding_;
};

} // namespace zivc

#include "cpu_kernel-inl.hpp"

#endif // ZIVC_CPU_KERNEL_HPP
