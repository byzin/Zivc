/*!
  \file error.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "error.hpp"
// Standard C++ library
#include <exception>
#include <system_error>
#include <string>
#include <string_view>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/vulkan/utility/vulkan.hpp"
#include "zivc/vulkan/utility/vulkan_hpp.hpp"

/*!
  \def ERROR_CODE_STRING_CASE 
  \brief No brief description

  No detailed description.

  \param [in] code No description.
  \param [out] output No description.
  */
#define ERROR_CODE_STRING_CASE(code, output) case ErrorCode::k ## code : { \
                                               (output) = #code ## s; \
                                               break; \
                                             }

namespace example {

/*!
  \details No detailed description

  \param [in] code No description.
  \return No description
  */
std::string getErrorCodeString(const ErrorCode code) noexcept
{
  using namespace std::string_literals;
  std::string code_str;
  switch (code) {
    ERROR_CODE_STRING_CASE(GlfwInitializationFailed, code_str)
    ERROR_CODE_STRING_CASE(ImGuiInitializationFailed, code_str)
    ERROR_CODE_STRING_CASE(VulkanError, code_str)
  }
  return code_str;
}

/*!
  \details No detailed description
  */
ErrorCategory::ErrorCategory() noexcept
{
}

/*!
  \details No detailed description
  */
ErrorCategory::~ErrorCategory() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
const char* ErrorCategory::name() const noexcept
{
  return kName;
}

/*!
  \details No detailed description

  \param [in] condition No description.
  \return No description
  */
std::string ErrorCategory::message(const int condition) const
{
  const auto code = zisc::cast<ErrorCode>(condition);
  std::string code_str = getErrorCodeString(code);
  return code_str;
}

/*!
  \details No detailed description

  \param [in] code No description.
  */
SystemError::SystemError(const ErrorCode code) :
    std::system_error(zisc::cast<int>(code), ErrorCategory{})
{
}

/*!
  \details No detailed description

  \param [in] code No description.
  */
SystemError::SystemError(const ErrorCode code, const std::string_view what_arg) :
    std::system_error(zisc::cast<int>(code), ErrorCategory{}, what_arg.data())
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
SystemError::SystemError(SystemError&& other) noexcept : std::system_error(other)
{
}

/*!
  \details No detailed description
  */
SystemError::~SystemError() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
SystemError& SystemError::operator=(SystemError&& other) noexcept
{
  std::system_error::operator=(other);
  return *this;
}

/*!
  \details No detailed description

  \param [in] result No description.
  */
void checkVulkanResult(const int result)
{
  const auto r = zisc::cast<zivcvk::Result>(result);
  if (r != zivcvk::Result::eSuccess) {
    const auto desc = "Vulkan error: " + zivcvk::to_string(r) + ".";
    throw SystemError{ErrorCode::kVulkanError, desc};
  }
}

} // namespace example
