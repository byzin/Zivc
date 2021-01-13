/*!
  \file pod_data.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_POD_DATA_HPP
#define ZIVC_POD_DATA_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/concepts.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam Types No description.
  */
template <typename Type, typename ...Types>
class PodData : public PodData<Types...>
{
  using ParentData = PodData<Types...>;

 public:
  //! Check if two POD data are equal in values
  bool isEqual(const PodData& other) const noexcept;

  //! Set a value by the given position
  template <std::size_t kIndex, zisc::TriviallyCopyable T>
  void set(const T& value) noexcept;

 private:
  static constexpr std::size_t kPosition = sizeof...(Types);


  static_assert(zisc::TriviallyCopyable<Type>);
  Type value_;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
class PodData<void>
{
 public:
  //! Check if two POD data are equal in values
  bool isEqual(const PodData& other) const noexcept;

  //! Dummy function
  template <std::size_t kIndex, zisc::TriviallyCopyable T>
  void set(const T& value) noexcept;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <zisc::TriviallyCopyable Type>
class PodData<Type>
{
 public:
  //! Check if two POD data are equal in values
  bool isEqual(const PodData& other) const noexcept;

  //! Set a value by the given position
  template <std::size_t kIndex, zisc::TriviallyCopyable T>
  void set(const T& value) noexcept;

 private:
  Type value_;
};

//! Check if two POD data are equal in values
template <typename ...Types>
bool operator==(const PodData<Types...>& lhs, const PodData<Types...>& rhs) noexcept;

//! Check if two POD data are equal in values
template <typename ...Types>
bool operator!=(const PodData<Types...>& lhs, const PodData<Types...>& rhs) noexcept;

//! Concatenate POD arguments
template <typename Type, typename ...Types>
auto concatPodData(const PodData<Types...>& data) noexcept;

} // namespace zivc

#include "pod_data-inl.hpp"

#endif // ZIVC_POD_DATA_HPP
