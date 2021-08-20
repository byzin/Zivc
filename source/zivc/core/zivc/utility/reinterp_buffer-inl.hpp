/*!
  \file reinterp_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_REINTERP_BUFFER_INL_HPP
#define ZIVC_REINTERP_BUFFER_INL_HPP

#include "reinterp_buffer.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/error.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer_init_params.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ReinterpBuffer<Derived, T>::ReinterpBuffer(BufferP buffer, IdData&& id) noexcept :
    Buffer<T>(std::move(id)),
    buffer_{buffer}
{
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ReinterpBuffer<Derived, T>::ReinterpBuffer(ReinterpBuffer&& other) noexcept :
    Buffer<T>(std::move(other.id().copy())),
    buffer_{other.internalBuffer()}
{
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ReinterpBuffer<Derived, T>::~ReinterpBuffer() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ZivcObject* ReinterpBuffer<Derived, T>::getParent() noexcept
{
  ZivcObject* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    ZISC_ASSERT(false, "'getParent' cannot be called with const qualified buffer.");
  }
  else {
    auto b = internalBuffer();
    data = b->getParent();
  }
  return data;
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
const ZivcObject* ReinterpBuffer<Derived, T>::getParent() const noexcept
{
  auto b = internalBuffer();
  return b->getParent();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ZivcObject* ReinterpBuffer<Derived, T>::getOwn() noexcept
{
  ZivcObject* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    ZISC_ASSERT(false, "'getOwn' cannot be called with const qualified buffer.");
  }
  else {
    auto b = internalBuffer();
    data = b->getOwn();
  }
  return data;
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
const ZivcObject* ReinterpBuffer<Derived, T>::getOwn() const noexcept
{
  auto b = internalBuffer();
  return b->getOwn();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
std::size_t ReinterpBuffer<Derived, T>::capacityInBytes() const noexcept
{
  auto b = internalBuffer();
  return b->capacityInBytes();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
std::size_t ReinterpBuffer<Derived, T>::heapIndex() const noexcept
{
  auto b = internalBuffer();
  return b->heapIndex();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isDebugMode() const noexcept
{
  auto b = internalBuffer();
  return b->isDebugMode();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isDeviceLocal() const noexcept
{
  auto b = internalBuffer();
  return b->isDeviceLocal();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostCached() const noexcept
{
  auto b = internalBuffer();
  return b->isHostCached();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostCoherent() const noexcept
{
  auto b = internalBuffer();
  return b->isHostCoherent();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostVisible() const noexcept
{
  auto b = internalBuffer();
  return b->isHostVisible();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void* ReinterpBuffer<Derived, T>::mapMemoryData() const
{
  auto b = internalBuffer();
  return b->mapMemoryData();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
zisc::pmr::memory_resource* ReinterpBuffer<Derived, T>::memoryResource() noexcept
{
  zisc::pmr::memory_resource* mem_resource = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    ZISC_ASSERT(false, "'memoryResource' cannot be called with const qualified buffer.");
  }
  else {
    auto b = internalBuffer();
    mem_resource = b->memoryResource();
  }
  return mem_resource;

}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
const zisc::pmr::memory_resource* ReinterpBuffer<Derived, T>::memoryResource() const noexcept
{
  auto b = internalBuffer();
  return b->memoryResource();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void* ReinterpBuffer<Derived, T>::rawBufferData() noexcept
{
  void* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    ZISC_ASSERT(false, "'rawBufferData' cannot be called with const qualified buffer.");
  }
  else {
    auto b = internalBuffer();
    data = b->rawBufferData();
  }
  return data;
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
const void* ReinterpBuffer<Derived, T>::rawBufferData() const noexcept
{
  auto b = internalBuffer();
  return b->rawBufferData();
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::setSize(const std::size_t s)
{
  if constexpr (std::is_const_v<BufferT>) {
    ZISC_ASSERT(false, "'setSize' cannot be called with const qualified buffer.");
  }
  else {
    auto b = internalBuffer();
    const std::size_t new_size = (sizeof(Type) * s) / b->typeSize();
    b->setSize(new_size);
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
std::size_t ReinterpBuffer<Derived, T>::sizeInBytes() const noexcept
{
  auto b = internalBuffer();
  return b->sizeInBytes();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
SubPlatformType ReinterpBuffer<Derived, T>::type() const noexcept
{
  auto b = internalBuffer();
  return b->type();
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::unmapMemoryData() const noexcept
{
  auto b = internalBuffer();
  b->unmapMemoryData();
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::destroyData() noexcept
{
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::initData([[maybe_unused]] const BufferInitParams& params)
{
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::updateDebugInfoImpl() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
auto ReinterpBuffer<Derived, T>::internalBuffer() noexcept -> BufferP
{
  ZISC_ASSERT(buffer_ != nullptr, "The internal buffer is null.");
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
auto ReinterpBuffer<Derived, T>::internalBuffer() const noexcept -> ConstBufferP
{
  ZISC_ASSERT(buffer_ != nullptr, "The internal buffer is null.");
  return buffer_;
}

} // namespace zivc

#endif // ZIVC_REINTERP_BUFFER_INL_HPP
