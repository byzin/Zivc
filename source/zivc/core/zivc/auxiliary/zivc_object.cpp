/*!
  \file zivc_object.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "zivc_object.hpp"
// Standard C++ library
#include <memory>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "error.hpp"
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] id No description.
  */
ZivcObject::ZivcObject(IdData&& id) noexcept : id_{std::move(id)}
{
}

/*!
  \details No detailed description
  */
ZivcObject::~ZivcObject() noexcept
{
  destroyObject();
}

/*!
  \details No detailed description
  */
void ZivcObject::destroyObject() noexcept
{
  own_.reset();
  parent_.reset();
}

/*!
  \details No detailed description

  \return No description
  */
ZivcObject* ZivcObject::getParent()
{
  ZivcObject* p = parent_.get();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
const ZivcObject* ZivcObject::getParent() const noexcept
{
  const ZivcObject* p = parent_.get();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
ZivcObject* ZivcObject::getOwn()
{
  return this;
}

/*!
  \details No detailed description

  \return No description
  */
const ZivcObject* ZivcObject::getOwn() const noexcept
{
  return this;
}

/*!
  \details No detailed description

  \return No description
  */
bool ZivcObject::hasParent() const noexcept
{
  const bool result = getParent() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  */
void ZivcObject::initObject(SharedPtr&& parent, WeakPtr&& own) noexcept
{
  parent_ = std::move(parent);
  own_ = std::move(own);
}

/*!
  \details No detailed description

  \return No description
  */
bool ZivcObject::isDebugMode() const noexcept
{
  ZIVC_ASSERT(hasParent(), "Parent is null.");
  const ZivcObject* p = getParent();
  const bool mode = p->isDebugMode();
  return mode;
}

/*!
  \details No detailed description

  \return No description
  */
IdData ZivcObject::issueId() noexcept
{
  ZIVC_ASSERT(hasParent(), "Parent is null.");
  ZivcObject* p = getParent();
  IdData child_id = p->issueId();
  return child_id;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* ZivcObject::memoryResource()
{
  ZIVC_ASSERT(hasParent(), "Parent is null.");
  ZivcObject* p = getParent();
  zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* ZivcObject::memoryResource() const noexcept
{
  ZIVC_ASSERT(hasParent(), "Parent is null.");
  const ZivcObject* p = getParent();
  const zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \param [in] object_name No description.
  */
void ZivcObject::setName(const std::string_view object_name)
{
  IdData& object_id = id();
  object_id.setName(object_name);
  updateDebugInfo();
}

/*!
  \details No detailed description

  \param [in] object_name No description.
  */
void ZivcObject::setNameIfEmpty(const std::string_view object_name)
{
  IdData& object_id = id();
  if (!object_id.hasName())
    object_id.setName(object_name);
  updateDebugInfo();
}

/*!
  \details No detailed description

  \return No description
  */
BackendType ZivcObject::type() const noexcept
{
  ZIVC_ASSERT(hasParent(), "Parent is null.");
  const ZivcObject* p = getParent();
  return p->type();
}

} // namespace zivc
