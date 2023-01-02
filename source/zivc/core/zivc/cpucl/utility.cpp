/*!
  \file utility.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "utility.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/algorithm.hpp"
// Zivc
#include "types.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/error.hpp"

namespace zivc::cl::inner {

/*!
  \details No detailed description

  \return No description
  */
uint32b WorkItem::getDimension() noexcept
{
  return dimension_;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
size_t WorkItem::getGlobalIdOffset(const uint32b dimension) noexcept
{
  const size_t offset = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? global_id_offset_[dimension]
      : 0u;
  return offset;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
size_t WorkItem::getNumOfGroups(const uint32b dimension) noexcept
{
  const size_t size = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? num_of_work_groups_[dimension]
      : 1u;
  return size;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
size_t WorkItem::getWorkGroupId(const uint32b dimension) noexcept
{
  const size_t id = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? work_group_id_[dimension]
      : 0u;
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  */
void WorkItem::setDimension(const uint32b dimension) noexcept
{
  dimension_ = dimension;
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
void WorkItem::setGlobalIdOffset(const std::array<uint32b, 3>& offset) noexcept
{
  global_id_offset_ = offset;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
void WorkItem::setNumOfGroups(const std::array<uint32b, 3>& size) noexcept
{
  num_of_work_groups_ = size;
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
void WorkItem::setWorkGroupId(const uint32b id) noexcept
{
  const size_t nwx = get_num_groups(0);
  const size_t nwy = get_num_groups(1);
  work_group_id_[0] = id % nwx;
  work_group_id_[1] = (id / nwx) % nwy;
  work_group_id_[2] = id / (nwx * nwy);
  ZIVC_ASSERT(work_group_id_[2] < get_num_groups(2),
              "The given work-group ID is invalid: ID=", id);
}

thread_local uint32b WorkItem::dimension_;
thread_local std::array<::zivc::uint32b, 3> WorkItem::global_id_offset_;
thread_local std::array<::zivc::uint32b, 3> WorkItem::num_of_work_groups_;
thread_local std::array<::zivc::uint32b, 3> WorkItem::work_group_id_;

} // namespace zivc::cl::inner
