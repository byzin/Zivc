/*!
  \file kernel_params.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_PARAMS_HPP
#define ZIVC_KERNEL_PARAMS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
// Zivc
#include "id_data.hpp"
#include "zivc/kernel_set.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
class KernelParams
{
 public:
  // Type aliases
  using KernelSetType = KSet;
  using Function = void (*)(Args...);


  //! Initialize parameters
  KernelParams(Function ptr, std::string_view kernel_name) noexcept;


  //! Return the dimension of the kernel
  static constexpr std::size_t dimension() noexcept;

  //! Return the underlying function
  Function func() const noexcept;

  //! Return the kernel name
  std::string_view kernelName() const noexcept;

  //! Return the maximum kernel name length
  static constexpr std::size_t maxKernelNameLength() noexcept;

  //! Set a function
  void setFunc(Function ptr) noexcept;

  //! Set a kernel name
  void setKernelName(std::string_view kernel_name) noexcept;

 private:
  //! Initialize parameters
  void initialize(std::string_view kernel_name) noexcept;


  Function function_;
  IdData::NameType kernel_name_;
};

} // namespace zivc

#include "kernel_params-inl.hpp"

#endif // ZIVC_KERNEL_PARAMS_HPP
