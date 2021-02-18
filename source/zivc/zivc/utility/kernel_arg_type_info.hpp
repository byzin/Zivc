/*!
  \file kernel_arg_type_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_TYPE_INFO_HPP
#define ZIVC_KERNEL_ARG_TYPE_INFO_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

/*!
  \brief Information of a pod kernel argument

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class KernelArgTypeInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = false;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = true;
  static constexpr bool kIsBuffer = false;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The Type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The Type is reference.");
  static_assert(std::is_standard_layout_v<ElementType>,
                "The POD type isn't standard layout type.");
  static_assert(std::is_trivially_copyable_v<ElementType>,
                "The POD type isn't trivially copyable type.");
  static_assert(zisc::EqualityComparable<ElementType>,
                "The POD type isn't equality comparable.");
};

} // namespace zivc

#include "kernel_arg_type_info-inl.hpp"

#endif // ZIVC_KERNEL_ARG_TYPE_INFO_HPP
