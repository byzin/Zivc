/*!
  \file geometric.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_GEOMETRIC_HPP
#define ZIVC_CL_GEOMETRIC_HPP

// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class Geometry
{
 public:
  //! Return the cross product of p0.xyz and p1.xyz
  template <typename FloatN>
  static FloatN cross(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product
  template <typename FloatN>
  static auto dot(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product of p0.xyz and p1.xyz
  static float dot3(const float4 p0, const float4 p1) noexcept;

  //! Compute dot product of p0.xyz and p1.xyz
  static double dot3(const double4 p0, const double4 p1) noexcept;

  //! Return the distance between p0 and p1
  template <typename FloatN>
  static auto distance(const FloatN p0, const FloatN p1) noexcept;

  //! Return the distance between p0.xyz and p1.xyz
  static float distance3(const float4 p0, const float4 p1) noexcept;

  //! Return the distance between p0.xyz and p1.xyz
  static double distance3(const double4 p0, const double4 p1) noexcept;

  //! Return the length of vector p
  template <typename FloatN>
  static auto length(const FloatN p) noexcept;

  //! Return the length of vector p.xyz
  static float length3(const float4 p) noexcept;

  //! Return the length of vector p.xyz
  static double length3(const double4 p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <typename FloatN>
  static FloatN normalize(const FloatN p) noexcept;

  //! Return a vector in the same direction as p.xyz but with a length of 1
  static float4 normalize3(const float4 p) noexcept;

  //! Return a vector in the same direction as p.xyz but with a length of 1
  static double4 normalize3(const double4 p) noexcept;


  /*!
    \brief No brief description

    No detailed description.
    */
  struct Native
  {
    //! Return the cross product of p0.xyz and p1.xyz
    template <typename FloatN>
    static FloatN cross(const FloatN p0, const FloatN p1) noexcept;

    //! Compute dot product
    template <typename FloatN>
    static auto dot(const FloatN p0, const FloatN p1) noexcept;

    //! Compute dot product of p0.xyz and p1.xyz
    static float dot3(const float4 p0, const float4 p1) noexcept;

    //! Compute dot product of p0.xyz and p1.xyz
    static double dot3(const double4 p0, const double4 p1) noexcept;

    //! Return the distance between p0 and p1
    template <typename FloatN>
    static auto distance(const FloatN p0, const FloatN p1) noexcept;

    //! Return the distance between p0.xyz and p1.xyz
    static float distance3(const float4 p0, const float4 p1) noexcept;

    //! Return the distance between p0.xyz and p1.xyz
    static double distance3(const double4 p0, const double4 p1) noexcept;

    //! Return the length of vector p
    template <typename FloatN>
    static auto length(const FloatN p) noexcept;

    //! Return the length of vector p.xyz
    static float length3(const float4 p) noexcept;

    //! Return the length of vector p.xyz
    static double length3(const double4 p) noexcept;

    //! Return a vector in the same direction as p but with a length of 1
    template <typename FloatN>
    static FloatN normalize(const FloatN p) noexcept;

    //! Return a vector in the same direction as p.xyz but with a length of 1
    static float4 normalize3(const float4 p) noexcept;

    //! Return a vector in the same direction as p.xyz but with a length of 1
    static double4 normalize3(const double4 p) noexcept;
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  struct Fallback
  {
    //! Return the cross product of p0.xyz and p1.xyz
    template <typename FloatN>
    static FloatN cross(const FloatN p0, const FloatN p1) noexcept;

    //! Compute dot product
    template <typename FloatN>
    static auto dot(const FloatN p0, const FloatN p1) noexcept;

    //! Compute dot product of p0.xyz and p1.xyz
    static float dot3(const float4 p0, const float4 p1) noexcept;

    //! Compute dot product of p0.xyz and p1.xyz
    static double dot3(const double4 p0, const double4 p1) noexcept;

    //! Return the distance between p0 and p1
    template <typename FloatN>
    static auto distance(const FloatN p0, const FloatN p1) noexcept;

    //! Return the distance between p0.xyz and p1.xyz
    static float distance3(const float4 p0, const float4 p1) noexcept;

    //! Return the distance between p0.xyz and p1.xyz
    static double distance3(const double4 p0, const double4 p1) noexcept;

    //! Return the length of vector p
    template <typename FloatN>
    static auto length(const FloatN p) noexcept;

    //! Return the length of vector p.xyz
    static float length3(const float4 p) noexcept;

    //! Return the length of vector p.xyz
    static double length3(const double4 p) noexcept;

    //! Return a vector in the same direction as p but with a length of 1
    template <typename FloatN>
    static FloatN normalize(const FloatN p) noexcept;

    //! Return a vector in the same direction as p.xyz but with a length of 1
    static float4 normalize3(const float4 p) noexcept;

    //! Return a vector in the same direction as p.xyz but with a length of 1
    static double4 normalize3(const double4 p) noexcept;
  };

  // Type aliases
  using Impl = Native;
};

// OpenCL style function aliases

//! Return the cross product of p0.xyz and p1.xyz
template <typename FloatN>
FloatN cross(const FloatN p0, const FloatN p1) noexcept;

//! Compute dot product
template <typename FloatN>
auto dot(const FloatN p0, const FloatN p1) noexcept;

//! Compute dot product
float dot3(const float4 p0, const float4 p1) noexcept;

//! Compute dot product
double dot3(const double4 p0, const double4 p1) noexcept;

//! Return the distance between p0 and p1
template <typename FloatN>
auto distance(const FloatN p0, const FloatN p1) noexcept;

//! Return the distance between p0 and p1
float distance3(const float4 p0, const float4 p1) noexcept;

//! Return the distance between p0 and p1
double distance3(const double4 p0, const double4 p1) noexcept;

//! Return the length of vector p
template <typename FloatN>
auto length(const FloatN p) noexcept;

//! Return the length of vector p
float length3(const float4 p) noexcept;

//! Return the length of vector p
double length3(const double4 p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
template <typename FloatN>
FloatN normalize(const FloatN p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
float4 normalize3(const float4 p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
double4 normalize3(const double4 p) noexcept;

} // namespace zivc

#include "geometric-inl.hpp"

#endif /* ZIVC_CL_GEOMETRIC_HPP */
