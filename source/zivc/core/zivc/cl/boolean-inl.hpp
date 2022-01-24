/*!
  \file boolean-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_BOOLEAN_INL_HPP
#define ZIVC_CL_BOOLEAN_INL_HPP

#include "boolean.hpp"
// Zivc
#include "types.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] value No description.
  \return No description
  */
inline
constexpr Boolean& Boolean::operator=(const bool value) noexcept
{
  set(value);
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr Boolean::operator bool() const noexcept
{
  return toBool();
}

/*!
  \details No detailed description

  \param [in] value No description.
  */
inline
constexpr void Boolean::set(const bool value) noexcept
{
  state_ = value ? getTrue() : getFalse();
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool Boolean::toBool() const noexcept
{
  const bool result = state() == getTrue();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint8b Boolean::getFalse() noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint8b Boolean::getTrue() noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint8b Boolean::state() const noexcept
{
  return state_;
}

static_assert(sizeof(Boolean) == 1, "The size of zivc::Boolean isn't 1.");

} /* namespace zivc */

#endif /* ZIVC_CL_BOOLEAN_INL_HPP */
