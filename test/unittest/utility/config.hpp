/*!
  \file config.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_CONFIG_HPP
#define ZIVC_TEST_CONFIG_HPP

// Standard C++ library
#include <array>
#include <limits>
#include <memory>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

class Context;

} // namespace zivc

namespace ztest {

/*!
  \brief No brief description

  No detailed description.
  */
class Config : private zisc::NonCopyable<Config>
{
 public:
  //! Destroy a config
  ~Config() noexcept;


  //! Destroy undering context
  void destroyContext() noexcept;

  //! Return the device ID
  zivc::uint32b deviceId() const noexcept;

  //! Enable debug mode
  void enableDebugMode(const bool flag) noexcept;

  //! Make test use global context instead of each test's context
  void enableGlobalContext(const bool flag) noexcept;

  //! Return the global config
  static Config& globalConfig() noexcept;

  //! Return invalid device ID
  static constexpr zivc::uint32b invalidDeviceId() noexcept
  {
    return (std::numeric_limits<zivc::uint32b>::max)();
  }

  //! Check if debug mode is enabled
  bool isDebugMode() const noexcept;

  //! Check if test uses global context instead of each test's context
  bool isGlobalContextEnabled() const noexcept;

  //! Return the memory resource for unit test
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the memory resource for unit test
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Query a zivc context
  std::shared_ptr<zivc::Context> queryContext() noexcept;

  //! Return the device ID
  void setDeviceId(const zivc::uint32b id) noexcept;

  //! Return the work size of test kernel
  static std::size_t testKernelWorkSize1d() noexcept;

 private:
  //! Initialize a config
  Config() noexcept;


  //! Initialize a confik
  void initialize() noexcept;


  std::shared_ptr<zivc::Context> context_;
  std::unique_ptr<zisc::pmr::memory_resource> mem_resource_;
  zivc::uint32b device_id_ = invalidDeviceId();
  zisc::Boolean is_debug_mode_ = true;
  zisc::Boolean is_global_context_enabled_;
  [[maybe_unused]] zivc::Padding<2> padd_;
};

} // namespace ztest

#endif // ZIVC_TEST_CONFIG_HPP
