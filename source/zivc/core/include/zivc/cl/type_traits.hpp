/*!
  \file type_traits.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPE_TRAITS_HPP
#define ZIVC_CL_TYPE_TRAITS_HPP

// Zivc
#include "types.hpp"

namespace zivc {

// Helper classes

//! brief Compile-time constant of specified type with specified value
template <typename T, T v>
struct IntegralConstant;

//! A helper alias template of IntegralConstant
template <bool value>
using BoolConstant = IntegralConstant<bool, value>;

//!
using TrueT = BoolConstant<true>;

//!
using FalseT = BoolConstant<false>;


// Primary type categories

////! Check if a type is void
//template <typename T>
//struct IsVoid;
//
////! Helper variable template of IsVoid
//template <typename T>
//inline constexpr bool kIsVoid = IsVoid<T>::kValue;
//
////! Check if a type is an integral type
//template <typename T>
//struct IsIntegral;
//
////! Helper variable template of IsIntegral
//template <typename T>
//inline constexpr bool kIsIntegral = IsIntegral<T>::kValue;
//
////! Check if a type is a floating point type
//template <typename T>
//struct IsFloat;
//
////! Helper variable template of IsFloat
//template <typename T>
//inline constexpr bool kIsFloat = IsFloat<T>::kValue;
//
////! Check if a type is an array type
//template <typename T>
//struct IsArray;
//
////! Helper variable template of IsArray
//template <typename T>
//inline constexpr bool kIsArray = IsArray<T>::kValue;


// Type relationship

//! brief Check if two types are same
template <typename T, typename U>
struct IsSame;

//! Helper variable template of IsSame
template <typename T, typename U>
inline constexpr bool kIsSame = IsSame<T, U>::kValue;


//! Make a signed integer type from byte size
template <size_t kBytes>
struct IntegerFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

//!
template <size_t kBytes>
using IntegerTFromBytes = typename IntegerFromBytes<kBytes>::Type;

//! Make an unsigned integer type from byte size
template <size_t kBytes>
struct UIntegerFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

//!
template <size_t kBytes>
using UIntegerTFromBytes = typename UIntegerFromBytes<kBytes>::Type;

//! Make a floating point type from byte size
template <size_t kBytes>
struct FloatFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

//!
template <size_t kBytes>
using FloatTFromBytes = typename FloatFromBytes<kBytes>::Type;

//! Information of a vector type
template <typename T>
struct VectorTypeInfo
{
  using ElementT = T;
  using Type = T;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 1u;}
};

//! Make a vector type
template <typename ElementType, size_t kN>
struct VectorFromElems
{
  using Type = ElementType;
};

//!
template <typename ElementType, size_t kN>
using VectorTFromElems = typename VectorFromElems<ElementType, kN>::Type;

//!
template <size_t kN>
using Integer8VecT = VectorTFromElems<int8b, kN>;

//!
template <size_t kN>
using Integer16VecT = VectorTFromElems<int16b, kN>;

//!
template <size_t kN>
using Integer32VecT = VectorTFromElems<int32b, kN>;

//!
template <size_t kN>
using Integer64VecT = VectorTFromElems<int64b, kN>;

//!
template <size_t kN>
using UInteger8VecT = VectorTFromElems<uint8b, kN>;

//!
template <size_t kN>
using UInteger16VecT = VectorTFromElems<uint16b, kN>;

//!
template <size_t kN>
using UInteger32VecT = VectorTFromElems<uint32b, kN>;

//!
template <size_t kN>
using UInteger64VecT = VectorTFromElems<uint64b, kN>;

//!
template <size_t kN>
using Float16VecT = VectorTFromElems<half, kN>;

//!
template <size_t kN>
using Float32VecT = VectorTFromElems<float, kN>;

//!
template <size_t kN>
using Float64VecT = VectorTFromElems<double, kN>;

//! Integer vector type that has same element size and alignment as the given type
template <typename T>
using IntegerTFromVec = VectorTFromElems<
    IntegerTFromBytes<sizeof(typename VectorTypeInfo<T>::ElementT)>,
    VectorTypeInfo<T>::size()>;

//! Unsigned integer type that has same element size and alignment as the given type
template <typename T>
using UIntegerTFromVec = VectorTFromElems<
    UIntegerTFromBytes<sizeof(typename VectorTypeInfo<T>::ElementT)>,
    VectorTypeInfo<T>::size()>;

//! Floating point type that has same element size and alignment as the given type
template <typename T>
using FloatTFrom = VectorTFromElems<
    FloatTFromBytes<sizeof(typename VectorTypeInfo<T>::ElementT)>,
    VectorTypeInfo<T>::size()>;

//! Information of an address space type
template <typename T>
class AddressSpaceInfo;
//{
//  using DataT = typename ASpaceInfo::DataT;
//  using Pointer = PrivatePtr<DataT>;
//  using ConstPointer = ConstPrivatePtr<DataT>;
//  template <typename Type>
//  using AddressSpacePointerT = PrivatePtr<Type>;
//  template <typename Type>
//  using ConstAddressSpacePointerT = ConstPrivatePtr<Type>;
//  static constexpr int32b isGlobal() noexcept;
//  static constexpr int32b isLocal() noexcept;
//  static constexpr int32b isConstant() noexcept;
//  static constexpr int32b isPrivate() noexcept;
//  static constexpr int32b isGeneric() noexcept;
//  static constexpr int32b isPointer() noexcept;
//};

// Type categories

//! Check if a type is integer scalar or vector type
template <typename T> struct IsInteger;

//!
template <typename T>
constexpr Constant<int32b> kIsInteger = IsInteger<T>::kValue;

//! Check if a type is signed integer scalar or vector type
template <typename T> struct IsSignedInteger;

//!
template <typename T>
constexpr Constant<int32b> kIsSignedInteger = IsSignedInteger<T>::kValue;

//! Check if a type is unsigned integer scalar or vector type
template <typename T> struct IsUnsignedInteger;

//!
template <typename T>
constexpr Constant<int32b> kIsUnsignedInteger = IsUnsignedInteger<T>::kValue;

//! Check if a type is floating point scalar or vector type
template <typename T> struct IsFloat;

//!
template <typename T>
constexpr Constant<int32b> kIsFloat = IsFloat<T>::kValue;

//! Check if a type is half scalar or vector type
template <typename T> struct IsHalf;

//!
template <typename T>
constexpr Constant<int32b> kIsHalf = IsHalf<T>::kValue;

//! Check if a type is float scalar or vector type
template <typename T> struct IsSingle;

//!
template <typename T>
constexpr Constant<int32b> kIsSingle = IsSingle<T>::kValue;

//! Check if a type is double scalar or vector type
template <typename T> struct IsDouble;

//!
template <typename T>
constexpr Constant<int32b> kIsDouble = IsDouble<T>::kValue;

//! Check if a type is an arithmetic type
template <typename T> struct IsArithmetic;

//!
template <typename T>
constexpr Constant<int32b> kIsArithmetic = IsArithmetic<T>::kValue;

//! Check if a type is a signed arithmetic type
template <typename T> struct IsSigned;

//!
template <typename T>
constexpr Constant<int32b> kIsSigned = IsSigned<T>::kValue;

// Type properties

//! Check if a type is a pointer type
template <typename T> struct IsPointer;

//!
template <typename T>
constexpr Constant<int32b> kIsPointer = IsPointer<T>::kValue;

//! Check if a type is scalar type
template <typename T> struct IsScalar;

//!
template <typename T>
constexpr Constant<int32b> kIsScalar = IsScalar<T>::kValue;

//! Check if a type is vector type
template <typename T> struct IsVector;

//!
template <typename T>
constexpr Constant<int32b> kIsVector = IsVector<T>::kValue;

//! Check if a type is lvalue reference
template <typename T> struct IsLValueReference;

//!
template <typename T>
constexpr Constant<int32b> kIsLValueReference = IsLValueReference<T>::kValue;

//! Check if a type is rvalue reference
template <typename T> struct IsRValueReference;

//!
template <typename T>
constexpr Constant<int32b> kIsRValueReference = IsRValueReference<T>::kValue;

// Miscellaneous transformations

//! Add 'const' and/or 'volatile' specifiers to the given type
template <typename T> struct AddCv;

//!
template <typename T>
using AddCvT = typename AddCv<T>::Type;

//! Add 'const' specifiers to the given type
template <typename T> struct AddConst;

//!
template <typename T>
using AddConstT = typename AddConst<T>::Type;

//! Add 'volatile' specifiers to the given type
template <typename T> struct AddVolatile;

//!
template <typename T>
using AddVolatileT = typename AddVolatile<T>::Type;

//! Remove 'const' and/or 'volatile' specifiers from the given type
template <typename T> struct RemoveCv;

//! Helper type template of RemoveCv
template <typename T>
using RemoveCvT = typename RemoveCv<T>::Type;

//! Remove 'const' specifiers from the given type
template <typename T> struct RemoveConst;

//!
template <typename T>
using RemoveConstT = typename RemoveConst<T>::Type;

//! Remove 'volatile' specifiers from the given type
template <typename T> struct RemoveVolatile;

//!
template <typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::Type;

//! Add 'lvalue' reference to the given type
template <typename T> struct AddLValueReference;

//!
template <typename T>
using AddLValueReferenceT = typename AddLValueReference<T>::Type;

//! Add 'rvalue' reference to the given type
template <typename T> struct AddRValueReference;

//!
template <typename T>
using AddRValueReferenceT = typename AddRValueReference<T>::Type;

//! Remove a reference from the given type
template <typename T> struct RemoveReference;

//!
template <typename T>
using RemoveReferenceT = typename RemoveReference<T>::Type;

//! Combine RemoveCv and RemoveReference
template <typename T> struct RemoveCvref;

//! Helper type of RemoveCvRef
template <typename T>
using RemoveCvrefT = typename RemoveCvref<T>::Type;

//! Add a pointer to the given type
template <typename T> struct AddPointer;

//!
template <typename T>
using AddPointerT = typename AddPointer<T>::Type;

//! Remove a pointer from the given type
template <typename T> struct RemovePointer;

//!
template <typename T>
using RemovePointerT = typename RemovePointer<T>::Type;

//! Choose one type or another based on compile-time boolean
template <bool kCondition, typename T, typename F> struct Conditional;

//!
template <bool kCondition, typename T, typename F>
using ConditionalT = typename Conditional<kCondition, T, F>::Type;

//! Return the type argument unchanged
template <typename T> struct TypeIdentity;

//!
template <typename T>
using TypeIdentityT = typename TypeIdentity<T>::Type;

// Comparison type

constexpr int32b kSFalse = 0b0; //!< Represent 'false' for scalar operation

constexpr int32b kSTrue = 0b1; //!< Represent 'true' for scalar operation

constexpr int32b kVFalse = 0b0; //!< Represent 'false' for vector operation

constexpr int32b kVTrue = ~0b0; //!< Represent 'true' for vector operation

constexpr int32b kFalse = kSFalse; //!< Represent 'false'

template <typename Type = int32b>
constexpr int32b kTrue = kIsScalar<Type> ? kSTrue : kVTrue; //!< Represent 'true'

//! Integer type that is used as a comparison result of the given type
template <typename T>
using CompResult = ConditionalT<kIsScalar<RemoveCvT<T>>,
                                int32b,
                                IntegerTFromVec<RemoveCvT<T>>>;

// Vector

//! Provide the constant member variable value equal to the the number of elements of a vector T
template <typename T>
inline constexpr size_t kVectorSize = VectorTypeInfo<T>::size();

//! Provide the member variable type equal to the scalar type of a vector T
template <typename T>
using ScalarT = typename VectorTypeInfo<T>::ElementT;

// Address space

//! Provide the Type which is the same as T, except that its address space qualifier is removed
template <typename T>
struct RemoveAddressSpace;

//! Helper type template of RemoveAddressSpace
template <typename T>
using RemoveAddressSpaceT = typename RemoveAddressSpace<T>::Type;

//! Combine RemoveCvRef and RemoveAddressSpace
template <typename T>
struct RemoveCvrefAddress;

//! Helper type template of RemoveCvRefAddress
template <typename T>
using RemoveCvrefAddressT = typename RemoveCvrefAddress<T>::Type;

} // namespace zivc

#include "type_traits-inl.hpp"

#endif /* ZIVC_CL_TYPE_TRAITS_HPP */
