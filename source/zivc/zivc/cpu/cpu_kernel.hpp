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
class CpuKernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...> :
    public Kernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>
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
  void run(std::add_lvalue_reference_t<ArgTypes>... args,
           const LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const Parameters& params) override;

 private:
  /*!
    \brief No brief description

    No detailed description.

    \tparam ArgType No description.
    \tparam RestTypes No description.
    */
  template <typename ArgType, typename ...RestTypes>
  class Launcher 
  {
   public:
    //! Launch the given function 
    template <typename Type, typename ...Types>
    static void exec(Function func,
                     const LaunchOptions& launch_options,
                     Type&& value,
                     Types&&... rest) noexcept;

   private:
    //! Invoke the given function 
    template <typename ...Types>
    static void invoke(Function func,
                       const LaunchOptions& launch_options,
                       Types&&... args) noexcept;
  };

  //! Expand the given work size to 3d work size array
  static std::array<uint32b, 3> expandTo3d(
    const std::array<uint32b, kDimension>& work_size) noexcept;

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;


  Function kernel_ = nullptr;
};

} // namespace zivc

#include "cpu_kernel-inl.hpp"

#endif // ZIVC_CPU_KERNEL_HPP
