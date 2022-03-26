/*!
  \file test.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Zisc
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/zivc.hpp"
#include "zivc/zivc_config.hpp"
// Test
#include "config.hpp"

namespace ztest {

/*!
  \details No detailed description

  \return No description
  */
zivc::SharedContext createContext()
{
  auto& config = Config::globalConfig();
  if (!config.isGlobalContextEnabled())
    config.destroyContext();
  zivc::SharedContext context = config.queryContext();
  return context;
}

} // namespace ztest
