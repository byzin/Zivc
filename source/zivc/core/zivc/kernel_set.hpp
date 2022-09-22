/*!
  \file kernel_set.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_SET_HPP
#define ZIVC_KERNEL_SET_HPP

// Standard C++ library
#include <concepts>
#include <memory>
#include <span>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam SetType No description.
  */
template <typename SetType>
class KernelSet
{
 public:
  //! Initialize the kernel set
  KernelSet(zisc::pmr::memory_resource* mem_resource) noexcept;


  //! Return the ID number of the kernel set
  static constexpr uint64b id() noexcept;

  //! Return the kernel set name
  static constexpr std::string_view name() noexcept;

  //! Return the SPIR-V code of the kernel set
  std::span<const uint32b> spirVCode() const noexcept;

 protected:
  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

 private:
  //! Initialize the kernel set
  void initialize() noexcept;

  //! Load the SPIR-V code
  static void loadSpirVCode(zisc::pmr::vector<uint32b>* spirv_code_out) noexcept;


  zisc::pmr::vector<uint32b> spirv_code_;
};

//! Specify a type is derived from KernelSet
template <typename Type>
concept DerivedKSet = std::derived_from<Type, KernelSet<Type>>;

} // namespace zivc

#include "kernel_set-inl.hpp"

#endif // ZIVC_KERNEL_SET_HPP
