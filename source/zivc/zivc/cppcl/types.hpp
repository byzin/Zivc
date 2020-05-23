/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPES_HPP
#define ZIVC_CL_TYPES_HPP

// Zisc
#include "zisc/ieee_754_binary.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

// Scalar data types
using uchar = uint8b;
using ushort = uint16b;
using ulong = uint64b;
using half = zisc::Binary16;
// Pointer types
using size_t = uint32b;
using ptrdiff_t = int32b;
using intptr_t = int32b;
using uintptr_t = uint32b;

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_TYPES_HPP
