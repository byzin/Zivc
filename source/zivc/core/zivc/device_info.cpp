/*!
  \file device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "device_info.hpp"
// Standard C++ library
#include <utility>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
DeviceInfo::DeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept :
    heap_info_list_{decltype(heap_info_list_)::allocator_type{mem_resource}}
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
DeviceInfo::DeviceInfo(DeviceInfo&& other) noexcept :
    heap_info_list_{std::move(other.heap_info_list_)}
{
}

/*!
  \details No detailed description
  */
DeviceInfo::~DeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
DeviceInfo& DeviceInfo::operator=(DeviceInfo&& other) noexcept
{
  heap_info_list_ = std::move(other.heap_info_list_);
  return *this;
}

} // namespace zivc
