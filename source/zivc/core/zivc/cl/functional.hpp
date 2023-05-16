/*!
  \file functional.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_FUNCTIONAL_HPP
#define ZIVC_CL_FUNCTIONAL_HPP

namespace zivc {

// Forward declaration

//! Function object implementing 'x == y'
template <typename T = void>
struct EqualTo;

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct EqualTo
{
  //! Check whether lhs is equal to rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

} /* namespace zivc */

#include "functional-inl.hpp"

#endif /* ZIVC_CL_FUNCTIONAL_HPP */
