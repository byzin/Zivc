/*!
  \file launch_result-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_LAUNCH_RESULT_INL_HPP
#define ZIVC_LAUNCH_RESULT_INL_HPP

#include "launch_result.hpp"
// Standard C++ library
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/non_copyable.hpp"
// Zivc
#include "fence.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
LaunchResult::LaunchResult() noexcept :
    is_async_{}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
inline
LaunchResult::LaunchResult(LaunchResult&& other) noexcept :
    fence_{std::move(other.fence_)},
    is_async_{other.is_async_}
{
}

/*!
  \details No detailed description
  */
inline
LaunchResult::~LaunchResult() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
inline
LaunchResult& LaunchResult::operator=(LaunchResult&& other) noexcept
{
  fence_ = std::move(other.fence_);
  is_async_ = other.is_async_;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
Fence& LaunchResult::fence() noexcept
{
  return fence_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const Fence& LaunchResult::fence() const noexcept
{
  return fence_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool LaunchResult::isAsync() const noexcept
{
  return is_async_;
}

/*!
  \details No detailed description

  \param [in] is_async No description.
  */
inline
void LaunchResult::setAsync(const bool is_async) noexcept
{
  is_async_ = is_async;
}

} // namespace zivc

#endif // ZIVC_LAUNCH_RESULT_INL_HPP
