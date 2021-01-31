/*!
  \file kernel_common.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_COMMON_HPP
#define ZIVC_KERNEL_COMMON_HPP

// Standard C++ library
#include <cstddef>
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"

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


  //! Return the number of kernel arguments
  virtual std::size_t argSize() const noexcept = 0;

  //! Return the work-group dimension
  virtual std::size_t dimensionSize() const noexcept = 0;
};

} // namespace zivc

#endif // ZIVC_KERNEL_COMMON_HPP
