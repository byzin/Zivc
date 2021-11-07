/*!
  \file error.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ERROR_HPP
#define ZIVC_ERROR_HPP

// Standard C+ library
#include <exception>
#include <string>
#include <string_view>
#include <system_error>

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
enum class ErrorCode : int
{
  kInitializationFailed,
  kAvailableFenceNotFound,
  kNumOfParametersLimitExceeded,
  kVulkanInitializationFailed,
  kVulkanLibraryNotFound,
  kVulkanWindowSurfaceNotFound,
};

//! Return the string of the given error code
std::string getErrorCodeString(const ErrorCode code) noexcept;

/*!
  \brief No brief description

  No detailed description.
  */
class ErrorCategory : public std::error_category
{
 public:
  //! Initialize an error category
  ErrorCategory() noexcept;

  //! Finalize the error category
  ~ErrorCategory() noexcept override;


  //! Return the name of the category
  const char* name() const noexcept override;

  //! Return the explanatory string
  std::string message(const int condition) const override;

 private:
  static constexpr char kName[] = "Zivc";
};

/*!
  \brief No brief description

  No detailed description.
  */
class SystemError : public std::system_error
{
 public:
  //! Initialize a system erro
  SystemError(const ErrorCode code);

  //! Initialize a system erro
  SystemError(const ErrorCode code, const std::string_view what_arg);

  //! Move data
  SystemError(SystemError&& other) noexcept;

  //! Finalize the system error
  ~SystemError() noexcept override;


  //! Move data
  SystemError& operator=(SystemError&& other) noexcept;
};

} // namespace zivc

#endif // ZIVC_ERROR_HPP
