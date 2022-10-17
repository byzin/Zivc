/*!
  \file kernel_arg_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_INFO_HPP
#define ZIVC_KERNEL_ARG_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
// Zisc
#include "zisc/boolean.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc::internal {

/*!
  \brief No brief description

  No detailed description.  */
class KernelArgInfo
{
 public:
  //! Initialize a parse result
  constexpr KernelArgInfo() noexcept;

  //! Initialize a parse result
  constexpr KernelArgInfo(const bool is_global,
                          const bool is_local,
                          const bool is_constant,
                          const bool is_pod) noexcept;


  //! Check if the argument is global qualified
  constexpr bool isGlobal() const noexcept;

  //! Check if the argument is local qualified
  constexpr bool isLocal() const noexcept;

  //! Check if the argument is constant qualified
  constexpr bool isConstant() const noexcept;

  //! Check if the argument is pod
  constexpr bool isPod() const noexcept;

  //! Check if the argument is buffer parameter
  constexpr bool isBuffer() const noexcept;

  //! Check if the argument is parameter for kernel execution
  constexpr bool isParameter() const noexcept;

  //! Return the position of the argument
  constexpr std::size_t index() const noexcept;

  //! Return the local offset at the argument
  constexpr std::size_t localOffset() const noexcept;

  //! Return the POD offset at the argument
  constexpr std::size_t podOffset() const noexcept;

  //! Set an index of the argument
  constexpr void setIndex(const std::size_t index) noexcept;

  //! Set a local offset at the argument
  constexpr void setLocalOffset(const std::size_t offset) noexcept;

  //! Set a POD offset at the argument
  constexpr void setPodOffset(const std::size_t offset) noexcept;

 private:
  std::size_t index_;
  uint8b local_offset_;
  uint8b pod_offset_;
  zisc::Boolean is_global_;
  zisc::Boolean is_local_;
  zisc::Boolean is_constant_;
  zisc::Boolean is_pod_;
  [[maybe_unused]] std::array<uint8b, 2> padding_{0, 0};
};

} // namespace zivc::internal

#include "kernel_arg_info-inl.hpp"

#endif // ZIVC_KERNEL_ARG_INFO_HPP
