/*!
  \file synchronization.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_SYNCHRONIZATION_HPP
#define ZIVC_CL_SYNCHRONIZATION_HPP

// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

/*!
 \brief Memory address space
 */
enum cl_mem_fence_flags
{
  CLK_LOCAL_MEM_FENCE =  0b01 << 0,
  CLK_GLOBAL_MEM_FENCE = 0b01 << 1
};

// Forward declaration
void barrier(const int32b /* flags */) noexcept;

//! Dummy function
inline
void barrier(const int32b /* flags */) noexcept {}

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_SYNCHRONIZATION_HPP
