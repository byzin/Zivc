/*!
  \file vector_data.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_VECTOR_DATA_HPP
#define ZIVC_CL_VECTOR_DATA_HPP

// Zivc
#include "types.hpp"
#include "type_traits.hpp"

namespace zivc {

/*!
  \brief Reading and writing functions of vector data
  */
class VectorData
{
 public:
  //! Read a data from address (p + offset * 2)
  template <typename AddressSpacePointer>
  static auto load2(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read a data from address (p + offset * 3)
  template <typename AddressSpacePointer>
  static auto load3(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read a data from address (p + offset * 4)
  template <typename AddressSpacePointer>
  static auto load4(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read a data from address (p + offset * 8)
  template <typename AddressSpacePointer>
  static auto load8(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read a data from address (p + offset * 16)
  template <typename AddressSpacePointer>
  static auto load16(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float loadHalf(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset * 2). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float2 loadHalf2(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset * 3). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float3 loadHalf3(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset * 4). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float4 loadHalf4(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset * 8). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float8 loadHalf8(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Read half data from address (p + offset * 16). The address must be 16bit aligned
  template <typename AddressSpacePointer>
  static float16 loadHalf16(const size_t offset, const AddressSpacePointer p) noexcept;

  //! Write to the address (p + offset * 2)
  template <typename VectorType, typename AddressSpacePointer>
  static void store2(const VectorType data,
                     const size_t offset,
                     AddressSpacePointer p) noexcept;

  //! Write to the address (p + offset * 3)
  template <typename VectorType, typename AddressSpacePointer>
  static void store3(const VectorType data,
                     const size_t offset,
                     AddressSpacePointer p) noexcept;

  //! Write to the address (p + offset * 4)
  template <typename VectorType, typename AddressSpacePointer>
  static void store4(const VectorType data,
                     const size_t offset,
                     AddressSpacePointer p) noexcept;

  //! Write to the address (p + offset * 8)
  template <typename VectorType, typename AddressSpacePointer>
  static void store8(const VectorType data,
                     const size_t offset,
                     AddressSpacePointer p) noexcept;

  //! Write to the address (p + offset * 16)
  template <typename VectorType, typename AddressSpacePointer>
  static void store16(const VectorType data,
                      const size_t offset,
                      AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset)
  template <typename AddressSpacePointer>
  static void storeHalf(const float data,
                        const size_t offset,
                        AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 2)
  template <typename AddressSpacePointer>
  static void storeHalf2(const float2 data,
                         const size_t offset,
                         AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 3)
  template <typename AddressSpacePointer>
  static void storeHalf3(const float3 data,
                         const size_t offset,
                         AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 4)
  template <typename AddressSpacePointer>
  static void storeHalf4(const float4 data,
                         const size_t offset,
                         AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 8)
  template <typename AddressSpacePointer>
  static void storeHalf8(const float8 data,
                         const size_t offset,
                         AddressSpacePointer p) noexcept;

  //! The value is converted to a half and writen to the address (p + offset * 16)
  template <typename AddressSpacePointer>
  static void storeHalf16(const float16 data,
                          const size_t offset,
                          AddressSpacePointer p) noexcept;
};

// OpenCL function aliases

//! Read a data from address (p + offset * 2)
template <typename AddressSpacePointer>
auto vload2(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read a data from address (p + offset * 3)
template <typename AddressSpacePointer>
auto vload3(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read a data from address (p + offset * 4)
template <typename AddressSpacePointer>
auto vload4(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read a data from address (p + offset * 8)
template <typename AddressSpacePointer>
auto vload8(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read a data from address (p + offset * 16)
template <typename AddressSpacePointer>
auto vload16(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset). The address must be 16bit aligned
template <typename AddressSpacePointer>
float vload_half(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset * 2). The address must be 16bit aligned
template <typename AddressSpacePointer>
float2 vload_half2(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset * 3). The address must be 16bit aligned
template <typename AddressSpacePointer>
float3 vload_half3(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset * 4). The address must be 16bit aligned
template <typename AddressSpacePointer>
float4 vload_half4(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset * 8). The address must be 16bit aligned
template <typename AddressSpacePointer>
float8 vload_half8(const size_t offset, const AddressSpacePointer p) noexcept;

//! Read half data from address (p + offset * 16). The address must be 16bit aligned
template <typename AddressSpacePointer>
float16 vload_half16(const size_t offset, const AddressSpacePointer p) noexcept;

//! Write to the address (p + offset * 2)
template <typename VectorType, typename AddressSpacePointer>
void vstore2(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept;

//! Write to the address (p + offset * 3)
template <typename VectorType, typename AddressSpacePointer>
void vstore3(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept;

//! Write to the address (p + offset * 4)
template <typename VectorType, typename AddressSpacePointer>
void vstore4(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept;

//! Write to the address (p + offset * 8)
template <typename VectorType, typename AddressSpacePointer>
void vstore8(const VectorType data,
             const size_t offset,
             AddressSpacePointer p) noexcept;

//! Write to the address (p + offset * 16)
template <typename VectorType, typename AddressSpacePointer>
void vstore16(const VectorType data,
              const size_t offset,
              AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset)
template <typename AddressSpacePointer>
void vstore_half(const float data,
                 const size_t offset,
                 AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 2)
template <typename AddressSpacePointer>
void vstore_half2(const float2 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 3)
template <typename AddressSpacePointer>
void vstore_half3(const float3 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 4)
template <typename AddressSpacePointer>
void vstore_half4(const float4 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 8)
template <typename AddressSpacePointer>
void vstore_half8(const float8 data,
                  const size_t offset,
                  AddressSpacePointer p) noexcept;

//! The value is converted to a half and writen to the address (p + offset * 16)
template <typename AddressSpacePointer>
void vstore_half16(const float16 data,
                   const size_t offset,
                   AddressSpacePointer p) noexcept;

} // namespace zivc

#include "vector_data-inl.hpp"

#endif /* ZIVC_CL_VECTOR_DATA_HPP */
