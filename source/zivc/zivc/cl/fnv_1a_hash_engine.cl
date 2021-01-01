/*!
  \file fnv_1a_hash_engine.cl
  \author Sho Ikeda

  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FNV_1A_HASH_ENGINE_CL
#define ZIVC_FNV_1A_HASH_ENGINE_CL

#include "types.cl"
// Zivc
#include "hash_engine.cl"

namespace zivc {

/*!
  */
template <typename ResultType>
class Fnv1aHashEngine : public HashEngine<Fnv1aHashEngine<ResultType>, ResultType>
{
 private:
  template <typename, typename> friend class HashEngine;

  //! Implementation of the hash function
  template <typename Integer8Ptr>
  static ResultType hashValue(const Integer8Ptr seed, const size_t n) noexcept;

  //!
  static constexpr ResultType prime() noexcept;

  //!
  static constexpr ResultType offset() noexcept;
};

using Fnv1aHash32 = Fnv1aHashEngine<uint32b>;
using Fnv1aHash64 = Fnv1aHashEngine<uint64b>;

} // namespace zivc

#include "fnv_1a_hash_engine-inl.cl"

#endif // ZIVC_FNV_1A_HASH_ENGINE_CL
