/*!
  \file gui_object.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_object.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory/std_memory_resource.hpp"

namespace example {

/*!
  \details No detailed description
  */
GuiObject::GuiObject() noexcept
{
}

/*!
  \details No detailed description
  */
GuiObject::~GuiObject() noexcept
{
}

/*!
  \details No detailed description
  */
void GuiObject::destroy() noexcept
{
  destroyData();
  own_.reset();
  parent_.reset();
}

/*!
  \details No detailed description

  \return No description
  */
GuiObject* GuiObject::getParent() noexcept
{
  auto* p = parent_.get();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
const GuiObject* GuiObject::getParent() const noexcept
{
  const auto* p = parent_.get();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
GuiObject* GuiObject::getOwn() noexcept
{
  return this;
}

/*!
  \details No detailed description

  \return No description
  */
const GuiObject* GuiObject::getOwn() const noexcept
{
  return this;
}

/*!
  \details No detailed description

  \return No description
  */
bool GuiObject::hasParent() const noexcept
{
  const bool result = getParent() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* GuiObject::memoryResource() noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  auto* p = getParent();
  zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* GuiObject::memoryResource() const noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  const auto* p = getParent();
  const zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description
  */
void GuiObject::destroyData() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
auto GuiObject::getOwnPtr() const noexcept -> const WeakPtr&
{
  return own_;
}

/*!
  \details No detailed description

  \return No description
  */
bool GuiObject::hasOwnPtr() const noexcept
{
  const auto& o = getOwnPtr();
  const bool result = !o.expired();
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  */
void GuiObject::initObject(SharedPtr&& parent, WeakPtr&& own) noexcept
{
  parent_ = std::move(parent);
  own_ = std::move(own);
}

} // namespace example
