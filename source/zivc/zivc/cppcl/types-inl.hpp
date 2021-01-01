/*!
  \file types-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_TYPES_INL_HPP
#define ZIVC_CL_TYPES_INL_HPP

#include "types.hpp"
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

namespace inner {

//! Specify that a type is half type
template <typename Type>
concept Half = zisc::SameAs<half, std::remove_cvref_t<Type>>;

} // namespace inner

} // namespace cl

} // namespace zivc

#endif // ZIVC_CL_TYPES_INL_HPP
