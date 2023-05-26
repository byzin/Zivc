/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_UTILITY_HPP
#define ZIVC_CPUCL_UTILITY_HPP

// Standard C++ library
#include <array>
// Zivc
#include "types.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.
  */
class WorkItem
{
 public:
  //! Return the number of dimensions in use
  static uint32b getDimension() noexcept;

  //! Return the number of local work-items
  static constexpr size_t getEnqueuedLocalSize(const uint32b dimension) noexcept;

  //! Return the global work-item ID for dimension
  static size_t getGlobalId(const uint32b dimension) noexcept;

  //! Return the 3d global offset used in global id calculation
  static size_t getGlobalIdOffset(const uint32b dimension) noexcept;

  //! Return the work-items 1-dimensional global ID
  static size_t getGlobalLinearId() noexcept;

  //! Return the number of global work-items for dimension
  static size_t getGlobalSize(const uint32b dimension) noexcept;

  //! Return the unique local work-item ID
  static constexpr size_t getLocalId(const uint32b dimension) noexcept;

  //! Return the work-items 1-dimensional local ID
  static size_t getLocalLinearId() noexcept;

  //! Return the number of local work-items
  static constexpr size_t getLocalSize(const uint32b dimension) noexcept;

  //! Return the number of work-groups for dimension
  static size_t getNumOfGroups(const uint32b dimension) noexcept;

  //! Return the work-group ID for dimension
  static size_t getWorkGroupId(const uint32b dimension) noexcept;

  //! Set the number of dimensions in use
  static void setDimension(const uint32b dimension) noexcept;

  //! Set the 3d global offset used in global id calculation
  static void setGlobalIdOffset(const std::array<uint32b, 3>& offset) noexcept;

  //! Set the number of work-groups
  static void setNumOfGroups(const std::array<uint32b, 3>& size) noexcept;

  //! Set the work-group ID
  static void setWorkGroupId(const uint32b id) noexcept;

 private:
  static thread_local uint32b dimension_;
  static thread_local std::array<uint32b, 3> global_id_offset_;
  static thread_local std::array<uint32b, 3> num_of_work_groups_;
  static thread_local std::array<uint32b, 3> work_group_id_;
};

// OpenCL functions

//! Assert the message on standard error output and raise an error
template <typename... Types>
void assertIfFalse(const bool condition, const char* format, const Types&... args);

//! Write the result of the format to stdout
template <typename... Types>
int printf(const char* format, const Types&... args);

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
