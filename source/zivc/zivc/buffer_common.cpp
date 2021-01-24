/*!
  \file buffer_common.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "buffer_common.hpp"
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"
#include "zivc_config.hpp"

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
