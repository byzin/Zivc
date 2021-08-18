/*!
  \file vulkan_buffer_impl-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_VULKAN_BUFFER_IMPL_INL_HPP
#define ZIVC_VULKAN_BUFFER_IMPL_INL_HPP

#include "vulkan_buffer_impl.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/utility.hpp"
// Zivc
#include "zivc/buffer.hpp"
#include "zivc/kernel_common.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/buffer_launch_options.hpp"
#include "zivc/utility/launch_result.hpp"
#include "zivc/utility/launch_options.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] fill_kernel No description.
  \param [in] value No description.
  \param [out] buffer No description.
  \param [in] launch_options No description.
  \return No description
  */
template <KernelArg T, KernelArg D> inline
LaunchResult VulkanBufferImpl::fill(KernelCommon* fill_kernel,
                                    typename Buffer<D>::ConstReference value,
                                    Buffer<T>* buffer,
                                    const BufferLaunchOptions<D>& launch_options) const noexcept
{
  auto dest_buffer = buffer->template reinterp<uint8b>();
  const uint8b* data = zisc::reinterp<const uint8b*>(std::addressof(value));
  constexpr std::size_t data_size = sizeof(value);
  const std::size_t offset = launch_options.destOffset();
  const std::size_t size = launch_options.size();
  LaunchResult result = fill(fill_kernel, data, data_size, &dest_buffer, 
                             launch_options, offset, size);
  return result;
}

} // namespace zivc

#endif // ZIVC_VULKAN_BUFFER_IMPL_INL_HPP
