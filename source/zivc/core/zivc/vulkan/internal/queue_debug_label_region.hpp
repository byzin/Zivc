/*!
  \file queue_debug_label_region.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_QUEUE_DEBUG_LABEL_REGION_HPP
#define ZIVC_QUEUE_DEBUG_LABEL_REGION_HPP

// Standard C++ library
#include <span>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "zivc/zivc_config.hpp"
#include "zivc/vulkan/utility/vulkan.hpp"

namespace zivc {

// Forward declaration
class VulkanDispatchLoader;

} /* namespace zivc */

namespace zivc::internal {

/*!
  \brief No brief description

  No detailed description.
  */
class QueueDebugLabelRegion : private zisc::NonCopyable<QueueDebugLabelRegion>
{
 public:
  //! Open a debug label region
  QueueDebugLabelRegion(const VkQueue& q,
                        const VulkanDispatchLoader& dispatcher,
                        const std::string_view label_name,
                        const std::span<const float, 4> color) noexcept;

  //! Move data
  QueueDebugLabelRegion(QueueDebugLabelRegion&& other) noexcept;

  //! Close the debug label region
  ~QueueDebugLabelRegion() noexcept;


  //! Move data
  QueueDebugLabelRegion& operator=(QueueDebugLabelRegion&& other) noexcept;


  //! Close the debug label region
  void end() noexcept;

 private:
  //! Open a debug label region
  void begin(const std::string_view label_name,
             const std::span<const float, 4> color) noexcept;


  VkQueue queue_ = ZIVC_VK_NULL_HANDLE;
  const VulkanDispatchLoader* dispatcher_ = nullptr;
};

} // namespace zivc::internal

#endif // ZIVC_QUEUE_DEBUG_LABEL_REGION_HPP
