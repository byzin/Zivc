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
#include <string>
// Zisc
#include "zisc/non_copyable.hpp"
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

  //! Return the device name
  std::string& deviceName() noexcept;

  //! Return the device name
  const std::string& deviceName() const noexcept;

  //! Return the global config
  static Config& globalConfig() noexcept;

  //! Return the device ID
  static zivc::uint32b getDeviceId(std::string name) noexcept;

  //! Return invalid device ID
  static constexpr zivc::uint32b invalidDeviceId() noexcept
  {
    return (std::numeric_limits<zivc::uint32b>::max)();
  }

 private:
  //! Initialize a config
  Config() noexcept;


  static std::unique_ptr<Config> global_config_;


  std::string device_name_ = "cpu";
};

} // namespace ztest

#endif // ZIVC_TEST_CONFIG_HPP
