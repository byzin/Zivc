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

//! Specify OpenCL built-in scalar types
template <typename Type>
concept Scalar = zisc::SameAs<bool, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<char, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<signed char, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<unsigned char, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<uchar, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<short, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<signed short, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<unsigned short, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<ushort, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<int, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<signed int, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<unsigned int, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<uint, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<signed long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<unsigned long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<ulong, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<long long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<signed long long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<unsigned long long, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<float, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<double, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<half, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<size_t, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<ptrdiff_t, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<intptr_t, std::remove_cvref_t<Type>> ||
                 zisc::SameAs<uintptr_t, std::remove_cvref_t<Type>>;

//! Specify a type is the same size as another type
template <typename T, typename U>
concept SameSizeAs = sizeof(T) == sizeof(U);

} // namespace zivc::cl

#include "types-inl.hpp"

#endif // ZIVC_CPUCL_TYPES_HPP
