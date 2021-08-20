/*!
  \file fill_data-inl.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_FILL_DATA_INL_CL
#define ZIVC_FILL_DATA_INL_CL

#include "fill_data.cl"
// Internal kernel
#include "fill_info.cl"

namespace zivc {

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <typename Type> inline
Type FillData<Type>::get(const size_t index) const noexcept
{
  const Type data = storage_[index];
  return data;
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] index No description.
  */
template <typename Type> inline
void FillData<Type>::set(const Type data, const size_t index) noexcept
{
  storage_[index] = data;
}

} // namespace zivc

#endif // ZIVC_FILL_DATA_INL_CL
