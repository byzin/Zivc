/*!
  \file geometric-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_GEOMETRIC_INL_HPP
#define ZIVC_CL_GEOMETRIC_INL_HPP

#include "geometric.hpp"
// Zivc
#include "bit.hpp"
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Geometry::cross(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector isn't 3 or 4.");
  const FloatN result = ZIVC_CL_GLOBAL_NAMESPACE::cross(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::dot(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZIVC_CL_GLOBAL_NAMESPACE::dot(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float Geometry::dot3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  const float result = dot(q0, q1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double Geometry::dot3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  const double result = dot(q0, q1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::distance(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZIVC_CL_GLOBAL_NAMESPACE::distance(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float Geometry::distance3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  const float result = distance(q0, q1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double Geometry::distance3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  const double result = distance(q0, q1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::length(const FloatN p) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZIVC_CL_GLOBAL_NAMESPACE::length(p);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
float Geometry::length3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  const float result = length(q);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
double Geometry::length3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  const double result = length(q);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Geometry::normalize(const FloatN p) noexcept
{
  static_assert(kIsFloat<FloatN>, "The FloatN isn't floating point.");
  const FloatN result = ZIVC_CL_GLOBAL_NAMESPACE::normalize(p);
  return result;
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 Geometry::normalize3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  const float3 result = ZIVC_CL_GLOBAL_NAMESPACE::normalize(q);
  return castBit<float4>(result);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 Geometry::normalize3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  const double3 result = ZIVC_CL_GLOBAL_NAMESPACE::normalize(q);
  return castBit<double4>(result);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
FloatN cross(const FloatN p0, const FloatN p1) noexcept
{
  const FloatN result = Geometry::cross(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto dot(const FloatN p0, const FloatN p1) noexcept
{
  const auto result = Geometry::dot(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float dot3(const float4 p0, const float4 p1) noexcept
{
  const float result = Geometry::dot3(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double dot3(const double4 p0, const double4 p1) noexcept
{
  const double result = Geometry::dot3(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto distance(const FloatN p0, const FloatN p1) noexcept
{
  const auto result = Geometry::distance(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float distance3(const float4 p0, const float4 p1) noexcept
{
  const float result = Geometry::distance3(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double distance3(const double4 p0, const double4 p1) noexcept
{
  const double result = Geometry::distance3(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
auto length(const FloatN p) noexcept
{
  const auto result = Geometry::length(p);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
float length3(const float4 p) noexcept
{
  const float result = Geometry::length3(p);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
double length3(const double4 p) noexcept
{
  const double result = Geometry::length3(p);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
FloatN normalize(const FloatN p) noexcept
{
  const FloatN result = Geometry::normalize(p);
  return result;
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 normalize3(const float4 p) noexcept
{
  const float4 result = Geometry::normalize3(p);
  return result;
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 normalize3(const double4 p) noexcept
{
  const double4 result = Geometry::normalize3(p);
  return result;
}

} // namespace zivc

#endif /* ZIVC_CL_GEOMETRIC_INL_HPP */
