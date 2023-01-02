/*!
  \file kernel_arg_cache-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
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

namespace zivc::internal {

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

  \tparam kIndex No description.
  \return No description
  */
template <typename ...ArgTypes, typename ...Types>
template <std::size_t kIndex> inline
auto KernelArgCache<KernelArgCache<ArgTypes...>, Types...>::get() noexcept
    -> CacheT<kIndex>&
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
    -> const CacheT<kIndex>&
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
  return ValueCacheT::isValid() && PrecedenceCacheT::isValid();
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
  using VoidCacheT = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);

  VoidCacheT::printValue<ValueCacheT>(indent, "ArgCache", output);
  (*output) << " {" << std::endl;

  ValueCacheT::printTree(indent + 1, output);

  VoidCacheT::printIndent(indent, output);
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
    CacheT<kIndex> value) noexcept
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
  const bool has_tail_padding = (std::alignment_of_v<ValueCacheT> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(ValueCacheT))
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
    -> CacheT<kIndex>&
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
    -> const CacheT<kIndex>&
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
  return ValueCacheT::isValid();
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
  using VoidCacheT = KernelArgCache<void>;
  VoidCacheT::printValue<ValueCacheT>(indent, "ArgCache", output);
  (*output) << " {" << std::endl;

  ValueCacheT::printTree(indent + 1, output);

  VoidCacheT::printIndent(indent, output);
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
void KernelArgCache<KernelArgCache<ArgTypes...>>::set(CacheT<kIndex> value) noexcept
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
auto KernelArgCache<Type, Types...>::get() noexcept -> CacheT<kIndex>&
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
auto KernelArgCache<Type, Types...>::get() const noexcept -> const CacheT<kIndex>&
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
  using VoidCacheT = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);
  VoidCacheT::printValue<PlainCacheT>(indent, "value", output);
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
void KernelArgCache<Type, Types...>::set(CacheT<kIndex> value) noexcept
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
  const bool has_tail_padding = (PlainCacheT::kAlignment <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(PlainCacheT))
      : 0;
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Type>::get() noexcept -> CacheT<kIndex>&
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
auto KernelArgCache<Type>::get() const noexcept -> const CacheT<kIndex>&
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
  using VoidCacheT = KernelArgCache<void>;
  VoidCacheT::printValue<KernelArgCache>(indent, "value", output);
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
void KernelArgCache<Type>::set(CacheT<kIndex> value) noexcept
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
auto KernelArgCache<Buffer<Type>&, Types...>::get() noexcept -> CacheT<kIndex>&
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
auto KernelArgCache<Buffer<Type>&, Types...>::get() const noexcept -> const CacheT<kIndex>&
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
  using VoidCacheT = KernelArgCache<void>;
  PrecedenceCacheT::printTree(indent, output);
  VoidCacheT::printValue<BufferPtr>(indent, "buffer", output);
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
void KernelArgCache<Buffer<Type>&, Types...>::set(CacheT<kIndex> value) noexcept
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
  const bool has_tail_padding = (std::alignment_of_v<BufferPtr> <
                                 std::alignment_of_v<PrecedenceCacheT>);
  const std::size_t s = has_tail_padding
      ? sizeof(KernelArgCache) - (sizeof(PrecedenceCacheT) + sizeof(BufferPtr))
      : 0;
  return s;
}

/*!
  \details No detailed description

  \tparam kIndex No description.
  \return No description
  */
template <KernelArg Type> template <std::size_t kIndex> inline
auto KernelArgCache<Buffer<Type>&>::get() noexcept -> CacheT<kIndex>&
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
auto KernelArgCache<Buffer<Type>&>::get() const noexcept -> const CacheT<kIndex>&
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
  using VoidCacheT = KernelArgCache<void>;
  VoidCacheT::printValue<BufferPtr>(indent, "buffer", output);
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
void KernelArgCache<Buffer<Type>&>::set(CacheT<kIndex> value) noexcept
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

  \tparam InTypes No description.
  \tparam PreTypes No description.
  \tparam NewTypes No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename ...InTypes, typename ...PreTypes, typename ...NewTypes> inline
auto concat(
    [[maybe_unused]] const KernelArgCache<KernelArgCache<InTypes...>, PreTypes...>& lhs,
    [[maybe_unused]]const KernelArgCache<NewTypes...>& rhs) noexcept
{
  using InCacheT = KernelArgCache<InTypes...>;
  using PreCacheT = KernelArgCache<InCacheT, PreTypes...>;
  using NewCacheT = KernelArgCache<NewTypes...>;
  static_assert(PreCacheT::isValid() && NewCacheT::isValid());
  if constexpr (PreCacheT::size() == 0) {
    return NewCacheT{};
  }
  else if constexpr (NewCacheT::size() == 0) {
    return PreCacheT{};
  }
  else { // Both caches are valid
    static_assert(2 <= InCacheT::size());
    constexpr std::size_t size = sizeof(NewCacheT);
    constexpr std::size_t alignment = std::alignment_of_v<NewCacheT>;
    constexpr std::size_t pad_size = InCacheT::tailPaddingSize();
    constexpr bool is_pad_reusable = (size <= pad_size) && (alignment <= pad_size);
    if constexpr (is_pad_reusable) {
      using NewInCacheT = decltype(concat(InCacheT{}, rhs));
      static_assert(!std::is_reference_v<NewInCacheT>);
      using NewCacheT = KernelArgCache<NewInCacheT, PreTypes...>;
      return NewCacheT{};
    }
    else {
      constexpr std::size_t pad_size = PreCacheT::tailPaddingSize();
      constexpr bool is_pad_reusable = (size <= pad_size) && (alignment <= pad_size);
      if constexpr (is_pad_reusable) {
        using NewInCacheT = KernelArgCache<NewTypes..., InCacheT>;
        using NewCacheT = KernelArgCache<NewInCacheT, PreTypes...>;
        return NewCacheT{};
      }
      else {
        using NewCacheT = KernelArgCache<NewTypes..., InCacheT, PreTypes...>;
        return NewCacheT{};
      }
    }
  }
}

/*!
  \details No detailed description

  \tparam PreType No description.
  \tparam PreTypes No description.
  \tparam NewTypes No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename PreType, typename ...PreTypes, typename ...NewTypes> inline
auto concat([[maybe_unused]] const KernelArgCache<PreType, PreTypes...>& lhs,
            [[maybe_unused]] const KernelArgCache<NewTypes...>& rhs) noexcept
{
  using PreCacheT = KernelArgCache<PreType, PreTypes...>;
  using NewCacheT = KernelArgCache<NewTypes...>;
  static_assert(PreCacheT::isValid() && NewCacheT::isValid());
  if constexpr (PreCacheT::size() == 0) {
    return NewCacheT{};
  }
  else if constexpr (NewCacheT::size() == 0) {
    return PreCacheT{};
  }
  else { // Both caches are valid
    constexpr std::size_t size = sizeof(NewCacheT);
    constexpr std::size_t alignment = std::alignment_of_v<NewCacheT>;
    constexpr std::size_t pad_size = PreCacheT::tailPaddingSize();
    constexpr bool is_pad_reusable = (size <= pad_size) && (alignment <= pad_size);
    if constexpr (is_pad_reusable) {
      using NewCacheT = KernelArgCache<KernelArgCache<NewTypes..., PreType>, PreTypes...>;
      return NewCacheT{};
    }
    else {
      using NewCacheT = KernelArgCache<NewTypes..., PreType, PreTypes...>;
      return NewCacheT{};
    }
  }
}

/*!
  \details No detailed description

  \tparam PreTypes No description.
  \tparam NewTypes No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename ...PreTypes, typename ...NewTypes> inline
auto operator+(const KernelArgCache<PreTypes...>& lhs,
               const KernelArgCache<NewTypes...>& rhs) noexcept
{
  return concat(lhs, rhs);
}

} // namespace zivc::internal

#endif // ZIVC_KERNEL_ARG_CACHE_INL_HPP
