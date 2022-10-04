/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
#include <utility>
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
  using BaseKernelT = Kernel<KernelInitParams<kDim, KSet, FuncArgs...>, Args...>;
  using ParamsT = typename BaseKernelT::ParamsT;
  using FunctionT = typename ParamsT::FunctionT;
  using LaunchOptionsT = typename BaseKernelT::LaunchOptionsT;


  //! Initialize the kernel
  CpuKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~CpuKernel() noexcept override;


  //! Return the underlying kernel function
  FunctionT kernel() const noexcept;

  //! Execute a kernel
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult run(Args... args, const LaunchOptionsT& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const ParamsT& params) override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  //! Command object for kernel execution
  struct Command
  {
    //! Exec the underlying kernel
    void operator()() noexcept;

    CpuKernel* kernel_ = nullptr;
  };


  //! Make a struct of arg cache
  template <typename Type, typename ...Types, typename ...ArgTypes>
  static auto makeArgCacheT(const KernelArgCache<ArgTypes...>& cache) noexcept;


  // Storage types
  using ArgCacheT = decltype(makeArgCacheT<Args...>(KernelArgCache<void>{}));
  using CommandStorageT = std::aligned_storage_t<
      sizeof(void*),
      std::alignment_of_v<void*>>;
  using AtomicStorageT = std::aligned_storage_t<
      sizeof(std::atomic<uint32b>),
      std::alignment_of_v<std::atomic<uint32b>>>;


  //! Return the memory for command
  AtomicStorageT* atomicStorage() noexcept;

  //! Return the memory for command
  CommandStorageT* commandStorage() noexcept;

  //! Create a command for a kernel submission
  Command* createCommand() noexcept;

  //! Create an ID counter for a kernel submission
  std::atomic<uint32b>* createIdCounter() noexcept;

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;

  //! Run the underlying kernel
  template <std::size_t kIndex, std::size_t kCacheIndex, typename ...Types>
  void runImpl(Types&&... cl_args) noexcept;

  //! Run the underlying kernel
  template <std::size_t... indices>
  void runImpl(const std::index_sequence<indices...> index_seq) noexcept;

  //! Update arg cache
  template <std::size_t kIndex, KernelArg Type, typename ...Types>
  void updateArgCache(const Type value, Types&&... values) noexcept;

  //! Update arg cache
  template <std::size_t kIndex, KernelArg Type, typename ...Types>
  void updateArgCache(Buffer<Type>& value, Types&&... values) noexcept;


  FunctionT kernel_ = nullptr;
  ArgCacheT arg_cache_;
  CommandStorageT command_storage_;
  AtomicStorageT atomic_storage_;
  [[maybe_unused]] uint32b padding_;
};

} // namespace zivc

#include "cpu_kernel-inl.hpp"

#endif // ZIVC_CPU_KERNEL_HPP
