/*!
  \file glfw.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GLFW_HPP
#define EXAMPLE_GLFW_HPP

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace example {

/*!
  \brief No brief description

  No detailed description.
  */
enum class GlfwSurfaceType : unsigned int
{
  kNone = 0,
  kWin32,
  kMacOS,
  kMetal,
  kXlib,
  kXcb,
  kWayland
};

//! Initialize glfw library for vulkan
int initGlfwVulkan(void* instance,
                   void* get_instance_proc_addr,
                   GLFWwindow* window,
                   const void* alloc,
                   const GlfwSurfaceType surface_type,
                   void** surface);

} // namespace example

#endif // EXAMPLE_GLFW_HPP
