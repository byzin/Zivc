/*!
  \file cpu_sub_platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_SUB_PLATFORM_INL_HPP
#define ZIVC_CPU_SUB_PLATFORM_INL_HPP

#include "cpu_sub_platform.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b CpuSubPlatform::maxTaskBatchSize() noexcept
{
  return 1024;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuSubPlatform::numOfThreads() const noexcept
{
  return zisc::cast<std::size_t>(num_of_threads_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuSubPlatform::taskBatchSize() const noexcept
{
  return zisc::cast<std::size_t>(task_batch_size_);
}

} // namespace zivc

#endif // ZIVC_CPU_SUB_PLATFORM_INL_HPP
