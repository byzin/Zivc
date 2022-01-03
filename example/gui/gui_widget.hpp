/*!
  \file gui_widget.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_WIDGET_HPP
#define EXAMPLE_GUI_WIDGET_HPP

// Gui
#include "gui_object.hpp"

namespace example {

/*!
  \brief No brief description
  */
class GuiWidget : public GuiObject
{
 public:
  //! Create a gui widget
  GuiWidget() noexcept;

  //! Destroy the widget
  ~GuiWidget() noexcept override;


  //! Draw a widget
  virtual void draw() noexcept = 0;
};

} // namespace example

#endif // EXAMPLE_GUI_WIDGET_HPP
