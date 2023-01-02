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
#include <type_traits>
// Zivc
#include "vector.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

/*!
  */
class Geometry
{
 public:
  //! Return the cross product of p0.xyz and p1.xyz
  template <typename FloatN>
  static FloatN cross(const FloatN& p0, const FloatN& p1) noexcept;

  //! Compute dot product
  template <typename FloatN>
  static auto dot(const FloatN& p0, const FloatN& p1) noexcept;

  //! Return the distance between p0 and p1
  template <typename FloatN>
  static auto distance(const FloatN& p0, const FloatN& p1) noexcept;

  //! Return the length of vector p
  template <typename FloatN>
  static auto length(const FloatN& p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <typename FloatN>
  static FloatN normalize(const FloatN& p) noexcept;

 private:
  /*!
    */
  class Vec
  {
   public:
    //! Return the cross product of p0.xyz and p1.xyz
    template <typename Float, size_t kN>
    static auto cross(const Vector<Float, kN>& p0,
                      const Vector<Float, kN>& p1) noexcept;

    //! Compute dot product
    template <typename Float, size_t kN>
    static auto dot(const Vector<Float, kN>& p0,
                    const Vector<Float, kN>& p1) noexcept;

    //! Return a vector in the same direction as p but with a length of 1
    template <typename Float, size_t kN>
    static auto normalize(const Vector<Float, kN>& p) noexcept;
  };
};

//! Return the cross product of p0.xyz and p1.xyz
template <typename FloatN>
FloatN cross(const FloatN& p0, const FloatN& p1) noexcept;

//! Compute dot product
template <typename FloatN>
auto dot(const FloatN& p0, const FloatN& p1) noexcept;

//! Return the distance between p0 and p1
template <typename FloatN>
auto distance(const FloatN& p0, const FloatN& p1) noexcept;

//! Return the length of vector p
template <typename FloatN>
auto length(const FloatN& p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
template <typename FloatN>
FloatN normalize(const FloatN& p) noexcept;

} // namespace cl

} // namespace zivc

#include "geometric-inl.hpp"

#endif // ZIVC_CPUCL_GEOMETRIC_HPP
