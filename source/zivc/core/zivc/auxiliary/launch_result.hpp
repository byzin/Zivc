/*!
  \file launch_result.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_LAUNCH_RESULT_HPP
#define ZIVC_LAUNCH_RESULT_HPP

// Standard C++ library
#include <array>
#include <type_traits>
// Zisc
#include "zisc/boolean.hpp"
#include "zisc/non_copyable.hpp"
// Zivc
#include "fence.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
class Device;

/*!
  \brief No brief description

  No detailed description.
  */
class LaunchResult : private zisc::NonCopyable<LaunchResult>
{
 public:
  //! Create an empty result
  LaunchResult() noexcept;

  //! Move a data
  LaunchResult(LaunchResult&& other) noexcept;

  //! Finalize the result
  ~LaunchResult() noexcept;


  //! Move a data
  LaunchResult& operator=(LaunchResult&& other) noexcept;


  //! Return the fence of the kernel execution
  Fence& fence() noexcept;

  //! Return the fence of the kernel execution
  const Fence& fence() const noexcept;

  //! Check if the result has a active fence
  bool hasFence() const noexcept;

  //! Check whether the execution is asyncronous
  bool isAsync() const noexcept;

  //! Set async mode
  void setAsync(const bool is_async) noexcept;

 private:
  Fence fence_;
  zisc::Boolean is_async_;
  [[maybe_unused]] Padding<std::alignment_of_v<Fence> - 1> pad_;
};

} // namespace zivc

#include "launch_result-inl.hpp"

#endif // ZIVC_LAUNCH_RESULT_HPP
