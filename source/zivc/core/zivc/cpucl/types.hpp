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
#include <concepts>
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
concept Half = std::same_as<half, std::remove_cvref_t<Type>>;

//! Specify a type is arithmetic
template <typename Type>
concept Arithmetic = zisc::Arithmetic<Type> || Half<Type>;

//! Specify OpenCL built-in scalar types
template <typename Type>
concept Scalar = std::same_as<bool, std::remove_cvref_t<Type>> ||
                 std::same_as<char, std::remove_cvref_t<Type>> ||
                 std::same_as<signed char, std::remove_cvref_t<Type>> ||
                 std::same_as<unsigned char, std::remove_cvref_t<Type>> ||
                 std::same_as<uchar, std::remove_cvref_t<Type>> ||
                 std::same_as<short, std::remove_cvref_t<Type>> ||
                 std::same_as<signed short, std::remove_cvref_t<Type>> ||
                 std::same_as<unsigned short, std::remove_cvref_t<Type>> ||
                 std::same_as<ushort, std::remove_cvref_t<Type>> ||
                 std::same_as<int, std::remove_cvref_t<Type>> ||
                 std::same_as<signed int, std::remove_cvref_t<Type>> ||
                 std::same_as<unsigned int, std::remove_cvref_t<Type>> ||
                 std::same_as<uint, std::remove_cvref_t<Type>> ||
                 std::same_as<long, std::remove_cvref_t<Type>> ||
                 std::same_as<signed long, std::remove_cvref_t<Type>> ||
                 std::same_as<unsigned long, std::remove_cvref_t<Type>> ||
                 std::same_as<ulong, std::remove_cvref_t<Type>> ||
                 std::same_as<long long, std::remove_cvref_t<Type>> ||
                 std::same_as<signed long long, std::remove_cvref_t<Type>> ||
                 std::same_as<unsigned long long, std::remove_cvref_t<Type>> ||
                 std::same_as<float, std::remove_cvref_t<Type>> ||
                 std::same_as<double, std::remove_cvref_t<Type>> ||
                 std::same_as<half, std::remove_cvref_t<Type>> ||
                 std::same_as<size_t, std::remove_cvref_t<Type>> ||
                 std::same_as<ptrdiff_t, std::remove_cvref_t<Type>> ||
                 std::same_as<intptr_t, std::remove_cvref_t<Type>> ||
                 std::same_as<uintptr_t, std::remove_cvref_t<Type>>;

//! Specify a type is the same size as another type
template <typename T, typename U>
concept SameSizeAs = sizeof(T) == sizeof(U);

} // namespace zivc::cl

#include "types-inl.hpp"

#endif // ZIVC_CPUCL_TYPES_HPP
