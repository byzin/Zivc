/*!
  \file cpu_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_BUFFER_INL_HPP
#define ZIVC_CPU_BUFFER_INL_HPP

#include "cpu_buffer.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <exception>
#include <memory>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "cpu_device.hpp"
#include "zivc/backend.hpp"
#include "zivc/buffer.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/auxiliary/buffer_init_params.hpp"
#include "zivc/auxiliary/buffer_launch_options.hpp"
#include "zivc/auxiliary/error.hpp"
#include "zivc/auxiliary/id_data.hpp"
#include "zivc/auxiliary/launch_result.hpp"
#include "zivc/auxiliary/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <KernelArg T> inline
CpuBuffer<T>::CpuBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
CpuBuffer<T>::~CpuBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t CpuBuffer<T>::capacityInBytes() const noexcept
{
  const std::size_t n = (buffer_data_) ? rawBuffer().capacity() : 0;
  const std::size_t c = sizeof(Type) * n;
  return c;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto CpuBuffer<T>::data() noexcept -> Pointer
{
  return buffer_data_ ? rawBuffer().data() : nullptr;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto CpuBuffer<T>::data() const noexcept -> ConstPointer
{
  return buffer_data_ ? rawBuffer().data() : nullptr;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
std::size_t CpuBuffer<T>::heapIndex() const noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool CpuBuffer<T>::isDeviceLocal() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool CpuBuffer<T>::isHostCached() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool CpuBuffer<T>::isHostCoherent() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
bool CpuBuffer<T>::isHostVisible() const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
void* CpuBuffer<T>::mapMemoryData() const
{
  Pointer d = const_cast<Pointer>(data());
  ZIVC_ASSERT(d != nullptr, "The data is null.");
  return static_cast<void*>(d);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
void* CpuBuffer<T>::rawBufferData()
{
  void* p = static_cast<void*>(data());
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const void* CpuBuffer<T>::rawBufferData() const noexcept
{
  const void* p = static_cast<const void*>(data());
  return p;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <KernelArg T> inline
void CpuBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_size = Buffer<T>::size();
  if (s != prev_size) {
    prepareBuffer();
    zisc::pmr::vector<Type>& buff = rawBuffer();
    const std::size_t prev_cap = buff.capacity();

    // Allocate new memory
    try {
      buff.resize(s);
    }
    catch (const std::exception& error) {
      std::string message = "Buffer memory allocation failed. size: " +
                            std::to_string(s) + " bytes.";
      message = createErrorMessage(*this, message);
      throw SystemError{ErrorCode::kBufferMemoryAllocationFailed, message};
    }

    const std::size_t cap = buff.capacity();
    if (cap != prev_cap) {
      CpuDevice& device = parentImpl();
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
template <KernelArg T> inline
std::size_t CpuBuffer<T>::sizeInBytes() const noexcept
{
  const std::size_t n = (buffer_data_) ? rawBuffer().size() : 0;
  const std::size_t s = sizeof(Type) * n;
  return s;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void CpuBuffer<T>::unmapMemoryData() const noexcept
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void CpuBuffer<T>::destroyData() noexcept
{
  buffer_data_.reset();
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <KernelArg T> inline
void CpuBuffer<T>::initData([[maybe_unused]] const BufferInitParams& params)
{
  prepareBuffer();
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void CpuBuffer<T>::updateDebugInfoImpl() noexcept
{
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> template <KernelArg D> inline
LaunchResult CpuBuffer<T>::copyFromImpl(const BufferCommon& source,
                                        BufferCommon* dest,
                                        const BufferLaunchOptions<D>& launch_options)
{
  using DataT = std::remove_cvref_t<D>;
  using DataPtr = std::add_pointer_t<DataT>;
  using ConstDataPtr = std::add_pointer_t<std::add_const_t<DataT>>;
  const auto* src_ptr = static_cast<ConstDataPtr>(source.rawBufferData());
  src_ptr = src_ptr + launch_options.sourceOffset();
  auto* dst_ptr = static_cast<DataPtr>(dest->rawBufferData());
  dst_ptr = dst_ptr + launch_options.destOffset();
  std::copy_n(src_ptr, launch_options.size(), dst_ptr);

  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [out] dest No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> template <KernelArg D> inline
LaunchResult CpuBuffer<T>::fillImpl(typename Buffer<D>::ConstReference value,
                                    BufferCommon* dest,
                                    const BufferLaunchOptions<D>& launch_options)
{
  using DataT = std::remove_cvref_t<D>;
  using DataPtr = std::add_pointer_t<DataT>;
  auto* dst_ptr = static_cast<DataPtr>(dest->rawBufferData());
  dst_ptr = dst_ptr + launch_options.destOffset();
  std::fill_n(dst_ptr, launch_options.size(), value);

  LaunchResult result{};
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
CpuDevice& CpuBuffer<T>::parentImpl() noexcept
{
  ZivcObject* p = Buffer<T>::getParent();
  return *static_cast<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
const CpuDevice& CpuBuffer<T>::parentImpl() const noexcept
{
  const ZivcObject* p = Buffer<T>::getParent();
  return *static_cast<const CpuDevice*>(p);
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void CpuBuffer<T>::prepareBuffer() noexcept
{
  if (!buffer_data_) {
    zisc::pmr::memory_resource* mem_resource = Buffer<T>::memoryResource();
    using BufferImplTypeT = typename decltype(buffer_data_)::element_type;
    typename BufferImplTypeT::allocator_type alloc{mem_resource};
    buffer_data_ = zisc::pmr::allocateUnique<BufferImplTypeT>(mem_resource,
                                                              BufferImplTypeT{alloc});
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto CpuBuffer<T>::rawBuffer() noexcept -> zisc::pmr::vector<Type>&
{
  return *buffer_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto CpuBuffer<T>::rawBuffer() const noexcept -> const zisc::pmr::vector<Type>&
{
  return *buffer_data_;
}

} // namespace zivc

#endif // ZIVC_CPU_BUFFER_INL_HPP
