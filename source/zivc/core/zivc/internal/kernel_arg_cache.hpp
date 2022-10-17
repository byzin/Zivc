/*!
  \file kernel_arg_cache.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_CACHE_HPP
#define ZIVC_KERNEL_ARG_CACHE_HPP

// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <ostream>
#include <string_view>
#include <type_traits>
// Zivc
#include "zivc/zivc_config.hpp"
#include "zivc/cpucl/vector.hpp"

// #define ZIVC_PRINT_CACHE_TREE

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

} /* namespace zivc */

namespace zivc::internal {

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif // Z_GCC || Z_CLANG

/*!
  \brief Represent invalid cache type

  No detailed description.

  \tparam Types No description.
  */
template <typename ...Types>
class KernelArgCache
{
 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 0;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
class KernelArgCache<void>
{
 public:
  //! Represent the required alignment to conform the extended alignment rule.
  template <KernelArg T>
  struct AlignmentInfo
  {
    static constexpr std::size_t kValue = std::is_class_v<T>
        ? (std::max)(std::alignment_of_v<T>, static_cast<std::size_t>(16))
        : std::alignment_of_v<T>;
  };

  //! Represent the required alignment to conform the extended alignment rule.
  template <KernelArg T, std::size_t kSize>
  struct AlignmentInfo<cl::Vector<T, kSize>>
  {
    static constexpr std::size_t kValue = std::alignment_of_v<cl::Vector<T, kSize>>;
  };


  //! The number of args the cache has
  static constexpr std::size_t kSize = 0;


  //! Return the type by the given index
  template <std::size_t>
  using CacheT = void;


  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print indent spaces
  static void printIndent(const std::size_t indent, std::ostream* output) noexcept;

  //! Print indent spaces
  template <typename Type>
  static void printValue(const std::size_t indent,
                         const std::string_view name,
                         std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam ArgTypes No description.
  \tparam Types No description.
  */
template <typename ...ArgTypes, typename ...Types>
class KernelArgCache<KernelArgCache<ArgTypes...>, Types...>
{
  // Type aliases
  using ValueCacheT = KernelArgCache<ArgTypes...>;
  using PrecedenceCacheT = KernelArgCache<Types...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = ValueCacheT::kSize + PrecedenceCacheT::kSize;

  //! Check if the cache has value 
  template <std::size_t kIndex>
  static constexpr bool kHasValue = (PrecedenceCacheT::kSize <= kIndex) &&
                                    (kIndex < kSize);


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kHasValue<kIndex>,
      typename ValueCacheT::template CacheT<
          kHasValue<kIndex> ? kIndex - PrecedenceCacheT::kSize : 0>,
      typename PrecedenceCacheT::template CacheT<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  static_assert(0 < ValueCacheT::size());
  static_assert(0 < PrecedenceCacheT::size());

  PrecedenceCacheT precedence_;
  ValueCacheT value_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam ArgTypes No description.
  */
template <typename ...ArgTypes>
class KernelArgCache<KernelArgCache<ArgTypes...>>
{
  // Type aliases
  using ValueCacheT = KernelArgCache<ArgTypes...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = ValueCacheT::kSize;

  //! Check if the cache has value 
  template <std::size_t kIndex>
  static constexpr bool kHasValue = (0 <= kIndex) && (kIndex < kSize);


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kHasValue<kIndex>,
      typename ValueCacheT::template CacheT<kIndex>,
      void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  static_assert(0 < ValueCacheT::size());

  ValueCacheT value_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam Types No description.
  */
template <KernelArg Type, typename ...Types>
class KernelArgCache<Type, Types...>
{
  // Type aliases
  using PlainT = std::remove_cv_t<Type>;
  using PlainCacheT = KernelArgCache<Type>;
  using PrecedenceCacheT = KernelArgCache<Types...>;

 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1 + PrecedenceCacheT::kSize;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kIndex == (kSize - 1),
      PlainT,
      typename PrecedenceCacheT::template CacheT<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  PrecedenceCacheT precedence_;
  alignas(PlainCacheT::kAlignment) PlainT value_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <KernelArg Type>
class KernelArgCache<Type>
{
  // Type aliases
  using PlainT = std::remove_cv_t<Type>;
  using AlignmentInfoT = KernelArgCache<void>::AlignmentInfo<PlainT>;

 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1;

  //! Represent the required alignment to conform the extended alignment rule. \todo Consider vector alignment
  static constexpr std::size_t kAlignment = AlignmentInfoT::kValue;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kIndex == (kSize - 1), PlainT, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  alignas(kAlignment) PlainT value_;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  \tparam Types No description.
  */
template <KernelArg Type, typename ...Types>
class KernelArgCache<Buffer<Type>&, Types...>
{
  // Type aliases
  using BufferPtr = std::add_pointer_t<Buffer<Type>>;
  using PrecedenceCacheT = KernelArgCache<Types...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1 + PrecedenceCacheT::kSize;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kIndex == (kSize - 1),
      BufferPtr,
      typename PrecedenceCacheT::template CacheT<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  PrecedenceCacheT precedence_;
  BufferPtr value_ = nullptr;
};

/*!
  \brief No brief description

  No detailed description.

  \tparam Type No description.
  */
template <KernelArg Type>
class KernelArgCache<Buffer<Type>&>
{
  // Type aliases
  using BufferPtr = std::add_pointer_t<Buffer<Type>>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheT = std::conditional_t<kIndex == (kSize - 1), BufferPtr, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheT<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheT<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

#if defined(ZIVC_PRINT_CACHE_TREE)
  //! Print the cache tree
  static void printTree(const std::size_t indent, std::ostream* output) noexcept;
#endif // ZIVC_PRINT_CACHE_TREE

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheT<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  BufferPtr value_ = nullptr;
};

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

//! Check if two cache are equal in values
template <typename ...Types>
bool operator==(const KernelArgCache<Types...>& lhs,
                const KernelArgCache<Types...>& rhs) noexcept;

//! Concatenate types of caches
template <typename ...InTypes, typename ...PreTypes, typename ...NewTypes>
auto concat(const KernelArgCache<KernelArgCache<InTypes...>, PreTypes...>& lhs,
            const KernelArgCache<NewTypes...>& rhs) noexcept;

//! Concatenate types of caches
template <typename PreType, typename ...PreTypes, typename ...NewTypes>
auto concat(const KernelArgCache<PreType, PreTypes...>& lhs,
            const KernelArgCache<NewTypes...>& rhs) noexcept;

//! Concatenate types of caches
template <typename ...PreTypes, typename ...NewTypes>
auto operator+(const KernelArgCache<PreTypes...>& lhs,
               const KernelArgCache<NewTypes...>& rhs) noexcept;

} // namespace zivc::internal

#include "kernel_arg_cache-inl.hpp"

#endif // ZIVC_KERNEL_ARG_CACHE_HPP
