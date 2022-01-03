/*!
  \file buffer_init_params.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \note No notation.
  \attention No attention.

  \copyright
  No copyright
  */

#ifndef ZIVC_BUFFER_INIT_PARAMS_HPP
#define ZIVC_BUFFER_INIT_PARAMS_HPP

// Zisc
#include "zisc/boolean.hpp"
// Zivc
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class BufferInitParams
{
 public:
  /*!
    \brief No brief description

    No detailed description.
    */
  enum class DescriptorType
  {
    kUniform,
    kStorage
  };


  //! Initialize parameters
  BufferInitParams() noexcept;

  //! Initialize parameters
  BufferInitParams(const BufferUsage flag) noexcept;


  //! Return the buffer usage flag
  BufferUsage bufferUsage() const noexcept;

  //! Return the descriptor type for Vulkan
  DescriptorType descriptorType() const noexcept;

  //! Check if the buffer has internal flag
  bool internalBufferFlag() const noexcept;

  //! Set the buffer usage
  void setBufferUsage(const BufferUsage flag) noexcept;

  //! Set descriptor type for Vulkan
  void setDescriptorType(const DescriptorType type) noexcept;

  //! Set internal buffer flag
  void setInternalBufferFlag(const bool flag) noexcept;

 private:
  //! Initialize
  void initialize() noexcept;


  BufferUsage flag_ = BufferUsage::kDeviceOnly;
  DescriptorType descriptor_type_ = DescriptorType::kStorage;
  zisc::Boolean is_internal_buffer_;
  [[maybe_unused]] Padding<3> pad_;
};

} // namespace zivc

#include "buffer_init_params-inl.hpp"

#endif // ZIVC_BUFFER_INIT_PARAMS_HPP
