/*!
  \file kernel_set-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_SET_INL_HPP
#define ZIVC_KERNEL_SET_INL_HPP

#include "kernel_set.hpp"
// Standard C++ library
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
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
template <typename SetType> inline
KernelSet<SetType>::KernelSet(zisc::pmr::memory_resource* mem_resource) noexcept :
    spirv_code_{typename decltype(spirv_code_)::allocator_type{mem_resource}}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
constexpr uint64b KernelSet<SetType>::id() noexcept
{
  constexpr uint64b i = SetType::id();
  return i;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
constexpr std::string_view KernelSet<SetType>::name() noexcept
{
  constexpr std::string_view n = SetType::name();
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
std::span<const uint32b> KernelSet<SetType>::spirVCode() const noexcept
{
  return spirv_code_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
zisc::pmr::memory_resource* KernelSet<SetType>::memoryResource() noexcept
{
  auto alloc = spirv_code_.get_allocator();
  return alloc.resource();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename SetType> inline
const zisc::pmr::memory_resource* KernelSet<SetType>::memoryResource() const noexcept
{
  auto alloc = spirv_code_.get_allocator();
  return alloc.resource();
}

/*!
  \details No detailed description
  */
template <typename SetType> inline
void KernelSet<SetType>::initialize() noexcept
{
  loadSpirVCode(std::addressof(spirv_code_));
}

/*!
  \details No detailed description

  \param [out] spirv_code_out No description.
  */
template <typename SetType> inline
void KernelSet<SetType>::loadSpirVCode(zisc::pmr::vector<uint32b>* spirv_code_out) noexcept
{
  SetType::loadSpirVCode(spirv_code_out);
}

} // namespace zivc

#endif // ZIVC_KERNEL_SET_INL_HPP
