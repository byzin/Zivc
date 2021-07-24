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
#include <ostream>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/concepts.hpp"
#include "zisc/utility.hpp"
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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <typename ...ArgTypes, typename ...Types> inline
void KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);

  VoidCache::printValue<CacheT>(indent, "ArgCache", output);
  (*output) << " {" << std::endl;

  CacheT::printTree(indent + 1, output);

  VoidCache::printIndent(indent, output);
  (*output) << "}" << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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
  const bool has_tail_padding = (std::alignment_of_v<CacheT> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(CacheT))
      : 0;
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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <typename ...ArgTypes> inline
void KernelArgCache<KernelArgCache<ArgTypes...>>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  VoidCache::printValue<CacheT>(indent, "ArgCache", output);
  (*output) << " {" << std::endl;

  CacheT::printTree(indent + 1, output);

  VoidCache::printIndent(indent, output);
  (*output) << "}" << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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
  return 0;
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
  const bool result1 = zisc::equal(value_, other.value_);
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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <KernelArg Type, typename ...Types> inline
void KernelArgCache<Type, Types...>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);
  VoidCache::printValue<PlainCache>(indent, "value", output);
  (*output) << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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
  const bool has_tail_padding = (PlainCache::kAlignment <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(PlainCache))
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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <KernelArg Type> inline
void KernelArgCache<Type>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  VoidCache::printValue<KernelArgCache>(indent, "value", output);
  (*output) << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <KernelArg Type, typename ...Types> inline
void KernelArgCache<Buffer<Type>&, Types...>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);
  VoidCache::printValue<BufferP>(indent, "buffer", output);
  (*output) << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <KernelArg Type> inline
void KernelArgCache<Buffer<Type>&>::printTree(
    const std::size_t indent,
    std::ostream* output) noexcept
{
  using VoidCache = KernelArgCache<void>;
  VoidCache::printValue<BufferP>(indent, "buffer", output);
  (*output) << std::endl;
}
#endif // ZIVC_PRINT_CACHE_TREE

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

#if defined(ZIVC_PRINT_CACHE_TREE)
/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
inline
void KernelArgCache<void>::printIndent(const std::size_t indent,
                                       std::ostream* output) noexcept
{
  constexpr char indent_space[] = "  ";
  for (std::size_t i = 0; i < indent; ++i)
    (*output) << indent_space;
}

/*!
  \details No detailed description

  \param [in] indent No description.
  \param [out] output No description.
  */
template <typename Type> inline
void KernelArgCache<void>::printValue(const std::size_t indent,
                                      const std::string_view name,
                                      std::ostream* output) noexcept
{
  constexpr std::size_t size = sizeof(Type);
  constexpr std::size_t alignment = std::alignment_of_v<Type>;
  printIndent(indent, output);
  (*output) << name << " (size=" << size << ", alignment=" << alignment << ")";
}
#endif // ZIVC_PRINT_CACHE_TREE

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

  \tparam Type No description.
  \tparam ArgTypes No description.
  \tparam Types No description.
  \param [in] cache No description.
  \return No description
  */
template <typename Type, typename ...ArgTypes, typename ...Types> inline
auto concatArgCache([[maybe_unused]] const KernelArgCache<KernelArgCache<ArgTypes...>, Types...>& cache) noexcept
{
  using CacheT = KernelArgCache<ArgTypes...>;
  static_assert(CacheT::isValid());
  static_assert(1 < CacheT::size());
  using PrecedenceCacheT = KernelArgCache<CacheT, Types...>;
  static_assert(PrecedenceCacheT::isValid());
  static_assert(0 < PrecedenceCacheT::size());

  using TypeCacheT = KernelArgCache<Type>;
  constexpr std::size_t t_size = sizeof(TypeCacheT);
  constexpr std::size_t t_alignment = std::alignment_of_v<TypeCacheT>;

  constexpr std::size_t cache_padding_size = CacheT::tailPaddingSize();
  constexpr bool is_cache_padding_resuable = (t_size <= cache_padding_size) &&
                                             (t_alignment <= cache_padding_size);
  if constexpr (is_cache_padding_resuable) {
    using NewCacheT = decltype(concatArgCache<Type>(CacheT{}));
    using NewArgCacheT = KernelArgCache<NewCacheT, Types...>;
    return NewArgCacheT{};
  }
  else {
    constexpr std::size_t padding_size = PrecedenceCacheT::tailPaddingSize();
    constexpr bool is_padding_resuable = (t_size <= padding_size) &&
                                         (t_alignment <= padding_size);
    if constexpr (is_padding_resuable) {
      using NewCacheT = KernelArgCache<Type, CacheT>;
      using NewArgCacheT = KernelArgCache<NewCacheT, Types...>;
      return NewArgCacheT{};
    }
    else {
      using NewArgCacheT = KernelArgCache<Type, CacheT, Types...>;
      return NewArgCacheT{};
    }
  }
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
    using TypeCacheT = KernelArgCache<Type1>;
    constexpr std::size_t t_size = sizeof(TypeCacheT);
    constexpr std::size_t t_alignment = std::alignment_of_v<TypeCacheT>;

    constexpr std::size_t padding_size = PrecedenceCacheT::tailPaddingSize();
    constexpr bool is_padding_resuable = (t_size <= padding_size) &&
                                         (t_alignment <= padding_size);
    if constexpr (is_padding_resuable) {
      using NewArgCacheT = KernelArgCache<KernelArgCache<Type1, Type2>, Types...>;
      return NewArgCacheT{};
    }
    else {
      using NewArgCacheT = KernelArgCache<Type1, Type2, Types...>;
      return NewArgCacheT{};
    }
  }
  else {
    using NewArgCacheT = KernelArgCache<Type1>;
    return NewArgCacheT{};
  }
}

} // namespace zivc

#endif // ZIVC_KERNEL_ARG_CACHE_INL_HPP
