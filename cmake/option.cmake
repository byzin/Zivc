# file: option.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2021 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Validate options
function(validateOptions)
endfunction(validateOptions)


# Set command options
function(initProjectOptions)
  set(option_description "Build unit tests.")
  Zisc_setBooleanOption(ZIVC_BUILD_TESTS OFF ${option_description})

  set(option_description "Suppress excessive warnings.")
  Zisc_setBooleanOption(ZIVC_SUPPRESS_EXCESSIVE_WARNING ON ${option_description})

  validateOptions()
endfunction(initProjectOptions)
