/*!
  \file buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_HPP
#define ZIVC_BUFFER_HPP

// Standard C++ library
#include <concepts>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zivc
#include "zivc_config.hpp"
#include "utility/buffer_common.hpp"
#include "utility/buffer_init_params.hpp"
#include "utility/buffer_launch_options.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class MappedMemory;
template <DerivedBuffer, KernelArg> class ReinterpBuffer;


/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <KernelArg T>
class Buffer : public BufferCommon
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Buffer>;
  using WeakPtr = std::weak_ptr<Buffer>;
  using Type = std::remove_cv_t<std::remove_reference_t<T>>;
  using ConstType = std::add_const_t<Type>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_lvalue_reference_t<ConstType>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;
  using LaunchOptionsT = BufferLaunchOptions<Type>;
  template <KernelArg NewType>
  using ReinterpBufferT = ReinterpBuffer<BufferCommon, NewType>;
  template <KernelArg NewType>
  using ConstReinterpBufferT = ReinterpBuffer<const BufferCommon, NewType>;


  //! Initialize the buffer
  Buffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~Buffer() noexcept override;


  //! Clear the contents of the buffer
  void clear() noexcept override;

  //! Copy from the given buffer
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult copyFrom(const Buffer& source, const LaunchOptionsT& launch_options);

  //! Create launch options
  LaunchOptionsT createOptions() const noexcept;

  //! Destroy the buffer
  void destroy() noexcept override;

  //! Fill the buffer with specified value
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult fill(ConstReference value, const LaunchOptionsT& launch_options);

  //! Initialize the buffer
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const BufferInitParams& params);

  //! Map a buffer memory to a host
  [[nodiscard]]
  MappedMemory<Type> mapMemory();

  //! Map a buffer memory to a host
  [[nodiscard]]
  MappedMemory<ConstType> mapMemory() const;

  //! Convert a type of a buffer interface to NewType
  template <KernelArg NewType>
  [[nodiscard]]
  ReinterpBufferT<NewType> reinterp() noexcept;

  //! Convert a type of a buffer interface to NewType
  template <KernelArg NewType>
  [[nodiscard]]
  ConstReinterpBufferT<NewType> reinterp() const noexcept;

  //! Return the size of the element type in bytes
  constexpr std::size_t typeSizeInBytes() const noexcept override;

 protected:
  template <KernelArg SrcType, std::same_as<std::remove_const_t<SrcType>> DstType>
  friend LaunchResult copy(const Buffer<SrcType>&,
                           Buffer<DstType>*,
                           const BufferLaunchOptions<DstType>&);
  template <KernelArg Type>
  friend LaunchResult fill(typename Buffer<Type>::ConstReference,
                           Buffer<Type>*,
                           const BufferLaunchOptions<Type>&);


  //! Copy from the given buffer
  template <template<typename> typename Derived, KernelArg SrcType>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult copyFromDerived(const Buffer<SrcType>& source,
                               const LaunchOptionsT& launch_options);

  //! Clear the contents of the buffer
  virtual void destroyData() noexcept = 0;

  //! Fill the buffer with specified value
  template <template<typename> typename Derived>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult fillDerived(ConstReference value,
                           const LaunchOptionsT& launch_options);

  //! Initialize the buffer common data
  void initCommon(const BufferInitParams& params) noexcept;

  //! Initialize the buffer
  virtual void initData(const BufferInitParams& params) = 0;
};


// Type aliases
template <KernelArg Type>
using SharedBuffer = typename Buffer<Type>::SharedPtr;
template <KernelArg Type>
using WeakBuffer = typename Buffer<Type>::WeakPtr;


//! Copy from the source to the dest
template <KernelArg SrcType, std::same_as<std::remove_const_t<SrcType>> DstType>
[[nodiscard("The result can have a fence when external sync mode is on.")]]
LaunchResult copy(const Buffer<SrcType>& source,
                  Buffer<DstType>* dest,
                  const BufferLaunchOptions<DstType>& launch_options);

//! Fill the buffer with specified value
template <KernelArg Type>
[[nodiscard("The result can have a fence when external sync mode is on.")]]
LaunchResult fill(typename Buffer<Type>::ConstReference value,
                  Buffer<Type>* dest,
                  const BufferLaunchOptions<Type>& launch_options);

} // namespace zivc

#include "buffer-inl.hpp"

#endif // ZIVC_BUFFER_HPP
