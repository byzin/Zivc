/*!
  \file shader_desc_map-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_SHADER_DESC_MAP_INL_HPP
#define ZIVC_SHADER_DESC_MAP_INL_HPP

#include "shader_desc_map.hpp"
// Standard C++ library
#include <bitset>
#include <cstddef>
#include <map>
#include <memory>
#include <utility>
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
struct ShaderDescMap::PushConstantMap
{
  uint32b offset_ = 0;
  uint32b size_ = 0;
};

/*!
  \brief No brief description

  No detailed description.
  */
struct ShaderDescMap::KernelDescMap
{
  struct BufferMap;
  struct LocalMap;


  //! Initialize the desc map
  KernelDescMap(zisc::pmr::memory_resource* mem_resource) noexcept;


  zisc::pmr::vector<BufferMap> buffer_map_list_;
  zisc::pmr::vector<LocalMap> local_map_list_;
};

/*!
  \brief No brief description

  No detailed description.
  */
struct ShaderDescMap::KernelDescMap::BufferMap
{
  uint32b binding_ = 0;
};

/*!
  \brief No brief description

  No detailed description.
  */
struct ShaderDescMap::KernelDescMap::LocalMap
{
  uint32b spec_id_ = 0;
};

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
inline
ShaderDescMap::KernelDescMap::KernelDescMap(zisc::pmr::memory_resource* mem_resource) noexcept :
    buffer_map_list_{decltype(buffer_map_list_)::allocator_type{mem_resource}},
    local_map_list_{decltype(local_map_list_)::allocator_type{mem_resource}}
{
}

/*!
  \brief No brief description

  No detailed description.
  */
struct ShaderDescMap::SpecConstantMap
{
  uint32b spec_id_ = 0;
};

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
inline
ShaderDescMap::ShaderDescMap(zisc::pmr::memory_resource* mem_resource) noexcept :
    push_constant_map_list_{decltype(push_constant_map_list_)::allocator_type{mem_resource}},
    kernel_desc_map_list_{decltype(kernel_desc_map_list_)::allocator_type{mem_resource}},
    spec_constant_map_list_{decltype(spec_constant_map_list_)::allocator_type{mem_resource}}
{
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* ShaderDescMap::resource() const noexcept
{
  return push_constant_map_list_.get_allocator().resource();
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ShaderDescMap::hasKernelDescMap(const HashT id) const noexcept
{
  return kernel_desc_map_list_.contains(id);
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ShaderDescMap::hasPushConstantMpa(const PushConstantType type) const noexcept
{
  const auto id = static_cast<std::size_t>(type);
  return constant_map_flags[id];
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ShaderDescMap::hasSpecConstantMpa(const SpecConstantType type) const noexcept
{
  const auto id = static_cast<std::size_t>(PushConstantType::kMax) +
                  static_cast<std::size_t>(type);
  return constant_map_flags[id];
}

/*!
  \details No detailed description
  */
inline
void ShaderDescMap::initialize()
{
  push_constant_map_list_.resize(static_cast<std::size_t>(PushConstantType::kMax));
  spec_constant_map_list_.resize(static_cast<std::size_t>(SpecConstantType::kMax));
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::kernelDescMap(const HashT id) noexcept
    -> KernelDescMap&
{
  return kernel_desc_map_list_.at(id);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::kernelDescMap(const HashT id) const noexcept
    -> const KernelDescMap&
{
  return kernel_desc_map_list_.at(id);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::kernelDescMapList() noexcept
    -> zisc::pmr::map<HashT, KernelDescMap>&
{
  return kernel_desc_map_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::kernelDescMapList() const noexcept
    -> const zisc::pmr::map<HashT, KernelDescMap>&
{
  return kernel_desc_map_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::pushConstantMap(const PushConstantType type) const noexcept
    -> const PushConstantMap&
{
  const auto id = static_cast<std::size_t>(type);
  return push_constant_map_list_[id];
}

/*!
  \details No detailed description

  \param [in] id No description.
  \param [in] map No description.
  */
inline
void ShaderDescMap::setKernelDescMap(const HashT id, KernelDescMap&& map) noexcept
{
  kernel_desc_map_list_.emplace(id, std::move(map));
}

/*!
  \details No detailed description

  \param [in] type No description.
  \param [in] map No description.
  */
inline
void ShaderDescMap::setPushConstantMap(const PushConstantType type,
                                       const PushConstantMap& map) noexcept
{
  const auto id = static_cast<std::size_t>(type);
  push_constant_map_list_[id] = map;
  constant_map_flags.set(id);
}

/*!
  \details No detailed description

  \param [in] type No description.
  \param [in] map No description.
  */
inline
void ShaderDescMap::setSpecConstantMap(const SpecConstantType type,
                                       const SpecConstantMap& map) noexcept
{
  auto id = static_cast<std::size_t>(type);
  spec_constant_map_list_[id] = map;
  id += static_cast<std::size_t>(PushConstantType::kMax);
  constant_map_flags.set(id);
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ShaderDescMap::specConstantMap(const SpecConstantType type) const noexcept
    -> const SpecConstantMap&
{
  const auto id = static_cast<std::size_t>(type);
  return spec_constant_map_list_[id];
}

} /* namespace zivc::internal */

#endif /* ZIVC_SHADER_DESC_MAP_INL_HPP */
