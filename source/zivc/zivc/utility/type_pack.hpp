/*!
  \file type_pack.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TYPE_PACK_HPP
#define ZIVC_TYPE_PACK_HPP

// Standard C++ library
#include <cstddef>

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam Types No description.
  */
template <typename ...Types>
struct TypePack
{
  //! First type in Types
  using Type = void;


  //! Return the number of types
  static constexpr std::size_t size() noexcept;
};

} // namespace zivc

#include "type_pack-inl.hpp"

#endif // ZIVC_TYPE_PACK_HPP
