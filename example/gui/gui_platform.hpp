/*!
  \file gui_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_PLATFORM_HPP
#define EXAMPLE_GUI_PLATFORM_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/boolean.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/vulkan/vulkan.hpp"
// Gui
#include "gui_object.hpp"

// Forward declaration
// GLFW
struct GLFWwindow;
// ImGui
struct ImDrawData;
struct ImGuiContext;
struct ImGui_ImplVulkanH_Window;
//
namespace zivc {
class Context;
} // namespace zivc

namespace example {

// Forward declaration
class GuiApplicationOptions;
class GuiWidget;

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
                  zivc::Context& context,
                  const GuiApplicationOptions& options);

  //! Show main window
  bool show(GuiWidget* main_window);

 protected:
  //! Destroy the platform
  void destroyData() noexcept override;

 private:
  //! Check vulkan result
  static void checkVulkanResult(const VkResult result);

  //! Destroy the glfw instance
  void destroyGlfw() noexcept;

  //! Destroy the ImGui instance
  void destroyImGui() noexcept;

  //! Destroy vulkan objects
  void destroyVulkan() noexcept;

  //! Destroy vulkan window objects
  void destroyVulkanWindow() noexcept;

  //! Return the vulkan dispatcher
  const zivc::VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return underlying minimum image count
  int minImageCount() const noexcept;

  //! Initialize glfw
  void initGlfw(const GuiApplicationOptions& options);

  //! Initialize ImGui
  void initImGui(const GuiApplicationOptions& options);

  //! Initialize vulkan
  void initVulkan(zivc::Context& context);

  //! Initialize vulkan window
  void initVulkanWindow(const GuiApplicationOptions& options);

  //! Initialize vulkan descriptor pool
  void initVulkanDescriptorPool(zivc::Context& context);

  //! Initialize vulkan device
  void initVulkanDevice(zivc::Context& context);

  //! Return the instance used for the gui
  VkInstance instance() const noexcept;

  //! Check if the swap chain is rebuilt
  bool isSwapChainRebuilt() const noexcept;

  //! Core implementation of presentation
  void presentFrameImpl();

  //! Return the queue family index for GUI
  zivc::uint32b queueFamilyIndexForGui() const noexcept;

  //! Return the queue for GUI
  VkQueue queueForGui() const noexcept;

  //! Raise a glfw error
  static void raiseGlfwError(const int error, const char* description);

  //! Raise a glfw error
  static void raiseImguiError(const char* description);

  //! Render a frame
  void renderFrame();

  //! Core implementation of rendering a frame
  void renderFrameImpl(ImDrawData* draw_data);

  //! Resize swap chain
  void resizeSwapChain() noexcept;

  //! Select a vulkan device for the gui
  static std::size_t selectDevice(const zivc::Context& context) noexcept;

  //! Set minimum image count
  void setMinImageCount(const int image_count) noexcept;

  //! Set swap chain rebuild flag
  void setSwapChainRebuild(const bool is_rebuilt) noexcept;

  //! Return the window surface type
  zivc::VulkanBackend::WindowSurfaceType surfaceType() const noexcept;

  //! Return the underlying window data
  ImGui_ImplVulkanH_Window& windowData() noexcept;

  //! Return the underlying window data
  const ImGui_ImplVulkanH_Window& windowData() const noexcept;


  // ImGui
  std::shared_ptr<ImGui_ImplVulkanH_Window> window_data_;
  ImGuiContext* imgui_context_ = nullptr;
  zivc::int32b min_image_count_ = -1;
  zisc::Boolean is_swap_chain_rebuilt_;
  // GLFW
  GLFWwindow* glfw_window_ = nullptr;
  // Vulkan
  zivc::SharedDevice device_;
  VkDescriptorPool descriptor_pool_ = ZIVC_VK_NULL_HANDLE;
  VkAllocationCallbacks vk_allocator_;
};

} // namespace example

#endif // EXAMPLE_GUI_PLATFORM_HPP
