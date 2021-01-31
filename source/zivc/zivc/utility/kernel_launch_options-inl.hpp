/*!
  \file kernel_launch_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP
#define ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP

#include "kernel_launch_options.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>
#include <type_traits>
// Zivc
#include "id_data.hpp"
#include "zivc/kernel_set.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions(const std::array<uint32b, kDim>& work_size) noexcept :
    work_size_{work_size}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] queue_index No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
KernelLaunchOptions(const std::array<uint32b, kDim>& work_size,
                    const uint32b queue_index) noexcept :
    work_size_{work_size},
    queue_index_{queue_index}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t
KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
bool KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
isExternalSyncMode() const noexcept
{
  const bool result = is_external_sync_mode_;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::string_view KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
label() const noexcept
{
  std::string_view l{label_.data()};
  return l;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const std::array<float, 4>& KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
labelColor() const noexcept
{
  return label_color_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
numOfArgs() noexcept
{
  const std::size_t s = sizeof...(Args);
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
uint32b KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] is_active No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setExternalSyncMode(const bool is_active) noexcept
{
  is_external_sync_mode_ = is_active ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \param [in] launch_label No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setLabel(const std::string_view launch_label) noexcept
{
  std::strncpy(label_.data(), launch_label.data(), launch_label.size() + 1);
}

/*!
  \details No detailed description

  \param [in] label_color No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setLabelColor(const std::array<float, 4>& label_color) noexcept
{
  label_color_ = label_color;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setQueueIndex(const uint32b queue_index) noexcept
{
  queue_index_ = queue_index;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] dim No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setWorkSize(const uint32b work_size, const std::size_t dim) noexcept
{
  work_size_[dim] = work_size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
setWorkSize(const std::array<uint32b, kDim>& work_size) noexcept
{
  work_size_ = work_size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const std::array<uint32b, kDim>& KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
workSize() const noexcept
{
  return work_size_;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void KernelLaunchOptions<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initialize() noexcept
{
  setLabel(KSet::name());
}

} // namespace zivc

#endif // ZIVC_KERNEL_LAUNCH_OPTIONS_INL_HPP
