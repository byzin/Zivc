/*!
  \file fence.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FENCE_HPP
#define ZIVC_FENCE_HPP

// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/non_copyable.hpp"
// Zivc
#include "../zivc_config.hpp"

namespace zivc {

// Forward declaration
class Device;

class Fence : private zisc::NonCopyable<Fence>
{
 public:
  using DataT = std::aligned_storage_t<28, 8>;


  //! Create a deactive fence
  Fence() noexcept;

  //! Create an active fence
  Fence(Device* device);

  //! Move a data
  Fence(Fence&& other) noexcept;

  //! Finalize the fence
  ~Fence() noexcept;


  //! Move a data
  Fence& operator=(Fence&& other) noexcept;

  //! Check whether the fence is active
  explicit operator bool() const noexcept;


  //! Clear the fence state
  void clear() noexcept;

  //! Return the data
  DataT& data() noexcept;

  //! Return the data
  const DataT& data() const noexcept;

  //! Check whether the fence is active
  bool isActive() const noexcept;

  //! Set a device
  void setDevice(Device* device);

  //! Wait for the execution completion when the fence is active
  void wait() const noexcept;

 private:
  DataT fence_;
  Device* device_;
};

} // namespace zivc

#include "fence-inl.hpp"

#endif // ZIVC_FENCE_HPP
