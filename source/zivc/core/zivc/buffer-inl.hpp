/*!
  \file buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_INL_HPP
#define ZIVC_BUFFER_INL_HPP

#include "buffer.hpp"
// Standard C++ library
#include <cstring>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
// Zivc
#include "zivc_config.hpp"
#include "utility/buffer_common.hpp"
#include "utility/buffer_init_params.hpp"
#include "utility/buffer_launch_options.hpp"
#include "utility/error.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/mapped_memory.hpp"
#include "utility/reinterp_buffer.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <KernelArg T> inline
Buffer<T>::Buffer(IdData&& id) noexcept : BufferCommon(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void Buffer<T>::clear() noexcept
{
  destroyData();
}

/*!
  \details No detailed description

  \param [in] source No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> inline
LaunchResult Buffer<T>::copyFrom(const Buffer& source,
                                 const LaunchOptionsT& launch_options)
{
  auto result = zivc::copy(source, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto Buffer<T>::createOptions() const noexcept -> LaunchOptionsT
{
  LaunchOptionsT options{size()};
  return options;
}

/*!
  \details No detailed description
  */
template <KernelArg T> inline
void Buffer<T>::destroy() noexcept
{
  clear();
  destroyObject();
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T> inline
LaunchResult Buffer<T>::fill(ConstReference value,
                             const LaunchOptionsT& launch_options)
{
  auto result = zivc::fill(value, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] params No description.
  */
template <KernelArg T> inline
void Buffer<T>::initialize(ZivcObject::SharedPtr&& parent,
                           WeakPtr&& own,
                           const BufferInitParams& params)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initCommon(params);
  initData(params);

  ZivcObject::setNameIfEmpty("Buffer");
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto Buffer<T>::mapMemory() -> MappedMemory<Type>
{
  if (!isHostVisible()) [[unlikely]] {
    const std::string message = createErrorMessage(
        *this,
        "isHostVisible()==false buffer cannot be mapped to host address space.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  MappedMemory<Type> memory{this};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
auto Buffer<T>::mapMemory() const -> MappedMemory<ConstType>
{
  if (!isHostVisible()) [[unlikely]] {
    const std::string message = createErrorMessage(
        *this,
        "isHostVisible()==false buffer cannot be mapped to host address space.");
    throw SystemError{ErrorCode::kInvalidInstruction, message};
  }
  MappedMemory<ConstType> memory{this};
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> template <KernelArg NewType> inline
auto Buffer<T>::reinterp() noexcept -> ReinterpBufferT<NewType>
{
  auto id_copy = id().copy();
  auto own = static_cast<BufferCommon*>(getOwn());
  ReinterpBufferT<NewType> new_buffer{own, std::move(id_copy)};
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> template <KernelArg NewType> inline
auto Buffer<T>::reinterp() const noexcept -> ConstReinterpBufferT<NewType>
{
  auto id_copy = id().copy();
  auto own = static_cast<const BufferCommon*>(getOwn());
  ConstReinterpBufferT<NewType> new_buffer{own, std::move(id_copy)};
  return new_buffer;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
constexpr std::size_t Buffer<T>::typeSizeInBytes() const noexcept
{
  const std::size_t size = sizeof(Type);
  return size;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T>
template <template<typename> typename Derived, KernelArg SrcType> inline
LaunchResult Buffer<T>::copyFromDerived(const Buffer<SrcType>& source,
                                        const LaunchOptionsT& launch_options)
{
  //! \todo Remove me
  ZIVC_ASSERT(type() == source.type(), "Type mismatch found.");
  //! \todo Remove me
  ZIVC_ASSERT(getParent() == source.getParent(), "Type mismatch found.");
  auto result = Derived<T>::copyFromImpl(source, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T>
template <template<typename> typename Derived> inline
LaunchResult Buffer<T>::fillDerived(ConstReference value,
                                    const LaunchOptionsT& launch_options)
{
  auto result = Derived<T>::fillImpl(value, this, launch_options);
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg T> inline
void Buffer<T>::initCommon(const BufferInitParams& params) noexcept
{
  setUsage(params.usage());
  setFlag(params.flag());
}

} // namespace zivc

#endif // ZIVC_BUFFER_INL_HPP
