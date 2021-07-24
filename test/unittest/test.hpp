/*!
  \file test.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_TEST_HPP
#define ZIVC_TEST_TEST_HPP

// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"

namespace ztest {

//! Make a platform for unit test
zivc::SharedPlatform makePlatform();

} // namespace ztest

#endif // ZIVC_TEST_TEST_HPP
