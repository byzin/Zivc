/*!
  \file cpu_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_device.hpp"
// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <limits>
#include <memory>
// Zisc
#include "zisc/utility.hpp"
#include "zisc/memory/memory.hpp"
#include "zisc/memory/std_memory_resource.hpp"
#include "zisc/thread/future.hpp"
#include "zisc/thread/thread_manager.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zivc/device.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/cppcl/utility.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/fence.hpp"

namespace {

using CpuFence = zisc::Future<void>;

}

namespace zivc {

static_assert(std::alignment_of_v<Fence::Data> %
              std::alignment_of_v<::CpuFence> == 0);
static_assert(sizeof(::CpuFence) <= sizeof(Fence::Data));

/*!
  \details No detailed description

  \param [in] id No description.
  */
CpuDevice::CpuDevice(IdData&& id) : Device(std::move(id))
{
}

/*!
  \details No detailed description
  */
CpuDevice::~CpuDevice() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
zisc::Memory::Usage& CpuDevice::memoryUsage([[maybe_unused]] const std::size_t heap_index) noexcept
{
  return heap_usage_;
}

/*!
  \details No detailed description

  \param [in] heap_index No description.
  \return No description
  */
const zisc::Memory::Usage& CpuDevice::memoryUsage([[maybe_unused]] const std::size_t heap_index) const noexcept
{
  return heap_usage_;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDevice::numOfFences() const noexcept
{
  constexpr std::size_t s = (std::numeric_limits<std::size_t>::max)();
  return s;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDevice::numOfQueues() const noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in,out] fence No description.
  */
void CpuDevice::returnFence(Fence* fence) noexcept
{
  auto* f = zisc::reinterp<::CpuFence*>(std::addressof(fence->data()));
  std::destroy_at(f);
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void CpuDevice::setFenceSize([[maybe_unused]] const std::size_t s)
{
}

/*!
  \details No detailed description

  \param [in] command No description.
  \param [in] dimension No description.
  \param [in] work_size No description.
  \param [in] global_id_offset No description.
  \param [in] id No description.
  \param [out] fence No description.
  */
void CpuDevice::submit(const Command& command,
                       const uint32b dimension,
                       const std::array<uint32b, 3>& work_size,
                       const std::array<uint32b, 3>& global_id_offset,
                       std::atomic<uint32b>* id,
                       Fence* fence)
{
  const auto batch_size = zisc::cast<uint32b>(taskBatchSize());
  auto task = [command, id, dimension, work_size, global_id_offset, batch_size]
  (const int64b, const int64b) noexcept
  {
    cl::inner::WorkItem::setDimension(dimension);
    cl::inner::WorkItem::setGlobalIdOffset(global_id_offset);
    cl::inner::WorkItem::setNumOfGroups(work_size);
    const uint32b num_of_works = work_size[0] * work_size[1] * work_size[2];
    const uint32b n = (num_of_works + (batch_size - 1)) / batch_size;
    for (uint32b block_id = issue(id); block_id < n; block_id = issue(id))
      execBatchCommand(command, block_id, num_of_works, batch_size);
  };

  auto& manager = threadManager();
  constexpr int64b start = 0;
  const int64b end = manager.numOfThreads();
  constexpr auto parent_id = zisc::ThreadManager::kAllPrecedences;
  auto result = manager.enqueueLoop(std::move(task), start, end, parent_id);
  if (fence->isActive()) {
    auto* fen = zisc::reinterp<CpuFence*>(std::addressof(fence->data()));
    *fen = std::move(result);
  }
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void CpuDevice::takeFence(Fence* fence)
{
  auto* memory = std::addressof(fence->data());
  [[maybe_unused]] auto* f = ::new (zisc::cast<void*>(memory)) ::CpuFence{};
}

/*!
  \details No detailed description
  */
void CpuDevice::waitForCompletion() const
{
  auto& manager = const_cast<CpuDevice*>(this)->threadManager();
  manager.waitForCompletion();
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
void CpuDevice::waitForCompletion([[maybe_unused]] const uint32b queue_index) const
{
  waitForCompletion();
}

/*!
  \details No detailed description

  \param [in] fence No description.
  */
void CpuDevice::waitForCompletion(const Fence& fence) const
{
  if (fence) {
    const auto* memory = std::addressof(fence.data());
    const auto& f = *zisc::reinterp<const ::CpuFence*>(memory);
    f.wait();
  }
}

/*!
  \details No detailed description
  */
void CpuDevice::destroyData() noexcept
{
  thread_manager_.reset();
}

/*!
  \details No detailed description
  */
void CpuDevice::initData()
{
  auto& platform = parentImpl();

  heap_usage_.setPeak(0);
  heap_usage_.setTotal(0);
  auto* mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<zisc::ThreadManager> alloc{mem_resource};
  thread_manager_ = zisc::pmr::allocateUnique(alloc,
                                              platform.numOfThreads(),
                                              mem_resource);
}

/*!
  \details No detailed description
  */
void CpuDevice::updateDebugInfoImpl()
{
}

/*!
  \details No detailed description

  \param [in] command No description.
  \param [in] block_id No description.
  \param [in] group_id_max No description.
  \param [in] batch_size No description.
  */
inline
void CpuDevice::execBatchCommand(const Command& command,
                                 const uint32b block_id,
                                 const uint32b group_id_max,
                                 const uint32b batch_size) noexcept
{
  for (uint32b i = 0; i < batch_size; ++i) {
    const uint32b group_id = block_id * batch_size + i;
    if (group_id < group_id_max) {
      cl::inner::WorkItem::setWorkGroupId(group_id);
      command();
    }
  }
}

/*!
  \details No detailed description

  \param [in,out] counter No description.
  \return No description
  */
inline
uint32b CpuDevice::issue(std::atomic<uint32b>* counter) noexcept
{
  const uint32b id = counter->fetch_add(1, std::memory_order::acq_rel);
  return id;
}

} // namespace zivc
