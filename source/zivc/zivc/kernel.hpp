/*!
  \file kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_HPP
#define ZIVC_KERNEL_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/zisc_config.hpp"
// Zivc
#include "utility/id_data.hpp"
#include "utility/launch_result.hpp"
#include "utility/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <typename Type> class Buffer;
template <typename SetType, typename ...ArgTypes> class KernelParameters;
template <typename ...ArgTypes> class KernelArgParser;

template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class Kernel
{
  static_assert(kDimension == std::numeric_limits<std::size_t>::max());
};

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
  \tparam SetType No description.
  \tparam FuncArgTypes No description.
  \tparam ArgTypes No description.
  */
template <std::size_t kDimension, typename SetType, typename ...FuncArgTypes, typename ...ArgTypes>
class Kernel<kDimension, KernelParameters<SetType, FuncArgTypes...>, ArgTypes...>
    : public ZivcObject
{
  static_assert(zisc::Algorithm::isInBounds(kDimension, 1u, 4u),
                "The kDimension should be 1, 2 or 3.");

 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Kernel>;
  using WeakPtr = std::weak_ptr<Kernel>;
  using ArgParser = KernelArgParser<FuncArgTypes...>;
  using Parameters = KernelParameters<SetType, FuncArgTypes...>;


  /*!
    \brief No brief description

    No detailed description.
    */
  class LaunchOptions
  {
   public:
    static constexpr std::size_t kNumOfArgs = sizeof...(ArgTypes);


    using CommandStorage = std::aligned_storage_t<
        sizeof(void*) * (kNumOfArgs + 2),
        std::alignment_of_v<void*>>;
    using AtomicStorage = std::aligned_storage_t<
        sizeof(std::atomic<uint32b>),
        std::alignment_of_v<std::atomic<uint32b>>>;


    //! Initialize the launch info
    LaunchOptions() noexcept;

    //! Initialize the launch info
    LaunchOptions(const std::array<uint32b, kDimension>& work_size) noexcept;

    //! Initialize the launch info
    LaunchOptions(const std::array<uint32b, kDimension>& work_size,
                  const uint32b queue_index) noexcept;


    //! Return the memory for cpu command
    CommandStorage* cpuCommandStorage() noexcept;

    //! Return the memory for cpu command
    const CommandStorage* cpuCommandStorage() const noexcept;

    //! Return the memory for an atomic counter in cpu command
    AtomicStorage* cpuAtomicStorage() noexcept;

    //! Return the memory for an atomic counter in cpu command
    const AtomicStorage* cpuAtomicStorage() const noexcept;

    //! Return the work-group dimension
    static constexpr std::size_t dimension() noexcept;

    //! Check whether external sync mode is required
    bool isExternalSyncMode() const noexcept;

    //! Return the label of the launching
    std::string_view label() const noexcept;

    //! Return the color of the label
    const std::array<float, 4>& labelColor() const noexcept;

    //! Return the number of kernel arguments
    static constexpr std::size_t numOfArgs() noexcept;

    //! Return the queue index
    uint32b queueIndex() const noexcept;

    //! Set external sync mode
    void setExternalSyncMode(const bool is_active) noexcept;

    //! Set the label of the launching
    void setLabel(const std::string_view launch_label) noexcept;

    //! Set the color of the label
    void setLabelColor(const std::array<float, 4>& label_color) noexcept;

    //! Set the queue index which is used for a kernel execution
    void setQueueIndex(const uint32b queue_index) noexcept;

    //! Set the work item size
    void setWorkSize(const uint32b work_size, const std::size_t dim) noexcept;

    //! Set the work item size
    void setWorkSize(const std::array<uint32b, kDimension>& work_size) noexcept;

    //! Return the work item size
    const std::array<uint32b, kDimension>& workSize() const noexcept;

   private:
    //! Initialize the options
    void initialize() noexcept;


    CommandStorage cpu_command_storage_;
    AtomicStorage cpu_atomic_storage_;
    IdData::NameType label_;
    std::array<float, 4> label_color_ = {1.0f, 1.0f, 1.0f, 1.0f};
    std::array<uint32b, kDimension> work_size_;
    uint32b queue_index_ = 0;
    int8b is_external_sync_mode_ = zisc::kFalse;
    std::array<uint8b, std::alignment_of_v<void*> - 1> padding_;
  };


  //! Initialize the kernel
  Kernel(IdData&& id) noexcept;

  //! Finalize the kernel
  virtual ~Kernel() noexcept;


  //! Destroy the buffer
  void destroy() noexcept;

  //! Return the work-group dimension
  static constexpr std::size_t dimension() noexcept;

  //! Initialize the kernel
  void initialize(ZivcObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const Parameters& params);

  //! Make launch options
  LaunchOptions makeOptions() const noexcept;

  //! Return the number of kernel arguments
  static constexpr std::size_t numOfArgs() noexcept;

  //! Execute a kernel
  virtual LaunchResult run(ArgTypes... args, LaunchOptions& launch_options) = 0;

 protected:
  //! Clear the contents of the kernel
  virtual void destroyData() noexcept = 0;

  //! Expand the given work size to 3d work size array
  static std::array<uint32b, 3> expandWorkSize(
      const std::array<uint32b, kDimension>& work_size) noexcept;

  //! Initialize the kernel 
  virtual void initData(const Parameters& params) = 0;
};

} // namespace zivc

#include "kernel-inl.hpp"

#endif // ZIVC_KERNEL_HPP
