/*!
  \file pod_data-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_POD_DATA_INL_HPP
#define ZIVC_POD_DATA_INL_HPP

#include "pod_data.hpp"
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"

namespace zivc {

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <typename Type, typename ...Types> inline
bool PodData<Type, Types...>::isEqual(const PodData& other) const noexcept
{
  bool result = value_ == other.value_;
  result = result && ParentData::isEqual(other);
  return result;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam T No description.
  \param [in] value No description.
  */
template <typename Type, typename ...Types>
template <std::size_t kIndex, zisc::TriviallyCopyable T> inline
void PodData<Type, Types...>::set(const T& value) noexcept
{
  if constexpr (kIndex == kPosition)
    value_ = value;
  else
    ParentData::template set<kIndex>(value);
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
inline
bool PodData<void>::isEqual([[maybe_unused]] const PodData& other) const noexcept
{
  return true;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam T No description.
  \param [in] value No description.
  */
template <std::size_t kIndex, zisc::TriviallyCopyable T> inline
void PodData<void>::set([[maybe_unused]] const T& value) noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <zisc::TriviallyCopyable Type> inline
bool PodData<Type>::isEqual(const PodData& other) const noexcept
{
  bool result = value_ == other.value_;
  return result;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \tparam T No description.
  \param [in] value No description.
  */
template <zisc::TriviallyCopyable Type>
template <std::size_t kIndex, zisc::TriviallyCopyable T> inline
void PodData<Type>::set(const T& value) noexcept
{
  static_assert(kIndex == 0, "Missed POD data is passed.");
  value_ = value;
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename ...Types> inline
bool operator==(const PodData<Types...>& lhs, const PodData<Types...>& rhs) noexcept
{
  const bool result = lhs.isEqual(rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename ...Types> inline
bool operator!=(const PodData<Types...>& lhs, const PodData<Types...>& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Types No description.
  \param [in] data No description.
  \return No description
  */
template <typename Type, typename ...Types> inline
auto concatPodData([[maybe_unused]] const PodData<Types...>& data) noexcept
{
  if constexpr (zisc::SameAs<PodData<void>, PodData<Types...>>)
    return PodData<Type>{};
  else
    return PodData<Types..., Type>{};
}

} // namespace zivc

#endif // ZIVC_POD_DATA_INL_HPP
