/*!
  \file reinterp_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_REINTERP_BUFFER_HPP
#define ZIVC_REINTERP_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer_init_params.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
class BufferCommon;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <DerivedBuffer Derived, KernelArg T>
class ReinterpBuffer : public Buffer<T>
{
 public:
  // Type aliases
  using Type = typename Buffer<T>::Type;
  using ConstType = typename Buffer<T>::ConstType;
  using Reference = typename Buffer<T>::Reference;
  using ConstReference = typename Buffer<T>::ConstReference;
  using Pointer = typename Buffer<T>::Pointer;
  using ConstPointer = typename Buffer<T>::ConstPointer;
  using BufferT = std::remove_volatile_t<Derived>;
  using BufferP = std::add_pointer_t<BufferT>;
  using ConstBufferP = std::add_const_t<BufferP>;


  //! Initialize the buffer
  ReinterpBuffer(BufferP buffer, IdData&& id) noexcept;

  //! Move data
  ReinterpBuffer(ReinterpBuffer&& other) noexcept;

  //! Finalize the buffer
  ~ReinterpBuffer() noexcept override;


  //! Return the capacity of the buffer in bytes
  std::size_t capacityInBytes() const noexcept override;

  //! Return the parent pointer
  ZivcObject* getParent() noexcept override;

  //! Return the parent pointer
  const ZivcObject* getParent() const noexcept override;

  //! Return the own pointer
  ZivcObject* getOwn() noexcept override;

  //! Return the own pointer
  const ZivcObject* getOwn() const noexcept override;

  //! Return the index of used heap
  std::size_t heapIndex() const noexcept override;

  //! Check if the zivc object is in debug mode
  bool isDebugMode() const noexcept override;

  //! Check if the buffer is the most efficient for the device access
  bool isDeviceLocal() const noexcept override;

  //! Check if the buffer is cached on the host
  bool isHostCached() const noexcept override;

  //! Check if the buffer doesn't need to be unmapped
  bool isHostCoherent() const noexcept override;

  //! Check if the buffer can be mapped for the host access
  bool isHostVisible() const noexcept override;

  //! Map a buffer memory to a host
  [[nodiscard]]
  void* mapMemoryData() const override;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept override;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept override;

  //! Return the underlying buffer data
  void* rawBufferData() noexcept override;

  //! Return the underlying buffer data
  const void* rawBufferData() const noexcept override;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the size of the buffer in bytes
  std::size_t sizeInBytes() const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Unmap a buffer memory
  void unmapMemoryData() const noexcept override;

 protected:
  //! Clear the contents of the buffer
  void destroyData() noexcept override;

  //! Initialize the buffer
  void initData(const BufferInitParams& params) override;

  //! Update debug info
  void updateDebugInfoImpl() noexcept override;

 private:
  //! Return the internal buffer pointer
  BufferP internalBuffer() noexcept;

  //! Return the internal buffer pointer
  ConstBufferP internalBuffer() const noexcept;


  BufferP buffer_ = nullptr;
};

} // namespace zivc

#include "reinterp_buffer-inl.hpp"

#endif // ZIVC_REINTERP_BUFFER_HPP
