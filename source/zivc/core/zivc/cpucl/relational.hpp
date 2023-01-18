/*!
  \file relational.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_RELATIONAL_HPP
#define ZIVC_CPUCL_RELATIONAL_HPP

// Standard C++ library
#include <concepts>
#include <cstddef>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

/*!
  \brief No brief description

  No detailed description.
  */
class Relation
{
 public:
  //! Return the component-wize compare of lhs == rhs
  template <std::floating_point Float>
  static CompResult<Float> isequal(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs == rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isequal(const Vector<Float, kN>& lhs,
                                          const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of lhs != rhs
  template <std::floating_point Float>
  static CompResult<Float> isnotequal(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs != rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isnotequal(const Vector<Float, kN>& lhs,
                                             const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of lhs > rhs
  template <std::floating_point Float>
  static CompResult<Float> isgreater(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs > rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isgreater(const Vector<Float, kN>& lhs,
                                            const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of lhs >= rhs
  template <std::floating_point Float>
  static CompResult<Float> isgreaterequal(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs >= rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isgreaterequal(const Vector<Float, kN>& lhs,
                                                 const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of lhs < rhs
  template <std::floating_point Float>
  static CompResult<Float> isless(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs < rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isless(const Vector<Float, kN>& lhs,
                                         const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of lhs <= rhs
  template <std::floating_point Float>
  static CompResult<Float> islessequal(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of lhs <= rhs
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> islessequal(const Vector<Float, kN>& lhs,
                                              const Vector<Float, kN>& rhs) noexcept;

  //! Return the component-wize compare of (lhs < rhs) || (lhs > rhs)
  template <std::floating_point Float>
  static CompResult<Float> islessgreater(const Float lhs, const Float rhs) noexcept;

  //! Return the component-wize compare of (lhs < rhs) || (lhs > rhs)
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> islessgreater(const Vector<Float, kN>& lhs,
                                                const Vector<Float, kN>& rhs) noexcept;

  //! Determin if the given floating point is a finite value 
  template <std::floating_point Float>
  static CompResult<Float> isfinite(const Float value) noexcept;

  //! Determin if the given floating point is a finite value
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isfinite(const Vector<Float, kN>& value) noexcept;

  //! Determin if the given floating point is a positive or negative infinity
  template <std::floating_point Float>
  static CompResult<Float> isinf(const Float value) noexcept;

  //! Determin if the given floating point is a positive or negative infinity
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isinf(const Vector<Float, kN>& value) noexcept;

  //! Determin if the given floating point is a Nan
  template <std::floating_point Float>
  static CompResult<Float> isnan(const Float value) noexcept;

  //! Determin if the given floating point is a Nan
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isnan(const Vector<Float, kN>& value) noexcept;

  //! Determin if the given floating point is a normal value 
  template <std::floating_point Float>
  static CompResult<Float> isnormal(const Float value) noexcept;

  //! Determin if the given floating point is a normal value
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> isnormal(const Vector<Float, kN>& value) noexcept;

  //! Determin if the given floating point is negative
  template <std::floating_point Float>
  static CompResult<Float> signbit(const Float value) noexcept;

  //! Determin if the given floating point is negative
  template <std::floating_point Float, size_t kN>
  static CompResultVec<Float, kN> signbit(const Vector<Float, kN>& value) noexcept;

  //! Return 1 if the msb in any component of x is set; otherwise return 0
  template <std::integral Integer>
  static int32b any(const Integer x) noexcept;

  //! Return 1 if the msb in any component of x is set; otherwise return 0
  template <std::integral Integer, size_t kN>
  static int32b any(const Vector<Integer, kN>& x) noexcept;

  //! Return 1 if the msb in all component of x is set; otherwise return 0
  template <std::integral Integer>
  static int32b all(const Integer x) noexcept;

  //! Return 1 if the msb in all component of x is set; otherwise return 0
  template <std::integral Integer, size_t kN>
  static int32b all(const Vector<Integer, kN>& x) noexcept;

  //! Each component of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
  template <zisc::Scalar Type>
  static Type bitselect(const Type a, const Type b, const Type c) noexcept;

  //! Each component of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
  template <zisc::Scalar Type, size_t kN>
  static Vector<Type, kN> bitselect(const Vector<Type, kN>& a,
                                    const Vector<Type, kN>& b,
                                    const Vector<Type, kN>& c) noexcept;

  //! Each component of the result is set to c[i] ? b[i] : a[i]
  template <zisc::Scalar Type, std::integral Integer>
  static Type select(const Type a, const Type b, const Integer c) noexcept;

  //! Each component of the result is set to c[i] ? b[i] : a[i]
  template <zisc::Scalar Type, std::integral Integer, size_t kN>
  static Vector<Type, kN> select(const Vector<Type, kN>& a,
                                 const Vector<Type, kN>& b,
                                 const Vector<Integer, kN>& c) noexcept;
};

//! Return the component-wize compare of lhs == rhs
template <std::floating_point Float>
CompResult<Float> isequal(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs == rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isequal(const Vector<Float, kN>& lhs,
                                 const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
template <std::floating_point Float>
CompResult<Float> isnotequal(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isnotequal(const Vector<Float, kN>& lhs,
                                    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
template <std::floating_point Float>
CompResult<Float> isgreater(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isgreater(const Vector<Float, kN>& lhs,
                                   const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
template <std::floating_point Float>
CompResult<Float> isgreaterequal(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isgreaterequal(const Vector<Float, kN>& lhs,
                                        const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
template <std::floating_point Float>
CompResult<Float> isless(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isless(const Vector<Float, kN>& lhs,
                                const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
template <std::floating_point Float>
CompResult<Float> islessequal(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> islessequal(const Vector<Float, kN>& lhs,
                                     const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of (lhs < rhs) || (lhs > rhs)
template <std::floating_point Float>
CompResult<Float> islessgreater(const Float lhs, const Float rhs) noexcept;

//! Return the component-wize compare of (lhs < rhs) || (lhs > rhs)
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> islessgreater(const Vector<Float, kN>& lhs,
                                       const Vector<Float, kN>& rhs) noexcept;

//! Determin if the given floating point is a finite value 
template <std::floating_point Float>
CompResult<Float> isfinite(const Float value) noexcept;

//! Determin if the given floating point is a finite value
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isfinite(const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <std::floating_point Float>
CompResult<Float> isinf(const Float value) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isinf(const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is a Nan
template <std::floating_point Float>
CompResult<Float> isnan(const Float value) noexcept;

//! Determin if the given floating point is a Nan
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isnan(const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is a normal value 
template <std::floating_point Float>
CompResult<Float> isnormal(const Float value) noexcept;

//! Determin if the given floating point is a normal value
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> isnormal(const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is negative
template <std::floating_point Float>
CompResult<Float> signbit(const Float value) noexcept;

//! Determin if the given floating point is negative
template <std::floating_point Float, size_t kN>
CompResultVec<Float, kN> signbit(const Vector<Float, kN>& value) noexcept;

//! Return 1 if the msb in any component of x is set; otherwise return 0
template <std::integral Integer>
int32b any(const Integer x) noexcept;

//! Return 1 if the msb in any component of x is set; otherwise return 0
template <std::integral Integer, size_t kN>
int32b any(const Vector<Integer, kN>& x) noexcept;

//! Return 1 if the msb in all component of x is set; otherwise return 0
template <std::integral Integer>
int32b all(const Integer x) noexcept;

//! Return 1 if the msb in all component of x is set; otherwise return 0
template <std::integral Integer, size_t kN>
int32b all(const Vector<Integer, kN>& x) noexcept;

//! Each component of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <zisc::Scalar Type>
Type bitselect(const Type a, const Type b, const Type c) noexcept;

//! Each component of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <zisc::Scalar Type, size_t kN>
Vector<Type, kN> bitselect(const Vector<Type, kN>& a,
                           const Vector<Type, kN>& b,
                           const Vector<Type, kN>& c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <zisc::Scalar Type, std::integral Integer>
Type select(const Type a, const Type b, const Integer c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <zisc::Scalar Type, std::integral Integer, size_t kN>
Vector<Type, kN> select(const Vector<Type, kN>& a,
                        const Vector<Type, kN>& b,
                        const Vector<Integer, kN>& c) noexcept;

} // namespace zivc::cl

#include "relational-inl.hpp"

#endif // ZIVC_CPUCL_RELATIONAL_HPP
