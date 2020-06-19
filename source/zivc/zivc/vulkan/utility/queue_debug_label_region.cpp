/*!
  \file queue_begin_label_region.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "queue_debug_label_region.hpp"
// Standard C++ library
#include <array>
#include <memory>
#include <string_view>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "vulkan.hpp"
#include "vulkan_dispatch_loader.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] q No description.
  \param [in] dispatcher No description.
  \param [in] label_name No description.
  \param [in] color No description.
  */
QueueDebugLabelRegion::QueueDebugLabelRegion(const VkQueue& q,
                                             const VulkanDispatchLoader& dispatcher,
                                             const std::string_view label_name,
                                             const std::array<float, 4>& color) noexcept
    : queue_{q},
      dispatcher_{std::addressof(dispatcher)}
{
  begin(label_name, color);
}

/*!
  \details No detailed description
  */
QueueDebugLabelRegion::~QueueDebugLabelRegion() noexcept
{
  end();
}

/*!
  \details No detailed description
  */
void QueueDebugLabelRegion::end() noexcept
{
  zivcvk::Queue q{queue_};
  if (q) {
    const auto loader = dispatcher_->loaderImpl();
    q.endDebugUtilsLabelEXT(*loader);
  }
  queue_ = VK_NULL_HANDLE;
  dispatcher_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] label_name No description.
  \param [in] color No description.
  */
void QueueDebugLabelRegion::begin(const std::string_view label_name,
                                  const std::array<float, 4>& color) noexcept
{
  zivcvk::Queue q{queue_};
  if (q) {
    const auto loader = dispatcher_->loaderImpl();
    const zivcvk::DebugUtilsLabelEXT info{label_name.data(), color};
    q.beginDebugUtilsLabelEXT(std::addressof(info), *loader);
  }
}

} // namespace zivc
