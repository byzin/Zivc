/*!
  \file cmd_record_region.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cmd_record_region.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "vulkan_dispatch_loader.hpp"
#include "vulkan_hpp.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] command_buffer No description.
  \param [in] flag No description.
  \param [in] dispatcher No description.
  */
CmdRecordRegion::CmdRecordRegion(const VkCommandBuffer& command_buffer,
                                 const VulkanDispatchLoader& dispatcher,
                                 const VkCommandBufferUsageFlags flag) noexcept :
    command_buffer_{command_buffer},
    dispatcher_{std::addressof(dispatcher)}
{
  begin(flag);
}

/*!
  \details No detailed description
  */
CmdRecordRegion::~CmdRecordRegion() noexcept
{
  end();
}

/*!
  \details No detailed description
  */
void CmdRecordRegion::end() noexcept
{
  zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer) {
    const auto loader = dispatcher_->loaderImpl();
    command_buffer.end(*loader);
  }
  command_buffer_ = VK_NULL_HANDLE;
  dispatcher_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
void CmdRecordRegion::begin(const VkCommandBufferUsageFlags flag) noexcept
{
  zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer) {
    const auto loader = dispatcher_->loaderImpl();
    const zivcvk::CommandBufferBeginInfo info{
        zisc::cast<zivcvk::CommandBufferUsageFlags>(flag)};
    auto result = command_buffer.begin(std::addressof(info), *loader);
  }
}

} // namespace zivc
