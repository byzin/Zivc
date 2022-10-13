/*!
  \file queue_begin_label_region.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "queue_debug_label_region.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <memory>
#include <span>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "zivc/zivc_config.hpp"
#include "zivc/vulkan/utility/vulkan_dispatch_loader.hpp"
#include "zivc/vulkan/utility/vulkan_hpp.hpp"

namespace zivc::internal {

/*!
  \details No detailed description

  \param [in] q No description.
  \param [in] dispatcher No description.
  \param [in] label_name No description.
  \param [in] color No description.
  */
QueueDebugLabelRegion::QueueDebugLabelRegion(
    const VkQueue& q,
    const VulkanDispatchLoader& dispatcher,
    const std::string_view label_name,
    const std::span<const float, 4> color) noexcept :
        queue_{q},
        dispatcher_{std::addressof(dispatcher)}
{
  begin(label_name, color);
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
QueueDebugLabelRegion::QueueDebugLabelRegion(QueueDebugLabelRegion&& other) noexcept
{
  std::swap(queue_, other.queue_);
  std::swap(dispatcher_, other.dispatcher_);
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

  \param [in,out] other No description.
  \return No description
  */
QueueDebugLabelRegion& QueueDebugLabelRegion::operator=(QueueDebugLabelRegion&& other) noexcept
{
  end();
  std::swap(queue_, other.queue_);
  std::swap(dispatcher_, other.dispatcher_);
  return *this;
}

/*!
  \details No detailed description
  */
void QueueDebugLabelRegion::end() noexcept
{
  const vk::Queue q{queue_};
  if (q) {
    q.endDebugUtilsLabelEXT(dispatcher_->loader());
  }
  queue_ = ZIVC_VK_NULL_HANDLE;
  dispatcher_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] label_name No description.
  \param [in] color No description.
  */
void QueueDebugLabelRegion::begin(const std::string_view label_name,
                                  const std::span<const float, 4> color) noexcept
{
  const vk::Queue q{queue_};
  if (q) {
    std::array<float, 4> c{{0.0f, 0.0f, 0.0f, 0.0f}};
    std::copy_n(color.begin(), color.size(), c.begin());
    const vk::DebugUtilsLabelEXT info{label_name.data(), c};
    q.beginDebugUtilsLabelEXT(&info, dispatcher_->loader());
  }
}

} // namespace zivc::internal
