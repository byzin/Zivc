/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPUCL_TYPES_HPP
#define ZIVC_CPUCL_TYPES_HPP

// Standard C++ library
#include <cstddef>
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
using size_t = std::size_t;
using ptrdiff_t = std::ptrdiff_t;
using intptr_t = std::intptr_t;
using uintptr_t = std::uintptr_t;

} // namespace cl

} // namespace zivc

#include "types-inl.hpp"

#endif // ZIVC_CPUCL_TYPES_HPP
