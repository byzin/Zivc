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
#include "zivc/utility/kernel_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
template <KernelParameter> class Buffer;
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
  LaunchResult run(Args... args, const LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Params& params) override;

  //! Update debug info
  void updateDebugInfoImpl() override;

 private:
  // Storage types
  using CommandStorage = std::aligned_storage_t<
      sizeof(void*) * (sizeof...(Args) + 2),
      std::alignment_of_v<void*>>;
  using AtomicStorage = std::aligned_storage_t<
      sizeof(std::atomic<uint32b>),
      std::alignment_of_v<std::atomic<uint32b>>>;


  /*!
    \brief No brief description

    No detailed description.

    \tparam UnprocessedArgs No description.
    */
  template <typename ...UnprocessedArgs>
  class Launcher 
  {
   public:
    //! Launch the given function 
    template <typename Type, typename ...Types>
    static void exec(Function func,
                     const LaunchOptions& launch_options,
                     Type&& value,
                     Types&&... rest) noexcept;
  };

  /*!
    \brief No brief description

    No detailed description.

    \tparam Type No description.
    \tparam Types No description.
    */
  template <typename Type, typename ...Types>
  struct LauncherHelper
  {
    using NextLauncher = Launcher<Types...>;
  };

  //! Return the memory for command
  AtomicStorage* atomicStorage() noexcept;

  //! Return the memory for command
  CommandStorage* commandStorage() noexcept;

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;


  Function kernel_ = nullptr;
  CommandStorage command_storage_;
  AtomicStorage atomic_storage_;
  [[maybe_unused]] uint32b padding_;
};

} // namespace zivc

#include "cpu_kernel-inl.hpp"

#endif // ZIVC_CPU_KERNEL_HPP
