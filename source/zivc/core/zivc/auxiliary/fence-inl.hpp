/*!
  \file fence-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FENCE_INL_HPP
#define ZIVC_FENCE_INL_HPP

#include "fence.hpp"
// Standard C++ library
#include <memory>
#include <type_traits>
#include <utility>
// Zivc
#include "../zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
Fence::Fence() noexcept : Fence(nullptr)
{
}

/*!
  \details No detailed description

  \param [in] device No description.
  */
inline
Fence::Fence(Device* device) : device_{nullptr}
{
  setDevice(device);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
inline
Fence::Fence(Fence&& other) noexcept : Fence(nullptr)
{
  std::swap(fence_, other.fence_);
  std::swap(device_, other.device_);
}

/*!
  \details No detailed description
  */
inline
Fence::~Fence() noexcept
{
  clear();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
inline
Fence& Fence::operator=(Fence&& other) noexcept
{
  std::swap(fence_, other.fence_);
  std::swap(device_, other.device_);
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
Fence::operator bool() const noexcept
{
  const bool result = isActive();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto Fence::data() noexcept -> DataT&
{
  return fence_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto Fence::data() const noexcept -> const DataT&
{
  return fence_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool Fence::isActive() const noexcept
{
  const bool result = device_ != nullptr;
  return result;
}

} // namespace zivc

#endif // ZIVC_FENCE_INL_HPP
