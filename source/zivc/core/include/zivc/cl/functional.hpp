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

// Arithmetic

//! Function object implementing 'x + y'
template <typename T = void>
struct Plus;

//! Function object implementing 'x - y'
template <typename T = void>
struct Minus;

//! Function object implementing 'x * y'
template <typename T = void>
struct Multiply;

//! Function object implementing 'x / y'
template <typename T = void>
struct Divide;

//! Function object implementing 'x % y'
template <typename T = void>
struct Modulu;

//! function object implementing '-x'
template <typename T = void>
struct Negate;

// Comparison

//! Function object implementing 'x == y'
template <typename T = void>
struct EqualTo;

//! Function object implementing 'x != y'
template <typename T = void>
struct NotEqualTo;

//! Function object implementing 'x > y'
template <typename T = void>
struct Greater;

//! Function object implementing 'x < y'
template <typename T = void>
struct Less;

//! Function object implementing 'x >= y'
template <typename T = void>
struct GreaterEqual;

//! Function object implementing 'x <= y'
template <typename T = void>
struct LessEqual;

// Logical

//! Function object implementing 'x && y'
template <typename T = void>
struct LogicalAnd;

//! Function object implementing 'x || y'
template <typename T = void>
struct LogicalOr;

//! Function object implementing '!x'
template <typename T = void>
struct LogicalNot;

// Bitwise

//! Function object implementing 'x & y'
template <typename T = void>
struct BitAnd;

//! Function object implementing 'x | y'
template <typename T = void>
struct BitOr;

//! Function object implementing 'x ^ y'
template <typename T = void>
struct BitXor;

//! Function object implementing '~x'
template <typename T = void>
struct BitNot;

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Plus
{
  //! Return the sum of lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Minus
{
  //! Return the difference between lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Multiply
{
  //! Return the product of lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Divide
{
  //! Return the result of division of lhs by rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Modulu
{
  //! Return the remainder of the division of lhs by rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Negate
{
  //! Return the negation of arg
  constexpr T operator()(const T arg) const noexcept;
};

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

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct NotEqualTo 
{
  //! Check whether lhs is not equal to rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Greater
{
  //! Check whether lhs is greater than rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct Less
{
  //! Check whether lhs is less than rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct GreaterEqual
{
  //! Check whether lhs is greater than or equal to rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct LessEqual 
{
  //! Check if lhs is less than or equal to rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct LogicalAnd 
{
  //! Return the logical AND of lhs and rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct LogicalOr
{
  //! Return the logical OR of lhs and rhs
  constexpr bool operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct LogicalNot
{
  //! Return the logical NOT of arg
  constexpr bool operator()(const T arg) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct BitAnd
{
  //! Return the result of bitwise AND of lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct BitOr
{
  //! Return the result of bitwise OR of lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct BitXor
{
  //! Return the result of bitwise XOR of lhs and rhs
  constexpr T operator()(const T lhs, const T rhs) const noexcept;
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct BitNot
{
  //! Return the result of bitwise NOT of arg
  constexpr T operator()(const T arg) const noexcept;
};

} /* namespace zivc */

#include "functional-inl.hpp"

#endif /* ZIVC_CL_FUNCTIONAL_HPP */
