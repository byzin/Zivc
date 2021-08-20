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
#include "zivc/cl/atomic.cl"
#include "zivc/cl/types.cl"
#include "zivc/cl/utility.cl"
// Internal kernel
#include "utility/fill_info.cl"

using uint8b = zivc::uint8b;

/*!
  \details No detailed description

  \param [in] data No description.
  \param [out] buffer No description.
  \param [in] info No description.
  */
//__kernel void Zivc_fillU8Kernel(zivc::ConstGlobalPtr<uint8b> data,
//                                zivc::GlobalPtr<uint8b> buffer,
//                                const zivc::FillInfo info)
//{
//  const size_t id = zivc::getGlobalIdX();
//  if (id < info.bufferSize())
//    return;
//
//  zivc::GlobalPtr<uint8b> ptr = buffer + (info.dataSize() * (id + info.idOffset()));
//  for (size_t i = 0; i < info.dataSize(); ++i) {
////    const size_t index = info.dataSize() * (id + info.idOffset()) + i;
////    buffer[index] = data[i];
////    ptr[i] = data[i];
//    const uint8b value = data[i];
//    zivc::atomic_xchg(ptr + i, value);
//  }
//}

//using uint16b = zivc::uint16b;
//__kernel void Zivc_fillU8Kernel(zivc::GlobalPtr<uint16b> data,
//                                zivc::GlobalPtr<uint16b> buffer,
//                                const zivc::FillInfo info)
//{
//  const size_t id = zivc::getGlobalIdX();
//  if (id < info.bufferSize())
//    return;
//
//  zivc::GlobalPtr<uint16b> ptr = buffer + (info.dataSize() * (id + info.idOffset()));
//  for (size_t i = 0; i < info.dataSize(); ++i) {
//    ptr[i] = data[i];
//  }
//}

//using uint32b = zivc::uint32b;
//__kernel void Zivc_fillU8Kernel(zivc::GlobalPtr<uint32b> data,
//                                zivc::GlobalPtr<uint32b> buffer,
//                                const zivc::FillInfo info)
//{
//  const size_t id = zivc::getGlobalIdX();
//  if (id < info.bufferSize())
//    return;
//
//  zivc::GlobalPtr<uint32b> ptr = buffer + (info.dataSize() * (id + info.idOffset()));
//  for (size_t i = 0; i < info.dataSize(); ++i) {
//    ptr[i] = data[i];
//  }
//}

__kernel void Zivc_fillU8Kernel(zivc::GlobalPtr<uint2> data,
                                zivc::GlobalPtr<uint2> buffer,
                                const zivc::FillInfo info)
{
  const size_t id = zivc::getGlobalIdX();
  if (id < info.bufferSize())
    return;

  zivc::GlobalPtr<uint2> ptr = buffer + (info.dataSize() * (id + info.idOffset()));
  for (size_t i = 0; i < info.dataSize(); ++i) {
//    const size_t index = info.dataSize() * (id + info.idOffset()) + i;
//    buffer[index] = data[i];
    ptr[i] = data[i];
  }
}

//__kernel void Zivc_fillU8Kernel(zivc::GlobalPtr<uint4> data,
//                                zivc::GlobalPtr<uint4> buffer,
//                                const zivc::FillInfo info)
//{
//  const size_t id = zivc::getGlobalIdX();
//  if (id < info.bufferSize())
//    return;
//
//  zivc::GlobalPtr<uint4> ptr = buffer + (info.dataSize() * (id + info.idOffset()));
//  for (size_t i = 0; i < info.dataSize(); ++i) {
////    const size_t index = info.dataSize() * (id + info.idOffset()) + i;
////    buffer[index] = data[i];
//    ptr[i] = data[i];
//  }
//}

#endif // ZIVC_FILL_U8_KERNEL_CL
