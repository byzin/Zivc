/*!
  \file cl_undefine.hpp
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

#undef kernel
#undef __kernel
#undef ZIVC_CL_GLOBAL_NAMESPACE

#if defined(Z_CLANG)
#pragma clang diagnostic pop
#endif /* Z_CLANG */
