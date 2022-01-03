/*!
  \file gui_application.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_application.hpp"
// Standard C++ library
#include <memory>
#include <stdexcept>
#include <thread>
// ImGui
#include "imgui.h"
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/kernel_set/kernel_set-example.hpp"
// Gui
#include "gui_application_options.hpp"
#include "gui_platform.hpp"
#include "utility/error.hpp"

namespace example {

/*!
  \details No detailed description
  */
GuiApplication::GuiApplication() noexcept
{
}

/*!
  \details No detailed description
  */
GuiApplication::~GuiApplication() noexcept
{
  destroy();
}

/*!
  \details No detailed description
  */
void GuiApplication::draw() noexcept
{
  // 1. Show the big demo window
  if (is_demo_window_active_)
    ImGui::ShowDemoWindow(&is_demo_window_active_);

  static bool show_another_window = false;
  // 2. Show a simple window that we create ourselves.
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, World");
    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Another Window", &show_another_window);
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
//    ImGui::ColorEdit3("clear color", (float*)
    if (ImGui::Button("Button")) {
      [[maybe_unused]] auto make_kernel = [this]() noexcept
      {
        auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(example, testKernel, 1);
        return zdevice_->makeKernel(kernel_params);
      };
      using KernelT = decltype(make_kernel())::element_type;
      auto run_kernel = [this]()
      {
        {
          auto* kernel = zisc::cast<KernelT*>(zkernel_.get());
          auto launch_options = kernel->makeOptions();
          launch_options.setWorkSize({1});
          launch_options.setExternalSyncMode(true);
          launch_options.setLabel("ExampleKernel");
          auto result = kernel->run(*zbuffer1_, *zbuffer2_, 1, launch_options);
          zdevice_->waitForCompletion(result.fence());
        }
        {
          auto options = zbuffer2_->makeOptions();
          options.setSize(1);
          options.setExternalSyncMode(true);
          auto result = zivc::copy(*zbuffer2_, zbuffer_host_.get(), options);
          zdevice_->waitForCompletion(result.fence());
          const auto& mem = zbuffer_host_->mapMemory();
          counter += mem[0];
        }
      };
      if (kernel_thread_ && kernel_thread_->joinable())
        kernel_thread_->join();
      kernel_thread_ = std::make_unique<std::thread>(run_kernel);
//      counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    constexpr float one_sec_ms = 1000.0f;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", one_sec_ms / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // 3. Show another simple window
  if (show_another_window) {
    ImGui::Begin("Another Window", &show_another_window);
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
      show_another_window = false;
    ImGui::End();
  }
}

/*!
  \details No detailed description

  \return No description
  */
int GuiApplication::run()
{
  while (platform()->show(this)) {
    //
  }
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* GuiApplication::memoryResource() noexcept
{
  zisc::pmr::memory_resource* mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* GuiApplication::memoryResource() const noexcept
{
  const zisc::pmr::memory_resource* mem_resource = (custom_mem_resource_ != nullptr)
      ? custom_mem_resource_
      : default_mem_resource_.get();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
GuiPlatform* GuiApplication::platform() noexcept
{
  auto* p = dynamic_cast<GuiPlatform*>(platform_.get());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
const GuiPlatform* GuiApplication::platform() const noexcept
{
  const auto* p = dynamic_cast<const GuiPlatform*>(platform_.get());
  return p;
}

/*!
  \details No detailed description
  */
void GuiApplication::initialize(WeakPtr&& own,
                                zivc::Platform& zplatform,
                                GuiApplicationOptions& options)
{
  // Clear the previous platform data first
  destroy();

  initObject(nullptr, std::move(own));
  setMemoryResource(options.memoryResource());

  auto* mem_resource = memoryResource();
  {
    zisc::pmr::polymorphic_allocator<GuiPlatform> alloc{mem_resource};
    platform_ = std::allocate_shared<GuiPlatform>(alloc);
    SharedObject parent{getOwnPtr()};
    WeakObject own{platform_};
    platform()->initialize(std::move(parent), std::move(own), zplatform, options);
  }

  // Kernel
  {
    // Create a device
    const auto& device_info_list = zplatform.deviceInfoList();
    std::size_t device_index = 0;
    for (std::size_t i = 0; i < device_info_list.size(); ++i) {
      const zivc::DeviceInfo& info = *device_info_list[i];
      if (info.type() == zivc::SubPlatformType::kVulkan) {
        device_index = i;
        break;
      }
    }
    try {
      zdevice_ = zplatform.queryDevice(device_index);
    }
    catch (const std::runtime_error& error) {
      std::cerr << error.what() << std::endl;
    }
  }
  {
    zbuffer_host_ = zdevice_->makeBuffer<zivc::int32b>(zivc::BufferUsage::kHostOnly);
    zbuffer_host_->setSize(1);
    auto mem = zbuffer_host_->mapMemory();
    constexpr int value = 10;
    mem[0] = value;
  }
  {
    zbuffer1_ = zdevice_->makeBuffer<zivc::int32b>(zivc::BufferUsage::kDeviceOnly);
    zbuffer1_->setSize(1);
    auto options = zbuffer_host_->makeOptions();
    options.setSize(1);
    options.setExternalSyncMode(true);
    auto result = zivc::copy(*zbuffer_host_, zbuffer1_.get(), options);
    zdevice_->waitForCompletion(result.fence());
  }
  {
    zbuffer2_ = zdevice_->makeBuffer<zivc::int32b>(zivc::BufferUsage::kDeviceOnly);
    zbuffer2_->setSize(1);
  }
  {
    auto kernel_params = ZIVC_MAKE_KERNEL_INIT_PARAMS(example, testKernel, 1);
    zkernel_ = zdevice_->makeKernel(kernel_params);
  }

  is_demo_window_active_ = true;
}

/*!
  \details No detailed description
  */
void GuiApplication::destroyData() noexcept
{
  if (kernel_thread_ && kernel_thread_->joinable()) {
    kernel_thread_->join();
    kernel_thread_.reset();
  }

  zkernel_.reset();
  zbuffer2_.reset();
  zbuffer1_.reset();
  zbuffer_host_.reset();
  zdevice_.reset();

  platform_.reset();

  default_mem_resource_.reset();
  custom_mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
void GuiApplication::setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept
{
  custom_mem_resource_ = mem_resource;
  if (mem_resource != nullptr)
    default_mem_resource_.reset();
  else
    default_mem_resource_ = std::make_unique<zisc::SimpleMemoryResource>();
}

/*!
  \details No detailed description

  \param [in] options No description.
  \return No description
  */
SharedGuiApp makeGuiApp(zivc::Platform& zplatform,
                        GuiApplicationOptions& options)
{
  SharedGuiApp app;

  // Create an application
  auto* mem_resource = options.memoryResource();
  if (mem_resource != nullptr) {
    zisc::pmr::polymorphic_allocator<GuiApplication> alloc{mem_resource};
    app = std::allocate_shared<GuiApplication>(alloc);
  }
  else {
    app = std::make_shared<GuiApplication>();
  }

  // Initialize the app
  WeakObject own{app};
  try {
    app->initialize(std::move(own), zplatform, options);
  }
  catch (const std::runtime_error& error) {
    app->destroy();
    app.reset();
    throw error;
  }

  return app;
}

} // namespace example
