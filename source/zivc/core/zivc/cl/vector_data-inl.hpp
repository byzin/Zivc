/*!
  \file vector_data-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_VECTOR_DATA_INL_HPP
#define ZIVC_CL_VECTOR_DATA_INL_HPP

#include "vector_data.hpp"
// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto VectorData::load2(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload2(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto VectorData::load3(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload3(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto VectorData::load4(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload4(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto VectorData::load8(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload8(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto VectorData::load16(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload16(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float VectorData::loadHalf(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float2 VectorData::loadHalf2(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half2(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float3 VectorData::loadHalf3(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half3(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float4 VectorData::loadHalf4(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half4(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float8 VectorData::loadHalf8(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half8(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float16 VectorData::loadHalf16(const size_t offset, const AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  return ZIVC_CL_GLOBAL_NAMESPACE::vload_half16(offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void VectorData::store2(const VectorType data,
                        const size_t offset,
                        AddressSpacePointer p) noexcept
{
  // Vector size check
  using VecInfo = VectorTypeInfo<RemoveCvT<VectorType>>;
  static_assert(VecInfo::size() == 2, "The size of vector isn't 2.");

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void VectorData::store3(const VectorType data,
                        const size_t offset,
                        AddressSpacePointer p) noexcept
{
  // Vector size check
  using VecInfo = VectorTypeInfo<RemoveCvT<VectorType>>;
  static_assert(VecInfo::size() == 3, "The size of vector isn't 3.");

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void VectorData::store4(const VectorType data,
                        const size_t offset,
                        AddressSpacePointer p) noexcept
{
  // Vector size check
  using VecInfo = VectorTypeInfo<RemoveCvT<VectorType>>;
  static_assert(VecInfo::size() == 4, "The size of vector isn't 4.");

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void VectorData::store8(const VectorType data,
                        const size_t offset,
                        AddressSpacePointer p) noexcept
{
  // Vector size check
  using VecInfo = VectorTypeInfo<RemoveCvT<VectorType>>;
  static_assert(VecInfo::size() == 8, "The size of vector isn't 8.");

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void VectorData::store16(const VectorType data,
                         const size_t offset,
                         AddressSpacePointer p) noexcept
{
  // Vector size check
  using VecInfo = VectorTypeInfo<RemoveCvT<VectorType>>;
  static_assert(VecInfo::size() == 16, "The size of vector isn't 16.");

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore16(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf(const float data,
                           const size_t offset,
                           AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf2(const float2 data,
                            const size_t offset,
                            AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf3(const float3 data,
                            const size_t offset,
                            AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf4(const float4 data,
                            const size_t offset,
                            AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf8(const float8 data,
                            const size_t offset,
                            AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void VectorData::storeHalf16(const float16 data,
                             const size_t offset,
                             AddressSpacePointer p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpacePointer>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  ZIVC_CL_GLOBAL_NAMESPACE::vstore_half16(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto vload2(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::load2(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto vload3(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::load3(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto vload4(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::load4(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto vload8(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::load8(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
auto vload16(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::load16(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float vload_half(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float2 vload_half2(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf2(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float3 vload_half3(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf3(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float4 vload_half4(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf4(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float8 vload_half8(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf8(offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpacePointer> inline
float16 vload_half16(const size_t offset, const AddressSpacePointer p) noexcept
{
  return VectorData::loadHalf16(offset, p);
}
/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void vstore2(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept
{
  VectorData::store2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void vstore3(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept
{
  VectorData::store3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void vstore4(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept
{
  VectorData::store4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void vstore8(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept
{
  VectorData::store8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpacePointer> inline
void vstore16(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept
{
  VectorData::store16(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half(const float data,
                 const size_t offset,
                 AddressSpacePointer p) noexcept
{
  VectorData::storeHalf(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half2(const float2 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept
{
  VectorData::storeHalf2(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half3(const float3 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept
{
  VectorData::storeHalf3(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half4(const float4 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept
{
  VectorData::storeHalf4(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half8(const float8 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept
{
  VectorData::storeHalf8(data, offset, p);
}

/*!
  \details No detailed description

  \tparam AddressSpacePointer No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename AddressSpacePointer> inline
void vstore_half16(const float16 data,
                   const size_t offset,
                   AddressSpacePointer p) noexcept
{
  VectorData::storeHalf16(data, offset, p);
}

} // namespace zivc

#endif /* ZIVC_CL_VECTOR_DATA_INL_HPP */
