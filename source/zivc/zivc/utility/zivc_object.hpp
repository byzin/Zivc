/*!
  \file zivc_object.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZIVC_ZIVC_OBJECT_HPP
#define ZIVC_ZIVC_OBJECT_HPP

// Standard C++ library
#include <memory>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zivc
#include "id_data.hpp"
#include "zivc/zivc_config.hpp"

namespace zivc {

/*!
  \brief No brief description

  No detailed description.
  */
class ZivcObject : private zisc::NonCopyable<ZivcObject>
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<ZivcObject>;
  using WeakPtr = std::weak_ptr<ZivcObject>;


  //! Initialize the object
  ZivcObject(IdData&& id) noexcept;

  //! Destroy the object
  virtual ~ZivcObject() noexcept;


  //! Destroy the object
  void destroyObject() noexcept;

  //! Check if the object has own weak pointer
  bool hasOwn() const noexcept;

  //! Check if the object has shared parent pointer
  bool hasParent() const noexcept;

  //! Return the underlying ID data
  IdData& id() noexcept;

  //! Return the underlying ID data
  const IdData& id() const noexcept;

  //! Initialize the object
  void initObject(SharedPtr&& parent, WeakPtr&& own) noexcept;

  //! Check if the zivc object is in debug mode
  virtual bool isDebugMode() const noexcept;

  //! Issue an ID of a child object
  virtual IdData issueId() noexcept;

  //! Return the underlying memory resource
  virtual zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  virtual const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Set the object name
  void setName(const std::string_view object_name) noexcept;

  //! Set the object name
  void setNameIfEmpty(const std::string_view object_name) noexcept;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept;

 protected:
  //! Return the own weak pointer
  const WeakPtr& getOwn() const noexcept;

  //! Return the parent pointer
  ZivcObject* getParent() noexcept;

  //! Return the parent pointer
  const ZivcObject* getParent() const noexcept;

  //! Update debug info
  void updateDebugInfo() noexcept;

  //! Update debug info
  virtual void updateDebugInfoImpl() noexcept = 0;

 private:
  SharedPtr parent_;
  WeakPtr own_;
  IdData id_;
};

} // namespace zivc

#include "zivc_object-inl.hpp"

#endif // ZIVC_ZIVC_OBJECT_HPP
