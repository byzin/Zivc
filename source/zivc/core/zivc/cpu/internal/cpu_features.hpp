/*!
  \file cpu_features.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_CPU_FEATURES_HPP
#define ZIVC_CPU_FEATURES_HPP

namespace zivc::internal {

//! Get cpu features
void getCpuFeatures(char* cpu_name, char* vendor_name) noexcept;

} // namespace zivc::internal

#endif // ZIVC_CPU_FEATURES_HPP
