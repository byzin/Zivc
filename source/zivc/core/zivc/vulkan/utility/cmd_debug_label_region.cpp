/*!
  \file cmd_debug_label_region.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cmd_debug_label_region.hpp"
// Standard C++ library
#include <array>
#include <memory>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "vulkan_dispatch_loader.hpp"
#include "vulkan_hpp.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] dispatcher No description.
  \param [in] label_name No description.
  \param [in] color No description.
  */
CmdDebugLabelRegion::CmdDebugLabelRegion(const VkCommandBuffer& command_buffer,
                                         const VulkanDispatchLoader& dispatcher,
                                         const std::string_view label_name,
                                         const std::array<float, 4>& color) noexcept
    : command_buffer_{command_buffer},
      dispatcher_{std::addressof(dispatcher)}
{
  begin(label_name, color);
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
CmdDebugLabelRegion::CmdDebugLabelRegion(CmdDebugLabelRegion&& other) noexcept
{
  std::swap(command_buffer_, other.command_buffer_);
  std::swap(dispatcher_, other.dispatcher_);
}

/*!
  \details No detailed description
  */
CmdDebugLabelRegion::~CmdDebugLabelRegion() noexcept
{
  end();
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
CmdDebugLabelRegion& CmdDebugLabelRegion::operator=(CmdDebugLabelRegion&& other) noexcept
{
  end();
  std::swap(command_buffer_, other.command_buffer_);
  std::swap(dispatcher_, other.dispatcher_);
  return *this;
}

/*!
  \details No detailed description
  */
void CmdDebugLabelRegion::end() noexcept
{
  zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer)
    command_buffer.endDebugUtilsLabelEXT(dispatcher_->loader());
  command_buffer_ = ZIVC_VK_NULL_HANDLE;
  dispatcher_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] label_name No description.
  \param [in] color No description.
  */
void CmdDebugLabelRegion::begin(const std::string_view label_name,
                                const std::array<float, 4>& color) noexcept
{
  zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer) {
    const zivcvk::DebugUtilsLabelEXT info{label_name.data(), color};
    command_buffer.beginDebugUtilsLabelEXT(&info, dispatcher_->loader());
  }
}

} // namespace zivc
