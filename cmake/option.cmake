# file: option.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2023 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Validate options
function(validateOptions)
endfunction(validateOptions)


# Set command options
function(initProjectOptions)
  include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../source/zivc/cmake/general.cmake")

  set(option_description "Build examples.")
  Zivc_setBooleanOption(ZIVC_BUILD_EXAMPLES ON ${option_description})

  set(option_description "Build unit tests.")
  Zivc_setBooleanOption(ZIVC_BUILD_TESTS OFF ${option_description})

  set(option_description "Suppress excessive warnings.")
  Zivc_setBooleanOption(ZIVC_SUPPRESS_EXCESSIVE_WARNING ON ${option_description})

  set(option_description "Enable Vulkan backend.")
  Zivc_setBooleanOption(Z_ENABLE_VULKAN_BACKEND ON ${option_description})

  validateOptions()
endfunction(initProjectOptions)
