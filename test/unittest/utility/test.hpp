/*!
  \file test.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_TEST_HPP
#define ZIVC_TEST_TEST_HPP

// Standard C++ library
#include <initializer_list>
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"

namespace ztest {

//! Make a platform for unit test
zivc::SharedPlatform makePlatform();

//! Fill a device buffer with the give value
template <typename Type>
void fillDeviceBuffer(const Type& value, zivc::Buffer<Type>* buffer);

//! Initialize a device buffer with the given initializer list
template <typename Type>
void setDeviceBuffer(zivc::Device& device,
                     std::initializer_list<Type> init_list,
                     zivc::Buffer<Type>* buffer);

//! Copy buffer data
template <typename Type>
void copyBuffer(const zivc::Buffer<Type>& source, zivc::Buffer<Type>* dest);

} // namespace ztest

#include "test-inl.hpp"

#endif // ZIVC_TEST_TEST_HPP
