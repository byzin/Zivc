/*!
  \file gui_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstdlib>
#include <iostream>
#include <stdexcept>
// Zisc
#include "zisc/memory/simple_memory_resource.hpp"
// GUI
#include "gui_application.hpp"
#include "gui_application_options.hpp"

namespace {

int execGuiApp(example::GuiApplicationOptions& options)
{
  example::SharedGuiApp app;
  int result = EXIT_FAILURE;

  // Create an app
  try {
    app = example::makeGuiApp(options);
  }
  catch (const std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
  }
  // Execute the app
  if (app) {
    result = app->run();
    app->destroy();
  }

  return result;
}

} // namespace

/*!
  \details No detailed description

  \param [in] argc No description.
  \param [in] argv No description.
  \return No description
  */
int main(int argc, char** argv)
{
  zisc::SimpleMemoryResource mem_resource;
  example::GuiApplicationOptions options{&mem_resource};
  options.setWindowTitle("Zivc gui example");
  return ::execGuiApp(options);
}
