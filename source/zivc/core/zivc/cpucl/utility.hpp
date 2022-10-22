/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_UTILITY_HPP
#define ZIVC_CPUCL_UTILITY_HPP

// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

// OpenCL functions

//! Write the result of the format to stdout
template <typename Type>
void printf(const char* format, const Type& value) noexcept;

// Work-Item functions

//! Return the number of local work-items
constexpr size_t get_enqueued_local_size(const uint32b dimension) noexcept;

//! Return the global work-item ID for dimension
size_t get_global_id(const uint32b dimension) noexcept;

//! Return the work-items 1-dimensional global ID
size_t get_global_linear_id() noexcept;

//! Return the 3d global offset used in global ID calculation
size_t get_global_offset(const uint32b dimension) noexcept;

//! Return the number of global work-items for dimension
size_t get_global_size(const uint32b dimension) noexcept;

//! Return the work-group ID
size_t get_group_id(const uint32b dimension) noexcept;

//! Return the unique local work-item ID
constexpr size_t get_local_id(const uint32b dimension) noexcept;

//! Return the work-items 1-dimensional local ID
size_t get_local_linear_id() noexcept;

//! Return the number of local work-items
constexpr size_t get_local_size(const uint32b dimension) noexcept;

//! Return the number of work-groups that will execute a kernel
size_t get_num_groups(const uint32b dimension) noexcept;

//! Return the number of dimensions in use
uint32b get_work_dim() noexcept;

} // namespace zivc::cl

#include "utility-inl.hpp"

#endif // ZIVC_CPUCL_UTILITY_HPP
