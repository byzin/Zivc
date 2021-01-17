/*!
  \file device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_DEVICE_INL_HPP
#define ZIVC_DEVICE_INL_HPP

#include "device.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/error.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "buffer.hpp"
#include "kernel_set.hpp"
#include "zivc_config.hpp"
#include "utility/kernel_arg_parser.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <zisc::TriviallyCopyable T> inline
SharedBuffer<T> Device::makeBuffer(const BufferUsage flag)
{
  auto buffer = ::zivc::makeBuffer<T>(this, flag);
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
auto Device::makeKernel(const KernelParams<kDim, KSet, Args...>& params)
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
  \param [in] flag No description.
  \return No description
  */
template <template<typename> typename Derived, zisc::TriviallyCopyable T> inline
SharedBuffer<T> Device::makeDerivedBuffer(const BufferUsage flag)
{
  using BufferT = Derived<T>;
  zisc::pmr::polymorphic_allocator<BufferT> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferT>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakBuffer<T> own{buffer};
  buffer->initialize(std::move(parent), std::move(own), flag);

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
auto Device::makeDerivedKernel(const KernelParams<kDim, KSet, Args...>& params)
    -> SharedKernel<kDim, KSet, Args...>
{
  using Parser = KernelArgParser<Args...>;
  using KernelT = typename Parser::template DerivedKernelType<Derived, kDim, KSet>;
  using SharedKernelT = SharedKernel<kDim, KSet, Args...>;
  using WeakKernelT = WeakKernel<kDim, KSet, Args...>;

  zisc::pmr::polymorphic_allocator<KernelT> alloc{memoryResource()};
  SharedKernelT kernel = std::allocate_shared<KernelT>(alloc, issueId());

  ZivcObject::SharedPtr parent{getOwn()};
  WeakKernelT own{kernel};
  kernel->initialize(std::move(parent), std::move(own), params);

  return kernel;
}

} // namespace zivc

#endif // ZIVC_DEVICE_INL_HPP
