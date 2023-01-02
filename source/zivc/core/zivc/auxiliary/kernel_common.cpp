/*!
  \file kernel_common.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "kernel_common.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
// Zivc
#include "id_data.hpp"
#include "zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
KernelCommon::KernelCommon(IdData&& id) noexcept : ZivcObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
KernelCommon::~KernelCommon() noexcept
{
}

} // namespace zivc
