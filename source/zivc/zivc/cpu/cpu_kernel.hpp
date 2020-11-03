/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/kernel_parameters.hpp"
#include "zivc/utility/launch_result.hpp"

namespace zivc {

// Forward declaration
template <typename Type> class Buffer;
class CpuDevice;
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class CpuKernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
  \tparam FuncArgTypes No description.
  \tparam ArgTypes No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
class CpuKernel<kDimension,
                KernelParameters<SetType, FuncArgTypes...>,
                ArgTypes...> :
    public Kernel<kDimension,
                  KernelParameters<SetType, FuncArgTypes...>,
                  ArgTypes...>
{
 public:
  // Type aliases
  using BaseKernel = Kernel<kDimension,
                            KernelParameters<SetType, FuncArgTypes...>,
                            ArgTypes...>;
  using Parameters = typename BaseKernel::Parameters;
  using Function = typename Parameters::Function;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  CpuKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~CpuKernel() noexcept override;


  //! Return the underlying kernel function
  Function kernel() const noexcept;

  //! Execute a kernel
  LaunchResult run(ArgTypes... args, LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Parameters& params) override;

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
