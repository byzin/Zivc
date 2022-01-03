/*!
  \file fill_u128_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_U128_KERNEL_CL
#define ZIVC_FILL_U128_KERNEL_CL

// Zivc
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"
// Internal kernel
#include "utility/fill_info.cl"

/*!
  \details No detailed description

  \param [in] data No description.
  \param [out] buffer No description.
  \param [in] info No description.
  */
__kernel void Zivc_fillU128Kernel(zivc::ConstGlobalPtr<uint4> data,
                                 zivc::GlobalPtr<uint4> buffer,
                                 const zivc::FillInfo info)
{
  const size_t id = zivc::getGlobalIdX();
  for (size_t ite = 0; ite < info.batchSize(); ++ite) {
    const size_t element_number = info.batchSize() * id + ite;
    if (info.elementSize() <= element_number)
      break;
    const size_t element_index = info.elementOffset() + element_number;
    const size_t buffer_index = info.dataSize() * element_index;
    zivc::GlobalPtr<uint4> ptr = buffer + buffer_index;
    for (size_t i = 0; i < info.dataSize(); ++i)
      ptr[i] = data[i];
  }
}

#endif // ZIVC_FILL_U128_KERNEL_CL
