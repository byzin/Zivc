/*!
  \file math-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_MATH_INL_HPP
#define ZIVC_CL_MATH_INL_HPP

#include "math.hpp"
// Zivc
#include "bit.hpp"
#include "limits.hpp"
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl::sqrt(x);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Native::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const FloatN y = ZIVC_CL_GLOBAL_NAMESPACE::sqrt(x);
  return y;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Math::Fallback::sqrt(const FloatN x) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  return Impl<ElementT>::sqrt(x);
}

/*!
  \brief No brief description

  No detailed description.
  */
template <>
struct Math::Fallback::Util<float>
{
};

/*!
  \brief The math implementation for float

  No detailed description.
  */
template <>
struct Math::Fallback::Impl<float>
{
  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept
  {
  }
};

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] x No description.
  \return No description
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN x) noexcept
{
  return Math::sqrt(x);
}

} /* namespace zivc */

#endif /* ZIVC_CL_MATH_INL_HPP */
