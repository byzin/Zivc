/*!
  \file fill_u16_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CL_FILL_U16_KERNEL_CL
#define ZIVC_CL_FILL_U16_KERNEL_CL

// Zivc
#include "zivc/cl/types.hpp"
#include "zivc/cl/utility.hpp"
// Internal kernel
#include "utility/fill_info.hpp"

using uint16b = zivc::uint16b;

/*!
  \details No detailed description

  \param [in] data No description.
  \param [out] buffer No description.
  \param [in] info No description.
  */
__kernel void Zivc_fillU16Kernel(zivc::ConstGlobalPtr<uint16b> data,
                                 zivc::GlobalPtr<uint16b> buffer,
                                 const zivc::FillInfo info)
{
  const size_t id = zivc::getGlobalIdX();
  for (size_t ite = 0; ite < info.batchSize(); ++ite) {
    const size_t element_number = info.batchSize() * id + ite;
    if (info.elementSize() <= element_number)
      break;
    const size_t buffer_index = info.elementOffset() + element_number;
    const size_t data_index = buffer_index % info.dataSize();
    buffer[buffer_index] = data[data_index];
  }
}

#endif // ZIVC_CL_FILL_U16_KERNEL_CL
