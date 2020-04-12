/*!
  \file kernel_set-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_SET_INL_HPP
#define ZIVC_KERNEL_SET_INL_HPP

#include "kernel_set.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
constexpr uint32b KernelSet<SetType>::id() noexcept
{
  return SetType::id();
}

/*!
  \details No detailed description

  \param [out] spirv_code_out No description.
  */
template <typename SetType> inline
void KernelSet<SetType>::loadSpirVCode(
    zisc::pmr::vector<uint32b>* spirv_code_out) noexcept
{
  SetType::loadSpirVCode(spirv_code_out);
}

} // namespace zivc

#endif // ZIVC_KERNEL_SET_INL_HPP
