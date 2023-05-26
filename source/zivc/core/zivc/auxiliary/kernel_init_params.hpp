/*!
  \file kernel_init_params.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_INIT_PARAMS_HPP
#define ZIVC_KERNEL_INIT_PARAMS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
// Zivc
#include "id_data.hpp"
#include "../kernel_set.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args>
class KernelInitParams
{
 public:
  // Type aliases
  using KernelSetT = KSet;
  using FunctionT = void (*)(Args...);


  //! Initialize parameters
  KernelInitParams(FunctionT ptr, std::string_view kernel_name) noexcept;


  //! Return the dimension of the kernel
  static constexpr std::size_t dimension() noexcept;

  //! Return the underlying function
  FunctionT func() const noexcept;

  //! Return the kernel name
  std::string_view kernelName() const noexcept;

  //! Return the maximum kernel name length
  static constexpr std::size_t maxKernelNameLength() noexcept;

  //! Set a function
  void setFunc(FunctionT ptr) noexcept;

  //! Set a kernel name
  void setKernelName(std::string_view kernel_name) noexcept;

  //! Set a ptr to a VkCommandBuffer object
  void setVulkanCommandBufferPtr(const void* command_buffer_ptr) noexcept;

  //! Return a ptr to a VkCommandBuffer object
  const void* vulkanCommandBufferPtr() const noexcept;

 private:
  //! Initialize parameters
  void initialize(std::string_view kernel_name) noexcept;


  FunctionT function_;
  IdData::NameT kernel_name_;
  const void* command_buffer_ptr_ = nullptr;
};

} // namespace zivc

#include "kernel_init_params-inl.hpp"

#endif // ZIVC_KERNEL_INIT_PARAMS_HPP
