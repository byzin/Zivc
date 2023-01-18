/*!
  \file type_traits-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
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
    using Type = VecType; \
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

ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char2, int8b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char3, int8b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char4, int8b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short2, int16b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short3, int16b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short4, int16b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int2, int32b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int3, int32b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int4, int32b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long2, int64b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long3, int64b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long4, int64b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar2, uint8b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar3, uint8b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar4, uint8b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort2, uint16b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort3, uint16b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort4, uint16b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint2, uint32b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint3, uint32b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint4, uint32b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong2, uint64b, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong3, uint64b, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong4, uint64b, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half2, half, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half3, half, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half4, half, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float2, float, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float3, float, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float4, float, 4);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double2, double, 2);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double3, double, 3);
ZIVC_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double4, double, 4);

namespace inner {

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper
{
  using DataType = T;
  using Pointer = PrivatePtr<DataType>;
  using ConstPointer = ConstPrivatePtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSTrue;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSFalse;
};

#if defined(ZIVC_CL_CPU)

/*!
  \brief No brief description

  No detailed description.

  \tparam kASpaceType No description.
  \tparam T No description.
  */
template <AddressSpaceType kASpaceType, typename T>
struct AddressSpaceInfoHelper<AddressSpacePointer<kASpaceType, T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = AddressSpacePointer<kASpaceType, DataType>;
  using ConstPointer = AddressSpacePointer<kASpaceType, const DataType>;
  static constexpr int32b kIsGlobal = (kASpaceType == AddressSpaceType::kGlobal)
      ? kSTrue
      : kSFalse;
  static constexpr int32b kIsLocal = (kASpaceType == AddressSpaceType::kLocal)
      ? kSTrue
      : kSFalse;
  static constexpr int32b kIsConstant = (kASpaceType == AddressSpaceType::kConstant)
      ? kSTrue
      : kSFalse;
  static constexpr int32b kIsPrivate = (kASpaceType == AddressSpaceType::kPrivate)
      ? kSTrue
      : kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSTrue;
};

#else // ZIVC_CL_CPU

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<Global<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = GlobalPtr<DataType>;
  using ConstPointer = ConstGlobalPtr<DataType>;
  static constexpr int32b kIsGlobal = kSTrue;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<GlobalPtr<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = GlobalPtr<DataType>;
  using ConstPointer = ConstGlobalPtr<DataType>;
  static constexpr int32b kIsGlobal = kSTrue;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<Local<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = LocalPtr<DataType>;
  using ConstPointer = ConstLocalPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSTrue;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<LocalPtr<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = LocalPtr<DataType>;
  using ConstPointer = ConstLocalPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSTrue;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<Constant<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = ConstantPtr<DataType>;
  using ConstPointer = ConstConstantPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSTrue;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<ConstantPtr<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = ConstantPtr<DataType>;
  using ConstPointer = ConstConstantPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSTrue;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<Private<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = PrivatePtr<DataType>;
  using ConstPointer = ConstPrivatePtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSTrue;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<PrivatePtr<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = PrivatePtr<DataType>;
  using ConstPointer = ConstPrivatePtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSTrue;
  static constexpr int32b kIsGeneric = kSFalse;
  static constexpr int32b kIsPointer = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<Generic<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = GenericPtr<DataType>;
  using ConstPointer = ConstGenericPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSTrue;;
  static constexpr int32b kIsPointer = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddressSpaceInfoHelper<GenericPtr<T>>
{
  using DataType = RemoveCvType<T>;
  using Pointer = GenericPtr<DataType>;
  using ConstPointer = ConstGenericPtr<DataType>;
  static constexpr int32b kIsGlobal = kSFalse;
  static constexpr int32b kIsLocal = kSFalse;
  static constexpr int32b kIsConstant = kSFalse;
  static constexpr int32b kIsPrivate = kSFalse;
  static constexpr int32b kIsGeneric = kSTrue;;
  static constexpr int32b kIsPointer = kSTrue;
};

#endif // ZIVC_CL_CPU

} // namespace inner

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
class AddressSpaceInfo
{
  using ADataType = RemoveCvType<RemovePointerType<T>>;
  using ASpaceInfo = inner::AddressSpaceInfoHelper<ADataType>;

 public:
  using DataType = typename ASpaceInfo::DataType;
  using Pointer = typename ASpaceInfo::Pointer;
  using ConstPointer = typename ASpaceInfo::ConstPointer;

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

  //! Check if the Type is generic address space type
  static constexpr int32b isGeneric() noexcept
  {
    return ASpaceInfo::kIsGeneric;
  }

  //! Check if given address space type is pointer
  static constexpr int32b isPointer() noexcept
  {
    const int32b is_pointer = kIsPointer<T> | ASpaceInfo::kIsPointer;
    return is_pointer;
  }
};

namespace inner {

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSignedIntegerHelper
{
  static constexpr int32b kValue = kSFalse;
};

#define ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSignedIntegerHelper< Type > \
  { \
    static constexpr int32b kValue = kSTrue; \
  }

ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int8b);
#if defined(ZIVC_CL_CPU)
ZIVC_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char);
#endif // ZIVC_CL_CPU
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

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsUnsignedIntegerHelper
{
  static constexpr int32b kValue = kSFalse;
};

#define ZIVC_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsUnsignedIntegerHelper< Type > \
  { \
    static constexpr int32b kValue = kSTrue; \
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

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsHalfHelper
{
  static constexpr int32b kValue = kSFalse;
};

#define ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsHalfHelper< Type > \
  { \
    static constexpr int32b kValue = kSTrue; \
  }

ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half2);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half3);
ZIVC_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half4);

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSingleHelper
{
  static constexpr int32b kValue = kSFalse;
};

#define ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSingleHelper< Type > \
  { \
    static constexpr int32b kValue = kSTrue; \
  }

ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float2);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float3);
ZIVC_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float4);

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsDoubleHelper
{
  static constexpr int32b kValue = kSFalse;
};

#define ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsDoubleHelper< Type > \
  { \
    static constexpr int32b kValue = kSTrue; \
  }

ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double2);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double3);
ZIVC_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double4);

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsPointerHelper
{
  static constexpr int32b kValue = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsPointerHelper<T*>
{
  static constexpr int32b kValue = kSTrue;
};

} // namespace inner

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSignedInteger
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsSignedIntegerHelper<Type>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsUnsignedInteger
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsUnsignedIntegerHelper<Type>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsInteger
{
  static constexpr int32b kValue = kIsSignedInteger<T> | kIsUnsignedInteger<T>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsHalf
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsHalfHelper<Type>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSingle
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsSingleHelper<Type>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsDouble
{
  using Type = RemoveCvType<T>;
  static constexpr int32b kValue = inner::IsDoubleHelper<Type>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsFloat
{
  static constexpr int32b kValue = kIsHalf<T> | kIsSingle<T> | kIsDouble<T>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsArithmetic
{
  static constexpr int32b kValue = kIsInteger<T> | kIsFloat<T>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsSigned
{
  static constexpr int32b kValue = kIsSignedInteger<T> | kIsFloat<T>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsPointer
{
  static constexpr int32b kValue = inner::IsPointerHelper<RemoveCvType<T>>::kValue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsScalar
{
  static constexpr int32b kValue = (VectorTypeInfo<RemoveCvType<T>>::size() == 1)
      ? kSTrue
      : kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsVector
{
  static constexpr int32b kValue = (1 < VectorTypeInfo<RemoveCvType<T>>::size())
      ? kSTrue
      : kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsLValueReference
{
  static constexpr int32b kValue = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsLValueReference<T&>
{
  static constexpr int32b kValue = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsRValueReference
{
  static constexpr int32b kValue = kSFalse;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct IsRValueReference<T&&>
{
  static constexpr int32b kValue = kSTrue;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveConst
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddConst
{
  using Type = const T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddVolatile
{
  using Type = volatile T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddCv
{
  using Type = AddConstType<AddVolatileType<T>>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveConst<const T>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveVolatile
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveVolatile<volatile T>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveCv
{
  using Type = RemoveConstType<RemoveVolatileType<T>>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddLValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddRValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&&;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveReference
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveReference<T&>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveReference<T&&>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveCvref
{
  using Type = RemoveCvType<RemoveReferenceType<T>>;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct AddPointer
{
  using NoPointerType = RemovePointerType<RemoveReferenceType<T>>;
  using Type = NoPointerType*;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemovePointer
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemovePointer<T*>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemovePointer<T* const>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemovePointer<T* volatile>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemovePointer<T* const volatile>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T, typename F>
struct Conditional<true, T, F>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T, typename F>
struct Conditional<false, T, F>
{
  using Type = F;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct TypeIdentity
{
  using Type = T;
};

/*!
  \brief Provide the Type which is the same as T, except that its address space qualifier is removed

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace
{
  using Type = T;
};

#if defined(ZIVC_CL_VULKAN)

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace<Global<T>>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace<Local<T>>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace<Constant<T>>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace<Private<T>>
{
  using Type = T;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveAddressSpace<Generic<T>>
{
  using Type = T;
};

#endif /* ZIVC_CL_VULKAN */

/*!
  \brief Combine RemoveCvRef and RemoveAddressSpace

  No detailed description.

  \tparam T No description.
  */
template <typename T>
struct RemoveCvRefAddress
{
  using Type = RemoveAddressSpaceT<RemoveCvRefT<T>>;
};

} // namespace zivc

#endif /* ZIVC_CL_TYPE_TRAITS_INL_HPP */
