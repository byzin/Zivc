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
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t WorkItem::getEnqueuedLocalSize([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t WorkItem::getGlobalId(const uint32b dimension) noexcept
{
  const size_t id = getWorkGroupId(dimension) + getGlobalIdOffset(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t WorkItem::getGlobalLinearId() noexcept
{
  const uint32b dim = getDimension();
  // 1d
  const size_t gx = getGlobalId(0) - getGlobalIdOffset(0);
  size_t id = gx;
  // 2d
  if (2 <= dim) {
    const size_t n = getGlobalSize(0);
    const size_t gy = getGlobalId(1) - getGlobalIdOffset(1);
    id = id + n * gy;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = getGlobalSize(0) * getGlobalSize(1);
    const size_t gz = getGlobalId(2) - getGlobalIdOffset(2);
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
size_t WorkItem::getGlobalSize(const uint32b dimension) noexcept
{
  return getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t WorkItem::getLocalId([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t WorkItem::getLocalLinearId() noexcept
{
  const uint32b dim = getDimension();
  // 1d
  const size_t lx = getLocalId(0);
  size_t id = lx;
  // 2d
  if (2 <= dim) {
    const size_t n = getLocalSize(0);
    const size_t ly = getLocalId(1);
    id = id + n * ly;
  }
  // 3d
  if (3 <= dim) {
    const size_t n = getLocalSize(0) * getLocalSize(1);
    const size_t lz = getLocalId(2);
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
constexpr size_t WorkItem::getLocalSize([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] condition No description.
  \param [in] format No description.
  \param [in] args No description.
  */
template <typename... Types> inline
void assertIfFalse(const bool condition, const char* format, const Types&... args)
{
  if (!condition) {
    std::fprintf(stderr, format, args...);
    std::exit(EXIT_FAILURE);
  }
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] args No description.
  */
template <typename... Types> inline
int printf(const char* format, const Types&... args)
{
  //! \todo Implement type-safe printf
  return std::printf(format, args...);
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_enqueued_local_size(const uint32b dimension) noexcept
{
  return WorkItem::getEnqueuedLocalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_id(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalId(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_global_linear_id() noexcept
{
  return WorkItem::getGlobalLinearId();
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_offset(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalIdOffset(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_size(const uint32b dimension) noexcept
{
  return WorkItem::getGlobalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_group_id(const uint32b dimension) noexcept
{
  return WorkItem::getWorkGroupId(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_id(const uint32b dimension) noexcept
{
  return WorkItem::getLocalId(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t get_local_linear_id() noexcept
{
  return WorkItem::getLocalLinearId();
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_size(const uint32b dimension) noexcept
{
  return WorkItem::getLocalSize(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_num_groups(const uint32b dimension) noexcept
{
  return WorkItem::getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b get_work_dim() noexcept
{
  return WorkItem::getDimension();
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_UTILITY_INL_HPP
