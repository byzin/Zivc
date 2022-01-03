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
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

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
                          const bool is_pod,
                          const bool is_buffer) noexcept;


  //! Check if the argument is global qualified
  constexpr bool isGlobal() const noexcept;

  //! Check if the argument is local qualified
  constexpr bool isLocal() const noexcept;

  //! Check if the argument is constant qualified
  constexpr bool isConstant() const noexcept;

  //! Check if the argument is pod
  constexpr bool isPod() const noexcept;

  //! Check if the argument is 
  constexpr bool isBuffer() const noexcept;

  //! Return the position of the argument
  constexpr std::size_t index() const noexcept;

  //! Set an index of the argument
  constexpr void setIndex(const std::size_t index) noexcept;

 private:
  std::size_t index_;
  uint8b is_global_;
  uint8b is_local_;
  uint8b is_constant_;
  uint8b is_pod_;
  uint8b is_buffer_;
  [[maybe_unused]] std::array<uint8b, 3> padding_{0, 0, 0};
};

} // namespace zivc

#include "kernel_arg_info-inl.hpp"

#endif // ZIVC_KERNEL_ARG_INFO_HPP
