/*!
  \file bit.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_BIT_HPP
#define ZIVC_CL_BIT_HPP

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class Bit
{
 public:
  //! Reinterpret the object representation of one type as that of another
  template <typename To, typename From>
  static To castBit(const From& from) noexcept;

 private:
  //! The implementation of castBit
  template <typename To> struct CastImpl;
};

//! Reinterpret the object representation of one type as that of another
template <typename To, typename From>
To castBit(const From& from) noexcept;

} // namespace zivc

#include "bit-inl.hpp"

#endif // ZIVC_CL_BIT_HPP
