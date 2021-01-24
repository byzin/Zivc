/*!
  \file buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_HPP
#define ZIVC_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/zisc_config.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer_common.hpp"
#include "zivc_config.hpp"
#include "utility/buffer_launch_options.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
template <zisc::TriviallyCopyable> class MappedMemory;


/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <zisc::TriviallyCopyable T>
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
  using LaunchOptions = BufferLaunchOptions<Type>;


  //! Initialize the buffer
  Buffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~Buffer() noexcept override;


  //! Return the capacity of the buffer
  std::size_t capacity() const noexcept;

  //! Clear the contents of the buffer
  void clear() noexcept;

  //! Copy from the given buffer
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult copyFrom(const Buffer& source,
                        const LaunchOptions& launch_options = LaunchOptions{});

  //! Destroy the buffer
  void destroy() noexcept;

  //! Fill the buffer with specified value
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult fill(ConstReference value,
                    const LaunchOptions& launch_options = LaunchOptions{});

  //! Initialize the buffer
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const BufferUsage buffer_usage);

  //! Map a buffer memory to a host
  [[nodiscard]]
  MappedMemory<Type> makeMappedMemory();

  //! Map a buffer memory to a host
  [[nodiscard]]
  MappedMemory<ConstType> makeMappedMemory() const;

  //! Make launch options
  LaunchOptions makeOptions() const noexcept;

  //! Convert a type of a buffer interface to NewType
  template <zisc::TriviallyCopyable NewType>
  [[nodiscard]]
  Buffer<NewType>* reinterp() noexcept;

  //! Convert a type of a buffer interface to NewType
  template <zisc::TriviallyCopyable NewType>
  [[nodiscard]]
  const Buffer<NewType>* reinterp() const noexcept;

  //! Return the number of elements of the buffer
  std::size_t size() const noexcept;

 protected:
  template <zisc::TriviallyCopyable Type>
  friend LaunchResult copy(const Buffer<Type>&,
                           Buffer<Type>*,
                           const BufferLaunchOptions<Type>&);
  template <zisc::TriviallyCopyable Type>
  friend LaunchResult fill(typename Buffer<Type>::ConstReference,
                           Buffer<Type>*,
                           const BufferLaunchOptions<Type>&);


  //! Copy from the given buffer
  template <template<typename> typename Derived>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult copyFromDerived(const Buffer<T>& source,
                               const LaunchOptions& launch_options);

  //! Clear the contents of the buffer
  virtual void destroyData() noexcept = 0;

  //! Fill the buffer with specified value
  template <template<typename> typename Derived>
  [[nodiscard("The result can have a fence when external sync mode is on.")]]
  LaunchResult fillDerived(ConstReference value,
                           const LaunchOptions& launch_options);

  //! Initialize the buffer
  virtual void initData() = 0;
};


// Type aliases
template <zisc::TriviallyCopyable Type>
using SharedBuffer = typename Buffer<Type>::SharedPtr;
template <zisc::TriviallyCopyable Type>
using WeakBuffer = typename Buffer<Type>::WeakPtr;


//! Copy from the source to the dest
template <zisc::TriviallyCopyable Type>
[[nodiscard("The result can have a fence when external sync mode is on.")]]
LaunchResult copy(const Buffer<Type>& source,
                  Buffer<Type>* dest,
                  const BufferLaunchOptions<Type>& launch_options =
                      BufferLaunchOptions<Type>{});

//! Fill the buffer with specified value
template <zisc::TriviallyCopyable Type>
[[nodiscard("The result can have a fence when external sync mode is on.")]]
LaunchResult fill(typename Buffer<Type>::ConstReference value,
                  Buffer<Type>* dest,
                  const BufferLaunchOptions<Type>& launch_options =
                      BufferLaunchOptions<Type>{});

} // namespace zivc

#include "buffer-inl.hpp"

#endif // ZIVC_BUFFER_HPP
