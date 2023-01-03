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
//#include "floating_point.hpp"
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::load(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  // Type check
  using ElemT = typename ASpaceInfo::DataType;
  if constexpr (kIsHalf<ElemT>)
    return loadHalfImpl<kN>(offset, p);
  else
    return loadImpl<kN>(offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::store(const VectorType data,
                       const size_t offset,
                       AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  // Type check
  using ElemT = typename ASpaceInfo::DataType;
  if constexpr (kIsHalf<ElemT>)
    storeHalfImpl(data, offset, p);
  else
    storeImpl(data, offset, p);
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadImpl(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  using VectorT = VectorTypeFromElems<typename ASpaceInfo::DataType, kN>;
  VectorT data{};
  if constexpr (kN == 1)
    data = p[offset];
  else if constexpr (kN == 2)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload2(offset, p);
  else if constexpr (kN == 3)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload3(offset, p);
  else if constexpr (kN == 4)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload4(offset, p);
  else
    ; // Must not go this branch
  return data;
}

/*!
  \details No detailed description

  \tparam kN No description.
  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadHalfImpl(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  using VectorT = VectorTypeFromElems<float, kN>;
  VectorT data{};
  if constexpr (kN == 1)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload_half(offset, p);
  else if constexpr (kN == 2)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload_half2(offset, p);
  else if constexpr (kN == 3)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload_half3(offset, p);
  else if constexpr (kN == 4)
    data = ZIVC_CL_GLOBAL_NAMESPACE::vload_half4(offset, p);
  else
    ; // Must not go this branch
  return data;
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeImpl(const VectorType data,
                           const size_t offset,
                           AddressSpaceType p) noexcept
{
  // Source type check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  if constexpr (n == 1)
    p[offset] = data;
  else if constexpr (n == 2)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore2(data, offset, p);
  else if constexpr (n == 3)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore3(data, offset, p);
  else if constexpr (n == 4)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore4(data, offset, p);
  else
    ; // Must not go this branch
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeHalfImpl(const VectorType data,
                               const size_t offset,
                               AddressSpaceType p) noexcept
{
  // Source type check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

  // Destination type
  if constexpr (n == 1)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore_half(data, offset, p);
  else if constexpr (n == 2)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore_half2(data, offset, p);
  else if constexpr (n == 3)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore_half3(data, offset, p);
  else if constexpr (n == 4)
    ZIVC_CL_GLOBAL_NAMESPACE::vstore_half4(data, offset, p);
  else
    ; // Must not go this branch
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload2(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload3(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload4(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload_half(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<1>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload_half2(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload_half3(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam AddressSpaceType No description.
  \param [in] offset No description.
  \param [in] p No description.
  \return No description
  */
template <typename AddressSpaceType> inline
auto vload_half4(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore2(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore3(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore4(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half(const VectorType data,
                 const size_t offset,
                 AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 1, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half2(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half3(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  \details No detailed description

  \tparam VectorType No description.
  \tparam AddressSpaceType No description.
  \param [in] data No description.
  \param [in] offset No description.
  \param [out] p No description.
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half4(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  // Vector check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

} // namespace zivc

#endif /* ZIVC_CL_VECTOR_DATA_INL_HPP */
