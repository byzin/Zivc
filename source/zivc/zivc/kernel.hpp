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
#include <cstddef>
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/algorithm.hpp"
// Zivc
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <typename Type> class Buffer;
template <typename SetType, typename ...ArgTypes> class KernelParameters;

template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class Kernel
{
  static_assert(kDimension == std::numeric_limits<std::size_t>::max());
};

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
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
  using Parameters = KernelParameters<SetType, FuncArgTypes...>;


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
    LaunchOptions(const std::array<uint32b, kDimension>& work_size) noexcept;

    //! Initialize the launch info
    LaunchOptions(const std::array<uint32b, kDimension>& work_size,
                  const uint32b queue_index) noexcept;


    //! Return the work-group dimension
    static constexpr std::size_t dimension() noexcept;

    //! Return the number of kernel arguments
    static constexpr std::size_t numOfArgs() noexcept;

    //! Return the queue index
    uint32b queueIndex() const noexcept;

    //! Set the queue index which is used for a kernel execution
    void setQueueIndex(const uint32b queue_index) noexcept;

    //! Set the work group size
    void setWorkSize(const uint32b work_size, const std::size_t dim) noexcept;

    //! Set the work group size
    void setWorkSize(const std::array<uint32b, kDimension>& work_size) noexcept;

    //! Return the work group size
    const std::array<uint32b, kDimension>& workSize() const noexcept;

   private:
    static constexpr std::size_t kNumOfArgs = sizeof...(ArgTypes);


    std::array<uint32b, kDimension> work_size_;
    uint32b queue_index_ = 0;
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
  virtual void run(std::add_lvalue_reference_t<ArgTypes>... args,
                   const LaunchOptions& launch_options) = 0;

 protected:
  //! Clear the contents of the kernel
  virtual void destroyData() noexcept = 0;

  //! Initialize the kernel 
  virtual void initData(const Parameters& params) = 0;
};

} // namespace zivc

#include "kernel-inl.hpp"

#endif // ZIVC_KERNEL_HPP
