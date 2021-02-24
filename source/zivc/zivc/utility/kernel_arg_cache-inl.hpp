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

  \return No description
  */
template <typename ...Types> inline
constexpr bool KernelArgCache<Types...>::isValid() noexcept
{
  return false;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <typename ...ArgTypes, typename ...Types>
template <std::size_t kIndex> inline
auto KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::get() noexcept
    -> CacheType<kIndex>&
{
  if constexpr (kHasValue<kIndex>)
    return value_.template get<kIndex - PrecedenceCacheT::kSize>();
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <typename ...ArgTypes, typename ...Types>
template <std::size_t kIndex> inline
auto KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::get() const noexcept
    -> const CacheType<kIndex>&
{
  if constexpr (kHasValue<kIndex>)
    return value_.template get<kIndex - PrecedenceCacheT::kSize>();
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <typename ...ArgTypes, typename ...Types> inline
bool KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::isEqual(
    const KernelArgCache& other) const noexcept
{
  const bool result1 = value_ == other.value_;
  const bool result2 = precedence_ == other.precedence_;
  return result1 && result2;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes, typename ...Types> inline
constexpr bool KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::isValid() noexcept
{
  return CacheT::isValid() && PrecedenceCacheT::isValid();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes, typename ...Types> inline
constexpr std::size_t KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <typename ...ArgTypes, typename ...Types>
template <std::size_t kIndex> inline
void KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::set(
    CacheType<kIndex> value) noexcept
{
  if constexpr (kHasValue<kIndex>)
    value_.template set<kIndex - PrecedenceCacheT::kSize>(value);
  else
    precedence_.template set<kIndex>(value);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes, typename ...Types> inline
constexpr std::size_t KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::tailPaddingSize() noexcept
{
  std::size_t s = CacheT::tailPaddingSize();
  const bool has_tail_padding = (std::alignment_of_v<CacheT> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  if (has_tail_padding)
    s += sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(CacheT));
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <typename ...ArgTypes> template <std::size_t kIndex> inline
auto KernelArgCache<KernelArgCache<ArgTypes...>>::get() noexcept
    -> CacheType<kIndex>&
{
  if constexpr (kHasValue<kIndex>)
    return value_.template get<kIndex>();
  else
    static_assert(kHasValue<kIndex>, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <typename ...ArgTypes> template <std::size_t kIndex> inline
auto KernelArgCache<KernelArgCache<ArgTypes...>>::get() const noexcept
    -> const CacheType<kIndex>&
{
  if constexpr (kHasValue<kIndex>)
    return value_.template get<kIndex>();
  else
    static_assert(kHasValue<kIndex>, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <typename ...ArgTypes> inline
bool KernelArgCache<KernelArgCache<ArgTypes...>>::isEqual(
    const KernelArgCache& other) const noexcept
{
  const bool result = value_ == other.value_;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
constexpr bool KernelArgCache<KernelArgCache<ArgTypes...>>::isValid() noexcept
{
  return CacheT::isValid();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
constexpr std::size_t KernelArgCache<KernelArgCache<ArgTypes...>>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <typename ...ArgTypes> template <std::size_t kIndex> inline
void KernelArgCache<KernelArgCache<ArgTypes...>>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kHasValue<kIndex>)
    value_.template set<kIndex>(value);
  else
    static_assert(kHasValue<kIndex>, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
constexpr std::size_t KernelArgCache<KernelArgCache<ArgTypes...>>::tailPaddingSize() noexcept
{
  const std::size_t s = CacheT::tailPaddingSize();
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Type, Types...>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == (size() - 1))
    return value_;
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Type, Types...>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == (size() - 1))
    return value_;
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> inline
bool KernelArgCache<Type, Types...>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result1 = value_ == other.value_;
  const bool result2 = precedence_ == other.precedence_;
  return result1 && result2;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr bool KernelArgCache<Type, Types...>::isValid() noexcept
{
  return PrecedenceCacheT::isValid(); 
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr std::size_t KernelArgCache<Type, Types...>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
void KernelArgCache<Type, Types...>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == (size() - 1))
    value_ = value;
  else
    precedence_.template set<kIndex>(value);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr std::size_t KernelArgCache<Type, Types...>::tailPaddingSize() noexcept
{
  const bool has_tail_padding = (std::alignment_of_v<PlainType> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(PlainType))
      : 0;
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Type>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == 0)
    return value_;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Type>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == 0)
    return value_;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
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

  \return No description
  */
template <KernelArg Type> inline
constexpr bool KernelArgCache<Type>::isValid() noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
constexpr std::size_t KernelArgCache<Type>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type> template <std::size_t kIndex> inline
void KernelArgCache<Type>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == 0)
    value_ = value;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
constexpr std::size_t KernelArgCache<Type>::tailPaddingSize() noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&, Types...>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == (size() - 1))
    return value_;
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&, Types...>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == (size() - 1))
    return value_;
  else
    return precedence_.template get<kIndex>();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <KernelArg Type, typename ...Types> inline
bool KernelArgCache<Buffer<Type>&, Types...>::isEqual(const KernelArgCache& other) const noexcept
{
  const bool result1 = value_ == other.value_;
  const bool result2 = precedence_ == other.precedence_;
  return result1 && result2;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr bool KernelArgCache<Buffer<Type>&, Types...>::isValid() noexcept
{
  return PrecedenceCacheT::isValid();
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr std::size_t KernelArgCache<Buffer<Type>&, Types...>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type, typename ...Types> template <std::size_t kIndex> inline
void KernelArgCache<Buffer<Type>&, Types...>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == (size() - 1))
    value_ = value;
  else
    precedence_.template set<kIndex>(value);
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type, typename ...Types> inline
constexpr std::size_t KernelArgCache<Buffer<Type>&, Types...>::tailPaddingSize() noexcept
{
  const bool has_tail_padding = (std::alignment_of_v<BufferP> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(BufferP))
      : 0;
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&>::get() noexcept -> CacheType<kIndex>&
{
  if constexpr (kIndex == 0)
    return value_;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&>::get() const noexcept -> const CacheType<kIndex>&
{
  if constexpr (kIndex == 0)
    return value_;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
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

  \return No description
  */
template <KernelArg Type> inline
constexpr bool KernelArgCache<Buffer<Type>&>::isValid() noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
constexpr std::size_t KernelArgCache<Buffer<Type>&>::size() noexcept
{
  return kSize;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \param [in] value No description.
  */
template <KernelArg Type> template <std::size_t kIndex> inline
void KernelArgCache<Buffer<Type>&>::set(CacheType<kIndex> value) noexcept
{
  if constexpr (kIndex == 0)
    value_ = value;
  else
    static_assert(kIndex == 0, "Invalid index was specified.");
}

/*!
  \details No detailed description

  \return No description
  */
template <KernelArg Type> inline
constexpr std::size_t KernelArgCache<Buffer<Type>&>::tailPaddingSize() noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr bool KernelArgCache<void>::isValid() noexcept
{
  return true;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t KernelArgCache<void>::size() noexcept
{
  return kSize;
}

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

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam Types No description.
  \param [in] cache No description.
  \return No description
  */
template <typename Type1, typename Type2, typename ...Types> inline
auto concatArgCache([[maybe_unused]] const KernelArgCache<Type2, Types...>& cache) noexcept
{
  using PrecedenceCacheT = KernelArgCache<Type2, Types...>;
  static_assert(PrecedenceCacheT::isValid());

  if constexpr (0 < PrecedenceCacheT::size()) {
    constexpr std::size_t padding_size = PrecedenceCacheT::tailPaddingSize();
    constexpr bool is_padding_resuable = (sizeof(Type1) <= padding_size) &&
                                         (std::alignment_of_v<Type1> <= padding_size);
    if constexpr (is_padding_resuable)
      return KernelArgCache<KernelArgCache<Type1, Type2>, Types...>{};
    else
      return KernelArgCache<Type1, Type2, Types...>{};
  }
  else {
    return KernelArgCache<Type1>{};
  }

}

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_CACHE_INL_HPP
