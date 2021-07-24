/*!
  \file config.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "config.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/zisc_config.hpp"
#include "zisc/memory/simple_memory_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace ztest {

/*!
  \details No detailed description
  */
Config::~Config() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
zivc::uint32b Config::deviceId() const noexcept
{
  return device_id_;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
void Config::enableDebugMode(const bool flag) noexcept
{
  is_debug_mode_ = flag ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \return No description
  */
bool Config::isDebugMode() const noexcept
{
  const bool flag = is_debug_mode_ == zisc::kTrue;
  return flag;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* Config::memoryResource() noexcept
{
  return mem_resource_.get();
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* Config::memoryResource() const noexcept
{
  return mem_resource_.get();
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
void Config::setDeviceId(const zivc::uint32b id) noexcept
{
  device_id_ = id;
}

/*!
  \details No detailed description

  \return No description
  */
Config& Config::globalConfig() noexcept
{
  if (!global_config_) {
    global_config_.reset(new Config{});
  }
  return *global_config_;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t Config::testKernelWorkSize1d() const noexcept
{
  return 1920 * 1080;
}

/*!
  \details No detailed description
  */
Config::Config() noexcept
{
  initialize();
}

/*!
  \details No detailed description
  */
void Config::initialize() noexcept
{
  mem_resource_ = std::make_unique<zisc::SimpleMemoryResource>();
}

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wexit-time-destructors"
#endif // Z_GCC || Z_CLANG

// Declaration of static member
std::unique_ptr<Config> Config::global_config_;

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

} // namespace ztest
