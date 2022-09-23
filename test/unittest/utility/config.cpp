/*!
  \file config.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "config.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
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
  */
void Config::destroyContext() noexcept
{
  context_.reset();
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
  is_debug_mode_ = flag;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
void Config::enableGlobalContext(const bool flag) noexcept
{
  is_global_context_enabled_ = flag;
}

/*!
  \details No detailed description

  \return No description
  */
Config& Config::globalConfig() noexcept
{
  static Config global_config{};
  return global_config;
}

/*!
  \details No detailed description

  \return No description
  */
bool Config::isDebugMode() const noexcept
{
  return is_debug_mode_;
}

/*!
  \details No detailed description

  \return No description
  */
bool Config::isGlobalContextEnabled() const noexcept
{
  return is_global_context_enabled_;
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

  \return No description
  */
zivc::SharedContext Config::queryContext() noexcept
{
  if (!context_) {
    const zisc::pmr::polymorphic_allocator<zivc::Context> alloc{memoryResource()};
    zivc::ContextOptions options{memoryResource()};
    options.setContextName("UnitTest");
    options.setContextVersionMajor(zivc::Config::versionMajor());
    options.setContextVersionMinor(zivc::Config::versionMinor());
    options.setContextVersionPatch(zivc::Config::versionPatch());
    options.enableVulkanBackend(0 < deviceId());
    options.enableDebugMode(isDebugMode());
    context_ = zivc::createContext(options);
  }
  return context_;
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
std::size_t Config::testKernelWorkSize1d() noexcept
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
  mem_resource_ = std::make_unique<zisc::AllocFreeResource>();
}

} // namespace ztest
