/*!
  \file backend.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "backend.hpp"
// Standard C++ library
#include <atomic>
#include <memory>
#include <utility>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "context.hpp"
#include "context_options.hpp"
#include "zivc_config.hpp"
#include "utility/id_data.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
Backend::Backend(Context* context) noexcept :
    ZivcObject(context->issueId()),
    context_{context}
{
}

/*!
  \details No detailed description
  */
Backend::~Backend() noexcept
{
}

/*!
  \details No detailed description
  */
void Backend::destroy() noexcept
{
  destroyData();
  destroyObject();
}

/*!
  \details No detailed description

  \param [in,out] own No description.
  \param [in,out] options No description.
  */
void Backend::initialize(WeakPtr&& own, ContextOptions& options)
{
  // Clear the previous backend data first
  destroy();

  initObject(nullptr, std::move(own));
  initData(options);

  setNameIfEmpty("Backend");
}

/*!
  \details No detailed description

  \return No description
  */
bool Backend::isDebugMode() const noexcept
{
  const bool mode = context_->isDebugMode();
  return mode;
}

/*!
  \details No detailed description

  \return No description
  */
IdData Backend::issueId() noexcept
{
  IdData id = context_->issueId();
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* Backend::memoryResource()
{
  zisc::pmr::memory_resource* mem_resource = context_->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* Backend::memoryResource() const noexcept
{
  const zisc::pmr::memory_resource* mem_resource = context_->memoryResource();
  return mem_resource;
}

} // namespace zivc
