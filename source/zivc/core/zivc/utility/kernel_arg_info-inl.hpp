/*!
  \file kernel_arg_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_INFO_INL_HPP
#define ZIVC_KERNEL_ARG_INFO_INL_HPP

#include "kernel_arg_info.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
constexpr KernelArgInfo::KernelArgInfo() noexcept :
    index_{0},
    is_global_{},
    is_local_{},
    is_constant_{},
    is_pod_{},
    is_buffer_{}
{
}

/*!
  \details No detailed description

  \param [in] is_global No description.
  \param [in] is_local No description.
  \param [in] is_constant No description.
  \param [in] is_pod No description.
  \param [in] is_buffer No description.
  */
inline
constexpr KernelArgInfo::KernelArgInfo(const bool is_global,
                                       const bool is_local,
                                       const bool is_constant,
                                       const bool is_pod,
                                       const bool is_buffer) noexcept :
    index_{0},
    is_global_{is_global},
    is_local_{is_local},
    is_constant_{is_constant},
    is_pod_{is_pod},
    is_buffer_{is_buffer}
{
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isGlobal() const noexcept
{
  return is_global_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isLocal() const noexcept
{
  return is_local_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isConstant() const noexcept
{
  return is_constant_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isPod() const noexcept
{
  return is_pod_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isBuffer() const noexcept
{
  return is_buffer_;
}
/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t KernelArgInfo::index() const noexcept
{
  return index_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  */
inline
constexpr void KernelArgInfo::setIndex(const std::size_t index) noexcept
{
  index_ = index;
}

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_INFO_INL_HPP
