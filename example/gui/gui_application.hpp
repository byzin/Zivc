/*!
  \file gui_application.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_APPLICATION_HPP
#define EXAMPLE_GUI_APPLICATION_HPP

// Standard C++ library
#include <memory>
#include <thread>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Gui
#include "gui_widget.hpp"

// Forward declaration
namespace zivc {
class Context;
} // namespace zivc

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


  //! Draw application widgets
  void draw() noexcept override;

  //! Exec the application
  int run();

  //! Initialize the application
  void initialize(WeakPtr&& own,
                  zivc::Context& context,
                  GuiApplicationOptions& options);

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
  bool is_demo_window_active_ = true;
  [[maybe_unused]] zivc::Padding<7> pad_;
  // Example
  zivc::SharedDevice zdevice_;
  zivc::SharedBuffer<zivc::int32b> zbuffer_host_;
  zivc::SharedBuffer<zivc::int32b> zbuffer1_;
  zivc::SharedBuffer<zivc::int32b> zbuffer2_;
  zivc::SharedKernelCommon zkernel_;
  std::unique_ptr<std::thread> kernel_thread_;
};

// Type aliases
using SharedGuiApp = std::shared_ptr<GuiApplication>;

//! Make a gui application
SharedGuiApp createGuiApp(zivc::Context& context,
                          GuiApplicationOptions& options);

} // namespace example

#endif // EXAMPLE_GUI_APPLICATION_HPP
