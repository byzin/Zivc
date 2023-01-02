/*!
  \file synchronization-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_SYNCHRONIZATION_INL_HPP
#define ZIVC_CL_SYNCHRONIZATION_INL_HPP

#include "synchronization.hpp"
// Zivc
#include "types.hpp"

namespace zivc {

/*!
  */
inline
void Synchronization::barrier(const int32b flags) noexcept
{
  ZIVC_CL_GLOBAL_NAMESPACE::barrier(flags);
}

/*!
  */
inline
void Synchronization::barrierGlobal() noexcept
{
  barrier(CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void Synchronization::barrierLocal() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE);
}

/*!
  */
inline
void Synchronization::barrierAll() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void barrier(const int32b flags) noexcept
{
  Synchronization::barrier(flags);
}

} // namespace zivc

#endif /* ZIVC_CL_SYNCHRONIZATION_INL_HPP */
