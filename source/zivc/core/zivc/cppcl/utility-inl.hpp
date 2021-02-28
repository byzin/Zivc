/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_UTILITY_INL_HPP
#define ZIVC_CL_UTILITY_INL_HPP

#include "utility.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "types.hpp"
#include "vector.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

namespace inner {

/*!
  \brief No brief description

  No detailed description.
  */
class WorkItem
{
 public:
  //! Return the number of dimensions in use
  static uint32b getDimension() noexcept;

  //! Return the 3d global offset used in global id calculation
  static size_t getGlobalIdOffset(const uint32b dimension) noexcept;

  //! Return the number of work-groups for dimension
  static size_t getNumOfGroups(const uint32b dimension) noexcept;

  //! Return the work-group ID for dimension
  static size_t getWorkGroupId(const uint32b dimension) noexcept;

  //! Set the number of dimensions in use
  static void setDimension(const uint32b dimension) noexcept;

  //! Set the 3d global offset used in global id calculation
  static void setGlobalIdOffset(const std::array<uint32b, 3>& offset) noexcept;

  //! Set the number of work-grous
  static void setNumOfGroups(const std::array<uint32b, 3>& size) noexcept;

  //! Set the work-group ID
  static void setWorkGroupId(const uint32b id) noexcept;

 private:
  static thread_local uint32b dimension_;
  static thread_local std::array<uint32b, 3> global_id_offset_;
  static thread_local std::array<uint32b, 3> num_of_work_groups_;
  static thread_local std::array<uint32b, 3> work_group_id_;
};

} // namespace inner

/*!
  \details No detailed description

  \param [in] format No description.
  \param [in] value No description.
  */
template <typename Type> inline
void printf(const char* format, const Type& value) noexcept
{
  std::printf(format, value);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_enqueued_local_size([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_id(const uint32b dimension) noexcept
{
  const auto id = get_group_id(dimension) + get_global_offset(dimension);
  return id;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_offset(const uint32b dimension) noexcept
{
  return inner::WorkItem::getGlobalIdOffset(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_global_size(const uint32b dimension) noexcept
{
  return get_num_groups(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_group_id(const uint32b dimension) noexcept
{
  return inner::WorkItem::getWorkGroupId(dimension);
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_id([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_linear_id([[maybe_unused]] const uint32b dimension) noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
constexpr size_t get_local_size([[maybe_unused]] const uint32b dimension) noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] dimension No description.
  \return No description
  */
inline
size_t get_num_groups(const uint32b dimension) noexcept
{
  return inner::WorkItem::getNumOfGroups(dimension);
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b get_work_dim() noexcept
{
  return inner::WorkItem::getDimension();
}

namespace inner {

/*!
  */
template <typename Type, typename T, size_t kN> inline
auto convertToVector(const Vector<Type, kN>& value)
{
  Vector<T, kN> result;
  for (size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<T>(value[index]);
  return result;
}

} // namespace inner

/*!
  */
template <typename Type> inline
int8b convert_char(Type&& value) noexcept
{
  return zisc::cast<int8b>(value);
}

/*!
  */
template <typename Type> inline
char2 convert_char2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, int8b, 2>(value);
}

/*!
  */
template <typename Type> inline
char3 convert_char3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, int8b, 3>(value);
}

/*!
  */
template <typename Type> inline
char4 convert_char4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, int8b, 4>(value);
}

/*!
  */
template <typename Type> inline
uint8b convert_uchar(Type&& value) noexcept
{
  return zisc::cast<uint8b>(value);
}

/*!
  */
template <typename Type> inline
uchar2 convert_uchar2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, uint8b, 2>(value);
}

/*!
  */
template <typename Type> inline
uchar3 convert_uchar3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, uint8b, 3>(value);
}

/*!
  */
template <typename Type> inline
uchar4 convert_uchar4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, uint8b, 4>(value);
}

/*!
  */
template <typename Type> inline
int16b convert_short(Type&& value) noexcept
{
  return zisc::cast<int16b>(value);
}

/*!
  */
template <typename Type> inline
short2 convert_short2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, int16b, 2>(value);
}

/*!
  */
template <typename Type> inline
short3 convert_short3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, int16b, 3>(value);
}

/*!
  */
template <typename Type> inline
short4 convert_short4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, int16b, 4>(value);
}

/*!
  */
template <typename Type> inline
uint16b convert_ushort(Type&& value) noexcept
{
  return zisc::cast<uint16b>(value);
}

/*!
  */
template <typename Type> inline
ushort2 convert_ushort2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, uint16b, 2>(value);
}

/*!
  */
template <typename Type> inline
ushort3 convert_ushort3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, uint16b, 3>(value);
}

/*!
  */
template <typename Type> inline
ushort4 convert_ushort4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, uint16b, 4>(value);
}

/*!
  */
template <typename Type> inline
int32b convert_int(Type&& value) noexcept
{
  return zisc::cast<int32b>(value);
}

/*!
  */
template <typename Type> inline
int2 convert_int2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, int32b, 2>(value);
}

/*!
  */
template <typename Type> inline
int3 convert_int3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, int32b, 3>(value);
}

/*!
  */
template <typename Type> inline
int4 convert_int4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, int32b, 4>(value);
}

/*!
  */
template <typename Type> inline
uint32b convert_uint(Type&& value) noexcept
{
  return zisc::cast<uint32b>(value);
}

/*!
  */
template <typename Type> inline
uint2 convert_uint2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, uint32b, 2>(value);
}

/*!
  */
template <typename Type> inline
uint3 convert_uint3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, uint32b, 3>(value);
}

/*!
  */
template <typename Type> inline
uint4 convert_uint4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, uint32b, 4>(value);
}

/*!
  */
template <typename Type> inline
int64b convert_long(Type&& value) noexcept
{
  return zisc::cast<int64b>(value);
}

/*!
  */
template <typename Type> inline
long2 convert_long2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, int64b, 2>(value);
}

/*!
  */
template <typename Type> inline
long3 convert_long3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, int64b, 3>(value);
}

/*!
  */
template <typename Type> inline
long4 convert_long4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, int64b, 4>(value);
}

/*!
  */
template <typename Type> inline
uint64b convert_ulong(Type&& value) noexcept
{
  return zisc::cast<uint64b>(value);
}

/*!
  */
template <typename Type> inline
ulong2 convert_ulong2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, uint64b, 2>(value);
}

/*!
  */
template <typename Type> inline
ulong3 convert_ulong3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, uint64b, 3>(value);
}

/*!
  */
template <typename Type> inline
ulong4 convert_ulong4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, uint64b, 4>(value);
}

/*!
  */
template <typename Type> inline
float convert_float(Type&& value) noexcept
{
  return zisc::cast<float>(value);
}

/*!
  */
template <typename Type> inline
float2 convert_float2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, float, 2>(value);
}

/*!
  */
template <typename Type> inline
float3 convert_float3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, float, 3>(value);
}

/*!
  */
template <typename Type> inline
float4 convert_float4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, float, 4>(value);
}

/*!
  */
template <typename Type> inline
double convert_double(Type&& value) noexcept
{
  return zisc::cast<double>(value);
}

/*!
  */
template <typename Type> inline
double2 convert_double2(const Vector<Type, 2>& value) noexcept
{
  return inner::convertToVector<Type, double, 2>(value);
}

/*!
  */
template <typename Type> inline
double3 convert_double3(const Vector<Type, 3>& value) noexcept
{
  return inner::convertToVector<Type, double, 3>(value);
}

/*!
  */
template <typename Type> inline
double4 convert_double4(const Vector<Type, 4>& value) noexcept
{
  return inner::convertToVector<Type, double, 4>(value);
}
/*!
  */
template <typename Type> inline
int8b as_char(Type&& value) noexcept
{
  return *zisc::reinterp<const int8b*>(&value);
}

/*!
  */
template <typename Type> inline
char2 as_char2(Type&& value) noexcept
{
  return *zisc::reinterp<const char2*>(&value);
}

/*!
  */
template <typename Type> inline
char3 as_char3(Type&& value) noexcept
{
  return *zisc::reinterp<const char3*>(&value);
}

/*!
  */
template <typename Type> inline
char4 as_char4(Type&& value) noexcept
{
  return *zisc::reinterp<const char4*>(&value);
}

/*!
  */
template <typename Type> inline
uint8b as_uchar(Type&& value) noexcept
{
  return *zisc::reinterp<const uint8b*>(&value);
}

/*!
  */
template <typename Type> inline
uchar2 as_uchar2(Type&& value) noexcept
{
  return *zisc::reinterp<const uchar2*>(&value);
}

/*!
  */
template <typename Type> inline
uchar3 as_uchar3(Type&& value) noexcept
{
  return *zisc::reinterp<const uchar3*>(&value);
}

/*!
  */
template <typename Type> inline
uchar4 as_uchar4(Type&& value) noexcept
{
  return *zisc::reinterp<const uchar4*>(&value);
}

/*!
  */
template <typename Type> inline
int16b as_short(Type&& value) noexcept
{
  return *zisc::reinterp<const int16b*>(&value);
}

/*!
  */
template <typename Type> inline
short2 as_short2(Type&& value) noexcept
{
  return *zisc::reinterp<const short2*>(&value);
}

/*!
  */
template <typename Type> inline
short3 as_short3(Type&& value) noexcept
{
  return *zisc::reinterp<const short3*>(&value);
}

/*!
  */
template <typename Type> inline
short4 as_short4(Type&& value) noexcept
{
  return *zisc::reinterp<const short4*>(&value);
}

/*!
  */
template <typename Type> inline
uint16b as_ushort(Type&& value) noexcept
{
  return *zisc::reinterp<const uint16b*>(&value);
}

/*!
  */
template <typename Type> inline
ushort2 as_ushort2(Type&& value) noexcept
{
  return *zisc::reinterp<const ushort2*>(&value);
}

/*!
  */
template <typename Type> inline
ushort3 as_ushort3(Type&& value) noexcept
{
  return *zisc::reinterp<const ushort3*>(&value);
}

/*!
  */
template <typename Type> inline
ushort4 as_ushort4(Type&& value) noexcept
{
  return *zisc::reinterp<const ushort4*>(&value);
}

/*!
  */
template <typename Type> inline
int32b as_int(Type&& value) noexcept
{
  return *zisc::reinterp<const int32b*>(&value);
}

/*!
  */
template <typename Type> inline
int2 as_int2(Type&& value) noexcept
{
  return *zisc::reinterp<const int2*>(&value);
}

/*!
  */
template <typename Type> inline
int3 as_int3(Type&& value) noexcept
{
  return *zisc::reinterp<const int3*>(&value);
}

/*!
  */
template <typename Type> inline
int4 as_int4(Type&& value) noexcept
{
  return *zisc::reinterp<const int4*>(&value);
}

/*!
  */
template <typename Type> inline
uint32b as_uint(Type&& value) noexcept
{
  return *zisc::reinterp<const uint32b*>(&value);
}

/*!
  */
template <typename Type> inline
uint2 as_uint2(Type&& value) noexcept
{
  return *zisc::reinterp<const uint2*>(&value);
}

/*!
  */
template <typename Type> inline
uint3 as_uint3(Type&& value) noexcept
{
  return *zisc::reinterp<const uint3*>(&value);
}

/*!
  */
template <typename Type> inline
uint4 as_uint4(Type&& value) noexcept
{
  return *zisc::reinterp<const uint4*>(&value);
}

/*!
  */
template <typename Type> inline
int64b as_long(Type&& value) noexcept
{
  return *zisc::reinterp<const int64b*>(&value);
}

/*!
  */
template <typename Type> inline
long2 as_long2(Type&& value) noexcept
{
  return *zisc::reinterp<const long2*>(&value);
}

/*!
  */
template <typename Type> inline
long3 as_long3(Type&& value) noexcept
{
  return *zisc::reinterp<const long3*>(&value);
}

/*!
  */
template <typename Type> inline
long4 as_long4(Type&& value) noexcept
{
  return *zisc::reinterp<const long4*>(&value);
}

/*!
  */
template <typename Type> inline
uint64b as_ulong(Type&& value) noexcept
{
  return *zisc::reinterp<const uint64b*>(&value);
}

/*!
  */
template <typename Type> inline
ulong2 as_ulong2(Type&& value) noexcept
{
  return *zisc::reinterp<const ulong2*>(&value);
}

/*!
  */
template <typename Type> inline
ulong3 as_ulong3(Type&& value) noexcept
{
  return *zisc::reinterp<const ulong3*>(&value);
}

/*!
  */
template <typename Type> inline
ulong4 as_ulong4(Type&& value) noexcept
{
  return *zisc::reinterp<const ulong4*>(&value);
}

/*!
  */
template <typename Type> inline
float as_float(Type&& value) noexcept
{
  return *zisc::reinterp<const float*>(&value);
}

/*!
  */
template <typename Type> inline
float2 as_float2(Type&& value) noexcept
{
  return *zisc::reinterp<const float2*>(&value);
}

/*!
  */
template <typename Type> inline
float3 as_float3(Type&& value) noexcept
{
  return *zisc::reinterp<const float3*>(&value);
}

/*!
  */
template <typename Type> inline
float4 as_float4(Type&& value) noexcept
{
  return *zisc::reinterp<const float4*>(&value);
}

/*!
  */
template <typename Type> inline
double as_double(Type&& value) noexcept
{
  return *zisc::reinterp<const double*>(&value);
}

/*!
  */
template <typename Type> inline
double2 as_double2(Type&& value) noexcept
{
  return *zisc::reinterp<const double2*>(&value);
}

/*!
  */
template <typename Type> inline
double3 as_double3(Type&& value) noexcept
{
  return *zisc::reinterp<const double3*>(&value);
}

/*!
  */
template <typename Type> inline
double4 as_double4(Type&& value) noexcept
{
  return *zisc::reinterp<const double4*>(&value);
}

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_UTILITY_INL_HPP
