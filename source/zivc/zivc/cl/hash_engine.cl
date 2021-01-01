/*!
  \file hash_engine.cl
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_HASH_ENGINE_CL
#define ZIVC_HASH_ENGINE_CL

// Zivc
#include "types.cl"

namespace zivc {

/*!
  */
template <typename HashClass, typename ResultType>
class HashEngine
{
 public:
  //! Compute a hash value
  template <typename Integer>
  static ResultType  hash(const Integer seed) noexcept;

  //! Compute a hash value
  template <typename IntegerPtr>
  static ResultType  hash(const IntegerPtr seed, const size_t n) noexcept;
};

} // namespace zivc

#include "hash_engine-inl.cl"

#endif // ZIVC_HASH_ENGINE_CL
