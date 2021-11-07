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
#include <cstddef>
#include <cstdio>
#include <memory>
#include <utility>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/vulkan/utility/vulkan.hpp"
#include "zivc/vulkan/utility/vulkan_hpp.hpp"
#include "zivc/vulkan/utility/vulkan_dispatch_loader.hpp"
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

  \return No description
  */
zivc::VulkanDevice& GuiPlatform::device() noexcept
{
  auto* d = zisc::cast<zivc::VulkanDevice*>(device_.get());;
  return *d;
}

/*!
  \details No detailed description

  \return No description
  */
const zivc::VulkanDevice& GuiPlatform::device() const noexcept
{
  const auto* d = zisc::cast<const zivc::VulkanDevice*>(device_.get());;
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
                             zivc::Platform& zplatform,
                             const GuiApplicationOptions& options)
{
  // Clear the previous platform data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initVulkan(zplatform);
  initGlfw(options);
}

/*!
  \details No detailed description
  */
void GuiPlatform::destroyData() noexcept
{
  destroyGlfw();
  destroyVulkan();
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
void GuiPlatform::destroyVulkan() noexcept
{
  if (descriptor_pool_ != ZIVC_VK_NULL_HANDLE) {
    auto d = zisc::cast<zivcvk::Device>(device().device());
    const auto* loader = device().dispatcher().loaderImpl();
    zivcvk::AllocationCallbacks alloc{desc_pool_allocator_};
    zivcvk::DescriptorPool desc_pool{descriptor_pool_};
    d.destroyDescriptorPool(desc_pool, alloc, *loader);
    descriptor_pool_ = ZIVC_VK_NULL_HANDLE;
  }

  desc_pool_allocator_ = VkAllocationCallbacks{};
  device_.reset();
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

  \param [in] zplatform No description.
  */
void GuiPlatform::initVulkan(zivc::Platform& zplatform)
{
  initVulkanDevice(zplatform);
  initVulkanDescriptorPool(zplatform);
}

/*!
  \details No detailed description

  \param [in] zplatform No description.
  */
void GuiPlatform::initVulkanDescriptorPool(zivc::Platform& zplatform)
{
  auto* vulkan_platform = zisc::reinterp<zivc::VulkanSubPlatform*>(
      zplatform.subPlatform(zivc::SubPlatformType::kVulkan));
  desc_pool_allocator_ = vulkan_platform->makeAllocator();

  constexpr zivc::uint32b pool_list_size = 11;
  constexpr zivc::uint32b max_pool_size = 1000;
  std::array<zivcvk::DescriptorPoolSize, pool_list_size> pool_size_list{{
    {zivcvk::DescriptorType::eSampler, max_pool_size},
    {zivcvk::DescriptorType::eCombinedImageSampler, max_pool_size},
    {zivcvk::DescriptorType::eSampledImage, max_pool_size},
    {zivcvk::DescriptorType::eStorageImage, max_pool_size},
    {zivcvk::DescriptorType::eUniformTexelBuffer, max_pool_size},
    {zivcvk::DescriptorType::eStorageTexelBuffer, max_pool_size},
    {zivcvk::DescriptorType::eUniformBuffer, max_pool_size},
    {zivcvk::DescriptorType::eStorageBuffer, max_pool_size},
    {zivcvk::DescriptorType::eUniformBufferDynamic, max_pool_size},
    {zivcvk::DescriptorType::eStorageBufferDynamic, max_pool_size},
    {zivcvk::DescriptorType::eInputAttachment, max_pool_size}
  }};

  auto d = zisc::cast<zivcvk::Device>(device().device());
  const auto* loader = device().dispatcher().loaderImpl();
  zivcvk::AllocationCallbacks alloc{desc_pool_allocator_};

  zivcvk::DescriptorPoolCreateInfo info;
  info.setFlags(zivcvk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);
  constexpr zivc::uint32b max_set_size = max_pool_size * pool_list_size;
  info.setMaxSets(max_set_size);
  info.setPoolSizeCount(pool_list_size);
  info.setPPoolSizes(pool_size_list.data());
  zivcvk::DescriptorPool desc_pool = d.createDescriptorPool(info, alloc, *loader);
  descriptor_pool_ = zisc::cast<VkDescriptorPool>(desc_pool);
}

/*!
  \details No detailed description

  \param [in] zplatform No description.
  */
void GuiPlatform::initVulkanDevice(zivc::Platform& zplatform)
{
  auto* vulkan_platform = zisc::reinterp<zivc::VulkanSubPlatform*>(
      zplatform.subPlatform(zivc::SubPlatformType::kVulkan));

  // Create a device
  const std::size_t device_index = selectDevice(zplatform);
  const auto& device_info_list = vulkan_platform->deviceInfoList();
  const zivc::VulkanDeviceInfo& device_info = device_info_list[device_index];
  device_ = vulkan_platform->makeDevice(device_info,
                                        zivc::VulkanDeviceCapability::kGui);
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

/*!
  \details No detailed description

  \param [in] zplatform No description.
  \return No description
  */
std::size_t GuiPlatform::selectDevice(const zivc::Platform& zplatform) noexcept
{
  const auto* vulkan_platform = zisc::reinterp<const zivc::VulkanSubPlatform*>(
      zplatform.subPlatform(zivc::SubPlatformType::kVulkan));
  const auto& device_info_list = vulkan_platform->deviceInfoList();

  // Select primary discrete gpu
  bool is_device_found = false;
  std::size_t device_index = 0;
  for (device_index = 0; device_index < device_info_list.size(); ++device_index) {
    const zivc::VulkanDeviceInfo& device_info = device_info_list[device_index];
    const VkPhysicalDeviceProperties& prop = device_info.properties().properties1_;
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

} // namespace example
