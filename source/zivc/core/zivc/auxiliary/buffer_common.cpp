/*!
  \file buffer_common.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "buffer_common.hpp"
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
BufferCommon::BufferCommon(IdData&& id) noexcept : ZivcObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
BufferCommon::~BufferCommon() noexcept
{
}

} // namespace zivc
