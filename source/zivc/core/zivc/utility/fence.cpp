/*!
  \file fence.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "fence.hpp"
// Standard C++ library
#include <type_traits>
// Zivc
#include "zivc/device.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
void Fence::clear() noexcept
{
  if (isActive())
    device_->returnFence(this);
  device_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] device No description.
  */
void Fence::setDevice(Device* device)
{
  clear();
  device_ = device;
  if (isActive())
    device_->takeFence(this);
}

/*!
  \details No detailed description
  */
void Fence::wait() const noexcept
{
  if (isActive())
    device_->waitForCompletion(*this);
}

} // namespace zivc
