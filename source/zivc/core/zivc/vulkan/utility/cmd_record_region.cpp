/*!
  \file cmd_record_region.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cmd_record_region.hpp"
// Standard C++ library
#include <memory>
#include <utility>
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
  \param [in] flags No description.
  \param [in] dispatcher No description.
  */
CmdRecordRegion::CmdRecordRegion(const VkCommandBuffer& command_buffer,
                                 const VulkanDispatchLoader& dispatcher,
                                 const VkCommandBufferUsageFlags flags) noexcept :
    command_buffer_{command_buffer},
    dispatcher_{std::addressof(dispatcher)}
{
  begin(flags);
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
CmdRecordRegion::CmdRecordRegion(CmdRecordRegion&& other) noexcept
{
  std::swap(command_buffer_, other.command_buffer_);
  std::swap(dispatcher_, other.dispatcher_);
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

  \param [in,out] other No description.
  \return No description
  */
CmdRecordRegion& CmdRecordRegion::operator=(CmdRecordRegion&& other) noexcept
{
  end();
  std::swap(command_buffer_, other.command_buffer_);
  std::swap(dispatcher_, other.dispatcher_);
  return *this;
}

/*!
  \details No detailed description
  */
void CmdRecordRegion::end() noexcept
{
  const zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer)
    command_buffer.end(dispatcher_->loader());
  command_buffer_ = ZIVC_VK_NULL_HANDLE;
  dispatcher_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] flags No description.
  */
void CmdRecordRegion::begin(const VkCommandBufferUsageFlags flags)
{
  const zivcvk::CommandBuffer command_buffer{command_buffer_};
  if (command_buffer) {
    const zivcvk::CommandBufferBeginInfo info{
        zisc::cast<zivcvk::CommandBufferUsageFlags>(flags)};
    command_buffer.begin(info, dispatcher_->loader());
  }
}

} // namespace zivc
