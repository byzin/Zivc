/*!
  \file gui.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui.hpp"
// Zivc
#include "zivc/vulkan/utility/vulkan.hpp"
// GLFW
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
// ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
