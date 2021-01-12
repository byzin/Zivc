/*!
  \file kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_KERNEL_INL_HPP
#define ZIVC_KERNEL_INL_HPP

#include "kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>
// Zivc
#include "kernel_set.hpp"
#include "utility/id_data.hpp"
#include "utility/zivc_object.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::LaunchOptions() noexcept
{
  initialize();
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::LaunchOptions(const std::array<uint32b, kDim>& work_size) noexcept :
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
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::LaunchOptions(const std::array<uint32b, kDim>& work_size,
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
auto Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::cpuCommandStorage() const noexcept -> CommandStorage*
{
  const CommandStorage* mem = std::addressof(cpu_command_storage_);
  return const_cast<CommandStorage*>(mem);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::cpuAtomicStorage() const noexcept -> AtomicStorage*
{
  const AtomicStorage* mem = std::addressof(cpu_atomic_storage_);
  return const_cast<AtomicStorage*>(mem);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
bool Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::isExternalSyncMode() const noexcept
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
std::string_view Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::label() const noexcept
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
const std::array<float, 4>& Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::labelColor() const noexcept
{
  return label_color_;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::numOfArgs() noexcept
{
  return kNumOfArgs;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
uint32b Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] is_active No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setExternalSyncMode(const bool is_active) noexcept
{
  is_external_sync_mode_ = is_active ? zisc::kTrue : zisc::kFalse;
}

/*!
  \details No detailed description

  \param [in] launch_label No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setLabel(const std::string_view launch_label) noexcept
{
  std::strncpy(label_.data(), launch_label.data(), launch_label.size() + 1);
}

/*!
  \details No detailed description

  \param [in] label_color No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setLabelColor(const std::array<float, 4>& label_color) noexcept
{
  label_color_ = label_color;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setQueueIndex(const uint32b queue_index) noexcept
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
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setWorkSize(const uint32b work_size, const std::size_t dim) noexcept
{
  work_size_[dim] = work_size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::setWorkSize(const std::array<uint32b, kDim>& work_size) noexcept
{
  work_size_ = work_size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
const std::array<uint32b, kDim>& Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::workSize() const noexcept
{
  return work_size_;
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
LaunchOptions::initialize() noexcept
{
  setLabel(KSet::name());
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::Kernel(IdData&& id) noexcept :
    ZivcObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::~Kernel() noexcept
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::destroy() noexcept
{
  destroyData();
  destroyObject();
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
dimension() noexcept
{
  return kDim;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] params No description.
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
void Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
initialize(ZivcObject::SharedPtr&& parent,
           WeakPtr&& own,
           const Params& params)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initData(params);

  ZivcObject::setNameIfEmpty("Kernel");
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
auto Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
makeOptions() const noexcept -> LaunchOptions
{
  return LaunchOptions{};
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
constexpr std::size_t Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
numOfArgs() noexcept
{
  const std::size_t size = sizeof...(Args);
  return size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \return No description
  */
template <std::size_t kDim, DerivedKSet KSet, typename ...FuncArgs, typename ...Args>
inline
std::array<uint32b, 3> Kernel<KernelParams<kDim, KSet, FuncArgs...>, Args...>::
expandWorkSize(const std::array<uint32b, kDim>& work_size) noexcept
{
  std::array<uint32b, 3> work_size_3d{{1, 1, 1}};
  for (std::size_t i = 0; i < kDim; ++i)
    work_size_3d[i] = work_size[i];
  return work_size_3d;
}

} // namespace zivc

#endif // ZIVC_KERNEL_INL_HPP
