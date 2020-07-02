/*!
  \file cpu_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_BUFFER_INL_HPP
#define ZIVC_CPU_BUFFER_INL_HPP

#include "cpu_buffer.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "cpu_device.hpp"
#include "zivc/buffer.hpp"
#include "zivc/sub_platform.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <typename T> inline
CpuBuffer<T>::CpuBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
CpuBuffer<T>::~CpuBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() noexcept -> zisc::pmr::vector<Type>&
{
  return *buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() const noexcept -> const zisc::pmr::vector<Type>&
{
  return *buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::data() noexcept -> Pointer
{
  return buffer_->data();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::data() const noexcept -> ConstPointer
{
  return buffer_->data();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool CpuBuffer<T>::isDeviceLocal() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool CpuBuffer<T>::isHostCached() const noexcept
{
  return false;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool CpuBuffer<T>::isHostCoherent() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
bool CpuBuffer<T>::isHostVisible() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
void CpuBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_size = size();
  if (s != prev_size) {
    prepareBuffer();
    const std::size_t prev_cap = buffer().capacity();
    buffer().resize(s);
    const std::size_t cap = buffer().capacity();
    if (cap != prev_cap) {
      auto& device = parentImpl();
      const std::size_t prev_mem_size = sizeof(Type) * prev_cap;
      device.notifyDeallocation(prev_mem_size);
      const std::size_t mem_size = sizeof(Type) * cap;
      device.notifyAllocation(mem_size);
    }
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t CpuBuffer<T>::size() const noexcept
{
  const std::size_t s = (buffer_) ? buffer().size() : 0;
  return s;
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult CpuBuffer<T>::copyFromImpl(const Buffer<T>& source,
                                        const BufferLaunchOptions<T>& launch_options)
{
  const auto cpu_source = zisc::cast<const CpuBuffer*>(std::addressof(source));
  const auto& src_buffer = cpu_source->buffer();
  const auto src = src_buffer.begin() + launch_options.sourceOffset();
  auto& dst_buffer = buffer();
  auto dst = dst_buffer.begin() + launch_options.destOffset();
  std::copy_n(src, launch_options.size(), dst);
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::destroyData() noexcept
{
  buffer_.reset();
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <typename T> inline
LaunchResult CpuBuffer<T>::fillImpl(ConstReference value,
                                    const BufferLaunchOptions<T>& launch_options)
{
  auto& dest_buffer = buffer();
  Pointer dest = dest_buffer.begin() + launch_options.destOffset();
  std::fill_n(dest, launch_options.size(), value);
  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::initData()
{
  prepareBuffer();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::mappedMemory() const -> Pointer
{
  Pointer d = const_cast<Pointer>(data());
  ZISC_ASSERT(d != nullptr, "The data is null.");
  return d;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::unmapMemory() const noexcept
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::updateDebugInfoImpl() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
CpuDevice& CpuBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *zisc::treatAs<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const CpuDevice& CpuBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::treatAs<const CpuDevice*>(p);
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::prepareBuffer() noexcept
{
  if (!buffer_) {
    auto mem_resource = Buffer<T>::memoryResource();
    using BufferImplType = typename decltype(buffer_)::element_type;
    typename BufferImplType::allocator_type alloc{mem_resource};
    BufferImplType buffer{alloc};
    buffer_ = zisc::pmr::allocateUnique<BufferImplType>(mem_resource,
                                                        std::move(buffer));
  }
}

// Device

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> CpuDevice::makeBuffer(const BufferUsage flag) noexcept
{
  using BufferType = CpuBuffer<T>;
  zisc::pmr::polymorphic_allocator<BufferType> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferType>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakBuffer<T> own{buffer};
  buffer->initialize(std::move(parent), std::move(own), flag);

  return buffer;
}

} // namespace zivc

#endif // ZIVC_CPU_BUFFER_INL_HPP
