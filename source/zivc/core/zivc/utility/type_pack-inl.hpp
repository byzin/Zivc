/*!
  \file type_pack-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TYPE_PACK_INL_HPP
#define ZIVC_TYPE_PACK_INL_HPP

#include "type_pack.hpp"
// Standard C++ library
#include <cstddef>

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Types> inline
constexpr std::size_t TypePack<Types...>::size() noexcept
{
  return 0;
}

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  \tparam Types No description.
  */
template <typename T, typename ...Types>
struct TypePack<T, Types...>
{
  //! Alias of type T
  using Type = T;


  //! Return the number of types
  static constexpr std::size_t size() noexcept
  {
    return sizeof...(Types) + 1;
  }
};

} // namespace zivc

#endif // ZIVC_TYPE_PACK_INL_HPP
