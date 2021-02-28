/*!
  \file cmd_debug_label_region.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CMD_DEBUG_LABEL_REGION_HPP
#define ZIVC_CMD_DEBUG_LABEL_REGION_HPP

// Standard C++ library
#include <array>
#include <string_view>
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
class CmdDebugLabelRegion : private zisc::NonCopyable<CmdDebugLabelRegion>
{
 public:
   //! Open a debug label region
   CmdDebugLabelRegion(const VkCommandBuffer& command_buffer,
                       const VulkanDispatchLoader& dispatcher,
                       const std::string_view label_name,
                       const std::array<float, 4>& color) noexcept;

   //! Move data
   CmdDebugLabelRegion(CmdDebugLabelRegion&& other) noexcept;

   //! Close the debug label region
   ~CmdDebugLabelRegion() noexcept;


   //! Move data
   CmdDebugLabelRegion& operator=(CmdDebugLabelRegion&& other) noexcept;


   //! Close the debug label region
   void end() noexcept;

 private:
   //! Open a debug label region
   void begin(const std::string_view label_name,
              const std::array<float, 4>& color) noexcept;


   VkCommandBuffer command_buffer_ = ZIVC_VK_NULL_HANDLE;
   const VulkanDispatchLoader* dispatcher_ = nullptr;
};

} // namespace zivc

#endif // ZIVC_CMD_DEBUG_LABEL_REGION_HPP
