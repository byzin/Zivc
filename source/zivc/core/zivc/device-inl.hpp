/*!
  \file device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INL_HPP
#define ZIVC_DEVICE_INL_HPP

#include "device.hpp"
// Standard C++ library
#include <memory>
#include <stdexcept>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer.hpp"
#include "kernel_set.hpp"
#include "zivc_config.hpp"
#include "utility/buffer_init_params.hpp"
#include "utility/kernel_arg_parser.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] params No description.
  \return No description
  */
template <KernelArg T> inline
SharedBuffer<T> Device::makeBuffer(const BufferInitParams& params)
{
  auto buffer = ::zivc::makeBuffer<T>(this, params);
  return buffer;
}

/*!
  \details No detailed description

  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  \param [in] params No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto Device::makeKernel(const KernelInitParams<kDim, KSet, Args...>& params)
    -> SharedKernel<kDim, KSet, Args...>
{
  auto kernel = ::zivc::makeKernel<kDim, KSet, Args...>(this, params);
  return kernel;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const DeviceInfo& Device::deviceInfo() const noexcept
{
  return *device_info_;
}

/*!
  \details No detailed description

  \tparam Derived No description.
  \tparam T No description.
  \param [in] params No description.
  \return No description
  */
template <template<typename> typename Derived, KernelArg T> inline
SharedBuffer<T> Device::makeDerivedBuffer(const BufferInitParams& params)
{
  using BufferT = Derived<T>;
  zisc::pmr::polymorphic_allocator<BufferT> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferT>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwnPtr()};
  WeakBuffer<T> own{buffer};
  try {
    buffer->initialize(std::move(parent), std::move(own), params);
  }
  catch (const std::runtime_error& error) {
    buffer->destroy();
    buffer.reset();
    throw error;
  }

  return buffer;
}

/*!
  \details No detailed description

  \tparam Derived No description.
  \tparam kDim No description.
  \tparam KSet No description.
  \tparam Args No description.
  \param [in] params No description.
  \return No description
  */
template <template<typename, typename...> typename Derived,
          std::size_t kDim, DerivedKSet KSet, typename ...Args> inline
auto Device::makeDerivedKernel(const KernelInitParams<kDim, KSet, Args...>& params)
    -> SharedKernel<kDim, KSet, Args...>
{
  using Parser = KernelArgParser<Args...>;
  using KernelT = typename Parser::template KernelType<Derived, kDim, KSet>;
  using SharedKernelT = SharedKernel<kDim, KSet, Args...>;
  using WeakKernelT = WeakKernel<kDim, KSet, Args...>;

  zisc::pmr::polymorphic_allocator<KernelT> alloc{memoryResource()};
  SharedKernelT kernel = std::allocate_shared<KernelT>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwnPtr()};
  WeakKernelT own{kernel};
  try {
    kernel->initialize(std::move(parent), std::move(own), params);
  }
  catch (const std::runtime_error& error) {
    kernel->destroy();
    kernel.reset();
    throw error;
  }

  return kernel;
}

} // namespace zivc

#endif // ZIVC_DEVICE_INL_HPP
