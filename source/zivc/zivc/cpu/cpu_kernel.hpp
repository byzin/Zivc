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
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "zivc/kernel.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
template <typename> class Buffer;
class CpuDevice;
template <std::size_t, DerivedKSet, typename...> class KernelParams;
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
class CpuKernel<KernelParams<kDim, KSet, FuncArgs...>, Args...> :
    public Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>;
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
  LaunchResult run(Args... args, LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Params& params) override;

  //! Update debug info
  void updateDebugInfoImpl() noexcept override;

 private:
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

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;


  Function kernel_ = nullptr;
};

} // namespace zivc

#include "cpu_kernel-inl.hpp"

#endif // ZIVC_CPU_KERNEL_HPP
