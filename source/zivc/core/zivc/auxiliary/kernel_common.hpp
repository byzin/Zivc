/*!
  \file kernel_common.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_COMMON_HPP
#define ZIVC_KERNEL_COMMON_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zivc
#include "id_data.hpp"
#include "zivc_object.hpp"
#include "../zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class KernelCommon : public ZivcObject
{
 public:
  //! Initialize the kernel
  KernelCommon(IdData&& id) noexcept;

  //! Finalize the kernel
  ~KernelCommon() noexcept override;


  //! Return the number of kernel arguments required
  virtual std::size_t argSize() const noexcept = 0;

  //! Return the number of buffer arguments required
  virtual std::size_t bufferSize() const noexcept = 0;

  //! Return the work-group dimension
  virtual std::size_t dimensionSize() const noexcept = 0;

  //! Return the number of local parameters are used
  virtual std::size_t localSize() const noexcept = 0;

  //! Return the number of pod arguments required
  virtual std::size_t podSize() const noexcept = 0;
};

// Type aliases
using SharedKernelCommon = std::shared_ptr<KernelCommon>;

} // namespace zivc

#endif // ZIVC_KERNEL_COMMON_HPP
