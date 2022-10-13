/*!
  \file kernel_arg_pack.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_PACK_HPP
#define ZIVC_KERNEL_ARG_PACK_HPP

// Standard C++ library
#include <cstddef>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

} /* namespace zivc */

namespace zivc::internal {

/*!
  \brief No brief description

  No detailed description.

  \tparam Types No description.
  */
template <typename ...Types>
struct KernelArgPack
{
  //! First type in Types
  using ValueT = void;


  //! Return the number of types
  static constexpr std::size_t size() noexcept;
};

//! Concatenate the two packs
template <typename Type1, typename ...Types2>
auto operator+(const KernelArgPack<Type1>& lhs,
               const KernelArgPack<Types2...>& rhs) noexcept;

} // namespace zivc::internal

#include "kernel_arg_pack-inl.hpp"

#endif // ZIVC_KERNEL_ARG_PACK_HPP
