/*!
  \file geometric-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_GEOMETRIC_INL_HPP
#define ZIVC_CPUCL_GEOMETRIC_INL_HPP

#include "geometric.hpp"
// Standard C++ library
#include <concepts>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "math.hpp"
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float3 Geometry::cross(const float3& p0, const float3& p1) noexcept
{
  return crossImpl(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float4 Geometry::cross(const float4& p0, const float4& p1) noexcept
{
  return crossImpl(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double3 Geometry::cross(const double3& p0, const double3& p1) noexcept
{
  return crossImpl(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double4 Geometry::cross(const double4& p0, const double4& p1) noexcept
{
  return crossImpl(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float Geometry::dot(const Vector<Float, kN>& p0,
                    const Vector<Float, kN>& p1) noexcept
{
  return dotImpl(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float Geometry::distance(const Vector<Float, kN>& p0,
                         const Vector<Float, kN>& p1) noexcept
{
  return distanceImpl(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float Geometry::length(const Vector<Float, kN>& p) noexcept
{
  return lengthImpl(p);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> Geometry::normalize(const Vector<Float, kN>& p) noexcept
{
  return normalizeImpl(p);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
Vector<Float, kN> Geometry::crossImpl(const Vector<Float, kN>& p0,
                                      const Vector<Float, kN>& p1) noexcept
{
  Vector<Float, kN> result{};
  {
    Vector<Float, kN>* rptr = result.alignedThis();
    const Vector<Float, kN>* xptr = p0.alignedThis();
    const Vector<Float, kN>* yptr = p1.alignedThis();
    rptr->x = xptr->y * yptr->z - xptr->z * yptr->y;
    rptr->y = xptr->z * yptr->x - xptr->x * yptr->z;
    rptr->z = xptr->x * yptr->y - xptr->y * yptr->x;
  }
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
Float Geometry::dotImpl(const Vector<Float, kN>& p0,
                        const Vector<Float, kN>& p1) noexcept
{
  const Vector d = p0 * p1;
  const Float result = d.sum();
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
Float Geometry::distanceImpl(const Vector<Float, kN>& p0,
                             const Vector<Float, kN>& p1) noexcept
{
  const Float result = length(p0 - p1);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
Float Geometry::lengthImpl(const Vector<Float, kN>& p) noexcept
{
  const Float d2 = dot(p, p);
  const Float result = Math::sqrt(d2);
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, size_t kN> inline
Vector<Float, kN> Geometry::normalizeImpl(const Vector<Float, kN>& p) noexcept
{
  const Float d2 = dot(p, p);
  const Vector<Float, kN> result = p * Math::rsqrt(d2);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float3 cross(const float3& p0, const float3& p1) noexcept
{
  return Geometry::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float4 cross(const float4& p0, const float4& p1) noexcept
{
  return Geometry::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double3 cross(const double3& p0, const double3& p1) noexcept
{
  return Geometry::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double4 cross(const double4& p0, const double4& p1) noexcept
{
  return Geometry::cross(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float dot(const Vector<Float, kN>& p0,
          const Vector<Float, kN>& p1) noexcept
{
  return Geometry::dot(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float distance(const Vector<Float, kN>& p0,
               const Vector<Float, kN>& p1) noexcept
{
  return Geometry::distance(p0, p1);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Float length(const Vector<Float, kN>& p) noexcept
{
  return Geometry::length(p);
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam kN No description.
  \param [in] p No description.
  \return No description
  */
template <std::floating_point Float, std::size_t kN> inline
Vector<Float, kN> normalize(const Vector<Float, kN>& p) noexcept
{
  return Geometry::normalize(p);
}

} // namespace zivc::cl

#endif // ZIVC_CPUCL_GEOMETRIC_INL_HPP
