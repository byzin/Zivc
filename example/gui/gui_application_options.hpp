/*!
  \file gui_application_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_APPLICATION_OPTIONS_HPP
#define EXAMPLE_GUI_APPLICATION_OPTIONS_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace example {

/*!
  \brief No brief description
  */
class GuiApplicationOptions : private zisc::NonCopyable<GuiApplicationOptions>
{
 public:
  //! Create a default application options
  GuiApplicationOptions() noexcept;

  //! Create an application options
  GuiApplicationOptions(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move option data
  GuiApplicationOptions(GuiApplicationOptions&& other) noexcept;


  //! Move option data
  GuiApplicationOptions& operator=(GuiApplicationOptions&& other) noexcept;


  //! Return the application description
  std::string_view description() const noexcept;

  //! Enable unlimited frame rate mode
  void enableUnlimitedFrameRate(const bool flag) noexcept;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Set the application description
  void setDescription(const std::string_view desc) noexcept;

  //! Set memory resource used in an application
  void setMemoryResource(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Set the window height size
  void setWindowHeight(const std::size_t size) noexcept;

  //! Set the window title
  void setWindowTitle(const std::string_view title) noexcept;

  //! Set the window width size
  void setWindowWidth(const std::size_t size) noexcept;

  //! Check if unlimited frame rate mode is enabled
  bool unlimitedFrameRateEnabled() const noexcept;

  //! Return the window height size
  std::size_t windowHeight() const noexcept;

  //! Return the window title
  std::string_view windowTitle() const noexcept;

  //! Return the window width size
  std::size_t windowWidth() const noexcept;

 private:
  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::string description_;
  std::string window_title_;
  std::size_t window_width_ = 1280;
  std::size_t window_height_ = 720;
  zivc::uint8b unlimited_frame_rate_enabled_ = zisc::kTrue;
};

} // namespace example

#endif // EXAMPLE_GUI_APPLICATION_OPTIONS_HPP
