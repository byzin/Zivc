/*!
  \file functional-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_FUNCTIONAL_INL_HPP
#define ZIVC_CL_FUNCTIONAL_INL_HPP

#include "functional.hpp"

namespace zivc {

/*!
  \brief Specialization of EqualTo

  No detailed description.
  */
template <>
struct EqualTo<void>
{
  /*!
    \details No detailed description

    \tparam T No description.
    \tparam U No description.
    \param [in] lhs No description.
    \param [in] rhs No description.
    \return No description
    */
  template <typename T, typename U>
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs == rhs)
  {
    return lhs == rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool EqualTo<T>::operator()(const T lhs, const T rhs) const noexcept
{
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif // Z_GCC || Z_CLANG
  return lhs == rhs;
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG
}

} /* namespace zivc */

#endif /* ZIVC_CL_FUNCTIONAL_INL_HPP */
