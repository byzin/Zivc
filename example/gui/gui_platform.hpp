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
#include <cstddef>
#include <memory>
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/vulkan/utility/vulkan.hpp"
// Gui
#include "gui_object.hpp"

// Forward declaration
struct GLFWwindow;
namespace zivc {
class Platform;
} // namespace zivc

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


  //! Return the underlying vulkan device
  zivc::VulkanDevice& device() noexcept;

  //! Return the underlying vulkan device
  const zivc::VulkanDevice& device() const noexcept;

  //! Initialize the gui instance
  void initialize(SharedPtr&& parent,
                  WeakPtr&& own,
                  zivc::Platform& zplatform,
                  const GuiApplicationOptions& options);

 protected:
  //! Destroy the platform
  void destroyData() noexcept override;

 private:
  //! Destroy the glfw instance
  void destroyGlfw() noexcept;

  //! Destroy vulkan objects
  void destroyVulkan() noexcept;

  //! Initialize glfw
  void initGlfw(const GuiApplicationOptions& options);

  //! Initialize vulkan
  void initVulkan(zivc::Platform& zplatform);

  //! Initialize vulkan descriptor pool
  void initVulkanDescriptorPool(zivc::Platform& zplatform);

  //! Initialize vulkan device
  void initVulkanDevice(zivc::Platform& zplatform);

  //! Raise a glfw error
  static void raiseGlfwError(const int error, const char* description);

  //! Select a vulkan device for the gui
  static std::size_t selectDevice(const zivc::Platform& zplatform) noexcept;


  GLFWwindow* glfw_window_ = nullptr;
  zivc::SharedDevice device_;
  VkDescriptorPool descriptor_pool_ = ZIVC_VK_NULL_HANDLE;
  VkAllocationCallbacks desc_pool_allocator_;
};

} // namespace example

#endif // EXAMPLE_GUI_PLATFORM_HPP
