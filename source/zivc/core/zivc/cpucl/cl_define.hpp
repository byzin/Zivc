/*!
  \file cl_define.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#if defined(Z_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif /* Z_CLANG */

/*!
  \def ZIVC_CL_GLOBAL_NAMESPACE
  \brief No brief description

  No detailed description.
  */
#define ZIVC_CL_GLOBAL_NAMESPACE ::zivc::cl

#if defined(__kernel)
static_assert(false, "The macro '__kernel' is already defined.");
#endif /* __kernel */

/*!
  \def __kernel
  \brief A definition for the compatibility with OpenCL

  No detailed description.
  */
#define __kernel inline

#if defined(kernel)
static_assert(false, "The macro 'kernel' is already defined.");
#endif /* kernel */

/*!
  \def kernel
  \brief A definition for the compatibility with OpenCL

  No detailed description.
  */
#define kernel __kernel

#if defined(Z_CLANG)
#pragma clang diagnostic pop
#endif /* Z_CLANG */
