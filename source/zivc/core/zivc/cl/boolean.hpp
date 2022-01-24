/*!
  \file boolean.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_BOOLEAN_HPP
#define ZIVC_CL_BOOLEAN_HPP

// Zivc
#include "types.hpp"

namespace zivc {

/*!
  \brief Capable of holding one of the two values: true or false.

  Capable of holding one of the two values: true or false.
  The sizeof(Boolean) is 1 is guaranteed.

  \note No notation.
  \attention No attention.
  */
class Boolean
{
 public:
  //! Convert from a built-in boolean
  constexpr Boolean& operator=(const bool value) noexcept;

  //! Convert to a built-in boolean
  constexpr operator bool() const noexcept;


  //! Convert from a built-in boolean
  constexpr void set(const bool value) noexcept;

  //! Convert to a built-in boolean
  constexpr bool toBool() const noexcept;

 private:
  //! Return the false value
  static constexpr uint8b getFalse() noexcept;

  //! Return the true value
  static constexpr uint8b getTrue() noexcept;

  //! Return the underlying state
  constexpr uint8b state() const noexcept;


  uint8b state_;
};

} /* namespace zivc */

#include "boolean-inl.hpp"

#endif /* ZIVC_CL_BOOLEAN_HPP */
