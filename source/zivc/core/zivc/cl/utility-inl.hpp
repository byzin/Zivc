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
// Zisc
#if defined(ZIVC_CL_CPU)
#include "zisc/bit.hpp"
#include "zisc/concepts.hpp"
#endif // ZIVC_CL_CPU
// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] condition No description.
  \param [in] format No description.
  \param [in] args description.
  */
template <typename ...Types> inline
void Utility::assertIfFalse(const bool condition, ConstConstantPtr<char> format, const Types... args)
{
  if (!condition) {
#if defined(ZIVC_CL_CPU)
    ZIVC_CL_GLOBAL_NAMESPACE::assertIfFalse(condition, format.get(), args...);
#else // ZIVC_CL_CPU
    static_cast<void>(format);
#endif // ZIVC_CL_CPU
  }
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] format No description.
  \param [in] args No description.
  \return No description
  */
template <typename ...Types> inline
int32b Utility::print(ConstConstantPtr<char> format, const Types... args)
{
  int32b result = 0;
#if defined(ZIVC_CL_CPU)
  ZIVC_CL_GLOBAL_NAMESPACE::printf(format.get(), args...);
#else // ZIVC_CL_CPU
  static_cast<void>(format);
#endif // ZIVC_CL_CPU
  return result;
}

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
struct Utility::Cast
{
  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] value No description.
    \return No description
    */
  template <typename T>
  static Type cast(T value) noexcept
  {
    return static_cast<Type>(value);
  }
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
struct Utility::Reinterp
{
  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] object No description.
    \return No description
    */
  template <typename T>
  static Type reinterp(T object) noexcept
  {
    return reinterpret_cast<Type>(object);
  }
};

/*!
  \def ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL
  \brief No brief description

  No detailed description.

  \param [in] type No description.
  */
#define ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(type) \
  template <> \
  struct Utility::Cast< type > \
  { \
    template <typename T> \
    static type cast(T value) noexcept \
    { \
      if constexpr (kIsScalar<T>) \
        return static_cast< type >(static_cast<ScalarT< type >>(value)); \
      else \
        return convert_ ## type (value); \
    } \
  }

ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(char16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uchar16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(short16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ushort16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(int16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(uint16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(long16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(ulong16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(float16);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double2);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double3);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double4);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double8);
ZIVC_TYPE_CONVERTER_SPECIALIZATION_IMPL(double16);

#if defined(ZIVC_CL_CPU)

/*!
  \brief Specialization of Reinterp

  No detailed description.

  \tparam kASpaceType No description.
  \tparam Type No description.
  */
template <AddressSpaceType kASpaceType, typename Type>
struct Utility::Reinterp<AddressSpacePointer<kASpaceType, Type>>
{
  using ASpacePointer = AddressSpacePointer<kASpaceType, Type>;

  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] object No description.
    \return No description
    */
  template <zisc::Pointer T>
  static ASpacePointer reinterp(T object) noexcept
  {
    auto* data = reinterpret_cast<typename ASpacePointer::Pointer>(object);
    return ASpacePointer{data};
  }

  /*!
    \details No detailed description

    \tparam T No description.
    \param [in] object No description.
    \return No description
    */
  template <typename T>
  static ASpacePointer reinterp(AddressSpacePointer<kASpaceType, T> object) noexcept
  {
    auto* data = reinterpret_cast<typename ASpacePointer::Pointer>(object.get());
    return ASpacePointer{data};
  }
};

#endif // ZIVC_CL_CPU

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] value No description.
  \return No description
  */
template <typename Type, typename T> inline
Type Utility::cast(T value) noexcept
{
  using SrcT = RemoveCvT<T>;
  using DstT = RemoveCvT<Type>;
  if constexpr (kIsSame<DstT, SrcT>)
    return value;
  else
    return Cast<DstT>::cast(value);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] object No description.
  \return No description
  */
template <typename Type, typename T> inline
Type Utility::reinterp(T object) noexcept
{
  return Reinterp<RemoveVolatileT<Type>>::reinterp(object);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in] flag No description.
  \param [out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
bool Utility::updateIfTrue(const bool flag, AddressSpacePointer lhs, Type rhs) noexcept
{
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The lhs isn't pointer.");
  if (flag)
    *lhs = rhs;
  return flag;
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] condition No description.
  \param [in] format No description.
  \param [in] args description.
  */
template <typename ...Types> inline
void assertIfFalse(const bool condition, ConstConstantPtr<char> format, const Types... args)
{
  Utility::assertIfFalse(condition, format, args...);
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] format No description.
  \param [in] args No description.
  \return No description
  */
template <typename ...Types> inline
int32b print(ConstConstantPtr<char> format, const Types... args)
{
  return Utility::print(format, args...);
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam T No description.
  \param [in] value No description.
  \return No description
  */
template <typename Type, typename T> inline
Type cast(T value) noexcept
{
  return Utility::cast<Type, T>(value);
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
  return Utility::reinterp<Type, T>(object);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \tparam Type No description.
  \param [in] flag No description.
  \param [out] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename AddressSpacePointer, typename Type> inline
bool updateIfTrue(const bool flag, AddressSpacePointer lhs, Type rhs) noexcept
{
  return Utility::updateIfTrue(flag, lhs, rhs);
}

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

/*!
  \def ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL
  \brief No brief description

  No detailed description.

  \param [in] scalar_type No description.
  \param [in] vector_type No description.
  \param [in] vector_func No description.
  */
#define ZIVC_MAKING_VECTOR_SPECIALIZATION_IMPL(scalar_type, vector_type, vector_func) \
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
  } \
  inline \
  constexpr vector_type ## 8 make ## vector_func ## 8(const scalar_type v) noexcept \
  { \
    return vector_type ## 8{v, v, v, v, v, v, v, v}; \
  } \
  inline \
  constexpr vector_type ## 8 make ## vector_func ## 8(const scalar_type v0, \
                                                      const scalar_type v1, \
                                                      const scalar_type v2, \
                                                      const scalar_type v3, \
                                                      const scalar_type v4, \
                                                      const scalar_type v5, \
                                                      const scalar_type v6, \
                                                      const scalar_type v7) noexcept \
  { \
    return vector_type ## 8{v0, v1, v2, v3, v4, v5, v6, v7}; \
  } \
  inline \
  constexpr vector_type ## 8 make ## vector_func ## 8(const vector_type ## 2 v0, \
                                                      const vector_type ## 2 v1, \
                                                      const vector_type ## 2 v2, \
                                                      const vector_type ## 2 v3) noexcept \
  { \
    return vector_type ## 8{v0, v1, v2, v3}; \
  } \
  inline \
  constexpr vector_type ## 8 make ## vector_func ## 8(const vector_type ## 4 v0, \
                                                      const vector_type ## 4 v1) noexcept \
  { \
    return vector_type ## 8{v0, v1}; \
  } \
  inline \
  constexpr vector_type ## 16 make ## vector_func ## 16(const scalar_type v) noexcept \
  { \
    return vector_type ## 16{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; \
  } \
  inline \
  constexpr vector_type ## 16 make ## vector_func ## 16(const scalar_type v0, \
                                                        const scalar_type v1, \
                                                        const scalar_type v2, \
                                                        const scalar_type v3, \
                                                        const scalar_type v4, \
                                                        const scalar_type v5, \
                                                        const scalar_type v6, \
                                                        const scalar_type v7, \
                                                        const scalar_type v8, \
                                                        const scalar_type v9, \
                                                        const scalar_type v10, \
                                                        const scalar_type v11, \
                                                        const scalar_type v12, \
                                                        const scalar_type v13, \
                                                        const scalar_type v14, \
                                                        const scalar_type v15) noexcept \
  { \
    return vector_type ## 16{v0, v1, v2, v3, v4, v5, v6, v7, \
                             v8, v9, v10, v11, v12, v13, v14, v15}; \
  } \
  inline \
  constexpr vector_type ## 16 make ## vector_func ## 16(const vector_type ## 2 v0, \
                                                        const vector_type ## 2 v1, \
                                                        const vector_type ## 2 v2, \
                                                        const vector_type ## 2 v3, \
                                                        const vector_type ## 2 v4, \
                                                        const vector_type ## 2 v5, \
                                                        const vector_type ## 2 v6, \
                                                        const vector_type ## 2 v7) noexcept \
  { \
    return vector_type ## 16{v0, v1, v2, v3, v4, v5, v6, v7}; \
  } \
  inline \
  constexpr vector_type ## 16 make ## vector_func ## 16(const vector_type ## 4 v0, \
                                                        const vector_type ## 4 v1, \
                                                        const vector_type ## 4 v2, \
                                                        const vector_type ## 4 v3) noexcept \
  { \
    return vector_type ## 16{v0, v1, v2, v3}; \
  } \
  inline \
  constexpr vector_type ## 16 make ## vector_func ## 16(const vector_type ## 8 v0, \
                                                        const vector_type ## 8 v1) noexcept \
  { \
    return vector_type ## 16{v0, v1}; \
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

} // namespace zivc

#endif /* ZIVC_CL_UTILITY_INL_HPP */
