/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_TYPES_HPP
#define ZIVC_CPUCL_TYPES_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/ieee_754_binary.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc::cl {

// Scalar data types
using uchar = uint8b;
using ushort = uint16b;
using ulong = uint64b;
using half = zisc::Binary16;

// Pointer types
using size_t = std::size_t;
using ptrdiff_t = std::ptrdiff_t;
using intptr_t = std::intptr_t;
using uintptr_t = std::uintptr_t;

// Concepts

//! Specify that a type is half type
template <typename Type>
concept Half = zisc::SameAs<half, std::remove_cvref_t<Type>>;

//! Specify a type is arithmetic
template <typename Type>
concept Arithmetic = zisc::Arithmetic<Type> || Half<Type>;

} // namespace zivc::cl

#include "types-inl.hpp"

#endif // ZIVC_CPUCL_TYPES_HPP
