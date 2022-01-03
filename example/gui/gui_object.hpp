/*!
  \file gui_object.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2022 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef EXAMPLE_GUI_OBJECT_HPP
#define EXAMPLE_GUI_OBJECT_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/memory/std_memory_resource.hpp"

namespace example {

/*!
  \brief No brief description
  */
class GuiObject : private zisc::NonCopyable<GuiObject>
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<GuiObject>;
  using WeakPtr = std::weak_ptr<GuiObject>;


  //! Create a gui object
  GuiObject() noexcept;

  //! Destroy the gui object
  virtual ~GuiObject() noexcept;


  //! Destroy the gui object
  void destroy() noexcept;

  //! Return the parent pointer
  virtual GuiObject* getParent() noexcept;

  //! Return the parent pointer
  virtual const GuiObject* getParent() const noexcept;

  //! Return the own pointer
  virtual GuiObject* getOwn() noexcept;

  //! Return the own pointer
  virtual const GuiObject* getOwn() const noexcept;

  //! Check if the object has parent
  virtual bool hasParent() const noexcept;

  //! Return the underlying memory resource
  virtual zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  virtual const zisc::pmr::memory_resource* memoryResource() const noexcept;

 protected:
  //! Destroy the gui object
  virtual void destroyData() noexcept;

  //! Return the own weak pointer
  const WeakPtr& getOwnPtr() const noexcept;

  //! Check if the object has own weak pointer
  bool hasOwnPtr() const noexcept;

  //! Initialize the object
  void initObject(SharedPtr&& parent, WeakPtr&& own) noexcept;

 private:
  SharedPtr parent_;
  WeakPtr own_;
};

// Type aliases
using SharedObject = GuiObject::SharedPtr;
using WeakObject = GuiObject::WeakPtr;

} // namespace example

#endif // EXAMPLE_GUI_OBJECT_HPP
