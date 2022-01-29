/*!
  \file type_traits-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPE_TRAITS_INL_HPP
#define ZIVC_CL_TYPE_TRAITS_INL_HPP

#include "type_traits.hpp"
// Zivc
#include "types.hpp"

namespace zivc {

/*!
  No detailed description.

  \tparam T No description.
  \tparam v No description.
  */
template <typename T, T v>
struct IntegralConstant
{
  // Type aliases
  using ValueType = T;
  using Type = IntegralConstant;


  static constexpr ValueType kValue = v; //!< Static constant of type T with value v


  //! Return the wrapped value
  constexpr operator ValueType() const noexcept
  {
    return kValue;
  }

  //! Return the wrapped value
  constexpr ValueType operator()() const noexcept
  {
    return kValue;
  }
};

/*!
  No detailed description.

  \tparam T No description.
  \tparam U No description.
  */
template <typename T, typename U>
struct IsSame : public FalseType
{
};

/*!
  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSame<T, T> : public TrueType
{
};

#define ZIVC_SINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(IType, bytes) \
  template <> \
  struct IntegerFromBytes< bytes > \
  { \
    using Type = IType; \
    static_assert(sizeof(IType) == bytes, "The size of IType is wrong."); \
  }

ZIVC_SINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(int8b, 1);
ZIVC_SINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(int16b, 2);
ZIVC_SINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(int32b, 4);
ZIVC_SINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(int64b, 8);

#define ZIVC_UINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(UType, bytes) \
  template <> \
  struct UIntegerFromBytes< bytes > \
  { \
    using Type = UType; \
    static_assert(sizeof(UType) == bytes, "The size of UType is wrong."); \
  }

ZIVC_UINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(uint8b, 1);
ZIVC_UINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(uint16b, 2);
ZIVC_UINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(uint32b, 4);
ZIVC_UINTEGER_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(uint64b, 8);

#define ZIVC_FLOAT_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(FType, bytes) \
  template <> \
  struct FloatFromBytes< bytes > \
  { \
    using Type = FType; \
    static_assert(sizeof(FType) == bytes, "The size of FType is wrong."); \
  }

ZIVC_FLOAT_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(half, 2);
ZIVC_FLOAT_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(float, 4);
ZIVC_FLOAT_FROM_BYTES_TEMPLATE_SPECIALIZATION_IMPL(double, 8);

#define ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(VecType, ElemType, n) \
  template <> \
  struct VectorTypeInfo< VecType > \
  { \
    using ElementType = ElemType; \
    static constexpr size_t size() noexcept {return n;} \
    static_assert(n <= sizeof( VecType ) / sizeof( ElemType ), \
                  "The number of elements is wrong."); \
  }; \
  template <> \
  struct VectorFromElems< ElemType, n > \
  { \
    using Type = VecType; \
    static_assert(n <= sizeof( VecType ) / sizeof( ElemType ), \
                  "The number of elements is wrong."); \
  }

ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int8b, int8b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char2, int8b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char3, int8b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char4, int8b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int16b, int16b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short2, int16b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short3, int16b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short4, int16b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int32b, int32b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int2, int32b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int3, int32b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int4, int32b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int64b, int64b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long2, int64b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long3, int64b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long4, int64b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint8b, uint8b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar2, uint8b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar3, uint8b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar4, uint8b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint16b, uint16b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort2, uint16b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort3, uint16b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort4, uint16b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint32b, uint32b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint2, uint32b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint3, uint32b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint4, uint32b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint64b, uint64b, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong2, uint64b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong3, uint64b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong4, uint64b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half, half, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half2, half, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half3, half, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half4, half, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float, float, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float2, float, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float3, float, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float4, float, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double, double, 1);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double2, double, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double3, double, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double4, double, 4);

namespace inner {

template <typename T>
struct AddressSpaceInfoHelper
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultTrue<int32b>;
  static constexpr int32b kIsPointer = kResultFalse;
};

#if defined(ZIVC_CL_CPU)

template <AddressSpaceType kASpaceType, typename T>
struct AddressSpaceInfoHelper<AddressSpacePointer<kASpaceType, T>>
{
  using DataType = RemoveCvType<T>;
  static constexpr int32b kIsGlobal = (kASpaceType == AddressSpaceType::kGlobal)
      ? kResultTrue<int32b>
      : kResultFalse;
  static constexpr int32b kIsLocal = (kASpaceType == AddressSpaceType::kLocal)
      ? kResultTrue<int32b>
      : kResultFalse;
  static constexpr int32b kIsConstant = (kASpaceType == AddressSpaceType::kConstant)
      ? kResultTrue<int32b>
      : kResultFalse;
  static constexpr int32b kIsPrivate = (kASpaceType == AddressSpaceType::kPrivate)
      ? kResultTrue<int32b>
      : kResultFalse;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

#else // ZIVC_CL_CPU

template <typename T>
struct AddressSpaceInfoHelper<Global<T>>
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultTrue<int32b>;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultFalse;
};

template <typename T>
struct AddressSpaceInfoHelper<GlobalPtr<T>>
{
  using DataType = RemoveCvType<RemovePointerType<T>>;
  static constexpr int32b kIsGlobal = kResultTrue<int32b>;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

template <typename T>
struct AddressSpaceInfoHelper<Local<T>>
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultTrue<int32b>;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultFalse;
};

template <typename T>
struct AddressSpaceInfoHelper<LocalPtr<T>>
{
  using DataType = RemoveCvType<RemovePointerType<T>>;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultTrue<int32b>;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

template <typename T>
struct AddressSpaceInfoHelper<Constant<T>>
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultTrue<int32b>;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultFalse;
};

template <typename T>
struct AddressSpaceInfoHelper<ConstantPtr<T>>
{
  using DataType = RemoveCvType<RemovePointerType<T>>;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultTrue<int32b>;
  static constexpr int32b kIsPrivate = kResultFalse;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

template <typename T>
struct AddressSpaceInfoHelper<Private<T>>
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultTrue<int32b>;
  static constexpr int32b kIsPointer = kResultFalse;
};

template <typename T>
struct AddressSpaceInfoHelper<PrivatePtr<T>>
{
  using DataType = RemoveCvType<RemovePointerType<T>>;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultTrue<int32b>;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

template <typename T>
struct AddressSpaceInfoHelper<Generic<T>>
{
  using DataType = T;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultTrue<int32b>;
  static constexpr int32b kIsPointer = kResultFalse;
};

template <typename T>
struct AddressSpaceInfoHelper<GenericPtr<T>>
{
  using DataType = RemoveCvType<RemovePointerType<T>>;
  static constexpr int32b kIsGlobal = kResultFalse;
  static constexpr int32b kIsLocal = kResultFalse;
  static constexpr int32b kIsConstant = kResultFalse;
  static constexpr int32b kIsPrivate = kResultTrue<int32b>;
  static constexpr int32b kIsPointer = kResultTrue<int32b>;
};

#endif // ZIVC_CL_CPU

} // namespace inner

template <typename T>
class AddressSpaceInfo
{
  using ADataType = RemoveCvType<RemovePointerType<T>>;
  using ASpaceInfo = inner::AddressSpaceInfoHelper<ADataType>;

 public:
  using DataType = typename ASpaceInfo::DataType;

  //! Check if the Type is global address space type
  static constexpr int32b isGlobal() noexcept
  {
    return ASpaceInfo::kIsGlobal;
  }

  //! Check if the Type is local address space type
  static constexpr int32b isLocal() noexcept
  {
    return ASpaceInfo::kIsLocal;
  }

  //! Check if the Type is constant address space type
  static constexpr int32b isConstant() noexcept
  {
    return ASpaceInfo::kIsConstant;
  }

  //! Check if the Type is private address space type
  static constexpr int32b isPrivate() noexcept
  {
    return ASpaceInfo::kIsPrivate;
  }

  //! Check if given address space type is pointer
  static constexpr int32b isPointer() noexcept
  {
    const int32b is_pointer = kIsPointer<T> | ASpaceInfo::kIsPointer;
    return is_pointer;
  }
};

namespace inner {

template <typename T>
struct IsSignedIntegerHelper
{
  static constexpr int32b kValue = kResultFalse;
};

#define ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSignedIntegerHelper< Type > \
  { \
    static constexpr int32b kValue = kResultTrue<int32b>; \
  }

ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int8b);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char2);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char3);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char4);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int16b);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short2);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short3);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short4);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int32b);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int2);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int3);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int4);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int64b);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long2);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long3);
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long4);

template <typename T>
struct IsUnsignedIntegerHelper
{
  static constexpr int32b kValue = kResultFalse;
};

#define ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsUnsignedIntegerHelper< Type > \
  { \
    static constexpr int32b kValue = kResultTrue<int32b>; \
  }

ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint8b);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar2);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar3);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar4);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint16b);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort2);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort3);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort4);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint32b);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint2);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint3);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint4);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint64b);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong2);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong3);
ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong4);

template <typename T>
struct IsHalfHelper
{
  static constexpr int32b kValue = kResultFalse;
};

#define ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsHalfHelper< Type > \
  { \
    static constexpr int32b kValue = kResultTrue<int32b>; \
  }

ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half2);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half3);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half4);

template <typename T>
struct IsSingleFloatHelper
{
  static constexpr int32b kValue = kResultFalse;
};

#define ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSingleFloatHelper< Type > \
  { \
    static constexpr int32b kValue = kResultTrue<int32b>; \
  }

ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float2);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float3);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float4);

template <typename T>
struct IsDoubleHelper
{
  static constexpr int32b kValue = kResultFalse;
};

#define ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsDoubleHelper< Type > \
  { \
    static constexpr int32b kValue = kResultTrue<int32b>; \
  }

ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double2);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double3);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double4);

template <typename T>
struct IsPointerHelper
{
  static constexpr int32b kValue = kResultFalse;
};

template <typename T>
struct IsPointerHelper<T*>
{
  static constexpr int32b kValue = kResultTrue<int32b>;
};

} // namespace inner

template <typename T>
struct IsSignedInteger
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsSignedIntegerHelper<Type>::kValue;
};

template <typename T>
struct IsUnsignedInteger
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsUnsignedIntegerHelper<Type>::kValue;
};

template <typename T>
struct IsInteger
{
  static constexpr int32b kValue = kIsSignedInteger<T> | kIsUnsignedInteger<T>;
};

template <typename T>
struct IsHalf
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsHalfHelper<Type>::kValue;
};

template <typename T>
struct IsSingleFloat
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsSingleFloatHelper<Type>::kValue;
};

template <typename T>
struct IsDouble
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsDoubleHelper<Type>::kValue;
};

template <typename T>
struct IsFloatingPoint
{
  static constexpr int32b kValue = kIsHalf<T> | kIsSingleFloat<T> | kIsDouble<T>;
};

template <typename T>
struct IsArithmetic
{
  static constexpr int32b kValue = kIsInteger<T> | kIsFloatingPoint<T>;
};

template <typename T>
struct IsSigned
{
  static constexpr int32b kValue = kIsSignedInteger<T> | kIsFloatingPoint<T>;
};

template <typename T>
struct IsPointer
{
  static constexpr int32b kValue = inner::IsPointerHelper<RemoveCvType<T>>::kValue;
};

template <typename T>
struct IsLValueReference
{
  static constexpr int32b kValue = kResultFalse;
};

template <typename T>
struct IsLValueReference<T&>
{
  static constexpr int32b kValue = kResultTrue<int32b>;
};

template <typename T>
struct IsRValueReference
{
  static constexpr int32b kValue = kResultFalse;
};

template <typename T>
struct IsRValueReference<T&&>
{
  static constexpr int32b kValue = kResultTrue<int32b>;
};

template <typename T>
struct RemoveConst
{
  using Type = T;
};

template <typename T>
struct AddConst
{
  using Type = const T;
};

template <typename T>
struct AddVolatile
{
  using Type = volatile T;
};

template <typename T>
struct AddCv
{
  using Type = AddConstType<AddVolatileType<T>>;
};

template <typename T>
struct RemoveConst<const T>
{
  using Type = T;
};

template <typename T>
struct RemoveVolatile
{
  using Type = T;
};

template <typename T>
struct RemoveVolatile<volatile T>
{
  using Type = T;
};

template <typename T>
struct RemoveCv
{
  using Type = RemoveConstType<RemoveVolatileType<T>>;
};

template <typename T>
struct AddLValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&;
};

template <typename T>
struct AddRValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&&;
};

template <typename T>
struct RemoveReference
{
  using Type = T;
};

template <typename T>
struct RemoveReference<T&>
{
  using Type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
  using Type = T;
};

template <typename T>
struct RemoveCvref
{
  using Type = RemoveCvType<RemoveReferenceType<T>>;
};

template <typename T>
struct AddPointer
{
  using NoPointerType = RemovePointerType<RemoveReferenceType<T>>;
  using Type = NoPointerType*;
};

template <typename T>
struct RemovePointer
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T*>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* const>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* volatile>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* const volatile>
{
  using Type = T;
};

template <typename T, typename F>
struct Conditional<true, T, F>
{
  using Type = T;
};

template <typename T, typename F>
struct Conditional<false, T, F>
{
  using Type = F;
};

template <typename T>
struct TypeIdentity
{
  using Type = T;
};

} // namespace zivc

#endif /* ZIVC_CL_TYPE_TRAITS_INL_HPP */
