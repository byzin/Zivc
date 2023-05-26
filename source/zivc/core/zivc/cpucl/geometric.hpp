/*!
  \file geometric.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_GEOMETRIC_HPP
#define ZIVC_CPUCL_GEOMETRIC_HPP

// Standard C++ library
#include <concepts>
#include <type_traits>
// Zivc
#include "vector.hpp"
#include "../zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.
  */
class Geometry
{
 public:
  //! Return the cross product of p0.xyz and p1.xyz
  static float3 cross(const float3& p0, const float3& p1) noexcept;

  //! Return the cross product of p0.xyz and p1.xyz
  static float4 cross(const float4& p0, const float4& p1) noexcept;

  //! Return the cross product of p0.xyz and p1.xyz
  static double3 cross(const double3& p0, const double3& p1) noexcept;

  //! Return the cross product of p0.xyz and p1.xyz
  static double4 cross(const double4& p0, const double4& p1) noexcept;

  //! Compute dot product
  template <std::floating_point Float, std::size_t kN>
  static Float dot(const Vector<Float, kN>& p0,
                   const Vector<Float, kN>& p1) noexcept;

  //! Return the distance between p0 and p1
  template <std::floating_point Float, std::size_t kN>
  static Float distance(const Vector<Float, kN>& p0,
                        const Vector<Float, kN>& p1) noexcept;

  //! Return the length of vector p
  template <std::floating_point Float, std::size_t kN>
  static Float length(const Vector<Float, kN>& p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <std::floating_point Float, std::size_t kN>
  static Vector<Float, kN> normalize(const Vector<Float, kN>& p) noexcept;

 private:
  //! Return the cross product of p0.xyz and p1.xyz
  template <std::floating_point Float, size_t kN>
  static Vector<Float, kN> crossImpl(const Vector<Float, kN>& p0,
                                     const Vector<Float, kN>& p1) noexcept;

  //! Compute dot product
  template <std::floating_point Float, size_t kN>
  static Float dotImpl(const Vector<Float, kN>& p0,
                       const Vector<Float, kN>& p1) noexcept;

  //! Compute dot product
  template <std::floating_point Float, size_t kN>
  static Float distanceImpl(const Vector<Float, kN>& p0,
                            const Vector<Float, kN>& p1) noexcept;

  //! Compute dot product
  template <std::floating_point Float, size_t kN>
  static Float lengthImpl(const Vector<Float, kN>& p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <std::floating_point Float, size_t kN>
  static Vector<Float, kN> normalizeImpl(const Vector<Float, kN>& p) noexcept;
};

//! Return the cross product of p0.xyz and p1.xyz
float3 cross(const float3& p0, const float3& p1) noexcept;

//! Return the cross product of p0.xyz and p1.xyz
float4 cross(const float4& p0, const float4& p1) noexcept;

//! Return the cross product of p0.xyz and p1.xyz
double3 cross(const double3& p0, const double3& p1) noexcept;

//! Return the cross product of p0.xyz and p1.xyz
double4 cross(const double4& p0, const double4& p1) noexcept;

//! Compute dot product
template <std::floating_point Float, std::size_t kN>
Float dot(const Vector<Float, kN>& p0,
          const Vector<Float, kN>& p1) noexcept;

//! Return the distance between p0 and p1
template <std::floating_point Float, std::size_t kN>
Float distance(const Vector<Float, kN>& p0,
               const Vector<Float, kN>& p1) noexcept;

//! Return the length of vector p
template <std::floating_point Float, std::size_t kN>
Float length(const Vector<Float, kN>& p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
template <std::floating_point Float, std::size_t kN>
Vector<Float, kN> normalize(const Vector<Float, kN>& p) noexcept;

} // namespace zivc::cl

#include "geometric-inl.hpp"

#endif // ZIVC_CPUCL_GEOMETRIC_HPP
