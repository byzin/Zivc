/*!
  \file math_test.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_TEST_MATH_TEST_HPP
#define ZIVC_TEST_MATH_TEST_HPP

// Standard C++ library
#include <concepts>
#include <fstream>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/binary_serializer.hpp"
// Zivc
#include "zivc/zivc.hpp"

namespace ztest {

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] file_path No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadXList(const std::string_view file_path) noexcept
{
  std::ifstream reference_file{file_path.data(), std::ios_base::binary};

  zivc::int32b n = 0;
  zisc::BSerializer::read(&n, &reference_file);

  std::vector<Float> x_list{};
  x_list.resize(n);
  zisc::BSerializer::read(x_list.data(), &reference_file, sizeof(Float) * n);

  return x_list;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \param [in] file_path No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadExpectedList(const std::string_view file_path, const std::size_t n) noexcept
{
  std::ifstream reference_file{file_path.data(), std::ios_base::binary};

  std::vector<Float> expected_list{};
  expected_list.resize(n);
  zisc::BSerializer::read(expected_list.data(), &reference_file, sizeof(Float) * n);

  return expected_list;
}

/*!
  \details No detailed description

  \tparam Float No description.
  \return No description
  */
template <std::floating_point Float> inline
std::vector<Float> loadPositiveXList() noexcept
{
  auto file_path = (sizeof(Float) == 4)
      ? std::string_view{"resources/math_xpositivef_reference.bin"}
      : std::string_view{"resources/math_xpositived_reference.bin"};
  return loadXList<Float>(file_path);
}

} /* namespace ztest */

#endif /* ZIVC_TEST_MATH_TEST_HPP */
