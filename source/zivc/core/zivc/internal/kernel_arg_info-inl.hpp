/*!
  \file kernel_arg_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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

namespace zivc::internal {

/*!
  \details No detailed description
  */
inline
constexpr KernelArgInfo::KernelArgInfo() noexcept :
    index_{0},
    local_offset_{0},
    pod_offset_{0},
    is_global_{},
    is_local_{},
    is_constant_{},
    is_pod_{}
{
}

/*!
  \details No detailed description

  \param [in] is_global No description.
  \param [in] is_local No description.
  \param [in] is_constant No description.
  \param [in] is_pod No description.
  */
inline
constexpr KernelArgInfo::KernelArgInfo(const bool is_global,
                                       const bool is_local,
                                       const bool is_constant,
                                       const bool is_pod) noexcept :
    index_{0},
    local_offset_{0},
    pod_offset_{0},
    is_global_{is_global},
    is_local_{is_local},
    is_constant_{is_constant},
    is_pod_{is_pod}
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
  return isGlobal() || isConstant();
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgInfo::isParameter() const noexcept
{
  return isBuffer() || isPod();
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

  \return No description
  */
inline
constexpr std::size_t KernelArgInfo::localOffset() const noexcept
{
  return static_cast<std::size_t>(local_offset_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t KernelArgInfo::podOffset() const noexcept
{
  return static_cast<std::size_t>(pod_offset_);
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

/*!
  \details No detailed description

  \param [in] offset No description.
  */
inline
constexpr void KernelArgInfo::setLocalOffset(const std::size_t offset) noexcept
{
  local_offset_ = static_cast<uint8b>(offset);
}

/*!
  \details No detailed description

  \param [in] offset No description.
  */
inline
constexpr void KernelArgInfo::setPodOffset(const std::size_t offset) noexcept
{
  pod_offset_ = static_cast<uint8b>(offset);
}

} // namespace zivc::internal

#endif // ZIVC_KERNEL_ARG_INFO_INL_HPP
