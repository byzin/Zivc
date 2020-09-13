/*!
  \file cpu_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_device.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
// Zisc
#include "zisc/memory.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Zivc
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zivc/device.hpp"
#include "zivc/device_info.hpp"
#include "zivc/zivc_config.hpp"
#include "zivc/utility/id_data.hpp"
#include "zivc/utility/fence.hpp"

namespace {

using CpuFence = zisc::ThreadManager::SharedResult<void>;

}

namespace zivc {

static_assert(!(std::alignment_of_v<Fence::Data> % std::alignment_of_v<::CpuFence>));
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

  \return No description
  */
std::size_t CpuDevice::numOfFences() const noexcept
{
  constexpr std::size_t s = std::numeric_limits<std::size_t>::max();
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

  \param [in] number No description.
  \return No description
  */
std::size_t CpuDevice::peakMemoryUsage(const std::size_t number) const noexcept
{
  static_cast<void>(number);
  return heap_usage_.peak();
}

/*!
  \details No detailed description

  \param [in,out] fence No description.
  */
void CpuDevice::returnFence(Fence* fence) noexcept
{
  auto memory = zisc::treatAs<::CpuFence*>(std::addressof(fence->data()));
  std::destroy_at(memory);
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
void CpuDevice::setNumOfFences(const std::size_t s)
{
  static_cast<void>(s);
}

///*!
//  \details No detailed description
//
//  \tparam kDimension No description.
//  \param [in] works No description.
//  \param [in] command No description.
//  */
////template <std::size_t kDimension> inline
////void CpuDevice::submit(const std::array<uint32b, kDimension>& works,
////                       const Command& command) noexcept
////{
////  std::atomic<uint32b> id{0};
////  auto task = [this, &command, &works, &id](const uint, const uint)
////  {
////    const auto group_size = expandTo3dWorkGroupSize(works);
////    const uint32b num_of_works = group_size[0] * group_size[1] * group_size[2];
////    const uint32b n = ((num_of_works % taskBucketSize()) == 0)
////        ? num_of_works / taskBucketSize()
////        : num_of_works / taskBucketSize() + 1;
////    cl::clinner::WorkGroup::setWorkGroupSize(group_size);
////    for (uint32b bucket_id = id++; bucket_id < n; bucket_id = id++) {
////      for (uint32b i = 0; i < taskBucketSize(); ++i) {
////        const uint32b group_id = bucket_id * taskBucketSize() + i;
////        if (group_id < num_of_works) {
////          cl::clinner::WorkGroup::setWorkGroupId(group_id);
////          command();
////        }
////      }
////    }
////  };
////
////  constexpr uint start = 0;
////  const uint end = thread_manager_.numOfThreads();
////  auto result = thread_manager_.enqueueLoop(task, start, end, workResource());
////  result->wait();
////}

/*!
  \details No detailed description

  \param [in] command No description.
  \param [in] work_size No description.
  \param [out] fence No description.
  */
void CpuDevice::submit(const Command& command,
                       const std::array<uint32b, 3>& work_size,
                       Fence* fence) noexcept
{
}

/*!
  \details No detailed description

  \param [out] fence No description.
  */
void CpuDevice::takeFence(Fence* fence)
{
  auto memory = std::addressof(fence->data());
  auto f = ::new (zisc::cast<void*>(memory)) ::CpuFence{};
  static_cast<void>(f);
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t CpuDevice::totalMemoryUsage(const std::size_t number) const noexcept
{
  static_cast<void>(number);
  return heap_usage_.total();
}

/*!
  \details No detailed description
  */
void CpuDevice::waitForCompletion() const noexcept
{
  auto& manager = const_cast<CpuDevice*>(this)->threadManager();
  manager.waitForCompletion();
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
void CpuDevice::waitForCompletion(const uint32b queue_index) const noexcept
{
  static_cast<void>(queue_index);
  waitForCompletion();
}

/*!
  \details No detailed description

  \param [in] fence No description.
  */
void CpuDevice::waitForCompletion(const Fence& fence) const noexcept
{
  const auto memory = std::addressof(fence.data());
  const auto& f = *zisc::treatAs<const ::CpuFence*>(memory);
  if (f)
    f->wait();
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
  auto& device = parentImpl();

  heap_usage_.setPeak(0);
  heap_usage_.setTotal(0);
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<zisc::ThreadManager> alloc{mem_resource};
  thread_manager_ = zisc::pmr::allocateUnique(alloc,
                                              device.numOfThreads(),
                                              mem_resource);
}

/*!
  \details No detailed description
  */
void CpuDevice::updateDebugInfoImpl() noexcept
{
}

} // namespace zivc
