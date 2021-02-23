/*!
  \file kernel_arg_cache-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_CACHE_INL_HPP
#define ZIVC_KERNEL_ARG_CACHE_INL_HPP

#include "kernel_arg_cache.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Type, Types...>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == kPosition) {
    return value_;
  }
  else {
    return precedence_.template get<kIndex>();
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Type, Types...>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == kPosition) {
    return value_;
  }
  else {
    return precedence_.template get<kIndex>();
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> inline
bool KernelArgCache<Type, Types...>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result1 = precedence_ == other.precedence_;
  const bool result2 = value_ == other.value_;
  return result1 && result2;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
void KernelArgCache<Type, Types...>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == kPosition) {
    value_ = value;
  }
  else {
    precedence_.template set<kIndex>(value);
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Type>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == 0) {
    return value_;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
    return;
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Type>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == 0) {
    return value_;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
    return;
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type> inline
bool KernelArgCache<Type>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result = value_ == other.value_;
  return result;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type> template <std::size_t kIndex> inline
void KernelArgCache<Type>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == 0) {
    value_ = value;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&, Types...>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == kPosition) {
    return value_;
  }
  else {
    return precedence_.template get<kIndex>();
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&, Types...>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == kPosition) {
    return value_;
  }
  else {
    return precedence_.template get<kIndex>();
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> inline
bool KernelArgCache<Buffer<Type>&, Types...>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result1 = precedence_ == other.precedence_;
  const bool result2 = value_ == other.value_;
  return result1 && result2;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
void KernelArgCache<Buffer<Type>&, Types...>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == kPosition) {
    value_ = value;
  }
  else {
    precedence_.template set<kIndex>(value);
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == 0) {
    return value_;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
    return;
  }
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == 0) {
    return value_;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
    return;
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type> inline
bool KernelArgCache<Buffer<Type>&>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result = value_ == other.value_;
  return result;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type> template <std::size_t kIndex> inline
void KernelArgCache<Buffer<Type>&>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == 0) {
    value_ = value;
  }
  else {
    static_assert(kIndex != 0, "Invalid index was specified.");
  }
}

///*!
//  \details No detailed description
//
//  \param [in] other No description.
//  \return No description
//  */
//template <> inline
//bool KernelArgCache<void>::isEqual([[maybe_unused]] const KernelArgCache& other) const noexcept
//{
//  return true;
//}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename ...Types> inline
bool operator==(const KernelArgCache<Types...>& lhs,
                const KernelArgCache<Types...>& rhs) noexcept
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
bool operator!=(const KernelArgCache<Types...>& lhs,
                const KernelArgCache<Types...>& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Types No description.
  \param [in] cache No description.
  \return No description
  */
template <typename Type, typename ...Types> inline
auto concatArgCache([[maybe_unused]] const KernelArgCache<Types...>& cache) noexcept
{
  if constexpr (zisc::SameAs<KernelArgCache<void>, KernelArgCache<Types...>>)
    return KernelArgCache<Type>{};
  else
    return KernelArgCache<Type, Types...>{};
}

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_CACHE_INL_HPP
