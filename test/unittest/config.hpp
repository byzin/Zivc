/*!
  \file config.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_CONFIG_HPP
#define ZIVC_TEST_CONFIG_HPP

// Standard C++ library
#include <limits>
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

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


  //! Return the device ID
  zivc::uint32b deviceId() const noexcept;

  //! Return the global config
  static Config& globalConfig() noexcept;

  //! Return invalid device ID
  static constexpr zivc::uint32b invalidDeviceId() noexcept
  {
    return (std::numeric_limits<zivc::uint32b>::max)();
  }

  //! Return the memory resource for unit test
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the memory resource for unit test
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the device ID
  void setDeviceId(const zivc::uint32b id) noexcept;

 private:
  //! Initialize a config
  Config() noexcept;


  //! Initialize a confik
  void initialize() noexcept;


  static std::unique_ptr<Config> global_config_;


  std::unique_ptr<zisc::pmr::memory_resource> mem_resource_;
  zivc::uint32b device_id_ = invalidDeviceId();
  [[maybe_unused]] zivc::uint32b padding_ = 0;
};

} // namespace ztest

#endif // ZIVC_TEST_CONFIG_HPP
