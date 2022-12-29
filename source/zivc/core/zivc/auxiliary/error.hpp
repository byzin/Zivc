/*!
  \file error.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
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
// Zisc
#include "zisc/error.hpp"

/*!
  \def ZIVC_ASSERT(condition, ...)
  \brief If condition is false, assert outputs messages and calls std::abort

  No detailed description.

  \param [in] condition No description.
  \param [in] ... No description.
  */
#define ZIVC_ASSERT(condition, ...) ZISC_ASSERT(condition, __VA_ARGS__)

namespace zivc {

// Forward declaration
class ZivcObject;

/*!
  \brief No brief description

  No detailed description.
  */
enum class ErrorCode : int
{
  kInitializationFailed,
  kInvalidInstruction,
  kAvailableFenceNotFound,
  kBufferMemoryAllocationFailed,
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
  //! Initialize a system error
  SystemError(const ErrorCode code);

  //! Initialize a system error
  SystemError(const ErrorCode code, const std::string_view what_arg);


  //! Move data
  SystemError(SystemError&& other) noexcept;

  //! Finalize the system error
  ~SystemError() noexcept override;


  //! Move data
  SystemError& operator=(SystemError&& other) noexcept;
};

//! Create an error message
std::string createErrorMessage(const ZivcObject& object,
                               const std::string_view what_arg) noexcept;

} // namespace zivc

#endif // ZIVC_ERROR_HPP