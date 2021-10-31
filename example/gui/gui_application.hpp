/*!
  \file gui_application.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_APPLICATION_HPP
#define EXAMPLE_GUI_APPLICATION_HPP

// Standard C++ library
#include <memory>
// Gui
#include "gui_widget.hpp"
// Zisc
#include "zisc/memory/std_memory_resource.hpp"

namespace example {

// Forward declaration
class GuiApplicationOptions;
class GuiPlatform;

/*!
  \brief No brief description
  */
class GuiApplication : public GuiWidget
{
 public:
  //! Create a gui application
  GuiApplication() noexcept;

  //! Destroy the application
  ~GuiApplication() noexcept override;


  //! Exec the application
  int run();

  //! Initialize the application
  void initialize(WeakPtr&& own, GuiApplicationOptions& options);

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept override;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept override;

 private:
  //! Destroy the application
  void destroyData() noexcept override;

  //! Return the underlying platform instance
  GuiPlatform* platform() noexcept;

  //! Return the underlying platform instance
  const GuiPlatform* platform() const noexcept;

  //! Set memory resource used in the application
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;


  std::unique_ptr<zisc::pmr::memory_resource> default_mem_resource_;
  zisc::pmr::memory_resource* custom_mem_resource_ = nullptr;
  SharedObject platform_;
};

// Type aliases
using SharedGuiApp = std::shared_ptr<GuiApplication>;

//! Make a gui application
SharedGuiApp makeGuiApp(GuiApplicationOptions& options);

} // namespace example

#endif // EXAMPLE_GUI_APPLICATION_HPP
