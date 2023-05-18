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
  \brief Specialization of Plus

  No detailed description.
  */
template <>
struct Plus<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs + rhs)
  {
    return lhs + rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T Plus<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs + rhs;
}

/*!
  \brief Specialization of Minus

  No detailed description.
  */
template <>
struct Minus<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs - rhs)
  {
    return lhs - rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T Minus<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs - rhs;
}

/*!
  \brief Specialization of Multiply

  No detailed description.
  */
template <>
struct Multiply<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs * rhs)
  {
    return lhs * rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T Multiply<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs * rhs;
}

/*!
  \brief Specialization of Divide

  No detailed description.
  */
template <>
struct Divide<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs / rhs)
  {
    return lhs / rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T Divide<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs / rhs;
}

/*!
  \brief Specialization of Modulu

  No detailed description.
  */
template <>
struct Modulu<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs % rhs)
  {
    return lhs % rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T Modulu<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs % rhs;
}

/*!
  \brief Specialization of Negate

  No detailed description.
  */
template <>
struct Negate<void>
{
  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] arg No description.
    \return No description
    */
  template <typename T>
  constexpr auto operator()(const T arg) const noexcept -> decltype(-arg)
  {
    return -arg;
  }
};

/*!
  \details No detailed description

  \param [in] arg No description.
  \return No description
  */
template <typename T> inline
constexpr T Negate<T>::operator()(const T arg) const noexcept
{
  return -arg;
}

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

/*!
  \brief Specialization of NotEqualTo

  No detailed description.
  */
template <>
struct NotEqualTo<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs != rhs)
  {
    return lhs != rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool NotEqualTo<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return EqualTo<T>{}(lhs, rhs);
}

/*!
  \brief Specialization of Greater

  No detailed description.
  */
template <>
struct Greater<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs > rhs)
  {
    return lhs > rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool Greater<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs > rhs;
}

/*!
  \brief Specialization of Less

  No detailed description.
  */
template <>
struct Less<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs < rhs)
  {
    return lhs < rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool Less<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs < rhs;
}

/*!
  \brief Specialization of GreaterEqual

  No detailed description.
  */
template <>
struct GreaterEqual<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs >= rhs)
  {
    return lhs >= rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool GreaterEqual<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs >= rhs;
}

/*!
  \brief Specialization of LessEqual

  No detailed description.
  */
template <>
struct LessEqual<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs <= rhs)
  {
    return lhs <= rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool LessEqual<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs <= rhs;
}

/*!
  \brief Specialization of LogicalAnd

  No detailed description.
  */
template <>
struct LogicalAnd<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs && rhs)
  {
    return lhs && rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool LogicalAnd<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs && rhs;
}

/*!
  \brief Specialization of LogicalOr

  No detailed description.
  */
template <>
struct LogicalOr<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs || rhs)
  {
    return lhs || rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr bool LogicalOr<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs || rhs;
}

/*!
  \brief Specialization of LogicalNot

  No detailed description.
  */
template <>
struct LogicalNot<void>
{
  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] arg No description.
    \return No description
    */
  template <typename T>
  constexpr auto operator()(const T arg) const noexcept -> decltype(!arg)
  {
    return !arg;
  }
};

/*!
  \details No detailed description

  \param [in] arg No description.
  \return No description
  */
template <typename T> inline
constexpr bool LogicalNot<T>::operator()(const T arg) const noexcept
{
  return !arg;
}

/*!
  \brief Specialization of BitAnd

  No detailed description.
  */
template <>
struct BitAnd<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs & rhs)
  {
    return lhs & rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T BitAnd<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs & rhs;
}

/*!
  \brief Specialization of BitOr

  No detailed description.
  */
template <>
struct BitOr<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs | rhs)
  {
    return lhs | rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T BitOr<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs | rhs;
}

/*!
  \brief Specialization of BitXor

  No detailed description.
  */
template <>
struct BitXor<void>
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
  constexpr auto operator()(const T lhs, const U rhs) const noexcept -> decltype(lhs ^ rhs)
  {
    return lhs ^ rhs;
  }
};

/*!
  \details No detailed description

  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename T> inline
constexpr T BitXor<T>::operator()(const T lhs, const T rhs) const noexcept
{
  return lhs ^ rhs;
}

/*!
  \brief Specialization of BitNot

  No detailed description.
  */
template <>
struct BitNot<void>
{
  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] arg No description.
    \return No description
    */
  template <typename T>
  constexpr auto operator()(const T arg) const noexcept -> decltype(~arg)
  {
    return ~arg;
  }
};

/*!
  \details No detailed description

  \param [in] arg No description.
  \return No description
  */
template <typename T> inline
constexpr T BitNot<T>::operator()(const T arg) const noexcept
{
  return ~arg;
}

} /* namespace zivc */

#endif /* ZIVC_CL_FUNCTIONAL_INL_HPP */
