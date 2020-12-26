/*!
  \file buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_HPP
#define ZIVC_BUFFER_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/zisc_config.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc_config.hpp"
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/zivc_object.hpp"

namespace zivc {

// Forward declaration
template <typename> class MappedMemory;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
class Buffer : public ZivcObject
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


  /*!
    \brief No brief description

    No detailed description.
    */
  class LaunchOptions
  {
   public:
    //! Initialize the launch info
    LaunchOptions() noexcept;

    //! Initialize the launch info
    LaunchOptions(const std::size_t s) noexcept;

    //! Initialize the launch info
    LaunchOptions(const std::size_t s, const uint32b queue_index) noexcept;


    //! Return the offset index into the dest buffer at which to start
    std::size_t destOffset() const noexcept;

    //! Return the byte offset into the dest buffer at which to start
    std::size_t destOffsetInBytes() const noexcept;

    //! Check whether external sync mode is required
    bool isExternalSyncMode() const noexcept;

    //! Return the label of the launching
    std::string_view label() const noexcept;

    //! Return the color of the label
    const std::array<float, 4>& labelColor() const noexcept;

    //! Return the queue index
    uint32b queueIndex() const noexcept;

    //! Set the offset index into the dest buffer at which to start
    void setDestOffset(const std::size_t offset) noexcept;

    //! Set external sync mode
    void setExternalSyncMode(const bool is_active) noexcept;

    //! Set the queue index which is used for a kernel execution
    void setQueueIndex(const uint32b queue_index) noexcept;

    //! Set the label of the launching
    void setLabel(const std::string_view launch_label) noexcept;

    //! Set the color of the label
    void setLabelColor(const std::array<float, 4>& label_color) noexcept;

    //! Set the size of elements to be operated
    void setSize(const std::size_t s) noexcept;

    //! Set the offset index into the source buffer at which to start
    void setSourceOffset(const std::size_t offset) noexcept;

    //! Return the size of elements to be operated
    std::size_t size() const noexcept;

    //! Return the size in bytes to be operated
    std::size_t sizeInBytes() const noexcept;

    //! Return the offset index into the source buffer at which to start
    std::size_t sourceOffset() const noexcept;

    //! Return the byte offset into the source buffer at which to start
    std::size_t sourceOffsetInBytes() const noexcept;

   private:
    //! Initialize the options
    void initialize() noexcept;


    IdData::NameType label_;
    std::array<float, 4> label_color_{1.0f, 1.0f, 1.0f, 1.0f};
    std::size_t source_offset_ = 0;
    std::size_t dest_offset_ = 0;
    std::size_t size_ = kBufferWholeSize;
    uint32b queue_index_ = 0;
    uint8b is_external_sync_mode_ = zisc::kFalse;
    std::array<uint8b, 3> padding_;
  };


  //! Initialize the buffer
  Buffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~Buffer() noexcept override;


  //! Destroy the buffer
  void destroy() noexcept;

  //! Clear the contents of the buffer
  void clear() noexcept;

  //! Copy from the given buffer
  LaunchResult copyFrom(const Buffer& source,
                        const LaunchOptions& launch_options = LaunchOptions{});

  //! Fill the buffer with specified value
  LaunchResult fill(ConstReference value,
                    const LaunchOptions& launch_options = LaunchOptions{});

  //! Initialize the buffer
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const BufferUsage buffer_usage);

  //! Check if the buffer is the most efficient for the device access
  virtual bool isDeviceLocal() const noexcept = 0;

  //! Check if the buffer is cached on the host
  virtual bool isHostCached() const noexcept = 0;

  //! Check if the buffer doesn't need to be unmapped
  virtual bool isHostCoherent() const noexcept = 0;

  //! Check if the buffer can be mapped for the host access
  virtual bool isHostVisible() const noexcept = 0;

  //! Make launch options
  LaunchOptions makeOptions() const noexcept;

  //! Map a buffer memory to a host
  MappedMemory<Type> mapMemory();

  //! Map a buffer memory to a host
  MappedMemory<ConstType> mapMemory() const noexcept;

  //! Convert a type of a buffer interface to DstType
  template <typename DstType>
  Buffer<DstType>* reinterp() noexcept;

  //! Convert a type of a buffer interface to DstType
  template <typename DstType>
  const Buffer<DstType>* reinterp() const noexcept;

  //! Change the number of elements
  virtual void setSize(const std::size_t s) = 0;

  //! Return the number of elements
  virtual std::size_t size() const noexcept = 0;

  //! Return the buffer usage flag
  BufferUsage usage() const noexcept;

 protected:
  friend MappedMemory<Type>;
  friend MappedMemory<ConstType>;


  //! Copy from the given buffer
  virtual LaunchResult copyFromImpl(const Buffer& source,
                                    const LaunchOptions& launch_options) = 0;

  //! Clear the contents of the buffer
  virtual void destroyData() noexcept = 0;

  //! Fill the buffer with specified value
  virtual LaunchResult fillImpl(ConstReference value,
                                const LaunchOptions& launch_options) = 0;

  //! Initialize the buffer
  virtual void initData() = 0;

  //! Map a buffer memory to a host
  virtual Pointer mappedMemory() const = 0;

  //! Process the given options for the buffer
  LaunchOptions processOptions(LaunchOptions launch_options) const noexcept;

  //! Unmap a buffer memory
  virtual void unmapMemory() const noexcept = 0;

 private:
  BufferUsage buffer_usage_;
};

// Type aliases
template <typename Type>
using SharedBuffer = typename Buffer<Type>::SharedPtr;
template <typename Type>
using WeakBuffer = typename Buffer<Type>::WeakPtr;
template <typename Type>
using BufferLaunchOptions = typename Buffer<Type>::LaunchOptions;

//! Copy from the source to the dest
template <typename Type>
LaunchResult copy(const Buffer<Type>& source,
                  Buffer<Type>* dest,
                  const BufferLaunchOptions<Type>& launch_options =
                      BufferLaunchOptions<Type>{});

} // namespace zivc

#include "buffer-inl.hpp"

#endif // ZIVC_BUFFER_HPP

///*!
//  \file buffer.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZIVC_BUFFER_HPP
//#define ZIVC_BUFFER_HPP
//
//// Standard C++ library
//#include <cstddef>
//#include <type_traits>
//// Zisc
//#include "zisc/unique_memory_pointer.hpp"
//#include "zisc/non_copyable.hpp"
//// Zivc
//#include "zivc/mapped_memory.hpp"
//#include "zivc/zivc_config.hpp"
//
//namespace zivc {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T>
//class Buffer : private zisc::NonCopyable<Buffer<kDescriptor, T>>
//{
// public:
//  //! The type of the buffer. "const", "volatile" and "reference" are removed
//  using Type = std::remove_cv_t<std::remove_reference_t<T>>;
//  using ConstType = std::add_const_t<Type>;
//  using Pointer = std::add_pointer_t<Type>;
//  using ConstPointer = std::add_pointer_t<ConstType>;
//
//
//  //! Initialize a buffer
//  Buffer(const BufferUsage usage_flag) noexcept;
//
//  //!
//  virtual ~Buffer() noexcept;
//
//
//  //! Return the buffer type
//  static constexpr DescriptorType descriptorType() noexcept;
//
//  //! Return the device type
//  virtual SubPlatformType SubPlatformType() const noexcept = 0;
//
//  //! Check if a buffer memory is on device
//  virtual bool isDeviceMemory() const noexcept = 0;
//
//  //! Check if a buffer memory is on host
//  virtual bool isHostMemory() const noexcept = 0;
//
//  //! Check if a buffer is host visible
//  virtual bool isHostVisible() const noexcept = 0;
//
//  //! Map a buffer memory to a host
//  MappedMemory<kDescriptor, Type> mapMemory() noexcept;
//
//  //! Map a buffer memory to a host
//  MappedMemory<kDescriptor, ConstType> mapMemory() const noexcept;
//
//  //! Return the memory usage
//  virtual std::size_t memoryUsage() const noexcept = 0;
//
//  //! Read a data from a buffer
//  virtual void read(Pointer data,
//                    const std::size_t count,
//                    const std::size_t offset,
//                    const uint32b queue_index) const noexcept = 0;
//
//  //! Change the number of elements
//  virtual void setSize(const std::size_t s) noexcept = 0;
//
//  //! Return the number of elements
//  virtual std::size_t size() const noexcept = 0;
//
//  //! Convert a type of a buffer interface to DstType
//  template <typename DstType>
//  Buffer<kDescriptor, DstType>* reinterp() noexcept;
//
//  //! Convert a type of a buffer interface to DstType
//  template <typename DstType>
//  const Buffer<kDescriptor, DstType>* reinterp() const noexcept;
//
//  //! Return the buffer usage flag
//  BufferUsage usage() const noexcept;
//
//  //! Write a data to a buffer
//  virtual void write(ConstPointer data,
//                     const std::size_t count,
//                     const std::size_t offset,
//                     const uint32b queue_index) noexcept = 0;
//
// private:
//  static_assert(!std::is_pointer_v<T>, "Pointer type is restricted.");
//
//
//  friend MappedMemory<kDescriptor, Type>;
//  friend MappedMemory<kDescriptor, ConstType>;
//
//
//  //! Initialize a buffer
//  void initialize() noexcept;
//
//  //! Map a buffer memory to a host
//  virtual Pointer mappedMemory() const noexcept = 0;
//
//  //! Unmap a buffer memory
//  virtual void unmapMemory() const noexcept = 0;
//
//
//  BufferUsage usage_flag_;
//};
//
//// Type aliases
//template <typename Type>
//using UniformBuffer = Buffer<DescriptorType::kUniform, Type>;
//template <typename Type>
//using StorageBuffer = Buffer<DescriptorType::kStorage, Type>;
//template <DescriptorType kDescriptor, typename Type>
//using UniqueBuffer = zisc::UniqueMemoryPointer<Buffer<kDescriptor, Type>>;
//
//} // namespace zivc
//
//#include "buffer-inl.hpp"
//
//#endif // ZIVC_BUFFER_HPP
