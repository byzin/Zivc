/*!
  \file test-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_TEST_INL_HPP
#define ZIVC_TEST_TEST_INL_HPP

#include "test.hpp"

// Standard C++ library
#include <algorithm>
#include <initializer_list>
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"

namespace ztest {

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] value No description.
  \param [out] buffer No description.
  */
template <typename Type> inline
void fillDeviceBuffer(const Type& value, zivc::Buffer<Type>* buffer)
{
  auto options = buffer->makeOptions();
  options.setSize(buffer->size());
  options.requestFence(true);
  auto result = zivc::fill(value, buffer, options);
  result.fence().wait();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] device No description.
  \param [in] init_list No description.
  \param [out] buffer No description.
  */
template <typename Type> inline
void setDeviceBuffer(zivc::Device& device,
                     std::initializer_list<Type> init_list,
                     zivc::Buffer<Type>* buffer)
{
  buffer->setSize(init_list.size());
  auto tmp_buffer = device.makeBuffer<Type>(zivc::BufferUsage::kHostOnly);
  tmp_buffer->setSize(init_list.size());
  {
    auto mem = tmp_buffer->mapMemory();
    std::copy_n(init_list.begin(), init_list.size(), mem.begin());
  }
  auto options = tmp_buffer->makeOptions();
  options.setSize(init_list.size());
  options.requestFence(true);
  auto result = zivc::copy(*tmp_buffer, buffer, options);
  result.fence().wait();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] source No description.
  \param [out] dest No description.
  */
template <typename Type> inline
void copyBuffer(const zivc::Buffer<Type>& source, zivc::Buffer<Type>* dest)
{
  dest->setSize(source.size());
  auto options = source.makeOptions();
  options.setSize(source.size());
  options.requestFence(true);
  auto result = zivc::copy(source, dest, options);
  result.fence().wait();
}

} // namespace ztest


#endif /* ZIVC_TEST_TEST_INL_HPP */
