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
template <typename...> class KernelArgCache;

#if defined(Z_GCC) || defined(Z_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif // Z_GCC || Z_CLANG

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

  //! Represent the cache position in the given args
  static constexpr std::size_t kPosition = sizeof...(Types);

 public:
  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == kPosition,
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

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

 private:
  [[no_unique_address]] PrecedenceCacheT precedence_;
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
  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == 0, PlainType, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

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
  using PrecedenceCacheT = KernelArgCache<Types...>;
  using BufferP = std::add_pointer_t<Buffer<Type>>;

  //! Represent the cache position in the given args
  static constexpr std::size_t kPosition = sizeof...(Types);

 public:
  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == kPosition,
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

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

 private:
  [[no_unique_address]] PrecedenceCacheT precedence_;
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
  //! Return the type by the given index
  template <std::size_t kIndex>
  using CacheType = std::conditional_t<kIndex == 0, BufferP, void>;


  //! Return the value by the given index
  template <std::size_t kIndex>
  CacheType<kIndex>& get() noexcept;

  //! Return the value by the given index
  template <std::size_t kIndex>
  const CacheType<kIndex>& get() const noexcept;

  //! Check if two POD data are equal in values
  bool isEqual(const KernelArgCache& other) const noexcept;

  //! Set the given value by the given index
  template <std::size_t kIndex>
  void set(CacheType<kIndex> value) noexcept;

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
  //! Return the type by the given index
  template <std::size_t>
  using CacheType = void;


//  //! Check if two POD data are equal in values
//  bool isEqual(const KernelArgCache& other) const noexcept;
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
template <typename Type, typename ...Types>
auto concatArgCache(const KernelArgCache<Types...>& cache) noexcept;

} // namespace zivc

#include "kernel_arg_cache-inl.hpp"

#endif // ZIVC_KERNEL_ARG_CACHE_HPP
