/*!
  \file buffer_init_params-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_BUFFER_INIT_PARAMS_INL_HPP
#define ZIVC_BUFFER_INIT_PARAMS_INL_HPP

#include "buffer_init_params.hpp"
// Zivc
#include "zivc/zivc_config.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \details No detailed description
  */
inline
BufferInitParams::BufferInitParams() noexcept
{
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
BufferInitParams::BufferInitParams(const BufferUsage flag) noexcept :
    flag_{flag}
{
}

/*!
  \details No detailed description

  \return No description
  */
inline
BufferUsage BufferInitParams::bufferUsage() const noexcept
{
  return flag_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto BufferInitParams::descriptorType() const noexcept -> DescriptorType
{
  return descriptor_type_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool BufferInitParams::internalBufferFlag() const noexcept
{
  const bool result = is_internal_buffer_ == zisc::kTrue;
  return result;
}

/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
void BufferInitParams::setBufferUsage(const BufferUsage flag) noexcept
{
  flag_ = flag;
}

/*!
  \details No detailed description

  \param [in] type No description.
  */
inline
void BufferInitParams::setDescriptorType(const DescriptorType type) noexcept
{
  descriptor_type_ = type;
}


/*!
  \details No detailed description

  \param [in] flag No description.
  */
inline
void BufferInitParams::setInternalBufferFlag(const bool flag) noexcept
{
  is_internal_buffer_ = flag ? zisc::kTrue : zisc::kFalse;
}

} // namespace zivc

#endif // ZIVC_BUFFER_INIT_PARAMS_INL_HPP
