// Copyright 2012 Yandex

#ifndef LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
#define LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_

#include <string>

#include "ltr_client/configuration.h"
#include "ltr_client/factory.h"
#include "ltr_client/utility/object_info.h"

using std::string;

/**
 * Manages object creation and checks for circular dependencies
 */
class DependencyResolvingFactory {
 public:
  DependencyResolvingFactory() {}
  /**
   * Checks for circular dependencies. Throws exception if found
   */
  void checkCircularDependencyAbsence() const;
  /**
   * Creates an object
   * @param name - string with the object name
   */
  Any Create(const string& name) const;
  /**
   * Reads object names stored in ParametersContainer, creates these
   * objects and returns a ParametersContainer with created objects
   * @param parameters - ParametersContainer with object names
   */
  ParametersContainer Create(const ParametersContainer& parameters) const;
  /**
   * Creates an object, declared in config
   * @param object_name - string with the object name
   */
  template <typename RequestedType>
  RequestedType CreateObject(const string& object_name) const;
  /**
   * Initializes factory by given configuration.
   *
   * \param configuration - initializing configuration
   */
  void init(const Configuration::Ptr configuration);

 protected:
  /**
   * Generates a name for an object
   * @param object_info - all the object's properties
   */
  static string getObjectAlias(const ObjectInfo::Ptr object_info);

 private:
  bool tryBuildingObjectCreationChain(
      const ObjectInfo::Ptr object_info,
      ObjectInfosList* info_queue,
      ObjectInfosList* circularity_check_queue) const;

  Configuration::Ptr configuration_;
};


template <typename RequestedType>
RequestedType DependencyResolvingFactory::CreateObject(
          const string& object_name) const {
  const ObjectInfo::Ptr object_info =
      configuration_->object_infos.safeFind(object_name)->second;
  ParametersContainer parameters = Create(object_info->parameters);
  Any object = Factory::instance()->Create(getObjectAlias(object_info),
                                           parameters);
  return any_cast<RequestedType>(object);
}

#endif  // LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
