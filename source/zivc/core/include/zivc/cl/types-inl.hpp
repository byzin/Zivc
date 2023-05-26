/*!
  \file types-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPES_INL_HPP
#define ZIVC_CL_TYPES_INL_HPP

#if defined(ZIVC_CL_CPU) && !defined(ZIVC_CPUCL_CPUCL_HPP)
#include "zivc/cpucl/cpucl.hpp"
#include "zivc/cpucl/cl_define.hpp"
// Initialize the cl for cpu
using namespace zivc::cl;
#endif /* ZIVC_CL_CPU && ZIVC_CPUCL_CPUCL_HPP */

namespace zivc {

/*!
  No detailed description.
  */
class ZivcType
{
 public:
#if defined(ZIVC_CL_CPU)
  // Integer types
  using int8b = ::zivc::int8b;
  using int16b = ::zivc::int16b;
  using int32b = ::zivc::int32b;
  using int64b = ::zivc::int64b;
  // Unsigned integer types
  using uint8b = ::zivc::uint8b;
  using uint16b = ::zivc::uint16b;
  using uint32b = ::zivc::uint32b;
  using uint64b = ::zivc::uint64b;
  // Address space types
  // Global
  template <typename Type>
  using Global = Type;
  template <typename Type>
  using ConstGlobal = const Global<Type>;
  template <typename Type>
  using GlobalPtr = AddressSpacePointer<AddressSpaceType::kGlobal, Type>;
  template <typename Type>
  using ConstGlobalPtr = AddressSpacePointer<AddressSpaceType::kGlobal, const Type>;
  // Local
  template <typename Type>
  using Local = Type;
  template <typename Type>
  using ConstLocal = const Local<Type>;
  template <typename Type>
  using LocalPtr = AddressSpacePointer<AddressSpaceType::kLocal, Type>;
  template <typename Type>
  using ConstLocalPtr = AddressSpacePointer<AddressSpaceType::kLocal, const Type>;
  // Constant
  template <typename Type>
  using Constant = const Type;
  template <typename Type>
  using ConstantPtr = AddressSpacePointer<AddressSpaceType::kConstant, const Type>;
  template <typename Type>
  using ConstConstantPtr = AddressSpacePointer<AddressSpaceType::kConstant, const Type>;
  // Private
  template <typename Type>
  using Private = Type;
  template <typename Type>
  using ConstPrivate = const Private<Type>;
  template <typename Type>
  using PrivatePtr = AddressSpacePointer<AddressSpaceType::kPrivate, Type>;
  template <typename Type>
  using ConstPrivatePtr = AddressSpacePointer<AddressSpaceType::kPrivate, const Type>;
  // Generic
  template <typename Type>
  using Generic = Type;
  template <typename Type>
  using ConstGeneric = const Generic<Type>;
  template <typename Type>
  using GenericPtr = PrivatePtr<Type>;
  template <typename Type>
  using ConstGenericPtr = ConstPrivatePtr<Type>;
#else // Vulkan
  // Integer types
  using int8b = char;
  using int16b = short;
  using int32b = int;
  using int64b = long;
  // Unsigned integer types
  using uint8b = unsigned char;
  using uint16b = unsigned short;
  using uint32b = unsigned int;
  using uint64b = unsigned long;
  // Address space types
  // Global
  template <typename Type>
  using Global = __global Type;
  template <typename Type>
  using ConstGlobal = const Global<Type>;
  template <typename Type>
  using GlobalPtr = Global<Type>*;
  template <typename Type>
  using ConstGlobalPtr = const Global<Type>*;
  // Local
  template <typename Type>
  using Local = __local Type;
  template <typename Type>
  using ConstLocal = const Local<Type>;
  template <typename Type>
  using LocalPtr = Local<Type>*;
  template <typename Type>
  using ConstLocalPtr = const Local<Type>*;
  // Constant
  template <typename Type>
  using Constant = __constant Type;
  template <typename Type>
  using ConstantPtr = Constant<Type>*;
  template <typename Type>
  using ConstConstantPtr = const Constant<Type>*;
  // Private
  template <typename Type>
  using Private = __private Type;
  template <typename Type>
  using ConstPrivate = const Private<Type>;
  template <typename Type>
  using PrivatePtr = Private<Type>*;
  template <typename Type>
  using ConstPrivatePtr = const Private<Type>*;
  // Generic
  template <typename Type>
  using Generic = __generic Type;
  template <typename Type>
  using ConstGeneric = const Generic<Type>;
  template <typename Type>
  using GenericPtr = Generic<Type>*;
  template <typename Type>
  using ConstGenericPtr = const Generic<Type>*;
#endif
};

} // namespace zivc

#endif /* ZIVC_CL_TYPES_INL_HPP */
