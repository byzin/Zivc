/*!
  \file gui_application_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_application_options.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace example {

/*!
  \details No detailed description
  */
GuiApplicationOptions::GuiApplicationOptions() noexcept :
    GuiApplicationOptions(nullptr)
{
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
GuiApplicationOptions::GuiApplicationOptions(
    zisc::pmr::memory_resource* mem_resource) noexcept :
        mem_resource_{mem_resource}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
GuiApplicationOptions::GuiApplicationOptions(GuiApplicationOptions&& other) noexcept
{
  mem_resource_ = other.memoryResource();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
GuiApplicationOptions& GuiApplicationOptions::operator=(GuiApplicationOptions&& other)
    noexcept
{
  mem_resource_ = other.memoryResource();
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view GuiApplicationOptions::description() const noexcept
{
  return description_;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
void GuiApplicationOptions::enableUnlimitedFrameRate(const bool flag) noexcept
{
  unlimited_frame_rate_enabled_ = flag;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* GuiApplicationOptions::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* GuiApplicationOptions::memoryResource() const noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
void GuiApplicationOptions::setMemoryResource(
    zisc::pmr::memory_resource* mem_resource) noexcept
{
  mem_resource_ = mem_resource;
}

/*!
  \details No detailed description

  \param [in] desc No description.
  */
void GuiApplicationOptions::setDescription(const std::string_view desc) noexcept
{
  description_ = desc;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
void GuiApplicationOptions::setWindowHeight(const std::size_t size) noexcept
{
  window_height_ = size;
}

/*!
  \details No detailed description

  \param [in] title No description.
  */
void GuiApplicationOptions::setWindowTitle(const std::string_view title) noexcept
{
  window_title_ = title;
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
void GuiApplicationOptions::setWindowWidth(const std::size_t size) noexcept
{
  window_width_ = size;
}

/*!
  \details No detailed description

  \return No description
  */
bool GuiApplicationOptions::unlimitedFrameRateEnabled() const noexcept
{
  return unlimited_frame_rate_enabled_;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t GuiApplicationOptions::windowHeight() const noexcept
{
  return window_height_;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view GuiApplicationOptions::windowTitle() const noexcept
{
  return window_title_;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t GuiApplicationOptions::windowWidth() const noexcept
{
  return window_width_;
}

} // namespace example
