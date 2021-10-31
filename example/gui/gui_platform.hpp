/*!
  \file gui_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_PLATFORM_HPP
#define EXAMPLE_GUI_PLATFORM_HPP

// Standard C++ library
#include <memory>
// Gui
#include "gui_object.hpp"

// Forward declaration
struct GLFWwindow;

namespace example {

// Forward declaration
class GuiApplicationOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class GuiPlatform : public GuiObject
{
 public:
  //! Create a gui platform
  GuiPlatform() noexcept;

  //! Destroy the platform
  ~GuiPlatform() noexcept override;


  //! Initialize the gui instance
  void initialize(SharedPtr&& parent, WeakPtr&& own, const GuiApplicationOptions& options);

 protected:
  //! Destroy the platform
  void destroyData() noexcept override;

 private:
  //! Destroy the glfw instance
  void destroyGlfw() noexcept;

  //! Initialize glfw
  void initGlfw(const GuiApplicationOptions& options);

  //! Raise a glfw error
  static void raiseGlfwError(const int error, const char* description);

  GLFWwindow* glfw_window_ = nullptr;
};

} // namespace example

#endif // EXAMPLE_GUI_PLATFORM_HPP
