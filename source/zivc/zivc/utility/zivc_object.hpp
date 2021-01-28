/*!
  \file zivc_object.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2021 Sho Ikeda
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
#include "zisc/memory/std_memory_resource.hpp"
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

  //! Return the parent pointer
  virtual ZivcObject* getParent() noexcept;

  //! Return the parent pointer
  virtual const ZivcObject* getParent() const noexcept;

  //! Return the own pointer
  virtual ZivcObject* getOwn() noexcept;

  //! Return the own pointer
  virtual const ZivcObject* getOwn() const noexcept;

  //! Check if the object has shared parent pointer
  virtual bool hasParent() const noexcept;

  //! Return the underlying ID data
  IdData& id() noexcept;

  //! Return the underlying ID data
  const IdData& id() const noexcept;

  //! Initialize the object
  void initObject(SharedPtr&& parent, WeakPtr&& own) noexcept;

  //! Check if the zivc object is in debug mode
  virtual bool isDebugMode() const noexcept;

  //! Issue an ID of a child object
  [[nodiscard]]
  virtual IdData issueId() noexcept;

  //! Return the underlying memory resource
  virtual zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  virtual const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Set the object name
  void setName(const std::string_view object_name);

  //! Set the object name
  void setNameIfEmpty(const std::string_view object_name);

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept;

 protected:
  //! Return the own weak pointer
  const WeakPtr& getOwnPtr() const noexcept;

  //! Check if the object has own weak pointer
  bool hasOwnPtr() const noexcept;

  //! Update debug info
  void updateDebugInfo();

  //! Update debug info
  virtual void updateDebugInfoImpl() = 0;

 private:
  SharedPtr parent_;
  WeakPtr own_;
  IdData id_;
};

} // namespace zivc

#include "zivc_object-inl.hpp"

#endif // ZIVC_ZIVC_OBJECT_HPP
