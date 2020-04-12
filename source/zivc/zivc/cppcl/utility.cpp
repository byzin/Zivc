/*!
  \file utility.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "utility.hpp"
// Standard C++ library
#include <array>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

namespace cl {

namespace inner {

thread_local std::array<zivc::uint32b, 3> WorkGroup::work_group_id_;
thread_local std::array<zivc::uint32b, 3> WorkGroup::work_group_size_;

} // namespace inner

} // namespace cl

} // namespace zivc
