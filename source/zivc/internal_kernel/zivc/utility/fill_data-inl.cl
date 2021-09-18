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

namespace zivc {

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <typename Type> inline
Type FillData<Type>::operator[](const size_t index) const noexcept
{
  const Type value = get(index);
  return value;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename Type> inline
constexpr size_t FillData<Type>::length() noexcept
{
  return kLength;
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <typename Type> inline
bool FillData<Type>::isEqual(const FillData& other) const noexcept
{
  const bool result = data_.isEqual(other.data_);
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] index No description.
  */
template <typename Type> inline
void FillData<Type>::set(const Type value, const size_t index) noexcept
{
  data_.set(value, index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <typename Type> inline
Type FillData<Type>::get(const size_t index) const noexcept
{
  const Type value = data_.get(index);
  return value;
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
template <typename Type> template <size_t kN> inline
bool FillData<Type>::DataImpl<kN>::isEqual(const DataImpl& other) const noexcept
{
  const bool result = (data_ == other.data_)
      ? precedence_.isEqual(other.precedence_)
      : false;
  return result;
}

/*!
  \details No detailed description

  \param [in] value No description.
  \param [in] index No description.
  */
template <typename Type> template <size_t kN> inline
void FillData<Type>::DataImpl<kN>::set(const Type value, const size_t index) noexcept
{
  constexpr size_t position = FillData<Type>::length() - kN;
  if (position == index)
    data_ = value;
  else
    precedence_.set(value, index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <typename Type> template <size_t kN> inline
Type FillData<Type>::DataImpl<kN>::get(const size_t index) const noexcept
{
  constexpr size_t position = FillData<Type>::length() - kN;
  const Type value = (position == index) ? data_ : precedence_.get(index);
  return value;
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
//template <typename Type> template <> inline
//bool FillData<Type>::DataImpl<0>::isEqual(const DataImpl& other) const noexcept
//{
//  const bool result = data_ == other.data_;
//  return result;
//}
//
///*!
//  \details No detailed description
//
//  \param [in] value No description.
//  \param [in] index No description.
//  */
//template <typename Type> template <> inline
//void FillData<Type>::DataImpl<0>::set(const Type value, const size_t index) noexcept
//{
//  static_cast<void>(index);
//  data_ = value;
//}
//
///*!
//  \details No detailed description
//
//  \param [in] index No description.
//  \return No description
//  */
//template <typename Type> template <> inline
//Type FillData<Type>::DataImpl<0>::get(const size_t index) const noexcept
//{
//  static_cast<void>(index);
//  return data_;
//}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename Type> inline
bool operator==(const FillData<Type>& lhs, const FillData<Type>& rhs) noexcept
{
  const bool result = lhs.isEqual(rhs);
  return result;
}

/*!
  \details No detailed description

  \tparam Type No description.
  \param [in] lhs No description.
  \param [in] rhs No description.
  \return No description
  */
template <typename Type> inline
bool operator!=(const FillData<Type>& lhs, const FillData<Type>& rhs) noexcept
{
  const bool result = !(lhs == rhs);
  return result;
}

} // namespace zivc

#endif // ZIVC_FILL_DATA_INL_CL
