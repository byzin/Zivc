/*!
  \file kernel_arg_pack-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_PACK_INL_HPP
#define ZIVC_KERNEL_ARG_PACK_INL_HPP

#include "kernel_arg_pack.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zivc
#include "kernel_arg_type_info.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc::internal {

/*!
  \details No detailed description

  \return No description
  */
template <typename ...Types> inline
constexpr std::size_t KernelArgPack<Types...>::size() noexcept
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
struct KernelArgPack<T, Types...>
{
  //! Alias of type T
  using ValueT = T;


  //! Return the number of types
  static constexpr std::size_t size() noexcept
  {
    return sizeof...(Types) + 1;
  }
};

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Types2 No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename Type1, typename ...Types2> inline
auto operator+([[maybe_unused]] const KernelArgPack<Type1>& lhs,
               const KernelArgPack<Types2...>& rhs) noexcept
{
  using InfoT = KernelArgTypeInfo<Type1>;
  if constexpr (InfoT::kIsLocal) {
    return rhs;
  }
  else {
    using ElementT = typename InfoT::ElementT;
    using ArgT = std::conditional_t<InfoT::kIsPod, std::add_const_t<ElementT>,
                                                   std::add_lvalue_reference_t<Buffer<ElementT>>>;
    return KernelArgPack<ArgT, Types2...>{};
  }
}

} // namespace zivc::internal

#endif // ZIVC_KERNEL_ARG_PACK_INL_HPP
