/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPES_HPP
#define ZIVC_CL_TYPES_HPP

namespace zivc {

// Forward declaration

//! Definitions of basic types in zivc
class ZivcType;

} /* namespace zivc */

#include "types-inl.hpp"

namespace zivc {

// Type aliases

// Integer types
using int8b = ZivcType::int8b; //!< 8bit signed integer type
using int16b = ZivcType::int16b; //!< 16bit signed integer type
using int32b = ZivcType::int32b; //!< 32bit signed integer type
using int64b = ZivcType::int64b; //!< 64bit signed integer type
// Unsigned integer types
using uint8b = ZivcType::uint8b; //!< 8bit unsigned integer type
using uint16b = ZivcType::uint16b; //!< 16bit unsigned integer type
using uint32b = ZivcType::uint32b; //!< 32bit unsigned integer type
using uint64b = ZivcType::uint64b; //!< 64bit unsigned integer type

// Address space types
// Global
//! Alias of '__global'
template <typename Type>
using Global = ZivcType::Global<Type>;
//! Alias of 'const __global'
template <typename Type>
using ConstGlobal = ZivcType::ConstGlobal<Type>;
//! Alias of '__global*'
template <typename Type>
using GlobalPtr = ZivcType::GlobalPtr<Type>;
//! Alias of 'const __global*'
template <typename Type>
using ConstGlobalPtr = ZivcType::ConstGlobalPtr<Type>;
// Local
//! Alias of '__local'
template <typename Type>
using Local = ZivcType::Local<Type>;
//! Alias of 'const __local'
template <typename Type>
using ConstLocal = ZivcType::ConstLocal<Type>;
//! Alias of '__local*'
template <typename Type>
using LocalPtr = ZivcType::LocalPtr<Type>;
//! Alias of 'const __local*'
template <typename Type>
using ConstLocalPtr = ZivcType::ConstLocalPtr<Type>;
// Constant
//! Alias of '__constant'
template <typename Type>
using Constant = ZivcType::Constant<Type>;
//! Alias of '__constant*'
template <typename Type>
using ConstantPtr = ZivcType::ConstantPtr<Type>;
//! Alias of 'const __constant*'
template <typename Type>
using ConstConstantPtr = ZivcType::ConstConstantPtr<Type>;
// Private
//! Alias of '__private'
template <typename Type>
using Private = ZivcType::Private<Type>;
//! Alias of 'const __private'
template <typename Type>
using ConstPrivate = ZivcType::ConstPrivate<Type>;
//! Alias of '__private*'
template <typename Type>
using PrivatePtr = ZivcType::PrivatePtr<Type>;
//! Alias of 'const __private*'
template <typename Type>
using ConstPrivatePtr = ZivcType::ConstPrivatePtr<Type>;
// Generic
//! Alias of '__generic'
template <typename Type>
using Generic = ZivcType::Generic<Type>;
//! Alias of 'const __generic'
template <typename Type>
using ConstGeneric = ZivcType::ConstGeneric<Type>;
//! Alias of '__generic*'
template <typename Type>
using GenericPtr = ZivcType::GenericPtr<Type>;
//! Alias of 'const __generic*'
template <typename Type>
using ConstGenericPtr = ZivcType::ConstGenericPtr<Type>;

} // namespace zivc

#endif /* ZIVC_CL_TYPES_HPP */
