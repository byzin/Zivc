/*!
  \file glfw.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstdio>
// Zisc
#include "zisc/utility.hpp"
#include "error.hpp"

namespace example {

/*!
  \details No detailed description

  \param [in] get_instance_proc_addr No description.
  */
void initGlfwVulkan(void* instance,
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
      zisc::reinterp<PFN_vkGetInstanceProcAddr>(get_instance_proc_addr);
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
    constexpr std::size_t max_desc_size = 1024;
    std::array<char, max_desc_size> desc;
    std::sprintf(desc.data(),
                 "Glfw Error: surface initialization failed. surfacetype=%d.\n",
                 zisc::cast<int>(surface_type));
    throw SystemError{ErrorCode::kGlfwInitializationFailed, desc.data()};
    break;
   }
  }

  {
    auto* ins = zisc::reinterp<VkInstance>(instance);
    auto* w = zisc::reinterp<_GLFWwindow*>(window);
    const auto* vk_alloc = zisc::reinterp<const VkAllocationCallbacks*>(alloc);
    auto* s = zisc::reinterp<VkSurfaceKHR*>(surface);
    VkResult result = _glfwPlatformCreateWindowSurface(ins, w, vk_alloc, s);
    checkVulkanResult(zisc::cast<int>(result));
  }
}

} // namespace example
