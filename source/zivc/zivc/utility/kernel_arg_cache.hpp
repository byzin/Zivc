/*!
  \file kernel_arg_cache.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_ARG_CACHE_HPP
#define ZIVC_KERNEL_ARG_CACHE_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

// Forward declaration
template <KernelArg> class Buffer;

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

  \tparam ArgTypes No description.
  \tparam Types No description.
  */
template <typename ...ArgTypes, typename ...Types>
class KernelArgCache<KernelArgCache<ArgTypes...>, Types...>
{
  // Type aliases
  using CacheT = KernelArgCache<ArgTypes...>;
  using PrecedenceCacheT = KernelArgCache<Types...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = CacheT::kSize + PrecedenceCacheT::kSize;

  //! Check if the cache has value 
  template <std::size_t kIndex>
  static constexpr bool kHasValue = (PrecedenceCacheT::kSize <= kIndex) &&
                                    (kIndex < kSize);


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kHasValue<kIndex>,
      typename CacheT::template CacheType<
          kHasValue<kIndex> ? kIndex - PrecedenceCacheT::kSize : 0>,
      typename PrecedenceCacheT::template CacheType<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  static_assert(0 < CacheT::size());
  static_assert(0 < PrecedenceCacheT::size());

  PrecedenceCacheT precedence_;
  CacheT value_;
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
  using CacheT = KernelArgCache<ArgTypes...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = CacheT::kSize;

  //! Check if the cache has value 
  template <std::size_t kIndex>
  static constexpr bool kHasValue = (0 <= kIndex) && (kIndex < kSize);


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kHasValue<kIndex>,
      typename CacheT::template CacheType<kIndex>,
      void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  static_assert(0 < CacheT::size());

  CacheT value_;
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
  using PlainType = std::remove_cv_t<Type>;
  using PrecedenceCacheT = KernelArgCache<Types...>;

 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1 + PrecedenceCacheT::kSize;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == (kSize - 1),
      PlainType,
      typename PrecedenceCacheT::template CacheType<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  PrecedenceCacheT precedence_;
  PlainType value_;
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
  using PlainType = std::remove_cv_t<Type>;

 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == (kSize - 1), PlainType, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  PlainType value_;
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
  using BufferP = std::add_pointer_t<Buffer<Type>>;
  using PrecedenceCacheT = KernelArgCache<Types...>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1 + PrecedenceCacheT::kSize;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == (kSize - 1),
      BufferP,
      typename PrecedenceCacheT::template CacheType<kIndex>>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  PrecedenceCacheT precedence_;
  BufferP value_ = nullptr;
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
  using BufferP = std::add_pointer_t<Buffer<Type>>;


 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 1;


  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == (kSize - 1), BufferP, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

  //! Return the size of tail padding of the cache
  static constexpr std::size_t tailPaddingSize() noexcept;

 private:
  BufferP value_ = nullptr;
};

/*!
  \brief No brief description

  No detailed description.
  */
template <>
class KernelArgCache<void>
{
 public:
  //! The number of args the cache has
  static constexpr std::size_t kSize = 0;


  //! Return the type by the given index
  template <std::size_t>
  using CacheType = void;


  //! Check if the cache type is valid
  static constexpr bool isValid() noexcept;

  //! Return the number of args the cache has
  static constexpr std::size_t size() noexcept;
};

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic pop
#endif // Z_GCC || Z_CLANG

//! Check if two cache are equal in values
template <typename ...Types>
bool operator==(const KernelArgCache<Types...>& lhs,
                const KernelArgCache<Types...>& rhs) noexcept;

//! Check if two cache are equal in values
template <typename ...Types>
bool operator!=(const KernelArgCache<Types...>& lhs,
                const KernelArgCache<Types...>& rhs) noexcept;

//! Concatenate caches
template <typename Type1, typename Type2, typename ...Types>
auto concatArgCache(const KernelArgCache<Type2, Types...>& cache) noexcept;

} // namespace zivc

#include "kernel_arg_cache-inl.hpp"

#endif // ZIVC_KERNEL_ARG_CACHE_HPP
