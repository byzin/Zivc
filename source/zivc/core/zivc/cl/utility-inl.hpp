/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_UTILITY_INL_HPP
#define ZIVC_CL_UTILITY_INL_HPP

#include "utility.hpp"
#if defined(ZIVC_CL_CPU)
#include <cstdio>
#include <cstdlib>
#endif // ZIVC_CL_CPU
// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

#if defined(Z_CLANG) || defined(Z_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif // Z_CLANG || Z_GCC

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] format No description.
  \param [in] args No description.
  \return No description
  */
template <typename ...Types> inline
int32b print(ConstConstantPtr<char> format, const Types&... args) noexcept
{
  int32b result = 0;
#if defined(ZIVC_CL_CPU)
  constexpr size_t num_of_args = sizeof...(args);
  if constexpr (0 < num_of_args)
    result = std::printf(format.get(), args...);
  else
    result = std::printf(format.get());
#else // ZIVC_CL_CPU
  static_cast<void>(format);
#endif // ZIVC_CL_CPU
  return result;
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] condition No description.
  \param [in] format No description.
  \param [in] args description.
  */
template <typename ...Types> inline
void assertIfFalse(const int32b condition,
                   ConstConstantPtr<char> format,
                   const Types... args) noexcept
{
  if (!condition) {
#if defined(ZIVC_CL_CPU)
    constexpr size_t num_of_args = sizeof...(args);
    if constexpr (0 < num_of_args)
      std::fprintf(stderr, format.get(), args...);
    else
      std::fprintf(stderr, format.get());
    std::abort();
#else // ZIVC_CL_CPU
    static_cast<void>(format);
#endif // ZIVC_CL_CPU
  }
}

#if defined(Z_CLANG) || defined(Z_GCC)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b getWorkDim() noexcept
{
  const uint dim = get_work_dim();
  return dim;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getGlobalSize(const uint32b dimension) noexcept
{
  const size_t size = get_global_size(dimension);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalSizeX() noexcept
{
  const size_t size = getGlobalSize(0);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalSizeY() noexcept
{
  const size_t size = getGlobalSize(1);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalSizeZ() noexcept
{
  const size_t size = getGlobalSize(2);
  return size;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getGlobalId(const uint32b dimension) noexcept
{
  const size_t id = get_global_id(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalIdX() noexcept
{
  const size_t id = getGlobalId(0);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalIdY() noexcept
{
  const size_t id = getGlobalId(1);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalIdZ() noexcept
{
  const size_t id = getGlobalId(2);
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getLocalSize(const uint32b dimension) noexcept
{
  const size_t size = get_local_size(dimension);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalSizeX() noexcept
{
  const size_t size = getLocalSize(0);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalSizeY() noexcept
{
  const size_t size = getLocalSize(1);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalSizeZ() noexcept
{
  const size_t size = getLocalSize(2);
  return size;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getEnqueuedLocalSize(const uint32b dimension) noexcept
{
  const size_t size = get_enqueued_local_size(dimension);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getEnqueuedLocalSizeX() noexcept
{
  const size_t size = getEnqueuedLocalSize(0);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getEnqueuedLocalSizeY() noexcept
{
  const size_t size = getEnqueuedLocalSize(1);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getEnqueuedLocalSizeZ() noexcept
{
  const size_t size = getEnqueuedLocalSize(2);
  return size;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getLocalId(const uint32b dimension) noexcept
{
  const size_t id = get_local_id(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalIdX() noexcept
{
  const size_t id = getLocalId(0);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalIdY() noexcept
{
  const size_t id = getLocalId(1);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalIdZ() noexcept
{
  const size_t id = getLocalId(2);
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getNumGroups(const uint32b dimension) noexcept
{
  const size_t size = get_num_groups(dimension);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getNumGroupsX() noexcept
{
  const size_t size = getNumGroups(0);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getNumGroupsY() noexcept
{
  const size_t size = getNumGroups(1);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getNumGroupsZ() noexcept
{
  const size_t size = getNumGroups(2);
  return size;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getGroupId(const uint32b dimension) noexcept
{
  const size_t id = get_group_id(dimension);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGroupIdX() noexcept
{
  const size_t id = getGroupId(0);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGroupIdY() noexcept
{
  const size_t id = getGroupId(1);
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGroupIdZ() noexcept
{
  const size_t id = getGroupId(2);
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t getGlobalOffset(const uint32b dimension) noexcept
{
  const size_t offset = get_global_offset(dimension);
  return offset;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalOffsetX() noexcept
{
  const size_t offset = getGlobalOffset(0);
  return offset;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalOffsetY() noexcept
{
  const size_t offset = getGlobalOffset(1);
  return offset;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalOffsetZ() noexcept
{
  const size_t offset = getGlobalOffset(2);
  return offset;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getGlobalLinearId() noexcept
{
  const size_t id = get_global_linear_id();
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
inline
size_t getLocalLinearId() noexcept
{
  const size_t id = get_local_linear_id();
  return id;
}

namespace inner {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
struct MakingVectorImpl 
{
  //! Create a vector
  template <typename ...ArgTypes>
  static constexpr Type make(const ArgTypes&... args) noexcept;
};

} /* namespace inner */

template <typename Type, typename ...ArgTypes> inline
constexpr Type make(const ArgTypes&... args) noexcept
{
  const Type v = inner::MakingVectorImpl<Type>::make(args...);
  return v;
}

/*!
  \def ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL
  \brief No brief description

  No detailed description.

  \param [in] scalar_type No description.
  \param [in] vector_type No description.
  \param [in] vector_func No description.
  */
#define ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(scalar_type, vector_type, vector_func) \
  namespace inner { \
    template <> \
    struct MakingVectorImpl< vector_type ## 2 > \
    { \
      template <typename ...ArgTypes> inline \
      static constexpr vector_type ## 2 make(const ArgTypes&... args) noexcept \
      { \
        return make ## vector_func ## 2(args...); \
      } \
    }; \
    template <> \
    struct MakingVectorImpl< vector_type ## 3 > \
    { \
      template <typename ...ArgTypes> inline \
      static constexpr vector_type ## 3 make(const ArgTypes&... args) noexcept \
      { \
        return make ## vector_func ## 3(args...); \
      } \
    }; \
    template <> \
    struct MakingVectorImpl< vector_type ## 4 > \
    { \
      template <typename ...ArgTypes> inline \
      static constexpr vector_type ## 4 make(const ArgTypes&... args) noexcept \
      { \
        return make ## vector_func ## 4(args...); \
      } \
    }; \
  } \
  inline \
  constexpr vector_type ## 2 make ## vector_func ## 2(const scalar_type v) noexcept \
  { \
    return vector_type ## 2{v, v}; \
  } \
  inline \
  constexpr vector_type ## 2 make ## vector_func ## 2(const scalar_type v0, \
                                                      const scalar_type v1) noexcept \
  { \
    return vector_type ## 2{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 3 make ## vector_func ## 3(const scalar_type v) noexcept \
  { \
    return vector_type ## 3{v, v, v}; \
  } \
  inline \
  constexpr vector_type ## 3 make ## vector_func ## 3(const scalar_type v0, \
                                                      const scalar_type v1, \
                                                      const scalar_type v2) noexcept \
  { \
    return vector_type ## 3{v0, v1, v2}; \
  } \
  inline \
  constexpr vector_type ## 3 make ## vector_func ## 3(const vector_type ## 2 v0, \
                                                      const scalar_type v1) noexcept \
  { \
    return vector_type ## 3{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 3 make ## vector_func ## 3(const scalar_type v0, \
                                                      const vector_type ## 2 v1) noexcept \
  { \
    return vector_type ## 3{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const scalar_type v) noexcept \
  { \
    return vector_type ## 4{v, v, v, v}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const scalar_type v0, \
                                                      const scalar_type v1, \
                                                      const scalar_type v2, \
                                                      const scalar_type v3) noexcept \
  { \
    return vector_type ## 4{v0, v1, v2, v3}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const vector_type ## 2 v0, \
                                                      const scalar_type v1, \
                                                      const scalar_type v2) noexcept \
  { \
    return vector_type ## 4{v0, v1, v2}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const scalar_type v0, \
                                                      const vector_type ## 2 v1, \
                                                      const scalar_type v2) noexcept \
  { \
    return vector_type ## 4{v0, v1, v2}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const scalar_type v0, \
                                                      const scalar_type v1, \
                                                      const vector_type ## 2 v2) noexcept \
  { \
    return vector_type ## 4{v0, v1, v2}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const vector_type ## 2 v0, \
                                                      const vector_type ## 2 v1) noexcept \
  { \
    return vector_type ## 4{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const vector_type ## 3 v0, \
                                                      const scalar_type v1) noexcept \
  { \
    return vector_type ## 4{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 4 make ## vector_func ## 4(const scalar_type v0, \
                                                      const vector_type ## 3 v1) noexcept \
  { \
    return vector_type ## 4{v0, v1}; \
  }

ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(int8b, char, Char)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(uint8b, uchar, UChar)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(int16b, short, Short)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(uint16b, ushort, UShort)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(int32b, int, Int)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(uint32b, uint, UInt)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(int64b, long, Long)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(uint64b, ulong, ULong)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(float, float, Float)
ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(double, double, Double)

namespace inner {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
struct TypeConverter
{
  template <typename T>
  static Type cast(T&& value) noexcept
  {
    return static_cast<Type>(forward<T>(value));
  }
};

#if defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif /* Z_CLANG */

/*!
  \def ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL
  \brief No brief description

  No detailed description.

  \param [in] type No description.
  */
#define ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(type) \
  template <> \
  struct TypeConverter< type > \
  { \
    using ScalarT = ScalarTypeT< type >; \
    static constexpr size_t kDestVectorN = kVectorSize< type >; \
    template <typename T> \
    static type cast(T&& value) noexcept \
    { \
      using SourceT = RemoveCvRefAddressT<T>; \
      constexpr size_t kSourceVectorN = kVectorSize<SourceT>; \
      if constexpr (kSourceVectorN == kDestVectorN) \
        return convert_ ## type (value); \
      else \
        return static_cast< type >(static_cast<ScalarT>(forward<T>(value))); \
    } \
  }

ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double4);

#if defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif /* Z_CLANG */

#if defined(ZIVC_CL_CPU)

template <AddressSpaceType kASpaceType, typename Type>
struct TypeConverter<AddressSpacePointer<kASpaceType, Type>>
{
  using ASpacePointer = AddressSpacePointer<kASpaceType, Type>;

  template <typename T>
  static ASpacePointer cast(AddressSpacePointer<kASpaceType, T> value) noexcept
  {
    return reinterp(value);
  }

  template <typename T>
  static ASpacePointer reinterp(AddressSpacePointer<kASpaceType, T> object) noexcept
  {
    auto data = reinterpret_cast<typename ASpacePointer::Pointer>(object.get());
    return ASpacePointer{data};
  }
};

#endif // ZIVC_CL_CPU

} // namespace inner

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] value No description.
  \return No description
  */
template <typename Type, typename T> inline
Type cast(T&& value) noexcept
{
  if constexpr (kIsSame<Type, T>) {
    return forward<T>(value);
  }
  else {
    using DstType = RemoveAddressSpaceT<RemoveCvT<Type>>;
    auto result = inner::TypeConverter<DstType>::cast(forward<T>(value));
    return result;
  }
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename Type1, typename Type2> inline
constexpr bool equal(const Type1& lhs, const Type2& rhs) noexcept
{
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif // Z_GCC || Z_CLANG
  const bool result = lhs == rhs;
#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] object No description.
  \return No description
  */
template <typename Type, typename T> inline
Type reinterp(T object) noexcept
{
  auto result = inner::TypeConverter<RemoveVolatileType<Type>>::reinterp(object);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] t No description.
  \return No description
  */
template <typename Type> inline
Type&& forward(RemoveReferenceType<Type>& t) noexcept
{
  return static_cast<Type&&>(t);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] t No description.
  \return No description
  */
template <typename Type> inline
Type&& forward(RemoveReferenceType<Type>&& t) noexcept
{
  static_assert(!kIsLValueReference<Type>, "The Type is lvalue reference.");
  return static_cast<Type&&>(t);
}

} // namespace zivc

#endif /* ZIVC_CL_UTILITY_INL_HPP */
