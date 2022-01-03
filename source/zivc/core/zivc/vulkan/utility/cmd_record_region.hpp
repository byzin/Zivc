/*!
  \file cmd_record_region.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CMD_RECORD_REGION_HPP
#define ZIVC_CMD_RECORD_REGION_HPP

// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "vulkan.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class VulkanDispatchLoader;

/*!
  \brief No brief description

  No detailed description.
  */
class CmdRecordRegion : private zisc::NonCopyable<CmdRecordRegion>
{
 public:
  //! Start recording a command buffer
  CmdRecordRegion(const VkCommandBuffer& command_buffer,
                  const VulkanDispatchLoader& dispatcher,
                  const VkCommandBufferUsageFlags flags) noexcept;

  //! Move data
  CmdRecordRegion(CmdRecordRegion&& other) noexcept;

  //! Finish recording a command buffer
  ~CmdRecordRegion() noexcept;


  //! Move data
  CmdRecordRegion& operator=(CmdRecordRegion&& other) noexcept;


  //! Finish recording a command buffer
  void end() noexcept;

 private:
  //! Start recording a command buffer
  void begin(const VkCommandBufferUsageFlags flags);


  VkCommandBuffer command_buffer_ = ZIVC_VK_NULL_HANDLE;
  const VulkanDispatchLoader* dispatcher_ = nullptr;
};

} // namespace zivc

#endif // ZIVC_CMD_RECORD_REGION_HPP
