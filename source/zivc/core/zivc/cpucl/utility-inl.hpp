/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_UTILITY_INL_HPP
#define ZIVC_CPUCL_UTILITY_INL_HPP

#include "utility.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

namespace inner {

/*!
  \brief No brief description

  No detailed description.
  */
class WorkItem
{
 public:
  //! Return the number of dimensions in use
  static uint32b getDimension() noexcept;

  //! Return the 3d global offset used in global id calculation
  static size_t getGlobalIdOffset(const uint32b dimension) noexcept;

  //! Return the number of work-groups for dimension
  static size_t getNumOfGroups(const uint32b dimension) noexcept;

  //! Return the work-group ID for dimension
  static size_t getWorkGroupId(const uint32b dimension) noexcept;

  //! Set the number of dimensions in use
  static void setDimension(const uint32b dimension) noexcept;

  //! Set the 3d global offset used in global id calculation
  static void setGlobalIdOffset(const std::array<uint32b, 3>& offset) noexcept;

  //! Set the number of work-grous
  static void setNumOfGroups(const std::array<uint32b, 3>& size) noexcept;

  //! Set the work-group ID
  static void setWorkGroupId(const uint32b id) noexcept;

 private:
  static thread_local uint32b dimension_;
  static thread_local std::array<uint32b, 3> global_id_offset_;
  static thread_local std::array<uint32b, 3> num_of_work_groups_;
  static thread_local std::array<uint32b, 3> work_group_id_;
};

} // namespace inner

/*!
  \details No detailed description

  \param [in] format No description.
  \param [in] value No description.
  */
template <typename Type> inline
void printf(const char* format, const Type& value) noexcept
{
  std::printf(format, value);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_enqueued_local_size([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_id(const uint32b dimension) noexcept
{
  const auto id = get_group_id(dimension) + get_global_offset(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_global_linear_id() noexcept
{
  const uint32b dim = get_work_dim();
  // 1d
  const size_t gx = get_global_id(0) - get_global_offset(0);
  size_t id = gx;
  // 2d
  if (2 <= dim) {
    const size_t n = get_global_size(0);
    const size_t gy = get_global_id(1) - get_global_offset(1);
    id = id + n * gy;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = get_global_size(0) * get_global_size(1);
    const size_t gz = get_global_id(2) - get_global_offset(2);
    id = id + n * gz;
  }
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_offset(const uint32b dimension) noexcept
{
  return inner::WorkItem::getGlobalIdOffset(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_size(const uint32b dimension) noexcept
{
  return get_num_groups(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_group_id(const uint32b dimension) noexcept
{
  return inner::WorkItem::getWorkGroupId(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_id([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_local_linear_id() noexcept
{
  const uint32b dim = get_work_dim();
  // 1d
  const size_t lx = get_local_id(0);
  size_t id = lx;
  // 2d
  if (2 <= dim) {
    const size_t n = get_local_size(0);
    const size_t ly = get_local_id(1);
    id = id + n * ly;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = get_local_size(0) * get_local_size(1);
    const size_t lz = get_local_id(2);
    id = id + n * lz;
  }
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_linear_id([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_size([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_num_groups(const uint32b dimension) noexcept
{
  return inner::WorkItem::getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b get_work_dim() noexcept
{
  return inner::WorkItem::getDimension();
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_UTILITY_INL_HPP
