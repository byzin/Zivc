/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPES_HPP
#define ZIVC_CL_TYPES_HPP

#include "types-inl.hpp"

namespace zivc {

// Type aliases

// Integer types
using int8b = inner::int8b; //!< 8bit signed integer type
using int16b = inner::int16b; //!< 16bit signed integer type
using int32b = inner::int32b; //!< 32bit signed integer type
using int64b = inner::int64b; //!< 64bit signed integer type
// Unsigned integer types
using uint8b = inner::uint8b; //!< 8bit unsigned integer type
using uint16b = inner::uint16b; //!< 16bit unsigned integer type
using uint32b = inner::uint32b; //!< 32bit unsigned integer type
using uint64b = inner::uint64b; //!< 64bit unsigned integer type

// Address space types
// Global
//! Alias of '__global'
template <typename Type>
using Global = inner::Global<Type>;
//! Alias of '__global*'
template <typename Type>
using GlobalPtr = inner::GlobalPtr<Type>;
//! Alias of 'const __global*'
template <typename Type>
using ConstGlobalPtr = inner::ConstGlobalPtr<Type>;
// Local
//! Alias of '__local'
template <typename Type>
using Local = inner::Local<Type>;
//! Alias of '__local*'
template <typename Type>
using LocalPtr = inner::LocalPtr<Type>;
//! Alias of 'const __local*'
template <typename Type>
using ConstLocalPtr = inner::ConstLocalPtr<Type>;
// Constant
//! Alias of '__constant'
template <typename Type>
using Constant = inner::Constant<Type>;
//! Alias of '__constant*'
template <typename Type>
using ConstantPtr = inner::ConstantPtr<Type>;
//! Alias of 'const __constant*'
template <typename Type>
using ConstConstantPtr = inner::ConstConstantPtr<Type>;
// Private
//! Alias of '__private'
template <typename Type>
using Private = inner::Private<Type>;
//! Alias of '__private*'
template <typename Type>
using PrivatePtr = inner::PrivatePtr<Type>;
//! Alias of 'const __private*'
template <typename Type>
using ConstPrivatePtr = inner::ConstPrivatePtr<Type>;
// Generic
//! Alias of '__generic'
template <typename Type>
using Generic = inner::Generic<Type>;
//! Alias of '__generic*'
template <typename Type>
using GenericPtr = inner::GenericPtr<Type>;
//! Alias of 'const __generic*'
template <typename Type>
using ConstGenericPtr = inner::ConstGenericPtr<Type>;

} // namespace zivc

#endif /* ZIVC_CL_TYPES_HPP */
