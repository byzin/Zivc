/*!
  \file cpu_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_BUFFER_HPP
#define ZIVC_CPU_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/buffer_init_params.hpp"
#include "zivc/utility/buffer_launch_options.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
class CpuDevice;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <KernelArg T>
class CpuBuffer : public Buffer<T>
{
 public:
  // Type aliases
  using Type = typename Buffer<T>::Type;
  using ConstType = typename Buffer<T>::ConstType;
  using Reference = typename Buffer<T>::Reference;
  using ConstReference = typename Buffer<T>::ConstReference;
  using Pointer = typename Buffer<T>::Pointer;
  using ConstPointer = typename Buffer<T>::ConstPointer;
  using LaunchOptionsT = typename Buffer<T>::LaunchOptionsT;


  //! Initialize the buffer
  CpuBuffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~CpuBuffer() noexcept override;


  //! Return the capacity of the buffer in bytes
  std::size_t capacityInBytes() const noexcept override;

  //! Return the underlying data pointer
  Pointer data() noexcept;

  //! Return the underlying data pointer
  ConstPointer data() const noexcept;

  //! Return the index of used heap
  std::size_t heapIndex() const noexcept override;

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

  //! Return the underlying buffer data
  void* rawBufferData() override;

  //! Return the underlying buffer data
  const void* rawBufferData() const noexcept override;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the size of the buffer in bytes
  std::size_t sizeInBytes() const noexcept override;

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
  friend Buffer<T>;


  //! Copy from the given buffer
  template <KernelArg D>
  static LaunchResult copyFromImpl(const BufferCommon& source,
                                   BufferCommon* dest,
                                   const BufferLaunchOptions<D>& launch_options);

  //! Fill the buffer with specified value
  template <KernelArg D>
  static LaunchResult fillImpl(typename Buffer<D>::ConstReference value,
                               BufferCommon* dest,
                               const BufferLaunchOptions<D>& launch_options);

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;

  //! Prepare a buffer for use
  void prepareBuffer() noexcept;

  //! Return the underlying buffer data
  zisc::pmr::vector<Type>& rawBuffer() noexcept;

  //! Return the underlying buffer data
  const zisc::pmr::vector<Type>& rawBuffer() const noexcept;


  ZivcObject::UniqueVectorT<Type> buffer_data_;
};

} // namespace zivc

#include "cpu_buffer-inl.hpp"

#endif // ZIVC_CPU_BUFFER_HPP
