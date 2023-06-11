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

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float3 Geometry::cross(const float3 p0, const float3 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float4 Geometry::cross(const float4 p0, const float4 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double3 Geometry::cross(const double3 p0, const double3 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double4 Geometry::cross(const double4 p0, const double4 p1) noexcept
{
  return Impl::cross(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Impl::dot(p0, p1);
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
  return Impl::dot3(p0, p1);
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
  return Impl::dot3(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Impl::distance(p0, p1);
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
  return Impl::distance3(p0, p1);
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
  return Impl::distance3(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Impl::length(p);
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
  return Impl::length3(p);
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
  return Impl::length3(p);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Impl::normalize(p);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 Geometry::normalize3(const float4 p) noexcept
{
  return Impl::normalize3(p);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 Geometry::normalize3(const double4 p) noexcept
{
  return Impl::normalize3(p);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float3 Geometry::Native::cross(const float3 p0, const float3 p1) noexcept
{
  const float3 result = ZIVC_CL_GLOBAL_NAMESPACE::cross(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float4 Geometry::Native::cross(const float4 p0, const float4 p1) noexcept
{
  const float4 result = ZIVC_CL_GLOBAL_NAMESPACE::cross(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double3 Geometry::Native::cross(const double3 p0, const double3 p1) noexcept
{
  const double3 result = ZIVC_CL_GLOBAL_NAMESPACE::cross(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double4 Geometry::Native::cross(const double4 p0, const double4 p1) noexcept
{
  const double4 result = ZIVC_CL_GLOBAL_NAMESPACE::cross(p0, p1);
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
auto Geometry::Native::dot(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const ElementT result = ZIVC_CL_GLOBAL_NAMESPACE::dot(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float Geometry::Native::dot3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  return dot(q0, q1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double Geometry::Native::dot3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  return dot(q0, q1);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::Native::distance(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const ElementT result = ZIVC_CL_GLOBAL_NAMESPACE::distance(p0, p1);
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float Geometry::Native::distance3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  return distance(q0, q1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double Geometry::Native::distance3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  return distance(q0, q1);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::Native::length(const FloatN p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const ElementT result = ZIVC_CL_GLOBAL_NAMESPACE::length(p);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
float Geometry::Native::length3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  return length(q);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
double Geometry::Native::length3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  return length(q);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Geometry::Native::normalize(const FloatN p) noexcept
{
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  const FloatN result = ZIVC_CL_GLOBAL_NAMESPACE::normalize(p);
  return result;
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 Geometry::Native::normalize3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  const float3 result = normalize(q);
  return castBit<float4>(result);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 Geometry::Native::normalize3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  const double3 result = normalize(q);
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
constexpr FloatN Geometry::Fallback::Impl::cross(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  FloatN result = static_cast<ElementT>(0);
  result.x = p0.y * p1.z - p0.z * p1.y;
  result.y = p0.z * p1.x - p0.x * p1.z;
  result.z = p0.x * p1.y - p0.y * p1.x;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float> inline
constexpr Float Geometry::Fallback::Impl::dotN1(const Float p0, const Float p1) noexcept
{
  const Float result = p0 * p1;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam Float2 No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float, typename Float2> inline
constexpr Float Geometry::Fallback::Impl::dotN2(const Float2 p0, const Float2 p1) noexcept
{
  const Float result = p0.x * p1.x +
                       p0.y * p1.y;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam Float3 No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float, typename Float3> inline
constexpr Float Geometry::Fallback::Impl::dotN3(const Float3 p0, const Float3 p1) noexcept
{
  const Float result = p0.x * p1.x +
                       p0.y * p1.y +
                       p0.z * p1.z;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam Float4 No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float, typename Float4> inline
constexpr Float Geometry::Fallback::Impl::dotN4(const Float4 p0, const Float4 p1) noexcept
{
  const Float result = p0.x * p1.x +
                       p0.y * p1.y +
                       p0.z * p1.z +
                       p0.w * p1.w;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam Float8 No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float, typename Float8> inline
constexpr Float Geometry::Fallback::Impl::dotN8(const Float8 p0, const Float8 p1) noexcept
{
  const Float result = p0.s0 * p1.s0 +
                       p0.s1 * p1.s1 +
                       p0.s2 * p1.s2 +
                       p0.s3 * p1.s3 +
                       p0.s4 * p1.s4 +
                       p0.s5 * p1.s5 +
                       p0.s6 * p1.s6 +
                       p0.s7 * p1.s7;
  return result;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \tparam Float16 No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename Float, typename Float16> inline
constexpr Float Geometry::Fallback::Impl::dotN16(const Float16 p0, const Float16 p1) noexcept
{
  const Float result = p0.s0 * p1.s0 +
                       p0.s1 * p1.s1 +
                       p0.s2 * p1.s2 +
                       p0.s3 * p1.s3 +
                       p0.s4 * p1.s4 +
                       p0.s5 * p1.s5 +
                       p0.s6 * p1.s6 +
                       p0.s7 * p1.s7 +
                       p0.s8 * p1.s8 +
                       p0.s9 * p1.s9 +
                       p0.sa * p1.sa +
                       p0.sb * p1.sb +
                       p0.sc * p1.sc +
                       p0.sd * p1.sd +
                       p0.se * p1.se +
                       p0.sf * p1.sf;
  return result;
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float3 Geometry::Fallback::cross(const float3 p0, const float3 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float4 Geometry::Fallback::cross(const float4 p0, const float4 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double3 Geometry::Fallback::cross(const double3 p0, const double3 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double4 Geometry::Fallback::cross(const double4 p0, const double4 p1) noexcept
{
  return Impl::cross(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float p0, const float p1) noexcept
{
  return Impl::dotN1<float>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float2 p0, const float2 p1) noexcept
{
  return Impl::dotN2<float, float2>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float3 p0, const float3 p1) noexcept
{
  return Impl::dotN3<float, float3>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float4 p0, const float4 p1) noexcept
{
  return Impl::dotN4<float, float4>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float8 p0, const float8 p1) noexcept
{
  return Impl::dotN8<float, float8>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot(const float16 p0, const float16 p1) noexcept
{
  return Impl::dotN16<float, float16>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double p0, const double p1) noexcept
{
  return Impl::dotN1<double>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double2 p0, const double2 p1) noexcept
{
  return Impl::dotN2<double, double2>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double3 p0, const double3 p1) noexcept
{
  return Impl::dotN3<double, double3>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double4 p0, const double4 p1) noexcept
{
  return Impl::dotN4<double, double4>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double8 p0, const double8 p1) noexcept
{
  return Impl::dotN8<double, double8>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot(const double16 p0, const double16 p1) noexcept
{
  return Impl::dotN16<double, double16>(p0, p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr float Geometry::Fallback::dot3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  return dot(q0, q1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
constexpr double Geometry::Fallback::dot3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  return dot(q0, q1);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::Fallback::length(const FloatN p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const ElementT d2 = dot(p, p);
  const ElementT result = sqrt(d2);
  return result;
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
float Geometry::Fallback::length3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  return length(q);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
inline
double Geometry::Fallback::length3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  return length(q);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
template <typename FloatN> inline
auto Geometry::Fallback::distance(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return length(p0 - p1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float Geometry::Fallback::distance3(const float4 p0, const float4 p1) noexcept
{
  const auto q0 = castBit<float3>(p0);
  const auto q1 = castBit<float3>(p1);
  return distance(q0, q1);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
double Geometry::Fallback::distance3(const double4 p0, const double4 p1) noexcept
{
  const auto q0 = castBit<double3>(p0);
  const auto q1 = castBit<double3>(p1);
  return distance(q0, q1);
}

/*!
  \details No detailed description

  \tparam FloatN No description.
  \param [in] p No description.
  \return No description
  */
template <typename FloatN> inline
FloatN Geometry::Fallback::normalize(const FloatN p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveVolatileT<FloatN>>;
  using ElementT = typename VecInfo::ElementT;
  static_assert(kIsFloat<ElementT>, "The FloatN isn't floating point.");
  const ElementT d2 = dot(p, p);
  const FloatN result = p * rsqrt(d2);
  return result;
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 Geometry::Fallback::normalize3(const float4 p) noexcept
{
  const auto q = castBit<float3>(p);
  const float3 result = normalize(q);
  return castBit<float4>(result);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 Geometry::Fallback::normalize3(const double4 p) noexcept
{
  const auto q = castBit<double3>(p);
  const double3 result = normalize(q);
  return castBit<double4>(result);
}

/*!
  \details No detailed description

  \param [in] p0 No description.
  \param [in] p1 No description.
  \return No description
  */
inline
float3 cross(const float3 p0, const float3 p1) noexcept
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
float4 cross(const float4 p0, const float4 p1) noexcept
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
double3 cross(const double3 p0, const double3 p1) noexcept
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
double4 cross(const double4 p0, const double4 p1) noexcept
{
  return Geometry::cross(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Geometry::dot(p0, p1);
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
  return Geometry::dot3(p0, p1);
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
  return Geometry::dot3(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Geometry::distance(p0, p1);
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
  return Geometry::distance3(p0, p1);
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
  return Geometry::distance3(p0, p1);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Geometry::length(p);
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
  return Geometry::length3(p);
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
  return Geometry::length3(p);
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
  static_assert(kIsFloat<RemoveVolatileT<FloatN>>, "The FloatN isn't floating point.");
  return Geometry::normalize(p);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
float4 normalize3(const float4 p) noexcept
{
  return Geometry::normalize3(p);
}

/*!
  \details No detailed description

  \param [in] p No description.
  \return No description
  */
inline
double4 normalize3(const double4 p) noexcept
{
  return Geometry::normalize3(p);
}

} // namespace zivc

#endif /* ZIVC_CL_GEOMETRIC_INL_HPP */
