/*!
  \file kernel_arg_type_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_TYPE_INFO_INL_HPP
#define ZIVC_KERNEL_ARG_TYPE_INFO_INL_HPP

#include "kernel_arg_type_info.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "zivc/cppcl/address_space_pointer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief Information of address space kernel argument

  No detailed description.

  \tparam kASpaceType No description.
  \tparam Type No description.
  */
template <cl::AddressSpaceType kASpaceType, KernelArg Type>
class KernelArgTypeInfo<cl::AddressSpacePointer<kASpaceType, Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = kASpaceType == cl::AddressSpaceType::kGlobal;
  static constexpr bool kIsLocal = kASpaceType == cl::AddressSpaceType::kLocal;
  static constexpr bool kIsConstant = kASpaceType == cl::AddressSpaceType::kConstant;
  static constexpr bool kIsPod = false;
  static constexpr bool kIsBuffer = kIsGlobal || kIsConstant;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
  static_assert(kASpaceType != cl::AddressSpaceType::kPrivate,
                "Private address space argument isn't allowed.");
};

/*!
  \brief Information of buffer kernel argument

  No detailed description.

  \tparam Type No description.
  */
template <KernelArg Type>
class KernelArgTypeInfo<Buffer<Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = true;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = false;
  static constexpr bool kIsBuffer = kIsGlobal || kIsConstant;

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
};

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_TYPE_INFO_INL_HPP
