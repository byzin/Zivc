/*!
  \file synchronization.cl
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SYNCHRONIZATION_CL
#define ZIVC_SYNCHRONIZATION_CL

// Zivc
#include "types.cl"

namespace zivc {

/*!
 */
class Synchronization
{
 public:
  //! Any variables stored in memories of the given types are flushed
  static void barrier(const int32b flags) noexcept;

  //! Any variables stored in global memory are flushed
  static void barrierGlobal() noexcept;

  //! Any variables stored in local memory are flushed
  static void barrierLocal() noexcept;

  //! Any variables stored in global and local memory are flushed
  static void barrierAll() noexcept;
};

// CL function aliases

//! Any variables stored in memories of the given types are flushed
void barrier(const int32b flags) noexcept;

} // namespace zivc

#include "synchronization-inl.cl"

#endif /* ZIVC_SYNCHRONIZATION_CL */
