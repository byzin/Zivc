/*!
  \file fill_u16_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_U16_KERNEL_CL
#define ZIVC_FILL_U16_KERNEL_CL

// Zivc
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"
// Internal kernel
#include "utility/fill_info.cl"

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
    const size_t element_index = info.elementOffset() + info.batchSize() * id + ite;
    if (info.elementSize() <= element_index)
      break;
    const size_t buffer_index = info.dataSize() * element_index;
    zivc::GlobalPtr<uint16b> ptr = buffer + buffer_index;
    for (size_t i = 0; i < info.dataSize(); ++i)
      ptr[i] = 0;
//      ptr[i] = data[i];
  }
  static_cast<void>(data);
}

#endif // ZIVC_FILL_U16_KERNEL_CL
