/*!
  \file gui_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_platform.hpp"
// Standard C++ library
#include <array>
#include <cstdio>
#include <memory>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/vulkan/utility/vulkan.hpp"
// GLFW
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
// ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
// Gui
#include "gui_application_options.hpp"
#include "utility/error.hpp"

namespace example {

/*!
  \details No detailed description
  */
GuiPlatform::GuiPlatform() noexcept
{
}

/*!
  \details No detailed description
  */
GuiPlatform::~GuiPlatform() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] options No description.
  */
void GuiPlatform::initialize(SharedPtr&& parent, WeakPtr&& own, const GuiApplicationOptions& options)
{
  // Clear the previous platform data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initGlfw(options);
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyData() noexcept
{
  destroyGlfw();
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyGlfw() noexcept
{
  if (glfw_window_ != nullptr) {
    glfwDestroyWindow(glfw_window_);
    glfw_window_ = nullptr;
  }
  glfwTerminate();
}

/*!
  \details No detailed description

  \param [in] options No description.
  */
void GuiPlatform::initGlfw(const GuiApplicationOptions& options)
{
  // Setup GLFW window
  {
    glfwSetErrorCallback(GuiPlatform::raiseGlfwError);
    if (glfwInit() == 0)
      raiseGlfwError(0, "Glfw initialization failed.");

    // Window hints
    // Window related
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    // Buffer related
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    // Context related
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    const int width = zisc::cast<int>(options.windowWidth());
    const int height = zisc::cast<int>(options.windowHeight());
    const char* title = options.windowTitle().data();
    glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
  }
}

/*!
  \details No detailed description

  \param [in] error No description.
  \param [in] description No description.
  */
void GuiPlatform::raiseGlfwError(const int error, const char* description)
{
  constexpr std::size_t max_desc_size = 1024;
  std::array<char, max_desc_size> desc;
  std::sprintf(desc.data(), "Glfw Error %d: %s\n", error, description);
  throw SystemError{ErrorCode::kGlfwInitializationFailed, desc.data()};
}

} // namespace example
