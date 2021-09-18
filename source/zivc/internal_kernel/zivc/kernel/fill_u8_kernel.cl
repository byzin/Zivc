/*!
  \file fill_u8_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_U8_KERNEL_CL
#define ZIVC_FILL_U8_KERNEL_CL

// Zivc
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"
// Internal kernel
#include "utility/fill_data.cl"
#include "utility/fill_info.cl"

using uint8b = zivc::uint8b;
static_assert(sizeof(zivc::FillDataU8) % sizeof(int4) == 0);

/*!
  \details No detailed description

  \param [in] data No description.
  \param [out] buffer No description.
  \param [in] info No description.
  */
__kernel void Zivc_fillU8Kernel(zivc::GlobalPtr<uint8b> buffer,
                                const zivc::FillInfo info,
                                const zivc::FillDataU8 data)
{
  const size_t id = zivc::getGlobalIdX();
  for (size_t ite = 0; ite < info.batchSize(); ++ite) {
    const size_t element_number = info.batchSize() * id + ite;
    if (info.elementSize() <= element_number)
      break;
    const size_t element_index = info.elementOffset() + element_number;
    const size_t buffer_index = info.dataSize() * element_index;
    zivc::GlobalPtr<uint8b> ptr = buffer + buffer_index;
    for (size_t i = 0; i < info.dataSize(); ++i)
      ptr[i] = data[i];
  }
  static_cast<void>(data);
}

#endif // ZIVC_FILL_U8_KERNEL_CL
