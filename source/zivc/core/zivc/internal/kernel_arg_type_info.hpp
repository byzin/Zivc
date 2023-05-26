/*!
  \file kernel_arg_type_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_TYPE_INFO_HPP
#define ZIVC_KERNEL_ARG_TYPE_INFO_HPP

// Standard C++ library
#include <concepts>
#include <cstddef>
#include <type_traits>
// Zivc
#include "../zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

} /* namespace zivc */

namespace zivc::internal {

/*!
  \brief Information of a pod kernel argument

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class KernelArgTypeInfo
{
 public:
  // Type aliases
  using ElementT = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = false;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = true;
  static constexpr bool kIsBuffer = false;
  static constexpr bool kIsParameter = true;

 private:
  static_assert(!std::is_pointer_v<ElementT>, "The Type is pointer.");
  static_assert(!std::is_reference_v<ElementT>, "The Type is reference.");
  static_assert(std::is_trivial_v<ElementT>, "The POD isn't trivial.");
  static_assert(std::is_standard_layout_v<ElementT>, "The POD isn't standard layout.");
  static_assert(std::equality_comparable<ElementT>, "The POD isn't equal comparable.");
};

} // namespace zivc::internal

#include "kernel_arg_type_info-inl.hpp"

#endif // ZIVC_KERNEL_ARG_TYPE_INFO_HPP
