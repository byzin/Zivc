/*!
  \file glfw.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// GLFW
#if defined(Z_WINDOWS)
#define _GLFW_WIN32
#elif defined(Z_MAC)
#define _GLFW_COCOA
#elif defined(Z_LINUX)
#define _GLFW_X11
#endif
extern "C" {
#include "../src/internal.h"
}

#include "glfw.hpp"

namespace example {

/*!
  \details No detailed description

  \param [in] get_instance_proc_addr No description.
  */
int initGlfwVulkan(void* instance,
                   void* get_instance_proc_addr,
                   GLFWwindow* window,
                   const void* alloc,
                   const GlfwSurfaceType surface_type,
                   void** surface)
{
  _glfw.vk.available = GLFW_TRUE;
  _glfw.vk.handle = nullptr;
  //_glfw.vk.extensions;
  _glfw.vk.GetInstanceProcAddr =
      reinterpret_cast<PFN_vkGetInstanceProcAddr>(get_instance_proc_addr);
  _glfw.vk.KHR_surface = GLFW_TRUE;
  switch (surface_type) {
   case GlfwSurfaceType::kWin32: {
#if defined(_GLFW_WIN32)
    _glfw.vk.KHR_win32_surface = GLFW_TRUE;
#endif // _GLFW_WIN32
    break;
   }
   case GlfwSurfaceType::kMacOS: {
#if defined(_GLFW_COCOA)
    _glfw.vk.MVK_macos_surface = GLFW_TRUE;
    _glfw.vk.EXT_metal_surface = GLFW_FALSE;
#endif // _GLFW_COCOA
    break;
   }
   case GlfwSurfaceType::kMetal: {
#if defined(_GLFW_COCOA)
    _glfw.vk.MVK_macos_surface = GLFW_FALSE;
    _glfw.vk.EXT_metal_surface = GLFW_TRUE;
#endif // _GLFW_COCOA
    break;
   }
   case GlfwSurfaceType::kXlib: {
#if defined(_GLFW_X11)
    _glfw.vk.KHR_xlib_surface = GLFW_TRUE;
    _glfw.vk.KHR_xcb_surface = GLFW_FALSE;
#endif
    break;
   }
   case GlfwSurfaceType::kXcb: {
#if defined(_GLFW_X11)
    _glfw.vk.KHR_xlib_surface = GLFW_FALSE;
    _glfw.vk.KHR_xcb_surface = GLFW_TRUE;
#endif
    break;
   }
   case GlfwSurfaceType::kNone:
   case GlfwSurfaceType::kWayland:
   default: {
    break;
   }
  }

  int result = static_cast<int>(VK_SUCCESS);
  {
    auto* ins = reinterpret_cast<VkInstance>(instance);
    auto* w = reinterpret_cast<_GLFWwindow*>(window);
    const auto* vk_alloc = reinterpret_cast<const VkAllocationCallbacks*>(alloc);
    auto* s = reinterpret_cast<VkSurfaceKHR*>(surface);
    const VkResult r = _glfwPlatformCreateWindowSurface(ins, w, vk_alloc, s);
    result = static_cast<int>(r);
  }

  return result;
}

} // namespace example
