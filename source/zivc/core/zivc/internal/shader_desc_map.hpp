/*!
  \file shader_desc_map.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHADER_DESC_MAP_HPP
#define ZIVC_SHADER_DESC_MAP_HPP

// Standard C++ library
#include <bitset>
#include <cstddef>
#include <map>
#include <memory>
#include <vector>
// Zisc
#include "zisc/hash/fnv_1a_hash_engine.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc::internal {

/*!
  \brief No brief description

  No detailed description.
  */
class ShaderDescMap
{
 public:
  //
  using HashEngineT = zisc::Fnv1aHash64;
  using HashT = HashEngineT::ValueT;

  struct PushConstantMap; //<! Represent a push constant data
  struct KernelDescMap; //<! Represent a kernel data
  struct SpecConstantMap; //!< Represent a spec constant data

  /*!
    \brief No brief description

    No detailed description.
    */
  enum class PushConstantType : std::size_t
  {
    kGlobalOffset = 0,
    kEnqueuedLocalSize,
    kGlobalSize,
    kRegionOffset,
    kNumWorkgroups,
    kRegionGroupOffset,
    kMax
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  enum class SpecConstantType : std::size_t
  {
    kWorkgroupSizeX = 0,
    kWorkgroupSizeY,
    kWorkgroupSizeZ,
    kWorkDim,
    kMax
  };


  //! Create a shader desc map
  ShaderDescMap(zisc::pmr::memory_resource* mem_resource) noexcept;


  //! Check if a kernel desc map exists by the given hash
  bool hasKernelDescMap(const HashT id) const noexcept;

  //! Check if a push constant map exists by the given type
  bool hasPushConstantMpa(const PushConstantType type) const noexcept;

  //! Check if a spec constant map exists by the given type
  bool hasSpecConstantMpa(const SpecConstantType type) const noexcept;

  //! Initialize the desc map
  void initialize();

  //! Return the underlying kernel desc map by the given hash
  KernelDescMap& kernelDescMap(const HashT id) noexcept;

  //! Return the underlying kernel desc map by the given hash
  const KernelDescMap& kernelDescMap(const HashT id) const noexcept;

  //! Return the underlying kernel desc map list
  const zisc::pmr::map<HashT, KernelDescMap>& kernelDescMapList() const noexcept;

  //! Return the underlying push constant map by the given type
  const PushConstantMap& pushConstantMap(const PushConstantType type) const noexcept;

  //! Set a kernel desc map
  void setKernelDescMap(const HashT id, KernelDescMap&& map) noexcept;

  //! Set a push constant map
  void setPushConstantMap(const PushConstantType type,
                          const PushConstantMap& map) noexcept;

  //! Set a spec constant map
  void setSpecConstantMap(const SpecConstantType type,
                          const SpecConstantMap& map) noexcept;

  //! Return the underlying spec constant map by the given type
  const SpecConstantMap& specConstantMap(const SpecConstantType type) const noexcept;

 private:
  //! Return the underlying memory resource
  zisc::pmr::memory_resource* resource() const noexcept;


  std::bitset<64> constant_map_flags;
  zisc::pmr::vector<PushConstantMap> push_constant_map_list_;
  zisc::pmr::map<HashT, KernelDescMap> kernel_desc_map_list_;
  zisc::pmr::vector<SpecConstantMap> spec_constant_map_list_;
};

} /* namespace zivc::internal */

#include "shader_desc_map-inl.hpp"

#endif /* ZIVC_SHADER_DESC_MAP_HPP */
