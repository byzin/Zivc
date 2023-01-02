/*!
  \file zivc_object-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_OBJECT_INL_HPP
#define ZIVC_ZIVC_OBJECT_INL_HPP

#include "zivc_object.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
IdData& ZivcObject::id() noexcept
{
  return id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const IdData& ZivcObject::id() const noexcept
{
  return id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ZivcObject::getOwnPtr() const noexcept -> const WeakPtr&
{
  return own_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ZivcObject::hasOwnPtr() const noexcept
{
  const WeakPtr& o = getOwnPtr();
  const bool result = !o.expired();
  return result;
}

/*!
  \details No detailed description
  */
inline
void ZivcObject::updateDebugInfo()
{
  if (isDebugMode())
    updateDebugInfoImpl();
}

} // namespace zivc

#endif // ZIVC_ZIVC_OBJECT_INL_HPP
