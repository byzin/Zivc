/*!
  \file kernel_set.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_SET_HPP
#define ZIVC_KERNEL_SET_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
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
  //! Return the ID number of the kernel set
  static constexpr uint32b id() noexcept;

  //! Load the SPIR-V code
  void loadSpirVCode(zisc::pmr::vector<uint32b>* spirv_code_out) noexcept;
};

} // namespace zivc

#include "kernel_set-inl.hpp"

#endif // ZIVC_KERNEL_SET_HPP
