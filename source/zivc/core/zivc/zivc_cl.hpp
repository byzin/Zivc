/*!
  \file zivc_cl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_CL_HPP
#define ZIVC_ZIVC_CL_HPP

// Zivc
#include "zivc_config.hpp"
#include "cpucl/cpucl.hpp"

namespace zivc::cl {
#include "zivc/cl/boolean.hpp"
} /* namespace zivc::cl */

namespace zivc {

// Data types corresponding to data types in OpenCL
// Scalar types
using cl_char = int8b; //!< Corresponding to 'char' in OpenCL
using cl_uchar = uint8b; //!< Corresponding to 'unsigned char' in OpenCL
using cl_short = int16b; //!< Corresponding to 'short' in OpenCL
using cl_ushort = uint16b; //!< Corresponding to 'unsigned short' in OpenCL
using cl_int = int32b; //!< Corresponding to 'int' in OpenCL
using cl_uint = uint32b; //!< Corresponding to 'unsigned int' in OpenCL
using cl_long = int64b; //!< Corresponding to 'long' in OpenCL
using cl_ulong = uint64b; //!< Corresponding to 'unsigned long' in OpenCL
using cl_half = cl::half; //!< Corresponding to 'half' in OpenCL
using cl_float = float; //!< Corresponding to 'float' in OpenCL
using cl_double = double; //!< Corresponding to 'double' in OpenCL
// Vector types
using cl_char2 = cl::char2; //!< Corresponding to 'char2' in OpenCL
using cl_char3 = cl::char3; //!< Corresponding to 'char3' in OpenCL
using cl_char4 = cl::char4; //!< Corresponding to 'char4' in OpenCL
using cl_char8 = cl::char8; //!< Corresponding to 'char8' in OpenCL
using cl_char16 = cl::char16; //!< Corresponding to 'char16' in OpenCL
using cl_uchar2 = cl::uchar2; //!< Corresponding to 'uchar2' in OpenCL
using cl_uchar3 = cl::uchar3; //!< Corresponding to 'uchar3' in OpenCL
using cl_uchar4 = cl::uchar4; //!< Corresponding to 'uchar4' in OpenCL
using cl_uchar8 = cl::uchar8; //!< Corresponding to 'uchar8' in OpenCL
using cl_uchar16 = cl::uchar16; //!< Corresponding to 'uchar16' in OpenCL
using cl_short2 = cl::short2; //!< Corresponding to 'short2' in OpenCL
using cl_short3 = cl::short3; //!< Corresponding to 'short3' in OpenCL
using cl_short4 = cl::short4; //!< Corresponding to 'short4' in OpenCL
using cl_short8 = cl::short8; //!< Corresponding to 'short8' in OpenCL
using cl_short16 = cl::short16; //!< Corresponding to 'short16' in OpenCL
using cl_ushort2 = cl::ushort2; //!< Corresponding to 'ushort2' in OpenCL
using cl_ushort3 = cl::ushort3; //!< Corresponding to 'ushort3' in OpenCL
using cl_ushort4 = cl::ushort4; //!< Corresponding to 'ushort4' in OpenCL
using cl_ushort8 = cl::ushort8; //!< Corresponding to 'ushort8' in OpenCL
using cl_ushort16 = cl::ushort16; //!< Corresponding to 'ushort16' in OpenCL
using cl_int2 = cl::int2; //!< Corresponding to 'int2' in OpenCL
using cl_int3 = cl::int3; //!< Corresponding to 'int3' in OpenCL
using cl_int4 = cl::int4; //!< Corresponding to 'int4' in OpenCL
using cl_int8 = cl::int8; //!< Corresponding to 'int8' in OpenCL
using cl_int16 = cl::int16; //!< Corresponding to 'int16' in OpenCL
using cl_uint2 = cl::uint2; //!< Corresponding to 'uint2' in OpenCL
using cl_uint3 = cl::uint3; //!< Corresponding to 'uint3' in OpenCL
using cl_uint4 = cl::uint4; //!< Corresponding to 'uint4' in OpenCL
using cl_uint8 = cl::uint8; //!< Corresponding to 'uint8' in OpenCL
using cl_uint16 = cl::uint16; //!< Corresponding to 'uint16' in OpenCL
using cl_long2 = cl::long2; //!< Corresponding to 'long2' in OpenCL
using cl_long3 = cl::long3; //!< Corresponding to 'long3' in OpenCL
using cl_long4 = cl::long4; //!< Corresponding to 'long4' in OpenCL
using cl_long8 = cl::long8; //!< Corresponding to 'long8' in OpenCL
using cl_long16 = cl::long16; //!< Corresponding to 'long16' in OpenCL
using cl_ulong2 = cl::ulong2; //!< Corresponding to 'ulong2' in OpenCL
using cl_ulong3 = cl::ulong3; //!< Corresponding to 'ulong3' in OpenCL
using cl_ulong4 = cl::ulong4; //!< Corresponding to 'ulong4' in OpenCL
using cl_ulong8 = cl::ulong8; //!< Corresponding to 'ulong8' in OpenCL
using cl_ulong16 = cl::ulong16; //!< Corresponding to 'ulong16' in OpenCL
using cl_half2 = cl::half2; //!< Corresponding to 'half2' in OpenCL
using cl_half3 = cl::half3; //!< Corresponding to 'half3' in OpenCL
using cl_half4 = cl::half4; //!< Corresponding to 'half4' in OpenCL
using cl_half8 = cl::half8; //!< Corresponding to 'half8' in OpenCL
using cl_half16 = cl::half16; //!< Corresponding to 'half16' in OpenCL
using cl_float2 = cl::float2; //!< Corresponding to 'float2' in OpenCL
using cl_float3 = cl::float3; //!< Corresponding to 'float3' in OpenCL
using cl_float4 = cl::float4; //!< Corresponding to 'float4' in OpenCL
using cl_float8 = cl::float8; //!< Corresponding to 'float8' in OpenCL
using cl_float16 = cl::float16; //!< Corresponding to 'float16' in OpenCL
using cl_double2 = cl::double2; //!< Corresponding to 'double2' in OpenCL
using cl_double3 = cl::double3; //!< Corresponding to 'double3' in OpenCL
using cl_double4 = cl::double4; //!< Corresponding to 'double4' in OpenCL
using cl_double8 = cl::double8; //!< Corresponding to 'double8' in OpenCL
using cl_double16 = cl::double16; //!< Corresponding to 'double16' in OpenCL

// Auxiliary types
using cl_Boolean = cl::zivc::Boolean;
template <typename Type>
using cl_CompResult = cl::CompResult<Type>;

} /* namespace zivc */

#endif /* ZIVC_ZIVC_CL_HPP */
