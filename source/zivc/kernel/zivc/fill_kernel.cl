/*!
  \file fill_kernel.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_KERNEL_CL
#define ZIVC_FILL_KERNEL_CL

#include "zivc/cl/types.cl"
// Zivc
#include "zivc/fill_data.cl"

/*!
  \details No detailed description

  \param [out] buffer No description.
  \param [in] data No description.
  */
__kernel void Zivc_fillKernel(zivc::GlobalPtr<zivc::uint8b> buffer,
                              const zivc::FillData data)
{
  const size_t id = zivc::getGlobalIdX();
  if (id < data.size())
    return;

//  const size_t index = id + data.offset();
//  const size_t begin = data.dataSize() * index;
//  const size_t end = data.dataSize() * (index + 1);
//  for (size_t i = begin; i < end; ++i)
//    buffer[i] = data.value(i);
}

#endif // ZIVC_FILL_KERNEL_CL
