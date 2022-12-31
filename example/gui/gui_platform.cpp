/*!
  \file gui_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_platform.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstdio>
#include <limits>
#include <memory>
#include <utility>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/utility.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/vulkan/utility/vulkan.hpp"
#include "zivc/vulkan/utility/vulkan_hpp.hpp"
#include "zivc/vulkan/utility/vulkan_dispatch_loader.hpp"
// GLFW
#include "utility/glfw.hpp"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
// Gui
#include "gui_application_options.hpp"
#include "gui_widget.hpp"
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

  \return No description
  */
zivc::VulkanDevice& GuiPlatform::device() noexcept
{
  auto* d = static_cast<zivc::VulkanDevice*>(device_.get());;
  return *d;
}

/*!
  \details No detailed description

  \return No description
  */
const zivc::VulkanDevice& GuiPlatform::device() const noexcept
{
  const auto* d = static_cast<const zivc::VulkanDevice*>(device_.get());;
  return *d;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] options No description.
  */
void GuiPlatform::initialize(SharedPtr&& parent,
                             WeakPtr&& own,
                             zivc::Context& context,
                             const GuiApplicationOptions& options)
{
  // Clear the previous platform data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initVulkan(context);
  initGlfw(options);
  initVulkanWindow(options);
  initImGui(options);
}

/*!
  \details No detailed description

  \param [in] main_window No description.
  \return No description
  */
bool GuiPlatform::show(GuiWidget* main_window)
{
  const bool should_close_window = glfwWindowShouldClose(glfw_window_) != 0;
  if (should_close_window)
    return false;

  // Pool and handle events (inputs, window resize, etc.)
  glfwPollEvents();

  // Resize swap chain?
  if (isSwapChainRebuilt()) {
    resizeSwapChain();
    setSwapChainRebuild(false);
  }

  // Start the Dear ImGui frame
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Draw main window
  main_window->draw();

  // Rendering a frame
  renderFrame();

  return true;
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyData() noexcept
{
  if (device_)
    device().waitForCompletion();

  destroyImGui();
  destroyVulkanWindow();
  destroyGlfw();
  destroyVulkan();
}

/*!
  \details No detailed description

  \param [in] result No description.
  */
void GuiPlatform::checkVulkanResult(const VkResult result)
{
  ::example::checkVulkanResult(static_cast<int>(result));
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
  */
void GuiPlatform::destroyImGui() noexcept
{
  if (imgui_context_ != nullptr) {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imgui_context_);
    imgui_context_ = nullptr;
  }
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyVulkan() noexcept
{
  if (descriptor_pool_ != ZIVC_VK_NULL_HANDLE) {
    auto d = static_cast<zivc::vk::Device>(device().device());
    const zivc::vk::AllocationCallbacks alloc{vk_allocator_};
    const zivc::vk::DescriptorPool desc_pool{descriptor_pool_};
    d.destroyDescriptorPool(desc_pool, alloc, dispatcher().loader());
    descriptor_pool_ = ZIVC_VK_NULL_HANDLE;
  }

  vk_allocator_ = VkAllocationCallbacks{};
  device_.reset();
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyVulkanWindow() noexcept
{
  if (window_data_) {
    VkDevice d = device().device();
    const VkAllocationCallbacks* alloc = &vk_allocator_;
    ImGui_ImplVulkanH_DestroyWindow(instance(), d, &windowData(), alloc);
  }
  setMinImageCount(-1);
  window_data_.reset();
}

/*!
  \details No detailed description

  \return No description
  */
const zivc::VulkanDispatchLoader& GuiPlatform::dispatcher() const noexcept
{
  return device().dispatcher();
}

/*!
  \details No detailed description

  \return No description
  */
int GuiPlatform::minImageCount() const noexcept
{
  return min_image_count_;
}

/*!
  \details No detailed description

  \param [in] options No description.
  */
void GuiPlatform::initGlfw(const GuiApplicationOptions& options)
{
  // Check type
  {
    using WSurfaceType = zivc::VulkanBackend::WindowSurfaceType;
    using GSurfaceType = GlfwSurfaceType;
    constexpr auto is_equal = [](WSurfaceType lhs, GlfwSurfaceType rhs) noexcept
    {
      return static_cast<int>(lhs) == static_cast<int>(rhs);
    };
    static_assert(is_equal(WSurfaceType::kWin32, GSurfaceType::kWin32));
    static_assert(is_equal(WSurfaceType::kMacOS, GSurfaceType::kMacOS));
    static_assert(is_equal(WSurfaceType::kMetal, GSurfaceType::kMetal));
    static_assert(is_equal(WSurfaceType::kXlib, GSurfaceType::kXlib));
    static_assert(is_equal(WSurfaceType::kXcb, GSurfaceType::kXcb));
    static_assert(is_equal(WSurfaceType::kWayland, GSurfaceType::kWayland));
  }

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
  // Setup GLFW vulkan
  VkSurfaceKHR surface = ZIVC_VK_NULL_HANDLE;
  {
    VkInstance ins = instance();
    auto* proc_addr = zisc::reinterp<void*>(dispatcher().instanceProcAddr());
    const auto surface_type = static_cast<GlfwSurfaceType>(surfaceType());
    auto* s = zisc::reinterp<void**>(&surface);
    initGlfwVulkan(ins, proc_addr, glfw_window_, &vk_allocator_, surface_type, s);
  }

  // 
  {
    auto* mem_resource = memoryResource();
    const zisc::pmr::polymorphic_allocator<ImGui_ImplVulkanH_Window> alloc{mem_resource};
    window_data_ = std::allocate_shared<ImGui_ImplVulkanH_Window>(alloc);
    windowData().Surface = surface;
  }
}

/*!
  \details No detailed description

  \param [in] options No description.
  */
void GuiPlatform::initImGui([[maybe_unused]] const GuiApplicationOptions& options)
{
  // Setup Dear ImGui context
  {
    IMGUI_CHECKVERSION();
    imgui_context_ = ImGui::CreateContext();

    // IO
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  }

  // Style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  {
    constexpr bool install_callbacks = true;
    ImGui_ImplGlfw_InitForVulkan(glfw_window_, install_callbacks);
    ImGui_ImplVulkan_InitInfo init_info{};
    init_info.Instance = instance();
    const zivc::VulkanDeviceInfo& info = device().deviceInfoImpl();
    init_info.PhysicalDevice = info.device();
    init_info.Device = device().device();
    init_info.QueueFamily = queueFamilyIndexForGui();
    init_info.Queue = queueForGui();
    init_info.PipelineCache = ZIVC_VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptor_pool_;
    init_info.MinImageCount = minImageCount();
    init_info.ImageCount = windowData().ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = &vk_allocator_;
    init_info.CheckVkResultFn = GuiPlatform::checkVulkanResult;
    ImGui_ImplVulkan_Init(&init_info, windowData().RenderPass);
  }

  // Upload Fonts
  {
    const ImGui_ImplVulkanH_Window& wd = windowData();

    //
    auto command_pool = zisc::cast<zivc::vk::CommandPool>(
        wd.Frames[wd.FrameIndex].CommandPool);
    const zivc::vk::CommandPoolResetFlags reset_flags{};
    auto d = zisc::cast<zivc::vk::Device>(device().device());
    d.resetCommandPool(command_pool, reset_flags, dispatcher().loader());

    //
    auto command_buffer = zisc::cast<zivc::vk::CommandBuffer>(
        wd.Frames[wd.FrameIndex].CommandBuffer);
    zivc::vk::CommandBufferBeginInfo begin_info{};
    begin_info.setFlags(zivc::vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    const zivc::vk::Result result = command_buffer.begin(&begin_info,
                                                       dispatcher().loader());
    checkVulkanResult(static_cast<VkResult>(result));
    ImGui_ImplVulkan_CreateFontsTexture(zisc::cast<VkCommandBuffer>(command_buffer));
    command_buffer.end(dispatcher().loader());

    //
    zivc::vk::SubmitInfo submit_info{};
    submit_info.setCommandBuffers(command_buffer);
    auto q = zisc::cast<zivc::vk::Queue>(queueForGui());
    q.submit(submit_info, nullptr, dispatcher().loader());
    device().waitForCompletion(zivc::VulkanDeviceCapability::kGui, 0);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
  }

  setSwapChainRebuild(false);

  // Set background color
  constexpr std::array<float, 4> color{{0.45f, 0.55f, 0.60f, 1.0f}};
  windowData().ClearValue.color.float32[0] = color[0];
  windowData().ClearValue.color.float32[1] = color[1];
  windowData().ClearValue.color.float32[2] = color[2];
  windowData().ClearValue.color.float32[3] = color[3];
}

/*!
  \details No detailed description

  \param [in] context No description.
  */
void GuiPlatform::initVulkan(zivc::Context& context)
{
  initVulkanDevice(context);
  initVulkanDescriptorPool(context);
}

/*!
  \details No detailed description
  */
void GuiPlatform::initVulkanWindow(const GuiApplicationOptions& options)
{
  // Initialize imgui vulkan
  {
    auto loader_func = [](const char* function_name, void* user_data) noexcept
    {
      const auto* platform = static_cast<const GuiPlatform*>(user_data);
      VkInstance instance = platform->instance();
      const zivc::VulkanDispatchLoader& dispatcher = platform->dispatcher();
      const PFN_vkGetInstanceProcAddr proc_addr = dispatcher.instanceProcAddr();
      return proc_addr(instance, function_name);
    };
    const bool result = ImGui_ImplVulkan_LoadFunctions(loader_func, this);
    if (!result)
      raiseImguiError("Loading vulkan functions failed.");
  }
  const zivc::VulkanDeviceInfo& info = device().deviceInfoImpl();
  // Check for WSI support
  {
    auto physical_device = zisc::cast<zivc::vk::PhysicalDevice>(info.device());
    const zivc::uint32b q_index = queueFamilyIndexForGui();
    auto s = zisc::cast<zivc::vk::SurfaceKHR>(windowData().Surface);
    const auto& loader = dispatcher().loader();
    const zivc::vk::Bool32 result = physical_device.getSurfaceSupportKHR(q_index,
                                                                       s,
                                                                       loader);
    if (result != VK_TRUE)
      raiseImguiError("No WSI support on physical device 0");
  }
  // Select surface format
  {
    const std::array<VkFormat, 4> requestSurfaceImageFormat{{
      VK_FORMAT_B8G8R8A8_UNORM,
      VK_FORMAT_R8G8B8A8_UNORM,
      VK_FORMAT_B8G8R8_UNORM,
      VK_FORMAT_R8G8B8_UNORM}};
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    const VkSurfaceFormatKHR f = ImGui_ImplVulkanH_SelectSurfaceFormat(
        info.device(),
        windowData().Surface,
        requestSurfaceImageFormat.data(),
        zisc::cast<int>(requestSurfaceImageFormat.size()),
        requestSurfaceColorSpace);
    windowData().SurfaceFormat = f;
  }
  // Select present mode
  {
    const std::array<VkPresentModeKHR, 3> present_modes{{
      VK_PRESENT_MODE_FIFO_KHR,
      VK_PRESENT_MODE_MAILBOX_KHR,
      VK_PRESENT_MODE_IMMEDIATE_KHR}};
    const std::size_t n_present_modes = options.unlimitedFrameRateEnabled()
        ? present_modes.size()
        : 1;
    const VkPresentModeKHR mode = ImGui_ImplVulkanH_SelectPresentMode(
        info.device(),
        windowData().Surface,
        present_modes.data(),
        zisc::cast<int>(n_present_modes));
    windowData().PresentMode = mode;
  }
  // Create SwapChain, RenderPass, Framebuffer, etc.
  {
    setMinImageCount(2);
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(glfw_window_, &width, &height);
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance(),
                                           info.device(),
                                           device().device(),
                                           &windowData(),
                                           queueFamilyIndexForGui(),
                                           &vk_allocator_,
                                           width,
                                           height,
                                           minImageCount());
  }
}

/*!
  \details No detailed description

  \param [in] context No description.
  */
void GuiPlatform::initVulkanDescriptorPool(zivc::Context& context)
{
  auto* vulkan_platform = zisc::reinterp<zivc::VulkanBackend*>(
      context.backend(zivc::BackendType::kVulkan));
  vk_allocator_ = vulkan_platform->createAllocator();

  constexpr zivc::uint32b pool_list_size = 11;
  constexpr zivc::uint32b max_pool_size = 1000;
  std::array<zivc::vk::DescriptorPoolSize, pool_list_size> pool_size_list{{
    {zivc::vk::DescriptorType::eSampler, max_pool_size},
    {zivc::vk::DescriptorType::eCombinedImageSampler, max_pool_size},
    {zivc::vk::DescriptorType::eSampledImage, max_pool_size},
    {zivc::vk::DescriptorType::eStorageImage, max_pool_size},
    {zivc::vk::DescriptorType::eUniformTexelBuffer, max_pool_size},
    {zivc::vk::DescriptorType::eStorageTexelBuffer, max_pool_size},
    {zivc::vk::DescriptorType::eUniformBuffer, max_pool_size},
    {zivc::vk::DescriptorType::eStorageBuffer, max_pool_size},
    {zivc::vk::DescriptorType::eUniformBufferDynamic, max_pool_size},
    {zivc::vk::DescriptorType::eStorageBufferDynamic, max_pool_size},
    {zivc::vk::DescriptorType::eInputAttachment, max_pool_size}
  }};

  auto d = zisc::cast<zivc::vk::Device>(device().device());
  const auto& loader = dispatcher().loader();
  const zivc::vk::AllocationCallbacks alloc{vk_allocator_};

  zivc::vk::DescriptorPoolCreateInfo info;
  info.setFlags(zivc::vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);
  constexpr zivc::uint32b max_set_size = max_pool_size * pool_list_size;
  info.setMaxSets(max_set_size);
  info.setPoolSizeCount(pool_list_size);
  info.setPPoolSizes(pool_size_list.data());
  const zivc::vk::DescriptorPool desc_pool = d.createDescriptorPool(info, alloc, loader);
  descriptor_pool_ = zisc::cast<VkDescriptorPool>(desc_pool);
}

/*!
  \details No detailed description

  \param [in] context No description.
  */
void GuiPlatform::initVulkanDevice(zivc::Context& context)
{
  // Create a device
  const std::size_t device_index = selectDevice(context);
  device_ = context.queryDevice(device_index);
}

/*!
  \details No detailed description

  \return No description
  */
VkInstance GuiPlatform::instance() const noexcept
{
  const auto* platform =
      static_cast<const zivc::VulkanBackend*>(device().getParent());
  return platform->instance();
}

/*!
  \details No detailed description

  \return No description
  */
bool GuiPlatform::isSwapChainRebuilt() const noexcept
{
  return is_swap_chain_rebuilt_;
}

/*!
  \details No detailed description
  */
void GuiPlatform::presentFrameImpl()
{
  if (isSwapChainRebuilt())
    return;

  ImGui_ImplVulkanH_Window& wd = windowData();

  auto render_complete_semaphore = zisc::cast<zivc::vk::Semaphore>(
      wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore);
  auto swapchain = zisc::cast<zivc::vk::SwapchainKHR>(wd.Swapchain);
  zivc::vk::PresentInfoKHR info{};
  info.setWaitSemaphores(render_complete_semaphore);
  info.setSwapchains(swapchain);
  info.setPImageIndices(&wd.FrameIndex);

  auto q = zisc::cast<zivc::vk::Queue>(queueForGui());
  const zivc::vk::Result result = q.presentKHR(info, dispatcher().loader());
  if ((result == zivc::vk::Result::eErrorOutOfDateKHR) ||
      (result == zivc::vk::Result::eSuboptimalKHR)) {
    setSwapChainRebuild(true);
    return;
  }
  checkVulkanResult(static_cast<VkResult>(result));
  wd.SemaphoreIndex = (wd.SemaphoreIndex + 1) % wd.ImageCount; // Now we can use the next set of semaphores
}

zivc::uint32b GuiPlatform::queueFamilyIndexForGui() const noexcept
{
  constexpr auto cap = zivc::VulkanDeviceCapability::kGui;
  const zivc::uint32b index = device().queueFamilyIndex(cap);
  return index;
}

/*!
  \details No detailed description

  \return No description
  */
VkQueue GuiPlatform::queueForGui() const noexcept
{
  constexpr auto cap = zivc::VulkanDeviceCapability::kGui;
  VkQueue q = device().getQueue(cap, 0);
  return q;
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
  std::snprintf(desc.data(), max_desc_size,  "Glfw Error %d: %s\n", error, description);
  throw SystemError{ErrorCode::kGlfwInitializationFailed, desc.data()};
}

/*!
  \details No detailed description

  \param [in] description No description.
  */
void GuiPlatform::raiseImguiError(const char* description)
{
  constexpr std::size_t max_desc_size = 1024;
  std::array<char, max_desc_size> desc;
  std::snprintf(desc.data(), max_desc_size, "Imgui Error: %s\n", description);
  throw SystemError{ErrorCode::kImGuiInitializationFailed, desc.data()};
}

/*!
  \details No detailed description
  */
void GuiPlatform::renderFrame()
{
  // Rendering
  ImGui::Render();
  ImDrawData* main_draw_data = ImGui::GetDrawData();
  const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f) ||
                                 (main_draw_data->DisplaySize.y <= 0.0f);
  if (!main_is_minimized)
    renderFrameImpl(main_draw_data);

  // Update and Render additional Platform Windows
  const ImGuiIO& io = ImGui::GetIO();
  constexpr auto viewport_flag = ImGuiConfigFlags_ViewportsEnable;
  if ((io.ConfigFlags & viewport_flag) == viewport_flag) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  // Present Main Platform Window
  if (!main_is_minimized)
    presentFrameImpl();
}

/*!
  \details No detailed description
  */
void GuiPlatform::renderFrameImpl(ImDrawData* draw_data)
{
  ImGui_ImplVulkanH_Window& wd = windowData();
  auto d = zisc::cast<zivc::vk::Device>(device().device());

  auto image_acquired_semaphre = zisc::cast<zivc::vk::Semaphore>(
      wd.FrameSemaphores[wd.SemaphoreIndex].ImageAcquiredSemaphore);
  auto render_complete_semaphore = zisc::cast<zivc::vk::Semaphore>(
      wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore);
  constexpr auto timeout = (std::numeric_limits<zivc::uint64b>::max)();

  {
    auto swapchain = zisc::cast<zivc::vk::SwapchainKHR>(wd.Swapchain);
    const zivc::vk::Result result = d.acquireNextImageKHR(swapchain,
                                                        timeout,
                                                        image_acquired_semaphre,
                                                        nullptr,
                                                        &wd.FrameIndex,
                                                        dispatcher().loader());
    if ((result == zivc::vk::Result::eErrorOutOfDateKHR) ||
        (result == zivc::vk::Result::eSuboptimalKHR)) {
      setSwapChainRebuild(true);
      return;
    }
    checkVulkanResult(static_cast<VkResult>(result));
  }

  ImGui_ImplVulkanH_Frame* fd = &wd.Frames[wd.FrameIndex];
  auto fence = zisc::cast<zivc::vk::Fence>(fd->Fence);
  {
    const zivc::vk::Result result = d.waitForFences(fence,
                                                  VK_TRUE,
                                                  timeout,
                                                  dispatcher().loader());
    checkVulkanResult(static_cast<VkResult>(result));
    d.resetFences(fence, dispatcher().loader());
  }
  auto command_buffer = zisc::cast<zivc::vk::CommandBuffer>(fd->CommandBuffer);
  {
    auto command_pool = zisc::cast<zivc::vk::CommandPool>(fd->CommandPool);
    const zivc::vk::CommandPoolResetFlags reset_flags{};
    d.resetCommandPool(command_pool, reset_flags, dispatcher().loader());

    zivc::vk::CommandBufferBeginInfo info{};
    info.setFlags(zivc::vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    command_buffer.begin(info, dispatcher().loader());
  }
  {
    zivc::vk::RenderPassBeginInfo info{};
    info.setRenderPass(zisc::cast<zivc::vk::RenderPass>(wd.RenderPass));
    info.setFramebuffer(zisc::cast<zivc::vk::Framebuffer>(fd->Framebuffer));
    const zivc::vk::Extent2D extent{{zisc::cast<zivc::uint32b>(wd.Width),
                                   zisc::cast<zivc::uint32b>(wd.Height)}};
    info.renderArea.setExtent(extent);
    info.setClearValues(*zisc::reinterp<const zivc::vk::ClearValue*>(&wd.ClearValue));

    constexpr auto flag = zivc::vk::SubpassContents::eInline;
    command_buffer.beginRenderPass(info, flag, dispatcher().loader());
  }

  // Record dear imgui primitives into command buffer
  ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

  {
    command_buffer.endRenderPass(dispatcher().loader());
    command_buffer.end(dispatcher().loader());
  }

  // Submit command buffer
  {
    const zivc::vk::PipelineStageFlags wait_stage =
        zivc::vk::PipelineStageFlagBits::eColorAttachmentOutput;
    zivc::vk::SubmitInfo info{};
    info.setWaitSemaphores(image_acquired_semaphre);
    info.setPWaitDstStageMask(&wait_stage);
    info.setCommandBuffers(command_buffer);
    info.setSignalSemaphores(render_complete_semaphore);
    auto q = zisc::cast<zivc::vk::Queue>(queueForGui());
    q.submit(info, fence, dispatcher().loader());
  }
}

/*!
  \details No detailed description
  */
void GuiPlatform::resizeSwapChain() noexcept
{
  int width = -1;
  int height = -1;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  if ((0 < width) && (0 < height)) {
    ImGui_ImplVulkan_SetMinImageCount(minImageCount());
    const zivc::VulkanDeviceInfo& info = device().deviceInfoImpl();
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance(),
                                           info.device(),
                                           device().device(),
                                           &windowData(),
                                           queueFamilyIndexForGui(),
                                           &vk_allocator_,
                                           width,
                                           height,
                                           minImageCount());
  }
}

/*!
  \details No detailed description

  \param [in] context No description.
  \return No description
  */
std::size_t GuiPlatform::selectDevice(const zivc::Context& context) noexcept
{
  // Find first discrete gpu
  const auto& device_info_list = context.deviceInfoList();
  bool is_device_found = false;
  std::size_t device_index = 0;
  for (device_index = 0; device_index < device_info_list.size(); ++device_index) {
    const zivc::DeviceInfo* info = device_info_list[device_index];
    if (info->type() != zivc::BackendType::kVulkan)
      continue;
    const auto* device_info = static_cast<const zivc::VulkanDeviceInfo*>(info);
    const VkPhysicalDeviceProperties& prop = device_info->properties().properties1_;
    if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      is_device_found = true;
      break;
    }
  }

  // If there is no discrete gpu, then just use primary gpu
  if (!is_device_found)
    device_index = 0;

  return device_index;
}

/*!
  \details No detailed description

  \param [in] image_count No description.
  */
void GuiPlatform::setMinImageCount(const int image_count) noexcept
{
  min_image_count_ = image_count;
}

/*!
  \details No detailed description

  \param [in] is_rebuilt No description.
  */
void GuiPlatform::setSwapChainRebuild(const bool is_rebuilt) noexcept
{
  is_swap_chain_rebuilt_ = is_rebuilt;
}

/*!
  \details No detailed description

  \return No description
  */
zivc::VulkanBackend::WindowSurfaceType GuiPlatform::surfaceType() const noexcept
{
  const auto* platform =
      static_cast<const zivc::VulkanBackend*>(device().getParent());
  return platform->windowSurfaceType();
}

/*!
  \details No detailed description

  \return No description
  */
ImGui_ImplVulkanH_Window& GuiPlatform::windowData() noexcept
{
  return *window_data_;
}

/*!
  \details No detailed description

  \return No description
  */
const ImGui_ImplVulkanH_Window& GuiPlatform::windowData() const noexcept
{
  return *window_data_;
}

} // namespace example
