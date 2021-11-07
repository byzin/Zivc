/*!
  \file gui_application.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_application.hpp"
// Standard C++ library
#include <memory>
#include <stdexcept>
// Zisc
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Gui
#include "gui_application_options.hpp"
#include "gui_platform.hpp"
#include "utility/error.hpp"

namespace example {

/*!
  \details No detailed description
  */
GuiApplication::GuiApplication() noexcept
{
}

/*!
  \details No detailed description
  */
GuiApplication::~GuiApplication() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \return No description
  */
int GuiApplication::run()
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* GuiApplication::memoryResource() noexcept
{
  zisc::pmr::memory_resource* mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* GuiApplication::memoryResource() const noexcept
{
  const zisc::pmr::memory_resource* mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
GuiPlatform* GuiApplication::platform() noexcept
{
  auto* p = dynamic_cast<GuiPlatform*>(platform_.get());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
const GuiPlatform* GuiApplication::platform() const noexcept
{
  const auto* p = dynamic_cast<const GuiPlatform*>(platform_.get());
  return p;
}

/*!
  \details No detailed description
  */
void GuiApplication::initialize(WeakPtr&& own,
                                zivc::Platform& zplatform,
                                GuiApplicationOptions& options)
{
  // Clear the previous platform data first
  destroy();

  initObject(nullptr, std::move(own));
  setMemoryResource(options.memoryResource());

  auto* mem_resource = memoryResource();
  {
    zisc::pmr::polymorphic_allocator<GuiPlatform> alloc{mem_resource};
    platform_ = std::allocate_shared<GuiPlatform>(alloc);
    SharedObject parent{getOwnPtr()};
    WeakObject own{platform_};
    platform()->initialize(std::move(parent), std::move(own), zplatform, options);
  }
}

/*!
  \details No detailed description
  */
void GuiApplication::destroyData() noexcept
{
  platform_.reset();

  default_mem_resource_.reset();
  custom_mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
void GuiApplication::setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept
{
  custom_mem_resource_ = mem_resource;
  if (mem_resource != nullptr)
    default_mem_resource_.reset();
  else
    default_mem_resource_ = std::make_unique<zisc::SimpleMemoryResource>();
}

/*!
  \details No detailed description

  \param [in] options No description.
  \return No description
  */
SharedGuiApp makeGuiApp(zivc::Platform& zplatform,
                        GuiApplicationOptions& options)
{
  SharedGuiApp app;

  // Create an application
  auto* mem_resource = options.memoryResource();
  if (mem_resource != nullptr) {
    zisc::pmr::polymorphic_allocator<GuiApplication> alloc{mem_resource};
    app = std::allocate_shared<GuiApplication>(alloc);
  }
  else {
    app = std::make_shared<GuiApplication>();
  }

  // Initialize the app
  WeakObject own{app};
  try {
    app->initialize(std::move(own), zplatform, options);
  }
  catch (const std::runtime_error& error) {
    app->destroy();
    app.reset();
    throw error;
  }

  return app;
}

} // namespace example
