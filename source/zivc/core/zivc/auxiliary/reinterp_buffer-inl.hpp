/*!
  \file reinterp_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_REINTERP_BUFFER_INL_HPP
#define ZIVC_REINTERP_BUFFER_INL_HPP

#include "reinterp_buffer.hpp"
// Standard C++ library
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer_init_params.hpp"
#include "error.hpp"
#include "id_data.hpp"
#include "zivc_object.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ReinterpBuffer<Derived, T>::ReinterpBuffer(BufferPtr buffer, IdData&& id) noexcept :
    Buffer<T>(std::move(id)),
    buffer_{buffer}
{
  initCommon(buffer_);
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ReinterpBuffer<Derived, T>::ReinterpBuffer(ReinterpBuffer&& other) noexcept :
    Buffer<T>(std::move(other.id().copy())),
    buffer_{other.internalBuffer()}
{
  initCommon(buffer_);
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
ZivcObject* ReinterpBuffer<Derived, T>::getParent()
{
  ZivcObject* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    const std::string message = createErrorMessage(
        *this,
        "'getParent' cannot be called with const qualified buffer.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  else {
    BufferPtr b = internalBuffer();
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
  BufferPtr b = internalBuffer();
  return b->getParent();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
ZivcObject* ReinterpBuffer<Derived, T>::getOwn()
{
  ZivcObject* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    const std::string message = createErrorMessage(
        *this,
        "'getOwn' cannot be called with const qualified buffer.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  else {
    BufferPtr b = internalBuffer();
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
  BufferPtr b = internalBuffer();
  return b->getOwn();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
std::size_t ReinterpBuffer<Derived, T>::capacityInBytes() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->capacityInBytes();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
std::size_t ReinterpBuffer<Derived, T>::heapIndex() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->heapIndex();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isDebugMode() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->isDebugMode();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isDeviceLocal() const noexcept
{
  BufferPtr b =internalBuffer();
  return b->isDeviceLocal();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostCached() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->isHostCached();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostCoherent() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->isHostCoherent();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
bool ReinterpBuffer<Derived, T>::isHostVisible() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->isHostVisible();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void* ReinterpBuffer<Derived, T>::mapMemoryData() const
{
  BufferPtr b = internalBuffer();
  return b->mapMemoryData();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
zisc::pmr::memory_resource* ReinterpBuffer<Derived, T>::memoryResource()
{
  zisc::pmr::memory_resource* mem_resource = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    const std::string message = createErrorMessage(
        *this,
        "'memoryResource' cannot be called with const qualified buffer.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  else {
    BufferPtr b = internalBuffer();
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
  BufferPtr b = internalBuffer();
  return b->memoryResource();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void* ReinterpBuffer<Derived, T>::rawBufferData()
{
  void* data = nullptr;
  if constexpr (std::is_const_v<BufferT>) {
    const std::string message = createErrorMessage(
        *this,
        "'rawBufferData' cannot be called with const qualified buffer.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  else {
    BufferPtr b = internalBuffer();
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
  BufferPtr b = internalBuffer();
  return b->rawBufferData();
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::setSize(const std::size_t s)
{
  if constexpr (std::is_const_v<BufferT>) {
    const std::string message = createErrorMessage(
        *this,
        "'setSize' cannot be called with const qualified buffer.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  else {
    BufferPtr b = internalBuffer();
    const std::size_t new_size = (sizeof(Type) * s) / b->typeSizeInBytes();
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
  BufferPtr b = internalBuffer();
  return b->sizeInBytes();
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
BackendType ReinterpBuffer<Derived, T>::type() const noexcept
{
  BufferPtr b = internalBuffer();
  return b->type();
}

/*!
  \details No detailed description
  */
template <DerivedBuffer Derived, KernelArg T> inline
void ReinterpBuffer<Derived, T>::unmapMemoryData() const noexcept
{
  BufferPtr b = internalBuffer();
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
void ReinterpBuffer<Derived, T>::initCommon(const ConstBufferPtr buffer) noexcept
{
  if (buffer != nullptr) {
    Buffer<T>::setUsage(buffer->usage());
    Buffer<T>::setFlag(buffer->flag());
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
auto ReinterpBuffer<Derived, T>::internalBuffer() noexcept -> BufferPtr
{
  ZIVC_ASSERT(buffer_ != nullptr, "The internal buffer is null.");
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <DerivedBuffer Derived, KernelArg T> inline
auto ReinterpBuffer<Derived, T>::internalBuffer() const noexcept -> ConstBufferPtr
{
  ZIVC_ASSERT(buffer_ != nullptr, "The internal buffer is null.");
  return buffer_;
}

} // namespace zivc

#endif // ZIVC_REINTERP_BUFFER_INL_HPP
